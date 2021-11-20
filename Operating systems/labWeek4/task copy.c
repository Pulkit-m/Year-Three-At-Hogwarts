/*
using system() command
authon: Pulkit Mahajan
2019ucs0073
*/

#include <stdlib.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <string.h>
#include <unistd.h>
#define SIZE 255
#define WSIZE 32
#define PATH_LENGTH 4096

struct node{
    // char command[50];
    char** command;
    pid_t node_pid ;
    struct node *next;
};

struct list{
    struct node* HEAD;
};

int count_words(char *str)
{
    // printf("_____________________________");
    int word_count = 1;
    for (int i = 0; (str[i] != '\0' && str[i] != '\n'); i++)
    {
        if (
                (str[i] == ' ' && (str[i + 1] != ' ' && str[i + 1] != '\t' && str[i+1]!='\n'))
                 || 
                (str[i] == '\t' && (str[i + 1] != ' ' && str[i + 1] != '\t' && str[i+1]!='\n'))
            )
        {
            word_count += 1;
        }
    }
    // printf("%d\n", word_count);
    return word_count;
}

// void getwords(char *str, char* TOKENS[],int word_count)
void getwords(char *str, char** TOKENS, int word_count)
// char** getwords(char* str)
{
    // printf("_____________________________");
    //needs to return an array of pointers to characters.
    // int word_count = count_words(str);
    // char TOKENS[word_count][32];  //TOKENS is a 2D array of tokens
    // char TOKENS[][32] = malloc(word_count*32*sizeof(char));
    int token_index = 0;
    int i = 0;
    while(str[i]!='\0' && str[i]!='\n')
    {
        if(str[i]!=' ' && str[i]!='\t')
        {            
            int j = i;
            TOKENS[token_index] = malloc(sizeof(char)*WSIZE);
            while(str[j]!=' ' && str[j]!='\t' && str[j]!='\n')
            {
                TOKENS[token_index][j-i] = str[j];
                j+=1;
            }
            TOKENS[token_index][j-i] = '\0';
            token_index+=1;
            i=j;
        }
        else{
            i+=1;
        }
    }
    // TOKENS[word_count] = malloc(sizeof(char)*2);
    // TOKENS[word_count][0] = '\0';
    TOKENS[word_count] = NULL;
}

pid_t system_call(char** argv)
{
    // printf("_____________________________");
    pid_t pid;
    int status;
    int fd[2];
    int p = pipe(fd);
    if((pid = fork())<0)
    {     /* fork a child process           */
        printf("*** ERROR: forking child process failed\n");
        exit(1);
    }
    else if (pid == 0) {          /* for the child process:         */
        printf("child process: %d\n",getpid());
        int child_process_id = getpid();
        write(fd[1], &child_process_id, sizeof(int));
        close(fd[1]);
        if (execvp(*argv, argv) < 0) {     /* execute the command  */
               printf("*** ERROR: exec failed\n");
               exit(1);
        }
    }
    else {                                  /* for the parent:      */
          while (wait(&status) != pid);       /* wait for completion  */
    }
    int child_process_id;
    read(fd[0],&child_process_id,sizeof(int));
    close(fd[0]);
    printf("child process again: %d\n",child_process_id);
    // returns process id of the child process.
    return child_process_id;
}


//executes a command and saves it to history
void execute(char cmnd[], struct list HISTORY)
{
    // printf("_____________________________");
    
    int word_count = count_words(cmnd);
    // char** TOKENS;
    // TOKENS = (char**)malloc(sizeof(char*)*word_count);
    char* TOKENS[word_count+1];
    getwords(cmnd, TOKENS, word_count);

    // strcpy(newnode->command, cmnd);
    struct node* newnode = malloc(sizeof(struct node));
    struct node* ptr = HISTORY.HEAD;
    while(ptr->next!=NULL)
    {
        ptr = ptr->next;
        printf("%d ",ptr->node_pid);
        // puts(ptr->command);
    }printf("\n");
    ptr->next = newnode;
    newnode->next = NULL;
    newnode->command = TOKENS;
    printf("Command in execution: %s \n", cmnd);
    // int r = system(cmnd); 
    pid_t process_invoked_id = system_call(TOKENS);
    newnode->node_pid = process_invoked_id;
}

//this is nothing but the implementation of HISTORY FULL command
// void printHistory(struct list HISTORY){
//     struct node* ptr = HISTORY.HEAD;
//     while(ptr!=NULL)
//     {
//         puts(ptr->command);
//         ptr = ptr->next;
//     }
// }

//this is nothing but the implementation of HISTORY Brief command
// void historyBrief(struct list HISTORY){
//     struct node* ptr = HISTORY.HEAD;
//     while(ptr!=NULL)
//     {
//         char ch;
//         ch = ptr->command[0];
//         int i = 0;
//         while((ch!=' ')&& (ch!= '\0'))
//         {
//             ch = ptr->command[i];
//             printf("%c",ch);
//             i+=1;
//         }printf("\n");
//         ptr = ptr->next;
//     }
// }



int main(int argc, char* argv[])
{
    struct list HISTORY;
    HISTORY.HEAD = malloc(sizeof(struct node));
    // strcpy(HISTORY.HEAD->command, argv[0]);
    HISTORY.HEAD->next = NULL;
    // *(HISTORY.HEAD->command) = argv[0];

    printf("Shell launched successfully \n");
    char *sys_name = (char*)malloc(32*sizeof(char));
    // sys_name = getlogin();
    cuserid(sys_name);
    char home[PATH_LENGTH];
    getcwd(home, sizeof(home));
    int n = 10;
    while(n--)
    {
        char pwd[PATH_LENGTH];
        getcwd(pwd, sizeof(pwd));
        // char* pwd = "/mnt/d/acer/Documents/Courses/Year3 At Hogwarts/Operating systems/labWeek4/folder";
        char* rel_path= (char*)malloc(sizeof(char)*65);
        if(strcmp(pwd, home)==0)
        {
            rel_path[0] = '\0';
        }
        else if(strlen(pwd)>strlen(home)){
            int temp = strlen(home);
            int i;
            for(i = temp; pwd[i]!='\0'; i++)
            {
                rel_path[i-temp] = pwd[i];
            }
            rel_path[i-temp] = '\0';
        }
        printf("<2019ucs0073@%s:~%s>", sys_name, rel_path );

        char command[SIZE];
        fgets(command, SIZE, stdin);
        // int word_count = count_words(command);
        // char* TOKENS[word_count+1];
        // getwords(command, TOKENS, word_count);
        execute(command, HISTORY);

    }

    // while(1){
    //     printf("please enter your command: ");
    //     char command[50];
    //     fgets(command, sizeof(command), stdin);

    //     char token[50];
    //     int i;
    //     for(i = 0 ; command[i]!=' ' && command[i]!='\0'; i++)
    //     {
    //         token[i] = command[i];
    //     }token[i]='\0';

    //     // puts(command);


    //     char HISTORY_BRIEF[] = "HISTORY BRIEF";
    //     char HISTORY_FULL[] = "HISTORY FULL";
    //     char STOP[] = "STOP";
    //     char EXEC[] = "EXEC ";

    //     char* isHISTORY_BRIEF = strstr(command, HISTORY_BRIEF);
    //     char* isHISTORY_FULL = strstr(command, HISTORY_FULL);
    //     char* isSTOP = strstr(command, STOP);
    //     char* isEXEC = strstr(command, EXEC);
    //     char* isCD = strstr(command, "cd ");

    //     if(isSTOP){
    //         printf("Exiting Normally, Bye \n<2019ucs0073@iitjammu.ac.in>\n");
    //         break;
    //     }
    //     else if(isHISTORY_BRIEF)
    //     {
    //         historyBrief(HISTORY);
    //     }
    //     else if(isHISTORY_FULL)
    //     {
    //         printHistory(HISTORY);
    //     }
    //     else if(isEXEC)
    //     {
    //         // EXEC ls -la
    //         // 012345
    //         // EXEC 2

    //         if(command[5] >= '0' && command[5] <= '9')
    //         {
    //             char substr[10];
    //             for(i = 5; command[i]!='\0' ; i++)
    //             {
    //                 substr[i-5] = command[i];
    //             }substr[i-5] = '\0';
    //             int k = atoi(substr);
    //             struct node* ptr = HISTORY.HEAD;
    //             int flag1 = 0;
    //             for(int i = 0 ; i < k; i++)
    //             {
    //                 if(ptr==NULL){
    //                     printf("***ERROR: Segmentation Fault(Printed by me)\n");
    //                     flag1 = 1;
    //                     break;
    //                 }
    //                 ptr = ptr->next;
    //             }     
    //             if(flag1==0)
    //             {
    //                 execute(ptr->command, HISTORY);
    //             }
    //         }
    //         else{
    //             char substr[50];
    //             for(i = 5; command[i]!='\0' ; i++)
    //             {
    //                 substr[i-5] = command[i];
    //             }substr[i-5] = '\0';
    //             // printf("%s \n",substr);
    //             char word1[50];
    //             int j;
    //             for(j = 0 ; substr[j]!=' ' && substr[j]!='\n' && substr[j]!='\0'; j++)
    //             {
    //                 word1[j] = substr[j];
    //             }word1[j] = '\0';
    //             // printf("first word: %s \n", word1);
    //             // struct node* ptr2 = HISTORY.HEAD;
    //             // int flag = 0;
    //             // while(ptr2!=NULL)
    //             // {
    //             //     if(strstr(ptr2->command, word1))
    //             //     {
    //             //         execute(substr, HISTORY);
    //             //         flag = 1;
    //             //         break;
    //             //     }
    //             //     ptr2 = ptr2->next;
    //             // }
    //             // if(flag == 0){
    //             //     printf("***ERROR: command not found \n");
    //             // }
    //             execute(substr, HISTORY);
    //         }
    //     }
    //     else{
    //         printf("command not found.\n");
    //     }
    // }
    return 0;
}