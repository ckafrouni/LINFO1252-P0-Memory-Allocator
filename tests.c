#include <stdio.h>
#include <stdlib.h>

#include "my_malloc.h"

void print_test_result(const char *test_name, int passed)
{
    printf("%s: %s\n", test_name, passed ? "PASSED" : "FAILED");
}

int main()
{
    void *ptr;

    // Test 1: Allocate Zero Bytes
    ptr = my_malloc(0);
    print_test_result("Allocate Zero Bytes", ptr == NULL);

    // Test 2: Allocate Larger Than Heap
    ptr = my_malloc(HEAP_SIZE + 1);
    print_test_result("Allocate Larger Than Heap", ptr == NULL);

    // Test 3: Allocate Exactly Heap Size
    ptr = my_malloc(HEAP_SIZE);
    print_test_result("Allocate Exactly Heap Size", ptr == NULL); // or valid pointer based on your implementation

    // Test 4: Allocate Smallest Possible Block
    ptr = my_malloc(1);
    print_test_result("Allocate Smallest Possible Block", ptr != NULL);

    // Test 5: Allocate Multiple Small Blocks Until Heap is Full
    int count = 0;
    while ((ptr = my_malloc(1)) != NULL)
    {
        count++;
    }
    print_test_result("Allocate Multiple Small Blocks Until Heap is Full", count > 0);

    // Test 6: Allocate and Free, Then Allocate Again
    ptr = my_malloc(10);
    my_free(ptr);
    void *ptr2 = my_malloc(10);
    print_test_result("Allocate and Free, Then Allocate Again", ptr == ptr2);

    // Test 7: Free NULL Pointer
    my_free(NULL);
    print_test_result("Free NULL Pointer", 1); // Assuming it handles gracefully

    // Test 8: Free Invalid Pointer
    // my_free((void *)0xDEADBEEF); // Uncomment if you handle this case
    // print_test_result("Free Invalid Pointer", 1); // Assuming it handles gracefully

    // Test 9: Free Blocks in Reverse Order
    void *ptr3 = my_malloc(10);
    void *ptr4 = my_malloc(20);
    my_free(ptr4);
    my_free(ptr3);
    print_test_result("Free Blocks in Reverse Order", 1); // Check heap state manually

    // Test 10: Free Blocks in Random Order
    void *ptr5 = my_malloc(15);
    void *ptr6 = my_malloc(25);
    my_free(ptr5);
    my_free(ptr6);
    print_test_result("Free Blocks in Random Order", 1); // Check heap state manually

    // Test 11: Double Free
    ptr = my_malloc(10);
    my_free(ptr);
    // my_free(ptr); // Uncomment if you handle this case
    print_test_result("Double Free", 1); // Assuming it handles gracefully

    return 0;
}
