/*
    #include <pthread.h>
    功能:终止一个线程,在哪个线程中调用表示终止哪个线程
    参数
        - retval:指针,作为一个返回值,可以在pthread_join()中获取到
    没有返回值
    void pthread_exit(void *retval);

*/
#include <pthread.h>
#include <stdio.h>
#include <string.h>

void* callback(void* arg) {
    printf("child thread id: %ld\n", pthread_self());
    return NULL;
}

int main() {
    // 创建一个子线程
    pthread_t tid;
    int ret = pthread_create(&tid, NULL, callback, NULL);
    if (ret != 0) {
        char* str;
        str = strerror(ret);
        printf("error: %s\n", str);
    }
    // 主线程
    for (int i = 0; i < 5; i++) {
        printf("%d\n", i);
    }
    // 打印id
    printf("tid: %ld, parent thread id: %ld\n", tid, pthread_self());
    // 让主线程退出,当至线程退出时,不会影响其他正常运行的线程
    pthread_exit(NULL);
    printf("main thread exit\n"); //不会执行
    return 0; //没有执行,进程没有退出
}