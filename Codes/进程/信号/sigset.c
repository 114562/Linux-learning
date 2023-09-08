/*

    #include <signal.h>
    以下信号集相关函数都是对自定义的信号集进行操作

    int sigemptyset(sigset_t *set);
        功能:清空信号集中的数据，将信号集中的所有的标志位置为0
        参数:
            - set:传出参数，是我们需要操作的信号集
        返回值:
            成功返回0
            失败返回-1
    int sigfillset(sigset_t *set);
        功能:将信号集中所有的标志位置为1
        参数:
            - set:传出参数，是我们需要操作的信号集
        返回值:
            成功返回0
            失败返回-1
    int sigaddset(sigset_t *set, int signum);
    功能:设置信号集中的某一个信号对应的标志位为1,表示阻塞这个信号
        参数:
            - set:传出参数，是我们需要操作的信号集
            - signum: 需要设置阻塞的信号
        返回值:
            成功返回0
            失败返回-1
    int sigdelset(sigset_t *set, int signum);
        功能:设置信号集中的某一个信号对应的标志位为0,表示不阻塞这个信号
        参数:
            - set:传出参数，是我们需要操作的信号集
            - signum: 需要设置不阻塞的信号
        返回值:
            成功返回0
            失败返回-1
    int sigismember(const sigset_t *set, int signum);
        功能:判断某个信号是否阻塞
        参数:
            - set:是我们需要操作的信号集
            - signum: 需要判断的信号
        返回值:
            1:signum阻塞
            0:signum不阻塞
            -1:失败
*/

#include <fcntl.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/signal.h>
#include <sys/types.h>

int main() {
    // 创建一个信号集
    sigset_t set;
    // 清空信号集
    sigemptyset(&set);

    // 判断SIGINT是否在信号集set里
    int ret = sigismember(&set, SIGINT);
    if (ret == 0) {
        printf("SIGINT不阻塞\n");
    } else if (ret == 1) {
        printf("SIGINT阻塞\n");
    }
    // 添加几个信号到信号集中
    sigaddset(&set, SIGINT);
    sigaddset(&set, SIGQUIT);
    // 判断SIGQUIT是否在信号集中
    ret = sigismember(&set, SIGQUIT);
    if (ret == 0) {
        printf("SIGQUIT不阻塞\n");
    } else if (ret == 1) {
        printf("SIGQUIT阻塞\n");
    }

    // 从信号集中删除信号
    sigdelset(&set, SIGQUIT);
    ret = sigismember(&set, SIGQUIT);
    if (ret == 0) {
        printf("SIGQUIT不阻塞\n");
    } else if (ret == 1) {
        printf("SIGQUIT阻塞\n");
    }

    return 0;
}