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

job_t *command_parse(char *buffer);
int command_builtin(process_t *proc);
int command_execute(job_t *job, process_t *proc, int in_fd, int out_fd,
					int mode);
int command_get_type(char *command);

#endif // __COMMAND_H_
