#include <arpa/inet.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

int main() {
    // 1.创建套接字
    int fd = socket(AF_INET, SOCK_STREAM, 0);
    if (fd == -1) {
        perror("socket");
        exit(0);
    }
    // 2.连接服务器
    struct sockaddr_in serveraddr;
    serveraddr.sin_family = AF_INET;
    inet_pton(AF_INET, " 192.168.203.59", &serveraddr.sin_addr.s_addr);
    serveraddr.sin_port = htons(9999);

    int ret = connect(fd, (struct sockaddr*)&serveraddr, sizeof(serveraddr));
    if (ret == -1) {
        perror("connect");
        exit(0);
    }

    // 通信
    char recvBuf[1024] = {0};
    while (1) {
        //  给客户端发送数据
        char* sendBuf = "hello , i am client";
        write(fd, sendBuf, strlen(sendBuf));
        // 读数据
        int len = read(fd, recvBuf, sizeof(recvBuf));
        if (len == -1) {
            perror("read");
            exit(0);
        } else if (len > 0) {
            printf("recv server data: %s\n", recvBuf);
        } else if (len == 0) {
            printf("server close..\n");
            break;
        }
        sleep(1);
    }

    close(fd);
    return 0;
}