#include "DBAL.h"

DBAL::DBAL(QString connectString): connectString(connectString)
{
    db = QSqlDatabase::addDatabase("QODBC");
    db.setDatabaseName(connectString);
    db.open();
}

DBAL::~DBAL()
{
    if (db.isOpen()) db.close();
}


void DBAL::checkPermissions(QVector<QString>& permissions, QString table, QString user) {
    QString statement = "SELECT information_schema.role_table_grants.privilege_type FROM information_schema.role_table_grants WHERE information_schema.role_table_grants.table_name = ? AND information_schema.role_table_grants.grantee = ?;";
    QSqlQuery query;
    QString userName = user;
    if (!db.isOpen()) db.open();
    query.prepare(statement);
    query.addBindValue(table);
    query.addBindValue(user);
    bool status = query.exec();

    while (query.next())
    {
        QString q1 = query.value(0).toString();
        permissions.push_back(q1);
    }

}


void DBAL::setConnectionString(QString conn) {
    connectString = conn;
    if (db.isOpen()) db.close();
    db.setDatabaseName(conn);

}


bool DBAL::execStatement(QString statement)
{
    QSqlQuery query(db);
    if (!db.isOpen()) db.open();
    bool status = query.exec(statement);
    if (!status)
    {
        qDebug() << query.lastError().databaseText();
        qDebug() << query.lastError().text();
        qDebug() << query.executedQuery();
    }
    db.close();
    return status;
}


template<typename T, typename ...Args>
bool DBAL::exec(T statement, Args... args)
{
    QSqlQuery query;
    if (!db.isOpen()) db.open();
    query.prepare(QString(statement));
    ((query.addBindValue(args)), ...);
    bool status = query.exec();
    if (!status)
    {
        qDebug() << query.lastError().databaseText();
        qDebug() << query.lastError().text();
        qDebug() << query.executedQuery();
    }
    db.close();
    return status;
}



QSqlQuery DBAL::execReturnResult(QString statement)
{
    if (!db.isOpen()) db.open();
    QSqlQuery query(statement);
    query.exec();
    db.close();
    return query;
}


bool DBAL::truncate()
{
    bool status = exec("TRUNCATE TABLE public.employer CASCADE;");
    return status;
}


bool DBAL::remove(int id)
{
    QString statement = "CALL delete_employer(?);";
    bool status = exec(statement, id);
    return status;
}


bool DBAL::insert(QString name, QString address, int exemption)
{
    QString statement = "CALL create_employer(?,?,?);";
    bool status = exec(statement, name, address, exemption);
    return status;
}


bool DBAL::update(QString name, QString address, int exemption, int id)
{
    QString statement = "CALL update_employer(?,?,?,?);";
    bool status = exec(statement, name, address, exemption, id);
    return status;
}


void DBAL::search(QStandardItemModel* m, int value)
{
    QString statement = QString("SELECT id, name,address, exemption FROM get_by_exemption(?);");
    if (!db.isOpen()) db.open();
    QSqlQuery query(db);
    query.prepare(statement);
    query.addBindValue(value);
    bool status = query.exec();
    if (!status)
    {
        qDebug() << query.lastError().driverText();
        qDebug() << query.lastError().databaseText();
    }
    m->removeRows(0, m->rowCount());
    while (query.next()) 
    {
        QList<QStandardItem*> items;

        items.append(new QStandardItem(query.value(0).toString()));
        items.append(new QStandardItem(query.value(1).toString()));
        items.append(new QStandardItem(query.value(2).toString()));
        items.append(new QStandardItem(query.value(3).toString()));
        m->appendRow(items);
    }
    db.close();
}


void DBAL::getAll(QStandardItemModel* m)
{
    QString statement = "SELECT id, name,address,exemption FROM public.employers;";

    if (!db.isOpen()) db.open();
    QSqlQuery query(db);
    bool status = query.exec(statement);
    if (!status) 
    {
        qDebug() << query.lastError().driverText();
        qDebug() << query.lastError().databaseText();
    }
    m->removeRows(0, m->rowCount());
    while (query.next()) {
        QList<QStandardItem*> items;

        items.append(new QStandardItem(query.value(0).toString()));
        items.append(new QStandardItem(query.value(1).toString()));
        items.append(new QStandardItem(query.value(2).toString()));
        items.append(new QStandardItem(query.value(3).toString()));
        m->appendRow(items);
    }
    db.close();
    
    
}