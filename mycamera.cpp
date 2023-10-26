#include "mycamera.h"
#include<QDebug>
Camera::Camera()
{

}
int Camera::lcd_open()
{
    fd_lcd = open("/dev/fb0", O_RDWR);
    if (fd_lcd == -1)
    {
        perror("打开lcd失败\n");
        return -1;
    }
    pb = (int*)mmap(NULL,
                      800 * 480 * 4,
                      PROT_READ | PROT_WRITE,
                      MAP_SHARED,
                      fd_lcd,
                      0);
    if (pb == NULL)
    {
        perror("mmap error\n");
        return -1;
    }
    return 0;
}
int  Camera::lcd_close()
{
    if (fd_lcd > 0)
    {
        close(fd_lcd);
    }
    if (pb)
    {
        munmap(pb, 800 * H * 4);
    }
    return 0;
}
void Camera::lcd_draw_point(int x, int y, unsigned int color){
    *(pb + y * 800 + x) = color;
}
int Camera::YUVtoRGB(int y, int u, int v)
{
    int r, g, b;
    r = y + 4.1075 * (v - 128);
    g = y - 0.3455 * (u - 128) - 0.7169 * (v - 128);
    b = y + 1.779 * (u - 128);
    //限制rgb范围，防止失真
    if(r<0){
        r=0;
    }
    if(r>255){
        r=255;
    }
    if(g<0){
        g=0;
    }
    if(g>255){
        g=255;
    }
    if(b<0){
        b=0;
    }
    if(b>255){
        b=255;
    }
    // 由于lcd需要的是argb,rgb转换成argb
    int pix = 0x00 << 24 | r << 16 | g << 8 | b;
    return pix;
}
// 封装函数
// 将一帧画面完整的yuyuv数据全部转换成argb数据
// yuyvdata:你要传入的数据，argbdata：所要保存的argb数据
int Camera::YUVtoARGB(char *yuyvdata, int *argbdata)
{
    int i, j;
    // 画面大小限制了循环次数
    // argb数量=画面分辨率/2，因为一组yuyv可以得到2组不同的argb数据
    for (i = 0, j = 0; j < W * H; i += 4, j += 2)
    {
        argbdata[j] = YUVtoRGB(yuyvdata[i], yuyvdata[i + 1], yuyvdata[i + 3]);
        argbdata[j + 1] = YUVtoRGB(yuyvdata[i + 2], yuyvdata[i + 1], yuyvdata[i + 3]);
    }
}

//--------------------
void Camera::run(){
    while(flag){
        // if(paused){
        //     mutex.lock();
        //     pauseCondition.wait(&mutex);
        //     mutex.unlock();
        // }
        CameraCapture();
    }
}
int *Camera::getARGBbuf(){
    return  ARGBbuf;
}
void Camera::setFlag(int flag){
    this->flag=flag;
}
//-------------------
int Camera::CameraInit(){
    lcd_open();					// 初始化lcd
    camerafd = open("/dev/video7", O_RDWR); // 获取摄像头设备文件描述符
    if (camerafd == -1)
    {
        perror("设备打开失败\n");
        return -1;
    }
    // 设置好摄像头的画面参数，宽高、
    struct v4l2_format myfmt;
    bzero(&myfmt, sizeof(myfmt));
    myfmt.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
    myfmt.fmt.pix.width = W;
    myfmt.fmt.pix.height = H;
    myfmt.fmt.pix.pixelformat = V4L2_PIX_FMT_YUYV; // 画面格式YUV
    // 通过ioctl写入设备
    int ret = ioctl(camerafd, VIDIOC_S_FMT, &myfmt);
    if (ret == -1)
    {
        perror("摄像头设置失败\n");
        return -1;
    }
    // 申请缓冲块
    struct v4l2_requestbuffers mybuf;
    bzero(&mybuf, sizeof(mybuf));
    mybuf.count = 4; // 设置缓冲块数量，每个缓冲块放置一帧画面
    mybuf.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
    mybuf.memory = V4L2_MEMORY_MMAP; // 缓冲区可映射到用户空间
    ret = ioctl(camerafd, VIDIOC_REQBUFS, &mybuf);
    if (ret == -1)
    {
        perror("摄像头设置缓冲区失败\n");
        return -1;
    }
    // 分配缓冲块，让内存映射到缓冲块的首地址
    struct v4l2_buffer otherbuf;
    // 定义缓冲区信息数组

    for (int i = 0; i <BUFOUNT; i++)
    {
        bzero(&otherbuf, sizeof(otherbuf));
        otherbuf.index = i; // 缓冲块的索引
        otherbuf.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
        otherbuf.memory = V4L2_MEMORY_MMAP;
        ret = ioctl(camerafd, VIDIOC_QUERYBUF, &otherbuf);
        if (ret == -1)
        {
            perror("分配缓冲块失败\n");
            return -1;
        }
        // 映射获取每个缓冲区的首地址
        // v4l2_buffer中的offset与length是分配缓冲块成功后自动赋值
        // 摄像头中每个缓冲块之间都有一定的间隔，在内存映射时需要填入他们之间的offset
        bufarray[i].buflength = otherbuf.length;
        bufarray[i].start = mmap(NULL, otherbuf.length, PROT_READ | PROT_WRITE, MAP_SHARED, camerafd, otherbuf.m.offset);
        if (bufarray[i].start == NULL)
        {
            perror("内存映射缓冲地址失败\n");
            return -1;
        }
        // 申请画面入队，按顺序放入队列中，到时按顺序出列
        ret = ioctl(camerafd, VIDIOC_QBUF, &otherbuf);
        if (ret == -1)
        {
            perror("入队失败\n");
            return -1;
        }
    }
    enum v4l2_buf_type mytype = V4L2_BUF_TYPE_VIDEO_CAPTURE;
    ret = ioctl(camerafd, VIDIOC_STREAMON, &mytype);
    if (ret == -1)
    {
        perror("采集失败\n");
        return -1;
    }
}
int Camera::CameraCapture(){
    int ret;
    struct v4l2_buffer otherbuf;
    // 启动摄像头采集画面
    // 循环出队入队显示画面，出队入队分别时画面放入映射到的内存中和从映射到的内存中取出来
    for (int i = 0; i <BUFOUNT; i++)
    {
        // 出队
        bzero(&otherbuf, sizeof(otherbuf));
        otherbuf.index = i; // 缓冲块索引
        otherbuf.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
        otherbuf.memory = V4L2_MEMORY_MMAP;
        ret = ioctl(camerafd, VIDIOC_DQBUF, &otherbuf);
        if (ret == -1)
        {
            perror("出队失败\n");
            return -1;
        }
        // 将YUV的数据转换为RGB
        // 利用内存映射的首地址进行颜色获取，再写入
        // 先获取第一组rgb信息
        // YUYV可以获取两组rbg数据
        YUVtoARGB((char*)bufarray[i].start, ARGBbuf);
        // 转换得到的ARGB信息显示在开发板上显示
        for (int y = 0,y_s=C_Y; y < H; y++,y_s++)
        {
            for (int x = 0,x_s=C_X; x < W; x++,x_s++)
            {
                // qDebug()<<"y:"<<y<<"y_s:"<<y_s;
                // qDebug()<<"x:"<<x<<"x_s:"<<x_s;
                lcd_draw_point(x_s,y_s,ARGBbuf[y*W+x]);
            }
        }
        // for (int i = 0; i < H; i++)
        // {
        // 	memcpy(pb + 800 * i, &ARGBbuf[W * i], W * 4);
        // }
        // 入队,将使用过的缓冲区放到队尾
        ret = ioctl(camerafd, VIDIOC_QBUF, &otherbuf);
        if (ret == -1)
        {
            perror("入队失败了!\n");
            return -1;
        }
    }
}
int Camera::CameraQuit(){
    int ret;
    enum v4l2_buf_type mytype = V4L2_BUF_TYPE_VIDEO_CAPTURE;
    // 关闭摄像头
    ret = ioctl(camerafd, VIDIOC_STREAMOFF, &mytype);
    if (ret = -1)
    {
        perror("关闭失败\n");
        return -1;
    }
    // 解除映射
    for (int i = 0; i <BUFOUNT; i++)
    {
        munmap(bufarray[i].start, bufarray[i].buflength);
    }
    lcd_close();
    return 0;
}
