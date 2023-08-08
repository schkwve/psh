/**
 * @file:		src/cflow.c
 * @author:		Jozef Nagy <schkwve@gmail.com>
 * @copyright:	MIT (See LICENSE.md)
 * @brief:		This file parses the commandline
 * 				to manage control flow.
 */

#include <glob.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <errno.h>

#include "psh.h"
#include "cflow.h"
#include "command.h"

/**
 * @brief	This routine creates a new process structure.
 */
process_t *cflow_parse(char *segment)
{
	int buffer_size = PSH_COMMAND_BUFSIZE;
	int pos = 0;
	char *cmd = strdup(segment);
	char *token;
	char **token_arr = (char **)malloc(buffer_size * sizeof(char *));

	if (!token_arr) {
		perror("psh");
		exit(1);
	}

	while ((token = strtok_r(segment, " \t\r\n\a", &segment))) {
		glob_t glob_buffer;
		int glob_count = 0;
		if (strchr(token, '*') != NULL || strchr(token, '?') != NULL) {
			glob(token, 0, NULL, &glob_buffer);
			glob_count = glob_buffer.gl_pathc;
		}

		if (pos + glob_count >= buffer_size) {
			buffer_size += PSH_COMMAND_BUFSIZE;
			buffer_size += glob_count;
			token_arr =
				(char **)realloc(token_arr, buffer_size * sizeof(char *));
			if (!token_arr) {
				perror("psh");
				exit(1);
			}
		}

		if (glob_count > 0) {
			int i;
			for (i = 0; i < glob_count; i++) {
				token_arr[pos++] = strdup(glob_buffer.gl_pathv[i]);
			}
			globfree(&glob_buffer);
		} else {
			token_arr[pos] = token;
			pos++;
		}
	}

	int i = 0;
	int argc = 0;
	char *in_path = NULL;
	char *out_path = NULL;

	while (i < pos) {
		if (token_arr[i][0] == '<' || token_arr[i][0] == '>') {
			break;
		}
		i++;
	}
	argc = i;

	for (; i < pos; i++) {
		if (token_arr[i][0] == '<') {
			if (strlen(token_arr[i]) == 1) {
				in_path = (char *)malloc((strlen(token_arr[i + 1]) + 1) *
										 sizeof(char));
				strcpy(in_path, token_arr[i + 1]);
				i++;
			} else {
				in_path = (char *)malloc(strlen(token_arr[i]) * sizeof(char));
				strcpy(in_path, token_arr[i] + 1);
			}
		} else if (token_arr[i][0] == '>') {
			if (strlen(token_arr[i]) == 1) {
				out_path = (char *)malloc((strlen(token_arr[i + 1]) + 1) *
										  sizeof(char));
				strcpy(out_path, token_arr[i + 1]);
				i++;
			} else {
				out_path = (char *)malloc(strlen(token_arr[i]) * sizeof(char));
				strcpy(out_path, token_arr[i] + 1);
			}
		} else {
			break;
		}
	}

	for (i = argc; i <= pos; i++) {
		token_arr[i] = NULL;
	}

	process_t *new_proc = (process_t *)malloc(sizeof(process_t));
	new_proc->cmd = cmd;
	new_proc->argv = token_arr;
	new_proc->argc = argc;
	new_proc->in_path = in_path;
	new_proc->out_path = out_path;
	new_proc->pid = -1;
	new_proc->type = command_get_type(token_arr[0]);
	new_proc->next = NULL;

	return new_proc;
}
