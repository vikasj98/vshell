#include "vshell.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>

char *built_in[] = {"cd", "exit", "help"};
void (*built_in_function[])(char **) = {vsh_cd, vsh_exit, vsh_help};
int num_builtin = sizeof(built_in)/sizeof(char*);

//Just print a string and exit
void vsh_print_welcome_message(){
	printf("A simple shell\n");
}

void vsh_execute(){
	char *line;	//Will hold the line read from the terminal
	char **words;	//Will hold the words ie. program and its arguments
	
	printf(">");
	line = vsh_read_line();	//Get a line from the terminal
	if(NULL == line)
	{
		fprintf(stderr, "Unable to read a line from the terminal\n");
		return;
	}
	words = vsh_extract_words(line);
	if(NULL == words)
	{
		free(line);
		fprintf(stderr, "Unable to split the line into words\n");
		return;
	}
	int i = 0;
	if(strcmp(words[0], "sudo") == 0)
	{
		printf("Hey, did you just order me to ");
		i = 1;
		do
		{
			printf("%s ",words[i++]);
		}while(words[i]!=NULL);
		printf("\nTry requesting me and I might do it\n");
		return;
	}
	if(strcmp(words[0], "please") == 0)
	{
		words[0] = "sudo";
	}
	for(i=0; i<num_builtin; i++)
	{
		if(strcmp(words[0], built_in[i]) == 0)
		{
			built_in_function[i](words);
			break;
		}	
	}
	if(i>=num_builtin)
	{
		vsh_execute_command(words);
	}	
	free(line);
	free(words);
}

void vsh_exit(char **words){
	printf("vshell is exiting\n");
	exit(0);
}

char* vsh_read_line(){
	int c;
	char *buffer = malloc(sizeof(char) * BUFFER_SIZE);
	if( NULL == buffer)
	{
		fprintf(stderr, "Unable to allocate buffer\n");
		return NULL;
	}
	int current_buffer_size = BUFFER_SIZE;
	int i = 0;	//Counter inside the loop
	do
	{
		c = getchar();
		buffer[i++] = c;
		if(i >= current_buffer_size)	//Current buffer is full and we need a larger buffer
		{
			buffer = realloc(buffer, sizeof(char) * (current_buffer_size + BUFFER_SIZE));
			if( NULL == buffer )
			{
				fprintf(stderr, "Unable to allocte buffer\n");
				return NULL;
			}
			current_buffer_size = current_buffer_size + BUFFER_SIZE;
		}
	}while( (c != EOF) && (c != '\n'));
	buffer[i-1] = '\0';
	return buffer;
}


/*
20 Aug 2019
0049
A very messy implementation of word separation. Currently deals with only single spaces and tabs between 
parameters.
Will need to refine it further
*/

char** vsh_extract_words(char* line){
	char **words;
	words = malloc(TOTAL_WORDS * sizeof(char*));
	int current_word_buffer = TOTAL_WORDS;
	int word_index = 0;
	char c;
	int current_index = 0, last_index = 0;
	do
	{
		c = line[current_index];
		if( (c==' ') || (c=='\0') || (c=='\t'))
		{
			int word_size = current_index - last_index ;
			words[word_index] = malloc((word_size + 1) * sizeof(char));
			if(NULL == words[word_index])
			{
				fprintf(stderr, "Error allocating buffer for a word\n");
				return NULL;
			}
			memcpy(words[word_index], line + last_index, word_size);
			words[word_index][word_size] = '\0';
			word_index++;
			if(word_index >= current_word_buffer)
			{
				words = realloc(words, sizeof(char*) * (current_word_buffer + TOTAL_WORDS));
				if(NULL == words)
				{
					fprintf(stderr, "Error allocationg buffer for words\n");
					return NULL;
				}
			}
			last_index = ++current_index;
			//Allocate memory to hold the word
			//Parse till non blank character
		}
		else
		{
			current_index++;
		}	
	}while(c != '\0');
	words[word_index] = NULL;
	
	return words;
}


void vsh_execute_command(char **words)
{
	pid_t pid, wpid;
	pid = fork();
	int status;
	if(pid == 0)	//Child process
	{
		if(execvp(words[0], words) == -1)
		{
			fprintf(stderr, "Error creating new process\n");
			return;
		}
	}
	else if(pid < 0)	//Not forked
	{
		fprintf(stderr, "Error forking a new process\n");
		return;
	}
	else
	{
		//In parent
		do
		{
			wpid = waitpid(pid, &status, WUNTRACED); 
		}while(!WIFEXITED(status) && !WIFSIGNALED(status));
	}
}

void vsh_cd(char **words)
{
	if(NULL == words[1])
	{
		fprintf(stderr, "You didn't pass an argument to cd\n");
		return;
	}
	if(0 != chdir(words[1]))
	{
		fprintf(stderr, "Error changing directory\n");
		return;
	}
}

void vsh_help(char **words)
{
	printf("vsh, a simple toy shell. Enter a command and its arguments separated by single spaces.\n");
	printf("The shell builtins are help, cd and exit.\n");
	printf("To find help regarding other commands try man command\n");
	printf("This shell is ridiculously simple and there's nothing fancy here.\n");
	printf("You can help expand this tou by suggesting or adding new functionalities\n");
}



