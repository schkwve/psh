/**
 * @file:		src/builtin.h
 * @author:		Jozef Nagy <schkwve@gmail.com>
 * @copyright:	MIT (See LICENSE.md)
 * @brief:		This file contains the routines for
 * 				built-in commands.
 */

#ifndef __BUILTIN_H_
#define __BUILTIN_H_

#include "psh.h"

#define NOT_IMPLEMENTED() \
	printf("%s has not been implemented yet.\n", __func__);

typedef int (*builtin_func)(process_t *);

/**
 * @brief	This routine creates and fills a hashtable
 * 			with pointers to built-in command functions.
 */
void builtin_init(void);

/**
 * @brief	This routine returns 1.
 */
int psh_true(process_t *proc);

/**
 * @brief	This routine returns 0.
 */
int psh_false(process_t *proc);

/**
 * @brief	This routines opens and prints the contents of a file from argv[1].
 */
int psh_cat(process_t *proc);

/**
 * @brief	This routine prints every argument back to stdio.
 */
int psh_echo(process_t *proc);

/**
 * @brief	This routine changes the current working directory to argv[1].
 * 			If no arguments are present, change to $HOME.
 */
int psh_chdir(process_t *proc);

/**
 * @brief	This routine brings PID to foreground
 */
int psh_fg(process_t *proc);

/**
 * @brief	This routine sets an envirnoment variable
 */
int psh_export(process_t *proc);

/**
 * @brief	This routine removes an envirnoment variable
 */
int psh_unset(process_t *proc);

/**
 * @brief	This routine exits with an exit code.
 * 			If exit code is not set, return 0.
 */
int psh_exit(process_t *proc);

#endif // __BUILTIN_H_
