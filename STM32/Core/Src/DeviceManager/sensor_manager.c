/*
 * sensor_manager.c
 *
 *  Created on: Oct 15, 2023
 *      Author: ntdat
 */

#include "DeviceManager/sensor_manager.h"
#include "Device/lcd_i2c.h"
#include "stdbool.h"
#include "Hal/uart.h"
#include "Hal/i2c.h"
#include "Hal/timer.h"
#include "math.h"

#define BH1750_DURATION    			500   // 2 minute
#define SHT30_DURATION				1000
#define READ_SENSOR_DURATION  		500
#define READ_SHT30_DURATION  		500
#define CAPACITY					0.1



// temp

LCD_I2C_DEVICE LCD;

// -------------------
enum {
	SENSOR_BEGIN,
	SENSOR_WAIT_FOR_READ,
	SENSOR_READ_DATA,
	SENSOR_SETUP_CONDITION
};
enum{
	TEMP_INDEX,
	HUMI_INDEX
};

static uint8_t BH1750_state = SENSOR_BEGIN;
static bool timeout_for_BH1750 = false;
static uint32_t timeout_BH1750_task_id;
static uint8_t Lux_index = 0;
static float Lux_value_temp[10] = {0};




static uint8_t SHT30_state = SENSOR_BEGIN;
static bool timeout_for_SHT30 = false;
static uint32_t timeout_SHT30_task_id;
static uint8_t SHT_index = 0;
static float SHT_value_temp[2][10] = {
		[TEMP_INDEX] = {0},
		[HUMI_INDEX] = {0}
};


SHT30_HandleTypeDef SHT ;

static bool flag_setup[NUM_SENSOR] = {0};

static char buffer[100];
static uint8_t size;

static void Serial_send_data(uint8_t *data , size_t len);

static float Calculate_mean(float *Array, uint8_t size);
static float Calculate_variance(float *Array, uint8_t size, float mean);

static void BH1750_timeout();
static void SHT30_timeout();

static void BH1750_run();
static void SHT30_run();

static void RP_state(){
	  size = sprintf(buffer, "BH1750 STATE: %d --SHT30 STATE: %d\r\n"
			  	  	  	   , BH1750_state,SHT30_state);
	  UART_send(UART_3, buffer, size);
}
void SENSOR_MANAGER_setup_state(uint8_t sensor){
	if(sensor < 0 && sensor >= NUM_SENSOR){
		return;
	}
	if(sensor == LUX_SENSOR){
		SCH_Delete_Task(timeout_BH1750_task_id);
		timeout_BH1750_task_id = SCH_Add_Task(BH1750_timeout, 3102, 0);
		BH1750_SetMode(CONTINUOUS_HIGH_RES_MODE_2);
		BH1750_state = SENSOR_SETUP_CONDITION;
	}else if(sensor == SOIL_HT_SENSOR){
		SCH_Delete_Task(timeout_SHT30_task_id);
		timeout_SHT30_task_id = SCH_Add_Task(SHT30_timeout, 3100, 0);
		SHT30_state = SENSOR_SETUP_CONDITION;
	}
	flag_setup[sensor] = true;
}
void SENSOR_MANAGER_clear_setup_state(uint8_t sensor){
	if(sensor < 0 || sensor >= NUM_SENSOR){
		return;
	}
	flag_setup[sensor] = false;
}

static float temperature = 0;
static float humidity = 0;
static float lux = 0;
static char line1[50];
static char line2[50];
static void Display_lcd(){
	LCD_I2C_Clear(&LCD);
	sprintf(line1,"T:%.1f%cC L:%.1f",temperature,0xDF, lux);
	sprintf(line2,"H:%.1f%%",humidity);
	LCD_I2C_SetCursor(&LCD, 0, 0);
	LCD_I2C_WriteString(&LCD, line1);
	LCD_I2C_SetCursor(&LCD, 0, 1);
	LCD_I2C_WriteString(&LCD, line2);
}

void SENSOR_MANAGER_init(){
	BH1750_Init(&hi2c1);
	BH1750_PowerState(BH1750_POWER_DOWN);
	SHT30_Init(&SHT, &hi2c1, SHT30_ADDRESS_ADDR_PIN_LOW );
//	SCH_Add_Task(RP_state, 1000, 1000);
	LCD_I2C_Init(&LCD, &hi2c1,0x4E, 16, 2);
	LCD_I2C_SetCursor(&LCD, 0, 0);
	LCD_I2C_WriteString(&LCD, "SYSTEM BOOSTING ...");
}

void SENSOR_MANAGER_run(){
	BH1750_run();
	SHT30_run();
}


static void BH1750_run(){
	switch (BH1750_state) {
		case SENSOR_BEGIN:
			timeout_BH1750_task_id = SCH_Add_Task(BH1750_timeout, 735, 0);
			BH1750_state = SENSOR_WAIT_FOR_READ;
			break;
		case SENSOR_WAIT_FOR_READ:
			if(timeout_for_BH1750){
				timeout_for_BH1750 = false;
				timeout_BH1750_task_id = SCH_Add_Task(BH1750_timeout, READ_SENSOR_DURATION , 0);
				BH1750_SetMode(CONTINUOUS_HIGH_RES_MODE_2);
				Lux_index = 0;
				BH1750_state = SENSOR_READ_DATA;
			}
			break;
		case SENSOR_READ_DATA:
			if(timeout_for_BH1750){
				timeout_for_BH1750 = false;
				if(Lux_index < 3){
					BH1750_ReadLight(&Lux_value_temp[Lux_index++]);
					timeout_BH1750_task_id = SCH_Add_Task(BH1750_timeout, READ_SENSOR_DURATION , 0);
				}else{
					float mean = Calculate_mean(Lux_value_temp, 3);
					float var = Calculate_variance(Lux_value_temp, 3, mean);
					if((float)sqrt(var) < (mean*CAPACITY)){
//						  size = sprintf(buffer, "!LUX:%.2f#\r\n", mean);
						  size = sprintf(buffer,"{\'lux\':%.2f}\r\n",mean);
						  DEVICE_MANAGER_set_value_condition(LUX,mean);
						  Serial_send_data(buffer,size);
					}
					lux = mean;

					BH1750_PowerState(BH1750_POWER_DOWN);
					BH1750_state = SENSOR_BEGIN;
				}
			}
			break;
		case SENSOR_SETUP_CONDITION:
			if(!flag_setup[LUX_SENSOR]){
				BH1750_PowerState(BH1750_POWER_DOWN);
				BH1750_state = SENSOR_BEGIN;
			}
			break;
		default:
			break;
	}
}


static void SHT30_run(){
	switch (SHT30_state) {
			case SENSOR_BEGIN:
				timeout_SHT30_task_id = SCH_Add_Task(SHT30_timeout, SHT30_DURATION, 0);
				SHT30_state = SENSOR_WAIT_FOR_READ;
				break;
			case SENSOR_WAIT_FOR_READ:
				if(timeout_for_SHT30){
					timeout_for_SHT30 = false;
					timeout_SHT30_task_id = SCH_Add_Task(SHT30_timeout, READ_SENSOR_DURATION , 0);
//					SHT30_Heater_Set_Enable(&SHT, true);
					SHT_index = 0;
					SHT30_state = SENSOR_READ_DATA;
				}
				break;
			case SENSOR_READ_DATA:
				if(timeout_for_SHT30){
					timeout_for_SHT30 = false;
					if(SHT_index < 10){
						SHT30_Read_Temperature_And_Humidity(&SHT, Single_HighRep_ClockStretch);
						SHT_value_temp[TEMP_INDEX][SHT_index] = SHT.Temp;
						SHT_value_temp[HUMI_INDEX][SHT_index] = SHT.Humi;
						SHT_index++;
						timeout_SHT30_task_id = SCH_Add_Task(SHT30_timeout, READ_SENSOR_DURATION , 0);
					}else{
//						SHT30_Heater_Set_Enable(&SHT, false);
						float mean_temp = Calculate_mean(SHT_value_temp[TEMP_INDEX], 10);
						float var_temp = Calculate_variance(SHT_value_temp[TEMP_INDEX], 10, mean_temp);
						if((float)sqrt(var_temp) < (mean_temp*CAPACITY)){
//							  size = sprintf(buffer, "!SOILTEMP:%.2f#\r\n", mean_temp);
//							  size = sprintf(buffer, "{\'temperature\':%.2f}\r\n", mean_temp);
//							  Serial_send_data(buffer,size);
							  DEVICE_MANAGER_set_value_condition(SOIL_TEMP,mean_temp);
						}


						float mean_humi = Calculate_mean(SHT_value_temp[HUMI_INDEX], 10);
						float var_humi = Calculate_variance(SHT_value_temp[HUMI_INDEX], 10, mean_humi);
						if((float)sqrt(var_humi) < (mean_humi*CAPACITY)){
//							 size = sprintf(buffer, "!SOILHUMI:%.2f#\r\n", mean_humi);
							 size = sprintf(buffer, "{\'humidity\':%.2f,\'temperature\':%.2f}\r\n", mean_humi,mean_temp);
							 Serial_send_data(buffer,size);
							 DEVICE_MANAGER_set_value_condition(SOIL_HUMI,mean_humi);
							 DEVICE_MANAGER_set_value_condition(SOIL_TEMP,mean_temp);
						}

						temperature = mean_temp;
						humidity = mean_humi;


						SHT30_state = SENSOR_BEGIN;
					}
				}
				break;
			case SENSOR_SETUP_CONDITION:
				if(!flag_setup[SOIL_HT_SENSOR]){
					SHT30_state = SENSOR_BEGIN;
					SCH_Delete_Task(timeout_SHT30_task_id);
				}
				if(timeout_for_SHT30){
					timeout_for_SHT30 = false;
					if(SHT_index < 3){
						SHT30_Read_Temperature_And_Humidity(&SHT, Single_HighRep_ClockStretch);
						SHT_value_temp[TEMP_INDEX][SHT_index] = SHT.Temp;
						SHT_value_temp[HUMI_INDEX][SHT_index] = SHT.Humi;
						SHT_index++;
						timeout_SHT30_task_id = SCH_Add_Task(SHT30_timeout, READ_SENSOR_DURATION + 33 , 0);
					}else{
//						SHT30_Heater_Set_Enable(&SHT, false);
						SHT_index = 0;
						timeout_SHT30_task_id = SCH_Add_Task(SHT30_timeout, READ_SENSOR_DURATION + 33, 0);
						float mean_temp = Calculate_mean(SHT_value_temp[TEMP_INDEX], 3);
						float var_temp = Calculate_variance(SHT_value_temp[TEMP_INDEX], 3, mean_temp);
						if((float)sqrt(var_temp) < (mean_temp*1)){
//							  Serial_send_data(buffer,size);
							  DEVICE_MANAGER_set_value_condition(SOIL_TEMP,mean_temp);
						}


						float mean_humi = Calculate_mean(SHT_value_temp[HUMI_INDEX], 3);
						float var_humi = Calculate_variance(SHT_value_temp[HUMI_INDEX], 3, mean_humi);
						if((float)sqrt(var_humi) < (mean_humi*1)){
							 size = sprintf(buffer, "{\'humidity\':%.2f,\'temperature\':%.2f}\r\n", mean_humi,mean_temp);
							 Serial_send_data(buffer,size);
							 DEVICE_MANAGER_set_value_condition(SOIL_HUMI,mean_humi);
						}

						temperature = mean_temp;
						humidity = mean_humi;
					}
				}
				break;
			default:
				break;
		}
}

static void BH1750_timeout(){
	timeout_for_BH1750 = true;
}


static void SHT30_timeout(){
	timeout_for_SHT30 = true;
}

static void Serial_send_data(uint8_t *data , size_t len){
	char buff[20];
//	uint64_t time  = HAL_GetTick();
//	uint8_t sz = sprintf(buff, "TIME :%d ", time);

//	UART_send(UART_3,buff,sz);
	UART_send(UART_3,data,len);
	Display_lcd();
}

static float Calculate_mean(float *Array, uint8_t size){
	float sum = 0;
	for(uint8_t index = 0 ; index < size ; ++index){
		sum += Array[index];
	}
	return (sum/size);
}

static float Calculate_variance(float *Array, uint8_t size, float mean){
	float squared_diff = 0;
	for(uint8_t index = 0 ; index < size ; ++index){
		squared_diff += (Array[index] - mean)*(Array[index] - mean);
	}
	return (squared_diff/size);
}
