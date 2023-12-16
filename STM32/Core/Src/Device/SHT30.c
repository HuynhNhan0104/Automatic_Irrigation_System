/*
 * SHT30.c
 *
 *  Created on: Sep 28, 2023
 *      Author: NHAN
 */

#include "Device/SHT30.h"

static uint8_t calculate_crc(const uint8_t *data, size_t length)
{
	uint8_t crc = 0xff;
	for (size_t i = 0; i < length; i++) {
		crc ^= data[i];
		for (size_t j = 0; j < 8; j++) {
			if ((crc & 0x80u) != 0) {
				crc = (uint8_t)((uint8_t)(crc << 1u) ^ 0x31u);
			} else {
				crc <<= 1u;
			}
		}
	}
	return crc;
}
void SHT30_Init(SHT30_HandleTypeDef* SHT,  I2C_HandleTypeDef* hi2c, uint8_t Address){
	 SHT->hi2c = hi2c;
	 SHT->Address = Address;
	 SHT->Temp = 0;
	 SHT->Humi = 0;
}
static uint8_t SHT30_Send_Command(SHT30_HandleTypeDef* SHT, uint16_t command){
	uint8_t command_buffer[2] = {(command & 0xff00u) >> 8u, command & 0xffu};

		if (HAL_I2C_Master_Transmit(SHT->hi2c, SHT->Address << 1u, command_buffer, sizeof(command_buffer),
		                            SHT30_I2C_Timeout) != HAL_OK) {
			return 0;
		}

		return 1;
}
uint8_t SHT30_Heater_Set_Enable(SHT30_HandleTypeDef * SHT, uint8_t enable){
	if(enable){
		return   SHT30_Send_Command(SHT,Heater_Enable);
	}
	else {
		return   SHT30_Send_Command(SHT,Heater_Disable);
	}
}


uint8_t SHT30_Read_Temperature_And_Humidity(SHT30_HandleTypeDef * SHT, uint16_t mode){
	 SHT30_Send_Command( SHT, mode);
	 HAL_Delay(1);
	 uint8_t buffer[6];
	if (HAL_I2C_Master_Receive(SHT->hi2c, SHT->Address << 1, buffer, sizeof(buffer), SHT30_I2C_Timeout) != HAL_OK) {
		return 0;
	}

	uint8_t temperature_crc = calculate_crc(buffer, 2);
	uint8_t humidity_crc = calculate_crc(buffer + 3, 2);
	if (temperature_crc != buffer[2] || humidity_crc != buffer[5]) {
		return 0;
	}
	uint16_t temperature_raw = (buffer[0]<< 8) | buffer[1] ;
	uint16_t humidity_raw = ( buffer[3] << 8) | buffer[4];

	SHT->Temp = -45.0f + 175.0f * (float)temperature_raw / 65535.0f;
	SHT->Humi  = 100.0f * (float)humidity_raw / 65535.0f;
	return 1;
}

uint8_t SHT30_Read_Temperature_And_Humidity_v2(SHT30_HandleTypeDef * SHT,uint16_t mode , float * temp, float * humi){
	SHT30_Send_Command( SHT, mode);
	HAL_Delay(1);
	uint8_t buffer[6];
	if (HAL_I2C_Master_Receive(SHT->hi2c, SHT->Address << 1, buffer, sizeof(buffer), SHT30_I2C_Timeout) != HAL_OK) {
		return 0;
	}

	uint8_t temperature_crc = calculate_crc(buffer, 2);
	uint8_t humidity_crc = calculate_crc(buffer + 3, 2);
	if (temperature_crc != buffer[2] || humidity_crc != buffer[5]) {
		return 0;
	}
	uint16_t temperature_raw = (buffer[0]<< 8) | buffer[1] ;
	uint16_t humidity_raw = ( buffer[3] << 8) | buffer[4];

	* temp  = -45.0f + 175.0f * (float)temperature_raw / 65535.0f;
	* humi  = 100.0f * (float)humidity_raw / 65535.0f;
	return 1;
}


