#ifndef _SHELL_H_
#define _SHELL_H_

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <limits.h>
#include <fcntl.h>
#include <errno.h>

/* for read/write buffers */
#define READ_BUF_SIZE 1024
#define WRITE_BUF_SIZE 1024
#define BUF_FLUSH -1

/* for command chaining */
#define CMD_NORM	0
#define CMD_OR		1
#define CMD_AND		2
#define CMD_CHAIN	3

/* for convert_number() */
#define CONVERT_LOWERCASE	1
#define CONVERT_UNSIGNED	2

/* 1 if using system getline() */
#define USE_GETLINE 0
#define USE_STRTOK 0

#define HIST_FILE	".simple_shell_history"
#define HIST_MAX	4096

extern char **environ;

/**
 * struct liststr - singly linked list
 * @num: the number field
 * @str: a string
 * @next: points to the next node
 */
typedef struct liststr
{
	int num;
	char *str;
	struct liststr *next;
} list_t;

/**
 * struct passinfo - contains pseudo-arguements to pass into a function,
 * allowing uniform prototype for function pointer struct
 * @arg: a string generated from getline containing arguements
 * @argv:an array of strings generated from arg
 * @path: a string path for the current command
 * @argc: the argument count
 * @line_count: the error count
 * @err_num: the error code for exit()s
 * @linecount_flag: if on count this line of input
 * @fname: the program filename
 * @env: linked list local copy of environ
 * @environ: custom modified copy of environ from LL env
 * @history: the history node
 * @alias: the alias node
 * @env_change: on if environ was changed
 * @status: the return status of the last exec'd command
 * @cmnd_buf: address of pointer to cmd_buf, on if chaining
 * @cmnd_buf_type: CMD_type ||, &&, ;
 * @readfd: the fd from which to read line input
 * @histcount: the history line number count
 */
typedef struct passinfo
{
	char *arg;
	char **argv;
	char *path;
	int argc;
	unsigned int line_count;
	int err_num;
	int linecount_flag;
	char *fname;
	list_t *env;
	list_t *history;
	list_t *alias;
	char **environ;
	int env_change;
	int status;

	char **cmnd_buf; /* pointer to cmd ; chain buffer, for memory mangement */
	int cmnd_buf_type; /* CMD_type ||, &&, ; */
	int readfd;
	int histcount;
} info_t;

#define INFO_INIT \
{NULL, NULL, NULL, 0, 0, 0, 0, NULL, NULL, NULL, NULL, NULL, 0, 0, NULL, \
		0, 0, 0}

/**
 * struct builtin - contains a builtin string and related function
 * @type: the builtin command flag
 * @func: the function
 */
typedef struct builtin
{
	char *type;
	int (*func)(info_t *);
} builtin_table;

/* toem_shloop.c */
int hsh(info_t *, char **);
int find_a_builtin(info_t *);
void find_a_cmd(info_t *);
void fork_a_cmd(info_t *);

/* toem_parser.c */
int is_cmnd(info_t *, char *);
char *dup_characs(char *, int, int);
char *find_a_path(info_t *, char *, char *);

/* loophsh.c */
int loophsh(char **);

/* toem_errors.c */
void _eputs(char *);
int _eputchar(char);
int _putfd(char c, int fd);
int _putsfd(char *str, int fd);

/* toem_string.c */
int _strlen(char *);
int _strcmp(char *, char *);
char *Starts_with(const char *, const char *);
char *_strcat(char *, char *);

/* toem_string1.c */
char *_strcpy(char *, char *);
char *_strdup(const char *);
void _puts(char *);
int _putchar(char);

/* toem_exits.c */
char *_strncpy(char *, char *, int);
char *_strncat(char *, char *, int);
char *_strchr(char *, char);

/* toem_tokenizer.c */
char **str_tow(char *, char *);
char **str_tow2(char *, char);

/* toem_realloc.c */
char *_memSet(char *, char, unsigned int);
void f_free(char **);
void *_reallocate(void *, unsigned int, unsigned int);

/* toem_memory.c */
int b_free(void **);

/* toem_atoi.c */
int interactive_mode(info_t *);
int is_Delim(char, char *);
int _isAlpha(int);
int _Atoi(char *);

/* toem_errors1.c */
int _err_atoi(char *);
void Print_error(info_t *, char *);
int Print_d(int, int);
char *Convert_number(long int, int, int);
void remove_Comments(char *);

/* toem_builtin.c */
int _myExit(info_t *);
int _myCd(info_t *);
int _myHelp(info_t *);

/* toem_builtin1.c */
int _myHistory(info_t *);
int _myAlias(info_t *);

/*toem_getline.c */
ssize_t get_Input(info_t *);
int _getline(info_t *, char **, size_t *);
void sigint_Handler(int);

/* toem_getinfo.c */
void Clear_info(info_t *);
void Set_info(info_t *, char **);
void Free_info(info_t *, int);

/* toem_environ.c */
char *_getenv(info_t *, const char *);
int _myEnv(info_t *);
int _mySetenv(info_t *);
int _myUnsetenv(info_t *);
int Populate_env_list(info_t *);

/* toem_getenv.c */
char **get_environment(info_t *);
int _unsetenv(info_t *, char *);
int _setenv(info_t *, char *, char *);

/* toem_history.c */
char *get_History_file(info_t *info);
int write_History(info_t *info);
int read_History(info_t *info);
int build_History_list(info_t *info, char *buf, int linecount);
int renumber_History(info_t *info);

/* toem_lists.c */
list_t *Add_node(list_t **, const char *, int);
list_t *Add_node_end(list_t **, const char *, int);
size_t Print_list_str(const list_t *);
int Delete_node_at_index(list_t **, unsigned int);
void Free_list(list_t **);

/* toem_lists1.c */
size_t List_len(const list_t *);
char **List_to_strings(list_t *);
size_t Print_list(const list_t *);
list_t *node_Starts_with(list_t *, char *, char);
ssize_t get_Node_index(list_t *, list_t *);

/* toem_vars.c */
int isChain(info_t *, char *, size_t *);
void checkChain(info_t *, char *, size_t *, size_t, size_t);
int replaceAlias(info_t *);
int replaceVars(info_t *);
int replaceString(char **, char *);

#endif
