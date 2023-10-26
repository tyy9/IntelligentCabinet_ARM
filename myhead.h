#ifndef MYHEAD_H
#define MYHEAD_H
//把所有常用的头文件全部包含
#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <string.h>
#include <stdlib.h>
#include <strings.h>
#include <stdbool.h>
#include <unistd.h>
#include <errno.h>
#include <dirent.h>
#include <linux/input.h>  //输入子系统模型有关的头文件
#include <sys/mman.h>   //内存映射有关的头文件
#include <sys/wait.h>
#include <signal.h>   //跟signal有关的头文件
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/msg.h>
#include <pthread.h>
#include <semaphore.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include<linux/videodev2.h>//跟v4l2架构有关
#include <termios.h>
#include <time.h>
#endif // MYHEAD_H
