/**
 * @file:		src/jobs.h
 * @author:		Jozef Nagy <schkwve@gmail.com>
 * @copyright:	MIT (See LICENSE.md)
 * @brief:		This file manages multiple jobs
 * 				running in the shell.
 */

#ifndef __JOBS_H_
#define __JOBS_H_

#include <sys/types.h>

#define MAX_JOBS 64

#define BG_EXEC 0
#define FG_EXEC 1
#define PIPE_EXEC 2

#define STATUS_RUNNING 0
#define STATUS_DONE 1
#define STATUS_SUSPENDED 2
#define STATUS_CONTINUED 3
#define STATUS_TERMINATED 4

#define PROC_FILTER_ALL 0
#define PROC_FILTER_DONE 1
#define PROC_FILTER_REMAINING 2

typedef struct process {
	char *cmd;
	int argc;
	char **argv;
	char *in_path;
	char *out_path;
	pid_t pid;
	int type;
	int status;
	struct process *next;
} process_t;

typedef struct {
	int id;
	process_t *root;
	char *cmd;
	pid_t pgid;
	int mode;
} job_t;

int job_get_next_id(void);
int job_print_proc(int id);
int job_print_status(int id);
int job_insert(job_t *job);
int job_remove(int id);
int job_run(job_t *job);
int job_set_proc_status(int pid, int status);
int job_pid_to_id(int pid);
int job_id_to_pid(int id);
void job_check_zombie(void);
int job_wait(int id);
int job_wait_pid(int pid);
int job_get_proc_count(int id, int filter);
int job_is_completed(int id);

#endif // __JOBS_H_
