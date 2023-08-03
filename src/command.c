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

#include "command.h"

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

	for (int i = 0; i < argc; i++) {
		printf("%s ", argv[argc]);
	}
}
