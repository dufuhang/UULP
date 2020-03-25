#include <stdio.h>
#include <termios.h>
#include <stdlib.h>
int main()
{
    //接受终端驱动程序的设置
    struct termios info;
    //判断是否接收成功
    int rv = tcgetattr(0, &info);
    if (rv == -1)
    {
        perror("tcgetattr");
        exit(1);
    }
    if (info.c_lflag & ECHO)
    {
        printf(" echo is on, since its bit is 1\n");
    }
    else
    {
        printf(" echo is off, since its bit is 0\n");
    }
    return 0;
}
