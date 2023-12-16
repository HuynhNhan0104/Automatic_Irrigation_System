/*
 * rtc.h
 *
 *  Created on: Jun 10, 2023
 *      Author: xuanthodo
 */

#ifndef INC_HAL_RTC_H_
#define INC_HAL_RTC_H_

typedef struct {
	uint8_t second;
	uint8_t minute;
	uint8_t hour;
	uint8_t date;
	uint8_t month;
	uint16_t year;
}RTC_t;

void RTC_init();
RTC_t RTC_get_time();
void RTC_set_time(RTC_t *rtc);

#endif /* INC_HAL_RTC_H_ */
