#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/mman.h>
#include <unistd.h>
#include "mmu.h"

void test_memory_functions() {
    printf("Starting memory management function tests...\n\n");

    // Test 1: Basic allocation and deallocation
    printf("Test 1: Basic allocation and deallocation\n");
    void* ptr1 = my_malloc(100);
    if (ptr1 == NULL) {
        printf("FAIL: my_malloc failed to allocate memory\n");
    } else {
        printf("PASS: my_malloc successfully allocated memory\n");
        my_free(ptr1);
        printf("my_free called without errors\n");
    }

    // Test 2: Multiple allocations
    printf("\nTest 2: Multiple allocations\n");
    void* ptr2 = my_malloc(50);
    void* ptr3 = my_malloc(75);
    void* ptr4 = my_malloc(25);
    if (ptr2 && ptr3 && ptr4) {
        printf("PASS: Multiple allocations successful\n");
        my_free(ptr2);
        my_free(ptr3);
        my_free(ptr4);
    } else {
        printf("FAIL: Multiple allocations failed\n");
    }

    // Test 3: Large allocation
    printf("\nTest 3: Large allocation\n");
    void* large_ptr = my_malloc(1000000);
    if (large_ptr) {
        printf("PASS: Large allocation successful\n");
        my_free(large_ptr);
    } else {
        printf("FAIL: Large allocation failed\n");
    }

    // Test 4: my_calloc
    printf("\nTest 4: my_calloc\n");
    int* int_array = (int*)my_calloc(5, sizeof(int));
    if (int_array) {
        int all_zero = 1;
        for (int i = 0; i < 5; i++) {
            if (int_array[i] != 0) {
                all_zero = 0;
                break;
            }
        }
        if (all_zero) {
            printf("PASS: my_calloc allocated and initialized memory to zero\n");
        } else {
            printf("FAIL: my_calloc did not initialize all elements to zero\n");
        }
        my_free(int_array);
    } else {
        printf("FAIL: my_calloc failed to allocate memory\n");
    }

    // Test 5: Allocation after free
    printf("\nTest 5: Allocation after free\n");
    void* ptr5 = my_malloc(100);
    my_free(ptr5);
    void* ptr6 = my_malloc(50);
    if (ptr6) {
        printf("PASS: Successfully allocated memory after freeing\n");
        my_free(ptr6);
    } else {
        printf("FAIL: Failed to allocate memory after freeing\n");
    }

    // Test 6: Stress test
    printf("\nTest 6: Stress test - multiple allocations and frees\n");
    #define NUM_ALLOCS 1000
    void* ptrs[NUM_ALLOCS];
    int success = 1;
    for (int i = 0; i < NUM_ALLOCS; i++) {
        ptrs[i] = my_malloc(rand() % 1000 + 1);
        if (!ptrs[i]) {
            success = 0;
            break;
        }
    }
    for (int i = 0; i < NUM_ALLOCS; i++) {
        if (ptrs[i]) {
            my_free(ptrs[i]);
        }
    }
    if (success) {
        printf("PASS: Stress test completed successfully\n");
    } else {
        printf("FAIL: Stress test failed\n");
    }

    printf("\nMemory management function tests completed.\n");
}

int main() {
    test_memory_functions();
    return 0;
}