/*
     #include <sys/stat.h>
    参数
        - pathname 操作文件的路径
        - statbuf 结构体变量，传出参数，用于保存获取到的文件的信息
    返回值
        成功 0
        失败 -1 设置errno
       int stat(const char *restrict pathname,
                struct stat *restrict statbuf);
        作用：获取一个文件相关的一些信息
       int lstat(const char *restrict pathname,
                struct stat *restrict statbuf);
        与上一个函数的区别：该函数获取的是软连接文件的信息，而上一个函数获取的是原函数的信息
        
*/

#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <fcntl.h>

int main(){
    struct stat statbuf;
    int ret = stat("a.txt" , &statbuf);
    if(ret == -1){
        perror("stat");
        return -1;
    }
    printf("size: %ld\n ",statbuf.st_size);
    return 0;
}