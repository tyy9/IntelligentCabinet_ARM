#include "tableitem.h"
#include "ui_tableitem.h"

#include<QDebug>
#include<QMessageBox>
#include<QStringList>
TableItem::TableItem(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::TableItem)
{
    ui->setupUi(this);
    scanflag=0;
    studentflag=0;
    store=NULL;
    OrderId="0";
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
void TableItem::setStudentflag(int studentflag){
    this->studentflag=studentflag;
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
                ui->btnTable4->setText(tableinfo->getTname()+":\n(空闲)");
                ui->btnTable4->setStyleSheet("color:rgb(0,255,0);");
            }else{
                ui->btnTable4->setText(tableinfo->getTname()+":\n(已占用)");
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
void TableItem::setScanflag(int scanflag){
    this->scanflag=scanflag;
}
void TableItem::storeFunc(){
    if(scanflag){
        QPushButton *button=qobject_cast<QPushButton*>(QObject::sender());
        QString ret=button->text();
        QStringList ret_list=ret.split(":\n");
        tname=ret_list.at(0);
        qDebug()<<"tname:"<<tname;
        if(ret_list.at(1).compare("(已占用)")==0){
            QMessageBox::warning(this,"错误","该餐柜已被占用");
        }else{
            store=new StoreDialog(this);
            store->exec();
        }
    }
}
void TableItem::CheckCardOK(int role,QString cardid){
    qDebug()<<"ok";
    if(role==2){
        //防止误刷
        if(scanflag){
            QMessageBox::information(this,"提示","已检测到工作卡,请把食物放入对应序号的餐柜中");
            //更新餐柜信息
            UpdateTableinfo(cardid,1);
            if(store!=NULL){
                store->close();
            }

        }
    }
    if(role==3){
        if(studentflag){
            checkUsrInfo(cardid);
            QMessageBox::information(this,"提示","已检测到学生卡，请取出您的外卖,序号:"+tname);
        }
    }

}
void TableItem::checkUsrInfo(QString cardid){
    qDebug()<<"checkUsrInfo"<<cardid;
    //获取url地址
    QString url_str=QString("http://192.168.63.168:8001/myintelligentcabinet/usr-order/findUsrAndTableByOrderId/%1")
                          .arg(cardid);
    //创建请求
    QUrl url(url_str);
    QNetworkRequest request(url);
    //把appcode和服务器主机域名添加到请求对象中
    request.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");
    manager_tableUpdate=new QNetworkAccessManager();
    manager_tableUpdate->post(request,QByteArray());
    connect(manager_tableUpdate,SIGNAL(finished(QNetworkReply*)),this,SLOT(getUsrInfo(QNetworkReply*)));
}
void TableItem::UpdateTableinfo(QString cardid,int status){
    //获取url地址
    QString url_str=QString("http://192.168.63.168:8001/myintelligentcabinet/table-info/updateTableByCard/%1/%2/%3")
                          .arg(status).arg(tname).arg(OrderId);
    //创建请求
    QUrl url(url_str);
    QNetworkRequest request(url);
    //把appcode和服务器主机域名添加到请求对象中
    request.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");
    manager_tableUpdate=new QNetworkAccessManager();
    QJsonObject jsonObj;
    jsonObj["cardId"]=cardid;
    jsonObj["state"]=1;
    qDebug()<<"OrderId:"<<OrderId;
    QJsonDocument jdoc(jsonObj);
    manager_tableUpdate->post(request,jdoc.toJson(QJsonDocument::Compact));
    connect(manager_tableUpdate,SIGNAL(finished(QNetworkReply*)),this,SLOT(UpdateReply(QNetworkReply*)));
}
void TableItem::getUsrInfo(QNetworkReply*reply){
   //tableitem->setScanflag(scan_flag);//只有扫描二维码后才可点击餐柜
    auto Info=reply->readAll();
    QTextCodec *codec=QTextCodec::codecForName("UTF-8");
    auto UsrInfo=codec->toUnicode(Info);
    UsrInfo.replace("\\n","\n");
    QJsonDocument jdoc=QJsonDocument::fromJson(UsrInfo.toUtf8());
    if(!jdoc.isNull()){
        auto jobj=jdoc.object();
        auto obj=jobj["data"].toObject();
        tname=obj["tName"].toString();
        qDebug()<<"getUsrInfo:"<<tname;
    }
    //出柜
    UpdateTableinfo("0",2);
}
void TableItem::UpdateReply(QNetworkReply* reply){
    qDebug()<<reply->readAll();
    //通知主窗口重新获取table数据并刷新
    emit refreshTableData();
}
