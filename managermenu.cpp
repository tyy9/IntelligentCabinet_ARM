#include "managermenu.h"
#include "ui_managermenu.h"

ManagerMenu::ManagerMenu(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::ManagerMenu)
{
    ui->setupUi(this);
    GetTableData();
    //设置列数
    ui->TableList->setColumnCount(5);
    ui->TableList->setHorizontalHeaderLabels(QStringList()<<"id"<<"t_name"<<"OrderId"<<"state"<<"card_id");
    ui->TableList->setRowCount(100);
}

ManagerMenu::~ManagerMenu()
{
    delete ui;
}
void ManagerMenu::GetTableData(){
    //获取url地址
    QString url_str=QString("http://192.168.63.168:8001/myintelligentcabinet/usr-order/findAll");
    //创建请求
    QUrl url(url_str);
    QNetworkRequest request(url);
    request.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");
    //get请求获取图片
    manager_getTableInfo=new QNetworkAccessManager();
    manager_getTableInfo->get(request);
    connect(manager_getTableInfo,SIGNAL(finished(QNetworkReply*)),this,SLOT(GetTableDataReply(QNetworkReply*)));
}
void ManagerMenu::GetTableDataReply(QNetworkReply* reply){
    //先清空容器
    foreach(auto tableinfo,tableinfo_v){
        delete tableinfo;
    }
    tableinfo_v.clear();
    //------------
    auto Info=reply->readAll();
    qDebug()<<Info;
    QTextCodec *codec=QTextCodec::codecForName("UTF-8");
    QString tableinfo=QString::fromUtf8(Info);
    //CodeInfo.replace("\\n","\n");
    qDebug().noquote() << tableinfo;
    // QMessageBox::information(this,"提示","订单号已记录");
    // 解析json数据
    // 遍历获取json数据
    // 解析JSON
    QJsonDocument jsonDoc = QJsonDocument::fromJson(Info);
    if (!jsonDoc.isNull()) {
        QJsonObject jsonObj = jsonDoc.object();

        // 获取"data"对象
        QJsonObject dataObj = jsonObj["data"].toObject();

        // 获取"table_list"数组
        QJsonArray tableList = dataObj["table_list"].toArray();

        // 遍历每个对象
        for (const QJsonValue &value : tableList) {
            QJsonObject tableObj = value.toObject();
            int id = tableObj["id"].toInt();
            int state = tableObj["state"].toInt();
            QString tname = tableObj["tname"].toString();
            QJsonValue orderIdValue = tableObj["orderId"];
            int orderId = orderIdValue.toInt();
            QString cardid=tableObj["cardid"].toString();
            TableInfo *tableinfo=new TableInfo();
            tableinfo->setTname(tname);
            tableinfo->setId(id);
            tableinfo->setOrderId(orderId);
            tableinfo->setState(state);
            tableinfo->setCardid(cardid);
            //tableinfo->toString();
            tableinfo_v.push_back(tableinfo);
        }
    }
    int i=0;
    foreach(auto tableinfo,tableinfo_v){
        ui->TableList->setItem(i,0,new QTableWidgetItem(QString::number(tableinfo->getId())));
        ui->TableList->setItem(i,1,new QTableWidgetItem(tableinfo->getTname()));
        ui->TableList->setItem(i,0,new QTableWidgetItem(QString::number(tableinfo->getOrderId())));
        ui->TableList->setItem(i,0,new QTableWidgetItem(QString::number(tableinfo->getState())));
        ui->TableList->setItem(i,1,new QTableWidgetItem(tableinfo->getcardid()));
        i++;
    }
}
