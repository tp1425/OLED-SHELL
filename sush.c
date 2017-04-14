#define _XOPEN_SOURCE 

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <errno.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include "list.h"
#include "tokenizer.h"

#define BUFFER_SIZE 1024
#define SUSHRC ".sushrc"
#define READ_MODE "r"
#define READ_END 0
#define WRITE_END 1

void read_sushrc_file(FILE *fp, char *buffer, struct token_list *token_list);
void flush_buffer(char *buffer);
void execute_command(struct token_list *token_list, int *exit_pipe);
void ctrl_c_handler(int signal);

int main(int argc, const char *argv[])
{
	pid_t pid;

	// create a file pointer that points the sushrc file
	FILE *fp;

	// change to the user's home directory
	chdir(getenv("HOME"));
	
	if(getenv("PS1") == NULL)
	{
		putenv("PS1=;) ");
	}

	// create a linked list to use to store the tokens of each command executed
	struct token_list *token_list = create_list();

	// create a buffer to temporarily store user commands and exit with an error if it cannot 		be created

	char *buffer = calloc(sizeof(char),1024);
	if(buffer == NULL)
	{
		fprintf(stderr,"Error: Buffer couldn't be created\n");
		exit(EXIT_FAILURE);
	}

	// open the .sushrc file for reading and return with an exit failure if it cannot be opened
	fp = fopen(SUSHRC,READ_MODE);
	if(fp != NULL)
	{
		read_sushrc_file(fp, buffer, token_list);
	}

	// create a buffer to save the current working directory
	char *working_dir = calloc(sizeof(char),1024);
	if(working_dir == NULL)
	{
		fprintf(stderr,"Error: Buffer couldn't be created\n");
		exit(EXIT_FAILURE);
	}

	do
	{
		int exit_pipe[2];
		if(pipe(exit_pipe) == -1)
		{
			fprintf(stderr,"pipe failed\n");
			exit(-1);	
		}

		pid = fork();	
		
		if(pid < 0)
		{
			fprintf(stderr, "process fork failed");

		}
	
		else if(pid == 0)	//execute command line operations with child process 
		{
			while(true)
			{
				signal(SIGINT, ctrl_c_handler);
				
				printf("%s PID: %d\n","CHILD",getpid());

				printf("%s",getenv("PS1"));

				char *buffer = calloc(sizeof(char),1024);
				if(buffer == NULL)
				{
					fprintf(stderr,"Error: Buffer couldn't be created\n");
					exit(EXIT_FAILURE);
				}

				fgets(buffer, BUFFER_SIZE - 1, stdin);

				tokenize(buffer,token_list);

				execute_command(token_list, exit_pipe);

				struct node *node_ptr = token_list->head;

				int i = 0;
				while(node_ptr != NULL)
				{
				printf("token %d: %s\t\t\tsize: %d\tis_special: %d   	\n",i,node_ptr->token,node_ptr->token_size,node_ptr->is_special);

					node_ptr = node_ptr->next;
					i++;
				}

				empty_list(token_list);
				printf("%s\n",buffer);
			}
		}
		else			//if the child is terminated, return to fork new process
		{
			wait(&pid);
			printf("%s PID: %d\n","PARENT",getpid());
			
			int message;
			close(exit_pipe[WRITE_END]);
			read(exit_pipe[READ_END], &message, sizeof(message));

			if(message == -1)		//if the message is -1, exit the shell completely
			{	
				exit(0);
			}

		}

	} while(true);

return 0;
}


void read_sushrc_file(FILE *fp, char *buffer, struct token_list *token_list)
{
	while(fgets(buffer, BUFFER_SIZE - 1, fp) != NULL)
	{
		tokenize(buffer,token_list);

		if(strcmp(token_list->head->token,"exit") == 0)
		{

			exit(EXIT_SUCCESS);
		}

		empty_list(token_list);
		printf("%s\n",buffer);
		flush_buffer(buffer);
	}
}

void flush_buffer(char *buffer)
{
	for(int i = 0; i < BUFFER_SIZE; i++)
	{
		buffer[i] = 0;
	}
}

void execute_command(struct token_list *token_list, int *exit_pipe)
{
	pid_t exec_pid;		//create a process type for exection

	if(strcmp(token_list->head->token,"exit") == 0)
	{
		int exit_val = -1;
		close(exit_pipe[READ_END]);
		write(exit_pipe[WRITE_END], &exit_val, sizeof(exit_val));	//if exit is entered, send a message to the parent 
		close(exit_pipe[WRITE_END]);
		exit(0);
		
	}

	if(strcmp(token_list->head->token,"setenv") == 0)			//format for handling the internal commands
	{
		destroy_list(token_list);
		exit(0);
		
	}

	exec_pid = fork();							//if external command, create a child process to execute
	
	if(exec_pid < 0)
	{
		fprintf(stderr, "execution process fork failed");		//print error 

	}
	else if(exec_pid == 0)							//let child execute commands
	{
		printf("%s EXECUTION PID: %d\n","CHILD",getpid());
		//execlp("/bin/ls", "ls", NULL);
	}
	else									//return to the parent when finished and return to primary process
	{
		wait(&exec_pid);
		printf("%s EXECUTION PID: %d\n","PARENT",getpid());

		exit(0);
	}

}

void ctrl_c_handler(int signal)							//detect Ctrl-C mid process and exit
{
	printf("\nThe Crtl-C was caught by the handler\n");
	exit(0);
}



























