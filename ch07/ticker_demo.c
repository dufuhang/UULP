#include <stdio.h>
#include <sys/time.h>
#include <signal.h>
#include <stdlib.h>

int set_ticker(int n_msecs);
void countdown(int signum);

int main()
{
    signal(SIGALRM, countdown);
    //传入的是毫秒!!!!!!
    if (set_ticker(500) == -1)
    {
        perror("set_ticker");
    }
    else
    {
        while (1)
        {
            printf("I am in a loop\n");
            sleep(0.5);
        }
    }
    return 0;
}

//信号中断处理函数
void countdown(int signum)
{
    static int num = 10;
    printf("%d .. ", num--);
    fflush(stdout);
    if (num < 0)
    {
        printf("DONE! \n");
        exit(0);
    }
}

/*
 * set_ticker(millseconds)
 * 将毫秒转化为秒和微秒
 * note: set_ticker(0) turns off ticker
 */
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
