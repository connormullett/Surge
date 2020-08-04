
#ifndef _THREAD_POOL_H_
#define _THREAD_POOL_H_

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <stdbool.h>
#include <limits.h>
#include <pthread.h>

#include "table.h"

#define SOCKETERROR (-1)
#define THREAD_POOL_SIZE 20

typedef struct sockaddr_in SA_IN;
typedef struct sockaddr SA;

pthread_mutex_t thread_lock = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t mutex_signal = PTHREAD_COND_INITIALIZER;

void* handle_connection(void* client_socket);
void* thread_work_job(void* arg);
int check(int exp, const char* msg);

pthread_t* thread_pool_init(void);

#endif
