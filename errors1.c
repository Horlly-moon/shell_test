#include "shell.h"

/**
 * _err_atoi - converts a string to an integer
 * @s: the string to be converted
 * Return: The converted number if successful, 0 if no numbers are found,
 *          or -1 on error.
 */
int _err_atoi(char *s)
{
	int i = 0;
	unsigned long int result = 0;

	if (*s == '+')
		s++;  /* TODO: why does this make main return 255? */
	for (i = 0;  s[i] != '\0'; i++)
	{
		if (s[i] >= '0' && s[i] <= '9')
		{
			result *= 10;
			result += (s[i] - '0');
			if (result > INT_MAX)
				return (-1);
		}
		else
			return (-1);
	}
	return (result);
}

/**
 * Print_error - prints an error message
 * @info: the parameter and return info struct
 * @estr: The string containing the specified error type.
 * Return: 0 if no numbers in string, converted number otherwise
 * -1 on error
 */
void Print_error(info_t *info, char *estr)
{
	_errputs(info->fname);
	_errputs(": ");
	Print_dec(info->line_count, STDERR_FILENO);
	_errputs(": ");
	_errputs(info->argv[0]);
	_errputs(": ");
	_errputs(estr);
}

/**
 * Print_dec - prints a decimal (base 10) number.
 * @input: The number to be printed.
 * @fd: the file descriptor to write to
 *
 * Return: The number of characters printed
 */
int Print_dec(int input, int fd)
{
	int (*__putchar)(char) = _putchar;
	int i, count = 0;
	unsigned int _abs_, current;

	if (fd == STDERR_FILENO)
		__putchar = _errputchar;
	if (input < 0)
	{
		_abs_ = -input;
		__putchar('-');
		count++;
	}
	else
		_abs_ = input;
	current = _abs_;
	for (i = 1000000000; i > 1; i /= 10)
	{
		if (_abs_ / i)
		{
			__putchar('0' + current / i);
			count++;
		}
		current %= i;
	}
	__putchar('0' + current);
	count++;

	return (count);
}

/**
 * Converting_number - conversion function, similar to itoa.
 * @num: The number to be converted.
 * @base: The base for conversion.
 * @flags: Flags for arguments.
 *
 * Return: string
 */
char *Converting_number(long int num, int base, int flags)
{
	static char *array;
	static char buffer[50];
	char sign = 0;
	char *ptr;
	unsigned long n = num;

	if (!(flags & CONVERT_UNSIGNED) && num < 0)
	{
		n = -num;
		sign = '-';

	}
	array = flags & CONVERT_LOWERCASE ? "0123456789abcdef" : "0123456789ABCDEF";
	ptr = &buffer[49];
	*ptr = '\0';

	do	{
		*--ptr = array[n % base];
		n /= base;
	} while (n != 0);

	if (sign)
		*--ptr = sign;
	return (ptr);
}

/**
 * removing_Comments - replaces the first instance of '#' with '\0' in the string.
 * @buf: The address of the string to modify.
 *
 * Return: Always 0;
 */
void removing_Comments(char *buf)
{
	int i;

	for (i = 0; buf[i] != '\0'; i++)
		if (buf[i] == '#' && (!i || buf[i - 1] == ' '))
		{
			buf[i] = '\0';
			break;
		}
}
