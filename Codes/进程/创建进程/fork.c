/*
    #include <unistd.h>
    作用: 用于创建子进程
    返回值: fork的返回值会返回两次:一次是在父进程中，一次是在子进程中
        在父进程中返回创建的子进程的ID
        在子进程中返回0
        如何父进程和子进程，通过fork的值返回
        在父进程中返回-1表示创建子进程失败，冰鞋设置errno
    什么时候会创建失败呢？
        1.当前系统的进程数已经达到了系统规定的上限，这时errno的值被设置为EAGAIN
        2.系统内存不足时，这时errno的值被设置为ENOMEM
    pid_t fork(void);

*/

#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <unistd.h>

int main() {
    // 创建子进程
    pid_t pid = fork();
    // 判断是父进程还是子进程
    if (pid > 0) {
        printf("pid : %d\n", pid);
        // 当前是父进程,返回的是创建的子进程的进程号
        printf("i am parent process , pid : %d , ppid: %d\n", getpid(), getppid());

    } else if (pid == 0) {
        // 当前是子进程
        printf("i am child process , pid : %d , ppid : %d\n", getpid(), getppid());
    }
    for (int i = 0; i < 3; i++) {
        printf("i : %d\n", i);
        sleep(1);
    }

    return 0;
}