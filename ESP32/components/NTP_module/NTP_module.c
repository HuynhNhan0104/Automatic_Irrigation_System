#include "NTP_module.h"

char Current_Date_Time[100];
const char *NTP_TAG = "NTP SYNC";
void time_sync_notification_cb(struct timeval *tv)
{
    ESP_LOGI(NTP_TAG, "Notification of a time synchronization event");
}

void Get_current_date_time(char *date_time){
	char strftime_buf[64];
	time_t now;
    struct tm timeinfo;
    time(&now);
    localtime_r(&now, &timeinfo);

// Set timezone to VN Standard Time
    setenv("TZ", TIME_ZONE, 1);
    tzset();

    localtime_r(&now, &timeinfo);
    // printf("%d:%d:%d %d_%d_%d %s\n", timeinfo.tm_hour, timeinfo.tm_min, timeinfo.tm_sec,timeinfo.tm_mday,timeinfo.tm_mon,timeinfo.tm_year, timeinfo.tm_isdst?"AM":"PM");
    strftime(strftime_buf, sizeof(strftime_buf), "%Z %c", &timeinfo);
    ESP_LOGI(NTP_TAG, "The current date/time in HCM city is: %s", strftime_buf);
    strcpy(date_time,strftime_buf);
}

static void initialize_sntp(void)
{
    ESP_LOGI(NTP_TAG, "Initializing SNTP");
    esp_sntp_setoperatingmode(SNTP_OPMODE_POLL);
    // esp_sntp_setservername(0, "asia.pool.ntp.org");
    esp_sntp_setservername(0, NTP_SERVER);

    sntp_set_time_sync_notification_cb(time_sync_notification_cb);
#ifdef CONFIG_SNTP_TIME_SYNC_METHOD_SMOOTH
    sntp_set_sync_mode(SNTP_SYNC_MODE_SMOOTH);
#endif
    esp_sntp_init();
}

static void obtain_time(void)
{


    initialize_sntp();
    // wait for time to be set
    time_t now = 0;
    struct tm timeinfo = { 0 };
    int retry = 0;
    const int retry_count = 10;
    while (sntp_get_sync_status() == SNTP_SYNC_STATUS_RESET && ++retry < retry_count) {
        ESP_LOGI(NTP_TAG, "Waiting for system time to be set... (%d/%d)", retry, retry_count);
        vTaskDelay(2000 / portTICK_PERIOD_MS);
    }
    time(&now);
    localtime_r(&now, &timeinfo);
}
 void Set_SystemTime_SNTP()  {

	 time_t now;
	    struct tm timeinfo;
	    time(&now);
	    localtime_r(&now, &timeinfo);
	    // Is time set? If not, tm_year will be (1970 - 1900).
	    if (timeinfo.tm_year < (2016 - 1900)) {
	        ESP_LOGI(NTP_TAG, "Time is not set yet. Connecting to WiFi and getting time over NTP.");
	        obtain_time();
	        // update 'now' variable with current time
	        time(&now);
	    }
}