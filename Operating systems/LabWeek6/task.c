#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <time.h>

#define MAX 16

struct arglims_t{
    int start; 
    int end;
    int thread_no; 
};

int ARRAY[MAX];

void* fill_a_place(void* arg)
{
    int index = *(int*)arg;
    ARRAY[index] = rand()%MAX;
    free(arg);
}

void* readArray(void* arg)
{
    int thread_no = *(int*)arg;
    for(int i = 0 ; i < MAX; i++)
    {
        printf("Thread %d [%d]: %d \n", thread_no+1,i, ARRAY[i]);
    }
    free(arg);
}

void* restrictedRead(void* arg)
{
    struct arglims_t* limit = (struct arglims_t*)arg;
    int start = limit->start;
    int end = limit->end;
    int thread_no = limit->thread_no;
    if(start < 0 || end>=MAX){
        perror("Access range out of bounds\n");
    }
    for(int i = start; i<= end; i++ )
    {
        printf("Thread %d [%d]: %d\n", thread_no+1,i, ARRAY[i]);
    }
    // free(arg);
}

int main(int argc, char* argv[])
{
    srand(time(NULL));
    printf("2019ucs0073 @pulkit_mahajan\n");
    printf("TASK - 1: Creating MAX number of threads to write to the global array\n");
    pthread_t thread[MAX];
    for(int i = 0 ; i < MAX; i++)
    {
        int* index = malloc(sizeof(int));
        *index = i;
        if(pthread_create(&thread[i], NULL, &fill_a_place,index)){
            perror("Error in creating a thread\n");
            return 1;
        }
    }

    for(int i = 0 ; i < MAX; i++)
    {
        if(pthread_join(thread[i], NULL)){
            perror("Error in joining a thread\n");
            return 2;
        }
        printf("%d ", ARRAY[i]);
    }printf("\n");


    printf("\n\n\nTASK - 2a: Consumer thread can read from any array location.\n");
    printf("for this I declare n consumer threads and make them print out the complete ARRAY to show that all the threads can access any location inside the array\n");

    int n;
    printf("How many consumer threads?: \n");
    scanf("%d", &n);
    pthread_t consumer[n];
    for(int i = 0 ; i < n; i++)
    {
        int* thread_no = malloc(sizeof(int));
        *thread_no = i;
        if(pthread_create(&consumer[i], NULL, &readArray, thread_no)){
            perror("Error in creating a thread\n");
            return 3;
        }
    }

    for(int i = 0 ; i < n ; i++)
    {
        if(pthread_join(consumer[i], NULL)){
            perror("Error in joining thread.\n");
            return 4;
        }
    }

    printf("\n\nTASK - 2b: Consumer threads are restricted to specific locations inside the array only. A consumer thread cannot read outside its permissible indices: \n");

    struct arglims_t limits[n];
    for(int i = 0 ; i < n ;i++)
    {
        int start, end;
        printf("Specify range for thread %d: \n", i);
        scanf("%d %d",&start, &end);
        limits[i].start = start;
        limits[i].end = end;
        limits[i].thread_no = i;
    }

    pthread_t consumer2[n];
    for(int i = 0 ; i < n; i++)
    {
        // int* index = malloc(sizeof(int));
        // *index = i;
        struct arglims_t* arg = malloc(sizeof(struct arglims_t));
        arg = &limits[i]; 
        if(pthread_create(&consumer2[i], NULL, &restrictedRead, arg)){
            perror("Error in creating thread\n");
            return 5;
        }
    }

    for(int i = 0 ; i < n; i++)
    {
        if(pthread_join(consumer2[i], NULL)){
            perror("Error in joining thread\n");
            return 6;
        }
    }

    return 0;
}