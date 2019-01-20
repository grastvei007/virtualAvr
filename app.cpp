#include "app.h"

#include <QDebug>

#include <device/inputdevicemanager.h>
#include <device/device.h>

#include "avr/messagehandler.h"
#include "avr/messagehandler.cpp"
#include "avr/message.h"
#include "avr/message.cpp"
#include "avr/messagetranslationsenter.h"
#include "avr/messagetranslationsenter.cpp"


//avr code to testing.
MessageHandler messageHandler;
MessageTranslationSenter mts;

Device *mDevice;
bool requestDeviceName;
bool requestTags;


void messageHandlerCallback(Message *&msg)
{
    qDebug() << "Translate";
    //USART_putstring("translate");
    mts.translateMessage(msg);
    msg->destroy();
    free(msg);
}


void deviceNameRequest()
{
    requestDeviceName = true;
}


void createTagsRequest()
{
    requestTags = true;
}



App::App(int argc, char *argv[]) : QCoreApplication(argc, argv),
    mLock(false)
{
    mDevice = nullptr;
    requestDeviceName = false;
    requestTags = false;
    mTimer = new QTimer(this);
    mTimer->setInterval(100);
    connect(mTimer, &QTimer::timeout, this, &App::onTimer);
    mTimer->start();

    connect(&InputDeviceManager::sGetInstance(), &InputDeviceManager::inputDeviceAvailable, this, &App::onDeviceAvailable);
    connect(&InputDeviceManager::sGetInstance(), &InputDeviceManager::inputDeviceConnected, this, &App::onDeviceConnected);
    InputDeviceManager::sGetInstance().setUseDefaultSerialSettingFlag(true);
    InputDeviceManager::sGetInstance().setDetectInputDevicesInterval(1000);

    messageHandler.init();
    messageHandler.setCallback(messageHandlerCallback);

    mts.init();
    mts.setDeviceNameFunc(deviceNameRequest);
    mts.setCreateTagsFunc(createTagsRequest);

}


void App::onDeviceAvailable(QString aDevice)
{
    qDebug() << aDevice;
    if( aDevice == "tnt0")
    {
        InputDeviceManager::sGetInstance().connectInputDevice(aDevice);
    }
}


void App::onDeviceConnected(QString aDevice)
{
    qDebug() << __FUNCTION__;
    // get the connected device.
    mDevice = InputDeviceManager::sGetInstance().getInputDevice(aDevice);
    if(mDevice)
    {
        connect(mDevice, &Device::dataRecieved, this, &App::onDataRecieved);
        mDevice->setOverideDataRead(true);
    }
}


void App::onDataRecieved(QByteArray aData)
{
    mBuffer.append(aData);
    while(mBuffer.count() > 0)
    {
        char c = mBuffer.front();
        mBuffer.remove(0,1);
        messageHandler.insertChar(c);
    }
    messageHandler.printBuffer();

}


void App::onTimer()
{
    if(mLock)
        return;
    mLock = true;

    if(requestDeviceName)
        sendDeviceName();
    else if(requestTags)
        createTags();
    else
        messageHandler.run();

    mLock = false;
}


void App::sendDeviceName()
{
    qDebug() << "Device name request";
    Message m;
    m.init();
    m.add("deviceName", "Atmega328Uno");
    m.finnish();
    int size = m.getSize();
    char test[size+1];
    m.getMessageData(test);
    //int size = m.getMessage(msg);
    QByteArray a(test, size+1);
    mDevice->write(a);

    m.destroy();


    requestDeviceName = false;
}


void App::createTags()
{
    qDebug() << "Create tags...";
    Message pb0;
    pb0.init();
    pb0.add("pb0", bool(0));
    pb0.finnish();
    int s = pb0.getSize();
    char p[s+1];
    pb0.getMessageData(p);
    QByteArray pp(p, s+1);
    mDevice->write(pp);


    requestTags = false;
}
