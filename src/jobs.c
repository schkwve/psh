/**
 * @file:		src/jobs.c
 * @author:		Jozef Nagy <schkwve@gmail.com>
 * @copyright:	MIT (See LICENSE.md)
 * @brief:		This file manages multiple jobs
 * 				running in the shell.
 */

#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

#include "command.h"
#include "jobs.h"
#include "psh.h"

const char *g_proc_status[] = { "running", "done", "suspended", "continued",
								"terminated" };

/**
 * @brief	This routine find a valid and free job ID.
 * 
 * @return	Job ID if a free one was found. Otherwise, -1.
 */
int job_get_next_id()
{
	for (int i = 0; i <= MAX_JOBS; i++) {
		if (shell->jobs[i] == NULL) {
			return i;
		}
	}

	return -1;
}

/**
 * @brief	This routine prints the PID of the job.
 * 
 * @return	-1 if job ID is invalid or the job doesn't exist. Otherwise, 0.
 */
int job_print_proc(int id)
{
	if (id > MAX_JOBS || shell->jobs[id] == NULL) {
		return -1;
	}

	printf("[%d]", id);

	process_t *proc;
	for (proc = shell->jobs[id]->root; proc != NULL; proc = proc->next) {
		printf(" %d", proc->pid);
	}
	printf("\n");

	return 0;
}

/**
 * @brief	This routine prints the status of the job.
 * 
 * @return	-1 if job ID is invalid or the job doesn't exist. Otherwise, 0.
 */
int job_print_status(int id)
{
	if (id > MAX_JOBS || shell->jobs[id] == NULL) {
		return -1;
	}

	printf("[%d]", id);

	process_t *proc;
	for (proc = shell->jobs[id]->root; proc != NULL; proc = proc->next) {
		printf("\t%d\t%s\t%s", proc->pid, g_proc_status[proc->status],
			   proc->cmd);
		if (proc->next != NULL) {
			printf("|\n");
		} else {
			printf("\n");
		}
	}

	return 0;
}

/**
 * @brief	This routine adds a new job
 * 
 * @return	Assigned job ID if successful. Otherwise, -1.
 */
int job_insert(job_t *job)
{
	int id = job_get_next_id();

	if (id < 0) {
		return -1;
	}

	job->id = id;
	shell->jobs[id] = job;

	return id;
}

/**
 * @brief	This routine removes a job.
 * 
 * @return	-1 if the ID is invalid or the job doesn't exist. Otherwise, 0.
 */
int job_remove(int id)
{
	if (id > MAX_JOBS || shell->jobs[id] == NULL) {
		return -1;
	}

	job_t *job = shell->jobs[id];
	process_t *proc;
	process_t *tmp;
	for (proc = job->root; proc != NULL;) {
		tmp = proc->next;
		free(proc->cmd);
		free(proc->argv);
		free(proc->in_path);
		free(proc->out_path);
		free(proc);
		proc = tmp;
	}
	free(job->cmd);
	free(job);
	shell->jobs[id] = NULL;

	return 0;
}

/**
 * @brief	This routine launches the command job
 * 
 * @return	Exit code
 */
int job_run(job_t *job)
{
	process_t *proc;
	int status = 0;
	int in_fd = 0;
	int fd[2];
	int job_id = -1;

	job_check_zombie();
	if (job->root->type == COMMAND_EXTERNAL) {
		job_id = job_insert(job);
	}

	for (proc = job->root; proc != NULL; proc = proc->next) {
		if (proc == job->root && proc->in_path != NULL) {
			in_fd = open(proc->in_path, O_RDONLY);
			if (in_fd < 0) {
				perror(proc->in_path);
				job_remove(job_id);
				return -1;
			}
		}
		if (proc->next != NULL) {
			pipe(fd);
			status = command_execute(job, proc, in_fd, fd[1], PIPE_EXEC);
			close(fd[1]);
			in_fd = fd[0];
		} else {
			int out_fd = 1;
			if (proc->out_path != NULL) {
				out_fd = open(proc->out_path, O_CREAT | O_WRONLY,
							  S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH);
				if (out_fd < 0) {
					out_fd = 1;
				}
			}
			status = command_execute(job, proc, in_fd, out_fd, job->mode);
		}
	}

	if (job->root->type == COMMAND_EXTERNAL) {
		if (status >= 0 && job->mode == FG_EXEC) {
			job_remove(job_id);
		} else if (job->mode == BG_EXEC) {
			job_print_proc(job_id);
		}
	}

	return status;
}

/**
 * @brief	This routine sets a status to the supplied PID.
 * 
 * @return	0 if status has been set successfully. Otherwise, -1.
 */
int job_set_proc_status(int pid, int status)
{
	int i;
	process_t *proc;

	for (i = 1; i <= MAX_JOBS; i++) {
		if (shell->jobs[i] == NULL) {
			continue;
		}
		for (proc = shell->jobs[i]->root; proc != NULL; proc = proc->next) {
			if (proc->pid == pid) {
				proc->status = status;
				return 0;
			}
		}
	}

	return -1;
}

/**
 * @brief	This routine finds PID in jobs
 */
int job_pid_to_id(int pid)
{
	process_t *proc;
	for (int i = 1; i <= MAX_JOBS; i++) {
		if (shell->jobs[i] != NULL) {
			for (proc = shell->jobs[i]->root; proc != NULL; proc = proc->next) {
				if (proc->pid == pid) {
					return i;
				}
			}
		}
	}

	return -1;
}

/**
 * @brief	This routine converts job ID to PID
 */
int job_id_to_pid(int id)
{
	if (id > MAX_JOBS) {
		return -1;
	}

	return shell->jobs[id]->pgid;
}

/**
 * @brief	This routine handles job status
 */
void job_check_zombie()
{
	int status;
	int pid;

	while ((pid = waitpid(-1, &status, WNOHANG | WUNTRACED | WCONTINUED)) > 0) {
		if (WIFEXITED(status)) {
			job_set_proc_status(pid, STATUS_DONE);
		} else if (WIFSTOPPED(status)) {
			job_set_proc_status(pid, STATUS_SUSPENDED);
		} else if (WIFCONTINUED(status)) {
			job_set_proc_status(pid, STATUS_CONTINUED);
		}

		int job_id = job_pid_to_id(pid);
		if (job_id > 0 && job_is_completed(job_id)) {
			job_print_status(job_id);
			job_remove(job_id);
		}
	}
}

/**
 * @brief	This routine waits until a job is done and sets appropriate status.
 * 
 * @return	Status.
 */
int job_wait(int id)
{
	if (id > MAX_JOBS || shell->jobs[id] == NULL) {
		return -1;
	}

	int proc_count = job_get_proc_count(id, PROC_FILTER_REMAINING);
	int wait_pid = -1;
	int wait_count = 0;
	int status = 0;

	do {
		wait_pid = waitpid(shell->jobs[id]->pgid, &status, WUNTRACED);
		wait_count++;

		if (WIFEXITED(status)) {
			job_set_proc_status(wait_pid, STATUS_DONE);
		} else if (WIFSIGNALED(status)) {
			job_set_proc_status(wait_pid, STATUS_TERMINATED);
		} else if (WSTOPSIG(status)) {
			status = -1;
			job_set_proc_status(wait_pid, STATUS_SUSPENDED);
			if (wait_count == proc_count) {
				job_print_status(id);
			}
		}
	} while (wait_count < proc_count);

	return status;
}

/**
 * @brief	This routine waits until a job is done and sets appropriate status.
 * 
 * @return	Status.
 */
int job_wait_pid(int pid)
{
	int status = 0;

	waitpid(pid, &status, WUNTRACED);
	if (WIFEXITED(status)) {
		job_set_proc_status(pid, STATUS_DONE);
	} else if (WIFSIGNALED(status)) {
		job_set_proc_status(pid, STATUS_TERMINATED);
	} else if (WSTOPSIG(status)) {
		status = -1;
		job_set_proc_status(pid, STATUS_SUSPENDED);
	}

	return status;
}

/**
 * @brief	This routine gets the count of current processes
 *
 * @return	Number of processes running
 */
int job_get_proc_count(int id, int filter)
{
	if (id > MAX_JOBS || shell->jobs[id] == NULL) {
		return -1;
	}

	int count = 0;
	process_t *proc;
	for (proc = shell->jobs[id]->root; proc != NULL; proc = proc->next) {
		if (filter == PROC_FILTER_ALL ||
			(filter == PROC_FILTER_DONE && proc->status == STATUS_DONE) ||
			(filter == PROC_FILTER_REMAINING && proc->status != STATUS_DONE)) {
			count++;
		}
	}

	return count;
}

/**
 * @brief	This routine checks if a job is completed.
 * 
 * @return	If a job is done, return 0. Otherwise 1.
 */
int job_is_completed(int id)
{
	if (id > MAX_JOBS || shell->jobs[id] == NULL) {
		return 0;
	}

	process_t *proc;
	for (proc = shell->jobs[id]->root; proc != NULL; proc = proc->next) {
		if (proc->status != STATUS_DONE) {
			return 0;
		}
	}

	return 1;
}
