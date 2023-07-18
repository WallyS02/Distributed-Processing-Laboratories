#include "list.h"

void push(list *lista, void* value, bool (*compare)(void*, void*), size_t data_size){
    sem_wait(&lista->semaphore);
    if(lista->head == NULL){
        lista->head = (element *) malloc(sizeof(element));
        lista->head->data = malloc(data_size);
        for (int i = 0; i < data_size; i++)
            *(char *)(lista->head->data + i) = *(char *)(value + i);
        lista->head->next = NULL;
        lista->head->previous = NULL;
        lista->tail = lista->head;
    }
    else {
        element *temp = lista->head;
        element *newElement = (element *) malloc(sizeof(element));
        newElement->data = malloc(data_size);
        for (int i = 0; i < data_size; i++)
            *(char *)(newElement->data + i) = *(char *)(value + i);
        while(temp != NULL){
            if((*compare)(temp->data, value)) {
                if (temp->previous == NULL) {
                    newElement->next = temp;
                    newElement->previous = NULL;
                    temp->previous = newElement;
                    temp = newElement;
                    lista->head = temp;
                    sem_post(&lista->semaphore);
                    return;
                }
                else {
                    element *prev = temp->previous;
                    newElement->next = temp;
                    newElement->previous = prev;
                    prev->next = newElement;
                    temp->previous = newElement;
                    sem_post(&lista->semaphore);
                    return;
                }
            }
            else {
                temp = temp->next;
            }
        }
        temp = lista->tail;
        newElement->previous = temp;
        newElement->next = NULL;
        temp->next = newElement;
        temp = newElement;
        lista->tail = temp;
    }
    sem_post(&lista->semaphore);
}

void removeElement(list *lista, void *value, void (*deletePtr)(void *), bool (*compareEqualPtr)(void *, void *)){
    sem_wait(&lista->semaphore);
    if(lista->head != NULL) {
        if(lista->head == lista->tail && (*compareEqualPtr)(lista->head->data, value) == 1){
            lista->head = NULL;
            lista->tail = NULL;
        }
        element *temp = lista->head;
        while(temp != NULL) {
            if ((*compareEqualPtr)(temp->data, value) == 1) {
                if (temp->next == NULL) {
                    element *deletion = temp;
                    temp = temp->previous;
                    temp->next = NULL;
                    lista->tail = temp;
		            (*deletePtr)(deletion->data);
                    free(deletion);
                    sem_post(&lista->semaphore);
                    return;
                } else if (temp->previous == NULL) {
                    element *deletion = temp;
                    temp = temp->next;
                    temp->previous = NULL;
                    lista->head = temp;
                    (*deletePtr)(deletion->data);
                    free(deletion);
                    sem_post(&lista->semaphore);
                    return;
                } else {
                    element *deletion1 = temp;
                    temp = temp->previous;
                    element *deletion2 = deletion1->next;
                    temp->next = deletion2;
                    deletion2->previous = temp;
		            (*deletePtr)(deletion1->data);
                    free(deletion1);
                    sem_post(&lista->semaphore);
                    return;
                }
            }
            temp = temp->next;
        }
    }
    sem_post(&lista->semaphore);
}

void print(element *root, void (*printPtr)(void *)){
    if(root != NULL){
        while(root != NULL) {
            (*printPtr)(root->data);
            printf(" ");
            root = root->next;
        }
    }
}

void dealocate(list *lista, void (*deletePtr)(void *)){
    if(lista->head != NULL){
        while(lista->head!= NULL) {
            element *temp;
            temp = lista->head;
            lista->head = lista->head->next;
            (*deletePtr)(temp->data);
            free(temp);
        }
    }
    sem_destroy(&lista->semaphore);
    free(lista);
}

list* List() {
    list* lista = (list*)malloc(sizeof(list));
    lista->head = NULL;
    lista->tail = NULL;
    sem_init(&lista->semaphore, 0, 1);
    return lista;
}
