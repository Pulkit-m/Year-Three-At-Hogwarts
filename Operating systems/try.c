#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>

void  parse(char *line, char **args)
{
     while (*line != '\0') {       /* if not the end of line ....... */ 
          while (*line == ' ' || *line == '\t' || *line == '\n')
               *line++ = '\0';     /* replace white spaces with 0    */
          *args++ = line;          /* save the argument position     */
          while (*line != '\0' && *line != ' ' && 
                 *line != '\t' && *line != '\n') 
               line++;             /* skip the argument until ...    */
     }
     *args = '\0';  
	                /* mark the end of argument list  */
}

void  execute(char **argv)
{
     pid_t  pid;
     int    status;

     if ((pid = fork()) < 0) {     /* fork a child process           */
          printf("*** ERROR: forking child process failed\n");
          exit(1);
     }
     else if (pid == 0) {          /* for the child process:         */
          if (execvp(*argv, argv) < 0) {     /* execute the command  */
               printf("*** ERROR: exec failed\n");
               exit(1);
          }
     }
     else {                                  /* for the parent:      */
    //       while (wait(&status) != pid)       /* wait for completion  */
    //            ;
    wait(NULL);
	 }
}

int main()
{
	
	FILE *file = fopen("commands.txt", "r");
	char line[255];
	while (fgets(line, sizeof(line), file))
	{
		puts(line);
		// const char delim[] = " ";
		// char *token = strtok(line, delim);
		char* args[255];
		parse(line, args);
		// system(line);
		// *args[n] = NULL;
		execute(args);

		// int a = 0;
		// while (token != NULL)
		// {
		// 	// char word[20];
		// 	char *ptr = token;
		// 	int i = 0;
		// 	while (*ptr != '\0')
		// 	{
		// 		args[a][i] = *ptr;
		// 		ptr++;
		// 		i++;
		// 	}
		// 	args[a][i] = '\0';
		// 	puts(args[a]);
		// 	token = strtok(NULL, delim);
		// 	// args[a] = word;
		// 	a++;
		// }
		// printf("%d\n", a);
	}

	return 0;

	// 	r = system("ls -a");
	// 	char line[50];
	// 	char command[50];
	// 	FILE* file = fopen("commands.txt","r");
	// 	for(int i = 0 ; i < 5 ; i++)
	// 	{
	// 		fgets(line, 100, file);
	// 		puts(line);
	// 		system(line);
	// 	}
	// 	char str[10];
	// 	fgets(str, 10, stdin);
	// 	puts(str);
	// 	char cmd[20];
	// 	char ch;
	// 	int i=0;;
	// 	while(ch!=' '){
	// 		ch = str[i];
	// 		cmd[i] = ch;
	// 		i+=1;
	// 	}cmd[i] = '\0';
	// 	char og[] = "touch";
	// if(strcmp(og,cmd)){puts(cmd);}
	// 	fclose(file);
	// 	system("history");
	// 	return r;
}
