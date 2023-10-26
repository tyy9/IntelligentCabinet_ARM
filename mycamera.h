#ifndef CAMERA_H
#define CAMERA_H

#include"myhead.h"
#include<QThread>
#include<QMutex>
#include<QWaitCondition>
#define W 160
#define H 120
#define BUFOUNT 4
#define C_X 640
#define C_Y 0
struct camerbuf
{
    void *start;   // 缓冲区的首地址
    int buflength; // 缓冲区的大小
};
class Camera:public QThread
{
public:
    Camera();
    int CameraInit();//初始化
    int CameraCapture();//缓冲区的出队与入队
    int CameraQuit();//相机销毁
    int lcd_open();//初始化lcd
    int lcd_close();//关闭lcd
    void lcd_draw_point(int x, int y, unsigned int color);
    int YUVtoRGB(int y, int u, int v);
    int YUVtoARGB(char *yuyvdata, int *argbdata);
    void run();
    int *getARGBbuf();
    void setFlag(int flag);
private:
    int fd_lcd;
    int *pb;
    struct camerbuf bufarray[BUFOUNT];
    int camerafd;
    bool paused;
    int ARGBbuf[W * H];
    int flag;
};

#endif // CAMERA_H
