/*
    创建FIFO文件
    1.通过命令: mkfifo
    2.通过函数SIS
       #include <sys/stat.h>
       #include <sys/types.h>
        参数
            - pathname 管道名称的路径
            - mode 文件的权限 和open的mode是一样的 是一个八进制的数
        返回值
            成功返回 0
            失败返回 -1
       int mkfifo(const char *pathname, mode_t mode);

*/

#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <sys/types.h>

int main() {
    int ret = mkfifo("/home/yjx/网络编程/fifo1", 0664);
    if (ret == -1) {
        perror("mkfifo");
        exit(0);
    }
    return 0;
}