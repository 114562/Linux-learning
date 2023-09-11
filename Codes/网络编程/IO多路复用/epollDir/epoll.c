/*
    #include <sys/epoll.h>
    功能: 创建一个新的epoll实例，在内核中创建一个数据，这个数据
          有两个重要数据：一个是需要检测的问价描述符的信息(红黑树)
          另一个是就绪列表，存放检测数据发送改变的文件描述符信息(双向链表)
    参数
        -size 改版之后无意义，随便填一个大于0的书
    返回值
        成功返回>0：文件描述符，操作epoll实例
        失败返回-1
-    int epoll_create(int size);

    功能: 对epoll实例进行管理,添加文件描述符信息，删除修改信息
    参数
        -epfd：epoll实例对应的文件描述符
        -op：要进行什么操作
            EPOLL_CTL_ADD:添加
            EPOLL_CTL_MOD:修改
            EPOLL_CTL_DEL:删除
        -fd：要检测的文件描述符
        -event：检测文件描述符什么事情
            struct epoll_event {
                uint32_t      events;  Epoll events
                epoll_data_t  data;     User data variable
            };
        常见的epoll检测事件
            EPOLLIN
            EPOLLOUT
            EPOLLERR
        typedef union epoll_data { //联合体,只有一个会存在
           void     *ptr;
           int       fd;
           uint32_t  u32;
           uint64_t  u64;
        };
-    int epoll_ctl(int epfd, int op, int fd, struct epoll_event *_Nullable event);

    功能：检测函数
    参数
        -epfd:epoll实例对应的文件描述符
        -events：传出参数，保存了发生变化的文件描述符的信息
        -maxevents：第二个结构体参数的大小
        -timeout：阻塞事件
            0表示不阻塞
            -1表示阻塞，直到检测到fd数据发生变化时解除阻塞
            > 0 阻塞时长(mm)
    返回值
        成功返回>0的数，表示变化的文件描述符的个数
        失败返回 -1
-    int epoll_wait(int epfd, struct epoll_event *events,int maxevents, int timeout);
*/
#include <arpa/inet.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/epoll.h>
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
                // 封装cfd文件描述符的信息到内核中，进行监听
                epev.events = EPOLLIN;
                epev.data.fd = cfd;
                epoll_ctl(epfd, EPOLL_CTL_ADD, cfd, &epev);
            } else {
                if(epevs[i].events & EPOLLOUT){
                    printf("检测到写\n");
                    continue;
                }
                // 有数据到达,需要通信
                char buf[1024] = {0};
                int len = read(epevs[i].data.fd, buf, sizeof(buf));
                if (len == -1) {
                    perror("read");
                    exit(0);
                } else if (len == 0) {
                    printf("client %d closed...\n", epevs[i].data.fd);
                    epoll_ctl(epfd, EPOLL_CTL_DEL, epevs[i].data.fd, NULL);
                    close(epevs[i].data.fd);
                } else if (len > 0) {
                    printf("read buf: %s\n", buf);
                    write(epevs[i].data.fd, buf, strlen(buf) + 1);
                }
            }
        }
    }
    close(lfd);
    close(epfd);
    return 0;
}