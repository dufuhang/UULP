#include <stdio.h>
#include <string.h>
#include <signal.h>
#include <stdlib.h>

//最大参数个数
#define MAXARGS 20
//每个参数最长
#define ARGLEN 100

int exe(char* []);
char* makestring(char*);

int main()
{
    char* arglist[MAXARGS + 1];     //an array of ptrs
    int numargs;                    //index into array
    char argbuf[ARGLEN];            //read stuff here
    numargs = 0;
    while (numargs < MAXARGS)
    {
        printf("Arg[%d]?", numargs);
        //读取成功且不是回车
        if (fgets(argbuf, ARGLEN, stdin) && *argbuf != '\n')
        {
            arglist[numargs++] = makestring(argbuf);
        }
        else
        {
            if (numargs > 0)
            {
                arglist[numargs] = NULL;    //close list
                exe(arglist);           //do it
                numargs = 0;                //reset
            }
        }
    }
    return 0;
}

//use execvp() to do it
int exe(char* arglist[])
{
    execvp(arglist[0], arglist);
    perror("execvp failed");
    exit(1);
}

//将命令转换为字符串存储
char* makestring(char* buf)
{
    char* cp;
    buf[strlen(buf) - 1] = '\0';
    cp = malloc(strlen(buf) + 1);
    if (cp == NULL)
    {
        fprintf(stderr, "no memory\n");
        exit(1);
    }
    strcpy(cp, buf);
    return cp;
}
