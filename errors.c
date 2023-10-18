#include "shell.h"

/**
 * _errputs - prints the specified string
 * @str: the string to be printed
 *
 * Return: Nothing
 */
void _errputs(char *str)
{
	int i = 0;

	if (!str)
		return;
	while (str[i] != '\0')
	{
		_errputchar(str[i]);
		i++;
	}
}

/**
 * _errputchar - writes the character 'c' to the standard error (stderr).
 * @c: The character to be printed.
 *
 * Return: On success 1.
 * On error, -1 is returned, and errno is set appropriately.
 */
int _errputchar(char c)
{
	static int i;
	static char buf[WRITE_BUF_SIZE];

	if (c == BUF_FLUSH || i >= WRITE_BUF_SIZE)
	{
		write(2, buf, i);
		i = 0;
	}
	if (c != BUF_FLUSH)
		buf[i++] = c;
	return (1);
}

/**
 * _putfd - writes the character 'c' to the specified file descriptor (fd).
 * @c: The character to be written.
 * @fd: The file descriptor to write to.
 *
 * Return: On success 1.
 * On error, -1 is returned, and errno is set appropriately.
 */
int _putfd(char c, int fd)
{
	static int i;
	static char buf[WRITE_BUF_SIZE];

	if (c == BUF_FLUSH || i >= WRITE_BUF_SIZE)
	{
		write(fd, buf, i);
		i = 0;
	}
	if (c != BUF_FLUSH)
		buf[i++] = c;
	return (1);
}

/**
 * _putsfd - writes the specified string to the specified file descriptor (fd).
 * @str: the string to be printed
 * @fd: The file descriptor to write to.
 *
 * Return: the number of chars put
 */
int _putsfd(char *str, int fd)
{
	int i = 0;

	if (!str)
		return (0);
	while (*str)
	{
		i += _putfd(*str++, fd);
	}
	return (i);
}

