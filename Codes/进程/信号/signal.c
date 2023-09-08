/*
    #include <signal.h>
    typedef void (*sighandler_t)(int);
    功能：设置某个信号的捕捉行为
    参数：
        - signum: 要捕捉的信号
        - handler: 捕捉到信号后如何处理
            SIG_IGN: 忽略信号
            SIG_DFL: 默认信号,使用信号默认行为
            回调函数: 这个函数是内核调用,程序员只负责写,捕捉到信号后如何去处理信号
                -需要程序员实现并且提前准备好，函数类型根据实际需求，看函数指针定义
                -不需要程序员调用,而是当信号产生时由内核调用
                -函数指针是实现回调的手段。函数实现之后,将函数名放到函数指针的位置就可以了
    返回值:
        成功返回上一次注册的信号处理函数的地址,第一次调用返回NULL
        失败返回宏SIG_ERR,设置错误号
    sighandler_t signal(int signum, sighandler_t handler);

    注意: The signals SIGKILL and SIGSTOP cannot be caught or ignored.
*/

#include <fcntl.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/time.h>
#include <unistd.h>

void myalarm(int num){
    printf("捕捉到了信号的编号是%d\n",num);
    printf("这是回调函数\n");
}


int main() {
    //注册信号捕捉
    signal(SIGALRM,myalarm);
    //typedef void (*sighandler_t)(int); 函数指针 sighandler_t是函数指针的名称


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