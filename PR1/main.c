#include <stdio.h>
#include <string.h>
#include <malloc.h>
#include <stdbool.h>

typedef struct element {
    struct element *next;
    struct element *previous;
    void *data;
} element;

typedef struct list {
    element *head;
    element *tail;
} list;

typedef struct Computer {
    int RAM_Capacity;
    int processor_core_number;
    float weight;
} Computer;

bool compareComputer(void *value1, void *value2){	// if greater value1 -> true
    Computer* comp1 = (Computer*)value1;
    Computer* comp2 = (Computer*)value2;
    if(comp1->RAM_Capacity > comp2->RAM_Capacity || comp1->RAM_Capacity < comp2->RAM_Capacity)
        return comp1->RAM_Capacity > comp2->RAM_Capacity;
    else if(comp1->processor_core_number > comp2->processor_core_number || comp1->processor_core_number < comp2->processor_core_number)
        return comp1->processor_core_number > comp2->processor_core_number;
    else if(comp1->weight > comp2->weight || comp1->weight < comp2->weight)
        return comp1->weight > comp2->weight;
}

bool compareInt(void *value1, void *value2){	// if greater value1 -> true
    return *(int *)value1 > *(int *)value2;
}

void printComputer(void *c){
    Computer* computer = (Computer*)c;
    printf("{\nRAM Capacity: %d\n", computer->RAM_Capacity);
    printf("Processor Core Number: %d\n", computer->processor_core_number);
    printf("Weight: %f\n}\n", computer->weight);
}

void printInt(void *i){
    printf("%d", *(int *)i);
}

void deleteComputer(void *computer){
    Computer* c = (Computer*)computer;
    free(c);
}

void deleteInt(void *number){

}

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

int main() {
    list* lista = List();
    char command[7];
    printf("Select list data type (int or computer): ");
    char listDataType[4];
    scanf("%s", listDataType);
    printf("User interface:\n"
           "push - push element on list\n1. value\n"
           "remove - remove element at given position of list\n1. position\n"
           "print - print list\n"
           "exit - shut down\n");
    do {
        scanf("%s", command);
        if(strcmp(command, "push") == 0) {
            if(strcmp(listDataType, "int") == 0) {
                int value;
                scanf("%d", &value);
                push(lista, &value, compareInt, sizeof(int));
            }
            else {
                int RAM_Capacity, processor_core_number;
                float weight;
                scanf("%d", &RAM_Capacity);
                scanf("%d", &processor_core_number);
                scanf("%f", &weight);
                Computer *value = (Computer *) malloc(sizeof(Computer));
                value->RAM_Capacity = RAM_Capacity;
                value->processor_core_number = processor_core_number;
                value->weight = weight;
                push(lista, value, compareComputer, sizeof(Computer));
            }
        }
        else if(strcmp(command, "remove") == 0) {
            int position;
            scanf("%d", &position);
	    if(strcmp(listDataType, "int") == 0)
            	removeElement(lista, position, deleteInt);
	    else removeElement(lista, position, deleteComputer);
        }
        else if(strcmp(command, "print") == 0) {
            if(strcmp(listDataType, "int") == 0)
                print(lista->head, printInt);
            else print(lista->head, printComputer);
            printf("\n");
        }
        else if(strcmp(command, "exit") == 0) {
            if(strcmp(listDataType, "int") == 0)
            	dealocate(lista, deleteInt);
	    else dealocate(lista, deleteComputer);
        }
        else {
            printf("Wrong Command, try again\n");
        }
    } while(strcmp(command, "exit") != 0);
    return 0;
}