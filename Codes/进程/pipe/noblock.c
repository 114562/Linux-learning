/*
    设置管道非阻塞
    int flags = fcntl(pipefd[0],GETFL); //获取原来的flag
    flags |= O_NONBLOCK
    fcntl(pipefd[0] , F_SETFL , flags);
*/

#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <unistd.h>

int main() {
    // 在fork之前创建管道
    int pipefd[2];
    int ret = pipe(pipefd);
    if (ret == -1) {
        perror("pipe");
        exit(0);
    }

    // 创建子进程
    pid_t pid = fork();
    if (pid > 0) {
        // 父进程
        printf("i am parent process, pid : %d\n", getpid());

        // 关闭写端
        close(pipefd[1]);

        // 从管道的读取端读取数据
        char buf[1024] = {0};
        // 设置读端为非阻塞
        int flags = fcntl(pipefd[0], F_GETFL);
        flags |= O_NONBLOCK;
        fcntl(pipefd[0], F_SETFL, flags);
        while (1) {
            int len = read(pipefd[0], buf, sizeof(buf));
            printf("len: %d\n", len);
            printf("parent recv : %s, pid : %d\n", buf, getpid());
            sleep(2);
        }

    } else if (pid == 0) {
        // 子进程
        printf("i am child process, pid : %d\n", getpid());
        // 关闭读端
        close(pipefd[0]);
        char buf[1024] = {0};
        while (1) {
            // 向管道中写入数据
            char* str = "hello,i am child";
            write(pipefd[1], str, strlen(str));
            memset(buf,0,sizeof(buf));
            sleep(10);
        }
    }
    return 0;
}
