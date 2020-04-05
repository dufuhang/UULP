#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/wait.h>

int main()
{
    int pid;
    int fd;

    printf("About to run who into a file\n");

    //create a new process or quit
    if ((pid = fork()) == -1)
    {
        perror("fork()"); 
        exit(1);
    }
    //child does the work
    if (pid == 0)
    {
        close(1);
        fd = creat("userlist", 0644);
        execlp("who", "who", NULL);
        perror("execlp");
        exit(1);
    }
    //parent waits then reports
    if (pid != 0)
    {
        wait(NULL);
        printf("Done running who. results in userlist\n");
    }
    return 0;
}
