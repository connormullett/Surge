
#ifndef _WORK_QUEUE_H_
#define _WORK_QUEUE_H_


typedef struct node {
    struct node* next;
    int* client_socket;
} node_t;

void enqueue(int* client_socket);
int* dequeue();


#endif
