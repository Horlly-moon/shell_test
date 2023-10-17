#include "shell.h"

/**
 * is_cmnd – it determines if a file is an executable command
 * @info: the info struct
 * @path: path to the file
 *
 * Return: 1 if true, 0 otherwise
 */
int is_cmnd(info_t *info, char *path)
{
	struct stat st;

	(void)info;
	if (!path || stat(path, &st))
		return (0);

	if (st.st_mode & S_IFREG)
	{
		return (1);
	}
	return (0);
}

/**
 * dup_characs – it duplicates characters
 * @pathstr: the PATH string
 * @start: starting index
 * @stop: stopping index
 *
 * Return: pointer to new buffer
 */
char *dup_characs(char *pathstr, int start, int stop)
{
	static char buf[1024];
	int i = 0, k = 0;

	for (k = 0, i = start; i < stop; i++)
		if (pathstr[i] != ':')
			buf[k++] = pathstr[i];
	buf[k] = 0;
	return (buf);
}

/**
 * find_a_path – it finds this cmd in the PATH string
 * @info: the info struct
 * @pathstr: the PATH string
 * @cmnd: the cmd to find
 *
 * Return: full path of cmd if found or NULL
 */
char *find_a_path(info_t *info, char *pathstr, char *cmnd)
{
	int i = 0, curr_pos = 0;
	char *path;

	if (!pathstr)
		return (NULL);
	if ((_strlen(cmnd) > 2) && Starts_with(cmnd, "./"))
	{
		if (is_cmnd(info, cmnd))
			return (cmnd);
	}
	while (1)
	{
		if (!pathstr[i] || pathstr[i] == ':')
		{
			path = dup_characs(pathstr, curr_pos, i);
			if (!*path)
				_strcat(path, cmnd);
			else
			{
				_strcat(path, "/");
				_strcat(path, cmnd);
			}
			if (is_cmnd(info, path))
				return (path);
			if (!pathstr[i])
				break;
			curr_pos = i;
		}
		i++;
	}
	return (NULL);
}
