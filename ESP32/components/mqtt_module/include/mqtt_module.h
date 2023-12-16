#ifndef _MQTT_MODULE_
#define _MQTT_MODULE_


#include <stdio.h>
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include "esp_wifi.h"
#include "esp_system.h"
#include "nvs_flash.h"
#include "esp_event.h"
#include "esp_netif.h"
// #include "protocol_examples_common.h"

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/semphr.h"
#include "freertos/queue.h"
#include "freertos/event_groups.h"


#include "lwip/sockets.h"
#include "lwip/dns.h"
#include "lwip/netdb.h"

#include "esp_log.h"
#include "mqtt_client.h"


#define MQTT_HOST "mqtt://demo.thingsboard.io"
#define MQTT_PORT 1883
#define ACCESS_TOKEN "GnwwzmkVeRMDT2a9u8qO"

// #define MQTT_HOST "mqtt://192.168.103.221"
// #define MQTT_PORT 1883
// #define ACCESS_TOKEN "bhcXkeXw6nmMWG23SqDc"


extern void (*on_connect)();
extern void (*on_disconnect)(void*);

extern void (*on_subscribe)(void*);
extern void (*on_unsubscribe)(void*);

extern void (*on_publish)(void*);
extern void (*on_message)(char*, char* );
extern void (*on_error)(void*);

extern esp_mqtt_client_handle_t client;
extern const char *MQTT_TAG;


void mqtt_event_handler(void *handler_args, esp_event_base_t base, int32_t event_id, void *event_data);
void mqtt_app_start(void);

int publishMessage(char* message);
#endif