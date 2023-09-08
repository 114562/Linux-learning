/*
    #include <sys/time.h>
    功能: 设置定时器(闹钟),可以替代alarm,精度微秒us,实现周期性定时
    参数:
        - which: 定时器以什么时间计时
                ITIMER_REAL: 真实时间,时间到达,发送SIGALARM信号 常用
                ITIMER_VIRTUAL: 用户时间,时间到达,发送SIGVTALRM信号
                ITIMER_PROF: 以该进程在用户态和内核态下所消耗的时间来计算
        - new_value: 设置定时器时间属性,
            struct itimerval {              //定时器结构体
               struct timeval it_interval;  //每个阶段的时间,间隔时间
               struct timeval it_value;     //延迟多长时间执行定时器
           };

           struct timeval {           // 时间结构体
               time_t      tv_sec;    //秒
               suseconds_t tv_usec;   //微秒
           };
        - old_value: 记录上一次的定时的时间参数 ,一般不使用,传递NULL
    返回值:
        成功为0
        失败-1并设置错误号


    int setitimer(int which, const struct itimerval *restrict new_value,
                     struct itimerval *_Nullable restrict old_value);
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/time.h>
#include <unistd.h>

int main() {
    struct itimerval new_value;
    // 设置值 过3秒以后,每隔两秒定时一次
    new_value.it_value.tv_sec = 3;  // 设置延迟时间
    new_value.it_value.tv_usec = 0;
    new_value.it_interval.tv_sec = 2;  // 设置间隔时间
    new_value.it_interval.tv_usec = 0;

    int ret = setitimer(ITIMER_REAL, &new_value, NULL);  // 非阻塞的
    printf("定时器开始了\n");
    int i = 1;
    while (1) {
        printf("%d\n", i++);
        sleep(1);
    }

    if (-1 == ret) {
        perror("setitimer");
        exit(0);
    }
    getchar();

    return 0;
}