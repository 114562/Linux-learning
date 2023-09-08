/*
    #include <unistd.h>
    功能: 创建一个匿名管道用来进程间的通信
    参数
        - int[2] 这个数组是一个传出参数
          pipe[0] 对应的是管道的读端
          pipe[1] 对应的是管道的写端
    返回值
        成功返回0
        失败返回-1并设置errno
    注意:匿名管道只能用于具有关系的进程间的通信
    int pipe(int pipefd[2]);
    管道默认是阻塞的，如果管道中没有数据，read阻塞，通过管道满了，write阻塞
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

int main() {
    // 创建管道,在fork()之前创建管道
    int pipefd[2];
    int ret = pipe(pipefd);
    if (ret == -1) {
        perror("pipe");
        exit(0);
    }
    // 创建子进程kenqing
    pid_t pid = fork();
    if (pid > 0) {
        // 父进程
        // 从管道的读取端读取数据
        char buf[1024] = {0};
        int len = read(pipefd[0], buf, sizeof(buf));
        printf("parent recv : %s , pid : %d\n", buf, getpid());
    } else if (pid == 0) {
        // 子进程
        sleep(5);
        char* str = "hello , i am child";
        write(pipefd[1], str, strlen(str));
    }
    return 0;
}