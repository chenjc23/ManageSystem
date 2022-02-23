#include <QCoreApplication>
#include <QCommandLineParser>
#include "../iotserverinterface.h"
#include "../udpserver.h"
#include "../tcpserver.h"
#include "../mqttpeer.h"
#include <QSqlDatabase>
#include <QSettings>

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    QSettings settings("../config.ini", QSettings::IniFormat);

    QSqlDatabase db;
    db = QSqlDatabase::addDatabase("QMYSQL");
    db.setDatabaseName(settings.value("db/dbName").toString());
    db.setHostName(settings.value("db/hostName").toString());
    db.setUserName(settings.value("db/userName").toString());
    db.setPassword(settings.value("db/password").toString());
    db.open();

    QCommandLineParser parser;
    parser.addHelpOption();

    QCommandLineOption udpOption(QStringList()<< "u" << "udp");
    QCommandLineOption tcpOption(QStringList()<< "t" << "tcp");
    QCommandLineOption mqttOption(QStringList()<< "m" << "mqtt");

    parser.addOption(udpOption);
    parser.addOption(tcpOption);
    parser.addOption(mqttOption);
    parser.process(a);

    IotServerInterface * server;

    if (parser.isSet(udpOption))
        server = new UdpServer;
    else if (parser.isSet(tcpOption))
        server = new TcpServer;
    else if (parser.isSet(mqttOption))
        server = new MqttPeer;
    else
        return 0;

    server->initServer();
    return a.exec();
}
