//关闭规范模式，是程序能够及时响应
#include <stdio.h>
#include <termios.h>

int get_response(char* question);
void set_cr_noecho_mode();
int tty_mode(int how);
#define QUESTION "Do you want another transaction"

int main()
{
    int response;
    tty_mode(0);                        //save tty mode
    set_cr_noecho_mode();
    response = get_response(QUESTION);  //get some answer
    tty_mode(1);                        //restore tty mode
    printf("\n");
    return response;
}

int get_response(char* question)
{
    int input;
    printf("%s(y/n)?\n", question);
    while (1)
    {
        switch (input = getchar())
        {
            case 'Y':
            case 'y': return 0; break;
            case 'N':
            case 'n': 
            case EOF: return 1; break;
        }
    }
}
void set_cr_noecho_mode()
{
    struct termios ttystate;
    tcgetattr(0, &ttystate);
    ttystate.c_lflag &= ~ECHO;              //no echo
    ttystate.c_lflag &= ~ICANON;            //no buffering
    //VMIN 告诉驱动程序一次可以读取多少个字符 
    ttystate.c_cc[VMIN] = 1;                //get 1 char at a time
    tcsetattr(0, TCSANOW, &ttystate);       //install settings
}
//how == 0  -> save current mode  用于后续的恢复
//how == 1  -> restore mode
int tty_mode(int how)
{
    //恢复设置的时候需要用到original_mode中的设置信息，所以声明为static
    static struct termios original_mode;
    if (how == 0)
    {
        tcgetattr(0, &original_mode);
    }
    else
    {
        return tcsetattr(0, TCSANOW, &original_mode);
    }
}
