/*
 * device_DEVICE_MANAGER.c
 *
 *  Created on: Oct 23, 2023
 *      Author: ntdat
 */
#include "Device/bh1750.h"
#include "Device/SHT30.h"
#include "DeviceManager/device_output_manager.h"
#include "HAL/timer.h"
#include "HAL/uart.h"
#include "stdarg.h"
#include "HAL/gpio.h"

#define AFTER_READ_SENSOR_DURATION   20
#define READ_SENSOR_DURATION   		 500
#define READ_HUMI_SENSOR_DURATION	 500
#define READ_TEMP_SENSOR_DURATION	 20000
#define STABLE_DURATION			30000 //30s


#define ON				RESET
#define OFF				SET


enum {
	PUMP_WAIT_FOR_CHANGE_CONDITION,
	PUMP_ON,
	DEVICE_UNDER_REMOTE_CONTROL = 50
};

// GPIO PIN
enum{
	PUMP_PIN
};

static GPIO_info_t gpio_table[] = {
		[PUMP_PIN ] =  {GPIOA, { GPIO_PIN_2, GPIO_MODE_OUTPUT_PP, GPIO_PULLDOWN, GPIO_SPEED_FREQ_LOW}}
};

static double       setpoint[NUM_CONDITION];
static double 		threshold[MAX_LEVEL][NUM_CONDITION];
static double 		value_condition[NUM_CONDITION] = {0};
static bool     	flag_condition[NUM_CONDITION] = {0};
static bool     	flag_under_control[NUM_CONDITION] = {0};

static uint32_t 	task_id[NUM_DEVICE];
static bool			timeout[NUM_DEVICE] = {0};
static uint8_t 		counter[NUM_DEVICE] = {0};

static int DEVICE_state[NUM_DEVICE] = {PUMP_WAIT_FOR_CHANGE_CONDITION};


static void PUMP_get_humi_value();
static void PUMP_timeout();
static void PUMP_run();
static void CONTROL_pump();

static void device_under_remote_control(uint8_t device);


static void (*func_ptr[NUM_DEVICE])() = {
		[PUMP] = PUMP_get_humi_value,
};
static void publish_state(){
	char buff[60];
	int size = sprintf(buff,"PUMP STATE =%d - %.2f\n" ,DEVICE_state[PUMP],value_condition[SOIL_HUMI]);
	UART_send(UART_3, buff, size);
}
void DEVICE_MANAGER_init(){

// INIT SET POINT AND THRESHOLD
	setpoint[LUX] = 950;
	setpoint[SOIL_HUMI] = 85;
	setpoint[SOIL_TEMP] = 25;


	threshold[CEILING_LEVEL][LUX] = 1000;
	threshold[CEILING_LEVEL][SOIL_HUMI] = 90;
	threshold[CEILING_LEVEL][SOIL_TEMP] = 27;


	threshold[FLOOR_LEVEL][LUX] = 900;
	threshold[FLOOR_LEVEL][SOIL_HUMI] = 80;
	threshold[FLOOR_LEVEL][SOIL_TEMP] = 23;

//	SCH_Add_Task(publish_state, 500, 1000);

// INIT GPIO
	int nb_io = sizeof(gpio_table)/sizeof(GPIO_info_t);
	for (uint8_t var = 0; var < nb_io; ++var) {
			HAL_GPIO_Init(gpio_table[var].port, &gpio_table[var].init_info);
			HAL_GPIO_WritePin(gpio_table[var].port, gpio_table[var].init_info.Pin, OFF);
	}
}

void DEVICE_MANAGER_run(){
	CONTROL_pump();
	PUMP_run();

}
void DEVICE_MANAGER_get_data_sensor(uint8_t condition , float * value){
	if(condition >= NUM_CONDITION){
		return;
	}
	* value = (float)value_condition[condition];
}
float DEVICE_MANAGER_get_setpoint( uint8_t condition){
	return setpoint[condition];
}
void DEVICE_MANAGER_change_setpoint(uint8_t condition, float value){
	setpoint[condition] = value;
	switch (condition) {
		case LUX:
			threshold[CEILING_LEVEL][condition] = value + 50;
			threshold[FLOOR_LEVEL][condition] = (value - 50 >= 0) ? value - 50 : 0;
			break;
		case SOIL_TEMP:

			break;
		case SOIL_HUMI:
			threshold[CEILING_LEVEL][condition] = value + 5;
			threshold[FLOOR_LEVEL][condition] = (value - 5 >= 0) ? value - 5 : 0;
			DEVICE_state[PUMP] = PUMP_WAIT_FOR_CHANGE_CONDITION;
			HAL_GPIO_WritePin(gpio_table[PUMP_PIN].port, gpio_table[PUMP_PIN].init_info.Pin, SET);
			SCH_Delete_Task(task_id[PUMP]);
			task_id[PUMP] = SCH_Add_Task(func_ptr[PUMP], READ_HUMI_SENSOR_DURATION, 0);
			break;
		default:
			break;
	}
}

void DEVICE_MANAGER_set_value_condition(uint8_t condition, float value){
	value_condition[condition] = value;
	flag_condition[condition] = true;
}

void DEVICE_MANAGER_clear_under_remote_control(uint8_t device){
	if(DEVICE_state[device] != DEVICE_UNDER_REMOTE_CONTROL){
		return;
	}
	switch (device) {
			case PUMP:
				HAL_GPIO_WritePin(gpio_table[PUMP_PIN].port, gpio_table[PUMP_PIN].init_info.Pin, OFF);
				break;
			default:
				break;
		}
		flag_under_control[device] = false;
}
void DEVICE_MANAGER_under_remote_control(uint8_t device, uint8_t value){
	switch (device) {
		case PUMP:
			HAL_GPIO_WritePin(gpio_table[PUMP_PIN].port, gpio_table[PUMP_PIN].init_info.Pin, (value > 0 ? ON :OFF));
			SCH_Delete_Task(task_id[PUMP]);
			DEVICE_state[PUMP] = DEVICE_UNDER_REMOTE_CONTROL;
			SENSOR_MANAGER_clear_setup_state(SOIL_HT_SENSOR);
			break;
		default:
			break;
	}
	flag_under_control[device] = true;
}


// ------------------------------------------------------------------------------------- //
static uint8_t pre_state = 0;

static void CONTROL_pump(){
	if(DEVICE_state[PUMP] == DEVICE_UNDER_REMOTE_CONTROL){
		return;
	}
	if(flag_condition[LUX] && flag_condition[SOIL_TEMP]){
		flag_condition[LUX] = false;
		flag_condition[SOIL_TEMP] = false;
		if((value_condition[LUX] >= 1500 || value_condition[LUX] <= 150) ||
				(value_condition[SOIL_TEMP] >= 40 || value_condition[SOIL_TEMP] <= 20)){

			if(DEVICE_state[PUMP] != -1){
				DEVICE_state[PUMP] = -1;
				HAL_GPIO_WritePin(gpio_table[PUMP_PIN].port, gpio_table[PUMP_PIN].init_info.Pin, OFF);
				SENSOR_MANAGER_clear_setup_state(SOIL_HT_SENSOR);

			}
		}else{
			if(DEVICE_state[PUMP] == -1){
				DEVICE_state[PUMP] = 0;
				HAL_GPIO_WritePin(gpio_table[PUMP_PIN].port, gpio_table[PUMP_PIN].init_info.Pin, OFF);
				SENSOR_MANAGER_clear_setup_state(SOIL_HT_SENSOR);
			}
		}
	}

}
static void PUMP_run(){
	switch (DEVICE_state[PUMP]) {
		case PUMP_WAIT_FOR_CHANGE_CONDITION:
			if(flag_condition[SOIL_HUMI]){
				flag_condition[SOIL_HUMI] = false;
				if(value_condition[SOIL_HUMI] >= threshold[CEILING_LEVEL][SOIL_HUMI]){
					return;
				}
				if((value_condition[SOIL_HUMI] <=  threshold[FLOOR_LEVEL][SOIL_HUMI] && value_condition[SOIL_HUMI] != 0)){
					SENSOR_MANAGER_setup_state(SOIL_HT_SENSOR);
					task_id[PUMP] = SCH_Add_Task(func_ptr[PUMP], READ_HUMI_SENSOR_DURATION, 0);
					HAL_GPIO_WritePin(gpio_table[PUMP_PIN].port, gpio_table[PUMP_PIN].init_info.Pin, ON);
					DEVICE_state[PUMP] = PUMP_ON;
				}
			}
			break;
		case PUMP_ON:
			if(flag_condition[SOIL_HUMI]){
				flag_condition[SOIL_HUMI] = false;
				if(value_condition[SOIL_HUMI] <= setpoint[SOIL_HUMI]){


				}else if((value_condition[SOIL_HUMI] > setpoint[SOIL_HUMI])){

					SENSOR_MANAGER_clear_setup_state(SOIL_HT_SENSOR);
					HAL_GPIO_WritePin(gpio_table[PUMP_PIN].port, gpio_table[PUMP_PIN].init_info.Pin, OFF);
					DEVICE_state[PUMP] = PUMP_WAIT_FOR_CHANGE_CONDITION;
				}
			}
			break;
		case DEVICE_UNDER_REMOTE_CONTROL:
			device_under_remote_control(PUMP);
			break;
		default:
			break;
	}
}

static void device_under_remote_control(uint8_t device){
	if(!flag_under_control[device]){
		DEVICE_state[device] = PUMP_WAIT_FOR_CHANGE_CONDITION;
	}
}

// --------------------- HELPER FUNCTION ---------------------------//
static uint8_t count_read[NUM_SENSOR] = {0};
static float tempo[NUM_CONDITION][3] = {0};

static float cc_mean(float * array){
	float sum = 0;
	for(int index = 0; index < 3 ;++index){
		sum += array[index];
	}
	return (sum/3);
}


static void PUMP_set_flag_con(){
	flag_condition[SOIL_HUMI] = true;
};
static void PUMP_get_humi_value(){
	if(count_read[SOIL_HT_SENSOR] < 3){
		SHT30_Read_Temperature_And_Humidity(&SHT, Single_HighRep_ClockStretch);
		tempo[SOIL_HUMI][count_read[SOIL_HT_SENSOR]] = SHT.Humi;
		tempo[SOIL_TEMP][count_read[SOIL_HT_SENSOR]] = SHT.Temp;
		count_read[SOIL_HT_SENSOR]++;
		task_id[PUMP] = SCH_Add_Task(PUMP_get_humi_value, READ_SENSOR_DURATION, 0);
	}else{
		count_read[SOIL_HT_SENSOR] = 0;
		value_condition[SOIL_HUMI] = cc_mean(tempo[SOIL_HUMI]);
		value_condition[SOIL_TEMP] = cc_mean(tempo[SOIL_TEMP]);
		task_id[PUMP] = SCH_Add_Task(PUMP_set_flag_con, AFTER_READ_SENSOR_DURATION, 0);
	}
}
static void PUMP_timeout(){
	timeout[PUMP] =  true;
}

