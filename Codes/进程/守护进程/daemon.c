/*
    写一个守护进程,每隔两秒获取系统时间,将这个时间写入到磁盘文件中
*/

#include <fcntl.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/time.h>
#include <sys/types.h>
#include <time.h>
#include <unistd.h>

void work(int num) {
    // 捕捉到信号之后，获取系统时间，写入磁盘
    time_t tm = time(NULL);
    // 转换格式
    struct tm* loc = localtime(&tm);
    // char buf[1024];
    // sprintf(buf, "%d-%d-%d %d:%d:%d\n", loc->tm_year, loc->tm_mon, loc->tm_mday, loc->tm_hour, loc->tm_min, loc->tm_sec);
    // printf("%s\n", buf);

    char* str = asctime(loc);
    int fd = open("time.txt", O_RDWR | O_CREAT | O_APPEND, 0664);
    write(fd, str, strlen(str));
    close(fd);
}

int main() {
    pid_t pid = fork();
    if (pid > 0) {
        exit(0);
    }
    // 2.将子进程提升为会话
    setsid();

    // 3.设置掩码
    umask(022);

    // 4.更改工作目录
    chdir("/home/yjx");

    // 5.关闭,重定向文件描述符
    int fd = open("/dev/null/", O_RDWR);
    dup2(fd, STDIN_FILENO);
    dup2(fd, STDOUT_FILENO);
    dup2(fd, STDERR_FILENO);

    // 6.业务逻辑

    // 捕捉定时信号
    struct sigaction act;
    act.sa_flags = 0;
    act.sa_handler = work;
    sigemptyset(&act.sa_mask);
    sigaction(SIGALRM, &act, NULL);
    // 创建定时器
    struct itimerval val;
    val.it_interval.tv_sec = 2;
    val.it_interval.tv_usec = 0;
    val.it_value.tv_sec = 2;
    val.it_value.tv_usec = 0;

    setitimer(ITIMER_REAL, &val, NULL);
    while (1)
        sleep(10);  // 死循环

    return 0;
}