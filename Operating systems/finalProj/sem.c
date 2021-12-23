#include <semaphore.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>
#include <pthread.h>
#define THREAD_NUM 16
//scenario
//we have 12 seats available on the server and 16 consumer threads that need access to the server 

sem_t semaphore;


void* routine(void* arg){
    printf("player %d waiting in the login queue\n", *(int*)arg);
    sem_wait(&semaphore);
    printf("Player %d logged in...\n", *(int*)arg);
    sleep(rand()%5+1);
    printf("Player %d logged out\n", *(int*)arg);
    sem_post(&semaphore);
    free(arg);
    
}

int main(int argc, char* argv[])
{
    srand(time(NULL));
    sem_init(&semaphore, 0, 12);
    pthread_t th[THREAD_NUM];
    for(int i = 0 ; i < THREAD_NUM; i++)
    {
        int* a = malloc(sizeof(int));
        *a = i;
        if(pthread_create(&th[i], NULL, &routine,a)){
            perror("unable to create a thread\n");
            return 1;
        }
    }

    for(int i = 0 ; i < THREAD_NUM; i++)
    {
        if(pthread_join(th[i], NULL)){
            perror("unable to join thread\n");
            return 1;
        }
    }

    sem_destroy(&semaphore);
return 0;
}

