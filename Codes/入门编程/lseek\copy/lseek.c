/*
    #include <unistd.h>
    参数
        - fd 文件描述符
        - offset 偏移量
        - whence  
       SEEK_SET
              The file offset is set to offset bytes.
              设置文件指针的偏移量

       SEEK_CUR
              The file offset is set to its current location plus offset bytes.
              设置偏移量：当前位置 + 第二个参数offset的值

       SEEK_END
              The file offset is set to the size of the file plus offset bytes.
              设置偏移量：文件大小 + 第二个offset的值 
    返回值：返回文件指针的位置  
    off_t lseek(int fd, off_t offset, int whence);
    函数的作用
        1.移动文件指针到头文件
            lseek(fd,0,SEEK_SET);
        2.获取当前文件指针的位置
            lseek(fd,0,SEEK_CUR);
        3.获取文件长度
            lseek(fd,0,SEEK_END);
        4.拓展文件的长度，当前文件10b，增加100字节
            lseek(fd,100,SEEK_END);
        
*/
#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

int main(){
    int fd = open("hello.txt" , O_RDWR);
    if(fd == -1){
        perror("open");
        return -1;
    }
    int ret = lseek(fd,100,SEEK_END);
    if(ret == -1){
        perror("lseek");
        return -1;
    }
    //写入空数据
    write(fd , " " , 1);
    close(fd);
    return 0;
}