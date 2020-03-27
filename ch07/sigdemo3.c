/* sigdemo3.c
 * perpose: show answers to signal questions
 * Q1 : 处理函数在一个信号达到后是否依然有效
 * Q2 : 当正在处理sigX信号时，sigX达到了会发生什么？
 * Q3 : 当正在处理sigX信号时，sigY达到了会发生什么？
 * Q4 : 当一个捕捉到一个信号时，read()会发生什么？
 */

#include <stdio.h>
#include <signal.h>
#include <string.h>
#define INPUTLEN 100

void inthandler(int);
void quithandler(int);

int main()
{
    char input[INPUTLEN];
    int nchars;

    signal(SIGINT, inthandler);
    signal(SIGQUIT, quithandler);

    do
    {
        printf("\nType a message\n");
        //从标准输入读取
        nchars = read(0, input, (INPUTLEN - 1));
        if (nchars == -1)
        {
            perror("read returned an error");
        }
        else
        {
            //在末尾添加'\0'，转换成字符串
            input[nchars] = '\0';
            printf("You typed: %s", input);
        }
    }while (strncmp(input, "quit", 4) != 0);
}

void inthandler(int s)
{
    printf(" Received signal %d..waiting\n", s);
    sleep(2);
    printf(" Leaving inthandler \n");
}

void quithandler(int s)
{
    printf(" Received signal %d..waiting\n", s);
    sleep(3);
    printf(" Leaving quithandler \n");
}
