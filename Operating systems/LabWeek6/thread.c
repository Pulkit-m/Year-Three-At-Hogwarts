#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <time.h>

int x = 0 ;
void* routine0()
{
	printf("first routine");
}

void* routine()
{
	//printf("Entering a thread\n");
	sleep(1);
	x+=10;
	sleep(1);
	printf("Value of x: %d\n",x);
}

void* routine2()
{
	x+=4;
	sleep(2);
	printf("value of x: %d\n", x);
}


int mails = 0;
void* routine3a()
{
	for(int i = 0 ; i < 100000 ; i++)
	{
		mails++;
	}
}

pthread_mutex_t mutex;
void* routine4()
{
	for(int i = 0 ; i < 100000; i++)
	{
		pthread_mutex_lock(&mutex);
		mails++;
		pthread_mutex_unlock(&mutex);
	}
}

void* rollDice()
{
	int value = (rand()%6)+1;
	int* result = malloc(sizeof(int));
	*result = value;
	return (void*)result;
}

int primes[10] = {2,3,5,7,11,13,17,19,23,29};

void* getPrime(void* arg)
{
	int index = *(int*)arg;
	int *value = malloc(sizeof(int));
	*value = primes[index];
	printf("%d ", primes[index]);
	free(arg);
	return (void*)value;
}


int main(int argc, char* argv[])
{

	pthread_t th[10];
	for(int i= 0 ; i < 10 ; i++)
	{
		int* a = malloc(sizeof(int));
		*a = i;
		if(pthread_create(&th[i], NULL, &getPrime, a)){
			perror("thread was not created.");
			return 1;
		}
	}


	int prime_copy[10];
	for(int i = 0 ; i < 10; i++)
	{
		int *res;
		if(pthread_join(th[i], (void**)&res))
		{
			perror("Error in joining thread\n");
			return 2;
		}
		prime_copy[i] = *res;
	}
	printf("\n");
	for(int i = 0 ; i < 10 ; i++)
	{
		printf("Value at %d: %d \n", i, prime_copy[i]);
	}


	// pthread_t t;
	// srand(time(NULL));
	// int **res; 
	// if(pthread_create(&t, NULL, &rollDice, NULL)){
	// 	perror("Error in creating a thread\n");
	// 	return 1;
	// }
	// if(pthread_join(t, (void**)res)){
	// 	perror("Error in joining back thread\n");
	// 	return 2;
	// }

	// printf("After rolling dice: %d\n",**res);
	// free(*res);
	



	// pthread_t t1, t2;
	// pthread_mutex_init(&mutex, NULL);
	// pthread_create(&t1, NULL, &routine4, NULL);
	// pthread_create(&t2, NULL, &routine4, NULL);
	// pthread_join(t1, NULL);
	// pthread_join(t2, NULL);
	// pthread_mutex_destroy(&mutex);
	// printf("Number of mails: %d\n",mails);

	// pthread_t th[10];
	// pthread_mutex_init(&mutex, NULL);
	
	// for(int i = 0 ; i < 10 ;i++)
	// {
	// 	if(pthread_create(&th[i], NULL, &routine4, NULL)){
	// 		perror("failed to create thread");
	// 		return i+1;
	// 	}
	// }
	// for(int i = 0 ; i < 10 ; i++)
	// {
	// 	if(pthread_join(th[i], NULL)){
	// 		perror("failed to join thread\n");
	// 		return i+1+10;
	// 	}
	// }
	
	// pthread_mutex_destroy(&mutex);
	// printf("Number of mails: %d\n", mails);



	return 0;
}


//race condititon can only occur on multicore processors
