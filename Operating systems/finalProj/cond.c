#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <errno.h>

int fuel = 0;
pthread_mutex_t mutexFuel;
pthread_cond_t cond;

void* fillingFuel(void* arg)
{
    for(int i = 0 ; i < 10; i++)
    {
        pthread_mutex_lock(&mutexFuel);
        fuel+=10;
        // pthread_cond_signal(&cond);
        pthread_cond_broadcast(&cond);
        //if no one is waiting for a signal then the signal will have no effect!    
        printf("Filled fuel. Balance: %d\n", fuel);
        pthread_mutex_unlock(&mutexFuel);
        sleep(1);
    }
}

void* car(void* arg)
{
    pthread_mutex_lock(&mutexFuel);
    while(fuel<20){
        printf("no fuel! waiting\n");
        pthread_cond_wait(&cond, &mutexFuel);
    }
    fuel-=15;
    printf("got fueled, balance fuel: %d \n", fuel);
    pthread_mutex_unlock(&mutexFuel);
    
}

int main(int argc, char* argv[])
{
    pthread_mutex_init(&mutexFuel, NULL);
    pthread_cond_init(&cond, NULL);
    pthread_t th[4];

    for(int i = 0 ; i < 5 ; i++)
    {
        if(i==4){
            if(pthread_create(&th[i], NULL,&fillingFuel, NULL )){
                perror("failed to create thread\n");
            }
        }
        else{
            if(pthread_create(&th[i], NULL, &car, NULL)){
                perror("failed to create thread\n");
            }
        }
    }

    for(int i = 0 ; i < 2; i++)
    {
        if(pthread_join(th[i], NULL)){
            perror("failed to join thread\n");
        }
    }

    pthread_cond_destroy(&cond);
    pthread_mutex_destroy(&mutexFuel);
    return 0;
}