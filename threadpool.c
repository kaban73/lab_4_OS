/**
 * Implementation of thread pool.
 */

#include <pthread.h>
#include <stdlib.h>
#include <stdio.h>
#include <semaphore.h>
#include "threadpool.h"

#define QUEUE_SIZE 10
#define NUMBER_OF_THREADS 3

#define TRUE 1

// this represents work that has to be 
// completed by a thread in the pool
typedef struct 
{
    void (*function)(void *p);
    void *data;
}
task;

// mutex and semaphore
pthread_mutex_t lock;
sem_t taskCount;

// the work queue
task worktodo;
task queue[QUEUE_SIZE + 1];
int head = 0;
int tail = 0;

// the worker bee
pthread_t bee[NUMBER_OF_THREADS];

// insert a task into the queue
// returns 0 if successful or 1 otherwise, 
int enqueue(task t) 
{
    return 0;
}

// remove a task from the queue
task dequeue() 
{
    return worktodo;
}

// the worker thread in the thread pool
void *worker(void *param)
{
    // execute the task
    execute(worktodo.function, worktodo.data);

    pthread_exit(0);
}

/**
 * Executes the task provided to the thread pool
 */
void execute(void (*somefunction)(void *p), void *p)
{
    (*somefunction)(p);
}

/**
 * Submits work to the pool.
 */
int pool_submit(void (*somefunction)(void *p), void *p)
{
    worktodo.function = somefunction;
    worktodo.data = p;

    int response = enqueue(worktodo);
    if (!response)
        sem_post(&taskCount);

    return response;
}

// initialize the thread pool
void pool_init(void)
{
    pthread_mutex_init(&lock, NULL);
    sem_init(&taskCount, 0, 0);
    for (int i = 0; i < NUMBER_OF_THREADS; i++)
        pthread_create(&bee[i],NULL,worker,NULL);
}

// shutdown the thread pool
void pool_shutdown(void)
{
    sem_destroy(&taskCount);
    pthread_mutex_destroy(&lock);
    for (int i = 0; i < NUMBER_OF_THREADS; i++) {
        pthread_cancel(bee[i]);
        pthread_join(bee[i],NULL);
    }
}
