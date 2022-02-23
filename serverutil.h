#ifndef SERVERUTIL_H
#define SERVERUTIL_H

#include "config.h"
#include <QtSql>

#include <QJsonObject>

namespace server {

QSqlDatabase getDbOpenConn(const QString &name = QString());

void addDeviceRecord(const QString &ipAdress, QJsonObject &jsonObject);

void updateDeviceState(int deviceID);
}

#endif // SERVERUTIL_H
