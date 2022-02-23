#include "udpserver.h"
#include "../serverutil.h"
#include <QNetworkDatagram>
#include <QString>
#include <QJsonParseError>
#include <QJsonDocument>
#include <QJsonValue>
#include <QJsonObject>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <iostream>

UdpServer::UdpServer(const QHostAddress &host, quint16 port) :
    host(host), port(port)
{

}

void UdpServer::initSocket() {
    std::cout << "init udp socket..." << std::endl;
    std::cout << "init udp socket2..." << std::endl;
    udpSocket = new QUdpSocket(this);
    udpSocket->bind(host, port);
    connect(udpSocket, &QUdpSocket::readyRead, this, &UdpServer::readPendingDatagrams);
}

void UdpServer::readPendingDatagrams() {
    while (udpSocket->hasPendingDatagrams()) {
        QNetworkDatagram datagram = udpSocket->receiveDatagram();
        QJsonParseError j_p;
        QJsonDocument j_d = QJsonDocument::fromJson(datagram.data(), &j_p);
        if (j_p.error != QJsonParseError::NoError) {
            return;
        }
        QJsonObject j_o = j_d.object();
        server::addDeviceRecord(datagram.senderAddress().toString(), j_o);
    }
}

