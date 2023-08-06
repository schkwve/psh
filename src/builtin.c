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

	// aliases
	hashtable_insert(g_builtin_hashtable, "cd", psh_chdir);
}

/**
 * @brief	This routine returns 0.
 */
int psh_true(int argc, const char **argv)
{
	(void)argc;
	(void)argv;
	return 0;
}

/**
 * @brief	This routine returns 1.
 */
int psh_false(int argc, const char **argv)
{
	(void)argc;
	(void)argv;
	return 1;
}

/**
 * @brief	This routines opens and prints the contents of a file from argv[1].
 */
int psh_cat(int argc, const char **argv)
{
	if (argc < 2) {
		// @todo: fix this
		printf("cat: error: file not specified\n");
		return 1;
	}

	char file_buffer;
	FILE *file_ptr = fopen(argv[1], "rb");
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
int psh_echo(int argc, const char **argv)
{
	for (int i = 1; i < argc; i++) {
		printf("%s ", argv[i]);
	}

	printf("\n");
	return 0;
}

/**
 * @brief	This routine changes the current working directory to argv[1].
 * 			If no arguments are present, change to $HOME.
 */
int psh_chdir(int argc, const char **argv)
{
	if (argc == 1) {
		char *buffer = malloc(PATH_MAX);
		getcwd(buffer, PATH_MAX);
		int ret = chdir(buffer);
		free(buffer);
		return ret;
	}

	return chdir(argv[1]);
}

/**
 * @brief	This routine exits with an exit code.
 * 			If exit code is not set, return 0.
 */
int psh_exit(int argc, const char **argv)
{
	int code = 0;
	if (argc > 1) {
		code = atoi(argv[1]);
	}

	exit(code);
}
