#include<stdlib.h>
#include<stdio.h>
#include<string.h>
#include<sys/wait.h>
#include<unistd.h>
#include<time.h>

void displayfile(FILE* fptr)
{
    fseek(fptr, 0, SEEK_SET);
    printf("The contents of newfile.txt are: \n");
    for(int i = 0 ; i < 100 ; i++)
    {
        int j = getw(fptr);
        if((i)%10==0)printf("\n");
        printf("%d ",j);

    }printf("\n");

}

int main(int argc, char* argv[])
    {
        FILE* chfile = fopen("newfile2.txt", "r+");
        srand(time(0));
        int randomNumbers[10];
        printf("\nChild: Now we chose 10 random numbers from 1 to 100... \n");
        for(int i = 0; i < 10; i++)
        {
            randomNumbers[i] = rand()%100 + 1;
            printf("%d ",randomNumbers[i]);
        }printf("\n");

        for(int i = 0 ; i < 10 ; i++)
        {
            int change = randomNumbers[i];
            fseek(chfile, (change-1)*4, SEEK_SET);
            putw(0, chfile);
        }

        displayfile(chfile);

        fclose(chfile);
        return 0;
    }