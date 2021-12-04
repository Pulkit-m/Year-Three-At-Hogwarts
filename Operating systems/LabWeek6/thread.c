#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>


int x = 0 ;
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


int main(int argc, char* argv[])
{
	pthread_t t1, t2;
	pthread_create(&t1, NULL, &routine, NULL);
	pthread_create(&t2, NULL, &routine2, NULL);
	pthread_join(t1, NULL);
	pthread_join(t2, NULL);
	return 0;
}
