#pragma once
#include <QMap>
#include <QString>
#include <QJsonObject>
#include <QJsonArray>
#include <QFileInfo>
#include <QFile>
#include <QDir>
#include <QJsonDocument>
class ConnectionManager
{
    QMap<QString, QString> connectionMap;
    QJsonDocument doc;
    QJsonArray users;
    QJsonArray usernames;
    QString fileName;
public:
    QJsonArray getUsers();
    QJsonArray getUserNames();
    ConnectionManager(QString);
    void addUser(QString username, QString connectionString);

};

