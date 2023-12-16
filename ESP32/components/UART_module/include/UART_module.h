#include <stdio.h>
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include "driver/gpio.h"
#include "driver/uart.h"

// #define TX_PIN GPIO_NUM_17
// #define RX_PIN GPIO_NUM_16
extern const uart_port_t uart_num;
extern const int RX_BUF_SIZE;
#define TXD_PIN (GPIO_NUM_4)
#define RXD_PIN (GPIO_NUM_5)

// void tx_task(void *arg);
// void rx_task(void *arg);
int sendData(const char* logName, const char* data);
void uart_init();