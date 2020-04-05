//open -> close -> dup -> close
#include <stdio.h>
#include <fcntl.h>
#include <stdlib.h>
#include <unistd.h>

// #define CLOSE_DUP    //open close, dup, close
// #define USE_DEP2     //open, dup2, close
int main()
{
    int fd;
    int newfd;
    char line[100];
    //read and print three lines
    fgets(line, 100, stdin); printf(" %s", line);
    fgets(line, 100, stdin); printf(" %s", line);
    fgets(line, 100, stdin); printf(" %s", line);

    //redirect input
    fd = open("data", O_RDONLY);    //open disk file
#ifndef CLOSE_DUP
    close(0);
    newfd = dup(fd);                //copy open fd to 0
#else
    newfd = dup2(fd, 0);            //close 0, dup fd to 0
#endif
    if (fd != 0)
    {
        fprintf(stderr, "Could not open data as fd 0\n");
        exit(1);
    }
    close(fd);                      //close original fd
    //read and print three lines
    fgets(line, 100, stdin); printf(" %s", line);
    fgets(line, 100, stdin); printf(" %s", line);
    fgets(line, 100, stdin); printf(" %s", line);
    return 0;
}
