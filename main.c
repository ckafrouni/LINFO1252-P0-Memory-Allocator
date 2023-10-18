#include <string.h>
#include <stdint.h>
#include <stdio.h>

#include "my_malloc.h"

void print_all_blocks()
{
    const uint16_t METADATA_SIZE = 2;
    uint16_t *start = (uint16_t *)MY_HEAP;
    uint16_t *current = (uint16_t *)(MY_HEAP + METADATA_SIZE); // Skip the initial "start" pointer

    printf("\033[34;1m=== HEAP STATE ===\n\033[0;1m");
    printf("heap address = %p\n", MY_HEAP);
    printf("1st free block's offset = %u\n", *start);
    printf("\033[34m------------------\n");

    int num_block = 0;
    while ((uint8_t *)current < MY_HEAP + sizeof(MY_HEAP))
    {
        uint16_t size = *current & ~1; // Mask out the lowest bit to get the size
        uint16_t second_size = *(current + (size / 2) - 1) & ~1;
        int is_allocated = *current & 1; // Check the lowest bit to see if it's allocated
        uint16_t offset = (uint8_t *)current - (uint8_t *)MY_HEAP;

        printf("\033[31m#%d\033[0;1m\t", num_block);
        if (is_allocated)
        {
            printf("\033[33mallocated\toffset = %u\t%%p = %p\tsize = ( %u - %u )\tcontent: \"%s\"\n\033[0;1m", offset, current, size, second_size, (char *)(current + 1));
        }
        else
        {
            uint16_t next_offset = *(current + 1);
            printf("\033[32mfree\t\toffset = %u\t%%p = %p\tsize = ( %u - %u )\tnext offset = %u\n\033[0;1m", offset, current, size, second_size, next_offset);
        }

        num_block++;
        current += size / METADATA_SIZE; // Move to the next block based on the size
    }

    printf("\033[34m==================\n\n\033[0m");
}

int main()
{
    my_init();
    printf("my_init()\n");
    print_all_blocks();

    char *p = (char *)my_malloc(6);
    printf("char *p = (char *)my_malloc(6);\n");
    print_all_blocks();
    strcpy(p, "Hello");
    printf("strcpy(p, \"Hello\");\n");
    print_all_blocks();

    char *q = (char *)my_malloc(7);
    strcpy(q, "world!");
    printf("char *q = (char *)my_malloc(7);\nstrcpy(q, \"world!\");\n");
    print_all_blocks();

    char *pq = (char *)my_malloc(13);
    sprintf(pq, "%s %s", p, q);
    printf("char *pq = (char *)my_malloc(12);\nsprintf(pq, \"%%s %%s\", p, q);\n");
    print_all_blocks();

    my_free(p);
    printf("my_free(p);\n");
    print_all_blocks();

    my_free(q);
    printf("my_free(q);\n");
    print_all_blocks();

    p = (char *)my_malloc(6);
    strcpy(p, "Hello");
    printf("char *p = (char *)my_malloc(6);\nstrcpy(p, \"Hello\");\n");
    print_all_blocks();

    q = (char *)my_malloc(7);
    strcpy(q, "world!");
    printf("q = (char *)my_malloc(7);\nstrcpy(q, \"world!\");\n");
    print_all_blocks();

    char *a = (char *) my_malloc(10);
    print_all_blocks();

    // my_free(pq);
    // printf("my_free(pq);\n");
    // print_all_blocks();

    return 0;
}