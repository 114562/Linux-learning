/*
    #include <signal.h>
    功能: 将自定义信号集中的数据设置到内核中(设置阻塞,解除阻塞,替换)
    参数
        - how: 如何对内核中阻塞的信号集进行处理
            SIG_BLOCK:将用户设置的阻塞信号集添加到内核中,内核中原来的数据不变
                      假设内核中吗，默认的阻塞信号集是mask: mask | set
            SIG_UNBLOCK: 根据用户设置的数据,对内核中的数据进行解除阻塞
                        mask &= ~set
            SIG_SETMASK: 覆盖内核中原来的值
        - set: 已经初始化好的用户自定义的信号集
        -oldset: 保存设置之前的内核中的阻塞信号集的状态，一般不适用，NULL
    返回值
        成功返回 0
        失败返回 -1 设置错误号EFAULT、EINVAL
    int sigprocmask(int how, const sigset_t *_Nullable restrict set,
                                  sigset_t *_Nullable restrict oldset);

    功能: 获取内核中的未决信号集
    参数:
        -set: 传出参数,保存的是未决中的传出信号集中的信息
    int sigpending(sigset_t *set);
*/

#include <fcntl.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

// 目的:将所有的常规信号(1-31)的未决状态打印到屏幕
int main() {
    sigset_t set;
    sigemptyset(&set);
    // 设置2和3号信号阻塞
    sigaddset(&set, SIGINT);
    sigaddset(&set, SIGQUIT);
    // 修改内核中的阻塞信号集
    sigprocmask(SIG_BLOCK, &set, NULL);
    int n = 0;
    while (1) {
        // 获取当前未决信号集的数据
        sigset_t pendingset;
        sigemptyset(&pendingset);
        sigpending(&pendingset);

        // 遍历
        for (int i = 1; i <= 32; i++) {
            if (sigismember(&pendingset, i) == 1) {
                printf("1");
            } else if (sigismember(&pendingset, i) == 0) {
                printf("0");
            } else {
                perror("sigismenber");
                exit(0);
            }
        }
        printf("\n");
        sleep(1);
        n++;
        if(n == 10){
            //解除阻塞
            sigprocmask(SIG_UNBLOCK, &set, NULL);
            sigprocmask(SIG_UNBLOCK, &set, NULL);
        }

    }

    return 0;
}