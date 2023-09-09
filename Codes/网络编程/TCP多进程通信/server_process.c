#include <arpa/inet.h>
#include <errno.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <wait.h>

void recyleChild(int arg) {
    while (1) {
        int ret = waitpid(-1, NULL, WNOHANG);  //-1 回收所有子进程 WNOHANG设置非阻塞
        if (ret == -1) {
            printf("所有的子进程回收\n");
            break;
        } else if (ret == 0) {
            break;
        } else {
            printf("子进程%d被回收了\n", ret);
        }
    }
}

int main() {
    // 注册信号捕捉
    struct sigaction act;
    act.sa_flags = 0;
    sigemptyset(&act.sa_mask);
    act.sa_handler = recyleChild;

    sigaction(SIGCHLD, &act, NULL);

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

    while (1) {
        // accept
        struct sockaddr_in caddr;
        int len = sizeof(caddr);

        int cfd = accept(lfd, (struct sockaddr*)&caddr, &len);
        if (cfd == -1) {
            if (errno == EINTR) {
                // 产生中断
                continue;
            }
            perror("accept");
            exit(0);
        }

        pid_t pid = fork();
        if (pid == 0) {
            // 子进程
            // 获取客户端的信息
            char clientIP[16] = {0};
            inet_ntop(AF_INET, &caddr.sin_addr.s_addr, clientIP, sizeof(clientIP));
            unsigned short port;
            port = ntohs(caddr.sin_port);
            printf("clinet IP: %s, port: %x\n", clientIP, port);

            // 读数据
            char recvBuf[1024];
            while (1) {
                int readLen = read(cfd, recvBuf, sizeof(recvBuf));
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
                write(cfd, recvBuf, strlen(recvBuf) + 1);
            }
            close(cfd);
            // 退出子进程
            exit(0);
        }
    }
    close(lfd);
    return 0;
}
