/**
 * @file:		src/helper.c
 * @author:		Jozef Nagy <schkwve@gmail.com>
 * @copyright:	MIT (See LICENSE.md)
 * @brief:		This file contains helper routines
 * 				to be used by psh.
 */

#include <string.h>

#include "helper.h"

/**
 * @brief	This routine trims excessive whitespace
 * 			"  Hello   World" -> "Hello World"
 */
char *strtrim(char *str)
{
	char *head = str;
	char *tail = str + strlen(str);

	while (*head == ' ') {
		head++;
	}
	while (*tail == ' ') {
		tail--;
	}
	*(tail - 1) = '\0';

	return head;
}
