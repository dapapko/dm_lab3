#include "ConnectionManager.h"

    QJsonArray ConnectionManager::getUsers() {
        return users;
    }


    QJsonArray ConnectionManager::getUserNames() { return usernames; }


    void ConnectionManager::addUser(QString username, QString connectionString) {
        QJsonObject obj = doc.object();
        QJsonValue usersValue =  obj.value(QString("users"));
        QJsonValue usernamesValue = obj.value(QString("usernames"));
        QJsonArray usernamesArray = usernamesValue.toArray();
        QJsonArray usersArray = usersValue.toArray();
        usernamesArray.push_back(QJsonValue(username));
        usersArray.push_back(QJsonValue(connectionString));
        obj.insert(QString("users"), usersArray);
        obj.insert(QString("usernames"), usernamesArray);
        this->users = obj["users"].toArray();
        this->usernames = obj["usernames"].toArray();
        doc.setObject(obj);
        QFile jsonFile(fileName);
        jsonFile.open(QIODevice::WriteOnly);
        jsonFile.write(doc.toJson());
        jsonFile.close();
    }


    ConnectionManager::ConnectionManager(QString openFileName): fileName(openFileName)
    {

        QFileInfo fileInfo(fileName);
        QDir::setCurrent(fileInfo.path());
        QFile jsonFile(fileName);
        if (!jsonFile.open(QIODevice::ReadOnly))
        {
            return;
        }

        QByteArray saveData = jsonFile.readAll();
        doc = QJsonDocument::fromJson(saveData);
        QJsonObject obj = doc.object();
        users = obj["users"].toArray();
        usernames = obj["usernames"].toArray();
    }


