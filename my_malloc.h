#ifndef MY_MALLOC_H
#define MY_MALLOC_H

#include <stddef.h>
#include <stdint.h>

extern uint8_t MY_HEAP[64000];

// Initialize the memory allocator
void my_init();

// Allocate a block of memory of the given size
void *my_malloc(size_t size);

// Free a previously allocated block of memory
void my_free(void *ptr);

#endif // MY_MALLOC_H
