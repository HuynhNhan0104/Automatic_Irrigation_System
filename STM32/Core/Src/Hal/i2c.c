

#include "main.h"
#include "Hal/i2c.h"



I2C_HandleTypeDef hi2c1;

void I2C_init(){
	  hi2c1.Instance = I2C1;
	  hi2c1.Init.Timing = 0x00303D5B;
	  hi2c1.Init.OwnAddress1 = 0;
	  hi2c1.Init.AddressingMode = I2C_ADDRESSINGMODE_7BIT;
	  hi2c1.Init.DualAddressMode = I2C_DUALADDRESS_DISABLE;
	  hi2c1.Init.OwnAddress2 = 0;
	  hi2c1.Init.OwnAddress2Masks = I2C_OA2_NOMASK;
	  hi2c1.Init.GeneralCallMode = I2C_GENERALCALL_DISABLE;
	  hi2c1.Init.NoStretchMode = I2C_NOSTRETCH_DISABLE;

	if (HAL_I2C_Init(&hi2c1) != HAL_OK)
	{
	Error_Handler();
	}
}

bool I2C_write(uint8_t address, uint8_t * data_w, size_t w_len){
	// Write
	bool success = HAL_I2C_Master_Transmit(&hi2c1, address, data_w, w_len, I2C_TIMEOUT) == HAL_OK;
	return success;
}

bool I2C_read(uint8_t address, uint8_t * data_r, size_t r_len){
	// Read
	bool success = HAL_I2C_Master_Receive(&hi2c1, address, data_r, r_len, I2C_TIMEOUT) == HAL_OK;
	return success;
}

bool I2C_write_and_read(uint8_t address, uint8_t * data_w, size_t w_len, uint8_t * data_r, size_t r_len){
	// Write
	bool success = I2C_write(address, data_w, w_len);
	// Read
	success = I2C_read(address, data_r, r_len) && success;
	return success;
}


bool I2C_mem_write(uint8_t address, uint16_t mem_address, uint16_t mem_size, uint8_t * data_w, size_t w_len){
	// Write
	bool success = HAL_I2C_Mem_Write(&hi2c1, address, mem_address, mem_size, data_w, w_len, I2C_TIMEOUT);
	return success;
}

bool I2C_mem_read(uint8_t address, uint16_t mem_address, uint16_t mem_size, uint8_t * data_w, size_t w_len){
	// Write
	bool success = HAL_I2C_Mem_Read(&hi2c1, address, mem_address, mem_size, data_w, w_len, I2C_TIMEOUT);
	return success;
}



