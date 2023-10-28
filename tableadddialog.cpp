#include "tableadddialog.h"
#include "ui_tableadddialog.h"
#include<QThread>
TableAddDialog::TableAddDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::TableAddDialog)
{
    ui->setupUi(this);
    tableinfo=new TableInfo();
}

TableAddDialog::~TableAddDialog()
{
    qDebug()<<"TableAddDialog close";
    delete manager_AddTable;
    delete tableinfo;
    delete ui;
}

void TableAddDialog::on_btnAdd_pressed()
{
    tableinfo->setOrderId((ui->lineEdit_OrderId->text()).toInt());
    tableinfo->setState((ui->lineEdit_state->text()).toInt());
    tableinfo->setTname(ui->lineEdit_tname->text());
    tableinfo->setCardid(ui->lineEdit_cardid->text());
    tableinfo->toString();
    //获取url地址
    QString url_str=QString("http://192.168.63.168:8001/myintelligentcabinet/table-info/addTable");
    //创建请求
    QUrl url(url_str);
    QNetworkRequest request(url);
    //把appcode和服务器主机域名添加到请求对象中
    request.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");
    manager_AddTable=new QNetworkAccessManager();
    QJsonObject jsonObj;
    jsonObj["tname"]=tableinfo->getTname();
    jsonObj["state"]=tableinfo->getState();
    jsonObj["orderId"]=tableinfo->getOrderId();
    jsonObj["cardId"]=tableinfo->getcardid();
    QJsonDocument jdoc(jsonObj);
    manager_AddTable->post(request,jdoc.toJson(QJsonDocument::Compact));
    connect(manager_AddTable,SIGNAL(finished(QNetworkReply*)),this,SLOT(AddTableReply(QNetworkReply*)));
}

void TableAddDialog::AddTableReply(QNetworkReply* reply){
    //tableitem->setScanflag(scan_flag);//只有扫描二维码后才可点击餐柜
    auto Info=reply->readAll();
    QTextCodec *codec=QTextCodec::codecForName("UTF-8");
    auto LoginInfo=codec->toUnicode(Info);
    LoginInfo.replace("\\n","\n");
    QJsonDocument jdoc=QJsonDocument::fromJson(LoginInfo.toUtf8());
    int code=20000;
    if (!jdoc.isNull()) {
        QJsonObject jsonObj = jdoc.object();
        code=jsonObj["code"].toInt();
    }
    if(code==20001){
        QMessageBox::information(this,"提示","添加成功");
        this->close();
        QThread::sleep(2);//先关后解析
        emit ReturnToMenu();
    }else{
        QMessageBox::information(this,"提示","添加失败");
    }

}

void TableAddDialog::on_btnReturn_pressed()
{
    this->close();
    QThread::sleep(1);//先关后解析
    emit ReturnToMenu();
}

