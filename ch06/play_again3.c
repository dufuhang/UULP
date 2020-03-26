//关闭规范模式，使程序能够及时响应
#include <stdio.h>
#include <termios.h>
#include <fcntl.h>
#include <string.h>
#include <ctype.h>

#define ASK "Do you want another transaction"
#define TRIES 3                         //max tries
#define SLEEPTIME 2                     //time per try
#define BEEP putchar('\a')              //alert user

int get_response(char* question, int maxtries);
int get_ok_char();
void set_cr_noecho_mode();
void set_nodelay_mode();
int tty_mode(int how);

int main()
{
    int response;
    tty_mode(0);                        //save tty mode
    set_cr_noecho_mode();
    set_nodelay_mode();                 //noinput -> EOF
    response = get_response(ASK,TRIES);  //get some answer
    tty_mode(1);                        //restore tty mode
    printf("\n");
    return response;
}

int get_response(char* question, int maxtries)
{
    int input;
    printf("%s(y/n)?\n", question);
    //这里不太懂为什么?
    fflush(stdout);                 //force output
    while (1)
    {
        sleep(SLEEPTIME);           //wait a bit
        input = tolower(get_ok_char()); //get next char
        if (input == 'y')
        {
            return 0;
        }
        if (input == 'n')
        {
            return 1;
        }
        if (maxtries-- == 0)    //out time
        {   
            return 2;           //sayso
        }
        BEEP;
    }
}

//skip over non-legal chars and return  y, Y, n, N or EOF
int get_ok_char()
{
    //这里c默认为0
    int c;
    //当没有输入时，c默认为0，转换成char即为'\0'，strchr()函数返回'\0'出现的位置, 不是NULL
    //所以无输入时，会直接退出
    while ((c = getchar()) != EOF && strchr("yYnN", c) == NULL)
        ;
    return c;
}

void set_cr_noecho_mode()
{
    struct termios ttystate;
    tcgetattr(0, &ttystate);
    //设备文件连接属性
    ttystate.c_lflag &= ~ECHO;              //no echo
    ttystate.c_lflag &= ~ICANON;            //no buffering
    //VMIN 告诉驱动程序一次可以读取多少个字符 
    ttystate.c_cc[VMIN] = 1;                //get 1 char at a time
    tcsetattr(0, TCSANOW, &ttystate);       //install settings
}

void set_nodelay_mode()
{
    int termflags;
    //磁盘文件连接属性
    termflags = fcntl(0, F_GETFL);  //read current settings
    termflags |= O_NDELAY;          //flip on nodelay bit
    fcntl(0, F_SETFL, termflags);   //and install settings
}

//how == 0  -> save current mode  用于后续的恢复
//how == 1  -> restore mode
int tty_mode(int how)
{
    //恢复设置的时候需要用到original_mode中的设置信息，所以声明为static
    static struct termios original_mode;
    static int original_flags;
    if (how == 0)
    {
        tcgetattr(0, &original_mode);
        original_flags = fcntl(0, F_GETFL);
    }
    else
    {
        tcsetattr(0, TCSANOW, &original_mode);
        fcntl(0, F_SETFL, original_flags);
    }
}
