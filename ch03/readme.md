在ls2.c的文件中，若将main函数写成如下形式：
```cpp
int main(int argc, char** argv)
{
    ...
    return 0;
}
```
程序会报错：
``
error adding symbols: Bad value 
collect2: error: ld returned 1 exit status
```
以及，ls2.c源文件会**突然消失**，还不知道为什么==
