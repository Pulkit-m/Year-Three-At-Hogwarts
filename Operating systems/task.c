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

void  parse(char *line, char **argv)
{
     while (*line != '\0') {       /* if not the end of line ....... */ 
          while (*line == ' ' || *line == '\t' || *line == '\n')
               *line++ = '\0';     /* replace white spaces with 0    */
          *argv++ = line;          /* save the argument position     */
          while (*line != '\0' && *line != ' ' && 
                 *line != '\t' && *line != '\n') 
               line++;             /* skip the argument until ...    */
     }
     *argv = '\0';                 /* mark the end of argument list  */
}

void execute_exec(char** argv)
{
    int status;
    pid_t pid = fork();
    if(pid <0)
    {
        printf("***Error: forking child process failed\n");
        exit(1);
    }
    else if(pid==0)
    {
        if(execvp(*argv, argv)<0)
        {
            printf("***Error: Exec failed\n");
            exit(1);
        }
    }
    else{
        while(wait(&status)!=pid)
            {;}
    }
}

void systemcall(char cmnd[])
{
    int i = 0, count = 1;
    while(cmnd[i]!='\0')
    {
        if(cmnd[i]==' ')
        {
            count++;
        }
        i++;
    }

    char* token = strtok(cmnd, " ");
    char* args[count+1];
    int p = 0;
    while(token!=NULL){
        args[p] = token;
        p++;
        token = strtok(NULL, " ");
    }
    args[count] = NULL;
    int status;
    pid_t pid = fork();
    if(pid == 0){
        execvp(args[0], args);
        perror("***Error: exec not completed");
        exit(1);
    }
    else{wait(NULL);}
   
}


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
    // int r = system(cmnd);
    systemcall(cmnd);
    

    // char* argv[255];
    // parse(cmnd, argv);
    // execute_exec(argv);

    // pid_t pid = ford();
    // if(pid == 0)
    // {
        
    // }
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
                printf("sochte hain abhi\n"); 
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
                printf("%s \n",substr);
                execute(substr, HISTORY);
            }
        }
        else{
            printf("command not found.\n");
        }
    }
    return 0;
}