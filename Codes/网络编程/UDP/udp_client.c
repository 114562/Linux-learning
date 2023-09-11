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
    struct sockaddr_in saddr;
    saddr.sin_port = htons(9999);
    saddr.sin_family = AF_INET;
    inet_pton(AF_INET, "127.0.0.1", &saddr.sin_addr.s_addr);

    int num = 1;
    // 通信
    while (1) {
        // 接收数据
        char ipbuf[18];
        char sendbuf[1024] = {0};
        sprintf(sendbuf, "hello , i am clent %d\n", num++);

        // 发送数据
        sendto(fd, sendbuf, strlen(sendbuf) + 1, 0, (struct sockaddr*)&saddr, sizeof(saddr));
        struct sockaddr_in cliaddr;
        socklen_t len = sizeof(cliaddr);
        int num = recvfrom(fd, sendbuf, sizeof(sendbuf), 0, NULL, NULL);

        printf("server say: %s\n", sendbuf);
        sleep(1);
    }
    close(fd);
    return 0;
}