#include <string.h>
#include <stdint.h>
#include <stdio.h>

#include "my_malloc.h"

/**
 * Prints the state of the heap
 *
 * The heap consists of a series of blocks, each of which has a header and a payload.
 * The header is 2 bytes long, and contains the size of the block (in bytes).
 * The payload is the actual data that is stored in the block.
 *
 * The first 2 bytes of the heap (aka: <S>) is an offset to the first free block.
 *
 * The first 2 bytes of each block (aka: <H>) is the size of the block (in bytes).
 *
 * The list of free blocks is a linked list, where each block contains the offset to the next free block.
 * The offset is the number of bytes between the two free blocks.
 */
void print_all_blocks()
{
    uint16_t *s = (uint16_t *)MY_HEAP;
    uint16_t *current = (uint16_t *)(MY_HEAP + *s);

    printf("\033[34;1m=== HEAP STATE ===\n\033[0;1m");
    printf("heap address = %p\n", MY_HEAP);
    printf("1st free block's offset = %u\n", *s);
    printf("\033[34m------------------\n");

    int n = 0;
    int nMax = 10;
    while ((uint8_t *)current < MY_HEAP + sizeof(MY_HEAP))
    {
        uint16_t size = *current;
        uint16_t offset_from_start = (uint8_t *)current - (uint8_t *)MY_HEAP;
        uint16_t next_offset = *(current + 1);

        printf("\033[31m#%d\033[0;1m\t", n);
        printf("\033[32mfree\t");
        printf("\033[33moffset from start =\033[0;1m %u\t", offset_from_start);
        printf("\033[33m%%p =\033[0;1m %p\t", current);
        printf("\033[33msize =\033[0;1m %u\t", size);
        printf("\033[33mnext offset =\033[0;1m %u\n", next_offset);
        
        n++;
        current = (uint16_t *)((uint8_t *)current + next_offset);
        if (n == nMax || next_offset == 0) break;
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
    printf("%s\n", p);
    print_all_blocks();

    char *q = (char *)my_malloc(7);
    strcpy(q, "world!");
    printf("char *q = (char *)my_malloc(7);\nstrcpy(q, \"world!\");\n");
    print_all_blocks();

    char *pq = (char *)my_malloc(13);
    sprintf(pq, "%s %s", p, q);
    printf("char *pq = (char *)my_malloc(12);\nsprintf(pq, \"%%s %%s\", p, q);\n");
    print_all_blocks();

    printf("%d\n", *(uint16_t *)(MY_HEAP + 2));
    my_free(p);
    printf("%d\n", *(uint16_t *)(MY_HEAP + 2));
    printf("my_free(p);\n");
    print_all_blocks();

    my_free(q);
    printf("my_free(q);\n");
    print_all_blocks();

    // p = (char *)my_malloc(6);
    // strcpy(p, "Hello");
    // printf("char *p = (char *)my_malloc(6);\nstrcpy(p, \"Hello\");\n");
    // print_all_blocks();

    // q = (char *)my_malloc(7);
    // strcpy(q, "world!");
    // printf("q = (char *)my_malloc(7);\nstrcpy(q, \"world!\");\n");
    // print_all_blocks();

    // char *a = (char *)my_malloc(10);
    // print_all_blocks();

    // my_free(pq);
    // printf("my_free(pq);\n");
    // print_all_blocks();

    return 0;
}