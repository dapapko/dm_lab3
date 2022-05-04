#include "dm_lab3.h"
#include "DBAL.h"
#include "Dbcreator.h"



QString dm_lab3::getDbConn() 
{
    return this->usernames.at(0).toString();
}


void dm_lab3::addEntries() 
{
    dbconn = this->users.at(this->connectionIndex).toString();
    DBAL dbal(dbconn);
    int exemption = ui.exemption->value();
    QString address = ui.addressField->text();
    QString name = ui.nameField->text();
    if (name.isEmpty() || address.isEmpty() || exemption < 1) 
    {
        QMessageBox::warning(this, "Please fill all fields", "All fields are mandatory");
        return;
    }
    bool status = dbal.insert(name,address, exemption);
    if (!status) 
    {
        QMessageBox::warning(this, "Result", "Insertion error");
        return;
    }
    loadEntries();
    
}


void dm_lab3::truncateTable() 
{
    dbconn = this->users.at(this->connectionIndex).toString();
    DBAL db(dbconn);
    bool status = db.truncate();
    if (!status) QMessageBox::warning(this, "", "Truncate error");
    loadEntries();
}


void dm_lab3::updateEntry()
{
    dbconn = this->users.at(this->connectionIndex).toString();
    DBAL dbal(dbconn);
    int id = ui.idInput->value();
    int exemption = ui.exemption->value();
    QString address = ui.addressField->text();
    QString name = ui.nameField->text();
    dbal.update(name, address, exemption, id);
    loadEntries();
}


void dm_lab3::getAll() 
{
    loadEntries();
}


void dm_lab3::loadEntries() 
{
    
    dbconn = this->users.at(this->connectionIndex).toString();
    DBAL db(dbconn);

    db.getAll(this->m);
      
}


void dm_lab3::deleteEntry()
{
    DBAL db(dbconn);
    int id = ui.idInput->value();
    db.remove(id);
    loadEntries();
    
}


void dm_lab3::searchEntries() 
{
    dbconn = this->users.at(this->connectionIndex).toString();
    DBAL db(dbconn);
    int fieldValue = -1;
    bool ok;
    int text = QInputDialog::getInt(this, tr("Get values by exemption"), "Exemption",0,-256,3000,1, &ok);
    if (ok && text != -1)
        fieldValue = text;
    db.search(this->m, fieldValue);
    
    
    
}

void dm_lab3::createProcs() {
    QFile f("C:\\dbprocs.sql");
    if (!f.open(QFile::ReadOnly | QFile::Text)) return;
    QTextStream in(&f);
    QString queryText = in.readAll();
    dbconn = this->users.at(this->connectionIndex).toString();
    DBAL db(dbconn);
    db.execStatement(queryText);
    
}

void dm_lab3::reconnect() 
{
    
    auto indexes = this->connections.selectionModel()->selectedIndexes();
    if (indexes.size() == 0)
    {
        QMessageBox::warning(this, "No user selected", "Select user");
        return;
    }
    this->connectionIndex = indexes.at(0).row();
    dbconn = this->manager.getUsers()[this->connectionIndex].toString();
    permissions = QVector<QString>();
    DBAL(dbconn).checkPermissions(permissions, "employers", this->manager.getUserNames()[this->connectionIndex].toString());
    recalculateButtonVisibility(permissions);

}


void dm_lab3::createDB() 
{
    dbconn = this->users[this->connectionIndex].toString();
    Dbcreator dbc(QString("lab3"), QString("public.employers"), dbconn);
    QMap<QString, QString> fields;
    fields["name"] = "TEXT NOT NULL";
    fields["id"] = "serial PRIMARY KEY";
    fields["address"] = "TEXT NOT NULL";
    fields["exemption"] = "integer NOT NULL";
    dbc.createDB();
    dbc.createTable(fields);
}


void dm_lab3::createUser() 
{
    dbconn = this->users.at(this->connectionIndex).toString();
    Dbcreator dbc(QString("lab3"), QString("public.employers"),dbconn);
    bool admin = ui.adminCheckbox->checkState();
    QString username = ui.username->text();
    QString password = ui.password->text();
    QString connectionString = QString("Driver={PostgreSQL Unicode};Server=localhost;Database=lab3;Uid=%1;Pwd=%2;").arg(username, password);
    dbc.createUser(username, password, admin);
    manager.addUser(username, connectionString);
    QStringList usersList;
    QString val;
    this->connections.addItem(connectionString);

}


void dm_lab3::deleteDB() 
{
    dbconn = this->users.at(this->connectionIndex).toString();
    Dbcreator dbc(QString("lab3"), QString("public.employers"), dbconn);
    dbc.deleteDB();
}


void dm_lab3::recalculateButtonVisibility(QVector<QString>& permissions) {
    if (!permissions.contains(QString("INSERT"))) { ui.addButton->hide(); }
    else { ui.addButton->show(); }
    if (!permissions.contains(QString("DELETE"))) { ui.deleteButton->hide(); }
    else { ui.deleteButton->show(); }
    if (!permissions.contains(QString("UPDATE"))) { ui.saveButton->hide(); }
    else { ui.saveButton->show(); }
    if (!permissions.contains(QString("TRUNCATE"))) { ui.truncateButton->hide(); }
    else { ui.truncateButton->show(); }
}


dm_lab3::dm_lab3(QWidget *parent)
    : QMainWindow(parent), manager(ConnectionManager(QString("C:\\dbconn.json"))), connections(QListWidget())
{
    this->usernames = this->manager.getUserNames();
    this->users = this->manager.getUsers();
    this->connectionIndex = 0;
    dbconn = this->users.at(this->connectionIndex).toString();
    QStringList usersList;
    QString val;
    for (int i = 0; i < this->users.size(); i++) {
        val = this->users.at(i).toString();
        usersList.push_back(val);
    }
    auto listModel = new QStandardItemModel();
    this->connections.addItems(usersList);
    this->connections.show();
    auto table = new QTableView(this); 
    int width = this->width() - 100;
    int height = this->height() - 100;
    permissions = QVector<QString>();
    DBAL(getDbConn()).checkPermissions(permissions, "employers", users[0].toString());
    this->m = new QStandardItemModel();
    QStringList labels = QString::fromUtf8("ID,Name,Address,Exemption").simplified().split(",");
    this->m->setHorizontalHeaderLabels(labels);
    this->m->setColumnCount(4);
    table->setModel(this->m);
    table->resize(width, height);
    table->setShowGrid(true);
    table->show();
    loadEntries();
    ui.setupUi(this);
    

}


dm_lab3::~dm_lab3() 
{
    db.close();
}