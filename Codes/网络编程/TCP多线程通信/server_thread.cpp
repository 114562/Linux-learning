#include <arpa/inet.h>
#include <errno.h>
#include <pthread.h>
#include <unistd.h>
#include <wait.h>

#include <cstring>
#include <iostream>

struct sockInfo {
    int fd;         // 通信的文件描述符
    pthread_t tid;  // 线程号
    struct sockaddr_in addr;
};
struct sockInfo sockInfos[128];

void* working(void* arg) {
    // 子线程和客户端通信 cfd 客户端的信息 线程号
    // 获取客户端的信息
    struct sockInfo* pinfo = (struct sockInfo*)arg;
    char clientIP[16];
    inet_ntop(AF_INET, &pinfo->addr, clientIP, sizeof(clientIP));
    unsigned short port;
    port = ntohs(pinfo->addr.sin_port);
    printf("clinet IP: %s, port: %x\n", clientIP, port);

    // 读数据
    char recvBuf[1024];
    while (1) {
        int readLen = read(pinfo->fd, &recvBuf, sizeof(recvBuf));
        if (readLen > 0) {
            printf("recv client: %s\n", recvBuf);
        } else if (readLen == 0) {
            printf("client closed..\n");
            break;
        } else if (readLen == -1) {
            perror("read");
            exit(0);
        }
        // 回写数据
        write(pinfo->fd, recvBuf, strlen(recvBuf) + 1);
    }
    close(pinfo->fd);
    return NULL;
}

int main() {
    // 创建套接字
    int lfd = socket(PF_INET, SOCK_STREAM, 0);
    if (lfd == -1) {
        perror("socket");
        exit(0);
    }

    // bind
    struct sockaddr_in saddr;
    saddr.sin_family = AF_INET;
    saddr.sin_port = ntohs(9999);
    saddr.sin_addr.s_addr = INADDR_ANY;

    int ret = bind(lfd, (struct sockaddr*)&saddr, sizeof(saddr));
    if (ret == -1) {
        perror("bind");
        exit(0);
    }

    // listen
    ret = listen(lfd, 128);
    if (ret == -1) {
        perror("listen");
        exit(0);
    }

    // 初始化数据
    int max = sizeof(sockInfos) / sizeof(sockInfos[0]);
    for (int i = 0; i < max; i++) {
        bzero(&sockInfos[i], sizeof(sockInfos[i]));
        sockInfos[i].fd = -1;  // 设置fd可用
        sockInfos[i].tid = -1;
    }

    // 循环等待客户端连接，一旦一个客户端连接进来，就创建一个子线程
    while (1) {
        struct sockaddr_in cliaddr;
        socklen_t len = sizeof(cliaddr);
        int cfd = accept(lfd, (struct sockaddr*)&cliaddr, &len);

        // 创建子线程
        struct sockInfo* pinfo;
        for (int i = 0; i < max; i++) {
            // 从这个数组中找到一个可用的sockInfo元素
            if (sockInfos[i].fd == -1) {
                // 可用
                pinfo = &sockInfos[i];
                break;
            }
            if (i == max - 1) {
                sleep(1);
                i--;
            }
        }
        pinfo->fd = cfd;
        memcpy(&pinfo->addr, &cliaddr, len);
        // 创建子线程
        pthread_create(&pinfo->tid, NULL, working, (void*)&pinfo);
        // 使用非阻塞的线程回收方式
        pthread_detach(pinfo->tid);
    }
    close(lfd);
}
