/**
 * @file:		src/helper.h
 * @author:		Jozef Nagy <schkwve@gmail.com>
 * @copyright:	MIT (See LICENSE.md)
 * @brief:		This file contains helper routines
 * 				to be used by psh.
 */

#ifndef __HELPER_H_
#define __HELPER_H_

/**
 * @brief	This routine trims excessive whitespace
 * 			"  Hello   World" -> "Hello World"
 */
char *strtrim(char *str);

#endif // __HELPER_H_
