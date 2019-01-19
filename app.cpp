#include "app.h"

#include <QDebug>

#include <device/inputdevicemanager.h>
#include <device/device.h>



App::App(int argc, char *argv[]) : QCoreApplication(argc, argv),
    mDevice(nullptr)
{
    connect(&InputDeviceManager::sGetInstance(), &InputDeviceManager::inputDeviceAvailable, this, &App::onDeviceAvailable);
    connect(&InputDeviceManager::sGetInstance(), &InputDeviceManager::inputDeviceConnected, this, &App::onDeviceConnected);
    InputDeviceManager::sGetInstance().setUseDefaultSerialSettingFlag(true);
    InputDeviceManager::sGetInstance().setDetectInputDevicesInterval(1000);
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
    qDebug() << aData;
}
