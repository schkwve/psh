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

#include "psh.h"
#include "command.h"
#include "builtin.h"
#include "hashtable.h"

static char *_g_buffer;

/**
 * @brief	Main entry point
 */
int main()
{
	atexit(free_everything);
	// user@hostname$
	// ??
	char *prompt = "$";

	size_t buffer_size = 512;
	_g_buffer = (char *)malloc(buffer_size * sizeof(char));
	if (_g_buffer == NULL) {
		perror(strerror(errno));
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

		// check for C-D / NULL
		if (_g_buffer == NULL) {
			exit(0);
		}

		command_parse(_g_buffer);
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
