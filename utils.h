#if !defined(UTILS_H)
#define UTILS_H

#include <stdio.h>
#include <stdint.h>

#include "my_malloc.h"

void print_all_blocks()
{
    uint16_t *s = (uint16_t *)MY_HEAP;
    uint16_t *current = (uint16_t *)(MY_HEAP + *s);

    printf("\033[34;1m=== HEAP STATE ===\n\033[0;1m");
    printf("heap address = %p\n", MY_HEAP);
    printf("1st free block's offset = %u\n", *s);
    printf("\033[34m------------------\n");

    if (*s == 0)
        return;

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
        if (n == nMax || next_offset == 0)
            break;
    }

    printf("\033[34m==================\n\n\033[0m");
}

#endif // UTILS_H
