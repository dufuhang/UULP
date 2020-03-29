#include <stdio.h>
#include <curses.h>
#include <signal.h>
#include <string.h>
#include <sys/time.h>

#define MESSAGE "hello"
#define BLANK   "     "

void move_msg(int signum);
int set_ticker(int n_msecs);

int row;    //current row
int col;    //current column
int dir;    //where we are going

int main()
{
    int delay;      //bigger >= slower
    int ndelay;    //new delay
    int c;          //user input

    initscr();
    crmode();       //开启CBREAK模式:当接收到一个键盘输入，直接返回给程序
    noecho();       //关闭回显
    clear();

    row = 10;  //start here
    col = 0;
    dir = 1;    //add 1 to row number
    delay = 200;    //200ms = 0.2sec

    move(row, col); //get into position
    addstr(MESSAGE);
    signal(SIGALRM, move_msg);
    set_ticker(delay);

    while (1)
    {
        ndelay = 0;
        c = getch();
        if (c == 'Q') break;
        if (c == ' ') dir = -dir;
        if (c == 'f' && delay > 2) ndelay = delay / 2;
        if (c == 's') ndelay = delay * 2;
        if (ndelay > 0) set_ticker(delay = ndelay);
    }
    endwin();
    return 0;
}

void move_msg(int signum)
{
    //signal(SIGALRM, move_msg);    //reset, just in case
    move(row, col);
    addstr(BLANK);
    col += dir;         //move to new column
    move(row, col);     //then set cursor
    addstr(MESSAGE);    //redo message
    refresh();

    if (dir == -1 && col <= 0)
    {
        dir = 1;
    }
    else if (dir == 1 && col + strlen(MESSAGE) >= COLS)
    {
        dir = -1;
    }
}
int set_ticker(int n_msecs)
{
    struct itimerval new_timeset;
    long n_sec, n_usecs;
    //  1秒 = 1000 毫秒
    //  1秒 = 1000000 微秒
    n_sec = n_msecs / 1000;     //将毫秒转换为秒,取整数部分
    n_usecs = (n_msecs % 1000) * 1000L;     //将小数部分转化为微秒

    new_timeset.it_interval.tv_sec = n_sec; //set reload
    new_timeset.it_interval.tv_usec = n_usecs;  //new ticker

    new_timeset.it_value.tv_sec = n_sec;    //store this
    new_timeset.it_value.tv_usec = n_usecs; //and this

    return setitimer(ITIMER_REAL, &new_timeset, NULL);
}
