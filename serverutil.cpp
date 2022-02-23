#include "config.h"
#include "serverutil.h"
#include <QtSql>
#include <QSet>
#include <QJsonObject>
#include <iostream>

QSqlDatabase server::getDbOpenConn(const QString &name)
{
    Config config;
    QSqlDatabase db;
    if (!name.isNull())
        db = QSqlDatabase::addDatabase("QMYSQL", name);
    else
        db = QSqlDatabase::addDatabase("QMYSQL");

    db.setDatabaseName(config.dbDatabaseName);
    db.setHostName(config.dbHostName);
    db.setUserName(config.dbUserName);
    db.setPassword(config.dbPassword);
    if (!db.open())
        std::cout << "DB Connection Fail";
    return db;
}

void server::addDeviceRecord(const QString &ipAdress, QJsonObject &jsonObject)
{
    QSqlQuery query;

    query.prepare("select id from device where ip=?");
    query.addBindValue(ipAdress);
    query.exec();

    int deviceID;
    if (!query.first()) {
        std::cout << "Upload from unknown ip address: " << ipAdress.toStdString() << std::endl;
        return;
    } else {
        deviceID = query.value(0).toInt();
    }

    QString sql = QString("insert into device_%1_attr set ").arg(deviceID);

    QList<QVariant> valueList;

    QJsonObject::iterator iter;
    for (iter = jsonObject.begin(); iter != jsonObject.end(); ++iter) {
        sql += QString("%1 = ?").arg(iter.key());
        if (iter + 1 != jsonObject.end()) sql += ",";
        valueList.append(iter.value().toVariant());
    }
    query.prepare(sql);

    for (QVariant value : valueList) {
        query.addBindValue(value);
    }
    query.exec();

    //std::cout << query.lastQuery().toStdString() << std::endl;
    if (query.lastError().type() != QSqlError::NoError)
        std::cout << query.lastError().text().toStdString() << std::endl;
    else
        std::cout << "Device " << deviceID << " upload a new record" << std::endl;
    // 更新设备状态
    server::updateDeviceState(deviceID);
}

void server::updateDeviceState(int deviceID)
{
    QSqlQuery query;
    QString sql = QString("update device set activated_time = "
                          "(case when activated_time is null then now() "
                          "else activated_time end), "
                          "current_state = "
                          "(case when current_state = \"未激活\" then \"已激活\" "
                          "else current_state end), "
                          "last_time = now() where id =%1").arg(deviceID);
    query.exec(sql);
    if (query.lastError().type() != QSqlError::NoError)
        std::cout << query.lastError().text().toStdString() << std::endl;
}
