/*
    #include <arpa/inet.h>
    // p:点分十进制的IP字符串，n:表示network，网络字节序的整数
    int inet_pton(int af, const char *src, void *dst);
        af:地址族: AF_INET AF_INET6
        src:需要转换的点分十进制的IP字符串
        dst:转换后的结果保存在这个里面

    // 将网络字节序的整数，转换成点分十进制的IP地址字符串
    const char * inet_ntop(int af, const void *src, char *dst, socklen_t size);
        af：地址族： AF_INET AF_INET6
        src：要转换的IP的整数的地址
        st：转换成IP地址字符串保存的地方
        size：第三个参数的大小(数组的容量)
        返回值:返回转换后的数据的地址(字符串)，和dst是一样的
*/

#include <arpa/inet.h>
#include <stdio.h>

int main() {
    // 创建一个点分十进制的IP字符串
    char buf[] = "192.168.1.4";
    unsigned int num;
    // 将点分十进制的IP字符串转换成网络字节序的整数
    inet_pton(AF_INET, buf, &num);  // AF_INET 代表IPv4
    unsigned char* p = (unsigned char*)&num;
    printf("%d %d %d %d\n", *p, *(p + 1), *(p + 2), *(p + 3));

    // 将网络字节序的IP整数转换成点分十进制的IP字符串
    char ip[16] = "";

    const char* str = inet_ntop(AF_INET, &num, ip, sizeof(ip));
    printf("str: %s\n", str);
    printf("ip: %s\n", ip);  // str 和 IP的地址是相同的
    return 0;
}
