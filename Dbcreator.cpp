#include "Dbcreator.h"


void Dbcreator::createDB()
{

    QSqlDatabase db = QSqlDatabase::addDatabase("QPSQL");
    db.setHostName("localhost");
    db.setPassword("Varvash4");
    db.setUserName("postgres");
    if (!db.isOpen()) db.open();
    QString statement = QString("CREATE DATABASE %1;").arg(dbname);
    QSqlQuery query(db);
    if (query.exec(statement)) {
        qDebug() << "It's OK";
    }
    else {
        qDebug() << "PostgreSQL error:" + query.lastError().text();
        qDebug() << query.lastQuery();
    }
    db.close();
}


void Dbcreator::createTable(QMap<QString, QString>& fields)
{
    QString back = fields.keys().back();
    QString cmd = QString("CREATE TABLE %1 ( \n").arg(tableName);
    for (auto e : fields.keys())
    {
        if (e != back) {
            cmd += QString("%1 %2,\n").arg(e, fields.value(e));
        }
        else {
            cmd += QString("%1 %2").arg(e, fields.value(e));
        }
    }
    cmd += " \n );";
    QSqlDatabase db = QSqlDatabase::addDatabase("QPSQL");
    db.setHostName("localhost");
    db.setPassword("Varvash4");
    db.setUserName("postgres");
    db.setDatabaseName("lab3");
    if (!db.isOpen()) db.open();
    QSqlQuery query(db);
    if (query.exec(cmd)) {
        qDebug() << "It's OK";
    }
    else {
        qDebug() << "PostgreSQL error:" + query.lastError().text();
        qDebug() << query.lastQuery();
    }
}


void Dbcreator::deleteDB()
{
    QString statement = QString("DROP DATABASE IF EXISTS %1 CASCADE;").arg(dbname);
    QSqlDatabase db = QSqlDatabase::addDatabase("QPSQL");
    db.setHostName("localhost");
    db.setPassword("Varvash4");
    db.setUserName("postgres");
    if (!db.isOpen()) db.open();
    QSqlQuery query(db);
    if (query.exec(statement)) {
        qDebug() << "It's OK";
    }
    else {
        qDebug() << "PostgreSQL error:" + query.lastError().text();
        qDebug() << query.lastQuery();
    }
    db.close();
}


void Dbcreator::createUser(QString username, QString password, bool admin)
{
    QString createUserStatement = QString("CREATE USER %1 WITH ENCRYPTED PASSWORD '%2';").arg(username, password);
    QString grantStatement = admin ? QString("GRANT ALL PRIVILEGES ON DATABASE %1 TO %2").arg(dbname, username) : QString("GRANT SELECT ON %1 TO %2;").arg(tableName, username);
    db.execStatement(createUserStatement);
    auto manager = ConnectionManager(QString("C:\\dbconn.json"));
    auto usernames = manager.getUserNames();
    auto users = manager.getUsers();
    db.setConnectionString(users.at(1).toString());
    db.execStatement(grantStatement);
}
