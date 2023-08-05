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

/**
 * @brief	This routine will allocate enough memory to store
 * 			the hashtable and initialize it with NULL values.
 */
hashtable_t *hashtable_create();

/**
 * @brief	This routine inserts an entry into a hashtable.
 */
void hashtable_insert(hashtable_t *hashtable, const char *key,
					  builtin_func func_ptr);

/**
 * @brief	This routine creates an hashtable entry
 */
hashtable_entry_t *hashtable_new_entry(const char *key, builtin_func func_ptr);

/**
 * @brief	This routine searches for a key in a hashtable.
 */
builtin_func hashtable_search(hashtable_t *hashtable, const char *key);

/**
 * @brief	This routine calculates the hash of a key
 * 			with basic collision mitigation
 */
int hashtable_get_hash(const char *key, const size_t hashmap_size,
					   const int att);

/**
 * @brief	This routine removes an entry from a hashtable.
 */
void hashtable_remove_entry(hashtable_t *hashtable, const char *key);

/**
 * @brief	This routine removes all entries from a hashtable and
 * 			free()'s it.
 */
void hashtable_destroy(hashtable_t *hashtable);

/**
 * @brief	This routine hashes a key.
 * 
 * @return	Hash
 */
int _hashtable_hash(const char *key, const int a, const size_t size);

#endif // __HASHTABLE_H_
