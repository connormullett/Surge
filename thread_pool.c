
#include "thread_pool.h"


void* thread_work_job(void* arg) {
    table_t* table = (table_t*)arg;

    while (true) {
        int* pclient;
        pthread_mutex_lock(&queue_thread_lock);

        if ((pclient = dequeue()) == NULL) {
            pthread_cond_wait(&mutex_signal, &queue_thread_lock);

            pclient = dequeue();
        }
        pthread_mutex_unlock(&queue_thread_lock);

        if (pclient != NULL) {

            printf("Connection: handling job %d\n", *pclient);
            handle_connection(pclient, table);
        }
    }
}


void* handle_connection(void* p_client_socket, table_t* table) {
    int client_socket = *((int*)p_client_socket);
    free(p_client_socket);
    char buffer[BUFSIZE];
    size_t bytes_read;
    int msgsize = 0;
    char* value = NULL;
    

    // read from socket into buffer
    while((bytes_read = read(client_socket, buffer + msgsize, sizeof(buffer) - msgsize)) > 0) {
        msgsize += bytes_read;
        if (msgsize > BUFSIZE - 1 || buffer[msgsize - 1] == '\n') break;
    }

    check(bytes_read, "read error");
    buffer[msgsize-1] = 0;
    fflush(stdout);

    request_t* request = parse_request_t(buffer);

    if (validate_request(request)) {

        pthread_mutex_lock(&table_thread_lock);

        if ( strcmp(request->operation, "get") == 0 ) {
            value = table_t_get(table, request->key);

        } else if ( strcmp(request->operation, "set") == 0 ) {
            table_t_set(table, request->key, request->value);

        } else if (strcmp(request->operation, "del") == 0 ) {
            table_t_delete(table, request->key);

        } else {
            value = "error has occured";
        }

        pthread_mutex_unlock(&table_thread_lock);
    }

    if (value == NULL) {
        value = "OK";
    }

    write(client_socket, value, strlen(value));

    free(request);
    close(client_socket);
    return NULL;
}

bool validate_request(request_t* request) {

    return true;
}

request_t* init_request_t(void) {
    request_t* out = (request_t*)malloc(sizeof(request_t));
    out->operation = NULL;
    out->key = NULL;
    out->value = NULL;
    return out;
}


request_t* parse_request_t(char* buffer) {
    request_t* request = init_request_t();

    request->operation = strtok(buffer, "\t");
    request->key = strtok(NULL, "\t");
    request->value = strtok(NULL, "\t");

    return request;
}


pthread_t* thread_pool_init(table_t* t) {
    pthread_t* thread_pool = (pthread_t*)malloc(sizeof(pthread_t) * THREAD_POOL_SIZE);
    for (int i = 0; i < THREAD_POOL_SIZE; i++) {
        pthread_create(&thread_pool[i], NULL, thread_work_job, t);
    }
    return thread_pool;
}


int check(int exp, const char* msg) {
    if (exp == SOCKETERROR) {
        perror(msg);
        exit(1);
    }
    return exp;
}
