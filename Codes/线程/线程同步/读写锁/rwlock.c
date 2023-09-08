/*

*/
// 案例:创建8个线程, 操作同一个全局变量
// 3个线程不定时写这个全局,5个线程不定时读这个全局变量

#include <pthread.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>

// 共享数据
int num = 1;

// 创建读写锁
pthread_rwlock_t rwlock;

void* writeNum(void* argv) {
    while (1) {
        pthread_rwlock_wrlock(&rwlock);
        num++;
        printf("write++, tid: %ld, num: %d\n", pthread_self(), num);
        pthread_rwlock_unlock(&rwlock);
        usleep(100);
    }

    return NULL;
}

void* readNum(void* argv) {
    while (1) {
        pthread_rwlock_rdlock(&rwlock);
        printf("===read, tid: %ld, num: %d\n", pthread_self(), num);
        pthread_rwlock_unlock(&rwlock);
        usleep(100);
    }
    return NULL;
}

int main() {
    // 初始化
    pthread_rwlock_init(&rwlock, NULL);

    // 创建3个写线程，5个读线程
    pthread_t wtids[3], rtids[5];
    for (int i = 0; i < 3; i++) {
        int ret = pthread_create(&wtids[i], NULL, writeNum, NULL);
    }
    for (int i = 0; i < 5; i++) {
        int ret = pthread_create(&rtids[i], NULL, readNum, NULL);
    }

    // 设置线程分离
    for (int i = 0; i < 3; i++) {
        pthread_detach(wtids[i]);
    }
    for (int i = 0; i < 5; i++) {
        pthread_detach(rtids[i]);
    }

    // 释放锁
    pthread_rwlock_destroy(&rwlock);

    // 退出主线程
    pthread_exit(NULL);

    return 0;
}