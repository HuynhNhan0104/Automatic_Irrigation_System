#include "main.h"
#include <Hal/flash.h>


static void FLASH_lock();
static void FLASH_unlock();

void FLASH_erase(uint32_t addr,uint32_t num_bytes)
{

}

void FLASH_write_int(uint32_t addr, uint16_t data)
{

}

uint16_t FLASH_read_int(uint32_t addr)
{
	uint16_t* val = (uint16_t *)addr;
	return *val;
}

void FLASH_write_buf(uint32_t addr, uint8_t* data, uint32_t data_len)
{

}

void FLASH_read_buf(uint32_t addr1 , uint8_t* data, uint32_t data_len)
{
	uint16_t temp_data;
	for (int var = 0; var < data_len; var+=2) {
		temp_data = FLASH_read_int(addr1 + var);
		data[var+1] = (temp_data&0xFF00)>> 8;
		data[var] = temp_data;
	}
	return;
}

static void FLASH_lock()
{
	HAL_FLASH_Lock();
}

static void FLASH_unlock()
{
	HAL_FLASH_Unlock();
}
