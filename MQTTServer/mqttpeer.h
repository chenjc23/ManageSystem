#ifndef MQTTPEER_H
#define MQTTPEER_H

#include <QObject>
#include <QtMqtt/qmqttclient.h>

class MqttPeer : public QObject
{
    Q_OBJECT
public:
    explicit MqttPeer(QObject *parent = nullptr);

    void init();

private:
    QMqttClient * client;

signals:

};

#endif // MQTTPEER_H
