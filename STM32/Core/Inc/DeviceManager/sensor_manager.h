/*
 * sensor_manager.h
 *
 *  Created on: Oct 15, 2023
 *      Author: ntdat
 */

#ifndef INC_DEVICEMANAGER_SENSOR_MANAGER_H_
#define INC_DEVICEMANAGER_SENSOR_MANAGER_H_

#include "Lib/scheduler/scheduler.h"
#include "device_output_manager.h"
#include "Device/bh1750.h"
#include "Device/SHT30.h"

extern SHT30_HandleTypeDef SHT ;

enum{
	LUX_SENSOR,
	UV_SENSOR,
	AIR_HT_SENSOR,
	SOIL_HT_SENSOR,
	NUM_SENSOR
};
void SENSOR_MANAGER_setup_state(uint8_t sensor);
void SENSOR_MANAGER_clear_setup_state(uint8_t sensor);
void SENSOR_MANAGER_init();
void SENSOR_MANAGER_run();

#endif /* INC_DEVICEMANAGER_SENSOR_MANAGER_H_ */
