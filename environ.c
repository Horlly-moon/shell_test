#include "shell.h"

/**
 * _myownEnv - prints the current environment
 * @info: A struct for potential arguments.
 * Return: Always 0
 */
int _myownEnv(info_t *info)
{
	Print_list_str(info->env);
	return (0);
}

/**
 * _getmyenv - retrieves the value of an environment variable.
 * @info: A structure for potential arguments.
 * @name: The name of the environment variable.
 *
 * Return: the value
 */
char *_getmyenv(info_t *info, const char *name)
{
	list_t *node = info->env;
	char *p;

	while (node)
	{
		p = Starts_with(node->str, name);
		if (p && *p)
			return (p);
		node = node->next;
	}
	return (NULL);
}

/**
 * _myownSetenv - Initialize a new environment variable or modify an existing one.
 * @info: A structure for potential arguments.
 *  Return: Always 0
 */
int _myownSetenv(info_t *info)
{
	if (info->argc != 3)
	{
		_errputs("Incorrect number of arguements\n");
		return (1);
	}
	if (my_setenv(info, info->argv[1], info->argv[2]))
		return (0);
	return (1);
}

/**
 * _myownUnsetenv - Remove an environment variable.
 * @info: A structure for potential arguments.
 * Return: Always 0
 */
int _myownUnsetenv(info_t *info)
{
	int i;

	if (info->argc == 1)
	{
		_errputs("Too few arguements.\n");
		return (1);
	}
	for (i = 1; i <= info->argc; i++)
		my_unsetenv(info, info->argv[i]);

	return (0);
}

/**
 * Populatemy_env_list - populates an environment linked list.
 * @info: A structure for potential arguments.
 * Return: Always 0
 */
int Populatemy_env_list(info_t *info)
{
	list_t *node = NULL;
	size_t i;

	for (i = 0; environ[i]; i++)
		Adding_node_end(&node, environ[i], 0);
	info->env = node;
	return (0);
}
