#include <stdio.h>
int main(int ac, char* av[])
{
    int i;
    printf("Number of args: %d, Args are :\n", ac);
    for (int i = 0; i < ac; i++)
    {
        printf("args[%d] %s\n", i, av[i]);
    }
    fprintf(stderr, "This message is sent to stderr.\n");
    return 0;
}
