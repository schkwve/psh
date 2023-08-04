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

typedef struct hashtable_entry {
	char *key;
	size_t key_length;
	void *func_ptr;
	struct hashtable_entry *next;
} hashtable_entry_t;

typedef struct {
	uint32_t size;
	hashtable_entry_t **entry;
} hashtable_t;

typedef int (*builtin_cmd)(const char *);

/**
 * @brief	This routine will allocate enough memory to store
 * 			the hashtable and initialize it with NULL values.
 */
hashtable_t *hashtable_create(uint32_t size);

/**
 * @brief	This routine destroys given hashtable
 */
void hashtable_destroy(hashtable_t *hashtable);

/**
 * @brief	This routine calculates the hash of key
 * 
 * @return	Hash of the key
 */
size_t hashtable_hash_str(const char *key, size_t key_length);

/**
 * @brief	This routine inserts an entry into a hashtable
 */
int hashtable_insert(hashtable_t *hashtable, const char *key, void *func_ptr);

/**
 * @brief	This routine calculates the hash of the key.
 * 
 * @return	Hash of key
*/
size_t _hashtable_get_index(hashtable_t *hashtable, const char *key);

#endif // __HASHTABLE_H_
