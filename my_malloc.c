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

    // Find the next free block after old_block
    uint8_t *next_block = (uint8_t *)old_block + *old_block;
    int merged_with_next = 0;

    // 1. Merge with the next block if possible
    if (next_free_block && next_block == (uint8_t *)next_free_block)
    {
        // Update next offset
        *(old_block + 1) = *(next_free_block + 1) == 0? 0: *old_block + *(next_free_block + 1);

        // Merge sizes
        *old_block += *next_free_block;
        merged_with_next = 1;
    }

    // 2. Merge with the previous block if possible
    if (prev_free_block && ((uint8_t *)prev_free_block) + *prev_free_block == (uint8_t *)old_block)
    {
        printf("Merging with previous\n");
        // Merge sizes
        *prev_free_block += *old_block;
        // Update next offset
        *(prev_free_block + 1) = *(old_block + 1) == 0? 0: *(prev_free_block + 1) + *(old_block + 1);
        return; // Early return as the block is merged with the previous one
    }

    // 3. Update start if there's no previous free block
    if (!prev_free_block)
    {
        // Update start
        *start = old_block_offset;
        if (!merged_with_next)
        {
            // Update next offset
            *(old_block + 1) = (uint8_t *)next_free_block - (uint8_t *)old_block;
        }
    }
    else
    {
        // Update the next offset of the previous free block
        *(prev_free_block + 1) = (uint8_t *)old_block - (uint8_t *)prev_free_block;
    }
}
