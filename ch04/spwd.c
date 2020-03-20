#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <dirent.h>
#include <stdlib.h>
#include <string.h>
ino_t get_inode(char*);
void printpathto(ino_t);
void inum_to_name(ino_t, char*, int);

int main()
{
    printpathto(get_inode("."));
    putchar('\n');
    return 0;
}

void printpathto(ino_t this_inode)
{
    ino_t my_inode;
    char its_name[BUFSIZ];
    //判断是否为根目录（根目录的父节点和自身节点均指向自己）
    if (get_inode("..") != this_inode)    
    {
        //若不为根目录，转到父目录搜索名字
        chdir("..");                                     //up one dir
        inum_to_name(this_inode, its_name, BUFSIZ);     //get its name
        my_inode = get_inode(".");                      //print head
        printpathto(my_inode);                          //recursively
        printf("/%s", its_name);                        //now print name of this
    }
}

void inum_to_name(ino_t inode_to_find, char* namebuf, int buflen)
{
    DIR* dir_ptr;
    struct dirent* direntp;
    dir_ptr = opendir(".");
    if (dir_ptr == NULL)
    {
        perror(".");
        exit(1);
    }
    //search directory for a file with specified num
    //循环读入条目
    while ((direntp = readdir(dir_ptr)) != NULL)
    {
        //要搜索此节点的名字，必须要去它的父目录搜索
        //若搜索到的i-节点号跟要搜索的一样，即将此节点的名字存入数组中
        if (direntp->d_ino == inode_to_find)
        {
            strncpy(namebuf, direntp->d_name, buflen);
            namebuf[buflen-1] = '\0';   //?这一步是干啥的？？？？
            closedir(dir_ptr);  //关闭目录
            return ;
        }
    }
    //错误处理
    fprintf(stderr, "error looking for inum %ld\n", inode_to_find);
    exit(1);
}

ino_t get_inode(char* fname)
{
    struct stat info;
    if (stat(fname, &info) == -1)
    {
        fprintf(stderr, "Cannot stat ");
        perror(fname);
        exit(1);
    }
    return info.st_ino;
}
