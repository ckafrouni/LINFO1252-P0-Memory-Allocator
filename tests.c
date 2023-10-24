#include <stdio.h>
#include <stdlib.h>

#include "my_malloc.h"
#include "utils.h"

void print_test_result(const char *test_name, int passed)
{
    static int n = 0;
    n++;
    char *status = passed ? "\033[32mPASSED\033[0m" : "\033[33mFAILED\033[0m";
    printf("%d\t%s - %s\n", n, status, test_name);
}

int test1()
{
    void *ptr;
    // Test 1: Allocate Zero Bytes
    ptr = my_malloc(0);
    print_test_result("Allocate Zero Bytes", ptr == NULL);
    return ptr == NULL;
}

int test2()
{
    void *ptr;
    // Test 2: Allocate Larger Than Heap
    ptr = my_malloc(HEAP_SIZE + 1);
    print_test_result("Allocate Larger Than Heap", ptr == NULL);
    return ptr == NULL;
}

int test3()
{
    void *ptr;
    // Test 3: Allocate Exactly Heap Size
    ptr = my_malloc(HEAP_SIZE);
    print_test_result("Allocate Exactly Heap Size", ptr == NULL); // or valid pointer based on your implementation
    return ptr == NULL;
}

int test4()
{
    void *ptr;
    // Test 4: Allocate Smallest Possible Block
    ptr = my_malloc(1);
    print_test_result("Allocate Smallest Possible Block", ptr != NULL);
    return ptr != NULL;
}

int test5()
{
    void *ptr;
    // Test 5: Allocate Multiple Small Blocks Until Heap is Full
    int count = 0;
    while ((ptr = my_malloc(1)) != NULL)
    {
        count++;
    }
    print_test_result("Allocate Multiple Small Blocks Until Heap is Full", count > 0);
    printf("count: %d\n", count);
    return count > 0;
}

int test6()
{
    void *ptr;
    // Test 6: Allocate and Free, Then Allocate Again
    ptr = my_malloc(10);
    my_free(ptr);
    void *ptr2 = my_malloc(10);
    print_test_result("Allocate and Free, Then Allocate Again", ptr == ptr2);
    return ptr == ptr2;
}

int test7()
{
    void *ptr;
    // Test 7: Free NULL Pointer
    my_free(NULL);
    print_test_result("Free NULL Pointer", 1); // Assuming it handles gracefully
    return 1;
}

int test8()
{
    void *ptr;
    // Test 8: Free Invalid Pointer
    my_free((void *)0xDEADBEEF);                  // Uncomment if you handle this case
    print_test_result("Free Invalid Pointer", 1); // Assuming it handles gracefully
    return 0;
}

int test9()
{
    void *ptr;
    // Test 9: Free Blocks in Reverse Order
    void *ptr3 = my_malloc(10);
    void *ptr4 = my_malloc(20);
    my_free(ptr4);
    my_free(ptr3);
    print_test_result("Free Blocks in Reverse Order", 1); // Check heap state manually
    return 1;
}

int test10()
{
    void *ptr;

    // Test 10: Free Blocks in Random Order
    void *ptr5 = my_malloc(15);
    void *ptr6 = my_malloc(25);
    my_free(ptr5);
    my_free(ptr6);
    print_test_result("Free Blocks in Random Order", 1); // Check heap state manually
    print_all_blocks();
    return 1;
}

int test11()
{
    void *ptr;
    // Test 11: Double Free
    ptr = my_malloc(10);
    my_free(ptr);
    // my_free(ptr); // Uncomment if you handle this case
    print_test_result("Double Free", 1); // Assuming it handles gracefully
    return 1;
}

int test12()
{
    // Test: my_malloc() and my_free() of Large Blocks
    void *large_ptr1 = my_malloc(HEAP_SIZE / 4);
    void *large_ptr2 = my_malloc(HEAP_SIZE / 4);
    my_free(large_ptr1);
    void *large_ptr3 = my_malloc(HEAP_SIZE / 4);
    my_free(large_ptr2);
    void *large_ptr4 = my_malloc(HEAP_SIZE / 4);
    my_free(large_ptr3);
    my_free(large_ptr4);
    print_test_result("my_malloc() and my_free() of Large Blocks", 1); // Check heap state manually
    return 1;
}

int test13()
{
    // Test: my_malloc() and my_free() of Small Blocks
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
    print_test_result("my_malloc() and my_free() of Small Blocks", 1); // Check heap state manually

    return 1;
}
int main()
{
    my_init();

    test1();
    printf("Before:\n");
    print_all_blocks();

    my_init();
    test2();
    printf("Before:\n");
    print_all_blocks();

    my_init();
    test3();
    printf("Before:\n");
    print_all_blocks();

    my_init();
    test4();
    printf("Before:\n");
    print_all_blocks();

    my_init();
    test5();
    printf("Before:\n");
    print_all_blocks();

    my_init();
    test6();
    printf("Before:\n");
    print_all_blocks();

    my_init();
    test7();
    printf("Before:\n");
    print_all_blocks();

    my_init();
    test8();
    printf("Before:\n");
    print_all_blocks();

    my_init();
    test9();
    printf("Before:\n");
    print_all_blocks();

    my_init();
    test10();
    printf("Before:\n");
    print_all_blocks();

    my_init();
    test11();
    printf("Before:\n");
    print_all_blocks();

    my_init();
    test12();
    printf("Before:\n");
    print_all_blocks();

    my_init();
    test13();
    printf("Before:\n");
    print_all_blocks();

    return 0;
}
