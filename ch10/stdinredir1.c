//最低可用文件描述符(Lowest Available fd)
#include <stdio.h>
#include <fcntl.h>
#include <stdlib.h>
#include <unistd.h>

int main()
{
    int fd;
    char line[100];
    //read and print three lines
    fgets(line, 100, stdin); printf(" %s", line);
    fgets(line, 100, stdin); printf(" %s", line);
    fgets(line, 100, stdin); printf(" %s", line);

    //redirect input
    close(0);
    fd = open("/etc/passwd", O_RDONLY);
    if (fd != 0)
    {
        fprintf(stderr, "Could not open data as fd 0\n");
        exit(1);
    }

    //read and print three lines
    fgets(line, 100, stdin); printf(" %s", line);
    fgets(line, 100, stdin); printf(" %s", line);
    fgets(line, 100, stdin); printf(" %s", line);
    return 0;
}
