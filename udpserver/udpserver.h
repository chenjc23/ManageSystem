#ifndef UDPSERVER_H
#define UDPSERVER_H

#include <QObject>
#include <QUdpSocket>
#include <QSqlDatabase>


class UdpServer : public QObject
{
    Q_OBJECT
public:
    explicit UdpServer(const QHostAddress &host, quint16 port);

    // init socket
    void initSocket();

signals:

public slots:
    void readPendingDatagrams();

private:
    const QHostAddress &host;
    const quint16 port;
    QUdpSocket *udpSocket;
    QSqlDatabase db;
};

#endif // UDPSERVER_H
