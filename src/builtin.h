/**
 * @file:		src/builtin.h
 * @author:		Jozef Nagy <schkwve@gmail.com>
 * @copyright:	MIT (See LICENSE.md)
 * @brief:		This file contains the routines for
 * 				built-in commands.
 */

#ifndef __BUILTIN_H_
#define __BUILTIN_H_

typedef int (*builtin_func)(const char **);

/**
 * @brief	This routine creates and fills a hashtable
 * 			with pointers to built-in command functions.
 */
void builtin_init();

/**
 * @brief	This routine prints every argument back to stdio.
 */
int echo(const char **argv);

#endif // __BUILTIN_H_
