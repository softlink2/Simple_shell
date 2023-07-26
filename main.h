#ifndef MAIN_H
#define MAIN_H

#include <unistd.h>
#include <sys/stat.h>
#include <stdio.h>
#include <fcntl.h>
#include <dirent.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>

#define BUFFSIZE 1024
/* User-defined boolean values */
typedef enum boolean {
	FALSE,
	TRUE
} bool_t;

/* command status */

typedef enum cmd
{
	NOT_FOUND,
	FOUND
} cmd_t;
ssize_t _getline(char **line, size_t *size, int fd);

int _isdigit(char c);
int _atoi(char *s);
/* =====Strings manipulation functions===== */

size_t _strspn(const char *s, char *accept);
size_t strpbrk_no(const char *s, char *accept);
char *_strncpy(char *dest, char *src, int n);
size_t _strlen(const char *s);
char **strtow(char *s);
char *_strdup(const char *str);
char *string_nconcat(char *s1, char *s2, unsigned int n);
int _strncmp(const char *s1, const char *s2, unsigned int n);
const char *_strchrnul(const char *s, int c);
size_t words_count(const char *s1, const char *s2);
char **tokenize_str(char *str, char *delim);
char *_strcat(char *dest, const char *src);

/* ========== Build-up functions ========== */
int get_cmd(int interactive_mode);
void execute_cmd(char **args, cmd_t cmd);
char *path_handler(char *cmd);
/* =====Memory Management===== */
void free_grid(char **grid);


extern char **environ;
/* =========== Environment list =========== */

/**
 * struct envlist - a list of environment variables
 * @env_var: a pointer to a variable's name.
 * @value: a pointer to a variable's value.
 * @next_env_var: a pointer to the next variable in the list.
 */
typedef struct envlist
{
	char *env_var;
	char *value;
	struct envlist *next_env_var;
} envlist_t;

envlist_t *build_env(void);
envlist_t *add_env_var(envlist_t **head, const char *var, const char *value);
void print_env_list(const envlist_t *h);
void free_env_list(envlist_t *h);
int _setenv(const char *name, const char *value, int overwrite);
int _unsetenv(const char *name);
char *_getenv(const char *name);

int overwrite_env_var(envlist_t *head, const char *name, const char *value);
int delete_env_var(envlist_t **head, const char *name);
int search_env_var(const envlist_t *head, const char *name);

/* ===== PATH Directories ===== */

/**
 * struct path - a list of directories in the PATH
 * @dir: a pointer to a pathname.
 * @next_dir: a pointer to the next pathname in the list.
 */
typedef struct path
{
	char *dir;
	struct path *next_dir;
} path_t;

path_t *build_path(void);
path_t *add_path_dir(path_t **head, const char *s);
void print_path_list(const path_t *h);
void free_path_list(path_t *h);


/**
 * struct builtin - a structure of built-in commands.
 * @f: a pointer to functions handling built-in commands.
 * @cmd: a pointer to a string / command.
 */
struct builtin
{
	char *cmd;
	int (*f)(char **);
};
int exit_cmd(char **);
int builtin_handler(char **);
int env_cmd(char **);
#endif		/* "main.h" included */
