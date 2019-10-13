/*
 * (Hopefully) Safe memory allocation and deallocation
 * uses linked lists
 */

#ifndef __mem_h__
#define __mem_h__

#include <stddef.h>

/*
 * Multipliers for easy size requests
 */
#define MEM_KILOBYTE 1024
#define MEM_MEGABYTE 1024*1024
#define MEM_GIGABYTE 1024*1024*1024

enum mem_flag {
        MEM_FREE,
        MEM_USED
};

/*
 * Request a block of memory that will be used for all subsequent allocation
 * requests. An implementation of a heap memory system, based on the version
 * in K&R and the DOOM source code
 */
int mem_init(size_t size);

/*
 * Clear all sectors, effectively wipe memory
 */
void mem_free_all();

/*
 * Free all memory used by the program.
 */
void mem_destroy(void);

/*
 * Request a portion of memory (replacement for malloc)
 */
void *mem_alloc(size_t size);

/*
 * Free a previously requested portion of memory to allow it to be reallocated
 */
void mem_free(void *ptr);

/*
 * Check the validity of memory, returns 1 if no corruption detected
 */
int mem_valid();

/*
 * reports how much memory is being used in total by the program
 */
size_t mem_total();

/*
 * reports how much of the programs internal heap memory is being used
 */
size_t mem_used();

/*
 * returns number of free bytes
 */
size_t mem_available();

/*
 * prints a summary of memory sectors to the console
 */
void mem_print_report();

void mem_dump();

#endif // __mem_h__
