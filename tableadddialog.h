#ifndef TABLEADDDIALOG_H
#define TABLEADDDIALOG_H

#include <QDialog>
#include"tableinfo.h"
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
#include<QString>
namespace Ui {
class TableAddDialog;
}

class TableAddDialog : public QDialog
{
    Q_OBJECT

public:
    explicit TableAddDialog(QWidget *parent = nullptr);
    ~TableAddDialog();

private slots:
    void on_btnAdd_pressed();
    void AddTableReply(QNetworkReply *reply);
    void on_btnReturn_pressed();

signals:
    void ReturnToMenu();
private:
    Ui::TableAddDialog *ui;
    TableInfo *tableinfo;
    QNetworkAccessManager manager_AddTable;
};

#endif // TABLEADDDIALOG_H
