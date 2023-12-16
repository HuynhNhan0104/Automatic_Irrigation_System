#ifndef _NTP_MODULE_
#define _NTP_MODULE_

#include <string.h>
#include "freertos/FreeRTOS.h"
#include "freertos/event_groups.h"
#include "esp_log.h"
#include "esp_event.h"


#include <stdlib.h>
#include <stdio.h>

#include <time.h>
#include <sys/time.h>
#include "esp_attr.h"
#include "esp_sleep.h"
#include "esp_sntp.h"

#define NTP_SERVER "pool.ntp.org"
#define TIME_ZONE "UTC-7"

extern const char *NTP_TAG ;

extern char Current_Date_Time[100];
void time_sync_notification_cb(struct timeval *tv);
void Get_current_date_time(char *date_time);
void Set_SystemTime_SNTP();
#endif