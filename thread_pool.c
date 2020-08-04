
#include "thread_pool.h"


int check(int exp, const char* msg) {
    if (exp == SOCKETERROR) {
        perror(msg);
        exit(1);
    }
    return exp;
}

void* thread_work_job(void* arg) {
    int* thread_id = (int*)arg;

    while (true) {
        int* pclient;
        pthread_mutex_lock(&thread_lock);
        

        pthread_mutex_unlock(&thread_lock);
    }
}


pthread_t* thread_pool_init(void) {
    pthread_t* thread_pool = (pthread_t*)malloc(sizeof(pthread_t) * THREAD_POOL_SIZE);
    for (int i = 0; i < THREAD_POOL_SIZE; i++) {
        pthread_create(&thread_pool[i], NULL, thread_work_job, &i);
    }
    return thread_pool;
}
