#ifndef MY_MALLOC_H
#define MY_MALLOC_H

#include <stdint.h>

#define HEAP_SIZE 64000

#define METADATA_SIZE 2
#define MIN_BLOCK_SIZE (2 * METADATA_SIZE) // header, and next offset

extern uint8_t MY_HEAP[HEAP_SIZE];

// Initialize the memory allocator
void my_init();

// Allocate a block of memory of the given size
void *my_malloc(size_t size);

// Free a previously allocated block of memory
void my_free(void *ptr);

#endif // MY_MALLOC_H
