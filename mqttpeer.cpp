#include "mqttpeer.h"
#include <QSettings>
#include <QSqlQuery>
#include <QtMqtt/qmqttclient.h>
#include <iostream>

MqttPeer::MqttPeer(QObject *parent) :
    IotServerInterface(parent),
    client(new QMqttClient(this))
{
    QSettings settings("../config.ini", QSettings::IniFormat, this);
    host = settings.value("ip/mqtt").toString();
    port = settings.value("port/mqtt").toInt();
}

void MqttPeer::initServer()
{
    logPrompt("Mqtt Peer Init...");
    client->setHostname(host);
    client->setPort(port);

    connect(client, &QMqttClient::connected, [=]{
        // 遍历设备进行设备topic的订阅
        subscribeTopics();
        logPrompt("Ready");
    });

    client->connectToHost();
    connect(client, &QMqttClient::messageReceived, this, &MqttPeer::dealWithMsg);
}

void MqttPeer::dealWithMsg(const QByteArray &msg, const QMqttTopicName &topic)
{
    int deviceID = getDeviceID(topic.name());
    addDeviceRecord(deviceID, msg);
}

void MqttPeer::subscribeTopics()
{
    QSqlQuery query;
    query.exec("select a.product_id, a.id from device a "
               "inner join product b on a.product_id=b.id "
               "where b.protocol=\"MQTT\"");
    while (query.next()) {
        QString topic = QString("nbiot/product_%1/device_%2/#").arg(
                    query.value(0).toInt()).arg(query.value(1).toInt());
        client->subscribe(topic);
    }
}
