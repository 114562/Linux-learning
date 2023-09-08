#include <fcntl.h>
#include <signal.h>
#include <stdio.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

void myFun(int num) {
    printf("捕捉到的信号: %d\n", num);
    // 回收子进程的资源
    // while (1) {
    //     wait(NULL);
    // }

    while (1) {
        int ret = waitpid(-1, NULL, WNOHANG);
        if (ret > 0) {
            printf("child die, pid = %d\n", getpid());
        } else if (ret == 0) {
            // 说明还有子进程活着
            break;
        } else {
            // 没有子进程了
            break;
        }
    }
}

int main() {
    // 提前设置好阻塞信号集,阻塞SIGCHLD,因为有可能子进程很快结束,父进程还没有注册完信号捕捉
    sigset_t set;
    sigemptyset(&set);
    sigaddset(&set, SIGCHLD);
    sigprocmask(SIG_BLOCK, &set, NULL);

    // 创建一些子进程
    pid_t pid;
    for (int i = 0; i < 20; i++) {
        pid = fork();
        if (pid == 0) {
            break;
        }
    }
    if (pid > 0) {
        // 父进程

        // 捕捉子进程死亡时发送的sigchild信号
        struct sigaction act;
        act.sa_flags = 0;
        act.sa_handler = myFun;
        sigemptyset(&act.sa_mask);
        sigaction(SIGCHLD, &act, NULL);

        // 注册完信号捕捉以后,就解除阻塞
        sigprocmask(SIG_UNBLOCK, &set, NULL);

        while (1) {
            printf("parent process pid: %d\n", getpid());
            sleep(2);
        }
    } else if (pid == 0) {
        printf("child process oid: %d\n", getpid());
    }
    return 0;
}