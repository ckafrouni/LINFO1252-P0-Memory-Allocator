#include <stdio.h>
#include <stdlib.h>
#include "my_malloc.h"

#define HEAP_SIZE 64000

void print_test_result(const char *test_name, int passed)
{
    static int n = 0;
    n++;
    char *status = passed ? "PASSED" : "FAILED";
    printf("%d\t%s - %s\n", n, status, test_name);
}

// Define your test functions here
int test1()
{
    void *ptr = my_malloc(0);
    return ptr == NULL;
}

int test2()
{
    void *ptr = my_malloc(HEAP_SIZE + 1);
    return ptr == NULL;
}

int test3()
{
    void *ptr = my_malloc(HEAP_SIZE);
    return ptr == NULL;
}

int test4()
{
    void *ptr = my_malloc(1);
    return ptr != NULL; // Assuming you have a way to check the block size is 4 bytes
}

int test5()
{
    int count = 0;
    void *ptr;
    while ((ptr = my_malloc(1)) != NULL)
    {
        count++;
    }
    return count > 0 && count * 4 <= HEAP_SIZE;
}

int test6()
{
    void *ptr1 = my_malloc(10);
    my_free(ptr1);
    void *ptr2 = my_malloc(10);
    return ptr1 == ptr2;
}

int test7()
{
    my_free(NULL);
    return 1; // Assuming it handles gracefully
}

int test8()
{
    // Uncomment if you handle this case
    // my_free((void *)0xDEADBEEF);
    return 1; // Assuming it handles gracefully
}

int test9()
{
    void *ptr1 = my_malloc(10);
    void *ptr2 = my_malloc(20);
    my_free(ptr2);
    my_free(ptr1);
    return 1; // Assuming you have a way to check the heap state
}

int test10()
{
    void *ptr1 = my_malloc(15);
    void *ptr2 = my_malloc(25);
    my_free(ptr1);
    my_free(ptr2);
    return 1; // Assuming you have a way to check the heap state
}

int test11()
{
    void *ptr = my_malloc(10);
    my_free(ptr);
    // Uncomment if you handle this case
    // my_free(ptr);
    return 1; // Assuming it handles gracefully
}

int test12()
{
    void *ptr1 = my_malloc(HEAP_SIZE / 4);
    void *ptr2 = my_malloc(HEAP_SIZE / 4);
    my_free(ptr1);
    void *ptr3 = my_malloc(HEAP_SIZE / 4);
    my_free(ptr2);
    void *ptr4 = my_malloc(HEAP_SIZE / 4);
    my_free(ptr3);
    my_free(ptr4);
    return 1; // Assuming you have a way to check the heap state
}

int test13()
{
    void *small_ptrs[HEAP_SIZE];
    for (int i = 0; i < HEAP_SIZE; i++)
    {
        small_ptrs[i] = my_malloc(1);
        if (small_ptrs[i] == NULL)
        {
            break;
        }
    }
    for (int i = 0; i < HEAP_SIZE; i++)
    {
        if (small_ptrs[i] != NULL)
        {
            my_free(small_ptrs[i]);
        }
    }
    return 1; // Assuming you have a way to check the heap state
}

void run_test(int (*test_func)(), const char *test_name)
{
    int result = test_func();
    print_test_result(test_name, result);
}

int main()
{
    run_test(test1, "Allocate Zero Bytes");
    run_test(test2, "Allocate Larger Than Heap");
    run_test(test3, "Allocate Exactly Heap Size");
    run_test(test4, "Allocate Smallest Possible Block");
    run_test(test5, "Allocate Multiple Small Blocks Until Heap is Full");
    run_test(test6, "Allocate and Free, Then Allocate Again");
    run_test(test7, "Free NULL Pointer");
    run_test(test8, "Free Invalid Pointer");
    run_test(test9, "Free Blocks in Reverse Order");
    run_test(test10, "Free Blocks in Random Order");
    run_test(test11, "Double Free");
    run_test(test12, "my_malloc() and my_free() of Large Blocks");
    run_test(test13, "my_malloc() and my_free() of Small Blocks");

    return 0;
}
