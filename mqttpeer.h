#ifndef MQTTPEER_H
#define MQTTPEER_H

#include <QObject>
#include "iotserverinterface.h"
#include <QtMqtt/qmqttclient.h>

class MqttPeer : public IotServerInterface
{
    Q_OBJECT
public:
    explicit MqttPeer(QObject *parent = nullptr);
    void initServer() override;

public slots:
    void dealWithMsg(const QByteArray &msg, const QMqttTopicName &topic);

private:
    QMqttClient * client;

    void subscribeTopics();


signals:

};

#endif // MQTTPEER_H
