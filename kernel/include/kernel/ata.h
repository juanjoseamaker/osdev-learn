#pragma once

#include <stdint.h>

void read_sectors_ATA_PIO(uint32_t target_address, uint32_t LBA, uint8_t sector_count);
void write_sectors_ATA_PIO(uint32_t LBA, uint8_t sector_count, uint16_t* bytes);
void init_ata();
