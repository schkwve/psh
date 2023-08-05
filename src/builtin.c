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

	hashtable_insert(g_builtin_hashtable, "true", psh_true);
	hashtable_insert(g_builtin_hashtable, "false", psh_false);
	hashtable_insert(g_builtin_hashtable, "echo", psh_echo);
}

/**
 * @brief	This routine returns 1.
 */
int psh_true(const char **argv)
{
	(void)argv;
	return 1;
}

/**
 * @brief	This routine returns 0.
 */
int psh_false(const char **argv)
{
	(void)argv;
	return 0;
}

/**
 * @brief	This routine prints every argument back to stdio.
 */
int psh_echo(const char **argv)
{
	NOT_IMPLEMENTED();
	(void)argv;
	return 0;
}
