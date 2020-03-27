/* sigactdemo.c
 */
#include <stdio.h>
#include <signal.h>
#define INPUTLEN 10

void inthandler(int);

int main()
{
    struct sigaction newhandler;            //new settings
    sigset_t blocked;                         //set of blocked sigs
    char x[INPUTLEN];

    //load these two members first
    newhandler.sa_handler = inthandler;
    //handler func
    newhandler.sa_flags = SA_RESETHAND | SA_RESTART;    //options
    //then build the list of blocked signals
    sigemptyset(&blocked);              //clear all bits
    sigaddset(&blocked, SIGQUIT);       //add SIGQUIT to list
    newhandler.sa_mask = blocked;       //store blockmask
    
    if (sigaction(SIGINT, &newhandler, NULL) == -1)
    {
        perror("sigaction");
    }
    else
    {
        while (1)
        {
            fgets(x, INPUTLEN, stdin);
            printf("input: %s", x);
        }
    }
}

void inthandler(int s)
{
    printf("Called with signal %d\n", s);
    sleep(s);
    printf("does handleing signal %d\n", s);
}
