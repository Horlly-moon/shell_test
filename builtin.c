#include "shell.h"

/**
 * _myownExit - exits the shell
 * @info: A structure that may contain relevant arguments. This parameter is used to maintain
 * the consistent function prototype.
 * Return: exits with a given exit status
 * (0) if info.argv[0] != "exit"
 */
int _myownExit(info_t *info)
{
	int exitcheck;

	if (info->argv[1]) /* If there is an exit arguement */
	{
		exitcheck = _err_atoi(info->argv[1]);
		if (exitcheck == -1)
		{
			info->status = 2;
			Print_error(info, "Illegal number: ");
			_errputs(info->argv[1]);
			_errputchar('\n');
			return (1);
		}
		info->err_num = _err_atoi(info->argv[1]);
		return (-2);
	}
	info->err_num = -1;
	return (-2);
}

/**
 * _myownCd - changes the current directory of the process.
 * @info: A structure that may contain relevant arguments. This parameter is used to maintain
 * the consistent function prototype.
 * constant function prototype.
 * Return: Always 0
 */
int _myownCd(info_t *info)
{
	char *s, *dir, buffer[1024];
	int chdir_ret;

	s = getcwd(buffer, 1024);
	if (!s)
		_puts("TODO: >>getcwd failure emsg here<<\n");
	if (!info->argv[1])
	{
		dir = _getmyenv(info, "HOME=");
		if (!dir)
			chdir_ret = /* TODO: what should this be? */
				chdir((dir = _getmyenv(info, "PWD=")) ? dir : "/");
		else
			chdir_ret = chdir(dir);
	}
	else if (_strcmp(info->argv[1], "-") == 0)
	{
		if (!_getmyenv(info, "OLDPWD="))
		{
			_puts(s);
			_putchar('\n');
			return (1);
		}
		_puts(_getmyenv(info, "OLDPWD=")), _putchar('\n');
		chdir_ret = /* TODO: what should this be? */
			chdir((dir = _getmyenv(info, "OLDPWD=")) ? dir : "/");
	}
	else
		chdir_ret = chdir(info->argv[1]);
	if (chdir_ret == -1)
	{
		Print_error(info, "can't cd to ");
		_errputs(info->argv[1]), _errputchar('\n');
	}
	else
	{
		my_setenv(info, "OLDPWD", _getenv(info, "PWD="));
		my_setenv(info, "PWD", getcwd(buffer, 1024));
	}
	return (0);
}

/**
 * _myownHelp - displays help information for the process.
 * @info: A structure that may contain relevant arguments. This parameter is used to maintain
 * the consistent function prototype.
 * 
 * Return: Always 0
 */
int _myownHelp(info_t *info)
{
	char **arg_array;

	arg_array = info->argv;
	_puts("help call works. Function not yet implemented \n");
	if (0)
		_puts(*arg_array); /* temp att_unused workaround */
	return (0);
}
