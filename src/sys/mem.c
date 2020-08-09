#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>     // strerror
#include <errno.h>

#include <smallengine/se_log.h>
#include <smallengine/se_sys.h>

#define MEM_INTEGRITY_CHECK 0x0123dead

/*
 * These structs are given here as they shouldn't be used outside of this
 * module.
 */
struct mem_sector {
        size_t size;
        int integrity;
        enum mem_flag flag;
        struct mem_sector *next;
        struct mem_sector *prev;
};

struct mem_heap {
        size_t size;    // total originally requested
        struct mem_sector *mem_list;
        struct mem_sector *rover;
};

static struct mem_heap *memory = NULL;

/*
 * Allocates memory, aborts program if unsuccessful
 */
static void *_checked_malloc(size_t size)
{
        void *ptr = malloc(size);
        if (ptr == NULL) {
                log_err("Unable to allocate memory!\n");
                fprintf(stderr, "%s\n", strerror(errno));
                mem_destroy();
                exit(1);
        }

        return ptr;
}

static int _corrupted(struct mem_sector *sector)
{
        printf("INT: %x\n", sector->integrity);
        if (sector->integrity != MEM_INTEGRITY_CHECK) {
                return 1;
        }

        return 0;
}

/*
 * Request a block of memory that will be used for all subsequent allocation
 * requests. An implementation of a heap memory system, based on the version
 * in K&R and the DOOM source code
 */
int mem_init(size_t size)
{
        memory = _checked_malloc(size + sizeof(struct mem_heap));
        memory->size = size;

        // point first sector just after the heap header data
        memory->mem_list = 
                (struct mem_sector *)((void *)memory + sizeof(struct mem_heap));

        memory->rover = memory->mem_list;

        memory->mem_list->flag = MEM_FREE;
        memory->mem_list->integrity = MEM_INTEGRITY_CHECK;
        
        // size is what is left after header info for the sector
        memory->mem_list->size = size - sizeof(struct mem_sector);
        memory->mem_list->next = memory->mem_list->prev = memory->mem_list;

        return 1;
}

/*
 * Clear all sectors, effectively wipe memory
 */
void mem_free_all()
{
        struct mem_sector *sector = memory->mem_list;
        sector->prev = sector;
        sector->next = sector;
        sector->flag = MEM_FREE;
        sector->integrity = MEM_INTEGRITY_CHECK;
        sector->size = memory->size - sizeof(struct mem_sector);

        memory->mem_list->next = sector;
        memory->mem_list->prev = sector;
}

/*
 * Free all memory used by the program.
 */
void mem_destroy(void)
{
        free(memory);
}

/*
 * Check the validity of memory, returns 1 if no corruption detected
 */
int mem_valid()
{
        struct mem_sector *sector = memory->mem_list;
        int valid = 1;

        do {
                if (_corrupted(sector)) {
                        log("[WARNING] Corrupted Memory at 0x%ld\n", 
                                (void *)sector - (void *)memory);
                        valid = 0;
                }

                sector = sector->next;
        } while (sector != memory->mem_list);

        return valid;
}
/*
 * Request a portion of memory (replacement for malloc)
 */
void *mem_alloc(size_t size)
{
        // consider checking if memory has been initialised and returning an error
        // or calling malloc (_checked_malloc) instead, would also need to check
        // when freeing this memory as well so may not be worth it

        struct mem_sector *start = memory->rover;
        struct mem_sector *sector = start;
        size_t required = size + sizeof(struct mem_sector);
        size_t remaining = 0;   // for fragments after allocated memory

        do {
                if (sector->flag == MEM_FREE && sector->size >= required) {
                        // set data for the sector
                        remaining = sector->size - size;
                        sector->size = size;
                        sector->flag = MEM_USED;

                        // create a new mem_sector using the remaining memory
                        if (remaining > sizeof(struct mem_sector)) {
                                struct mem_sector *new = 
                                    (struct mem_sector *)((void *)sector + required);

                                new->size = remaining -
                                        sizeof(struct mem_sector);

                                new->flag = MEM_FREE;
                                new->integrity = MEM_INTEGRITY_CHECK;
                                new->prev = sector;
                                new->next = sector->next;
                                sector->next = new;
                        }
                        
                        memory->rover = sector->next;
                        return (void *)((void *)sector + sizeof(struct mem_sector));
                }
                sector = sector->next;
        } while (sector != start);

        log_err("Unable to allocate memory! Quitting...");
        exit(1);
}

/*
 * Free a previously requested portion of memory to allow it to be reallocated
 */
void mem_free(void *ptr)
{
        // ptr points at memory immeditately after sector header, this
        // gets us to the start of the sector header itself
        struct mem_sector *sector = (struct mem_sector *)((void *)ptr - 
                sizeof(struct mem_sector));

        // check pointer is now at a valid sector header
        if (sector->integrity != MEM_INTEGRITY_CHECK) {
                log_err("Attempt to free bad pointer! (Invalid pointer or \
                        corrupted memory sector");
                // return as memory potentially freed and reused already,
                // prev and next point values could be nonsense
                return;
        }

        // check memory isn't already free
        if (sector->flag == MEM_FREE) {
                return;
        }

        sector->flag = MEM_FREE;

        // check if the sectors before and after this are free and merge them
        // do not attempt to connect first and last sectors
        struct mem_sector *check;

        if (sector != memory->mem_list) {
                check = sector->prev;

                if (check->flag == MEM_FREE) {
                        check->size += sector->size + sizeof(struct mem_sector);
                        check->next = sector->next;
                        check->next->prev = check;
                        
                        // move the rover to start looking here if it was sitting
                        // on sector
                        if (memory->rover == sector) {
                                memory->rover = check;
                        }

                        sector = check;
                }
        }

        check = sector->next;
        if (check == sector) { return; }
        if (check == memory->mem_list) { return; } // don't wrap sectors around

        if (check->flag == MEM_FREE) {
                check->prev->next = check->next;
                check->next->prev = check->prev;
                check->prev->size += check->size + sizeof(struct mem_sector);

                if (memory->rover == check) {
                        memory->rover = check->prev;
                }
        }

        return;
}


/*
 * reports how much memory is being used in total by the program
 */
size_t mem_total()
{
        return memory->size;
}

/*
 * reports how much of the programs internal heap memory is being used
 */
size_t mem_used()
{
        struct mem_sector *sector = memory->mem_list;
        size_t total = 0;

        do {
                
                if (sector->flag != MEM_FREE) {
                        total += sector->size;
                }

                // sector header takes up space too
                total += sizeof(struct mem_sector);
                sector = sector->next;
        } while (sector != memory->mem_list);

        return total;
}

/*
 * returns number of free bytes
 */
size_t mem_available()
{
        return memory->size - mem_used();
}

/*
 * return offset from start of memory block
 */
static long unsigned int _get_address(struct mem_sector *sector)
{
        return ((void *)sector - (void *)memory);
}

static void _print_sector_info(struct mem_sector *sector)
{
        log("LOC: 0x%08lx, SECTOR SIZE: %ld bytes, %s, %s [0x%08lx][0x%08lx]\n", 
            (void *)sector - (void *)memory, sector->size,
            (sector->integrity == MEM_INTEGRITY_CHECK) ? "Clean" : "Corrupt",
            (sector->flag > MEM_FREE) ? "Allocated" : "Free", 
            _get_address(sector->prev), _get_address(sector->next));
}

/*
 * prints a summary of memory sectors to the console
 */
void mem_print_report()
{
        struct mem_sector *start = memory->mem_list;
        struct mem_sector *sector = start;

        log("Total Memory: %ld\n", memory->size);

        do {
                _print_sector_info(sector);
                sector = sector->next;
        } while (sector != start);

        log("ROVER:\n");
        _print_sector_info(memory->rover);
        log("\n");
}

void mem_dump()
{
        printf("MAIN MEMORY AT: %p\n", memory);
        uint8_t *ptr = (uint8_t *)memory;
        int i = 0;
        int width = 8;

        while(i < memory->size + sizeof(struct mem_heap)) {
                printf("[0x%04x] 0x%02x%s", i, *(ptr+i), (i % (width) == (width-1)) ? "\n" : "  ");
                i++;
        }
        printf("\n\n");

        int x = 0;
        uint32_t *i_ptr = (uint32_t *)((uint8_t *)memory);
        while(x <256) {
                printf("0x%08x%s", *(i_ptr+x), (x % (width) == (width-1)) ? "\n" : "  ");
                x++;
        }
        printf("\n\n");
}

