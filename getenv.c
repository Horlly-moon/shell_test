#include "shell.h"

/**
 * getting_environment - returns a copy of the string array from the environ.
 * @info: A structure for potential arguments.
 *
 * Returns: Always 0.
 */
char **getting_environment(info_t *info)
{
	if (!info->environ || info->env_change)
	{
		info->environ = List_to_strings(info->env);
		info->env_change = 0;
	}

	return (info->environ);
}

/**
 * my_unsetenv - Removes an environment variable.
 * info: A structure for potential arguments.
 * var: The string representing the environment variable.
 *
 * Returns: 1 on successful removal, 0 otherwise.
 */
int my_unsetenv(info_t *info, char *var)
{
	list_t *node = info->env;
	size_t i = 0;
	char *p;

	if (!node || !var)
		return (0);

	while (node)
	{
		p = Starts_with(node->str, var);
		if (p && *p == '=')
		{
			info->env_change = Deleting_node_index(&(info->env), i);
			i = 0;
			node = info->env;
			continue;
		}
		node = node->next;
		i++;
	}
	return (info->env_change);
}

/**
 * my_setenv - Initializes a new environment variable or modifies an existing one.
 * info: A structure for potential arguments.
 * var: The string representing the environment variable property.
 * value: The string representing the environment variable value.
 *
 * Returns: Always 0.
 */
int my_setenv(info_t *info, char *var, char *value)
{
	char *buf = NULL;
	list_t *node;
	char *p;

	if (!var || !value)
		return (0);

	buf = malloc(_strlen(var) + _strlen(value) + 2);
	if (!buf)
		return (1);
	_strcopy(buf, var);
	_strcat(buf, "=");
	_strcat(buf, value);
	node = info->env;
	while (node)
	{
		p = Starts_with(node->str, var);
		if (p && *p == '=')
		{
			free(node->str);
			node->str = buf;
			info->env_change = 1;
			return (0);
		}
		node = node->next;
	}
	Adding_node_end(&(info->env), buf, 0);
	free(buf);
	info->env_change = 1;
	return (0);
}
