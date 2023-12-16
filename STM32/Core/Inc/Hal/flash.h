#ifndef FLASH_H
#define FLASH_H


#include "stdint.h"
#include "string.h"

void 	FLASH_erase(uint32_t addr,uint32_t num_bytes);
void 	FLASH_write_int(uint32_t addr, uint16_t data);
uint16_t FLASH_read_int(uint32_t addr);
void 	FLASH_write_buf(uint32_t addr, uint8_t * data , uint32_t data_len);
void 	FLASH_read_buf(uint32_t addr, uint8_t * data, uint32_t data_len);


#endif	// FLASH_H
