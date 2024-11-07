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

int spawn_process(char *argv[])
{
	pid_t pid;
	int status;
	posix_spawnattr_t attr;

	// Initialize spawn attributes
	posix_spawnattr_init(&attr);

	// Set flags if needed, for example, to specify the scheduling policy
	// posix_spawnattr_setflags(&attr, POSIX_SPAWN_SETSCHEDULER);

	// Spawn a new process
	if (posix_spawnp(&pid, argv[0], NULL, &attr, argv, environ) != 0)
	{
		perror("spawn failed");
		exit(EXIT_FAILURE);
	}

	// Wait for the spawned process to terminate
	if (waitpid(pid, &status, 0) == -1)
	{
		perror("waitpid failed");
		exit(EXIT_FAILURE);
	}

	if (WIFEXITED(status))
	{
		printf("Spawned process exited with status %d\n", WEXITSTATUS(status));
	}

	// Destroy spawn attributes
	posix_spawnattr_destroy(&attr);

	return EXIT_SUCCESS;
}

void cd(const char *TARGET)
{
	// FIX: check if target dir exists?
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
	const int NUM_ARGS = 20;		 // only supposed to have 20 arguments max
	char *tokLine[NUM_ARGS + 1]; // tokenized line
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

		tokLine[tokenCount] = NULL;

		// check if cmd is valid - check first element in line arr
		if (isAllowed(tokLine[0]) == 0)
		{
			printf("NOT ALLOWED!\n");
		}

		// 10. cd:
		if (strcmp(tokLine[0], "cd") == 0)
		{
			// cd only takes one argument (got the number of arguments during tokenization)
			if (tokenCount > 2)
			{
				printf("-rsh: cd: too many arguments\n");
			}

			// FIX: statement for too few args?

			// else call cd using the single arg as the target dir
			cd(tokLine[1]);
		}
		else if (strcmp(tokLine[0], "exit") == 0) // 11. exit:
		{
			return 0;
		}
		else if (strcmp(tokLine[0], "help") == 0) // 12. help:
		{
			printf("The allowed commands are:\n1: cp\n2: touch\n3: mkdir\n4: ls\n5: pwd\n6: cat\n7: grep\n8: chmod\n9: diff\n10: cd\n11: exit\n12: help\n");
		}
		else // cmds 1-9:
		{
			if (spawn_process(tokLine) != 0)
			{
				printf("-rsh: spawn process failed\n");
			}
		}
	}
	return 0;
}
