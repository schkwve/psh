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
void command_parse(char *buffer);

#endif // __COMMAND_H_
