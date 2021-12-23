#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>
#include <unistd.h>
#include <time.h>
#define THREAD_NUM 2

//manage shared memory access
//what if buffer is overflowing: more producers than consumers
//what if buffer is staying empty: more consumers than producer

/*
a note on sharing a lock between different routines. 
This still works.
suppose you have two separate routines, routine 1 has a code snippet A and routine 2 has a code snippet B
snippet A and B should access a shared buffer that should be accessed simultaneously. Then lock will definitely work
suppose A holds the lock. At the same time B won't be able to run because it will try to acquire the lock that A already 
holds; hence B will have to wait until A leaves the lock;
*/


pthread_mutex_t bufferlock;
sem_t emptySlots; 
sem_t fullSlots; 

int buffer[10];
int iterator =0; 

void* producer(void* arg){
    while(1){
        //produce
        int x = rand()%100; 

        sem_wait(&emptySlots);
        pthread_mutex_lock(&bufferlock);
        if(iterator< 10){
        //store in buffer
        buffer[iterator] = x;
        iterator++;
        }else{
            printf("skipped request\n");
        }
        pthread_mutex_unlock(&bufferlock);
        sem_post(&fullSlots);
    }
}

void* consumer(void* arg){
    while(1){
        int x = -1;
        sem_wait(&fullSlots);
        pthread_mutex_lock(&bufferlock);
        if(iterator>0){    
        //remove
            x = buffer[iterator-1];
            iterator--;
        }
        pthread_mutex_unlock(&bufferlock);
        sem_post(&emptySlots);
        //consume
        printf("Retrieved: %d\n",x);
        }
}

int main(int argc, char* argv[])
{
    srand(time(NULL));
    pthread_mutex_init(&bufferlock, NULL);
    sem_init(&emptySlots, 0, 10);
    sem_init(&fullSlots, 0, 0);
    pthread_t th[THREAD_NUM];
    for(int i = 0 ; i < THREAD_NUM; i++)
    {
        if(i%2==0){
            if(pthread_create(&th[i], NULL, &producer, NULL)){
                perror("unable to create thread\n");
                return 1;
            }
        }
        else{
            if(pthread_create(&th[i], NULL, consumer, NULL)){
                perror("unable to create a thread\n");
                return 1;
            }
        }
    }

    for(int i = 0  ;i < THREAD_NUM; i++){
        if(pthread_join(th[i], NULL)){
            perror("unable to join thread\n");
            return 1;
        }
    }

    pthread_mutex_destroy(&bufferlock);
    sem_destroy(&emptySlots);
    sem_destroy(&fullSlots);
    return 0;
}