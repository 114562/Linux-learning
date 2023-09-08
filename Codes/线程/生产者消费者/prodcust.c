/*
    生产者消费者模型
    使用了条件变量
*/

#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

// 链表当做容器
struct Node {
    int num;
    struct Node* next;
};

// 创建互斥量
pthread_mutex_t mutex;
// 创建条件变量
pthread_cond_t cond;

// 头结点
struct Node* head = NULL;

void* producer(void* arg) {
    while (1) {
        pthread_mutex_lock(&mutex);
        struct Node* newNode = (struct Node*)malloc(sizeof(struct Node));
        newNode->next = head;
        head = newNode;
        newNode->num = rand() % 1000;
        printf("add node, num: %d, tid: %ld\n", newNode->num, pthread_self());
        // 只要生产了一个,就通知消费者消费
        pthread_cond_signal(&cond);
        pthread_mutex_unlock(&mutex);
        usleep(100);
    }
    return NULL;
}

void* customer(void* arg) {
    while (1) {
        pthread_mutex_lock(&mutex);
        // 判断是否有数据
        if (head == NULL) {
            // 容器空了,阻塞
            // 当wait函数调用阻塞时会解锁,释放mutex,当不阻塞时,继续向下执行时会重新加锁
            pthread_cond_wait(&cond, &mutex);
            pthread_mutex_unlock(&mutex);
        } else {
            struct Node* tmp = head;
            printf("delete node, num: %d, tid: %ld\n", tmp->num, pthread_self());
            head = head->next;
            free(tmp);
            pthread_mutex_unlock(&mutex);
            usleep(100);
        }
    }
    return NULL;
}

// 创建5个生产者线程和5个消费者线程
int main() {
    pthread_mutex_init(&mutex, NULL);
    pthread_cond_init(&cond, NULL);
    pthread_t ptids[5], ctids[5];
    for (int i = 0; i < 5; i++) {
        pthread_create(&ptids[i], NULL, producer, NULL);
        pthread_create(&ctids[i], NULL, customer, NULL);
    }

    for (int i = 0; i < 5; i++) {
        pthread_detach(&ptids[i]);
        pthread_detach(&ctids[i]);
    }

    while (1) {
        sleep(10);
    }

    pthread_mutex_destroy(&mutex);
    pthread_cond_destroy(&cond);
    pthread_exit(NULL);
    return 0;
}