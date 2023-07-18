#include <stdio.h>
#include <unistd.h>
#include <pthread.h>
#include <fcntl.h>
#include <mqueue.h>

#define MONITOR_CHANNEL "/monitor_channel"
#define CONDITION_CHANNEL "/condition_channel"

typedef struct {
    int num_waiting;
    mqd_t monitor_mq;
    mqd_t condition_mq;
    pthread_mutex_t monitor_lock;
} Monitor;

Monitor monitorMain;
int sum = 0;

void init_monitor(Monitor* monitor) {
    struct mq_attr attr;
    attr.mq_flags = 0;
    attr.mq_maxmsg = 10;
    attr.mq_msgsize = sizeof(int);
    monitor->monitor_mq = mq_open(MONITOR_CHANNEL, O_CREAT | O_RDWR | O_NONBLOCK, 0666, &attr, NULL);
    monitor->condition_mq = mq_open(CONDITION_CHANNEL, O_CREAT | O_RDWR | O_NONBLOCK, 0666, &attr, NULL);
    pthread_mutex_init(&(monitor->monitor_lock), NULL);
    monitor->num_waiting = 0;
}

void monitor_enter(Monitor* monitor) {
    pthread_mutex_lock(&(monitor->monitor_lock));
}

void monitor_exit(Monitor* monitor) {
    if (monitor->num_waiting > 0) {
        int signal;
        mq_receive(monitor->condition_mq, (char*)&signal, sizeof(int), NULL);
    }
    pthread_mutex_unlock(&(monitor->monitor_lock));
}

void monitor_wait(Monitor* monitor) {
    monitor->num_waiting++;
    pthread_mutex_unlock(&(monitor->monitor_lock));
    int signal;
    mq_receive(monitor->condition_mq, (char*)&signal, sizeof(int), NULL);
    pthread_mutex_lock(&(monitor->monitor_lock));
    monitor->num_waiting--;
}

void monitor_notify(Monitor* monitor) {
    int signal = 1;
    mq_send(monitor->condition_mq, (const char*)&signal, sizeof(int), 0);
}

void* worker_thread(void* arg) {
    for(int i = 0; i < 100000; i++) {
        monitor_enter(&monitorMain);
        monitor_wait(&monitorMain);
        sum++;
        monitor_notify(&monitorMain);
        monitor_exit(&monitorMain);
    }
    pthread_exit(NULL);
}

int main() {
    init_monitor(&monitorMain);
    pthread_t threads[5];
    for (int i = 0; i < 5; i++) {
        pthread_create(&threads[i], NULL, worker_thread, NULL);
    }
    for (int i = 0; i < 5; i++) {
        pthread_join(threads[i], NULL);
    }
    printf("Sum: %d\n", sum);
    return 0;
}
