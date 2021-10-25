/*
using system() command
authon: Pulkit Mahajan
2019ucs0073
*/

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

struct node{
    char command[50];
    struct node *next;
};

struct list{
    struct node* HEAD;
};

//executes a command and saves it to history
void execute(char cmnd[], struct list HISTORY)
{
    struct node* newnode = malloc(sizeof(struct node));
    strcpy(newnode->command, cmnd);
    struct node* ptr = HISTORY.HEAD;
    while(ptr->next!=NULL)
    {
        ptr = ptr->next;
        // puts(ptr->command);
    }
    ptr->next = newnode;
    newnode->next = NULL;
    printf("Command in execution: %s \n", cmnd);
    int r = system(cmnd);
}

//this is nothing but the implementation of HISTORY FULL command
void printHistory(struct list HISTORY){
    struct node* ptr = HISTORY.HEAD;
    while(ptr!=NULL)
    {
        puts(ptr->command);
        ptr = ptr->next;
    }
}

//this is nothing but the implementation of HISTORY Brief command
void historyBrief(struct list HISTORY){
    struct node* ptr = HISTORY.HEAD;
    while(ptr!=NULL)
    {
        char ch;
        ch = ptr->command[0];
        int i = 0;
        while((ch!=' ')&& (ch!= '\0'))
        {
            ch = ptr->command[i];
            printf("%c",ch);
            i+=1;
        }printf("\n");
        ptr = ptr->next;
    }
}



int main(int argc, char* argv[])
{
    struct list HISTORY;
    HISTORY.HEAD = malloc(sizeof(struct node));
    strcpy(HISTORY.HEAD->command, argv[0]);
    HISTORY.HEAD->next = NULL;

    for(int i = 1 ; i < argc ; i++)
    {
        FILE* file = fopen(argv[i], "r");
        char line[50];
        while(fgets(line, sizeof(line), file))
        {
            execute(line, HISTORY);
        }
        fclose(file);
    }

    printf("==============================================\n\nFinished executing batch files\n");
    printf("SWITCHING TO INTERACTIVE MODE\n\n");

    while(1){
        printf("please enter your command: ");
        char command[50];
        fgets(command, sizeof(command), stdin);

        char token[50];
        int i;
        for(i = 0 ; command[i]!=' ' && command[i]!='\0'; i++)
        {
            token[i] = command[i];
        }token[i]='\0';

        // puts(command);


        char HISTORY_BRIEF[] = "HISTORY BRIEF";
        char HISTORY_FULL[] = "HISTORY FULL";
        char STOP[] = "STOP";
        char EXEC[] = "EXEC";

        char* isHISTORY_BRIEF = strstr(command, HISTORY_BRIEF);
        char* isHISTORY_FULL = strstr(command, HISTORY_FULL);
        char* isSTOP = strstr(command, STOP);
        char* isEXEC = strstr(command, EXEC);

        if(isSTOP){
            printf("Exiting Normally, Bye \n<2019ucs0073@iitjammu.ac.in>\n");
            break;
        }
        else if(isHISTORY_BRIEF)
        {
            historyBrief(HISTORY);
        }
        else if(isHISTORY_FULL)
        {
            printHistory(HISTORY);
        }
        else if(isEXEC)
        {
            if(command[5] >= '0' && command[5] <= '9')
            {
                char substr[10];
                for(i = 5; command[i]!='\0' ; i++)
                {
                    substr[i-5] = command[i];
                }substr[i-5] = '\0';
                int k = atoi(substr);
                struct node* ptr = HISTORY.HEAD;
                for(int i = 0 ; i < k; i++)
                {
                    ptr = ptr->next;
                }     
                execute(ptr->command, HISTORY);
            }
            else{
                char substr[50];
                for(i = 5; command[i]!='\0' ; i++)
                {
                    substr[i-5] = command[i];
                }substr[i-5] = '\0';
                // printf("%s \n",substr);
                char word1[50];
                int j;
                for(j = 0 ; substr[j]!=' ' && substr[j]!='\n' && substr[j]!='\0'; j++)
                {
                    word1[j] = substr[j];
                }word1[j] = '\0';
                printf("first word: %s \n", word1);
                struct node* ptr2 = HISTORY.HEAD;
                int flag = 0;
                while(ptr2!=NULL)
                {
                    if(strstr(ptr2->command, word1))
                    {
                        execute(substr, HISTORY);
                        flag = 1;
                        break;
                    }
                    ptr2 = ptr2->next;
                }
                if(flag == 0){
                    printf("***ERROR: command not found \n");
                }
                // execute(substr, HISTORY);
            }
        }
        else{
            printf("command not found.\n");
        }
    }
    return 0;
}