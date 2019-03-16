#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#define MAX_READ 1024

char* removeWord(char *line, char *word); //removes any word like PIPE, BG
int ouSystem(char *command); //executes the command 
char** parseString(char* str); //parses line of strings and returns array of words to execute execv system call
int pipeCounter(char* str); // counts the number of pipes in file

struct commands //to allocate n arrays of pipe
{
	const char **cmd;
};


int main (int argc, char **argv)
{
	FILE *stream;
	char *line = NULL;
	char *tempLine = NULL;
	size_t num = 0;
	ssize_t read;
	int flag = 0;
	int counter  = 0;
	int pipefd[2];
	int pid;
	char** cmd [pipeCounter(argv[1])]; //alocated this array to execuae n pipes

	stream = fopen("batch.oush", "r");
	if (stream == NULL)
	{
		fprintf(stderr,"error read only\n");
		exit(EXIT_FAILURE);
	}

	while ((read = getline(&line, &num, stream)) != -1)
	{
		//I tried  to execuae n pipes in this section
		/*  if(strstr(line, "PIPE") != NULL)
		    {
		    cmd[counter] = parseString(line);
		    flag = 1;
		    counter++;

		    while(flag =! 0)
		    {	
		    getline(&line, &num, stream);

		    if(strstr(line, "PIPE") != NULL)
		    {
		    flag = 1;
		    cmd[counter] = parseString(removeWord(line,  "PIPE"));

		    counter++;
		    }
		    else 
		    {
		    cmd[counter] = parseString(line);
		    flag = 0;
		    break;
		    }

		//allocate pipes



		}
		}*/
		//else if (strstr(line, "BG") != NULL)
		// {
		//do same as pipe
		// }
		// else
		// {
		//       ouSystem(line);
		// }


		if(strstr(line, "PIPE") != NULL) //checks if there is a pipe
		{

			int status;
			pid_t childPid;
			char* line1 = line;
			if(pipe(pipefd) == -1)
			{
				fprintf(stderr,"error on pipe\n");
				return -1;
			}


			switch (childPid = fork()) {
				case -1: // Error
					{
						fprintf(stderr,"error read fork\n");	
						return -1;
					}

				case 0: // Child 
					dup2(pipefd[0], 0);

					// close unused side of pipe

					close(pipefd[1]);

					getline(&line, &num, stream);
					//execvp(parseString(line)[0], parseString(line));
					ouSystem(line);
					break;
					_exit(127);                     // Failed exec 

				default: // Parent 


					dup2(pipefd[1], 1);
					// execvp(parseString(removeWord(line1, "PIPE"))[0], parseString(removeWord(line1, "PIPE")));

					// close unused side of pipe

					close(pipefd[1]);

					ouSystem(removeWord(line1, "PIPE"));
					close(childPid);
					// execvp(parseString(removeWord(line1, "PIPE"))[0], parseString(removeWord(line1, "PIPE")));
					if (waitpid(childPid, &status, 0) == -1)
					{

						return -1;

					}

					else
						return status;
					//break;
			}

		}

		else if (strstr(line, "BG") != NULL)
		{
			int status;
			pid_t childPid;
			char* line1 = line;
			if(pipe(pipefd) == -1)
			{
				fprintf(stderr,"error on pipe\n");
				return -1;
			}


			switch (childPid = fork()) {
				case -1: // Error
					{
						fprintf(stderr,"error read fork\n");
						return -1;
					}

				case 0: // Child 
					getline(&line, &num, stream);
					// execvp(parseString(line)[0], parseString(line));
					close(pipefd[0]);
					ouSystem(line);
					break;

					_exit(127);                     // Failed exec 

				default: // Parent
					close(pipefd[0]);

					execvp(parseString(removeWord(line1, "BG"))[0], parseString(removeWord(line1, "BG")));
					break;
			}
		}
		else
		{
			ouSystem(line);
		}

	}

	fclose(stream);
	//execvp(cmd[0][0], cmd[0]);
}

char* removeWord(char *line, char *word) ////removes any word like PIPE, BG
{
	char *buff;
	char *newLine;
	newLine = calloc(strlen(line)+1, sizeof(char));
	buff = strtok(line, " \t\n");
	while(buff)
	{
		if(strcmp(buff, word) != 0)
		{
			strcat(newLine, buff);
			strcat(newLine, " ");
		}

		buff = strtok(NULL, " \t\n");
	}
	return newLine;
}

int ouSystem(char *command) //excutes any command
{
	int status;
	pid_t childPid;

	switch (childPid = fork()) {
		case -1: /* Error */
			return -1;

		case 0: /* Child */
			execl("/bin/sh", "sh", "-c", command, (char *) NULL);
			_exit(127);                     /* Failed exec */

		default: /* Parent */
			if (waitpid(childPid, &status, 0) == -1)
				return -1;
			else
				return status;
	}
}

char** parseString(char* str) //parses line of strings and returns array of words to execute execv system call
{
	char* ptr = strtok(str, " ");
	char* stri[4];
	int j = 0;
	while(ptr != NULL)
	{
		stri[j] = ptr;
		ptr = strtok(NULL, " ");
		j++;
	}

	stri[j+1] = NULL;
	for (int i = 0; i<j; i++)
	{
		printf("%s\n", stri[i]);
	}

	char** strings = stri;

	return strings;

}

int pipeCounter(char* str) //counts number of pipes
{
	FILE *stream;
	char *line = NULL;
	size_t num = 0;
	ssize_t read;
	int flag = 0;
	int counter = 0;

	stream = fopen(str, "r");
	if (stream == NULL)
		exit(EXIT_FAILURE);

	while ((read = getline(&line, &num, stream)) != -1)
	{
		if(strstr(line, "PIPE") != NULL)
		{

			flag = 1;
			counter++;

			while(flag =! 0)
			{
				getline(&line, &num, stream);
				//push back to array

				if(strstr(line, "PIPE") != NULL)
				{
					flag = 1;
					counter++;
				}
				else
				{
					flag = 0;
					break;
				}
			}
		}

	}
	fclose(stream);
	return counter;
}
