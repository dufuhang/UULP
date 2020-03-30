#include <stdio.h>

int main()
{
    char* arglist[3];
    //第一个元素为程序的名称
    arglist[0] = "ls";
    arglist[1] = "-l";
    //最后一个元素必须是null
    arglist[2] = 0;
    printf("*** About to exec ls -l\n");
    execvp("ls", arglist);
    printf("*** ls is done.bye\n");
    return 0;
}
