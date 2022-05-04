#include "dm_lab3.h"
#include <QtWidgets/QApplication>
#include <QSql>
#include <QtSql/qsqldatabase.h>
#include <iostream>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    dm_lab3 w;  
    w.show();
    return a.exec();
}
