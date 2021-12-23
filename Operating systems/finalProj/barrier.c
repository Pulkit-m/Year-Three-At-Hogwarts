#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

pthread_barrier_t barrier;
//now there does not have to be separate
//for the barrier wait. As soon as the number of 
//waiting threads the count parameter, the 
//barrier is released.

void* routine(void* arg){
    printf("Waiting at the barrier\n");
    sleep(rand()%10+4);
    pthread_barrier_wait(&barrier);
    printf("Barrier crossed\n");
}
void* routine2(void* arg){
    printf("thread of different type waiting\n");
    printf("thread of type2 allowed to pass\n");
}

int main(int argc, char* argv[])
{
    pthread_t th[4];
    pthread_t th1[6];
    pthread_barrier_init(&barrier, NULL, 6);
    for(int i= 0 ; i < 4; i++)
    {
        if(pthread_create(&th[i], NULL, &routine, NULL)){
            perror("error in creating a thread.\n");
        }
    }
    for(int i = 0 ; i < 6; i++){
        pthread_create(&th1[i], NULL, &routine2, NULL);
    }
    for(int i = 0 ; i < 4; i++)
    {
        if(pthread_join(th[i], NULL)){
            perror("error in joining thread\n");
        }
    }

    pthread_barrier_destroy(&barrier);
    return 0;
}