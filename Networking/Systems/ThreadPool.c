#include "ThreadPool.h"

#include <stdlib.h>
#include <stdio.h>


void *genericThreadFunc(void *arg);

void addWork(ThreadPool *threadPool, ThreadJob *threadJob);

ThreadPool threadPoolConstructor(int numThreads) {
    ThreadPool threadPool;
    threadPool.numThreads = numThreads;
    threadPool.active = 1;
    threadPool.work = queue_constructor();

    threadPool.lock = (pthread_mutex_t) PTHREAD_MUTEX_INITIALIZER;
    threadPool.signal = (pthread_cond_t) PTHREAD_COND_INITIALIZER;
    pthread_mutex_lock(&threadPool.lock);
    threadPool.pool = malloc(sizeof(pthread_t[numThreads]));

    //create a thread for all requested threads
    for (int i = 0; i < numThreads; i++) {
        pthread_create(&threadPool.pool[i], NULL, genericThreadFunc, &threadPool);
    }

    pthread_mutex_unlock(&threadPool.lock);
    threadPool.addWork = addWork;
    return threadPool;
}

ThreadJob threadJobConstructor(void *(*job)(void *args), void *arg) {

    ThreadJob threadJob;
    threadJob.job = job;
    threadJob.arg = arg;
    return threadJob;

}

void threadPoolDestructor(ThreadPool *threadPool) {

    threadPool->active = 0;

    for (int i = 0; i < threadPool->numThreads; i++) {
        pthread_cond_signal(&threadPool->signal);
    }
    for (int i = 0; i < threadPool->numThreads; i++) {
        pthread_join(threadPool->pool[i], NULL);
    }

    free(threadPool->pool);
    queue_destructor(&threadPool->work);
}


void *genericThreadFunction(void *arg) {
    ThreadPool *threadPool = (ThreadPool *) arg;

    while (threadPool->active == 1) {
        //Lock the work queue
        pthread_mutex_lock(&threadPool->lock);
        pthread_cond_wait(&threadPool->signal, &threadPool->lock);

        ThreadJob threadJob = *(ThreadJob*)threadPool->work.peek(&threadPool->work);

        threadPool->work.pop(&threadPool->work);
        pthread_mutex_unlock(&threadPool->lock);

        //exec job
        if (threadJob.job) {
            threadJob.job(threadJob.arg);
        }
    }

    return NULL;
}


void addWork(ThreadPool *threadPool, ThreadJob *threadJob) {
    pthread_mutex_lock(&threadPool->lock);
    threadPool->work.push(*threadPool->work, &threadJob, sizeof(threadJob));
    pthread_cond_signal(&threadPool->signal);
    pthread_mutex_unlock(&threadPool->lock);
}