#pragma once

#include <QtWidgets/QMainWindow>
#include <QStandardItemModel>
#include <QTableView>
#include "ui_dm_lab3.h"
#include <QSql>
#include <qsqlerror.h>
#include <QtSql/qsqldatabase.h>
#include <QMessageBox>
#include <iostream>
#include <qsqlquery.h>
#include <qinputdialog>
#include <qpushbutton.h>
#include <QVBoxLayout>
#include <QLabel>
#include <qfile.h>
#include <qfiledialog.h>
#include <qjsondocument>
#include <qjsonobject>
#include <qjsonarray>
#include <QListWidget>
#include <QProcess>
#include "ConnectionManager.h"

class dm_lab3 : public QMainWindow
{
    Q_OBJECT
public slots:
    void searchEntries();
    void addEntries();
    void updateEntry();
    void deleteEntry();
    void loadEntries();
    void truncateTable();
    void createProcs();
    void reconnect();
    void createDB();
    void deleteDB();
    void createUser();
    void getAll();
public:
    dm_lab3(QWidget *parent = Q_NULLPTR);
    ~dm_lab3();
    QString getDbConn();

private:
    int connectionIndex;
    QString dbconn;
    QVector<QString> permissions;
    Ui::dm_lab3Class ui;
    QSqlDatabase db;
    QStandardItemModel* m;
    QListWidget connections;
    QJsonArray users;
    QJsonArray usernames;
    ConnectionManager manager;
    void recalculateButtonVisibility(QVector<QString>& permissions);

};
