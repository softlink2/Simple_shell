#include "main.h"

/**
 * exit_cmd - exits the program with a termination status.
 */
int exit_cmd(char **cmd)
{
	char error[1024] = "./hsh: 1: exit: Illegal number: ";
	int i, status = 0;

	if (cmd[1] == NULL)
	{
		free_grid(cmd);
		exit(EXIT_SUCCESS);
	}

	for (i = 0; cmd[1][i]; i++)
		if (!_isdigit(cmd[1][i]))
		{
			_strcat(error, cmd[1]);
			write(STDERR_FILENO, error, _strlen(error));
			write(STDERR_FILENO, "\n", 1);
			exit(2);
		}

	status = _atoi(cmd[1]);
	free_grid(cmd);
	exit(status);
}

int env_cmd(__attribute__((unused)) char **cmd)
{
	envlist_t *head;

	head = build_env();
	print_env_list(head);
	free_env_list(head);

	return (TRUE);
}
