#include "tcpserver.h"
#include <QTcpSocket>
#include <QSettings>
#include <QHostAddress>
#include <QNetworkDatagram>

TcpServer::TcpServer(QObject *parent) :
    IotServerInterface(parent),
    tcpServer(new QTcpServer(this))
{
    QSettings setting("../config.ini", QSettings::IniFormat, this);
    host = setting.value("ip/tcp").toString();
    port = setting.value("port/tcp").toInt();
}

void TcpServer::initServer()
{
    logPrompt("Tcp Server Init...");
    if (tcpServer->listen(QHostAddress(host), port))
        logPrompt("Ready");
    else
        logPrompt("Init fail");

    connect(tcpServer, &QTcpServer::newConnection,
            this, &TcpServer::createNewConnection);
}

void TcpServer::createNewConnection()
{
    QTcpSocket * tcpSocket = tcpServer->nextPendingConnection();
    connect(tcpSocket, &QTcpSocket::readyRead, [=]{
        int deviceID = getDeviceID(tcpSocket->peerAddress());
        if (deviceID)
            addDeviceRecord(deviceID, tcpSocket->readAll());
        else
            logPrompt("Upload from unknown ip address: "+
                      tcpSocket->peerAddress().toString().toStdString());
    });
}
