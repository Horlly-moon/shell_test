#include "shell.h"

/**
 * Clearmy_info - Initializes the info_t structure.
 * @info: The address of the info_t struct to be initialized.
 *
 * This function sets initial values and allocates memory as needed for the
 * info_t structure, preparing it for use.
 */
void Clearmy_info(info_t *info)
{
	info->arg = NULL;
	info->argv = NULL;
	info->path = NULL;
	info->argc = 0;
}

/**
 * Setmy_info - Initializes the info_t structure with an argument vector.
 * info: The address of the info_t struct to be initialized.
 * arv: The argument vector (typically argv) to be associated with the structure.
 *
 * This function sets initial values for the info_t structure and associates it with
 * the provided argument vector, preparing it for use in shell operations.
 */
void Setmy_info(info_t *info, char **arv)
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

		replacemyAlias(info);
		replacemyVars(info);
	}
}

/**
 * Freemy_info - Frees fields within the info_t structure.
 * info: The address of the info_t struct with fields to be freed.
 * all: If true, all fields are freed; if false, specific fields are freed.
 *
 * This function deallocates memory and frees specific fields within the info_t
 * structure based on the 'all' parameter. When 'all' is true, all fields are freed.
 * When 'all' is false, specific fields are freed as needed.
 */
void Freemy_info(info_t *info, int all)
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
