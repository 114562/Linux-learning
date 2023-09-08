/*
    #include <unistd.h>

    extern char **environ;
    参数
    - path: 需要指定的执行的文件的路径或者名称 推荐使用绝对路径
    - arg: 执行可执行文件所需要的参数列表
        第一个参数一般没有什么作用，为了方便，一般写执行程序的名称
        从第二个参数开始往后，就是程序执行所需要的参数列表
        参数最后需要以NULL结束(哨兵)
    返回值
        成功不返回(因为调用成功之后用户区已经交换，收取返回值也没有意义了)
        只有调用失败才会返回-1并且设置errno
    int execl(const char *pathname, const char *arg, ...);
*/
#include <stdio.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

int main() {
    // 创建一个子进程,在子进程中执行exec函数族中的函数
    pid_t pid = fork();
    if (pid > 0) {
        printf("i am parent process, pid: %d\n", getpid());
        sleep(1);
    } else if (pid == 0) {
        execl("hello", "hello", NULL);
        printf("i am child process, pid = %d\n", getpid());  // 在子进程中这些代码都会被替代掉，不会执行,执行hello可执行文件中的代码
    }
    for (int i = 0; i < 3; i++) {
        printf("i = %d , pid = %d\n", i, getpid());
    }
}