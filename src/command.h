/**
 * @file:		src/command.c
 * @author:		Jozef Nagy <schkwve@gmail.com>
 * @copyright:	MIT (See LICENSE.md)
 * @brief:		This file contains the routines for
 * 				parsing user input, calling built-in commands
 * 				as well as external binaries.
 */

#ifndef __COMMAND_H_
#define __COMMAND_H_

#include <stddef.h>

/**
 * @brief	Buffer size for user input tokenization
 */
#define PSH_COMMAND_BUFSIZE 8

/**
 * @brief	This routine parses user input and handles it.
 */
int command_parse(char *buffer);

/**
 * @brief	This routine checks if a command is built-in.
 * 
 * @return	1 if the command is built-in, 0 otherwise.
 */
int command_check_builtin(int argc, const char **argv);

#endif // __COMMAND_H_
