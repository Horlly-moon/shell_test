#include "shell.h"

/**
 * get_History - receives history file
 * info: info parameter struct
 *
 * Return: allocated string containg history file
 */

char *get_History_file(info_t *info)
{
	char *buf, *dir;

	dir = _getmyenv(info, "HOME=");
	if (!dir)
		return (NULL);
	buf = malloc(sizeof(char) * (_strlength(dir) + _strlength(HIST_FILE) + 2));
	if (!buf)
		return (NULL);
	buf[0] = 0;
	_strcopy(buf, dir);
	_strcat(buf, "/");
	_strcat(buf, HIST_FILE);
	return (buf);
}

/**
 * write_History - generate a file, or adds to an existing
 * info: the parameter struct
 *
 * Return: 1 if ran successfully, else return -1
 */
int write_History(info_t *info)
{
	ssize_t fd;
	char *filename = get_History_file(info);
	list_t *node = NULL;

	if (!filename)
		return (-1);

	fd = open(filename, O_CREAT | O_TRUNC | O_RDWR, 0644);
	free(filename);
	if (fd == -1)
		return (-1);
	for (node = info->history; node; node = node->next)
	{
		_putsfd(node->str, fd);
		_putfd('\n', fd);
	}
	_putfd(BUF_FLUSH, fd);
	close(fd);
	return (1);
}

/**
 * read_History - reads history from file
 * info: the parameter struct
 *
 * Return: histcount on success, 0 otherwise
 */
int read_History(info_t *info)
{
	int i, last = 0, linecount = 0;
	ssize_t fd, rdlen, fsize = 0;
	struct stat st;
	char *buf = NULL, *filename = get_History_file(info);

	if (!filename)
		return (0);

	fd = open(filename, O_RDONLY);
	free(filename);
	if (fd == -1)
		return (0);
	if (!fstat(fd, &st))
		fsize = st.st_size;
	if (fsize < 2)
		return (0);
	buf = malloc(sizeof(char) * (fsize + 1));
	if (!buf)
		return (0);
	rdlen = read(fd, buf, fsize);
	buf[fsize] = 0;
	if (rdlen <= 0)
		return (free(buf), 0);
	close(fd);
	for (i = 0; i < fsize; i++)
		if (buf[i] == '\n')
		{
			buf[i] = 0;
			build_History_list(info, buf + last, linecount++);
			last = i + 1;
		}
	if (last != i)
		build_History_list(info, buf + last, linecount++);
	free(buf);
	info->histcount = linecount;
	while (info->histcount-- >= HIST_MAX)
		Delete_node_at_index(&(info->history), 0);
	renumber_History(info);
	return (info->histcount);
}

/**
 * build_History_list - Adds an entry to a history linked list.
 * info: A structure for potential arguments, used for maintenance.
 * buf: The buffer containing the entry to add to the history list.
 * linecount: The history line count (histcount).
 *
 * Returns: Always 0.
 */
int build_History_list(info_t *info, char *buf, int linecount)
{
	list_t *node = NULL;

	if (info->history)
		node = info->history;
	Add_node_end(&node, buf, linecount);

	if (!info->history)
		info->history = node;
	return (0);
}

/**
 * renumber_History - Renumbers the history linked list after changes.
 * info: A structure containing potential arguments, used for maintenance.
 *
 * Returns: The new histcount.
 */
int renumber_History(info_t *info)
{
	list_t *node = info->history;
	int i = 0;

	while (node)
	{
		node->num = i++;
		node = node->next;
	}
	return (info->histcount = i);
}
