/*
 * get_command.c
 *
 *  Created on: Nov 5, 2023
 *      Author: ntdat
 */
#include "App/get_command.h"
#include "Hal/uart.h"
#include "Lib/scheduler/scheduler.h"
#include "Lib/utils/utils_string.h"
#include "stdbool.h"
#include "stdint.h"
#include "DeviceManager/device_output_manager.h"

#define UART_PORT    		UART_3
#define TIMEOUT_DURATION 	1000
#define MAX_SIZE_BUFF 		10

typedef struct {
	uint8_t sign;
	uint16_t name;
}command_info;
enum{
	COMMAND_ER,
	COMMAND_OK
};

enum{
	WAIT_FOR_COMMAND,
	GET_COMMAND,
	CONSTRUE_COMMAND
};

static command_info table_command_device[] = {
		[0] = {
				.sign = 'P',
				.name = PUMP
		}
};
static command_info table_command_condition[] = {
		[0] = {
				.sign = 'L',
				.name = LUX
		},
		[1] = {
				.sign = 'T',
				.name = SOIL_TEMP
		},
		[2] = {
				.sign = 'H',
				.name = SOIL_HUMI
		},
};
static uint8_t state = WAIT_FOR_COMMAND;
static uint8_t buffer[MAX_SIZE_BUFF];
static uint8_t length = 0;
static bool timeout = false;
static uint32_t task_id;
static uint16_t value;
static int i = 0;

static void clear_buffer();
static void timeout_fn();
static void refresh_timeout();
static void respone_command(uint8_t mess);

void SERIAL_get_command_run(){
	switch (state) {
		case WAIT_FOR_COMMAND:
			if(UART_receive_available(UART_PORT)){
				if(UART_receive_data(UART_PORT) == '!'){
					buffer[0] = '!';
					length = 1;
					task_id = SCH_Add_Task(timeout_fn, TIMEOUT_DURATION, 0);
					state =  GET_COMMAND;
				}

			}
			break;
		case GET_COMMAND:
			if(timeout){
				timeout = false;
				clear_buffer();
				state =  WAIT_FOR_COMMAND;
			}
			if(length >= MAX_SIZE_BUFF){
				clear_buffer();
				SCH_Delete_Task(task_id);
				state =  WAIT_FOR_COMMAND;
			}
			if(UART_receive_available(UART_PORT)){
				refresh_timeout();
				buffer[length] = UART_receive_data(UART_PORT);
				if(buffer[length] == '#'){
					SCH_Delete_Task(task_id);
					state =  CONSTRUE_COMMAND;
				}
				length++;
			}
			break;
		case CONSTRUE_COMMAND:;
			switch (buffer[1]) {
				case 'D':
					for(i = 0; i <= 0; ++i){
						if(buffer[2] == table_command_device[i].sign){
							if(buffer[3] == 'X'){
								DEVICE_MANAGER_clear_under_remote_control(table_command_device[i].name);
								respone_command(COMMAND_OK);
							}else{
								value = utils_string_to_int(&buffer[3], length - 4);
								DEVICE_MANAGER_under_remote_control(table_command_device[i].name, value);
								respone_command(COMMAND_OK);
							}
							break;
						}

					}

					if( i == 5 ){
						respone_command(COMMAND_ER);
					}
					break;
				case 'S':

					for(i = 0; i <= 2; ++i){
						if(buffer[2] == table_command_condition[i].sign){
							value = utils_string_to_int(&buffer[3], length - 4);
							DEVICE_MANAGER_change_setpoint(table_command_condition[i].name, value);
							respone_command(COMMAND_OK);
							break;
						}
					}

					if( i == 6 ){
						respone_command(COMMAND_ER);
					}
					break;
				default:
					respone_command(COMMAND_ER);
					break;
			}
			clear_buffer();
			state =  WAIT_FOR_COMMAND;
			break;
		default:
			break;
	}
}

static void clear_buffer(){
	length = 0;
}
static void timeout_fn(){
	timeout = true;
}
static void refresh_timeout(){
	SCH_Delete_Task(task_id);
	task_id = SCH_Add_Task(timeout_fn, TIMEOUT_DURATION, 0);
}
static void respone_command(uint8_t mess){
//	if(mess == COMMAND_ER){
//		UART_send(UART_PORT, buffer, length);
//		UART_send(UART_PORT, " ER\r\n", sizeof(" ER\r\n"));
//	}else if(mess =  COMMAND_OK){
//		UART_send(UART_PORT, buffer, length);
//		UART_send(UART_PORT, " OK\r\n", sizeof(" OK\r\n"));
//	}
	clear_buffer();
}
