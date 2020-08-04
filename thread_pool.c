
#include "thread_pool.h"
#include "work_queue.h"


int check(int exp, const char* msg) {
    if (exp == SOCKETERROR) {
        perror(msg);
        exit(1);
    }
    return exp;
}

void* thread_work_job(void* arg) {
    while (true) {
        int* pclient;
        pthread_mutex_lock(&queue_thread_lock);

        // if nothing on the queue, wait
        if ((pclient = dequeue()) == NULL) {
            pthread_cond_wait(&mutex_signal, &queue_thread_lock);
            // when signaled, try to pop more clients off the queue
            pclient = dequeue();
        }
        pthread_mutex_unlock(&queue_thread_lock);

        if (pclient != NULL) {
            // handle the client on the socket
            handle_connection(pclient);
        }
    }
}


void* handle_connection(void* p_client_socket) {
    int client_socket = *((int*)p_client_socket);
    free(p_client_socket);
    char buffer[BUFSIZE];
    size_t bytes_read;
    int msgsize = 0;
    

    // read from socket into buffer
    while((bytes_read = read(client_socket, buffer + msgsize, sizeof(buffer) - msgsize)) > 0) {
        msgsize += bytes_read;
        if (msgsize > BUFSIZE - 1 || buffer[msgsize - 1] == '\n') break;
    }

    check(bytes_read, "read error");
    buffer[msgsize-1] = 0;

    fflush(stdout);

    // parse message from buffer into request
    request_t* request = parse_request_t(buffer);

    return NULL;
}


request_t* parse_request_t(char* buffer) {
    char* token;
    request_t* request = (request_t*)malloc(sizeof(request_t));
    char* copy = (char*)malloc(strlen(buffer) + 1);

    strcpy(copy, buffer);
    token = strtok(copy, "\t");  // split string by tab

    request->operation = token;

    while (token != NULL) {
        request->operation = strtok(NULL, "\t");
        token = strtok(NULL, "\t");
    }

    free(copy);
    copy = NULL;

    return request;
}


pthread_t* thread_pool_init(void) {
    pthread_t* thread_pool = (pthread_t*)malloc(sizeof(pthread_t) * THREAD_POOL_SIZE);
    for (int i = 0; i < THREAD_POOL_SIZE; i++) {
        pthread_create(&thread_pool[i], NULL, thread_work_job, &i);
    }
    return thread_pool;
}
