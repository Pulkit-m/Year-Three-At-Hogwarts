#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <time.h>

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
    FILE* fp = fopen("newfile.txt", "w");
    int array[100];
    for(int i = 1 ; i < 101; i++)
    {
        putw(i,fp);
    }
    fclose(fp);

    FILE* temp = fopen("newfile.txt", "r");
    displayfile(temp);
    fclose(temp);

    printf("Parent: Initiating fork now... \n");

    pid_t pid = fork();
    if(pid == 0){
        //child process
        FILE* chfile = fopen("newfile.txt", "r+");
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
        exit(0);
    }
    else{
        wait(NULL);
        FILE* checking = fopen("newfile.txt", "r");
        printf("\nParent: My child changed numbers at the following indexes...\n");
        for(int i = 0 ; i < 100 ; i++)
        {
            if(getw(checking) == 0){
                printf("%d ", i+1);
            }
        }printf("\n");

        fclose(checking);
        exit(0);
    }

    return 0;
}

