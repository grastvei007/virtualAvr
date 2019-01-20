#ifndef APP_H
#define APP_H

#include <QCoreApplication>
#include <QString>
#include <QByteArray>
#include <QTimer>


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

    void onTimer();

private:
    void sendDeviceName();
    void createTags();

private:
    QByteArray mBuffer;

    QTimer *mTimer;
    bool mLock;
};

#endif // APP_H
