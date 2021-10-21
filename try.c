#include <string.h>
#include <stdlib.h>
#include <stdio.h>
int main()
{
	int r;
	r = system("ls -a");
	char line[50];
	char command[50];
	FILE* file = fopen("commands.txt","r");
	for(int i = 0 ; i < 5 ; i++)
	{
		fgets(line, 100, file);
		puts(line);
		system(line);
	}
	char str[10];
	fgets(str, 10, stdin);
	puts(str);
	char cmd[20];
	char ch;
	int i=0;;
	while(ch!=' '){
		ch = str[i];
		cmd[i] = ch;
		i+=1;
	}cmd[i] = '\0';
	char og[] = "touch";
if(strcmp(og,cmd)){puts(cmd);}
	fclose(file);
	system("history");
	return r;
}
