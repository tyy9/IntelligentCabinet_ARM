#include "mybutton.h"
#include<QDebug>
#include<QObject>
#include<QKeyEvent>
#include<QEvent>
#include<QCoreApplication>
#include<QApplication>
#include<QString>
#include<QLineEdit>
#include<QTableWidgetItem>
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
        qDebug()<<"windows"<<this->window()->objectName();
        //判断是光标聚焦的是什么控件
        if(qApp->focusWidget()&&qApp->focusWidget()->inherits("QLineEdit")){
            QLineEdit *text=qobject_cast<QLineEdit*>(QApplication::focusWidget());
            QString old_text=text->text();
            int length=old_text.length();
            length--;
            if(length<0){
                text->clear();
            }else{
                QString new_text=old_text.mid(0,length);//删除最后一个
                text->clear();
                text->setText(new_text);
            }
        }
            QTableWidget *tableWidget = qobject_cast<QTableWidget*>(qApp->focusWidget());
            if (tableWidget) {
                // 获取具有焦点的项目
                QTableWidgetItem *focusedItem = tableWidget->currentItem();

                if (focusedItem) {
                    // 检查具有焦点的项目是否是 QTableWidgetItem
                    if (dynamic_cast<QTableWidgetItem*>(focusedItem)) {
                        // 具有焦点的项目是 QTableWidgetItem
                        QTableWidgetItem *tableItem = dynamic_cast<QTableWidgetItem*>(focusedItem);
                        // 这里可以执行相关操作
                        QString old_text=tableItem->text();
                        int length=old_text.length();
                        length--;
                        if(length<0){
                            tableItem->setText("");
                        }else{
                            QString new_text=old_text.mid(0,length);//删除最后一个
                            tableItem->setText("");
                            tableItem->setText(new_text);
                        }
                    }
                }
            }
    }else{
        //定义按键事件
        QKeyEvent *keyevent=new QKeyEvent(QEvent::KeyPress,keyinfo.toInt(),Qt::NoModifier,keyinfo);
        //将按键事件发送到光标聚焦的控件
        QCoreApplication::postEvent(QApplication::focusWidget(),keyevent);
    }
}
