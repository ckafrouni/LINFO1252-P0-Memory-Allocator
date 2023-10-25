#include <stdio.h>
#include <stdlib.h>

#include "../src/my_malloc.h"
#include "./framework.h"

int is_pointer_in_heap(void *ptr)
{
    uint8_t *_ptr = (uint8_t *)ptr;
    return _ptr >= MY_HEAP + 4 && _ptr <= MY_HEAP + HEAP_SIZE - MIN_BLOCK_SIZE;
}

int test_initialising()
{
    my_init();

    uint16_t *start = (uint16_t *)MY_HEAP;
    uint16_t *block = (uint16_t *)(MY_HEAP + 1 * METADATA_SIZE);

    if (*start != METADATA_SIZE)
        return 0;

    if (*block != HEAP_SIZE - 1 * METADATA_SIZE)
        return 0;

    if (*(block + 1) != 0)
        return 0;

    return 1;
}

int test_allocating_0_and_1_byte()
{
    my_init();

    void *ptr = my_malloc(0);
    if (ptr != NULL)
        return 0;

    ptr = my_malloc(1);
    if (ptr == NULL)
        return 0;

    if (!is_pointer_in_heap(ptr))
        return 0;

    if (ptr != MY_HEAP + 2 * METADATA_SIZE)
        return 0;

    uint16_t *start = (uint16_t *)MY_HEAP;
    uint16_t *allocated_block = (uint16_t *)(MY_HEAP + 1 * METADATA_SIZE);

    if (*start != METADATA_SIZE + MIN_BLOCK_SIZE)
        return 0;

    if (*allocated_block != MIN_BLOCK_SIZE)
        return 0;

    uint16_t *first_free_block = (uint16_t *)(MY_HEAP + *start);
    if (*first_free_block != HEAP_SIZE - METADATA_SIZE - MIN_BLOCK_SIZE)
        return 0;

    if (*(first_free_block + 1) != 0)
        return 0;

    return 1;
}

int test_alloc_many_small_blocks_until_full()
{
    my_init();
    int block_size = 10 + METADATA_SIZE;
    int max_n_blocks = (HEAP_SIZE - METADATA_SIZE) / block_size;

    void *ptrs[max_n_blocks];
    for (int i = 0; i < max_n_blocks; i++)
    {
        ptrs[i] = my_malloc(block_size - 2);
        if (ptrs[i] == NULL)
            return 0;
        if (!is_pointer_in_heap(ptrs[i]))
            return 0;
    }

    void *ptr = my_malloc(block_size - 2);
    if (ptr != NULL)
        return 0;

    return 1;
}

int test_alloc_many_big_blocks_until_not_enough_space()
{
    my_init();
    int block_size = 6400; // We can fit 9, we would then have (64000 - 9 * 6400) - 2 = 6400 - 2 bytes left
    int max_n_blocks = (HEAP_SIZE - METADATA_SIZE) / block_size;

    void *ptrs[max_n_blocks];
    for (int i = 0; i < max_n_blocks; i++)
    {
        ptrs[i] = my_malloc(block_size - 2);
        if (ptrs[i] == NULL)
            return 0;
        if (!is_pointer_in_heap(ptrs[i]))
            return 0;
    }

    void *ptr = my_malloc(block_size - 2);
    if (ptr != NULL)
        return 0;

    void *ptr2 = my_malloc(block_size - 2 * METADATA_SIZE);
    if (ptr2 == NULL)
        return 0;
    if (!is_pointer_in_heap(ptr2))
        return 0;

    return 1;
}

int test_filling_free_one_then_fill()
{
    my_init();
    int block_size = 6400; // We can fit 9, we would then have (64000 - 9 * 6400) - 2 = 6400 - 2 bytes left
    int max_n_blocks = (HEAP_SIZE - METADATA_SIZE) / block_size;

    void *ptrs[max_n_blocks];
    for (int i = 0; i < max_n_blocks; i++)
    {
        ptrs[i] = my_malloc(block_size - 2);
        if (ptrs[i] == NULL)
            return 0;
        if (!is_pointer_in_heap(ptrs[i]))
            return 0;
    }

    void *ptr = my_malloc(block_size - 2);
    if (ptr != NULL)
        return 0;

    uint16_t *ptr_to_free = ((uint16_t *)ptrs[0]) - 1;
    my_free(ptr_to_free + 1);
    if (*(uint16_t *)MY_HEAP != METADATA_SIZE)
        return 0;

    if (*ptr_to_free != block_size)
        return 0;

    if (*(ptr_to_free + 1) != HEAP_SIZE - block_size)
        return 0;

    void *ptr2 = my_malloc(block_size - 2);
    if (ptr2 == NULL)
        return 0;
    if (!is_pointer_in_heap(ptr2))
        return 0;

    return 1;
}

int main()
{
    register_test("Initialising heap", test_initialising);
    register_test("Allocating 0 and 1 byte", test_allocating_0_and_1_byte);
    register_test("Allocating many small blocks until full", test_alloc_many_small_blocks_until_full);
    register_test("Allocating many big blocks until not enough space", test_alloc_many_big_blocks_until_not_enough_space);
    register_test("Free then fill spot", test_filling_free_one_then_fill);

    printf("\n");
    run_tests("Testing my_malloc and my_free");
    printf("\n");

    return 0;
}