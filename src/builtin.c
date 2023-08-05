/**
 * @file:		src/builtin.c
 * @author:		Jozef Nagy <schkwve@gmail.com>
 * @copyright:	MIT (See LICENSE.md)
 * @brief:		This file contains the routines for
 * 				built-in commands.
 */

#include <stddef.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#include "builtin.h"
#include "hashtable.h"

hashtable_t *g_builtin_hashtable;

/**
 * @brief	This routine creates and fills a hashtable
 * 			with pointers to built-in command functions.
 */
void builtin_init()
{
	g_builtin_hashtable = hashtable_create();

	hashtable_insert(g_builtin_hashtable, "echo", echo);
}

/**
 * @brief	This routine prints every argument back to stdio.
 */
int echo(const char **argv)
{
	printf("called %s\n", argv[0]);
	return 0;
}
