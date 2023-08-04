/**
 * @file:		src/hashtable.h
 * @author:		Jozef Nagy <schkwve@gmail.com>
 * @copyright:	MIT (See LICENSE.md)
 * @brief:		This file contains the routines for
 * 				hashtable lookup for built-in commands.
 */

#include <stddef.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "hashtable.h"
#include "builtin.h"

/**
 * @brief	This routine will allocate enough memory to store
 * 			the hashtable and initialize it with NULL values.
 */
hashtable_t *hashtable_create(uint32_t size)
{
	hashtable_t *hashtable = malloc(sizeof(*hashtable));
	hashtable->size = size;
	hashtable->entry = calloc(sizeof(hashtable_entry_t *), hashtable->size);

	return hashtable;
}

/**
 * @brief	This routine destroys given hashtable
 */
void hashtable_destroy(hashtable_t *hashtable)
{
	free(hashtable->entry);
	free(hashtable);
}

/**
 * @brief	This routine calculates the hash of key
 * 
 * @return	Hash of the key
 */
size_t hashtable_hash_str(const char *key, size_t key_length)
{
	(void)key;
	(void)key_length;
	return 0;
}

/**
 * @brief	This routine inserts an entry into a hashtable
 */
int hashtable_insert(hashtable_t *hashtable, const char *key, void *func_ptr)
{
	if (key == NULL || func_ptr == NULL) {
		return 0;
	}

	size_t index = _hashtable_get_index(hashtable, key);

	return index;
}

/**
 * @brief	This routine gets the index from the hash of the key.
 * 
 * @return	Key index
 */
size_t _hashtable_get_index(hashtable_t *hashtable, const char *key)
{
	size_t hash = hashtable_hash_str(key, strlen(key)) % hashtable->size;

	return hash;
}
