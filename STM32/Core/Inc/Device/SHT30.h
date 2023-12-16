/*
 * SHT30.h
 *
 *  Created on: Sep 28, 2023
 *      Author: NHAN
 */

#ifndef INC_SHT30_H_
#define INC_SHT30_H_

#include "main.h"



//Commands for Single Shot Data Acquisition Mode
#define Single_HighRep_ClockStretch 							0x2C06
#define Single_MediumRep_ClockStretch						0x2C0D
#define Single_SlowRep_ClockStretch							0x2C10
#define Single_HighRep_NoClockStretch						0x2400
#define Single_MediumRep_NoClockStretch					0x240B
#define Single_SlowRep_NoClockStretch						0x2416




//Commands for Periodic Data Acquisition Mode

// 0.5 measuare per second
#define Periodic_HighRep_0_5mps									0x2032
#define Periodic_MediumRep_0_5mps							0x2024
#define Periodic_SlowRep_0_5mps									0x202F

// 1 measure per second
#define Periodic_HighRep_1mps									0x2130
#define Periodic_MediumRep_1mps								0x2126
#define Periodic_SlowRep_1mps									0x212D

// 2 measure per second
#define Periodic_HighRep_2mps									0x2236
#define Periodic_MediumRep_2mps								0x2220
#define Periodic_SlowRep_2mps									0x222B

// 4 measure per second
#define Periodic_HighRep_4mps									0x2334
#define Periodic_MediumRep_4mps								0x2323
#define Periodic_SlowRep_4mps									0x2329


// 10 measure per second
#define Periodic_HighRep_10mps									0x2737
#define Periodic_MediumRep_10mps							0x2721
#define Periodic_SlowRep_10mps									0x272A

#define SHT30_ADDRESS_ADDR_PIN_LOW 0x44
#define SHT30_ADDRESS_ADDR_PIN_HIGH 0x45

#define Fetch_Data														0xE000
#define Accelerated_Response_Time 							0x2B32

#define Break_Command 											0x3093

#define Soft_Reset														0x30A2
#define Reset_Through_General_Call							0x0006

#define Heater_Enable													0x306D
#define Heater_Disable													0x3066
#define Readout_Status_Register									0xF32D
#define Clear_Status_Register										0x3041

#define SHT30_I2C_Timeout											30

 typedef struct {
	 I2C_HandleTypeDef* hi2c;
	 uint8_t Address;
	 float Temp;
	 float Humi;
 } SHT30_HandleTypeDef;



 void SHT30_Init(SHT30_HandleTypeDef* SHT,  I2C_HandleTypeDef* hi2c, uint8_t Address);
uint8_t SHT30_Read_Temperature_And_Humidity(SHT30_HandleTypeDef * SHT,uint16_t mode);
uint8_t SHT30_Heater_Set_Enable(SHT30_HandleTypeDef * SHT, uint8_t enable);
uint8_t SHT30_Read_Temperature_And_Humidity_v2(SHT30_HandleTypeDef * SHT,uint16_t mode , float * temp, float * humi);

#endif /* INC_SHT30_H_ */
