
/*
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/types.h>

//打开一个已经存在的文件
    参数：
        -pathname：要打开的文件路径 
        -flags  must include one of the following 
            access modes: O_RDONLY, O_WRONLY, or O_RDWR 这三个操作是互斥的
    返回值：
        -是文件描述符,若定位失败返回 -1 
        errno：属于Linux系统函数库，库里面的一个全局变量，记录的是最近的错误号
        perror：
    int open(const char *pathname, int flags);

#include <stdio.h>
作用：打印error对应的错误描述
    参数s：用户描述，比如hello，最终输出的内容是hello:xxx(实际的错误描述)
void perror(const char *s);

//创建一个文件
int open(const char *pathname, int flags, mode_t mode);

*/

#include <stdio.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <fcntl.h>
#include <unistd.h>

int main(){
    int fd = open("a.txt" , O_RDONLY);
    if(fd == -1){
        perror("open");
    }
    //关闭
    close(fd);
    return 0;
}

