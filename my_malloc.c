#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>

#include "my_malloc.h"

uint8_t MY_HEAP[64000];

void my_init()
{
    const uint16_t METADATA_SIZE = 2;
    const uint16_t HEAP_SIZE = 64000;

    // Initialize start block to point to 1st free block
    uint16_t *start = (uint16_t *)MY_HEAP;
    uint16_t *block = (uint16_t *)(MY_HEAP + 1 * METADATA_SIZE);

    *start = METADATA_SIZE; // offset to first free block

    uint16_t block_size = HEAP_SIZE - 1 * METADATA_SIZE;
    *(block + 0) = block_size; // block size (in bytes)
    *(block + 1) = 0;          // offset to next free block (0)
}

void *my_malloc(size_t size)
{
    const uint16_t METADATA_SIZE = 2;
    const uint16_t MIN_BLOCK_SIZE = 2 * METADATA_SIZE; // header, and next offset

    size += 1 * METADATA_SIZE; // Additional words for header
    size = size < MIN_BLOCK_SIZE ? MIN_BLOCK_SIZE : size;

    uint16_t *start = (uint16_t *)MY_HEAP;
    uint16_t *current = (uint16_t *)(MY_HEAP + *start);
    uint16_t *prev = NULL;

    while (current != NULL)
    {
        uint16_t block_size = *current;
        if (block_size >= size)
        {
            uint16_t remaining_size = block_size - size;
            uint16_t *new_block = NULL;

            if (remaining_size >= MIN_BLOCK_SIZE)
            {
                // Split block
                new_block = (uint16_t *)((uint8_t *)current + size);
                *new_block = remaining_size;
                *(new_block + 1) = *(current + 1) == 0 ? 0 : *(current + 1) - size; // offset to next free block
            }

            if (prev == NULL)
            {
                // (prev == NULL) only at the first iteration
                *start = new_block == NULL ? *start + *(current + 1) : (uint8_t *)new_block - MY_HEAP; // offset from MY_HEAP
            }
            else
            {
                *(prev + 1) = new_block == NULL ? *(prev + 1) + *(current + 1) : (uint8_t *)new_block - (uint8_t *)prev;
            }

            // Update current block
            *current = size;

            return (void *)(current + 1);
        }

        prev = current;
        current = *(current + 1) == 0 ? NULL : current + *(current + 1);
    }

    return NULL;
}

void my_free(void *pointer)
{
    const uint16_t HEAP_SIZE = 64000;

    uint16_t *start = (uint16_t *)MY_HEAP;
    uint16_t *old_block = ((uint16_t *)pointer) - 1;

    // Old block's offset
    uint16_t old_block_offset = (uint8_t *)old_block - MY_HEAP;
    if (old_block_offset > HEAP_SIZE || old_block_offset < 2)
        return;

    // Find surrounding free blocks
    uint16_t *prev_free_block = NULL;
    uint16_t *next_free_block = (uint16_t *)(MY_HEAP + *start);
    while (next_free_block < old_block)
    {
        prev_free_block = next_free_block;
        next_free_block = (uint16_t *)((uint8_t *)next_free_block + *(next_free_block + 1));
    }

    // 3 situations:

    printf("Old block: %p\n", old_block);
    printf("Prev free block: %p\n", prev_free_block);
    printf("Next free next: %p\n", next_free_block);

    // 1. We can merge the old block, with the one on the right
    if (((uint8_t *)next_free_block) == (uint8_t *)old_block + *(old_block))
    {
        printf("Monkey 1\n");
        *(old_block + 1) = *(next_free_block + 1) == 0? 0: *old_block + *(next_free_block + 1);
        *old_block += *next_free_block; // merge sizes
        next_free_block = *(next_free_block + 1) == 0? NULL: (uint16_t *)(((uint8_t *)old_block) + *(old_block+1));
    }

    // 2. There is no previous free block
    if (!prev_free_block) {
        *start = old_block_offset;
        printf("Monkey 2\n");
        *(old_block + 1) = (uint8_t *)next_free_block - (uint8_t *)old_block;
    } 

    // 3. The old block should be merged the previous block
    if ( prev_free_block && ((uint8_t *)prev_free_block) + *prev_free_block == (uint8_t *)old_block)
    {
        printf("Monkey 3\n");
        *prev_free_block += *old_block;
    }
}
