#include "shell.h"

/**
 * hshell – The primary shell execution loop
 * @info: the parameter & return info struct
 * @arv: the argument vector from main()
 *
 * Return: 0 on success, 1 on error, or error code
 */
int hshell(info_t *info, char **arv)
{
	ssize_t r = 0;
	int builtin_ret = 0;

	while (r != -1 && builtin_ret != -2)
	{
		Clearmy_info(info);
		if (Interactive_Mode(info))
			_puts("$ ");
		_errputchar(BUF_FLUSH);
		r = getting_Input(info);
		if (r != -1)
		{
			Setmy_info(info, arv);
			builtin_ret = finding_builtin(info);
			if (builtin_ret == -1)
				finding_cmd(info);
		}
		else if (Interactive_Mode(info))
			_putchar('\n');
		Freemy_info(info, 0);
	}
	write_History(info);
	Freemy_info(info, 1);
	if (!Interactive_Mode(info) && info->status)
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
 * finding_builtin - finds and execute a built-in command within a shell
 * @info: the parameter & return info struct
 *
 * Return: -1 if built-in command is not found,
 * 0 if the built-in command is found and executed successfully,
 * 1 if the built-in is found but not executed successfully,
 * 2 if the built-in command signals an exit()
 */
int finding_builtin(info_t *info)
{
	int i, built_in_ret = -1;
	builtin_table builtintbl[] = {
		{"exit", _myownExit},
		{"env", _myownEnv},
		{"help", _myownHelp},
		{"history", _myownHistory},
		{"setenv", _myownSetenv},
		{"unsetenv", _myownUnsetenv},
		{"cd", _myownCd},
		{"alias", _myownAlias},
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
 * finding_cmd - it locates a command      within the PATH environment.
 * @info: the parameter and return info struct
 * This function searches for the specified command within the directories listed
 * in the PATH environment variable.
 *
 * Return: void
 */
void finding_cmd(info_t *info)
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
		if (!is_myDelim(info->arg[i], " \t\n"))
			k++;
	if (!k)
		return;

	path = finding_path(info, _getmyenv(info, "PATH="), info->argv[0]);
	if (path)
	{
		info->path = path;
		forking_cmd(info);
	}
	else
	{
		if ((Interactive_Mode(info) || _getmyenv(info, "PATH=")
					|| info->argv[0][0] == '/') && is_cmnd(info, info->argv[0]))
			forking_cmd(info);
		else if (*(info->arg) != '\n')
		{
			info->status = 127;
			Print_error(info, "not found\n");
		}
	}
}

/**
 * forking_cmd - Forks a new execution thread to run a command.
 * @info: the parameter and return info struct
 * This function creates a new execution thread to run the specified command.
 *
 * Return: void
 */
void forking_cmd(info_t *info)
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
		if (execve(info->path, info->argv, getting_environment(info)) == -1)
		{
			Freemy_info(info, 1);
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
