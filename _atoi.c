#include "shell.h"

/**
 * Interactive_Mode - it returns true if shell is interactive mode.
 * @info: struct address
 *
 * Return: 1 if interactive mode, 0 otherwise
 */
int Interactive_Mode(info_t *info)
{
	return (isatty(STDIN_FILENO) && info->readfd <= 2);
}

/**
 * is_myDelim - checks if character is a delimeter
 * @c: the character to check
 * @delim: the delimeter string
 * Return: 1 if true, 0 if false
 */
int is_myDelim(char c, char *delim)
{
	while (*delim)
		if (*delim++ == c)
			return (1);
	return (0);
}

/**
 * _ismyAlpha - checks if a character is alphabetic.
 * @c: The character to be checked 
 * Return: 1 if 'c' is an alphabetic character, 0 otherwise.
 */

int _ismyAlpha(int c)
{
	if ((c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z'))
		return (1);
	else
		return (0);
}

/**
 * _myAtoi - converts a string to an integer.
 * @s: The string to be converted
 * Return: 0 if there are no numbers in the string, or the converted number otherwise.
 */

int _Atoi(char *s)
{
	int i, sign = 1, flag = 0, output;
	unsigned int result = 0;

	for (i = 0; s[i] != '\0' && flag != 2; i++)
	{
		if (s[i] == '-')
			sign *= -1;

		if (s[i] >= '0' && s[i] <= '9')
		{
			flag = 1;
			result *= 10;
			result += (s[i] - '0');
		}
		else if (flag == 1)
			flag = 2;
	}

	if (sign == -1)
		output = -result;
	else
		output = result;

	return (output);
}
