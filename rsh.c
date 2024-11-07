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

void cd(const char *TARGET)
{
	// FIX: check if target dir exists (not sure what dir structure looks like - can't implement this check yet)
	// FIX: do I need a statement if cd fails?
	if (chdir(TARGET) != 0)
	{
		printf("-rsh: cd: fail\n");
	}
}

int main()
{
	// TODO
	// Add variables as needed

	char line[256];
	char lineCopy[256];
	const int NUM_ARGS = 20; // only supposed to have 20 arguments max
	char *tokLine[NUM_ARGS]; // tokenized line
	int tokenCount = 0;

	while (1)
	{

		fprintf(stderr, "rsh>");

		if (fgets(line, 256, stdin) == NULL)
			continue;

		if (strcmp(line, "\n") == 0)
			continue;

		line[strlen(line) - 1] = '\0';

		// copy line before tokenizing
		strcpy(lineCopy, line);

		// tokenize line based on spaces
		char *token = strtok(lineCopy, " ");
		tokenCount = 0;

		while (token != NULL && tokenCount < NUM_ARGS)
		{
			tokLine[tokenCount++] = token;
			token = strtok(NULL, " ");
		}

		// check if cmd is valid - check first element in line arr
		if (isAllowed(tokLine[0]) == 0)
		{
			printf("NOT ALLOWED!\n");
		}

		// implement cmds 1-9:
		// create process...

		// 10. cd:
		if (strcmp(tokLine[0], "cd") == 0)
		{
			// cd only takes one argument (got the number of arguments during tokenization)
			if (tokenCount > 2)
			{
				printf("-rsh: cd: too many arguments\n");
			}
			// FIX: do I need a check for too few args?

			// else call cd using the single arg as the target dir
			cd(tokLine[1]);
		}
	}
	return 0;
}
