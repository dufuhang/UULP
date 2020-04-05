#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

#define CHILD_MESS  "I want a cookie\n"
#define PAR_MESS    "testing...\n"
#define oops(m, x)  {perror(m); exit(x);}

int main()
{
    int pipefd[2];  //the pipe
    int len;        //for write
    char buf[BUFSIZ];   //for read
    
    int read_len;

    if (pipe(pipefd) == -1)
    {
        oops("cannot get a pipe", 1);
    }
    //child writes to pipe every 5 seconds
    switch (fork())
    {
        case -1:
            oops("cannot fork", 2);
        case 0:
            len = strlen(CHILD_MESS);
            while (1)
            {
                if (write(pipefd[1], CHILD_MESS, len) != len)
                    oops("write", 3);
                sleep(5);
            }
        default:
            len = strlen(PAR_MESS);
            while (1)
            {
                if (write(pipefd[1], PAR_MESS, len) != len)
                    oops("write", 4);
                sleep(1);
                read_len = read(pipefd[0], buf, BUFSIZ);
                if (read_len <= 0)
                    break;
                write(1, buf, read_len);
            }
    }
    return 0;
}
