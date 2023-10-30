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
#include"tableadddialog.h"
#include<QList>
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
    void UpdateTableData(TableInfo *tableinfo);
    void DeleteTableData(TableInfo *tableinfo);
private slots:
    void GetTableDataReply(QNetworkReply* reply);
    void on_btnAdd_pressed();
    void refreshTableData();
    void on_TableList_cellClicked(int row, int column);
    void on_btnUpdate_pressed();
    void UpdateTableInfoReply(QNetworkReply *reply);
    void DeleteTableInfoReply(QNetworkReply *reply);
    void on_btnDelete_pressed();

    void on_TableList_itemSelectionChanged();

    void on_btnReturn_pressed();

signals:
    void ReturnSignal();
private:
    QNetworkAccessManager manager_getTableInfo;
    QNetworkAccessManager manager_UpdateTableInfo;
    QNetworkAccessManager manager_deleteTableInfo;
    QVector<TableInfo*> tableinfo_v;
    TableAddDialog *tableAddDialog;
    QVector<int> update_row;//支持多行更新
    QVector<int> delete_row;//支持多行删除
    Ui::ManagerMenu *ui;
};

#endif // MANAGERMENU_H
