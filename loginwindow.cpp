#include "loginwindow.h"
#include "ui_loginwindow.h"
#include<QWidget>
LoginWindow::LoginWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::LoginWindow)
{
    ui->setupUi(this);
}

LoginWindow::~LoginWindow()
{
    delete ui;
}



void LoginWindow::on_btnReturn_pressed()
{
    //获取上一个界面
    QWidget *w=this->parentWidget();
    w->show();
    delete this;
    //this->close();
}

