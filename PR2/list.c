#include "list.h"

void push(list *lista, void* value, bool (*compare)(void*, void*), size_t data_size){
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
                    return;
                }
                else {
                    element *prev = temp->previous;
                    newElement->next = temp;
                    newElement->previous = prev;
                    prev->next = newElement;
                    temp->previous = newElement;
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
}

void removeElement(list *lista, int position, void (*deletePtr)(void *)){
    if(lista->head != NULL) {
        if(lista->head == lista->tail && position == 0){
            lista->head = NULL;
            lista->tail = NULL;
        }
        int depth = 0;
        element *temp = lista->head;
        while(temp != NULL) {
            if (depth == position) {
                if (temp->next == NULL) {
                    element *deletion = temp;
                    temp = temp->previous;
                    temp->next = NULL;
                    lista->tail = temp;
		    (*deletePtr)(deletion->data);
                    free(deletion);
                    return;
                } else if (temp->previous == NULL) {
                    element *deletion = temp;
                    temp = temp->next;
                    temp->previous = NULL;
                    lista->head = temp;
		    (*deletePtr)(deletion->data);
                    free(deletion);
                    return;
                } else {
                    element *deletion1 = temp;
                    temp = temp->previous;
                    element *deletion2 = deletion1->next;
                    temp->next = deletion2;
                    deletion2->previous = temp;
		    (*deletePtr)(deletion1->data);
                    free(deletion1);
                    return;
                }
            }
            temp = temp->next;
            depth++;
        }
    }
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
    free(lista);
}

list* List() {
    list* lista = (list*)malloc(sizeof(list));
    lista->head = NULL;
    lista->tail = NULL;
    return lista;
}
