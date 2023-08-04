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

#include "builtin.h"

builtin_cmd_t *builtin_hashtable[BUILTIN_HASHTABLE_SIZE];
