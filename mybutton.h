#ifndef MYBUTTON_H
#define MYBUTTON_H

#include <QWidget>
#include<QPushButton>

class MyButton : public QPushButton
{
    Q_OBJECT
public:
    explicit MyButton(QWidget *parent = nullptr);

private slots:
    void func();
signals:

};

#endif // MYBUTTON_H
