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

typedef struct {
	char cur_user[64];
	char cwd[1024];
	job_t *jobs[MAX_JOBS];
} psh_info_t;

extern psh_info_t *shell;

/**
 * @brief	This routine free's all allocated memory
 * 			and destroys the hashtable.
 */
void free_everything(void);

#endif // __PSH_H_
