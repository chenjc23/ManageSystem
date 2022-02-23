#ifndef IOTSERVERINTERFACE_H
#define IOTSERVERINTERFACE_H

#include <QObject>
#include <iostream>
#include <string>
#include <QHostAddress>

class IotServerInterface : public QObject
{
    Q_OBJECT
public:
    explicit IotServerInterface(QObject *parent = nullptr);
    virtual void initServer() = 0;
    void setHost(const QString &host);
    void setPort(quint16 port);

protected:
    QString host;
    quint16 port;
    int getDeviceID(const QHostAddress &host);
    int getDeviceID(const QString &topic);
    void addDeviceRecord(int deviceID, const QByteArray &message);
    void updateDeviceState(int deviceID);
    void logPrompt(std::string msg);
    void logPrompt(int deviceID, std::string msg);

signals:

};

#endif // IOTSERVERINTERFACE_H
