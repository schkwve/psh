/**
 * @file:		src/builtin.h
 * @author:		Jozef Nagy <schkwve@gmail.com>
 * @copyright:	MIT (See LICENSE.md)
 * @brief:		This file contains the routines for
 * 				built-in commands.
 */

#ifndef __BUILTIN_H_
#define __BUILTIN_H_

#define NOT_IMPLEMENTED() printf("%s has not been implemented yet.\n", __func__);

typedef int (*builtin_func)(const char **);

/**
 * @brief	This routine creates and fills a hashtable
 * 			with pointers to built-in command functions.
 */
void builtin_init();

/**
 * @brief	This routine returns 1.
 */
int psh_true(const char **argv);

/**
 * @brief	This routine returns 0.
 */
int psh_false(const char **argv);

/**
 * @brief	This routine prints every argument back to stdio.
 */
int psh_echo(const char **argv);

#endif // __BUILTIN_H_
