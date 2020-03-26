#include <stdio.h>
#include <signal.h>
int main()
{
    int i;
    signal(SIGINT, SIG_IGN);
    printf("You can not stop me! \n");
    while (1)
    {
        printf("hahahaha\n");
        sleep(1);
    }
    return 0;
}

