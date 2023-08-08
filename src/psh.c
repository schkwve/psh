/**
 * @file:		src/psh.c
 * @author:		Jozef Nagy <schkwve@gmail.com>
 * @copyright:	MIT (See LICENSE.md)
 * @brief:		This file contains the main entry point.
 */

#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <signal.h>
#include <unistd.h>
#include <errno.h>

#include "psh.h"
#include "command.h"
#include "jobs.h"
#include "builtin.h"
#include "hashtable.h"

static char *_g_buffer;
psh_info_t *shell;

/**
 * @brief	Main entry point
 */
int main()
{
	atexit(free_everything);

	signal(SIGINT, SIG_IGN);
	signal(SIGTSTP, SIG_IGN);

	shell = (psh_info_t *)malloc(sizeof(psh_info_t));

	for (int i = 0; i <= MAX_JOBS; i++) {
		shell->jobs[i] = NULL;
	}

	char prompt[128];
	char hostname[1024];
	gethostname(hostname, 1024);

	getlogin_r(shell->cur_user, sizeof(shell->cur_user));

	snprintf(prompt, sizeof(prompt), "%s@%s $", shell->cur_user, hostname);

	job_t *job;

	size_t buffer_size = 512;
	_g_buffer = (char *)malloc(buffer_size * sizeof(char));
	if (_g_buffer == NULL) {
		perror("malloc");
		exit(1);
	}

	builtin_init();

	for (;;) {
		printf("%s ", prompt);

		int num_bytes = getline(&_g_buffer, &buffer_size, stdin);
		// EOF
		if (num_bytes == -1) {
			exit(0);
		} else if (num_bytes == 0) {
			continue;
		}

		printf("%d\n", num_bytes);

		// check for C-D / NULL
		if (_g_buffer == NULL) {
			exit(0);
		}

		job = command_parse(_g_buffer);
		job_run(job);
	}

	return 0;
}

/**
 * @brief	This routine free's all allocated memory
 * 			and destroys the hashtable.
 */
void free_everything()
{
	hashtable_destroy(g_builtin_hashtable);
	free(_g_buffer);
}
