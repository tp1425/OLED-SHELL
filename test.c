#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#define HOME "HOME"

int main(int argc, char **argv, char** envp)
{
	char *home = getenv(HOME);
	chdir(home);

	FILE *fp = fopen(".sushrc","r");
	if(fp == NULL)
	{
		fprintf(stderr,"Could not open .sushrc\n");
		exit(EXIT_FAILURE);
	}

  	return(0);
}