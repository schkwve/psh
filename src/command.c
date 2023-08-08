/**
 * @file:		src/command.c
 * @author:		Jozef Nagy <schkwve@gmail.com>
 * @copyright:	MIT (See LICENSE.md)
 * @brief:		This file contains the routines for
 * 				parsing user input, calling built-in commands
 * 				as well as external binaries.
 */

#include <stddef.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <unistd.h>

#include "hashtable.h"
#include "helper.h"
#include "command.h"
#include "jobs.h"
#include "cflow.h"
#include "builtin.h"

/**
 * @brief	This routine parses user input.
 * 
 * @return	Job structure
 */
job_t *command_parse(char *buffer)
{
	buffer = strtrim(buffer);
	char *cmd = strdup(buffer);
	process_t *root_proc = NULL;
	process_t *proc = NULL;
	char *line_cur = buffer;
	char *c = buffer;
	char *seg;
	int seg_len = 0;
	int mode = FG_EXEC;

	if (buffer[strlen(buffer) - 1] == '&') {
		mode = BG_EXEC;
		buffer[strlen(buffer) - 1] = '\0';
	}

	while (1) {
		if (*c == '\0' || *c == '|') {
			seg = (char *)malloc((seg_len + 1) * sizeof(char));
			strncpy(seg, line_cur, seg_len);
			seg[seg_len] = '\0';

			process_t *new_proc = cflow_parse(seg);
			if (!root_proc) {
				root_proc = new_proc;
				proc = root_proc;
			} else {
				proc->next = new_proc;
				proc = new_proc;
			}

			if (*c != '\0') {
				line_cur = c;
				while (*(++line_cur) == ' ')
					;
				c = line_cur;
				seg_len = 0;
				continue;
			} else {
				break;
			}
		} else {
			seg_len++;
			c++;
		}
	}

	job_t *new_job = (job_t *)malloc(sizeof(job_t));
	new_job->root = root_proc;
	new_job->cmd = cmd;
	new_job->pgid = -1;
	new_job->mode = mode;

	return new_job;
}

/**
 * @brief	This routine finds a builtin function and executes it.
 * 
 * @return	-255 if function wasn't found. Otherwise, the function's return value.
 */
int command_builtin(process_t *proc)
{
	builtin_func func = hashtable_search(g_builtin_hashtable, proc->argv[0]);
	if (func == NULL) {
		return -255;
	}

	return func(proc);
}

/**
 * @brief	This routine executes a supplied command.
 * 
 * @return	Status
 */
int command_execute(job_t *job, process_t *proc, int in_fd, int out_fd,
					int mode)
{
	int status = 0;
	proc->status = STATUS_RUNNING;

	if (proc->type == COMMAND_BUILTIN) {
		int saved_stdout = dup(1);
		int saved_stdin = dup(0);

		if (in_fd != 0) {
			dup2(in_fd, 0);
			close(in_fd);
		}

		if (out_fd != 1) {
			dup2(out_fd, 1);
			close(out_fd);
		}

		status = command_builtin(proc);

		dup2(saved_stdout, 1);
		close(saved_stdout);
		dup2(saved_stdin, 0);
		close(saved_stdin);
	} else {
		pid_t child_pid = fork();

		if (child_pid < 0) {
			return -1;
		} else if (child_pid == 0) {
			signal(SIGINT, SIG_DFL);

			proc->pid = getpid();
			if (job->pgid > 0) {
				setpgid(0, job->pgid);
			} else {
				job->pgid = proc->pid;
				setpgid(0, job->pgid);
			}

			if (in_fd != 0) {
				dup2(in_fd, 0);
				close(in_fd);
			}

			if (out_fd != 1) {
				dup2(out_fd, 1);
				close(out_fd);
			}

			if (execvp(proc->argv[0], proc->argv) < 0) {
				perror(proc->argv[0]);
				exit(1);
			}
		} else {
			proc->pid = child_pid;
			if (job->pgid > 0) {
				setpgid(child_pid, job->pgid);
			} else {
				job->pgid = proc->pid;
				setpgid(child_pid, job->pgid);
			}

			if (mode == FG_EXEC) {
				tcsetpgrp(0, job->pgid);
				status = job_wait(job->id);
				signal(SIGTTOU, SIG_IGN);
				tcsetpgrp(0, getpid());
				signal(SIGTTOU, SIG_DFL);
			}
		}
	}

	return status;
}

/**
 * @brief	This routine checks if a command is built-in.
 * 
 * @return	1 if the command is built-in, 0 otherwise.
 */
int command_get_type(char *command)
{
	builtin_func func = hashtable_search(g_builtin_hashtable, command);
	if (func == NULL) {
		return COMMAND_EXTERNAL;
	}

	return COMMAND_BUILTIN;
}
