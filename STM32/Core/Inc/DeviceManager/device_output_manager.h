/*
 * device_output_manager.h
 *
 *  Created on: Oct 23, 2023
 *      Author: ntdat
 */

#ifndef INC_DEVICEMANAGER_DEVICE_OUTPUT_MANAGER_H_
#define INC_DEVICEMANAGER_DEVICE_OUTPUT_MANAGER_H_

#include "main.h"
#include "stdint.h"
#include "stdbool.h"
#include "LIB/scheduler/scheduler.h"
#include "sensor_manager.h"

enum {
	LUX,
	SOIL_TEMP,
	SOIL_HUMI,
	NUM_CONDITION
};
enum{
	PUMP,
	NUM_DEVICE
};
enum {
	CEILING_LEVEL,
	FLOOR_LEVEL,
	MAX_LEVEL
};

float DEVICE_MANAGER_get_setpoint(uint8_t condition);
void DEVICE_MANAGER_get_data_sensor(uint8_t condition , float * value);
void DEVICE_MANAGER_change_setpoint(uint8_t condition, float value);
void DEVICE_MANAGER_set_value_condition(uint8_t condition, float value);
void DEVICE_MANAGER_clear_under_remote_control(uint8_t device);
void DEVICE_MANAGER_under_remote_control(uint8_t device, uint8_t value);
void DEVICE_MANAGER_init();
void DEVICE_MANAGER_run();

#endif /* INC_DEVICEMANAGER_DEVICE_OUTPUT_MANAGER_H_ */
