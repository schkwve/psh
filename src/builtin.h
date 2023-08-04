/**
 * @file:		src/builtin.h
 * @author:		Jozef Nagy <schkwve@gmail.com>
 * @copyright:	MIT (See LICENSE.md)
 * @brief:		This file contains the routines for
 * 				built-in commands.
 */

#ifndef __BUILTIN_H_
#define __BUILTIN_H_

#define BUILTIN_HASHTABLE_SIZE 1

typedef struct builtin_cmd {
	char *cmd;
	void *func;
	struct builtin_cmd *next;
} builtin_cmd_t;

extern builtin_cmd_t *builtin_hashtable[BUILTIN_HASHTABLE_SIZE];

#endif // __BUILTIN_H_
