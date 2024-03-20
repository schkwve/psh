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

void builtin_init(void);

int psh_true(process_t *proc);
int psh_false(process_t *proc);
int psh_cat(process_t *proc);
int psh_echo(process_t *proc);
int psh_chdir(process_t *proc);
int psh_fg(process_t *proc);
int psh_export(process_t *proc);
int psh_unset(process_t *proc);
int psh_exit(process_t *proc);

#endif // __BUILTIN_H_
