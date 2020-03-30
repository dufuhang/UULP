#include <stdio.h>
#include <stdlib.h>
#define DELAY 5

void child_code(int);
void parent_code(int);

void main()
{
    int newpid;
    printf("before:mypid is %d\n", getpid());

    if ((newpid = fork()) == -1)
    {
        perror("fork");
    }
    else if (newpid == 0)
    {
        child_code(DELAY);
    }
    else
    {
        parent_code(newpid);
    }
}

void child_code(int delay)
{
    printf("child %d here. will sleep for %d seconds\n", getpid(), delay);
    sleep(delay);
    printf("child done. about to exit\n");
    exit(17);
}

void parent_code(int childpid)
{
    int wait_rv;
    int child_status;
    int high_8, low_7, bit_7;
    wait_rv = wait(&child_status);
    printf("done waiting for %d. wait returned: %d\n",childpid, wait_rv);

    high_8 = child_status >> 8;     // 1111 1111 0000 0000
    low_7 = child_status & 0x7F;    // 0000 0000 0111 1111
    bit_7 = child_status & 0x80;    // 0000 0000 1000 0000
    printf("status:exit = %d. sig = %d, core = %d\n", high_8, low_7, bit_7);
}
