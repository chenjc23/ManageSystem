#include <QCoreApplication>
#include "mqttpeer.h"

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    MqttPeer mqttPeer;
    mqttPeer.init();

    return a.exec();
}
