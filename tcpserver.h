#ifndef TCPSERVER_H
#define TCPSERVER_H

#include <QObject>
#include <QTcpServer>
#include "iotserverinterface.h"

class TcpServer : public IotServerInterface
{
    Q_OBJECT
public:
    explicit TcpServer(QObject *parent = nullptr);
    void initServer() override;

public slots:
    void createNewConnection();

private:
    QTcpServer *tcpServer;

signals:

};

#endif // TCPSERVER_H
