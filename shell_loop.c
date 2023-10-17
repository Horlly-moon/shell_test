#include "shell.h"

/**
 * hsh – The main shell loop
 * @info: the parameter & return info struct
 * @arv: the argument vector from main()
 *
 * Return: 0 on success, 1 on error, or error code
 */
int hsh(info_t *info, char **arv)
{
	ssize_t r = 0;
	int builtin_ret = 0;

	while (r != -1 && builtin_ret != -2)
	{
		Clear_info(info);
		if (interactive_mode(info))
			_puts("$ ");
		_eputchar(BUF_FLUSH);
		r = get_Input(info);
		if (r != -1)
		{
			Set_info(info, arv);
			builtin_ret = find_a_builtin(info);
			if (builtin_ret == -1)
				find_a_cmd(info);
		}
		else if (interactive_mode(info))
			_putchar('\n');
		Free_info(info, 0);
	}
	write_History(info);
	Free_info(info, 1);
	if (!interactive_mode(info) && info->status)
		exit(info->status);
	if (builtin_ret == -2)
	{
		if (info->err_num == -1)
			exit(info->status);
		exit(info->err_num);
	}
	return (builtin_ret);
}

/**
 * find_a_builtin - it finds a builtin command
 * @info: the parameter & return info struct
 *
 * Return: -1 if builtin not found,
 * 0 if builtin executed successfully,
 * 1 if builtin found but not successful,
 * 2 if builtin signals exit()
 */
int find_a_builtin(info_t *info)
{
	int i, built_in_ret = -1;
	builtin_table builtintbl[] = {
		{"exit", _myExit},
		{"env", _myEnv},
		{"help", _myHelp},
		{"history", _myHistory},
		{"setenv", _mySetenv},
		{"unsetenv", _myUnsetenv},
		{"cd", _myCd},
		{"alias", _myAlias},
		{NULL, NULL}
	};

	for (i = 0; builtintbl[i].type; i++)
		if (_strcmp(info->argv[0], builtintbl[i].type) == 0)
		{
			info->line_count++;
			built_in_ret = builtintbl[i].func(info);
			break;
		}
	return (built_in_ret);
}

/**
 * find_a_cmd - it finds a command in PATH
 * @info: the parameter & return info struct
 *
 * Return: void
 */
void find_a_cmd(info_t *info)
{
	char *path = NULL;
	int i, k;

	info->path = info->argv[0];
	if (info->linecount_flag == 1)
	{
		info->line_count++;
		info->linecount_flag = 0;
	}
	for (i = 0, k = 0; info->arg[i]; i++)
		if (!is_Delim(info->arg[i], " \t\n"))
			k++;
	if (!k)
		return;

	path = find_a_path(info, _getenv(info, "PATH="), info->argv[0]);
	if (path)
	{
		info->path = path;
		fork_a_cmd(info);
	}
	else
	{
		if ((interactive_mode(info) || _getenv(info, "PATH=")
					|| info->argv[0][0] == '/') && is_cmnd(info, info->argv[0]))
			fork_a_cmd(info);
		else if (*(info->arg) != '\n')
		{
			info->status = 127;
			Print_error(info, "not found\n");
		}
	}
}

/**
 * fork_a_cmd - it forks a an exec thread to run cmd
 * @info: the parameter & return info struct
 *
 * Return: void
 */
void fork_a_cmd(info_t *info)
{
	pid_t child_pid;

	child_pid = fork();
	if (child_pid == -1)
	{
		/* TODO: PUT ERROR FUNCTION */
		perror("Error:");
		return;
	}
	if (child_pid == 0)
	{
		if (execve(info->path, info->argv, get_environment(info)) == -1)
		{
			Free_info(info, 1);
			if (errno == EACCES)
				exit(126);
			exit(1);
		}
		/* TODO: PUT ERROR FUNCTION */
	}
	else
	{
		wait(&(info->status));
		if (WIFEXITED(info->status))
		{
			info->status = WEXITSTATUS(info->status);
			if (info->status == 126)
				Print_error(info, "Permission denied\n");
		}
	}
}
