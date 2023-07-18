#ifndef _TYPES_H_
#define _TYPES_H_

typedef struct {
    struct element *next;
    struct element *previous;
    void *data;
} element;

typedef struct list {
    element *head;
    element *tail;
} list;

#endif
