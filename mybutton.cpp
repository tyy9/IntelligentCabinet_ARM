#include "mybutton.h"
#include<QDebug>
#include<QObject>
#include<QKeyEvent>
#include<QEvent>
#include<QCoreApplication>
#include<QApplication>
#include<QString>
#include<QLineEdit>>
MyButton::MyButton(QWidget *parent)
    : QPushButton{parent}
{
    connect(this,SIGNAL(pressed()),this,SLOT(func()));
}
void MyButton::func(){
   //获取发送者
    QPushButton *button=qobject_cast<QPushButton*>(QObject::sender());
    //获取文本
    QString keyinfo=button->text();
    qDebug()<<"key:"<<keyinfo;
    if(keyinfo.compare("del")==0){
        QLineEdit *l=qobject_cast<QLineEdit*>(QApplication::focusWidget());
        QString old_text=l->text();
        int length=old_text.length();
        length--;
        if(length<0){
            l->clear();
        }else{
            QString new_text=old_text.mid(0,length);//删除最后一个
            l->clear();
            l->setText(new_text);
        }

    }else{
        //定义按键事件
        QKeyEvent *keyevent=new QKeyEvent(QEvent::KeyPress,keyinfo.toInt(),Qt::NoModifier,keyinfo);
        //将按键事件发送到光标聚焦的控件
        QCoreApplication::postEvent(QApplication::focusWidget(),keyevent);
    }
}
