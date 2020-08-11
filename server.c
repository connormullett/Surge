
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <stdbool.h>
#include <limits.h>
#include <pthread.h>

#include "work_queue.h"
#include "thread_pool.h"
#include "cli.h"
#include "table.h"

#define SERVER_PORT 5250
#define SERVER_BACKLOG 100

typedef struct sockaddr_in SA_IN;
typedef struct sockaddr SA;

int main() {

    int server_socket, client_socket, addr_size;
    SA_IN server_addr, client_addr;

    table_t* t = table_t_init();
    thread_pool_init(t);

    check((server_socket = socket(AF_INET, SOCK_STREAM, 0)),
        "Failed to create socket");
    
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = INADDR_ANY;
    server_addr.sin_port = htons(SERVER_PORT);

    check(bind(server_socket, (SA*)&server_addr, sizeof(server_addr)),
        "Bind Failed");
    check(listen(server_socket, SERVER_BACKLOG), 
        "Listen Failed");
    
    printf("Listening on %d\n", SERVER_PORT);

    // starts cli thread
    pthread_t* cli_thread = (pthread_t*)malloc(sizeof(pthread_t));
    pthread_create(cli_thread, NULL, command_line, t);

    while(true) {
        addr_size = sizeof(SA_IN);

        check(client_socket = accept(server_socket,
            (SA*)&client_addr, (socklen_t*)&addr_size),
            "accept failed");

        int *pclient = malloc(sizeof(int));
        *pclient = client_socket;

        pthread_mutex_lock(&queue_thread_lock);
        enqueue(pclient);
        pthread_mutex_unlock(&queue_thread_lock);
        pthread_cond_signal(&mutex_signal);
        handle_connection(pclient, t);
    }

    return 0;
}
