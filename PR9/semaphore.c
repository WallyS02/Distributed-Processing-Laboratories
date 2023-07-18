#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/ipc.h>
#include <sys/msg.h>

#define KEY 6969

struct message {
    long mtype;
    int value;
};

int sum = 0;
int semaphore_id;

void init_semaphore(int initial_value) {
    semaphore_id = msgget(KEY, IPC_CREAT | 0666);
    struct message msg;
    msg.mtype = 1;
    msg.value = initial_value;
    msgsnd(semaphore_id, &msg, sizeof(msg.value), IPC_NOWAIT);
}

void semaphore_wait() {
    struct message msg;
    msgrcv(semaphore_id, &msg, sizeof(msg.value), 0, IPC_NOWAIT);
    while (msg.value == 0) {
        usleep(10000);
        msgrcv(semaphore_id, &msg, sizeof(msg.value), 0, IPC_NOWAIT);
    }
    msg.value = 0;
    msgsnd(semaphore_id, &msg, sizeof(msg.value), IPC_NOWAIT);
}

void semaphore_signal() {
    struct message msg;
    msgrcv(semaphore_id, &msg, sizeof(msg.value), 0, IPC_NOWAIT);
    msg.value = 1;
    msgsnd(semaphore_id, &msg, sizeof(msg.value), IPC_NOWAIT);
}

void destroy_semaphore() {
    msgctl(semaphore_id, IPC_RMID, NULL);
}

void* worker_thread(void *arg) {
    for(int i = 0; i < 100000; i++) {
        semaphore_wait();
        sum++;
        semaphore_signal();
    }
    pthread_exit(NULL);
}

int main() {
    init_semaphore(1);
    pthread_t threads[5];
    for (int i = 0; i < 5; i++) {
        pthread_create(&threads[i], NULL, worker_thread, NULL);
    }
    for (int i = 0; i < 5; i++) {
        pthread_join(threads[i], NULL);
    }
    printf("Sum: %d\n", sum);
    destroy_semaphore();
    return 0;
}
