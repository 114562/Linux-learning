#include <fcntl.h>
#include <stdio.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

/*
    //创建一个文件
    int open(const char *pathname, int flags, mode_t mode);
    参数
        -pathname：要创建的文件的路径
        -flags：对文件的操作权限和其他的设置
                是一个int类型的数据，占4个字节，32位，每一位就是一个标志位
        必选项：O_RDONLY, O_WRONLY, or O_RDWR 三选一
        可选项：O_CREATE 文件不存在，创建新文件
        -mode：八进制的数，表示用户创建出的新的操作权限
            最终的权限是 mode & ~umask  umask = 0022 ~umask = 0755
            0777 -> 111111111
            0755 -> 111101101 两者相与  umask作用是抹去某些权限

*/

int main() {
    // 创建一个新文件
    int fd = open("create.txt", O_RDWR | O_CREAT, 0777);
    if (fd == -1) {
        perror("open");
    }
    // 关闭文件描述符
    close(fd);
    return 0;
}