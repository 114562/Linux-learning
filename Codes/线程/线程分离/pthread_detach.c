/*
    #include <pthread.h>
    功能: 分离一个线程,被分离的线程在终止的时候会自动的释放资源返回给系统
          1.不能多次分离,会产生不可预料的行为
          2.不能去连接一个已经分离的线程,会报错
    参数
        -thread:需要分离的线程的id
    返回值
        成功返回0
        失败返回错误号
    int pthread_detach(pthread_t thread);
*/
#include <pthread.h>
#include <stdio.h>
#include <string.h>

void* callback(void* argv) {
    printf("child thread id: %ld\n", pthread_self());
    return NULL;
}

int main() {
    // 创建子线程
    pthread_t tid;
    int ret = pthread_create(&tid, NULL, callback, NULL);
    if (ret != 0) {
        char* str;
        str = strerror(ret);
        printf("errno1: %s\n", str);
    }
    // 输出主线程和子线程的id
    printf("tid: %ld, main thread id: %ld\n", tid, pthread_self());

    // 设置子线程分离,分离后,子线程结束后不需要pthread_join函数回收资源
    ret = pthread_detach(tid);
    if (ret != 0) {
        char* str;
        str = strerror(ret);
        printf("errno2: %s\n", str);
    }

    // 验证使用join报错
    ret = pthread_join(tid, NULL);
    if (ret != 0) {
        char* str;
        str = strerror(ret);
        printf("errno3: %s\n", str);
    }
    // 退出主线程
    pthread_exit(NULL);

    return 0;
}