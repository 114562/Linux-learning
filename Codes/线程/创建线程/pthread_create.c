/*
    一般情况下,main函数所在的线程称之为主线程,其他创建的线程称为子线程
    #include <pthread.h>
    功能:创建一个子线程
    参数
        - thread:传出参数,线程创建成功之后子线程的id
        - attr:设置线程的属性,一般使用默认值NULL
        - start_routine:函数指针,子线程需要处理的逻辑代码
        - arg:给第三个参数使用,传参
    返回值
        - 成功返回0
        - 错误返回错误号,与之前的errno不一样
          获取错误号的信息: char* strerror(errno); #include <string.h>
    int pthread_create(pthread_t * thread,const pthread_attr_t * attr,
            void *(*start_routine)(void *),void * arg);
*/

#include <pthread.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>

void* callback(void* arg) {
    printf("child thread...\n");
    return NULL;
}

int main() {
    // 创建一个子进程
    pthread_t tid;
    int ret = pthread_create(&tid, NULL, callback, NULL);
    if (ret != 0) {
        char* str = strerror(ret);
        printf("error: %s\n", str);
    }

    for (int i = 0; i < 5; i++) {
        printf("%d\n", i);
        sleep(1);
    }
}
