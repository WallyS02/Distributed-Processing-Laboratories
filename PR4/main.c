#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include "list.h"

int N = 10000;
int thread_number = 2;

typedef struct args {
    list* lista;
    int i;
} args;

void deleteInt(void *number){

}

bool compareInt(void *value1, void *value2){	// if greater value1 -> true
    return *(int *)value1 > *(int *)value2;
}

bool compareIntEqual(void *value1, void *value2){
    return *(int *)value1 == *(int *)value2;
}

void printInt(void *i){
    printf("%d", *(int *)i);
}

void *adding(void *arg){
    args *param = (args *)arg;
    list* lista = param->lista;
    int i = param->i;
    for(int j = 0; j < N; j++) {
        int value = (N * i + j);
        push(lista, &value, compareInt, sizeof(int));
    }
    return NULL;
}

void *removing(void *arg){
    args *param = (args *)arg;
    list* lista = param->lista;
    int i = param->i;
    for(int j = 0; j < N; j++) {
        int value = (N * i + j);
        removeElement(lista, &value, deleteInt, compareIntEqual);
    }
    return NULL;
}

int main() {
    list* lista = List();
    pthread_t threads[thread_number];
    args* arg[thread_number];
    for(int i = 0; i < thread_number / 2; i++) {
        arg[i] = (args*)malloc(sizeof(args));
        arg[i]->lista = lista;
        arg[i]->i = i;
        pthread_create(&threads[i], NULL, adding, (void *) arg[i]);
    }
    for(int i = 0; i < thread_number / 2; i++) {
        pthread_join(threads[i], NULL);
        free(arg[i]);
    }
    for(int i = thread_number / 2; i < thread_number; i++) {
        arg[i] = (args*)malloc(sizeof(args));
        arg[i]->lista = lista;
        arg[i]->i = i - (thread_number / 2);
        pthread_create(&threads[i], NULL, removing, (void *) arg[i]);
    }
    for(int i = thread_number / 2; i < thread_number; i++) {
        pthread_join(threads[i], NULL);
        free(arg[i]);
    }
    if(lista->head == NULL && lista->tail == NULL)
        printf("Empty list!\n");
    else
        printf("Not empty list!\n");
    dealocate(lista, deleteInt);
    return 0;
}