/*
    #include <sys/mman.h>
    功能: 映射一个文件的数据到内存中
    参数
        - void * addr:NULL,由内核决定
        - length: 要映射的数据的长度,这个值不能为0,建议使用文件的长度
            获取文件长度 stat 、 lseek
        - prot: 对申请的内存映射区的操作权限
            PROT_EXEC  Pages may be executed.可执行权限

            PROT_READ  Pages may be read.读权限

            PROT_WRITE Pages may be written.写权限

            PROT_NONE  Pages may not be accessed.没有权限
        要操作映射区内存,必须要有读的权限
        - flags:
            - MAP_SHARED: 映射区的数据会自动和磁盘文件进行同步,进程间通信必须要设置这个权限
            - MAP_PRIVATE: 不同步，映射区的数据改变了，对原来的文件不会修改，会重新创建一个新的文件
        - fd: 需要映射的那个文件的文件描述符
            -通过open得到，open的是一个磁盘文件
            -注意:文件的大小不能为0,open指定的权限不能和prot参数有冲突
                PROT: PROT_READ->open:只读/读写
                PROT: PROT_READ | PROT_WRITE open:只能是读写
        - offset: 偏移量,一般不用。碧玺指定的是4k的整数倍，0表示不偏移，从文件开头
        返回:返回创建的内存的首地址
            失败返回MAP_FAILED.（void*) -1
    void *mmap(void addr[.length], size_t length, int prot, int flags,int fd, off_t offset);
    int munmap(void addr[.length], size_t length);
    功能:释放内存映射
    参数
        - addr: 要释放的内存的首地址
        - length 要释放的大小，与mmap中的length一样

*/

/*
    使用内存映射实现进程间通信:
        1.有关系的进程(父子进程)
            -在没有子进程的时候创建内存映射区
            -有了内存映射区之后再创建子进程
            -父子进程共享创建的内存映射区
        2.没有关系的进程
            -准备大小不是0的磁盘文件
            -进程1，通过磁盘文件创建内存映射区
                -得到一个操作这块内存的指针
            -进程2，通过磁盘文件创建内存映射区
                -得到一个操作这块内存的指针
            -使用内存映射区进行通信
    注意:内存映射区是非阻塞的
*/

#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>
#include <wait.h>

//有关系的进程通信
int main() {
    // 1.打开这个文件
    int fd = open("test.txt", O_RDWR);
    if (fd == -1) {
        perror("open");
        exit(0);
    }
    // 1.获取文件大小
    int size = lseek(fd, 0, SEEK_END);

    // 2.创建内存映射区
    void* ptr = mmap(NULL, size, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
    if (ptr == MAP_FAILED) {
        perror("mmap");
        exit(0);
    }

    // 3.创建子进程
    pid_t pid = fork();

    if (pid > 0) {
        wait(NULL);
        char buf[64];
        strcpy(buf, (char*)ptr);
        printf("read data : %s\n", buf);
    } else if (pid == 0) {
        strcpy((char*)ptr, "nihao a , son!!!");
    }

    // 关闭内存映射区
    munmap(ptr, size);

    return 0;
}

//没有关系的进程通信
int main(){
    
}