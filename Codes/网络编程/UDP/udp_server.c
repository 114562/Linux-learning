#include <arpa/inet.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

int main() {
    // 创建一个通信的socket
    int fd = socket(PF_INET, SOCK_DGRAM, 0);
    if (fd == 0) {
        perror("socket");
        exit(0);
    }
    // bind
    struct sockaddr_in addr;
    addr.sin_addr.s_addr = INADDR_ANY;
    addr.sin_port = htons(9999);
    addr.sin_family = AF_INET;
    int ret = bind(fd, (struct sockaddr*)&addr, sizeof(addr));
    if (ret == -1) {
        perror("bind");
        exit(0);
    }

    // 通信
    while (1) {
        // 接收数据
        char ipbuf[18];
        char recvbuf[1024] = {0};
        struct sockaddr_in cliaddr;
        socklen_t len = sizeof(cliaddr);
        int num = recvfrom(fd, recvbuf, sizeof(recvbuf), 0, (struct sockaddr*)&cliaddr, &len);
        if (num == -1) {
            perror("revcfrom");
            exit(0);
        }
        printf("client IP: %s, Port: %d\n",
               inet_ntop(AF_INET, &cliaddr.sin_addr.s_addr, ipbuf, sizeof(ipbuf)),
               ntohs(cliaddr.sin_port));
        printf("client say: %s\n", recvbuf);

        // 发送数据
        sendto(fd, recvbuf, strlen(recvbuf) + 1, 0, (struct sockaddr*)&cliaddr, sizeof(cliaddr));
    }
    return 0;
}