#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <semaphore.h>
#include <time.h>


#define MAX 100
#define SIZE_BUFFER 100

int *rawData[SIZE_BUFFER]; //initial buffer
int *proData[SIZE_BUFFER]; //final buffer
int *dataSizes;            //stores the sizes of all produced and stored arrays in order of production.
int query_buffer;          //a single integer that updates everytime a new query is generated. 

//a new query is processed only when the previous query has been satisfied. 

int rawdc = 0;		//number of elements in the initial buffer
int prodc = 0;		//number of elements in the final buffer

// sem_t SearchComplete;
int searchComplete;	//I used semaphores in place of this later
// sem_t queryEmpty;

// sem_t full[MAX];
sem_t search_signal[MAX];	//when an ith array is sorted search_signal tells the thread[i] to begin its operations
sem_t query_signal[MAX];	//when a new query is generated, the searching threads are signalled to start searching
sem_t search_complete[MAX];	//when the search is complete, this tells the generator to take up next query
// sem_t empty[MAX];

pthread_mutex_t rawDataLock;     

// pthread_mutex_t proDataLock;
// pthread_mutex_t rawdcLock;
// pthread_mutex_t prodcLock;

pthread_cond_t rawDataCond; 	//ensures that sorting begins only after a new array has been inserted into the intial buffer.

void *producer()
{
    for (int i = 0; i < SIZE_BUFFER; i++)
    {
        pthread_mutex_lock(&rawDataLock);
        int size_of_new_array = rand() % 100 + 100;
        dataSizes[rawdc] = size_of_new_array;
        rawData[rawdc] = malloc(size_of_new_array * sizeof(int));
        for (int j = 0; j < size_of_new_array; j++)
        {
            rawData[rawdc][j] = rand() % 1000;
        }
        rawdc += 1;
        pthread_cond_signal(&rawDataCond);
        pthread_mutex_unlock(&rawDataLock);
        printf("written to rawData\n");
        sleep(2);
    }
}

void swap(int *xp, int *yp)
{
    int temp = *xp;
    *xp = *yp;
    *yp = temp;
}

void selectionSort(int arr[], int n)
{
    int i, j, min_idx;
    for (int i = 0; i < n - 1; i++)
    {
        min_idx = i;
        for (j = i + 1; j < n; j++)
        {
            if (arr[j] < arr[min_idx])
                min_idx = j;

            swap(&arr[min_idx], &arr[i]);
        }
    }
}

void *sorter_thread()
{
    for (int i = 0; i < MAX; i++)
    {
        pthread_mutex_lock(&rawDataLock);
        while (dataSizes[i] == 0)
        {
            //wait;
            pthread_cond_wait(&rawDataCond, &rawDataLock);
        }
        proData[i] = malloc(dataSizes[i] * sizeof(int));
        
        for (int j = 0; j < dataSizes[i]; j++)
        {
            proData[i][j] = rawData[i][j];
            printf("%d ", proData[i][j]);
        }
        printf("\n");
        // printf("yaha tak aaya ya nahi?\n"); //aa gaya
        selectionSort(proData[i], dataSizes[i]);
        // for (int j = 10; j < dataSizes[i]; j++)
        // {
        //     printf("%d ", proData[i][j]);
        // }
        printf("\n-+-+-+-+-+-+-+-+-+-+-+-+-+-+\n");
        printf("Sorting Array %d complete.\n", i);
        printf("-+-+-+-+-+-+-+-+-+-+-+-+-+-+\n");
        printf("\n");
        prodc += 1;
        sem_post(&search_signal[i]);

        pthread_mutex_unlock(&rawDataLock);
    }
}

void *explorer(void *args)
{
    int index = *(int *)args;
    sem_wait(&search_signal[index]);
    while (1)
    {
        sem_wait(&query_signal[index]);
        int query_number = query_buffer;
        int len_array = dataSizes[index];
        for (int i = 0; i < len_array; i++)
        {
            if (proData[index][i] == query_number)
            {
                printf("Array Id: %d || Query_Index: %d || Query: %d \n", index, i, query_number);
                // printf("====> Found %d at location %d in Array %d\n", query_number, i, index);
                // Found 33 at location 1 in Array 67
            }
        }
        // searchComplete++;
        sem_post(&search_complete[index]);
    }
    free(args);
}

void *query_generator(void *args)
{
    sleep(1);
    while (1)
    {
        //generate a random number
        int x = rand() % 1000;

        pthread_mutex_lock(&rawDataLock);
        int num_arrays = prodc;
        //num_arrays are the number of arrays currently present inside the finalBuffer
        //every query is of form of
        //num_arrays, x
        for (int i = 0; i < num_arrays; i++)
        {
            //this for loop waits for all the threads
            //to complete their running searches.
            sem_wait(&search_complete[i]);
        }
        query_buffer = x;
        for (int i = 0; i < num_arrays; i++)
        {
            //then this for loop wakes up #(num_arrays) threads
            sem_post(&query_signal[i]);
        }
        pthread_mutex_unlock(&rawDataLock);
        sleep(1);
    }
}

int main(int argc, char *argv[])
{
    printf("\n");
    printf("**************************************************************\n\n");
    printf("Lab-7 (Automated workflows) © Pulkit Mahajan\n\n");
    printf("**************************************************************\n\n");

    srand(time(NULL));
    dataSizes = (int *)calloc(SIZE_BUFFER, sizeof(int));
    // proDataSizes = (int*)calloc(SIZE_BUFFER, sizeof(int));
    pthread_mutex_init(&rawDataLock, NULL);
    // pthread_mutex_init(&proDataLock, NULL);
    // pthread_mutex_init(&rawdcLock, NULL);
    // pthread_mutex_init(&prodcLock, NULL);
    pthread_cond_init(&rawDataCond, NULL);

    for (int i = 0; i < MAX; i++)
    {
        sem_init(&search_signal[i], 0, 0);
        sem_init(&search_complete[i], 0, 1);
        sem_init(&query_signal[i], 0, 0);
    }

    pthread_t writer; //there is only one producer;
    pthread_t sorter;
    pthread_t query_thread;
    if (pthread_create(&writer, NULL, &producer, NULL))
    {
        perror("unable to create producer thread\n");
        return 1;
    }
    if (pthread_create(&sorter, NULL, &sorter_thread, NULL))
    {
        perror("unable to create sorter thread\n");
        return 2;
    }
    if (pthread_create(&query_thread, NULL, &query_generator, NULL))
    {
        perror("unable to create a user simulation thread\n");
        return 3;
    }

    pthread_t hound[MAX];
    for (int i = 0; i < MAX; i++)
    {
        int *index = malloc(sizeof(int));
        *index = i;
        if (pthread_create(&hound[i], NULL, &explorer, index))
        {
            perror("unable to generate a hound\n");
            return 4;
        }
    }

    if (pthread_join(writer, NULL))
    {
        perror("unable to join producer thread\n");
        return 1;
    }
    if (pthread_join(sorter, NULL))
    {
        perror("unable to join sorter thread\n");
        return 2;
    }
    if (pthread_join(query_thread, NULL))
    {
        perror("unable to join query generator\n");
        return 3;
    }
    for (int i = 0; i < MAX; i++)
    {
        if (pthread_join(hound[i], NULL))
        {
            perror("hound lost\n");
            return 4;
        }
    }

    pthread_cond_destroy(&rawDataCond);
    pthread_mutex_destroy(&rawDataLock);
    // pthread_mutex_destroy(&proDataLock);
    // pthread_mutex_destroy(&rawdcLock);
    // pthread_mutex_destroy(&prodcLock);
    for (int i = 0; i < MAX; i++)
    {
        sem_destroy(&search_signal[i]);
        sem_destroy(&query_signal[i]);
        sem_destroy(&search_complete[i]);
    }
    return 0;
}
