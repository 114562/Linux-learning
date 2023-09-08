/*
    #include <pthread.h>

    int pthread_attr_init(pthread_attr_t *attr);
        -初始化属性变量
    int pthread_attr_destroy(pthread_attr_t *attr);
        -释放线程属性资源
    int pthread_attr_setdetachstate(pthread_attr_t *attr, int detachstate);
        -设置线程分离的状态属性
    int pthread_attr_getdetachstate(const pthread_attr_t *attr,int *detachstate);
        -获取线程分离的状态属性
*/

#include <pthread.h>
#include <stdio.h>
#include <string.h>

void* callback(void* argv) {
    printf("child thread tid: %ld\n", pthread_self());
    return NULL;
}

int main() {
    // 创建一个线程属性变量
    pthread_attr_t attr;
    // 初始化线程属性
    pthread_attr_init(&attr);
    // 设置属性
    pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_DETACHED);

    // 创建子线程
    pthread_t tid;
    int ret = pthread_create(&tid, &attr, callback, NULL);
    if (ret != 0) {
        char* str;
        str = strerror(ret);
        printf("errno1: %s\n", str);
    }

    // 获取线程栈的大小
    size_t size;
    pthread_attr_getstacksize(&attr, &size);
    printf("thread stack size:%ld\n", size);

    // 输出主线程和子线程的id
    printf("tid: %ld, main thread id: %ld\n", tid, pthread_self());

    // 释放线程属性资源
    pthread_attr_destroy(&attr);

    // 退出主线程
    pthread_exit(NULL);

    return 0;
}