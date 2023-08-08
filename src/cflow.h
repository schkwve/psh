/**
 * @file:		src/cflow.c
 * @author:		Jozef Nagy <schkwve@gmail.com>
 * @copyright:	MIT (See LICENSE.md)
 * @brief:		This file parses the commandline
 * 				to manage control flow.
 */

#ifndef __CFLOW_H_
#define __CFLOW_H_

#include "psh.h"

/**
 * @brief	This routine creates a new process structure.
 */
process_t *cflow_parse(char *segment);

#endif // __CFLOW_H_
