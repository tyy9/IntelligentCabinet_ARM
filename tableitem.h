#ifndef TABLEITEM_H
#define TABLEITEM_H

#include <QWidget>
#include <QVector>
#include"tableinfo.h"
#include"storedialog.h"
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
namespace Ui {
class TableItem;
}

class TableItem : public QWidget
{
    Q_OBJECT

public:
    explicit TableItem(QWidget *parent = nullptr);
    ~TableItem();
    void LoadItemData(QVector<TableInfo*> &TableInfo_v);
    void setScanflag(int scanflag);
    void setStudentflag(int studentflag);
    void setOrderId(QString OrderId);
    void UpdateTableinfo(QString cardid,int status);
    void checkUsrInfo(QString cardid);
private slots:
    void storeFunc();
    void CheckCardOK(int role,QString cardid);
     void UpdateReply(QNetworkReply* reply);
    void getUsrInfo(QNetworkReply* reply);
signals:
     void refreshTableData();
private:
    Ui::TableItem *ui;
    QString tname;
    QString OrderId;
    int scanflag;
    int studentflag;//学生刷卡标志
    StoreDialog *store;
    QNetworkAccessManager *manager_tableUpdate;
    int table_index;//所选中的餐柜
};

#endif // TABLEITEM_H
