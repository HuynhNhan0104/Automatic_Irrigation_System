

#include "main.h"
#include "Hal/uart.h"
#include "Lib/utils/utils_buffer.h"

#define TX_TIMEOUT		0xFFFF

typedef struct {
	UART_HandleTypeDef * huart_p;
	utils_buffer_t * buffer;
	uint16_t temp_data;
}UART_info_t;


UART_HandleTypeDef huart1 = {
	.Instance = USART1,
	.Init.BaudRate = 115200,
	.Init.WordLength = UART_WORDLENGTH_8B,
	.Init.StopBits = UART_STOPBITS_1,
	.Init.Parity = UART_PARITY_NONE,
	.Init.Mode = UART_MODE_TX_RX,
	.Init.HwFlowCtl = UART_HWCONTROL_NONE,
	.Init.OverSampling = UART_OVERSAMPLING_16
};

UART_HandleTypeDef huart2 = {
	.Instance = USART2,
	.Init.BaudRate = 9600,
	.Init.WordLength = UART_WORDLENGTH_9B,
	.Init.StopBits = UART_STOPBITS_1,
	.Init.Parity = UART_PARITY_NONE,
	.Init.Mode = UART_MODE_TX_RX,
	.Init.HwFlowCtl = UART_HWCONTROL_NONE,
	.Init.OverSampling = UART_OVERSAMPLING_16
};

UART_HandleTypeDef huart3 = {
	.Instance = USART3,
	.Init.BaudRate = 115200,
	.Init.WordLength = UART_WORDLENGTH_8B,
	.Init.StopBits = UART_STOPBITS_1,
	.Init.Parity = UART_PARITY_NONE,
	.Init.Mode = UART_MODE_TX_RX,
	.Init.HwFlowCtl = UART_HWCONTROL_NONE,
	.Init.OverSampling = UART_OVERSAMPLING_16
};



static utils_buffer_t uart_buffer[UART_MAX];

static UART_info_t uart_table[UART_MAX] = {
		[UART_1] = {
			.huart_p = &huart1,
			.buffer = &uart_buffer[UART_1]
		},
		[UART_2] = {
			.huart_p = &huart2,
			.buffer = &uart_buffer[UART_2]
		},
		[UART_3] = {
			.huart_p = &huart3,
			.buffer = &uart_buffer[UART_3]
		},

};


bool UART_init(){
	bool success = true;
	// Init hal
	success = (HAL_UART_Init(uart_table[UART_1].huart_p) == HAL_OK) && success;
	success = (HAL_UART_Init(uart_table[UART_2].huart_p) == HAL_OK) && success;
	success = (HAL_UART_Init(uart_table[UART_3].huart_p) == HAL_OK) && success;

	// Init buffer
	success = utils_buffer_init(uart_table[UART_1].buffer, sizeof(uint8_t)) && success;
	success = utils_buffer_init(uart_table[UART_2].buffer, sizeof(uint16_t)) && success;
	success = utils_buffer_init(uart_table[UART_3].buffer, sizeof(uint8_t)) && success;


	HAL_UART_Receive_IT(uart_table[UART_1].huart_p, &uart_table[UART_1].temp_data, 1);
	HAL_UART_Receive_IT(uart_table[UART_2].huart_p, &uart_table[UART_2].temp_data, 1);
	HAL_UART_Receive_IT(uart_table[UART_3].huart_p, &uart_table[UART_3].temp_data, 1);

	return success;
}
bool UART_send(UART_id_t id, uint8_t *data , size_t len){
	HAL_UART_Transmit(uart_table[id].huart_p, data, len, 0xFFFF);
}
bool UART_receive_available(UART_id_t id){
	return utils_buffer_is_available(uart_table[id].buffer);
}

uint8_t UART_receive_data(UART_id_t id){
	uint8_t data;
	utils_buffer_pop(uart_table[id].buffer, &data);
	return data;
}

void UART_clear_buffer(UART_id_t id){
	utils_buffer_drop_all(uart_table[id].buffer);
}

void HAL_UART_RxCpltCallback(UART_HandleTypeDef * huart){
	if(huart->Instance == uart_table[UART_1].huart_p->Instance){
		utils_buffer_push(uart_table[UART_1].buffer, &uart_table[UART_1].temp_data);
		HAL_UART_Receive_IT(uart_table[UART_1].huart_p, &uart_table[UART_1].temp_data, 1);
	}else if(huart->Instance == uart_table[UART_2].huart_p->Instance){
		utils_buffer_push(uart_table[UART_2].buffer, &uart_table[UART_2].temp_data);
		HAL_UART_Receive_IT(uart_table[UART_2].huart_p, &uart_table[UART_2].temp_data, 1);
	}else if(huart->Instance == uart_table[UART_3].huart_p->Instance){
		utils_buffer_push(uart_table[UART_3].buffer, &uart_table[UART_3].temp_data);
		HAL_UART_Receive_IT(uart_table[UART_3].huart_p, &uart_table[UART_3].temp_data, 1);
	}
}
