#ifndef MANAGERMENU_H
#define MANAGERMENU_H

#include <QMainWindow>
#include<QWidget>
#include<QString>
#include<QNetworkAccessManager>
#include<QNetworkReply>
#include<QNetworkRequest>
#include<QPixmap>
#include<QDebug>
#include<QTextCodec>
#include<QJsonDocument>
#include<QJsonArray>
#include<QJsonObject>
#include<QJsonValue>
#include<QPushButton>
#include<QMessageBox>
#include<QVector>
#include"tableitem.h"
#include<QTableWidget>
#include<QTableWidgetItem>
namespace Ui {
class ManagerMenu;
}

class ManagerMenu : public QMainWindow
{
    Q_OBJECT

public:
    explicit ManagerMenu(QWidget *parent = nullptr);
    ~ManagerMenu();
    void GetTableData();
private slots:
    void GetTableDataReply(QNetworkReply* reply);
private:
    QNetworkAccessManager *manager_getTableInfo;
    QVector<TableInfo*> tableinfo_v;
    Ui::ManagerMenu *ui;
};

#endif // MANAGERMENU_H
