
#ifndef _WIFI_MODULE_
#define _WIFI_MODULE_

#include <string.h>
#include "freertos/FreeRTOS.h"
#include "freertos/event_groups.h"
#include "esp_wifi.h"
#include "esp_log.h"
#include "esp_event.h"
#include "nvs_flash.h"
#include <stdio.h>

extern const char *WIFI_TAG;
#define ESP_WIFI_SSID      "TP-Link_8780"
#define ESP_WIFI_PASS      "72951477"
// #define ESP_WIFI_SSID      "NHAN"
// #define ESP_WIFI_PASS      "password"
// #define ESP_WIFI_SSID      "GUTS99"
// #define ESP_WIFI_PASS      "12345678"
// #define ESP_WIFI_SSID      "Hehe"
// #define ESP_WIFI_PASS      "00008888"
#define ESP_MAXIMUM_RETRY  10


#define WIFI_CONNECTED_BIT BIT0
#define WIFI_FAIL_BIT      BIT1
extern int s_retry_num;



extern EventGroupHandle_t s_wifi_event_group;
void event_handler(void* arg, esp_event_base_t event_base, int32_t event_id, void* event_data);
void wifi_init_sta(void);

#endif