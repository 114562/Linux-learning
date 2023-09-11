/*
    #include <poll.h>

     struct pollfd {
        int   fd;          file descriptor
        short events;      requested events
        short revents;    returned events
    };
    参数
        -fds: 是一个struct pollfd结构体数组，这是一个需要检测的文件描述符的集合
        -nfds: 这个是第一个参数数组中最后一个有效元素的下标 + 1、
        -timeout: 阻塞时长
            0: 不阻塞
            -1: 阻塞,当检测到需要检测的文件描述符有变化时解除阻塞
            > 0: 阻塞时长
    返回值
        -1 表示失败
        >0(n) 表示成功,集合中有n个文件描述符发生变化
    int poll(struct pollfd *fds, nfds_t nfds, int timeout);

*/

#include <arpa/inet.h>
#include <poll.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/select.h>
#include <sys/time.h>
#include <sys/types.h>
#include <unistd.h>

int main() {
    // 创建socket
    int lfd = socket(PF_INET, SOCK_STREAM, 0);
    struct sockaddr_in saddr;
    saddr.sin_port = htons(9999);
    saddr.sin_family = AF_INET;
    saddr.sin_addr.s_addr = INADDR_ANY;

    // 绑定
    int ret = bind(lfd, (struct sockaddr*)&saddr, sizeof(saddr));
    if (ret == -1) {
        perror("bind");
        exit(0);
    }
    // 监听
    listen(lfd, 8);

    // 初始化检测文件描述符数组
    struct pollfd fds[1024];
    for (int i = 0; i < 1024; i++) {
        fds[i].fd = -1;
        fds[i].events = POLLIN;
    }
    fds[0].fd = lfd;
    int nfds = 0;

    while (1) {
        // 调用select系统函数,让内核帮忙检测那些文件描述符有数据
        int ret = poll(fds, nfds + 1, -1);
        if (ret == -1) {
            perror("select");
            exit(0);
        } else if (ret == 0) {
            continue;
        } else if (ret > 0) {
            // 检测到了有文件描述符对应的缓冲区的数据发生了改变
            if (fds[0].revents & POLLIN) {
                // 表示有新的客户端连接进来了
                struct sockaddr_in cliaddr;
                socklen_t len = sizeof(cliaddr);
                int cfd = accept(lfd, (struct sockaddr*)&cliaddr, &len);
                // 将新的文件描述符加入到集合中
                for (int i = 1; i < 1024; i++) {
                    if (fds[i].fd == -1) {
                        fds[i].fd = cfd;
                        fds[i].events = POLLIN;
                        break;
                    }
                }
                // 更新最大的文件描述符
                nfds = nfds > cfd ? nfds : cfd;
            }
            for (int i = 1; i <= nfds; i++) {
                // lfd一定是在最前面的,所有从后面开始监听
                if (fds[i].revents & POLLIN) {
                    // i文件描述符对应的客户端发来了数据
                    // 通信
                    char buf[1024] = {0};
                    int readlen = read(fds[i].fd, buf, sizeof(buf));
                    if (readlen == -1) {
                        perror("read");
                        exit(0);
                    } else if (readlen == 0) {
                        printf("client: %d closed...\n", fds[i].fd);
                        close(fds[i].fd);
                        fds[i].fd = -1;
                    } else {
                        printf("read buf: %s\n", buf);
                        write(fds[i].fd, buf, strlen(buf) + 1);
                    }
                }
            }
        }
    }
    close(lfd);

    return 0;
}
