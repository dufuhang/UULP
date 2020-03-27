/* show how sleep works
 * set handler, set alarm, pauses, then return s
 */
#include <stdio.h>
#include <signal.h>

void wakeup(int);
// #define SHHHH
int main()
{
    printf("about to sleeo for 4 seconds\n");
    signal(SIGALRM, wakeup);    //catch it
    alarm(4);   //set clock
    pause();    //freeze here
    printf("Morning so soon?\n");
}

void wakeup(int signum)
{
# ifndef SHHHH
    printf("Alarm received from kernel\n");
# endif
}
