#include <stdio.h>
#include <stdlib.h>
#include <spawn.h>
#include <sys/wait.h>
#include <unistd.h>
#include <string.h>

#define N 12

extern char **environ;

char *allowed[N] = {"cp", "touch", "mkdir", "ls", "pwd", "cat", "grep", "chmod", "diff", "cd", "exit", "help"};

int isAllowed(const char *cmd)
{
	int length = sizeof(allowed) / sizeof(allowed[0]);

	// compare allowed commands to given command
	for (int i = 0; i < length; i++)
	{
		// return 1 if cmd is one of the allowed commands
		if (strcmp(allowed[i], cmd) == 0)
		{
			return 1;
		}
	}

	// return 0 otherwise
	return 0;
}

int main()
{

	// TODO
	// Add variables as needed

	char line[256];

	while (1)
	{

		fprintf(stderr, "rsh>");

		if (fgets(line, 256, stdin) == NULL)
			continue;

		if (strcmp(line, "\n") == 0)
			continue;

		line[strlen(line) - 1] = '\0';

		// TODO
		// Add code to spawn processes for the first 9 commands
		// And add code to execute cd, exit, help commands
		// Use the example provided in myspawn.c
	}
	return 0;
}
