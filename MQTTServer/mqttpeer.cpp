#include "mqttpeer.h"
#include <QtMqtt/qmqttclient.h>
#include <iostream>
#include <QSettings>

MqttPeer::MqttPeer(QObject *parent) : QObject(parent)
{
    QSettings *settings = new QSettings("../config.ini", QSettings::IniFormat);
    QString host = settings->value("ip/mqtt").toString();
    int port = settings->value("port/mqtt").toInt();
    std::cout << host.toStdString() << port << std::endl;

    client = new QMqttClient(this);
    client->setHostname(host);
    client->setPort(port);

    connect(client, &QMqttClient::messageReceived, [=](const QByteArray &message, const QMqttTopicName &topic){
        std::cout << "in" << std::endl;
        std::cout << "Received Topic: " << topic.name().toStdString() << std::endl
                  << "Message: " << message.toStdString() << std::endl;
    });

    connect(client, &QMqttClient::connected, [=]{
        client->subscribe(QLatin1String("nbiot/chenjc"));
    });

}

void MqttPeer::init()
{
    std::cout << "MQTT server init ..." << std::endl;
    client->connectToHost();
}
