# Problems
在ls2.c的文件中，若将main函数写成如下形式：
```cpp
int main(int argc, char** argv)
{
    ...
    return 0;
}
```
程序会报错：
```
error adding symbols: Bad value 
collect2: error: ld returned 1 exit status
```

以及，ls2.c源文件会**突然消失**，还不知道为什么==

# Summary
- ls:列出目录里的文件及详细信息
- 如何操作目录？

    1.`opendir()`

    2.`readdir()`

    3.`closedir()`
- 编写ls命令（ls1）：
```
//主要算法流程
main()
    opendir()
    while (readdir())
        printf d_name
    closedir();
```
- ls1只能够打印目录里文件的名字，还需要打印文件的模式，所属用户，所属组，修改时间等信息，如何实现（ls2）？

    1.目录只包含文件名，文件的详细信息需要从另外的途径获得。使用:
    
        ```
        int result = stat(char* fname, struct stat* bufp)
        
        返回值：
        -1 遇到错误
        0  成功返回
        ```
        将文件信息复制到bufp指针指向的数据结构中。
        该数据结构中包含如下信息：
        
        ```
        st_mode     文件类型和许可权限
        st_uid      用户所有者的ID
        st_gid      所属组的ID
        st_size     所占字节数
        st_nlink    文件链接数
        st_mtime    文件最后修改时间
        st_atime    文件最后访问时间
        st_ctime    文件属性最后改变时间
        st_ino      文件的i节点号
        ```
    
    
    2.获取的文件详细信息中，模式字段和用户/组ID都不是我们想要的字符串形式，如何转换？

        (1)如何转换模式字段？使用掩码技术：将不想关的位通过位与运算置为0.在<sys/sst.h>中有如下字段：
            #define S_IFMT	0170000	//将文件的mode和它做与位运算，见得到的值与以下的值比较
            #define S_IFREG 0100000	//regular
            #define S_IFDIR	0040000	//directory
            #define S_IFBLK	0060000	//block special
            #define S_IFCHR	0020000	//character special
            #define S_IFIFO	0010000	//fifo
            #define S_IFLNK	0120000	//symbolic link
            #Define S_IFSOCK	0140000	//socket
    

    3.将用户/组ID转换成字符串

        (1)/etc/passwd包含用户列表。getpwuid()接收一个UID，返回一个指向struct passwd的指针，该结构定义在/usr/include/pwd.h中：
            struct passwd
            {
                char* pw_name;      //username
                char* pw_passwd;    //password
                _uid_t pw_uid;      //user id
                _gid_t pw_gid;      //group id
                char* pw_gecos;     //real name
                char* pw_dir;       //home directory
                char* pw_shell;     //shell program
            };
            //其中用户可属于多个组，passwd文件只列出了主组

        (2)/etc/group是组的列表。getgrgid()接收一个GID，返回一个指向struct group的指针，该数据结构定义在grp.h文件中：
            struct group {
               char   *gr_name;     //group name
               char   *gr_passwd;   //group password
               gid_t   gr_gid;      //group ID
               char  **gr_mem;      //NULL-terminated array of pointers to names of group members
           };

- 三个特殊位：

    1.set-user-ID

    2.set-group-ID

    3.sticky

- 设置和修改文件属性

```c
//修改文件的许可权限和特殊属性
int result = chmod(char* path, mode_t mode);

//返回值：
//-1 遇到错误
//0  成功返回
```

```c
//修改文件所有者和组
int chown(char* path, uid_t owner, gid_t group);
//path 文件名
//owner 新的文件所有者ID
//group 新的组ID

//返回值：
//-1 遇到错误
//0  成功返回
```

```c
//修改文件最后修改时间和最后访问时间
int utime(char* path, struct utimbuf* newtimes);

//newtimes 指向结构变量utimbuf的指针，在utime.h文件中
//返回值：
//-1 遇到错误
//0  成功返回
```

```c
//修改文件名或移动文件的位置
int result = rename(char* old, char* new);

//old 原来的文件名或者目录名
//new 新的文件名或者目录名

//返回值：
//-1 遇到错误
//0  成功返回
```
