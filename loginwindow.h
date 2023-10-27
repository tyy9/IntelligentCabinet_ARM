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

private:
    Ui::LoginWindow *ui;
    QNetworkAccessManager *manager_login;
};

#endif // LOGINWINDOW_H
