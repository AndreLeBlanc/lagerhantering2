#ifndef ALLOC_CHECK_H
#define ALLOC_CHECK_H

#include <stdio.h>
#include <stdlib.h>


/** Checks if allocation of memory was successful
 *  If memory was successfully allocated do nothing, else print error message and terminate program 
 *  \param p pointer to supposedly allocated memory
 */
void check_allocation(void* p);

#endif
