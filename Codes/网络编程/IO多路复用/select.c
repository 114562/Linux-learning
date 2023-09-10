#include <arpa/inet.h>
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

    // 创建一个fd_set集合,存放的是需要检测的文件描述符
    // 让tmp集合交给内核操作,rdset只能通过函数调用修改
    fd_set rdset, tmp;
    FD_ZERO(&rdset);
    int maxfd = lfd;

    // 添加
    FD_SET(lfd, &rdset);
    while (1) {
        tmp = rdset;
        // 调用select系统函数,让内核帮忙检测那些文件描述符有数据
        ret = select(maxfd + 1, &tmp, NULL, NULL, NULL);
        if (ret == -1) {
            perror("select");
            exit(0);
        } else if (ret == 0) {
            continue;
        } else if (ret > 0) {
            // 检测到了有文件描述符对应的缓冲区的数据发生了改变
            if (FD_ISSET(lfd, &tmp)) {
                // 表示有新的客户端连接进来了
                struct sockaddr_in cliaddr;
                socklen_t len = sizeof(cliaddr);
                int cfd = accept(lfd, (struct sockaddr*)&cliaddr, &len);
                // 将新的文件描述符加入到集合中
                FD_SET(cfd, &rdset);
                // 更新最大的文件描述符
                maxfd = maxfd > cfd ? maxfd : cfd;
            }
            for (int i = lfd + 1; i <= maxfd; i++) {
                // lfd一定是在最前面的,所有从后面开始监听
                if (FD_ISSET(i, &tmp)) {
                    // i文件描述符对应的客户端发来了数据
                    // 通信
                    char buf[1024] = {0};
                    int readlen = read(i, buf, sizeof(buf));
                    if (readlen == -1) {
                        perror("read");
                        exit(0);
                    } else if (readlen == 0) {
                        printf("client closed...\n");
                        FD_CLR(i, &rdset);
                        close(i);
                    } else {
                        printf("read buf: %s\n", buf);
                        write(i, buf, strlen(buf) + 1);
                    }
                }
            }
        }
    }
    close(lfd);

    return 0;
}
