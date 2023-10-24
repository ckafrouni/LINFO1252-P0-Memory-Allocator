#include <string.h>
#include <stdint.h>
#include <stdio.h>

#include "my_malloc.h"
#include "utils.h"

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


int main()
{
    my_init();
    printf("my_init()\n");
    print_all_blocks();

    size_t available = sizeof(MY_HEAP) - 2;
    printf("available : %lu\n", available);

    size_t size = available - 2;
    printf("size : %lu\n", size);
    char *wtf = (char *)my_malloc(size);
    print_all_blocks();
    (void)wtf;

    char *wtf1 = (char *)my_malloc(6);
    print_all_blocks();
    (void)wtf1;

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

    my_free(q);
    printf("my_free(q);\n");
    print_all_blocks();

    printf("%d\n", *(uint16_t *)(MY_HEAP + 2));
    my_free(p);
    printf("%d\n", *(uint16_t *)(MY_HEAP + 2));
    printf("my_free(p);\n");
    print_all_blocks();

    p = (char *)my_malloc(6);
    strcpy(p, "Hello");
    printf("char *p = (char *)my_malloc(6);\nstrcpy(p, \"Hello\");\n");
    print_all_blocks();

    q = (char *)my_malloc(7);
    strcpy(q, "world!");
    printf("q = (char *)my_malloc(7);\nstrcpy(q, \"world!\");\n");
    print_all_blocks();

    char *a = (char *)my_malloc(10);
    print_all_blocks();
    (void)a;

    my_free(pq);
    printf("my_free(pq);\n");
    print_all_blocks();

    return 0;
}