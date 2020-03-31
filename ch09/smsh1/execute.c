#include <sys/wait.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>

int execute(char* argv[])
{
    int pid;
    int child_info = -1;    //存储子进程exit()的返回值

    if (argv[0] == NULL)
    {
        return 0;
    }
    if ((pid = fork()) == -1)
    {
        perror("fork");
    }
    else if (pid == 0)  //子进程中
    {
        //将SIGINT,SIGQUIT设置为默认处理方式
        signal(SIGINT, SIG_DFL);
        signal(SIGQUIT, SIG_DFL);
        execvp(argv[0], argv);
        perror("cannot execute command");
        exit(1);
    }
    else
    {
        if (wait(&child_info) == -1)
        {
            perror("wait");
        }
    }
    return child_info;
}
