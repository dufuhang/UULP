#include <stdio.h>
void main()
{
    int ret_from_fork, mypid;
    mypid = getpid();
    printf("Before:my pid is %d\n", mypid);
    //fork()之后，子进程拥有老进程的程序和当前运行到的位置
    ret_from_fork = fork();
    sleep(1);
    printf("After:my pid is %d, fork() said %d\n", getpid(), ret_from_fork);
}
