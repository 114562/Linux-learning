/*
    #include <signal.h>
    功能:给某个进程pid发送某个信号sig 。任何进程发送任何信号
    参数:
        - pid: 需要发送给的进程id
            pid > 0: 将信号发送给指定的进程
            pid = 0: 将信号发送给当前进程组
            pid = -1: 将信号发送给每一个有权限接受这个信号的进程
            pid < -1: 这个pid是某个进程组的PID取反,0表示不发送任何信号
        - sig: 需要发送的信号的编号或者是宏值(两者等价,建议使用宏值)
    int kill(pid_t pid, int sig);

    #include <signal.h>
    功能:给当前进程发送信号,等同于kill(getpid(),sig);
    参数
        -sig: 表示要发送的信号
    返回值
        成功返回0
        失败返回非0
    int raise(int sig);

    #include <stdlib.h>
    功能:发送SIGABRT信号给当前进程,杀死当前进程 等同于kill(getpid(),SIGABRT);
    void abort(void);

*/
#include <signal.h>
#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <unistd.h>

int main() {
    pid_t pid = fork();
    if (pid == 0) {
        int i = 0;
        for (i = 0; i < 5; i++) {
            printf("child process%d\n", i);
            sleep(1);
        }
    } else if (pid > 0) {
        printf("parent process\n");
        sleep(2);
        printf("parent kills child process\n");
        kill(pid, SIGINT);
    }
    return 0;
}