#pragma once
#include <QString>
#include <QMap>
#include "DBAL.h"
#include "ConnectionManager.h"
class Dbcreator
{
    
        QString dbname;
        QString tableName;
        DBAL db;
        QString dbconn;
    public:
        Dbcreator(QString DBname, QString tableName, QString dbconn) : dbconn(dbconn), dbname(DBname), tableName(tableName), db(DBAL(dbconn))
        {}

        void createUser(QString username, QString password, bool admin);
        void deleteDB();
        void createDB();
        void createTable(QMap<QString, QString>& fields);
    
};

