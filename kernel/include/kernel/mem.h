#pragma once

#include <stdint.h>

#define BITMAP_ADDRESS 0x10000
#define MEM_START	   0x11000
#define MEM_END		   0x7F000

#define BLOCK_SIZE	   0x1000
#define ALIGN		   8

#define GET_BIT(bit)   (bitmap[bit / 32] & ( 1 << (bit % 32))) >> (bit % 32)
#define SET_BIT(bit)   bitmap[bit / 32] |= 1 << (bit % 32)
#define CLEAR_BIT(bit) bitmap[bit / 32] &= ~(1 << (bit % 32))

uint32_t mem_alloc();
void mem_free(uint32_t address);
void init_mem();
