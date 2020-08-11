
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
#include "work_queue.h"

#define SOCKETERROR (-1)
#define THREAD_POOL_SIZE 20
#define BUFSIZE 4096

static pthread_mutex_t table_thread_lock = PTHREAD_MUTEX_INITIALIZER;
static pthread_mutex_t queue_thread_lock = PTHREAD_MUTEX_INITIALIZER;
static pthread_cond_t mutex_signal = PTHREAD_COND_INITIALIZER;


typedef struct request_t {
    char* operation;
    char* key;
    char* value;
} request_t;

void* handle_connection(void* client_socket, table_t* table);
void* thread_work_job(void* arg);
int check(int exp, const char* msg);
request_t* parse_request_t(char* buffer);
bool validate_request(request_t* request);

char* execute(table_t* table, char* buffer);

char* cli_get(table_t* t, request_t* request);
char* cli_set(table_t* t, request_t* request);
char* cli_del(table_t* t, request_t* request);
char* cli_dump(table_t* t, request_t* request);

pthread_t* thread_pool_init(table_t* t);

#endif
