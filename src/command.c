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

#include "hashtable.h"
#include "command.h"
#include "builtin.h"

/**
 * @brief	This routine parses user input and handles it.
 */
void command_parse(char *buffer)
{
	int buffer_size = PSH_COMMAND_BUFSIZE;
	char *token;
	
	int argc = 0;
	char **argv = malloc(PSH_COMMAND_BUFSIZE * sizeof(char *));
	if (argv == NULL) {
		perror(strerror(errno));
		exit(1);
	}
	
	while ((token = strtok_r(buffer, " ", &buffer))) {
		argv[argc] = token;
		argc++;

		if (buffer_size >= PSH_COMMAND_BUFSIZE) {
			buffer_size += PSH_COMMAND_BUFSIZE;
			argv = realloc(argv, PSH_COMMAND_BUFSIZE * sizeof(char *));
			if (argv == NULL) {
				perror(strerror(errno));
				exit(1);
			}
		}
	}

	argv[argc] = NULL;

	if (command_check_builtin((const char **)argv)) {
		return;
	}

	//if (command_check_path(argv)) {
	//	return;
	//}

	printf("psh: comand not found: %s\n", argv[0]);
}

/**
 * @brief	This routine checks if a command is built-in.
 * 
 * @return	1 if the command is built-in, 0 otherwise.
 */
int command_check_builtin(const char **argv)
{
	// @todo: make argv[0] lowercase
	builtin_func func = hashtable_search(g_builtin_hashtable, argv[0]);
	if (func == NULL) {
		return 0;
	}

	return func(argv);
}
