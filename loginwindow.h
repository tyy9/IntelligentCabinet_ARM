#ifndef LOGINWINDOW_H
#define LOGINWINDOW_H

#include <QMainWindow>
#include<QWidget>
#include<QString>
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
#include "managermenu.h"
namespace Ui {
class LoginWindow;
}

class LoginWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit LoginWindow(QWidget *parent = nullptr);
    ~LoginWindow();

private slots:
    void on_btnReturn_pressed();
    void LoginReply(QNetworkReply* reply);
    void on_btnLogin_pressed();
signals:
    void ReturnSignal();
private:
    Ui::LoginWindow *ui;
    QNetworkAccessManager *manager_login;
    ManagerMenu *managermenu;
};

#endif // LOGINWINDOW_H
