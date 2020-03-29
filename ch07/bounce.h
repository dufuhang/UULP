//bounch.h
// some setting for the game
//
#include <sys/time.h>

#define BLANK      '  ' 
#define DFL_SYMBOL 'o'
#define TOP_ROW     5
#define BOT_ROW     20
#define LEFT_EDGE   10
#define RIGHT_EDGE  70
#define X_INIT      10  //starting col
#define Y_INIT      10  //starting row
#define TICKS_PER_SEC 50    //affecting row

#define  X_TTM      5
#define  Y_TTM      8

//ping pong ball
struct ppball
{
    int y_pos, x_pos,
        y_ttm, x_ttm,
        y_ttg, x_ttg, 
        y_dir, x_dir;
    char symbol;
};

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
