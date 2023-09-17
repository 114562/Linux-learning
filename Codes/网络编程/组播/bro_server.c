#include <arpa/inet.h>
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

int main() {
    // 创建socket
    int fd = socket(PF_INET, SOCK_DGRAM, 0);
    if (fd == -1) {
        perror("socket");
        exit(-1);
    }

    // 2.设置广播属性
    int op = 1;
    setsockopt(fd, SOL_SOCKET, SO_BROADCAST, &op, sizeof(op));

    struct sockaddr_in cliaddr;
    cliaddr.sin_family = AF_INET;
    cliaddr.sin_port = htons(9999);
    inet_pton(AF_INET, "172.25.154.255", &cliaddr.sin_addr.s_addr);

    int num = 0;
    while (1) {
        char sendBud[128];
        sprintf(sendBud, "hello, client..%d\n", num++);
        // 发送数据
        sendto(fd, sendBud, strlen(sendBud) + 1, 0, (struct sockaddr *)&cliaddr.sin_addr.s_addr, sizeof(cliaddr));
        printf("广播的数据:%s\n", sendBud);
        sleep(1);
    }

    close(fd);
    return 0;
}
