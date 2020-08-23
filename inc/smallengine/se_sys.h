#ifndef __SE_SYS_H__
#define __SE_SYS_H__

/**
 * @file se_sys.h
 * @author Paul Rafferty
 * @brief Low level system code
 *
 * The @c se_sys module handles low level engine code such as timing, most
 * kernel calls, passed arguments and memory management.
 */

#include <stdlib.h>
#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

/*****************
 * ARGS
 ****************/

/**
 * @brief Pass the programs argument count and pointer to the arg module.
 *
 * @param argc The argument count, take from @c main
 * @param argv The argument list, take from @c main
 */
void arg_init(int argc, char **argv);

/**
 * @brief Get number of arguments.
 *
 * @return @c int specifying number of arguments passed
 */
int arg_number(void);

/**
 * @brief Check if a parameter exists and give its index in the list.
 *
 * @param arg A @c char* string matching the argument to check for
 * @return An @c int giving the desired arguments index, 0 if not found
 */
int arg_check(const char *arg);

/*
 * @brief return a pointer to the argument at the given index, NULL on fail
 *
 * @param i An @c int specifying the index of the argument list to retrieve
 * @return A pointer of type @c char* to the argument, NULL if index is invalid
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

/**
 * @brief Initialise memory
 *
 * Request a block of memory that will be used for all subsequent allocation
 * requests. An implementation of a heap memory system, based on the version
 * in K&R and the DOOM source code
 *
 * @param size The total size of memory to claim for the program, in bytes
 * @return 1 on a successful initialisation, 0 otherwise
 */
int mem_init(size_t size);

/**
 * @brief Clear all sectors, effectively wipe memory
 *
 * Clears allocated memory but doesn't release the memory back to the kernel,
 * allowing it to be reused.
 */
void mem_free_all();

/**
 * @brief Free all memory used by the program.
 *
 * Kernel level free call, releases block of memory claimed by mem_init back
 * the operating system so it can no longer be used. Call this before ending
 * any program that uses mem_init.
 */
void mem_destroy(void);

/**
 * @brief Request a portion of memory (replacement for malloc).
 * 
 * This operates in a similar way to malloc but requests memory from the memory
 * modules own pool reserved using mem_init. This doesn't require a call to the
 * kernel.
 */
void *mem_alloc(size_t size);

/**
 * @brief Free a previously requested portion of memory.
 *
 * Request memory from initialised pool, doesn't request memory from kernel.
 *
 * @param size Size of memory required, in bytes
 * @return A pointer to the block of memory requested, NULL on fail
 */
void mem_free(void *ptr);

/**
 * @brief Checks the integrity of the memory pool.
 *
 * Each allocated block of memory is marked with a number, if that number has 
 * been changed, the memory is considered corrupted. This function checks all
 * allocated blocks of memory for corruption.
 *
 * @return 1 if corruption detected, 0 otherwise
 */
int mem_valid();

/**
 * @brief Reports how much memory is being used in total by the program.
 *
 * This returns the size of the initial block requested by mem_init.
 *
 * @return An @c int representing the total number of bytes in main pool.
 */
size_t mem_total();

/**
 * @brief Reports how much memory is being used from the main memory pool.
 *
 * Adds the size of all allocated blocks from the memory pool and reports how
 * much is in use.
 *
 * @return An @c int representing the number of used bytes of memory.
 */size_t mem_used();

/**
 * @brief Returns number of free bytes.
 *
 * Calculates the size of unallocated memory in the memory pool.
 *
 * @return An @c int representing the number of free bytes of memory.
 */
size_t mem_available();

/**
 * @brief Prints a summary of memory sectors to the terminal.
 *
 * This will print information about every sector of memory in the main memory
 * pool to the terminal, including a corruption check. Prints to stdout.
 */
void mem_print_report();

/**
 * @brief Prints all bytes in memory to the terminal in hexadecimal form.
 *
 * This will print the values of all bytes of memory to the terminal, 8 bytes 
 * to a line, in hexadecimal. This could be a lot of information depending on
 * the size of the memory pool.  Possibly useless.
 */
void mem_dump();

/*****************
 * TIMER
 ****************/

/**
 * @brief A timer used to control timing of main game loop.
 *
 * This object allows a frame rate or milliseconds per frame to be specified.
 * Each time it is updated it records how long ago the last frame ended and 
 * can calculate how long the system will need to wait to match the desired
 * frame rate. It also records total time since initialisation.
 */
struct timer {
        uint32_t frame_rate;    /** desired frame rate*/
        uint32_t frame_ms;      /** milliseconds per frame*/
        uint32_t total_ms;      /** total time since initialization*/
        uint32_t last_time;     /** time stamp of last frame*/
        uint32_t cur_time;      /** time stamp of current frame*/
        uint32_t delta_ms;      /** milliseconds since last frame*/
        double delta_s;         /** seconds since last frame*/
};


/**
 * @brief Initialise the clock and set desired framerate
 *
 * Set up the clock to run at the specified framerate, if 0 is set for
 * framerate then the timer will not set a time to wait between frames and the
 * loop will run at the max speed permitted by the cpu.
 *
 * @param frame_rate Desired refresh rate in frames per second
 * @return 1 if timer was initialised successfully, 0 otherwise
 */
struct timer timer_init(uint32_t frame_rate);

/*
 * @brief Update the timer and all values in the struct.
 *
 * This will cause information such as time since last tic, total time and
 * duration of last frame etc to be set in the timer.
 *
 * @param t A pointer to a @c timer struct
 */
void timer_tic(struct timer *t);

#ifdef __cplusplus
}
#endif

#endif
