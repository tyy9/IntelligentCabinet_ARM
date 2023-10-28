#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include"mycamera.h"
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
#include "nlohmann/json.hpp"
#include<QVector>
#include<iostream>
#include<string>
#include<QFileDialog>
#include<QImage>
#include<QByteArray>
#include<QBuffer>
#include<QVector>
#include<QFile>
#include<QIODevice>
#include<QMessageBox>
#include<QString>

#include "myserial.h"
#include<QTimer>
#include<QDate>
#include"loginwindow.h"
QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    void GetTableInfo(int page,int limit);
    void getFreeTableCount();
private slots:
    void on_btnScan_pressed();
    void BarCodeScan(QNetworkReply* reply);
    void receiveTableInfo(QNetworkReply* reply);
    void checkoutCard(int cardid);
    void CheckCardInfo(QNetworkReply* reply);
    void on_pushButton_3_pressed();
    void on_pushButton_2_pressed();
    void refreshTableData();
    void on_btnTakeItem_pressed();
    void getCount(QNetworkReply* reply);
    void MyClock();

    void on_btnAdmin_pressed();

signals:
    void checkCardOK(int role,QString cardid);

private:
    Ui::MainWindow *ui;
    Camera *camera;
    QString cardid;
    QNetworkAccessManager *manager_scan;
    QNetworkAccessManager *manager_getTableInfo;
    QNetworkAccessManager *manager_checkCardInfo;
    QNetworkAccessManager *manager_getFreeTableCount;
    LoginWindow *loginwindow;
    QVector<TableInfo*> tableinfo_v;
    TableItem *tableitem;
    StoreDialog *storedialog;
    int page;
    int scan_flag;
    int student_flag;
    int total;//餐柜总数量
    int pageSize;//页数
    MySerial *myserial;
    QTimer *timer;
    //0:条形码请求，1-4:CURD操作,5:管理员登录
};
#endif // MAINWINDOW_H
