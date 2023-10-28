#include "loginwindow.h"
#include "ui_loginwindow.h"

LoginWindow::LoginWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::LoginWindow)
{
    ui->setupUi(this);
    managermenu=NULL;
}

LoginWindow::~LoginWindow()
{
    qDebug()<<"LoginWindow close";
    delete manager_login;
    delete ui;
}

void LoginWindow::refreshData(){
    ui->lineEdit_password->clear();
    ui->lineEdit_usrname->clear();
//    if(managermenu!=NULL){
//        delete managermenu;
//        managermenu=NULL;
//    }
}

void LoginWindow::on_btnReturn_pressed()
{
    //获取上一个界面
    QWidget *w=this->parentWidget();
    w->show();
    emit ReturnSignal();
    delete this;
    //this->close();
}

void LoginWindow::on_btnLogin_pressed()
{
    //获取用户名与密码
    QString username=ui->lineEdit_usrname->text();
    QString password=ui->lineEdit_password->text();
    //获取url地址
    QString url_str=QString("http://192.168.63.168:8001/myintelligentcabinet/usr-order/login/%1/%2").arg(username).arg(password);
    //创建请求
    QUrl url(url_str);
    QNetworkRequest request(url);
    //把appcode和服务器主机域名添加到请求对象中
    request.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");
    manager_login=new QNetworkAccessManager();
    manager_login->post(request,QByteArray());
    connect(manager_login,SIGNAL(finished(QNetworkReply*)),this,SLOT(LoginReply(QNetworkReply*)));
}

void LoginWindow::LoginReply(QNetworkReply* reply){
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
        QMessageBox::information(this,"提示","登录成功");
        managermenu=new ManagerMenu(this);
        connect(managermenu,SLOT(ReturnSignal()),this,SLOT(refreshData()));
        this->hide();
        managermenu->show();
    }else{
        QMessageBox::information(this,"提示","登录失败");
    }
}


