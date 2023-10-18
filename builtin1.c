#include "shell.h"

/**
 * _myownHistory - displays the history list, showing each command
 * with line numbers starting at 0.
 * 
 * @info: A structure that may contain relevant arguments. is used to maintain
 * a consistent function prototype.
 * 
 *  Return: Always 0
 */
int _myownHistory(info_t *info)
{
	Print_list(info->history);
	return (0);
}

/**
 * Unset_alias - sets alias to string
 * @info: parameter struct
 * @str: the string alias
 *
 * Return: Always 0 on success, 1 on error
 */
int Unset_alias(info_t *info, char *str)
{
	char *p, c;
	int ret;

	p = _strchar(str, '=');
	if (!p)
		return (1);
	c = *p;
	*p = 0;
	ret = Deleting_node_index(&(info->alias),
		get_Node_index(info->alias, node_Starts_with(info->alias, str, -1)));
	*p = c;
	return (ret);
}

/**
 * Set_alias - sets an alias to a specified string.
 * @info: A parameter struct
 * @str: The string to be used as an alias.
 *
 * Return: Always 0 on success, 1 on error
 */
int Set_alias(info_t *info, char *str)
{
	char *p;

	p = _strchar(str, '=');
	if (!p)
		return (1);
	if (!*++p)
		return (Unset_alias(info, str));

	Unset_alias(info, str);
	return (Adding_node_end(&(info->alias), str, 0) == NULL);
}

/**
 * Print_alias - prints an alias string
 * @node: The alias node to be printed.
 *
 * Return: Always 0 on success, 1 on error
 */
int Print_alias(list_t *node)
{
	char *p = NULL, *a = NULL;

	if (node)
	{
		p = _strchar(node->str, '=');
		for (a = node->str; a <= p; a++)
		_putchar(*a);
		_putchar('\'');
		_puts(p + 1);
		_puts("'\n");
		return (0);
	}
	return (1);
}

/**
 * _myownAlias - emulates the behavior of the alias command (man alias).
 * @info: A structure that may contain relevant arguments.  is used to maintain
 *        a consistent function prototype.
 *  Return: Always 0
 */
int _myownAlias(info_t *info)
{
	int i = 0;
	char *p = NULL;
	list_t *node = NULL;

	if (info->argc == 1)
	{
		node = info->alias;
		while (node)
		{
			Print_alias(node);
			node = node->next;
		}
		return (0);
	}
	for (i = 1; info->argv[i]; i++)
	{
		p = _strchar(info->argv[i], '=');
		if (p)
			Set_alias(info, info->argv[i]);
		else
			Print_alias(node_Starts_with(info->alias, info->argv[i], '='));
	}

	return (0);
}
