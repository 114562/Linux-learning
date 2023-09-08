/*
    // 转换端口
    uint16_t htons(uint16_t hostshort); // 主机字节序 - 网络字节序
    uint16_t ntohs(uint16_t netshort); // 主机字节序 - 网络字节序
    // 转IP
    uint32_t htonl(uint32_t hostlong); // 主机字节序 - 网络字节序
    uint32_t ntohl(uint32_t netlong); // 主机字节序 - 网络字节序
*/

#include <arpa/inet.h>
#include <stdio.h>

int main() {
    // htons

    unsigned short a = 0x0102;  // 2字节
    printf("a: %x\n", a);
    unsigned short b = htons(a);
    printf("b: %x\n", b);

    printf("===============\n");

    // htonl 转 IP
    unsigned char buf[4] = {192, 168, 1, 100};
    unsigned int num = *(unsigned int*)buf;  // 转换成int* 取的时候是读取四个字节
    unsigned int sum = htonl(num);
    unsigned char* p = (unsigned char*)&sum;
    printf("%d %d %d %d\n", *p, *(p + 1), *(p + 2), *(p + 3));

    printf("===============\n");

    // ntohl
    unsigned char buf1[4] = {1, 1, 168, 192};
    int num1 = *(int*)buf1;
    int sum1 = ntohl(num1);
    unsigned char* p1 = (unsigned char*)&sum1;
    printf("%d %d %d %d\n", *p1, *(p1 + 1), *(p1 + 2), *(p1 + 3));

    // ntohs
    unsigned short c = 0x0304;
    printf("c: %x\n", c);
    unsigned short d = ntohs(c);
    printf("d: %x\n", d);

    return 0;
}