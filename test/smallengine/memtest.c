#include <stdio.h>
#include <string.h>
#include <assert.h>

#include <smallengine/se_sys.h>
#include <smallengine/se_log.h>

/*
 * Test memory system tracks all allocated blocks of memory
 */
void TST_MemTest()
{
        mem_init(1024);
        //mem_print_report();

        assert(mem_total() == 1024);
        assert(mem_used() == 32);

        void *ptr1 = mem_alloc(256);
        assert(mem_used() == 320);

        void *ptr2 = mem_alloc(128);
        void *ptr3 = mem_alloc(64);
        void *ptr4 = mem_alloc(200);
        //mem_print_report();

        assert(mem_used() == 808);
        //mem_print_report();

        // test freeing memory
        //printf("Freeing pointer 3:\n");
        mem_free(ptr3);
        mem_free(ptr3);
        //mem_print_report();

        assert(mem_used() == 744);

        //printf("Freeing pointer 2:\n");
        mem_free(ptr2);
        //mem_print_report();

        assert(mem_used() == 584);
        
        //printf("Freeing pointer 4:\n");
        mem_free(ptr4);
        //mem_print_report();

        assert(mem_used() == 320);
        
        //printf("Freeing pointer 1:\n");
        mem_free(ptr1);
        //mem_print_report();

        assert(mem_used() == 32);
        assert(mem_available() == 992);
        //mem_dump();

        log("[Memory Test] Complete, all tests pass!\n");

        return;
}

void TST_MemoryIntegrity()
{
        mem_free_all();
        
        int *ptr1 = (int *)mem_alloc(128);
        int *ptr2 = (int *)mem_alloc(128);
        void *ptr3 = mem_alloc(128);
        void *ptr4 = mem_alloc(128);

        // corrupt memory
        int *ptr5 = (int *)mem_alloc(64);
        int i = 0;
        for (i = 0; i < 5; i++) {
                ptr1[i] = 0xffffff;
                printf("Writing: %p\n", &ptr1[i]);
        }
        
        for (i = 0; i < 40; i++) {
                ptr2[i] = 0xffffff;
                printf("Writing: %p\n", &ptr2[i]);
        }
        printf("POINTERS: %p, %p\n", ptr1, ptr2);
        //printf("%p\n", ptr3);

        printf("VALUES: %d, %d\n", ptr1[3], ptr2[2]);

        mem_free(ptr3);
        mem_free(ptr4);
        mem_free(ptr5);

        mem_dump();
        mem_print_report();
}

int main()
{
        TST_MemTest();
        //TST_MemoryIntegrity();
       
        mem_destroy();

        return 0;
}
