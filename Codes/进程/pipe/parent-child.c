/*
    实现ps aux | grep xxx
    子进程: ps aus 子进程结束后,将数据发送给父进程
    父进程: 获取数据 过滤
    pipe()
    execlp()
    子进程将标准输出 stdout_fileno 重定向到管道的写段 dup2
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <unistd.h>
#include <wait.h>

int main() {
    // 创建一个管道
    int pipefd[2];
    int ret = pipe(pipefd);
    if (ret == -1) {
        perror("pipe");
        return -1;
    }
    // 创建子进程
    pid_t pid = fork();
    if (pid > 0) {
        // 关闭写端
        close(pipefd[1]);
        char buf[1024] = {0};

        // 从管道中读取数据
        int len = 1;
        while (len = read(pipefd[0], buf, sizeof(buf) - 1)) {
            // 过滤数据输出
            printf("%s", buf);
            memset(buf, 0, sizeof(buf));
        }
        wait(NULL);

    } else if (pid == 0) {
        // 关闭读端
        close(pipefd[0]);
        // 文件描述符重定向 stdout_fileno -> fd[1]
        dup2(pipefd[1], STDOUT_FILENO);
        // 执行ps aux
        execlp("ps", "ps", "aux", NULL);
        perror("execlp");
        exit(0);
    } else {
        perror("fork()");
        return -1;
    }

    return 0;
}