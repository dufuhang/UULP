#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <string.h>
#include <stdlib.h>


void header(FILE* fp, char* content_typel);
void cannot_do(int fd);
void do_404(char* item, int fd);
bool isadir(char* f);
bool not_exist(char* f);
void do_ls(char* dir, int fd);
char* file_type(char* f);
bool ends_in_cgi(char* f);
void do_exec(char* prog, int fd);
void do_cat(char* f, int fd);
void read_til_crnl(FILE* fp);
void process_rq(char* rq, int fd);

int main(int ac, char* av[])
{
    int sock, fd;
    FILE* fpin;
    char request[BUFSIZ];
    if (ac == 1)
    {
        fprintf(stderr, "usage: ws portnum\n");
        exit(1);
    }
    sock = make_server_socket(atoi(av[1]));
    if (sock == -1)
    {
        exit(2);
    }
    //main loop
    while (1)
    {
        //take a call and buffer
        fd = accept(sock, NULL, NULL);
        fpin = fdopen(fd, "r");

        //read request
        fgets(request, BUFSIZ, fpin);
        printf("got a call: request = %s", request);
        read_til_crnl(fpin);

        //do what client asks
        process_rq(request, fd);
        fclose(fpin);
    }
    return 0;
}

//持续读取，直到文件尾或者读到一个空行
void read_til_crnl(FILE* fp)
{
    char buf[BUFSIZ];
    while (fgets(buf, BUFSIZ, fp) != NULL && strcmp(buf, "\r\n") != 0);
}

//执行请求，并将响应写入fd
//在一个新进程（子进程）中处理请求
//rq是http命令：GET ... HTTP/1.0
void process_rq(char* rq, int fd)
{
    char cmd[BUFSIZ], arg[BUFSIZ];

    //创建新进程，若不是child则返回
    if (fork() != 0)
    {
        return ;
    }
    //args由./开始
    strcpy(arg, "./");
    if (sscanf(rq, "%s%s", cmd, arg+1) != 2)
    {
        return ;
    }
    if (strcpy(cmd, "GET") != 0)
    {
        cannot_do(fd);
    }
    else if (not_exist(arg))
    {
        do_404(arg, fd);
    }
    else if (isadir(arg))
    {
        do_ls(arg, fd);
    }
    else if (ends_in_cgi(arg))
    {
        do_exec(arg, fd);
    }
    else
    {
        do_cat(arg, fd);
    }
}

//the reply header thing: all functions need one
//if content_type is NULL, then don't send content type
void header(FILE* fp, char* content_type)
{
    fprintf(fp, "HTTP/1.0 200 OK\r\n");
    if (content_type)
    {
        fprintf(fp, "Content-type: %s\r\n", content_type);
    }
}

//simplr functions :
//  cannot_do()   unimplemented HTTP command
//  and do_404()    no such object
void cannot_do(int fd)
{
    FILE* fp = fdopen(fd, "w");
    fprintf(fp, "HTTP/1.0 501 Not Implemented\r\n");
    fprintf(fp, "Content-type:text/plain\r\n");
    fprintf(fp, "\r\n");

    fprintf(fp, "That command is not yet implemented\r\n");
    fclose(fp);
}

void do_404(char* item, int fd)
{
    FILE* fp = fdopen(fd, "w");
    fprintf(fp, "HTTP/1.0 404 Not Found\r\n");
    fprintf(fp, "Content-type:text/plain\r\n");
    fprintf(fp, "\r\n");

    fprintf(fp, "The item you requested: %s\r\nis not found\r\n", item);
    fclose(fp);
}

//the directory listing section
//判断是否是目录
bool isadir(char* f)
{
    struct stat info;
    return (stat(f, &info) != -1 && S_ISDIR(info.st_mode));
}
bool not_exist(char* f)
{
    struct stat info;
    return (stat(f, &info) == -1);
}
void do_ls(char* dir, int fd)
char* file_type(char* f)
bool ends_in_cgi(char* f)
void do_exec(char* prog, int fd)
void do_cat(char* f, int fd)
