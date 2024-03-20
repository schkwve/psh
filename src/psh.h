/**
 * @file:		src/psh.c
 * @author:		Jozef Nagy <schkwve@gmail.com>
 * @copyright:	MIT (See LICENSE.md)
 * @brief:		This file contains the main entry point.
 */

#ifndef __PSH_H_
#define __PSH_H_

#include <sys/types.h>

#include "jobs.h"

#define SHELL_NAME "Pretty SHell"
#define SHELL_VERSION "0.1"
#define SHELL_COPYRIGHT "Copyright (c) Jozef Nagy 2023-2024"

typedef struct {
	char cur_user[64];
	char cwd[1024];
	job_t *jobs[MAX_JOBS];
} psh_info_t;

extern psh_info_t *shell;

void free_everything(void);

#endif // __PSH_H_
