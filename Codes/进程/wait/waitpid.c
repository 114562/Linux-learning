/*
    #include <sys/wait.h>
    功能:回收指定进程号的子进程，可以设置是否阻塞
    参数:
        - pid:
            pid > 0 表示某个子进程的pid
            pid = 0 回收当前进程组的所有子进程
            pid = -1 表示回收所有的子进程,相当于wait()
            pid < -1 某个进程组的组id,绝对值代表该进程组的id，回收指定进程组中的子进程
        - wstatus : 进程退出时的状态信息，传入的是一个int类型的地址，传出参数
        - options:
            0: 阻塞
            WNOHANG: 非阻塞
    返回值
        > 0 : 返回子进程的id
        = 0 : options = WNOHANG, 表示还有子进程或者
        = -1: 错误或者没有子进程了
    pid_t waitpid(pid_t pid, int *_Nullable wstatus, int options);
*/

#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

int main() {
    // 有一个父进程，创建5个子进程（兄弟）
    pid_t pid;

    // 创建5个子进程
    for (int i = 0; i < 5; i++) {
        pid = fork();
        if (pid == 0) {
            break;
        }
    }

    if (pid > 0) {
        // 父进程
        while (1) {
            printf("parent, pid = %d\n", getpid());

            int st;
            // int ret = waitpid(-1, &st, 0); // 阻塞的情况
            int ret = waitpid(-1, &st, WNOHANG);  // 非阻塞

            if (ret == -1) {
                break;
            }
            if (ret == 0) {
                // 还有子进程存
                printf("还有子进程存在\n");
                sleep(1);
                continue;
            }
            if (ret > 0) {
                if (WIFEXITED(st)) {
                    // 是不是正常退出
                    printf("退出的状态码：%d\n", WEXITSTATUS(st));
                }
                if (WIFSIGNALED(st)) {
                    // 是不是异常终止
                    printf("被哪个信号干掉了：%d\n", WTERMSIG(st));
                }

                printf("child die, pid = %d\n", ret);
            }
            sleep(1);
        }

    } else if (pid == 0) {
        // 子进程
        while (1) {
            printf("child, pid = %d\n", getpid());
            sleep(1);
        }

        exit(0);
    }

    return 0;  // exit(0)
}