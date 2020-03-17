#include <stdio.h>
#define PAGELEN 24
#define LINELEN 512
void do_more(FILE* fp);
int see_more();
int main(int ac, char* av[])
{
    //文件指针
    FILE* fp;
    //若没有命令行参数，从输入流中读取
    if ( ac == 1)
    {
        do_more(stdin);
    }
    else
    {
        while ( --ac )
        {
            if ((fp = fopen(* ++av, "r")) != NULL)
            {
                do_more(fp);
                fclose(fp);
            }
            else
            {
                exit(1);
            }
        }
    }
    return 0;
}
//read PAGELEN lines, then call see_more() for further instrunctions
void do_more(FILE* fp)
{
    char line[LINELEN];
    int num_of_lines = 0;
    //
    int see_more(), reply;
    //more input?
    while (fgets(line, LINELEN, fp))
    {
        //打印了24行，询问用户是否继续
        if (num_of_lines == PAGELEN)
        {
            //调用see_more()函数询问用户
            reply = see_more();
            //退出
            if (reply == 0)
            {
                break;
            }
            num_of_lines -= reply;
        }
        if (fputs(line, stdout) == EOF)
        {
            exit(1);
        }
        num_of_lines++;
    }
}
//print message, wait for response, return # of lines to advance
// q means no, space means yes, CR means one line
int see_more()
{
    int c;
    //?
    printf("\33[7m more? \33[m");
    while ((c = getchar()) != EOF)
    {
        if (c == 'q')
        {
            return 0;
        }
        if (c == ' ')
        {
            //再打印一页（24行）
            return PAGELEN;
        }
        if (c == '\n')
        {
            //再打印一行
            return 1;
        }
    }
    return 0;
}
