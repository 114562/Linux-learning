/*
    #include <semaphore.h>
    参数

    int sem_init(sem_t *sem, int pshared, unsigned int value);
        - sem: 信号量变量的地址
        - pshared: 0用在线程间,非零用在进程间
        - value: 信号量中的值

    int sem_wait(sem_t *sem);
        - sem: 对信号量加锁,调用一次sem值-1,减为0之后阻塞,大 于0直接返回
    int sem_post(sem_t *sem);
        - sem: 对信号量解锁,调用一次sem值+1
*/
#include <pthread.h>
#include <semaphore.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

// 创建两个信号量
sem_t psem, csem;

// 链表当做容器
struct Node {
    int num;
    struct Node* next;
};

// 创建互斥量
pthread_mutex_t mutex;

// 头结点
struct Node* head = NULL;

void* producer(void* arg) {
    while (1) {
        sem_wait(&psem);
        pthread_mutex_lock(&mutex);

        struct Node* newNode = (struct Node*)malloc(sizeof(struct Node));
        newNode->next = head;
        head = newNode;
        newNode->num = rand() % 1000;
        printf("add node, num: %d, tid: %ld\n", newNode->num, pthread_self());

        pthread_mutex_unlock(&mutex);
        sem_post(&csem);
    }
    return NULL;
}

void* customer(void* arg) {
    while (1) {
        sem_wait(&csem);
        pthread_mutex_lock(&mutex);

        struct Node* tmp = head;
        printf("delete node, num: %d, tid: %ld\n", tmp->num, pthread_self());
        head = head->next;
        free(tmp);
        pthread_mutex_unlock(&mutex);
        sem_post(&psem);
    }
    return NULL;
}

// 创建5个生产者线程和5个消费者线程
int main() {
    pthread_mutex_init(&mutex, NULL);
    sem_init(&psem, 0, 8);
    sem_init(&csem, 0, 0);

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
    pthread_exit(NULL);
    return 0;
}