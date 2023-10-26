#ifndef MYSERIAL_H
#define MYSERIAL_H

#include"myhead.h"
#include<QThread>
#include<QMainWindow>
#include<QObject>
class MySerial:public QThread
{
    Q_OBJECT

public:
    MySerial();
    void init_tty(int fd);
    unsigned char CalBCC(unsigned char *buf, int n);
    int PiccRequest(int fd);
    int PiccAnticoll(int fd);
    int open_serial();
    void run();
signals:
    void checkCard(int card);
private:
    volatile unsigned int cardid ;
    struct timeval timeout;
};

#endif // MYSERIAL_H
