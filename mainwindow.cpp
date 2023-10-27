#include "mainwindow.h"
#include "ui_mainwindow.h"
#include<QTabWidget>
#include <QWidget>
using namespace nlohmann;
MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    //自动启动摄像头线程
    camera=new Camera();
    camera->CameraInit();
    camera->setFlag(1);
    camera->start();
    //-----------------------------
    //餐柜选项初始化
    tableitem=new TableItem(this);
    tableitem->setGeometry(0,280,800,200);
    page=1;
    //---------------------
    GetTableInfo(1,5);
    getFreeTableCount();
    scan_flag=0;
    student_flag=0;
    storedialog=NULL;
    //---------------------
    //串口初始化
    myserial=new MySerial();
    connect(myserial,SIGNAL(checkCard(int)),this,SLOT(checkoutCard(int)));
    myserial->start();
    //---------------
    //---定时器初始化
    timer=new QTimer(this);
    timer->setInterval(1000);
    timer->start();
    connect(timer,SIGNAL(timeout()),this,SLOT(MyClock()));
    //--------------
    //主窗口的自己的信号与槽
    connect(this,SIGNAL(checkCardOK(int,QString)),tableitem,SLOT(CheckCardOK(int,QString)));
    connect(tableitem,SIGNAL(refreshTableData()),this,SLOT(refreshTableData()));
}

MainWindow::~MainWindow()
{
    delete ui;
}
void MainWindow::MyClock(){
    //获取当前时间与日期
    QTime time(QTime::currentTime());
    QDate date(QDate::currentDate());
    ui->labelDate->setText(date.toString("yyyy-MM-dd"));
    ui->labelClock->setText(time.toString("hh:mm:ss"));
}
void MainWindow::refreshTableData(){
    getFreeTableCount();
    GetTableInfo(page,5);
}
void MainWindow::getFreeTableCount(){
    //获取url地址
    QString url_str=QString("http://192.168.63.168:8001/myintelligentcabinet/table-info/findFreeTable");
    //创建请求
    QUrl url(url_str);
    QNetworkRequest request(url);
    request.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");
    //get请求获取图片
    manager_getFreeTableCount=new QNetworkAccessManager();
    manager_getFreeTableCount->get(request);
    connect(manager_getFreeTableCount,SIGNAL(finished(QNetworkReply*)),this,SLOT(getCount(QNetworkReply*)));
}
void MainWindow::getCount(QNetworkReply* reply){
    auto Info=reply->readAll();
    QTextCodec *codec=QTextCodec::codecForName("UTF-8");
    auto CountInfo=codec->toUnicode(Info);
    CountInfo.replace("\\n","\n");
    QJsonDocument jdoc=QJsonDocument::fromJson(CountInfo.toUtf8());
    if(!jdoc.isNull()){
        QJsonObject jobj=jdoc.object();
        auto dataObj=jobj["data"].toObject();
        int count=dataObj["count"].toInt();
        ui->labelCount->setText(QString::number(count)+"/10");
    }
}
void MainWindow::GetTableInfo(int page,int limit){
    //获取url地址
    QString url_str=QString("http://192.168.63.168:8001/myintelligentcabinet/table-info/findTableByPage/%1/%2")
                          .arg(page).arg(limit);
    //创建请求
    QUrl url(url_str);
    QNetworkRequest request(url);
    request.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");
    //get请求获取图片
    manager_getTableInfo=new QNetworkAccessManager();
    manager_getTableInfo->post(request,QByteArray());
    connect(manager_getTableInfo,SIGNAL(finished(QNetworkReply*)),this,SLOT(receiveTableInfo(QNetworkReply*)));
}
void MainWindow::on_btnScan_pressed()
{
    //将订单号截图并上传到接口处理
    int *data_argb=camera->getARGBbuf();
    //将ARGB转换成uchar*的rbg数据
    uchar data[W*H*3];
    for(int i=0;i<W*H;i++){
        data[i*3]=data_argb[i]>>16&0xFF;//r
        data[i*3+1]=data_argb[i]>>8&0xFF;//g
        data[i*3+2]=data_argb[i]&0xFF;//b
    }
    QImage img(data,W,H,QImage::Format_RGB888);
    //将图片保存到缓冲中，并以字符数组形式传输
    QByteArray barry;
    //定义缓冲区
    QBuffer buf(&barry);
    img.save(&buf,"JPG");
    //保存图片到本地
    QFile file("/root/user/qt_demo/snap.jpg");
    file.open(QIODevice::WriteOnly|QIODevice::Truncate);
    if(!file.isOpen()){
        qDebug()<<"文件保存失败";
        return;
    }
    file.write(barry.toBase64().data());
    file.close();
    //获取url地址
    QString url_str="http://qrbarcode.market.alicloudapi.com/api/predict/ocr_qrcode";
    //创建请求
    QUrl url(url_str);
    QNetworkRequest request(url);
    //彭老师申请得到appcode
    QString appCode="APPCODE 800d7be2cf5042d2b364a4ba1c9b5a9c";
    //服务器主机域名
    QString host="qrbarcode.market.alicloudapi.com";
    //把appcode和服务器主机域名添加到请求对象中
    request.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");
    request.setRawHeader("Authorization",appCode.toLocal8Bit());
    request.setRawHeader("Host",host.toLocal8Bit());
    //get请求获取图片
    manager_scan=new QNetworkAccessManager();
    QJsonObject jsonObj;
    jsonObj["image"]=barry.toBase64().data();
    QJsonDocument jdoc(jsonObj);
    manager_scan->post(request,jdoc.toJson(QJsonDocument::Compact));
    connect(manager_scan,SIGNAL(finished(QNetworkReply*)),this,SLOT(BarCodeScan(QNetworkReply*)));
}
void MainWindow::BarCodeScan(QNetworkReply *reply)
{
    scan_flag=1;
    tableitem->setScanflag(1);//只有扫描scan_flag二维码后才可点击餐柜
    auto Info=reply->readAll();
    QString OrderId;
    QMessageBox::information(this,"提示","订单号已记录");
    QTextCodec *codec=QTextCodec::codecForName("UTF-8");
    auto CodeInfo=codec->toUnicode(Info);
    qDebug()<<CodeInfo;
    CodeInfo.replace("\\n","\n");
    QJsonDocument jdoc=QJsonDocument::fromJson(CodeInfo.toUtf8());
    if(!jdoc.isNull()){
        QJsonObject obj=jdoc.object();
        //获取codes对象
        QJsonArray codesObj=obj["codes"].toArray();
        //获取data数据
        for(const QJsonValue &value : codesObj ){
            QJsonObject dataObj=value.toObject();
            OrderId=dataObj["data"].toString();
        }
        qDebug()<<"id:"<<OrderId;
        ui->LabelOrderId->setText(OrderId);
        tableitem->setOrderId(OrderId);
    }
}
void MainWindow::receiveTableInfo(QNetworkReply *reply)
{
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
        tableitem->LoadItemData(tableinfo_v);
    }
}


void MainWindow::on_pushButton_3_pressed()
{
    page++;
    if(page>2){
        page=1;
    }
    foreach(auto tableinfo,tableinfo_v){
        delete tableinfo;
    }
    tableinfo_v.clear();
    GetTableInfo(page,5);
}


void MainWindow::on_pushButton_2_pressed()
{
    page--;
    if(page<=0){
        page=2;
    }
    foreach(auto tableinfo,tableinfo_v){
        delete tableinfo;
    }
    tableinfo_v.clear();
    GetTableInfo(page,5);
}
void MainWindow::checkoutCard(int int_cardid){
    if(scan_flag||student_flag){
        //有人扫描了二维码或者学生点击取餐后才可检测卡号
        cardid=QString::number(int_cardid,16);
        //获取url地址
        QString url_str=QString("http://192.168.63.168:8001/myintelligentcabinet/usr/checkCard/%1")
                              .arg(cardid);
        //创建请求
        QUrl url(url_str);
        QNetworkRequest request(url);
        request.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");
        //get请求获取图片
        manager_checkCardInfo=new QNetworkAccessManager();
        manager_checkCardInfo->post(request,QByteArray());
        connect(manager_checkCardInfo,SIGNAL(finished(QNetworkReply*)),this,SLOT(CheckCardInfo(QNetworkReply*)));
    }

}
void MainWindow::CheckCardInfo(QNetworkReply* reply){
    //tableitem->setScanflag(scan_flag);//只有扫描二维码后才可点击餐柜
    auto Info=reply->readAll();
    QString OrderId;
    QTextCodec *codec=QTextCodec::codecForName("UTF-8");
    auto CodeInfo=codec->toUnicode(Info);
    qDebug()<<CodeInfo;
    CodeInfo.replace("\\n","\n");
    //----------------
    int code=20000;
    //-----------------
    //用户身份
    int role=3;
    QJsonDocument jdoc=QJsonDocument::fromJson(CodeInfo.toUtf8());
    if (!jdoc.isNull()) {
        QJsonObject jsonObj = jdoc.object();
        code=jsonObj["code"].toInt();

        // 获取"data"对象
        QJsonObject dataObj = jsonObj["data"].toObject();

        // 获取"usr"对象
        QJsonObject usr_Obj= dataObj["usr"].toObject();
        role=usr_Obj["role"].toInt();
        qDebug()<<"role:"<<role;
    }
    if(code==20001){
        emit checkCardOK(role,cardid);
        //更新餐柜信息
        tableitem->setScanflag(0);
        scan_flag=0;
        student_flag=0;
        tableitem->setStudentflag(0);
        if(storedialog!=NULL){
            storedialog->close();
        }

    }
}

void MainWindow::on_btnTakeItem_pressed()
{
    student_flag=1;
    tableitem->setStudentflag(1);
    storedialog=new StoreDialog(this);
    storedialog->exec();
}

