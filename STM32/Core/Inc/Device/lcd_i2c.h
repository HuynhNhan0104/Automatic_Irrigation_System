/*
 * lcd_i2c.h
 *
 *  Created on: May 8, 2023
 *      Author: NHAN
 */

#ifndef INC_LCD_I2C_H_
#define INC_LCD_I2C_H_

#include "main.h"

#define LCD_COMMAND 	0x00
#define LCD_DATA 		0x01

#define LCD_EN 0x04  // Enable bit
#define LCD_RW 0x02  // Read/Write bit
#define LCD_RS 0x01  // Register select bit



// commands
#define LCD_CLEAR_DISPLAY 		0x01
#define LCD_RETURN_HOME 			0x02

#define LCD_ENTRY_MODE_SET 		0x04
#define LCD_DISPLAY_CONTROL 		0x08
#define LCD_CURSOR_SHIFT 		0x10
#define LCD_FUNCTION_SET 		0x20
#define LCD_SET_CGRAM_ADDR 		0x40
#define LCD_SET_DDRAM_ADDR	 	0x80

// flags for display entry mode
#define LCD_ENTRY_RIGHT 			0x00
#define LCD_ENTRY_LEFT 			0x02
#define LCD_ENTRY_SHIFT_INCREMENT 0x01
#define LCD_ENTRY_SHIFT_DECREMENT 0x00

// flags for display on/off control
#define LCD_DISPLAY_ON 			0x04
#define LCD_DISPLAY_OFF 			0x00
#define LCD_CURSOR_ON 			0x02
#define LCD_CURSOR_OFF 			0x00
#define LCD_BLINK_ON 			0x01
#define LCD_BLINK_OFF 			0x00

// flags for display/cursor shift
#define LCD_DISPLAY_MOVE 		0x08
#define LCD_CURSOR_MOVE 			0x00
#define LCD_MOVE_RIGHT 			0x04
#define LCD_MOVE_LEFT 			0x00

// flags for function set
#define LCD_8BITMODE 			0x10
#define LCD_4BITMODE 			0x00
#define LCD_2LINE 				0x08
#define LCD_1LINE 				0x00
#define LCD_5x10DOTS 			0x04
#define LCD_5x8DOTS 			0x00

#define LCD_BACKLIGHT 			0x08
#define LCD_NOBACKLIGHT 		0x00


typedef struct {
	I2C_HandleTypeDef* hi2c;
	uint8_t address;
	uint8_t colums;
	uint8_t rows;
	uint8_t entryMode;
	uint8_t displayCtrl;
	uint8_t cursorShift;
	uint8_t functionSet;
	uint8_t backLight;
} LCD_I2C_DEVICE;

//static void LCD_I2C_Write(  LCD_I2C_DEVICE* LCD,uint8_t data, uint8_t mode);
void LCD_I2C_Init(LCD_I2C_DEVICE* LCD, I2C_HandleTypeDef* hi2c, uint8_t address, uint8_t colums, uint8_t rows);
void LCD_I2C_SetCursor(LCD_I2C_DEVICE* LCD,uint8_t Xpos, uint8_t Ypos);
void LCD_I2C_Clear(LCD_I2C_DEVICE* LCD);
void LCD_I2C_Return_Home(LCD_I2C_DEVICE* LCD);

void LCD_I2C_WriteChar(  LCD_I2C_DEVICE* LCD,char data);
void LCD_I2C_WriteString( LCD_I2C_DEVICE* LCD, char *String);

#endif /* INC_LCD_I2C_H_ */
