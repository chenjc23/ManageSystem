#ifndef CONFIG_H
#define CONFIG_H

#include <QObject>

class Config : public QObject
{
    Q_OBJECT
public:
    explicit Config();
    QString dbHostName;
    QString dbDatabaseName;
    QString dbUserName;
    QString dbPassword;
    QString udpServerAddress;
    quint16 udpServerPort;
signals:

};

#endif // CONFIG_H
