/*
    #include <unistd.h>
    缩减或者扩展文件的尺寸至指定的大小
    参数
        - path 需要修改的文件的路径
        - length 需要最终文件变成的大小
    int truncate(const char *path, off_t length);
*/
#include <stdio.h>
#include <unistd.h>

int main() {
    int ret = truncate("b.txt", 5);
    if (ret == -1) {
        perror("truncate");
        return -1;
    }
    return 0;
}