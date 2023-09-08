/*
   #include <unistd.h>

    [[noreturn]] void _exit(int status);

     #include <stdlib.h>

    [[noreturn]] void exit(int status);

    参数
        - status: 是进程退出时的一个状态信息。父进程在回收子进程资源的时候可以获得
*/
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main() {
    printf("hello \n");  // \n刷新缓冲区所以会打印出来
    printf("world");     // 这里没有\n ,  先把数据放到缓冲区中，_exit()不会刷新缓冲区
    // exit(0); //标准c库exit函数会刷新缓冲区
    _exit(0);
    return 0;
}