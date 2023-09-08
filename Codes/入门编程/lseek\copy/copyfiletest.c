/*
    #include <unistd.h>
    参数
        - fd 文件描述符 通过文件描述符操作某一个文件
        - buf 需要读取数据存放的地方，一般是数组的地址 ，是缓冲区(传出参数)
        - count 指定的数组的大小

    返回值
        - 成功:
            >0 : 返回实际的读取到的字节数
            =0 : 文件已经读取完了
        - 失败
            -1 : 文件读取失败
    ssize_t read(int fd, void buf[.count], size_t count);


    #include <unistd.h>
    参数
        - fd 文件描述符 通过文件描述符操作某一个文件，一般通过open得到
        - buf 写入数据的地方，一般是数组的地址 ，是缓冲区(传出参数)
        - count 要写的数据的的实际的大小
    返回值
        - 成功:
            >0 : 返回实际的写入的字节数
            =0 : 没有任何内容写入
        - 失败
            -1 : 文件写入失败
    ssize_t write(int fd, const void buf[.count], size_t count);

*/

#include <fcntl.h>
#include <stdio.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

int main() {
    // 通过open打开文件
    int srcfd = open("english.txt", O_RDONLY);
    if (srcfd == -1) {
        perror("open");
        return -1;
    }

    // 创建一个新的文件(拷贝文件)
    int destfd = open("cpy.txt", O_WRONLY | O_CREAT, 0664);
    if (destfd == -1) {
        perror("copy");
        return -1;
    }
    // 频繁的读写数据
    char buf[1024] = {0};
    int len = 0;
    while ((len = read(srcfd, buf, sizeof(buf))) > 0) {
        write(destfd, buf, len);
    }
    // 关闭文件
    close(destfd);
    close(srcfd);

    return 0;
}