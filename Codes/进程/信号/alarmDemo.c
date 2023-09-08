// 1秒电脑能数多少个数
#include <stdio.h>
#include <unistd.h>
int main() {
    alarm(1);
    int i = 0;
    while (1) {
        printf("%d\n", i++);
    }
    return 0;
}