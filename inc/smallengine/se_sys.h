#ifndef __SE_SYS_H__
#define __SE_SYS_H__

#include <stdint.h>

struct timer {
        uint32_t frame_rate;    // desired frame rate
        uint32_t frame_ms;      // milliseconds per frame
        uint32_t total_ms;      // total time since initialization
        uint32_t last_time;     // time stamp of last frame
        uint32_t cur_time;      // time stamp of current frame
        uint32_t delta_ms;      // milliseconds since last frame
        double delta_s;         // seconds since last frame
};

/*****************
 * ARGS
 ****************/

/*
 * arg
 * handles all command line argument checking code
 */

/*
 * pass the programs argument count and pointer to the arg module
 */
void arg_init(int argc, char **argv);

/*
 * get number of arguments
 */
int arg_number(void);

/*
 * check if a parameter exists and give its index in the list
 */
int arg_check(char *arg);

/*
 * return a pointer to the argument at the given index, NULL on fail
 */
char *arg_get(int i);

/*******************
 * MEMORY
 ******************/

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

/*****************
 * TIMER
 ****************/

/*
 * Initialise the clock, returns 1 if successful
 * if 0 is passed, the timer will run as fast as it can
 */
struct timer timer_init(uint32_t frame_rate);

/*
 * update the timer and all values in the struct
 */
void timer_tic(struct timer *t);

#endif
