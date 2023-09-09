/*
    #include <arpa/inet.h> // 包含了这个头文件，上面两个就可以省略
    #include <sys/socket.h>
    #include <sys/types.h>

int socket(int domain, int type, int protocol);
    功能:创建一个套接字
    参数
        -domain: 协议族
              Name               Purpose                            Man page
        AF_UNIX  AF_LOCAL     Local communication                   unix(7)
            AF_INET6     IPv6 Internet protocols
            AF_INET      IPv4 Internet protocols                    ip(7)
        -type: 通信过程中使用的协议类型
            SOCK_STREAM 流式协议
            SOCK_DGRAM  报式协议
        - protocol: 具体的协议,一般写0，
            SOCK_STREAM流式协议默认使用TCP
            SOCK_DGRAM 报式协议默认使用UDP
    返回值: 返回文件描述符,操作的就是内核缓冲区
        成功返回fd
        失败返回 -1


int bind(int sockfd, const struct sockaddr *addr, socklen_t addrlen);
    功能: 绑定,将fd和本地的IP + 端口进行绑定
    参数:
        -sockfd: 通过socket函数得到的文件描述符
        -addr: 需要绑定的socket地址,这个地址就封装了IP和端口号的信息
        -addrlen: addr结构体占的内存大小

int listen(int sockfd, int backlog); // /proc/sys/net/core/somaxconn
    功能: 监听这个套接字上的连接
    参数
        - sockfd: 通过socket函数得到的文件描述符
        -backlog: 未连接和已经链接的套接字的和的最大值，一般为4096
    成功返回0,失败返回-1

int accept(int sockfd, struct sockaddr *addr, socklen_t *addrlen);
    功能:接受客户端连接,默认是一个阻塞等待连接,阻塞等待客户端连接
    参数
        -sockfd: 用于监听的文件描述符
        -addr: 传出参数,记录了连接成功后客户端的地址信息(IP,port)
        -addrlen: 指定第二个参数的文件大小
    返回值:
        成功返回用于通信的文件描述符
        失败返回-1

int connect(int sockfd, const struct sockaddr *addr, socklen_t addrlen);
    功能: 客户端连接服务器
    参数
        -sockfd: 用于通信的文件描述符
        -addr: 客户端要连接服务器的地址信息
        -addrlen: 第二个参数的内存大小
    返回值
        成功返回0
        失败返回-1
ssize_t write(int fd, const void *buf, size_t count); // 写数据
ssize_t read(int fd, void *buf, size_t count); // 读数据

*/

#include <arpa/inet.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

int main() {
    // 1.创建监听套接字
    int lfd = socket(AF_INET, SOCK_STREAM, 0);
    if (lfd == -1) {
        perror("socket");
        exit(0);
    }
    // 2.绑定
    struct sockaddr_in saddr;
    saddr.sin_family = AF_INET;
    // inet_pton(AF_INET, " 192.168.203.59", saddr.sin_addr.s_addr);
    saddr.sin_addr.s_addr = INADDR_ANY;
    saddr.sin_port = htons(9999);
    int ret = bind(lfd, (struct sockaddr*)&saddr, sizeof(saddr));
    if (ret == -1) {
        perror("bind");
        exit(0);
    }

    // 3.监听
    ret = listen(lfd, 8);
    if (ret == -1) {
        perror("listen");
        exit(0);
    }

    // 4.接收客户端连接
    struct sockaddr_in clientaddr;
    socklen_t len = sizeof(clientaddr);
    int cfd = accept(lfd, (struct sockaddr*)&clientaddr, &len);
    if (cfd == -1) {
        perror("accept");
        exit(0);
    }

    // 输出客户端信息,将其转换成主机字节序
    char clientIP[16];
    // 获取IP
    inet_ntop(AF_INET, &clientaddr.sin_addr.s_addr, clientIP, sizeof(clientIP));
    // 获取端口
    unsigned short clientPort = ntohs(clientaddr.sin_port);
    printf("clinet IP is: %s, port is: %d\n", clientIP, clientPort);

    // 5.获取客户端数据
    char recvBuf[1024] = {0};
    while (1) {
        len = read(cfd, recvBuf, sizeof(recvBuf));
        if (len == -1) {
            perror("read");
            exit(0);
        } else if (len > 0) {
            printf("recv client data: %s\n", recvBuf);
        } else if (len == 0) {
            printf("client close..\n");
            break;
        }
        // 给客户端发送数据
        char* sendBuf = "hello , i am server";
        write(cfd, sendBuf, strlen(sendBuf));
    }

    // 关闭文件描述符
    close(lfd);
    close(cfd);
    return 0;
}