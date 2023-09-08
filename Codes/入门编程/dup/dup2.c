/*
    #include <unistd.h>
    作用: 重定向文件描述符
    oldfd 指向 a.txt , newfd 指向 b.txt，调用函数成功后newfd 和b.txt做close，newfd指向了a.txt
    oldfd必须是一个有效的文件描述符，两者相同相当于什么都没做
    int dup2(int oldfd, int newfd);
*/

#include <stdio.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>


int main(){
    int fd = open("1.txt" , O_RDWR | O_CREAT , 0664);
    if(fd == -1){
        perror("open");
        return -1;
    }
    int fd1 = open("2.txt" , O_RDWR | O_CREAT, 0664);
    printf("fd: %d , fd1: %d\n",fd,fd1);

    int fd2 = dup2(fd,fd1);
    if(fd2 == -1){
        perror("dup2");
        return -1;
    }
    char * str = "hello world";
    //现在通过fd1去写数据，实际操作的是1.txt,而不是2.txt
    int ret = write(fd1,str,strlen(str));
    if (ret == -1)
    {
        perror("write");
        return -1;
    }
    printf("fd: %d , fd1: %d , fd2: %d\n",fd,fd1,fd2);
    

}