#include <stdio.h>
#include <stdlib.h>
#include <termios.h>

#define oops(s, x) { perror(s); exit(x); }

int main(int ac, char* av[])
{
    struct termios info;
    if (ac == 1)
    {
        exit(1);
    }
    if (tcgetattr(0, &info) == -1)
    {
        oops("tcgetattr", 1);
    }
    if (av[1][0] == 'y')
    {
        info.c_lflag |= ECHO;   //turn on bit
    }
    else
    {
        info.c_lflag &= ~ECHO;  //turn off bit
    }
    if (tcsetattr(0, TCSANOW, &info) == -1) //set attribs
    {
        oops("tcsetattr", 2);
    }
}
