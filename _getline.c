#include "main.h"

/**
 * _getline -
 * @line:
 * @n:
 * @fd:
 * Return:
 */
ssize_t _getline(char **cmd_line, size_t *n, int fd)
{
	char buf;
	ssize_t num_read;

	*cmd_line = calloc(sizeof(char), 10246 + 1);

	while ((num_read = read(fd, &buf, 1)) > 0 && (*n < 10246))
	{
		if (buf == '\n')
		{
			(*cmd_line)[*n] = buf;
			*n += num_read;
			break;
		}
		(*cmd_line)[*n] = buf;
		*n += num_read;
	}
	if (num_read <= 0)
		return (-1);
	(*cmd_line)[*n] = '\0';
	*cmd_line = realloc(*cmd_line, *n + 1);
	return (num_read);
}
