#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <time.h>

void displayfile(FILE* fptr)
{
    fseek(fptr, 0, SEEK_SET);
    printf("The contents of newfile3.txt are: \n");
    for(int i = 0 ; i < 100 ; i++)
    {
        int j = getw(fptr);
        if((i)%10==0)printf("\n");
        printf("%d ",j);

    }printf("\n");

}

int main(int argc, char* argv[])
{
    int fd[2];
    //fd[0]: read
    //fd[1]: write
    int p =  pipe(fd);
    if(p== -1){
        printf("an error occured while opening pipe\n");
        exit(0);
    }
    else{
        printf("Inter process communication channel established:\n");
    }

    FILE* fp = fopen("newfile3.txt", "w");
    int array[100];
    for(int i = 1 ; i < 101; i++)
    {
        putw(i,fp);
    }
    fclose(fp);

    FILE* temp = fopen("newfile3.txt", "r");
    displayfile(temp);
    fclose(temp);

    printf("Parent: Initiating fork now... \n");

    pid_t pid = fork();
    
    if(pid == 0){
        //child process
        //close read channel because we only need write for our child process.
        close(fd[0]);
        FILE* chfile = fopen("newfile3.txt", "r+");
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

        printf("Child: I changed some numbers, Let me inform my parent about these numbers...\n");
        write(fd[1], &randomNumbers, sizeof(randomNumbers));
        close(fd[1]);//now I don't need the write channel as well.

        displayfile(chfile);

        fclose(chfile);
        exit(0);
    }
    else{
        wait(NULL);
        close(fd[1]);//we only need the read channel for parent process
        int rndnos[10];
        read(fd[0], &rndnos, sizeof(rndnos));
        printf("\n\n");
        printf("Parent: Information recieved from the child process: \n");
        for(int i =  0 ; i < 10 ; i++)
        {
            printf("%d ",rndnos[i]);
        }printf("\nNo need to read the file to look for changes\n");
        
        close(fd[0]);

        // FILE* checking = fopen("newfile.txt", "r");
        // printf("\nParent: My child changed numbers at the following indexes...\n");
        // for(int i = 0 ; i < 100 ; i++)
        // {
        //     if(getw(checking) == 0){
        //         printf("%d ", i+1);
        //     }
        // }printf("\n");

        // fclose(checking);
        exit(0);
    }

    return 0;
}