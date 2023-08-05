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

typedef int (*builtin_func)(int argc, const char **);

/**
 * @brief	This routine creates and fills a hashtable
 * 			with pointers to built-in command functions.
 */
void builtin_init();

/**
 * @brief	This routine returns 1.
 */
int psh_true(int argc, const char **argv);

/**
 * @brief	This routine returns 0.
 */
int psh_false(int argc, const char **argv);

/**
 * @brief	This routines opens and prints the contents of a file from argv[1].
 */
int psh_cat(int argc, const char **argv);

/**
 * @brief	This routine prints every argument back to stdio.
 */
int psh_echo(int argc, const char **argv);

/**
 * @brief	This routine changes the current working directory to argv[1].
 * 			If no arguments are present, change to $HOME.
 */
int psh_chdir(int argc, const char **argv);

/**
 * @brief	This routine exits with an exit code.
 * 			If exit code is not set, return 0.
 */
int psh_exit(int argc, const char **argv);

#endif // __BUILTIN_H_
