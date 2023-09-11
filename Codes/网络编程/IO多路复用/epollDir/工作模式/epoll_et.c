#include <arpa/inet.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/epoll.h>
#include <unistd.h>
#include <errno.h>

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

    // 用epoll_create() 创建一个epoll实例
    int epfd = epoll_create(1);
    // 将监听的文件描述符的监测信息加到epoll的实例当中(内核中)
    struct epoll_event epev;
    epev.events = EPOLLIN;
    epev.data.fd = lfd;
    epoll_ctl(epfd, EPOLL_CTL_ADD, lfd, &epev);

    // 保存内核检测之后返回的文件描述符的信息
    struct epoll_event epevs[1024];
    while (1) {
        int ret = epoll_wait(epfd, epevs, 1024, -1);
        if (ret == -1) {
            perror("epoll_wait");
            exit(0);
        }
        // ret是检测到有多少个文件描述符发生改变的值
        printf("ret = %d\n", ret);
        for (int i = 0; i < ret; i++) {
            if (epevs[i].data.fd == lfd) {
                // 监听的文件描述符有数据到达,即有新的客户端连接
                struct sockaddr_in cliaddr;
                socklen_t len = sizeof(cliaddr);
                int cfd = accept(lfd, (struct sockaddr*)&cliaddr, &len);
                // read是否阻塞跟文件描述符的状态有关，设置文件描述符cfd为非阻塞
                int flag = fcntl(cfd, F_GETFL);
                flag |= O_NONBLOCK;
                fcntl(cfd, F_SETFL, flag);

                // 封装cfd文件描述符的信息到内核中，进行监听
                epev.events = EPOLLIN | EPOLLET;  // 设置边沿触发
                epev.data.fd = cfd;
                epoll_ctl(epfd, EPOLL_CTL_ADD, cfd, &epev);
            } else {
                if (epevs[i].events & EPOLLOUT) {
                    printf("检测到写\n");
                    continue;
                }
                // 循环读取出所有数据
                int len = 0;
                char buf[5] = {0};
                while ((len = read(epevs[i].data.fd, buf, sizeof(buf)) > 0)) {
                    // 打印数据
                    printf("recv data:%s\n", buf);
                    write(epevs[i].data.fd, buf, len);
                }
                if(len == 0){
                    printf("client: %d closed\n",epevs[i].data.fd);
                }else if(len == -1){
                    // 
                    if(errno == EAGAIN){
                        printf("data over\n");
                    }else{
                        perror("write");
                        exit(0);
                    }
                    
                }
            }
        }
    }
    close(lfd);
    close(epfd);
    return 0;
}