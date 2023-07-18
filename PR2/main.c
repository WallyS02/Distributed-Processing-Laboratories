#include "list.h"

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
