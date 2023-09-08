/*
    #include <signal.h>
    功能：检查或者改变信号的处理。信号捕捉
    参数
        - signum: 需要捕捉的信号的编号或者宏值(信号的名称)
        -act:捕捉到信号之后相应的处理动作
        -oldact:一般为NULL,上一次信号捕捉相关的设置
    返回值
        成功返回0,失败返回-1
    int sigaction(int signum, struct sigaction * act,
                struct sigaction * oldact);

    struct sigaction {
            void     (*sa_handler)(int); //函数指针,只想的函数就是信号捕捉到之后的处理函数
            void     (*sa_sigaction)(int, siginfo_t *, void *); //不常用,
            sigset_t   sa_mask; //临时阻塞信号集，在信号捕捉函数执行过程中,临时阻塞某些信号
            int        sa_flags; //   使用哪一个信号对捕捉到的信号进行处理 0表示使用sa_handler,也可以是SA_SIGINFO表示使用sa_sigaction
            void     (*sa_restorer)(void); //被废弃掉,NULL
    };

*/

#include <fcntl.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/time.h>
#include <unistd.h>

void myalarm(int num) {
    printf("捕捉到了信号的编号是%d\n", num);
    printf("这是回调函数\n");
}

int main() {
    struct sigaction act;
    act.sa_flags = 0;
    act.sa_handler = myalarm;
    // 清空临时阻塞信号集
    sigemptyset(&act.sa_mask);

    // 注册信号捕捉
    sigaction(SIGALRM, &act, NULL);
    // typedef void (*sighandler_t)(int); 函数指针 sighandler_t是函数指针的名称

    struct itimerval new_value;
    new_value.it_interval.tv_sec = 2;
    new_value.it_interval.tv_usec = 0;
    new_value.it_value.tv_sec = 3;
    new_value.it_value.tv_usec = 0;
    int ret = setitimer(ITIMER_REAL, &new_value, NULL);
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