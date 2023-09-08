/*
    #include <pthread.h>
    功能: 和一个已经终止的线程进行连接。对线程的资源进行释放，回收资源
        这个函数是阻塞函数,调用一次回收一个子线程
        一般在主线程中使用
    参数:
        - thread:指定回收的线程号
        - retval:接受子线程退出时的返回值
                 为什么是二级指针:
    返回值
        成功返回0
        失败返回错误号,非零
    int pthread_join(pthread_t thread, void **retval);
*/
#include <pthread.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>

int value = 10;  // 定义全局变量
void* callback(void* arg) {
    printf("child thread id: %ld\n", pthread_self());
    // sleep(3);
    // int tmp = 100; //这里是子进程的栈空间,当该线程结束之后是随机值,
    pthread_exit((void*)&value);  // 等同于return (void*)&tmp;
}

int main() {
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

    // 主线程调用pthread_join()回收子线程资源
    int* thread_retval;
    ret = pthread_join(tid, (void**)&thread_retval);  // 要回收的子线程没有结束,那么程序阻塞

    if (ret != 0) {
        char* str;
        str = strerror(ret);
        printf("error: %s\n", str);
    }
    printf("exit data: %d\n", *thread_retval);
    printf("回收子线程资源成功\n");

    // 打印id
    printf("tid: %ld, parent thread id: %ld\n", tid, pthread_self());
    // 让主线程退出,当至线程退出时,不会影响其他正常运行的线程
    pthread_exit(NULL);
    return 0;
}