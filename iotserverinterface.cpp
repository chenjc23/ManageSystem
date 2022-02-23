#include "iotserverinterface.h"
#include <iostream>
#include <QDateTime>
#include <QtSql>
#include <QJsonDocument>
#include <QJsonObject>
#include <string>


IotServerInterface::IotServerInterface(QObject *parent) : QObject(parent)
{

}

void IotServerInterface::setHost(const QString &host)
{
    this->host = host;
}

void IotServerInterface::setPort(quint16 port)
{
    this->port = port;
}

int IotServerInterface::getDeviceID(const QHostAddress &host)
{
    QSqlQuery query;
    query.prepare("select id from device where ip=?");
    query.addBindValue(host.toString());
    query.exec();

    if (query.first()) return query.value(0).toInt();
    else return 0;
}

int IotServerInterface::getDeviceID(const QString &topic)
{
    QString deviceSec = topic.section('/', 2, 2);
    return deviceSec.section('_', 1, 1).toInt();
}

void IotServerInterface::addDeviceRecord(int deviceID, const QByteArray &message)
{
    // 更新设备状态
    this->updateDeviceState(deviceID);
    // 插入记录
    QJsonParseError err;
    QJsonDocument msgDoc = QJsonDocument::fromJson(message, &err);
    // 若解析json报错则终止
    if (err.error != QJsonParseError::NoError) {
        logPrompt(deviceID, err.errorString().toStdString());
        return;
    }
    QJsonObject msgObj = msgDoc.object();

    QSqlQuery query;
    QString sql = QString("insert into device_%1_attr set ").arg(deviceID);

    QList<QVariant> valueList;

    QJsonObject::iterator iter;
    for (iter = msgObj.begin(); iter != msgObj.end(); ++iter) {
        sql += QString("%1 = ?").arg(iter.key());
        if (iter + 1 != msgObj.end()) sql += ",";
        valueList.append(iter.value().toVariant());
    }
    query.prepare(sql);

    for (QVariant value : valueList) {
        query.addBindValue(value);
    }
    query.exec();
    // 插入错误则显示错误信息
    if (query.lastError().type() != QSqlError::NoError)
        logPrompt(deviceID, query.lastError().text().toStdString());
    else
        logPrompt(deviceID, msgDoc.toJson().toStdString());
}

void IotServerInterface::updateDeviceState(int deviceID)
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
        logPrompt(deviceID, query.lastError().text().toStdString());
}

void IotServerInterface::logPrompt(std::string msg)
{
    std::cout << QDateTime::currentDateTime().toString("yyyy/M/d HH:mm:ss -- ").toStdString()
              << msg << std::endl;
}

void IotServerInterface::logPrompt(int deviceID, std::string msg)
{
    std::cout << QDateTime::currentDateTime().toString("yyyy/M/d HH:mm:ss -- ").toStdString()
              << "On device_" << deviceID << "'s upload: "
              << msg << std::endl;
}
