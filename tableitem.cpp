#include "tableitem.h"
#include "ui_tableitem.h"

#include<QDebug>
#include<QMessageBox>
TableItem::TableItem(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::TableItem)
{
    ui->setupUi(this);
    connect(ui->btnTable1,SIGNAL(pressed()),this,SLOT(storeFunc()));
    connect(ui->btnTable2,SIGNAL(pressed()),this,SLOT(storeFunc()));
    connect(ui->btnTable3,SIGNAL(pressed()),this,SLOT(storeFunc()));
    connect(ui->btnTable4,SIGNAL(pressed()),this,SLOT(storeFunc()));
    connect(ui->btnTable5,SIGNAL(pressed()),this,SLOT(storeFunc()));
}

TableItem::~TableItem()
{
    delete ui;
}
void TableItem::setOrderId(QString OrderId){
    this->OrderId=OrderId;
}
void TableItem::LoadItemData(QVector<TableInfo*> &TableInfo_v){
    //为每个tableButton赋值
    int i=1;
    foreach(auto tableinfo, TableInfo_v) {
        switch (i) {
        case 1:
            if(tableinfo->getState()==0){
                ui->btnTable1->setText(tableinfo->getTname()+":\n(空闲)");
                ui->btnTable1->setStyleSheet("color:rgb(0,255,0);");
            }else{
                ui->btnTable1->setText(tableinfo->getTname()+":\n(已占用)");
                ui->btnTable1->setStyleSheet("color:rgb(255,0,0);");
            }
            break;
        case 2:
            if(tableinfo->getState()==0){
                ui->btnTable2->setText(tableinfo->getTname()+":\n(空闲)");
                ui->btnTable2->setStyleSheet("color:rgb(0,255,0);");
            }else{
                ui->btnTable2->setText(tableinfo->getTname()+":\n(已占用)");
                ui->btnTable2->setStyleSheet("color:rgb(255,0,0);");
            }
            break;
        case 3:
            if(tableinfo->getState()==0){
                ui->btnTable3->setText(tableinfo->getTname()+":\n(空闲)");
                ui->btnTable3->setStyleSheet("color:rgb(0,255,0);");
            }else{
                ui->btnTable3->setText(tableinfo->getTname()+":\n(已占用)");
                ui->btnTable3->setStyleSheet("color:rgb(255,0,0);");
            }
            break;
        case 4:
            if(tableinfo->getState()==0){
                ui->btnTable4->setText(tableinfo->getTname()+"::\n(空闲)");
                ui->btnTable4->setStyleSheet("color:rgb(0,255,0);");
            }else{
                ui->btnTable4->setText(tableinfo->getTname()+"::\n(已占用)");
                ui->btnTable4->setStyleSheet("color:rgb(255,0,0);");
            }
            break;
        case 5:
            if(tableinfo->getState()==0){
                ui->btnTable5->setText(tableinfo->getTname()+":\n(空闲)");
                ui->btnTable5->setStyleSheet("color:rgb(0,255,0);");
            }else{
                ui->btnTable5->setText(tableinfo->getTname()+":\n(已占用)");
                ui->btnTable5->setStyleSheet("color:rgb(255,0,0);");
            }
            break;
        }
        i++;
    }
}
void TableItem::setStore_flag(int store_flag){
    this->store_flag=store_flag;
}
void TableItem::storeFunc(){
    if(store_flag){
        QPushButton *button=qobject_cast<QPushButton*>(QObject::sender());
        QString ret=button->text();
        tname=ret.split(":").at(0);
        qDebug()<<"tname:"<<tname;
        store=new StoreDialog(this);
        store->exec();
    }
}
void TableItem::CheckCardOK(int role,QString cardid){
    qDebug()<<"ok";
    if(role==2){
        QMessageBox::information(this,"提示","已检测到工作卡,请把食物放入对应序号的餐柜中");
        //更新餐柜信息
        UpdateTableinfo(role,cardid,1);
    }
    if(role==3){
        QMessageBox::information(this,"提示","已检测到学生卡，请取出您的外卖");
    }
    store->close();
}
void TableItem::UpdateTableinfo(int role,QString cardid,int status){
    //获取url地址
    QString url_str=QString("http://192.168.63.168:8001/myintelligentcabinet/table-info/updateTableByCard/%1/%2/%3")
                          .arg(status).arg(tname).arg(OrderId);
    //创建请求
    QUrl url(url_str);
    QNetworkRequest request(url);
    //把appcode和服务器主机域名添加到请求对象中
    request.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");
    //get请求获取图片
    manager_tableUpdate=new QNetworkAccessManager();
    QJsonObject jsonObj;
    jsonObj["cardId"]=cardid;
    jsonObj["state"]=1;
    qDebug()<<"OrderId:"<<OrderId;
    QJsonDocument jdoc(jsonObj);
    manager_tableUpdate->post(request,jdoc.toJson(QJsonDocument::Compact));
    connect(manager_tableUpdate,SIGNAL(finished(QNetworkReply*)),this,SLOT(UpdateReply(QNetworkReply*)));
}
void TableItem::UpdateReply(QNetworkReply* reply){
    qDebug()<<reply->readAll();
    //通知主窗口重新获取table数据并刷新
    emit refreshTableData();
}
