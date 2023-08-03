/**
 * @file:		src/psh.c
 * @author:		Jozef Nagy <schkwve@gmail.com>
 * @copyright:	MIT (See LICENSE.md)
 * @brief:		This file contains the main entry point.
 */

#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <errno.h>

#include "command.h"

/**
 * @brief	Main entry point
 */
int main()
{
	// user@hostname$
	// ??
	char *prompt = "$";

	size_t buffer_size = 512;
	char *buffer = (char *)malloc(buffer_size * sizeof(char));
	if (buffer == NULL) {
		perror(strerror(errno));
		exit(1);
	}

	for (;;) {
		printf("%s ", prompt);
		int num_bytes = getline(&buffer, &buffer_size, stdin);
		if (num_bytes == -1) {
			perror(strerror(errno));
			exit(1);
		} else if (num_bytes == 0) {
			continue;
		}

		command_parse(buffer);
	}

	free(buffer);
	return 0;
}
