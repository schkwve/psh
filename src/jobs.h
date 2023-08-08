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

/**
 * @brief	This routine find a valid and free job ID.
 * 
 * @return	Job ID if a free one was found. Otherwise, -1.
 */
int job_get_next_id();

/**
 * @brief	This routine prints the PID of the job.
 * 
 * @return	-1 if job ID is invalid or the job doesn't exist. Otherwise, 0.
 */
int job_print_proc(int id);

/**
 * @brief	This routine prints the status of the job.
 * 
 * @return	-1 if job ID is invalid or the job doesn't exist. Otherwise, 0.
 */
int job_print_status(int id);

/**
 * @brief	This routine adds a new job
 * 
 * @return	Assigned job ID if successful. Otherwise, -1.
 */
int job_insert(job_t *job);

/**
 * @brief	This routine removes a job.
 * 
 * @return	-1 if the ID is invalid or the job doesn't exist. Otherwise, 0.
 */
int job_remove(int id);

/**
 * @brief	This routine launches the command job
 * 
 * @return	Exit code
 */
int job_run(job_t *job);

/**
 * @brief	This routine sets a status to the supplied PID.
 * 
 * @return	0 if status has been set successfully. Otherwise, -1.
 */
int job_set_proc_status(int pid, int status);

/**
 * @brief	This routine finds PID in jobs
 */
int job_pid_to_id(int pid);

/**
 * @brief	This routine converts job ID to PID
 */
int job_id_to_pid(int id);

/**
 * @brief	This routine handles job status
 */
void job_check_zombie();

/**
 * @brief	This routine waits until a job is done and sets appropriate status.
 * 
 * @return	Status.
 */
int job_wait(int id);

/**
 * @brief	This routine waits until a job is done and sets appropriate status.
 * 
 * @return	Status.
 */
int job_wait_pid(int pid);

/**
 * @brief	This routine gets the count of current processes
 *
 * @return	Number of processes running
 */
int job_get_proc_count(int id, int filter);

/**
 * @brief	This routine checks if a job is completed.
 */
int job_is_completed(int id);

#endif // __JOBS_H_
