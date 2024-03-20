/**
 * @file:		src/hashtable.h
 * @author:		Jozef Nagy <schkwve@gmail.com>
 * @copyright:	MIT (See LICENSE.md)
 * @brief:		This file contains the routines for
 * 				hashtable lookup for built-in commands.
 */

#ifndef __HASHTABLE_H_
#define __HASHTABLE_H_

#include <stddef.h>

#include "builtin.h"

typedef struct {
	char *key;
	builtin_func func_ptr;
} hashtable_entry_t;

typedef struct {
	size_t size;
	size_t count;
	hashtable_entry_t **entry;
} hashtable_t;

extern hashtable_t *g_builtin_hashtable;

hashtable_t *hashtable_create(void);
void hashtable_destroy(hashtable_t *hashtable);

void hashtable_insert(hashtable_t *hashtable, const char *key,
					  builtin_func func_ptr);
hashtable_entry_t *hashtable_new_entry(const char *key, builtin_func func_ptr);
builtin_func hashtable_search(hashtable_t *hashtable, const char *key);
int hashtable_get_hash(const char *key, const size_t hashmap_size,
					   const int att);
void hashtable_remove_entry(hashtable_t *hashtable, const char *key);

#endif // __HASHTABLE_H_
