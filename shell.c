#include "main.h"

void ch_handler(char *s);
void error_exit(const char *s);
/**
 * main - Entry point.
 * Return: always 0.
 */
int main(void)
{
	char *prompt = "#cisfun~$ ";
	int interactive_mode = isatty(STDIN_FILENO);

	while (1)
	{
		if (interactive_mode)
			write(1, prompt, _strlen(prompt));
		if (get_cmd(interactive_mode))
			break;
	}
	return 0;
}

int get_cmd(int interactive_mode)
{
	char *path;
	char *cmd_line = NULL;
	char **args;
	size_t size = 0;
	struct stat statbuf;
	cmd_t cmd_stat = FOUND;		/* Command status */

	/* getting command from the commandline */
	if (_getline(&cmd_line, &size, STDIN_FILENO) == -1)
	{
		if (interactive_mode)
			write(1, "\n", 1);
		free(cmd_line);
		return (1);
	}
	if (*cmd_line == '\n')
	{
		free(cmd_line);
		return (0);
	}
	ch_handler(cmd_line);

	args = tokenize_str(cmd_line, " \n");	/* tokenizing the command */
	free(cmd_line);
	if (args[0] == NULL)
		return (0);
/*	if (builtin_handler(args) == TRUE)
	{
		free_grid(args);
		return (0);
	} */
	/* Search for command in current path and in PATH */
	if (stat(args[0], &statbuf) != 0)
	{
		cmd_stat = NOT_FOUND;
		path = path_handler(args[0]);
		if (path != NULL)
		{
			free(args[0]);
			args[0] = path;
			cmd_stat = FOUND;
		}
		execute_cmd(args, cmd_stat);
	}
	else
		execute_cmd(args, cmd_stat);
	free_grid(args);

	return (0);
}

/**
 * execute_cmd - executes command by calling execve in a child process
 * @args: an array of pointers to strings / commands.
 * @cmd: command status, whether found or not.
 */
void execute_cmd(char **args, cmd_t cmd_stat)
{
	pid_t child_pid;
	int status;

	if (cmd_stat == NOT_FOUND)
	{
		execve(args[0], args, environ);
		perror(args[0]);
		return;
	}

	child_pid = fork();
	switch (child_pid)
	{
		case -1:
			error_exit("fork");
			break;
		case 0:
			execve(args[0], args, environ);
			break;
		default:
			if (wait(&status) == -1)
				error_exit("wait");
			break;
	}
}

void error_exit(const char *s)
{
	perror(s);
	_exit(EXIT_FAILURE);
}


void ch_handler(char *s)
{
	int i, j;

	for (i = 0; s[i]; i++)
		if (s[i] == '\"' && s[i - 1] != '\\')
			for (j = i; s[j]; j++)
				s[j] = s[j + 1];

	for (i = 0; s[i]; i++)
		if (s[i] == '\\')
			for (j = i; s[j]; j++)
				s[j] = s[j + 1];
}

char *path_handler(char *cmd)
{/*
    char **args, *s, *tmp;
    int i;
    struct stat status;

    args = tokenize_str(_getenv("PATH"), ":");
    for (i = 0; args[i]; i++)
    {
    tmp = string_nconcat(args[i], "/", 1);
    s = string_nconcat(tmp, cmd, _strlen(cmd));
    free(tmp);
    if (stat(s, &status) == 0)
    {
    free_grid(args);
    return (s);
    }
    free(s);
    }
    free_grid(args); */

	char *path, *s;
	path_t *head, *tmp = NULL;
	struct stat status;


	head = build_path();
	for (tmp = head; tmp; tmp = tmp->next_dir)
	{
		path = string_nconcat(tmp->dir, "/", 1);
		s = string_nconcat(path, cmd, _strlen(cmd));
		free(path);
		if (stat(s, &status) == 0)
		{
			free_path_list(head);
			return (s);
		}
		free(s);
	}
	free_path_list(head);
	return (NULL);
}
