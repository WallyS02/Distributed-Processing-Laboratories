#ifndef _TYPES_H_
#define _TYPES_H_

#include <semaphore.h>

typedef struct element {
    struct element *next;
    struct element *previous;
    void *data;
} element;

typedef struct list {
    element *head;
    element *tail;
    sem_t semaphore;
} list;

#endif
