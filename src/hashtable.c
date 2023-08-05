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
#include <math.h>

#include "hashtable.h"
#include "builtin.h"

/**
 * @brief	This routine will allocate enough memory to store
 * 			the hashtable and initialize it with NULL values.
 */
hashtable_t *hashtable_create()
{
	hashtable_t *hashtable = malloc(sizeof(*hashtable));

	// 41 is the answer.
	hashtable->size = 41;
	hashtable->count = 0;
	hashtable->entry = calloc(hashtable->size, sizeof(hashtable_entry_t *));

	return hashtable;
}

/**
 * @brief	This routine inserts an entry into a hashtable.
 */
void hashtable_insert(hashtable_t *hashtable, const char *key, builtin_func func_ptr)
{
	hashtable_entry_t *entry = hashtable_new_entry(key, func_ptr);
	int index = hashtable_get_hash(entry->key, hashtable->size, 0);
	hashtable_entry_t *cur = hashtable->entry[index];
	int i = 1;
	while (cur != NULL) {
		index = hashtable_get_hash(entry->key, hashtable->size, i);
		cur = hashtable->entry[index];
		i++;
	}
	hashtable->entry[index] = entry;
	hashtable->count++;
}

/**
 * @brief	This routine creates an hashtable entry
 */
hashtable_entry_t *hashtable_new_entry(const char *key, builtin_func func_ptr)
{
	if (key == NULL || func_ptr == NULL) {
		return NULL;
	}

	hashtable_entry_t *entry = malloc(sizeof(hashtable_entry_t));
	entry->key = strdup(key);
	entry->func_ptr = func_ptr;

	return entry;
}

/**
 * @brief	This routine searches for a key in a hashtable.
 */
builtin_func hashtable_search(hashtable_t *hashtable, const char *key)
{
	int index = hashtable_get_hash(key, hashtable->size, 0);
	hashtable_entry_t *entry = hashtable->entry[index];
	
	int attempt = 1;
	while (entry != NULL) {
		if (strncmp(entry->key, key, strlen(key)) == 0) {
			return entry->func_ptr;
		}
		index = hashtable_get_hash(key, hashtable->size, attempt);
		entry = hashtable->entry[index];
		attempt++;
	}

	return NULL;
}

/**
 * @brief	This routine calculates the hash of a key
 * 			with basic collision mitigation
 */
int hashtable_get_hash(const char *key, const size_t hashmap_size, const int att)
{
	const int a = _hashtable_hash(key, 151, hashmap_size);
	const int b = _hashtable_hash(key, 163, hashmap_size);

	return (a + (att * (b + 1))) % hashmap_size;
}

/**
 * @brief	This routine removes an entry from a hashtable
 */
void hashtable_remove_entry(hashtable_entry_t *entry)
{
	free(entry->key);
	free(entry);
}

/**
 * @brief	This routine removes all entries from a hashtable and
 * 			free()'s it.
 */
void hashtable_destroy(hashtable_t *hashtable)
{
	for (size_t i = 0; i < hashtable->size; i++) {
		hashtable_entry_t *entry = hashtable->entry[i];
		if (entry != NULL) {
			hashtable_remove_entry(entry);
		}
	}

	free(hashtable->entry);
	free(hashtable);
}

/**
 * @brief	This routine hashes a key.
 * 
 * @return	Hash
 */
int _hashtable_hash(const char *key, const int a, const size_t size)
{
	uint64_t hash = 0;
	const size_t keylen = strlen(key);
	
	for (size_t i = 0; i < keylen; i++) {
		hash += (uint64_t)pow(a, keylen - (i + 1)) * key[i];
		hash = hash % size;
	}

	return (int)hash;
}
