/*
    #include <unistd.h>
    功能:设置定时器
    参数:
        - seconds:函数调用开始倒计时，当倒计时为0时，函数给当前进程发送一个信号:SIGALARM,进程终止
                  单位:秒 若参数为0，定时器无效
                  取消定时器：通过alarm(0)
    返回值:
    - 之前有定时器，返回之前定时器剩余时间
    - 之前没有定时器，返回0
    -SIGALARML: 默认终止当前的进程，每一个进程都有且仅有唯一的一个定时器
                重复调用会刷新定时器,返回的是之前定时器剩余的时间
    unsigned int alarm(unsigned int seconds);
    该函数是不阻塞的
*/

#include <stdio.h>
#include <unistd.h>

int main() {
    int seconds = alarm(5);
    printf("第一次调用 seconds: %d\n", seconds);
    sleep(2);
    seconds = alarm(5);
    printf("休眠两秒后第二次调用 seconds: %d\n", seconds);
    while (1) {
        ;
    }
    return 0;
}