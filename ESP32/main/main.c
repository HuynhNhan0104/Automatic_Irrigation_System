#include <string.h>
#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "nvs_flash.h"


#include "time.h"
#include <esp_event.h>
#include <driver/rtc_io.h>
#include <esp_timer.h>
#include <driver/gpio.h>
#include <stdio.h>
#include <freertos/FreeRTOS.h>
#include <freertos/task.h>
#include <freertos/semphr.h>
#include <driver/gpio.h>
#include <esp_log.h>
#include <esp_timer.h>
#include <sys/time.h>

#include "wifi_module.h"
#include "NTP_module.h"
#include "mqtt_module.h"
#include "UART_module.h"

// #define PUMP_PIN GPIO_NUM_2 // Replace with the GPIO pin connected to the pump relay
void process_data(uint8_t * data){
    int msg_id = 0;
    printf("%s \n" , (char*) data);
    msg_id=publishMessage((char*) data);
    ESP_LOGI(MQTT_TAG, "sent publish successful, msg_id=%d", msg_id);
}

void rx_task(void *arg)
{
    static const char *RX_TASK_TAG = "RX_TASK";
    esp_log_level_set(RX_TASK_TAG, ESP_LOG_INFO);
    uint8_t* data = (uint8_t*) malloc(RX_BUF_SIZE+1);
    while (1) {
        const int rxBytes = uart_read_bytes(UART_NUM_1, data, RX_BUF_SIZE, 1000 / portTICK_PERIOD_MS);
        if (rxBytes > 0) {
            data[rxBytes] = 0;
            ESP_LOGI(RX_TASK_TAG, "Read %d bytes: '%s'", rxBytes, data);
            ESP_LOG_BUFFER_HEXDUMP(RX_TASK_TAG, data, rxBytes, ESP_LOG_INFO);
            process_data(data);
        }
    }
    free(data);
}


// on_connect = (void *)on_connected;


void Node_init(){
    char command[64];
    strcpy(command,"!SH90#");
    sendData("uart_connected",(char*)command);
}

SemaphoreHandle_t xMutex; // Mutex to protect critical sections
esp_timer_handle_t timer;

void turnOnPump() {
    char command[64];

    strcpy(command,"!DPX#");
    sendData("uart_connected",(char*)command);

    strcpy(command,"!DP1#");
    sendData("uart_connected",(char*)command);

    vTaskDelay(pdMS_TO_TICKS(10000));

    strcpy(command,"!DP0#");
    sendData("uart_connected",(char*)command);

    strcpy(command,"!DPX#");
    sendData("uart_connected",(char*)command);

}

void timer_callback(void* arg) {
    // xSemaphoreTake(xMutex, portMAX_DELAY);

    // Execute the task that needs to be performed at 6:00 AM
    turnOnPump();

    // xSemaphoreGive(xMutex);
}

void timer_init(){
// Set up the timer to trigger the callback at 6:00 AM every day
    esp_timer_create_args_t timer_args = {
        .callback = &timer_callback,
        .arg = NULL,
        .dispatch_method = ESP_TIMER_TASK,
        .name = "pump_timer",
    };
    esp_timer_create(&timer_args, &timer);
}

void set_timer(int hour, int minute, int second){
    // Get the current time
    char strftime_buf[64];
    time_t now;
    time(&now);
    struct tm timeinfo;
    localtime_r(&now, &timeinfo);
    setenv("TZ", TIME_ZONE, 1);
    tzset();
    strftime(strftime_buf, sizeof(strftime_buf), "%Z %c", &timeinfo);
    printf("strftime_buff = %s \n", strftime_buf);


    // Set the timer to trigger at 6:00 AM next day
    timeinfo.tm_hour = hour;
    timeinfo.tm_min = minute;
    timeinfo.tm_sec = second;
    time_t alarm_time = mktime(&timeinfo) ;//+ 24 * 60 * 60; 

    strftime(strftime_buf, sizeof(strftime_buf), "%Z %c", &timeinfo);
    printf("strftime_buff = %s \n", strftime_buf);
    printf("diff-time = :%lld \n",alarm_time - now);
    if(esp_timer_is_active(timer) == 1){
        esp_timer_restart(timer, (alarm_time - now) * 1000000); // Convert seconds to microseconds
    }
    else{
        esp_timer_start_once(timer, (alarm_time - now) * 1000000); // Convert seconds to microseconds
    }
}


// define mqtt event
void on_connected() {
    int msg_id = 0;
    msg_id  = esp_mqtt_client_subscribe(client, "v1/devices/me/rpc/request/+", 0);
    ESP_LOGI(MQTT_TAG, "sent subscribe successful, msg_id=%d", msg_id);
}

void on_messaged(char* topic, char* data){
    char method[255];
    // char params[255];
    int hour = 0;
    int minute = 0;
    sscanf(data,"{\"method\":\"%[^\"]\",\"params\":\"%d:%d\"}",method, &hour, &minute);
    printf("%s %d %d\n",method, hour, minute);

    if(strcmp(method,"pump_scheduler") == 0){
        printf("hour: %d, minute: %d", hour, minute);
        set_timer(hour,minute,0);
    }
    if(strcmp(method,"led_scheduler") == 0){
        printf("hour: %d, minute: %d", hour, minute);
        set_timer(hour,minute,0);
    }
}

void app_main(void)
{
    //Initialize NVS
    esp_err_t ret = nvs_flash_init();
    if (ret == ESP_ERR_NVS_NO_FREE_PAGES || ret == ESP_ERR_NVS_NEW_VERSION_FOUND) {
        ESP_ERROR_CHECK(nvs_flash_erase());
        ret = nvs_flash_init();
    }
    ESP_ERROR_CHECK(ret);
    //wifi connect
    wifi_init_sta();
    vTaskDelay(pdMS_TO_TICKS(2000));
    on_connect = on_connected;
    on_message = on_messaged;
    mqtt_app_start();
    vTaskDelay(pdMS_TO_TICKS(5000));
    Set_SystemTime_SNTP();
    // mqtt_app_start();
    vTaskDelay(pdMS_TO_TICKS(2000));

    timer_init();//
    uart_init();
    // Node_init();
    // turnOnPump();
// creating timer here
    // xMutex = xSemaphoreCreateMutex();
    // set_timer(15,40,0);
    char current_time[64];

    xTaskCreate(rx_task,"recieve",4024,NULL, 1 ,NULL);
    // strcpy(current_time,"!DP0#");
    // uart_write((uint8_t*)current_time,strlen(current_time));
    // sendData("Uart Connection", current_time);

    while(1)  {
            Get_current_date_time(current_time);
            // sprintf("current date and time is = %s\n",current_time);
            // uart_write((uint8_t*)current_time,strlen(current_time));
            vTaskDelay(1000 / portTICK_PERIOD_MS);
    }
    

}
