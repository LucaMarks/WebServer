#ifndef HTTPSERVER_THREADPOOL_H
#define HTTPSERVER_THREADPOOL_H

#include "../../DataStructures/Lists/Queue.h"
#include <pthread.h>

//This is used to create a provided number of threads awaiting work to be added to a queue

typedef struct ThreadJob {
    //the function that needs to be executed
    void *(*job)(void *arg);

    void *arg;
}ThreadJob;

typedef struct ThreadPool {
    int numThreads;

    struct Queue work;

    //control switch
    int active;
    pthread_t *pool;
    pthread_mutex_t lock;
    pthread_cond_t signal;

    void (*addWork)(struct ThreadPool *threadPool, ThreadJob *threadJob);
}ThreadPool;

ThreadPool threadPoolConstructor(int numThreads);
ThreadJob threadJobConstructor(void *(*job)(void *arg), void *arg);

void threadPoolDestructor(ThreadPool *);

#endif //HTTPSERVER_THREADPOOL_H