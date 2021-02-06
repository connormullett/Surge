
#include <stdlib.h>
#include <stdio.h>

#include <work_queue.h>

node_t* head = NULL;
node_t* tail = NULL;

void enqueue(int* client_socket) {
    node_t* new_node = (node_t*)malloc(sizeof(node_t));
    new_node->client_socket = client_socket;
    new_node->next = NULL;
    
    if (tail == NULL) {
        head = new_node;
    } else {
        tail->next = new_node;
    }

    tail = new_node;
}


void dump_queue() {
    node_t* tmp = head;
    puts("work queue :");

    while (tmp != NULL) {
        printf("%p -> %d\n", tmp, *(tmp->client_socket));
        tmp = tmp->next;
    }
}


int* dequeue() {
    if (head == NULL) {
        return NULL;
    }

    int* result = head->client_socket;
    node_t* tmp = head;

    head = head->next;

    if (head == NULL) { tail = NULL; }

    printf("%d", *result);

    free(tmp);
    return result;
}
