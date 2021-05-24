#include <kernel/mem.h>

#include <stdbool.h>

uint32_t *bitmap = (uint32_t *) BITMAP_ADDRESS;

void init_mem() {
    for(int i = 0; i < (BLOCK_SIZE / 4); i++) {
        bitmap[i] = 0;
    }
}

int next_allocable_block() {
    for(int i = 0; i < 110; i++) {
        if(GET_BIT(i) == 0) {
            return i;
        }
    }

    return -1;
}

uint32_t mem_alloc() {
    int block = next_allocable_block();

    if(block == -1) return 0;

    SET_BIT(block);

    return MEM_START + (BLOCK_SIZE * block);
}

void mem_free(uint32_t address) {
    int block = (address - MEM_START) / BLOCK_SIZE;

    CLEAR_BIT(block);
}

// Heap allocation algorithm


