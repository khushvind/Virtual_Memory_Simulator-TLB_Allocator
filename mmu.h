#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/mman.h>
#include <unistd.h>


// Block structure to manage memory
typedef struct Block {
    size_t size;
    struct Block* next;
    struct Block* prev;
    int free;
} Block;

Block* free_blocks = NULL;
#define BLOCK_SIZE sizeof(Block)


// Function to find a free block
Block* get_free_block(size_t size) {
    if (size <= 0) {
        fprintf(stderr, "Invalid size requested\n");
        return NULL;
    }
    Block* current = free_blocks;
    while (current) {
        if (current->free && current->size >= size) {
            return current;
        }
        current = current->next;
    }
    return NULL;
}

// Function to request memory from the system
Block* request_memory(size_t size) {
    if (size <= 0) {
        fprintf(stderr, "Invalid size requested\n");
        return NULL;
    }

    size_t total_size = size + BLOCK_SIZE;
    void* request = mmap(NULL, total_size, PROT_READ | PROT_WRITE, MAP_PRIVATE | MAP_ANONYMOUS, -1, 0);
    if (request == MAP_FAILED) {
        fprintf(stderr, "mmap failed to allocate memory\n");
        return NULL;
    }

    Block* block = (Block*)request;
    block->free = 0;
    block->size = size;
    block->next = NULL;
    block->prev = NULL;
    return block;
}

// Function to split a block into two if it's too large
void split_block(Block* block, size_t size) {
    if (!block || size <= 0) {
        fprintf(stderr, "Invalid block or size\n");
        return;
    }
    if (block->size >= size + BLOCK_SIZE) {
        Block* splitted_block = (Block*)((char*)block + BLOCK_SIZE + size);
        splitted_block->free = 1;
        splitted_block->size = block->size - size - BLOCK_SIZE;
        splitted_block->next = block->next;
        splitted_block->next = NULL;
        block->size = size;
        block->next = splitted_block;
    }
}

// Function to coalesce adjacent free blocks
void coalesce() {
    Block* current = free_blocks;
    while (current && current->next) {
        if (current->free && current->next->free) {
            current->size += BLOCK_SIZE + current->next->size;
            current->next = current->next->next;
        } else {
            current = current->next;
        }
    }
}

// Function to allocate memory
void* my_malloc(size_t size) {
    if (size <= 0) {
        return NULL;
    }

    Block* block;
    if (free_blocks) {
        block = get_free_block(size);
        if (block) {
            if (block->size > size+BLOCK_SIZE) {
                split_block(block, size);
            }
            block->free = 0;
            return (block + 1);
        }
    }

    block = request_memory(size);
    
    if (!block) return NULL;
    

    if (!free_blocks) {
        free_blocks = block;
    } else {
        Block* current = free_blocks;
        while (current->next) {
            current = current->next;
        }
        current->next = block;
    }

    return (block + 1);
}

// Function to allocate and initialize memory to zero
void* my_calloc(size_t nelem, size_t size) {
    size_t total_size = nelem * size;
    void* ptr = my_malloc(total_size);
    if (ptr) {
        memset(ptr, 0, total_size);
    }
    return ptr;
}

// Function to release memory
void my_free(void* ptr) {
    if (!ptr) {
        return;
    }
    Block* block = (Block*)ptr - 1;
    block->free = 1;
    coalesce();

    // Use munmap to release the memory back to the system
    if (block->prev == NULL && block->next == NULL) {
        munmap(block, block->size + BLOCK_SIZE);
        free_blocks = NULL;
    }
}