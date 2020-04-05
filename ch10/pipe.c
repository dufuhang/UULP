//两个进程都可以读写管道，但是当一个进程读，一个进程写的时候，效率是最高的
//父进程读，子进程写
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>

#define oops(m,x) { perror(m); exit(x); }

int main(int ac, char** av)
{
    int thepipe[2];     //two file dexcriptors
    int newfd;          //useful for pipes
    int pid;            //and the pid
    if (ac != 3)
    {
        fprintf(stderr, "usage: pipe cmd1 cmd2\n");
        exit(1);
    }
    //get a pipd
    if (pipe(thepipe) == -1)
    {
        oops("Cannot get a pipe", 1);
    }

    //now we have a pipe, now lets get two processes
    if ((pid = fork()) == -1)
    {
        oops("Cannot fork", 2);
    }

    //Right Here there are two processes
    //parent will read from pipe
    if (pid > 0)                //parent will exec av[2]
    {
        //关闭写端口。
        close(thepipe[1]);      //parent doesn't write to pipe
        //将读端复制给标准输入0
        if (dup2(thepipe[0], 0) == -1)
        {
            oops("could not redirect stdin", 3);
        }
        //然后将原有的读端口关闭
        close(thepipe[0]);      //stdin is duped, close pipe
        execlp(av[2], av[2], NULL);
        oops(av[2], 4);
    }

    //child execs av[1] and writes into pipe
    //关闭写端口。
    close(thepipe[0]);          //child doesn't read from pipe
    //将写端复制给标准输入0
    if (dup2(thepipe[1], 1) == -1)
    {
        oops("could not redirect stdout", 4);
    }
    //stdout is duped, close pip
    //然后将原有的读端口关闭
    close(thepipe[1]);
    execlp(av[1], av[1], NULL);
    oops(av[1], 5);
    return 0;
}
