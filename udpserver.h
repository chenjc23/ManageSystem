#ifndef UDPSERVER_H
#define UDPSERVER_H

#include <QObject>
#include <QHostAddress>
#include <QUdpSocket>
#include "iotserverinterface.h"

class UdpServer : public IotServerInterface
{
    Q_OBJECT
public:
    explicit UdpServer(QObject *parent = nullptr);
    void initServer() override;

public slots:
    void readPendingMsg();

private:
    QUdpSocket *udpSocket;

signals:

};

#endif // UDPSERVER_H
