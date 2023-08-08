/**
 * @file:		src/builtin.c
 * @author:		Jozef Nagy <schkwve@gmail.com>
 * @copyright:	MIT (See LICENSE.md)
 * @brief:		This file contains the routines for
 * 				built-in commands.
 */

#include <stddef.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <errno.h>

#include <limits.h>
#include <unistd.h>

#include "builtin.h"
#include "hashtable.h"
#include "psh.h"

hashtable_t *g_builtin_hashtable;

/**
 * @brief	This routine creates and fills a hashtable
 * 			with pointers to built-in command functions.
 */
void builtin_init()
{
	g_builtin_hashtable = hashtable_create();

	hashtable_insert(g_builtin_hashtable, "true", psh_true);
	hashtable_insert(g_builtin_hashtable, "false", psh_false);
	hashtable_insert(g_builtin_hashtable, "echo", psh_echo);
	hashtable_insert(g_builtin_hashtable, "exit", psh_exit);
	hashtable_insert(g_builtin_hashtable, "chdir", psh_chdir);
	hashtable_insert(g_builtin_hashtable, "cat", psh_cat);
	hashtable_insert(g_builtin_hashtable, "export", psh_export);
	hashtable_insert(g_builtin_hashtable, "unset", psh_unset);

	// aliases
	hashtable_insert(g_builtin_hashtable, "cd", psh_chdir);
}

/**
 * @brief	This routine returns 0.
 */
int psh_true(process_t *proc)
{
	(void)proc;
	return 0;
}

/**
 * @brief	This routine returns 1.
 */
int psh_false(process_t *proc)
{
	(void)proc;
	return 1;
}

/**
 * @brief	This routines opens and prints the contents of a file from argv[1].
 */
int psh_cat(process_t *proc)
{
	if (proc->argc < 2) {
		// @todo: fix this
		printf("cat: error: file not specified\n");
		return 1;
	}

	char file_buffer;
	FILE *file_ptr = fopen(proc->argv[1], "rb");
	if (file_ptr == NULL) {
		perror("psh");
		return errno;
	}

	file_buffer = fgetc(file_ptr);
	while (file_buffer != EOF) {
		printf("%c", file_buffer);
		file_buffer = fgetc(file_ptr);
	}

	fclose(file_ptr);

	return 0;
}

/**
 * @brief	This routine prints every argument back to stdio.
 */
int psh_echo(process_t *proc)
{
	if (proc->argc < 2) {
		char *output = NULL;
		size_t length = 0;
		getline(&output, &length, stdin);
		printf("%s ", output);
		return 0;
	}
	for (int i = 1; i < proc->argc; i++) {
		if (strncmp(proc->argv[i], "$", 1) == 0) {
			memmove(proc->argv[i], proc->argv[i] + 1, strlen(proc->argv[i]));
			printf("%s ", getenv(proc->argv[i]));
			continue;
		}
		printf("%s ", proc->argv[i]);
	}

	printf("\n");
	return 0;
}

/**
 * @brief	This routine changes the current working directory to argv[1].
 * 			If no arguments are present, change to $HOME.
 */
int psh_chdir(process_t *proc)
{
	if (proc->argc < 2) {
		return chdir(getenv("HOME"));
	}

	return chdir(proc->argv[1]);
}

/**
 * @brief	This routine sets an envirnoment variable
 */
int psh_export(process_t *proc)
{
	if (proc->argc < 2) {
		printf("export: not enough arguments\n");
		return -1;
	}

	return putenv(proc->argv[1]);
}

/**
 * @brief	This routine removes an envirnoment variable
 */
int psh_unset(process_t *proc)
{
	if (proc->argc < 2) {
		printf("unset: not enough arguments\n");
		return -1;
	}

	return unsetenv(proc->argv[1]);
}

/**
 * @brief	This routine exits with an exit code.
 * 			If exit code is not set, return 0.
 */
int psh_exit(process_t *proc)
{
	int code = 0;
	if (proc->argc > 1) {
		code = atoi(proc->argv[1]);
	}

	exit(code);
}
