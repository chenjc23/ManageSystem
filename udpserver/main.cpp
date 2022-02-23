#include <QCoreApplication>
#include "../config.h"
#include "../serverutil.h"
#include <QSqlDatabase>
#include "../udpserver.h"
#include "../tcpserver.h"
#include "../mqttpeer.h"

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);
    Config config;
    QSqlDatabase db = server::getDbOpenConn();
    if (!db.isOpen()) return 0;
//    UdpServer server(QHostAddress(config.udpServerAddress),config.udpServerPort);
//    server.initSocket();
    MqttPeer s;
    s.initServer();
    return a.exec();
}
