#include "udpserver.h"
#include <QSettings>
#include <QHostAddress>
#include <QNetworkDatagram>

UdpServer::UdpServer(QObject *parent) :
    IotServerInterface(parent),
    udpSocket(new QUdpSocket(this))
{
    QSettings setting("../config.ini", QSettings::IniFormat, this);
    host = setting.value("ip/udp").toString();
    port = setting.value("port/udp").toInt();
}

void UdpServer::initServer()
{
    logPrompt("Udp Server Init...");
    if (udpSocket->bind(QHostAddress(host), port))
        logPrompt("Ready");
    else
        logPrompt("Init fail");

    connect(udpSocket, &QUdpSocket::readyRead,
            this, &UdpServer::readPendingMsg);
}

void UdpServer::readPendingMsg()
{
    while (udpSocket->hasPendingDatagrams()) {
        QNetworkDatagram datagram = udpSocket->receiveDatagram();
        int deviceID = getDeviceID(datagram.senderAddress());
        if (deviceID)
            addDeviceRecord(deviceID, datagram.data());
        else
            logPrompt("Upload from unknown ip address: "+
                      datagram.senderAddress().toString().toStdString());
    }
}
