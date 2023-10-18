#include "shell.h"

/**
 * Input_buf - Buffers chained commands for processing.
 * info: The parameter struct.
 * buf: The address of the buffer to store the chained commands.
 * len: The address of the length variable to track the buffer's length.
 *
 * Returns: The number of bytes read and stored in the buffer.
 */
ssize_t Input_buf(info_t *info, char **buf, size_t *len)
{
	ssize_t r = 0;
	size_t len_p = 0;

	if (!*len) /* if nothing left in the buffer, fill it */
	{
		/*be_free((void **)info->cmnd_buf);*/
		free(*buf);
		*buf = NULL;
		signal(SIGINT, sigint_Handler);
#if USE_GETLINE
		r = gettingline(buf, &len_p, stdin);
#else
		r = _gettingline(info, buf, &len_p);
#endif
		if (r > 0)
		{
			if ((*buf)[r - 1] == '\n')
			{
				(*buf)[r - 1] = '\0'; /* remove trailing newline */
				r--;
			}
			info->linecount_flag = 1;
			removing_Comments(*buf);
			build_History_list(info, *buf, info->histcount++);
			/* if (_strchar(*buf, ';')) is this a command chain? */
			{
				*len = r;
				info->cmnd_buf = buf;
			}
		}
	}
	return (r);
}

/**
 * getting_Input - is to get a line exempting the newline
 * info: parameter struct
 *
 * Return: is for bytes read
 */
ssize_t getting_Input(info_t *info)
{
	static char *buf; /* the ';' command chain buffer */
	static size_t i, j, len;
	ssize_t r = 0;
	char **buf_p = &(info->arg), *p;

	_putchar(BUF_FLUSH);
	r = Input_buf(info, &buf, &len);
	if (r == -1) /* EOF */
		return (-1);
	if (len) /* we have commands left in the chain buffer */
	{
		j = i; /* init new iterator to current buf position */
		p = buf + i; /* get pointer for return */

		checkmyChain(info, buf, &j, i, len);
		while (j < len) /* iterate to semicolon or end */
		{
			if (ismyChain(info, buf, &j))
				break;
			j++;
		}

		i = j + 1; /* increment past nulled ';'' */
		if (i >= len) /* reached end of buffer? */
		{
			i = len = 0; /* reset position and length */
			info->cmnd_buf_type = CMD_NORM;
		}

		*buf_p = p; /* pass back pointer to current command position */
		return (_strlen(p)); /* return length of current command */
	}

	*buf_p = buf; /* else not a chain, pass back buffer from _gettingline() */
	return (r); /* return length of buffer from _gettingline() */
}

/**
 * Read_buf - is meant to read a buffer
 * info: information on parameter struct
 * buf: is the buffer
 * i: size
 *
 * Return: r
 */
ssize_t Read_buf(info_t *info, char *buf, size_t *i)
{
	ssize_t r = 0;

	if (*i)
		return (0);
	r = read(info->readfd, buf, READ_BUF_SIZE);
	if (r >= 0)
		*i = r;
	return (r);
}

/**
 * _gettingline - receives next line of input from standard input
 * info: info of parameter struct
 * ptr: address of pointer to buffer, preallocated or NULL
 * length: size of preallocated ptr buffer if not NULL
 *
 * Return: s
 */
int _gettingline(info_t *info, char **ptr, size_t *length)
{
	static char buf[READ_BUF_SIZE];
	static size_t i, len;
	size_t k;
	ssize_t r = 0, s = 0;
	char *p = NULL, *new_p = NULL, *c;

	p = *ptr;
	if (p && length)
		s = *length;
	if (i == len)
		i = len = 0;

	r = Read_buf(info, buf, &len);
	if (r == -1 || (r == 0 && len == 0))
		return (-1);

	c = _strchar(buf + i, '\n');
	k = c ? 1 + (unsigned int)(c - buf) : len;
	new_p = _reallocate(p, s, s ? s + k : k + 1);
	if (!new_p) /* MALLOC FAILURE! */
		return (p ? free(p), -1 : -1);

	if (s)
		_strncat(new_p, buf + i, k - i);
	else
		_strncopy(new_p, buf + i, k - i + 1);

	s += k - i;
	i = k;
	p = new_p;

	if (length)
		*length = s;
	*ptr = p;
	return (s);
}

/**
 * sigint_Handler - doesnt allow ctrl-C execution
 * sig_num: for signal number
 *
 * Return: void
 */
void sigint_Handler(__attribute__((unused))int sig_num)
{
	_puts("\n");
	_puts("$ ");
	_putchar(BUF_FLUSH);
}
