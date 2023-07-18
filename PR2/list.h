#ifndef _LIST_H_
#define _LIST_H

#include "types.h"
#include <stdio.h>
#include <string.h>
#include <malloc.h>
#include <stdbool.h>


void push(list *lista, void* value, bool (*compare)(void*, void*), size_t data_size);
void removeElement(list *lista, int position, void (*deletePtr)(void *));
void print(element *root, void (*printPtr)(void *));
void dealocate(list *lista, void (*deletePtr)(void *));
list* List();

#endif
