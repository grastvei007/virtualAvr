#ifndef APP_H
#define APP_H

#include <QCoreApplication>
#include <QString>
#include <QByteArray>

class Device;

class App : public QCoreApplication
{
    Q_OBJECT
public:
    App(int argc, char *argv[]);

private slots:
    void onDeviceAvailable(QString aDevice);
    void onDeviceConnected(QString aDevice);
    void onDataRecieved(QByteArray aData);
private:
    Device *mDevice;
};

#endif // APP_H
