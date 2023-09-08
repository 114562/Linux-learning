/*
    #include <pthread.h>
    功能:取消线程(让线程终止)。
        可以终止某个线程的运行
        但不是立马终止,而是当子线程执行到一个取消点,鲜橙才会终止
        取消点:系统规定好的一些系统调用,粗略的理解为从用户区到内核区的切换，这个位置称之为取消点


    参数
        - thread: 要取消的线程的线程号
    返回值

    int pthread_cancel(pthread_t thread);
*/

#include <pthread.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>

void* callback(void* argv) {
    printf("child thread tid: %ld\n", pthread_self());
    for (int i = 0; i < 5; i++) {
        printf("child %d\n", i);
    }
    return NULL;
}

int main() {
    pthread_t tid;
    int ret = pthread_create(&tid, NULL, callback, NULL);
    if (ret != 0) {
        char* str = strerror(ret);
        printf("errno: %s\n", str);
    }

    // 取消线程
    pthread_cancel(tid);

    printf("tid : %ld, main thread id: %ld\n", tid, pthread_self());
    for (int i = 0; i < 5; i++) {
        printf("%d\n", i);
    }
    pthread_exit(NULL);

    return 0;
}