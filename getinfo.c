#include "shell.h"

/**
 * Clear_info - initializes info_t struct
 * @info: struct address
 */
void Clear_info(info_t *info)
{
	info->arg = NULL;
	info->argv = NULL;
	info->path = NULL;
	info->argc = 0;
}

/**
 * Set_info - initializes info_t struct
 * @info: struct address
 * @arv: argument vector
 */
void Set_info(info_t *info, char **arv)
{
	int i = 0;

	info->fname = arv[0];
	if (info->arg)
	{
		info->argv = str_tow(info->arg, " \t");
		if (!info->argv)
		{
			info->argv = malloc(sizeof(char *) * 2);
			if (info->argv)
			{
				info->argv[0] = _strdup(info->arg);
				info->argv[1] = NULL;
			}
		}
		for (i = 0; info->argv && info->argv[i]; i++)
			;
		info->argc = i;

		replaceAlias(info);
		replaceVars(info);
	}
}

/**
 * Free_info - frees info_t struct fields
 * @info: struct address
 * @all: true if freeing all fields
 */
void Free_info(info_t *info, int all)
{
	f_free(info->argv);
	info->argv = NULL;
	info->path = NULL;
	if (all)
	{
		if (!info->cmnd_buf)
			free(info->arg);
		if (info->env)
			Free_list(&(info->env));
		if (info->history)
			Free_list(&(info->history));
		if (info->alias)
			Free_list(&(info->alias));
		f_free(info->environ);
			info->environ = NULL;
		b_free((void **)info->cmnd_buf);
		if (info->readfd > 2)
			close(info->readfd);
		_putchar(BUF_FLUSH);
	}
}
