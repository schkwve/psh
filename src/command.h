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

#include "jobs.h"

#define COMMAND_BUILTIN 0
#define COMMAND_EXTERNAL 1

/**
 * @brief	Buffer size for user input tokenization
 */
#define PSH_COMMAND_BUFSIZE 64

/**
 * @brief	This routine parses user input.
 * 
 * @return	Job structure
 */
job_t *command_parse(char *buffer);

/**
 * @brief	This routine finds a builtin function and executes it.
 * 
 * @return	-255 if function wasn't found. Otherwise, the function's return value.
 */
int command_builtin(process_t *proc);

/**
 * @brief	This routine executes a supplied command.
 * 
 * @return	Status
 */
int command_execute(job_t *job, process_t *proc, int in_fd, int out_fd,
					int mode);

/**
 * @brief	This routine checks if a command is built-in.
 * 
 * @return	1 if the command is built-in, 0 otherwise.
 */
int command_get_type(char *command);

#endif // __COMMAND_H_
