#pragma once
#include <QString>
#include <QSqlDatabase>
#include <QVector>
#include <QSqlQuery>
#include <qsqlerror.h>
#include <qstandarditemmodel.h>

class DBAL {
	QString connectString;
	QSqlDatabase db;
public:
	void checkPermissions(QVector<QString>& permissions, QString table, QString user);
	DBAL(QString connectString);
	void setConnectionString(QString conn);
	bool execStatement(QString statement);
	bool remove(int id);
	bool insert(QString name, QString address, int exemption);
	bool update(QString name, QString address, int exemption, int id);
	bool truncate();
	void search(QStandardItemModel* m, int value);
	void getAll(QStandardItemModel*);
	template<typename T, typename ...Args>
	bool exec(T statement, Args... args);
	QSqlQuery execReturnResult(QString);
	~DBAL();

};