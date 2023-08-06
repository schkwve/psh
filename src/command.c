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
#include "command.h"
#include "builtin.h"

/**
 * @brief	This routine parses user input and handles it.
 * 
 * @return	Exit code
 */
int command_parse(char *buffer)
{
	int buffer_size = PSH_COMMAND_BUFSIZE;
	int argc = 0;
	char *token;
	char **argv = malloc(PSH_COMMAND_BUFSIZE * sizeof(char *));
	if (argv == NULL) {
		perror("psh");
		exit(1);
	}

	while ((token = strtok_r(buffer, " \t\r\n", &buffer))) {
		argv[argc] = token;
		argc++;

		if (buffer_size >= PSH_COMMAND_BUFSIZE) {
			buffer_size += PSH_COMMAND_BUFSIZE;
			argv = realloc(argv, PSH_COMMAND_BUFSIZE * sizeof(char *));
			if (argv == NULL) {
				perror("psh");
				exit(1);
			}
		}
	}

	argv[argc] = NULL;

	int builtin_lookup = command_check_builtin(argc, (const char **)argv);
	if (builtin_lookup != 0 && builtin_lookup != -255) {
		perror(argv[0]);
		return builtin_lookup;
	} else if (builtin_lookup == 0) {
		return builtin_lookup;
	}

	int stat_loc;
	pid_t child_pid = fork();

	if (child_pid == 0) {
		if (execvp(argv[0], argv) < 0) {
			perror("psh");
			exit(1);
		}
	} else {
		waitpid(child_pid, &stat_loc, WUNTRACED);
	}

	return -255;
}

/**
 * @brief	This routine checks if a command is built-in.
 * 
 * @return	1 if the command is built-in, 0 otherwise.
 */
int command_check_builtin(int argc, const char **argv)
{
	// @todo: make argv[0] lowercase
	builtin_func func = hashtable_search(g_builtin_hashtable, argv[0]);
	if (func == NULL) {
		return -255;
	}

	return func(argc, argv);
}
