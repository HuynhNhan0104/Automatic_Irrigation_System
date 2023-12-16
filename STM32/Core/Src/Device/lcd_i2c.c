/*
 * lcd_i2c.c
 *
 *  Created on: May 8, 2023
 *      Author: NHAN
 */


#include "Device/lcd_i2c.h"
//#include "main.h"
static void LCD_I2C_Delay(uint16_t time){
	HAL_Delay(time);
}

static void LCD_I2C_Write(  LCD_I2C_DEVICE* LCD, uint8_t data, uint8_t mode){
	uint8_t dataUpper = data & 0xF0;
	uint8_t dataLower = (data<<4) & 0xF0;
	uint8_t dataBuffer[4];
	if( LCD->backLight)
	{
		dataUpper |= LCD_BACKLIGHT;
		dataLower |= LCD_BACKLIGHT;
	}
	if(mode == LCD_DATA)
	{
		dataUpper |= LCD_RS;
		dataLower |= LCD_RS;
	}
	else if(mode == LCD_COMMAND)
	{
		dataUpper &= ~LCD_RS;
		dataLower &= ~LCD_RS;
	}
	//send 4 bit upper
	dataBuffer[0] = dataUpper | LCD_EN ; 				//set EN bit =1;
//	LCD_I2C_Delay(1);
	dataBuffer[1] =dataUpper ;	// set EN bit = 0

	//send 4 bit lower
	dataBuffer[2] = dataLower | LCD_EN;					//set EN bit =1;
//	LCD_I2C_Delay(1);
	dataBuffer[3] = dataLower;								//set EN bit =0;
	HAL_I2C_Master_Transmit(LCD->hi2c, LCD->address,(uint8_t*) dataBuffer, sizeof(dataBuffer), 100);
}
void LCD_I2C_Init(LCD_I2C_DEVICE* LCD, I2C_HandleTypeDef* hi2c, uint8_t address, uint8_t colums, uint8_t rows){
	LCD->hi2c 			= hi2c;
	LCD->address 		= address;
	LCD->colums 		= colums;
	LCD->rows 			= rows;
	// set LCD to 4bit mode, display 2 line, cell is 5x8 dots
	LCD->functionSet 	= LCD_FUNCTION_SET | LCD_4BITMODE |	LCD_2LINE | LCD_5x8DOTS;
	LCD->entryMode 		= LCD_ENTRY_MODE_SET	| LCD_ENTRY_LEFT | LCD_ENTRY_SHIFT_DECREMENT;
	//set LCD to display on, cursor off , blink off
	LCD->displayCtrl		= LCD_DISPLAY_CONTROL | LCD_DISPLAY_ON	| LCD_CURSOR_OFF | LCD_BLINK_OFF;
	LCD->cursorShift		= LCD_CURSOR_SHIFT	| LCD_CURSOR_MOVE	| LCD_MOVE_RIGHT;
	LCD->backLight 		= LCD_BACKLIGHT;

		LCD_I2C_Delay(50);

		LCD_I2C_Write(LCD, 0x33, LCD_COMMAND);
	//	CLCD_Delay(5);
		LCD_I2C_Write(LCD, 0x33, LCD_COMMAND);
		LCD_I2C_Delay(5);
		LCD_I2C_Write(LCD, 0x32, LCD_COMMAND);
		LCD_I2C_Delay(5);
		LCD_I2C_Write(LCD, 0x20, LCD_COMMAND);
		LCD_I2C_Delay(5);

		LCD_I2C_Write(LCD, LCD->entryMode, LCD_COMMAND);
		LCD_I2C_Delay(2);
		LCD_I2C_Write(LCD, LCD->displayCtrl, LCD_COMMAND);
		LCD_I2C_Delay(2);
		LCD_I2C_Write(LCD, LCD->cursorShift, LCD_COMMAND);
		LCD_I2C_Delay(2);
		LCD_I2C_Write(LCD, LCD->functionSet, LCD_COMMAND);
		LCD_I2C_Delay(2);

		LCD_I2C_Write(LCD, LCD_CLEAR_DISPLAY, LCD_COMMAND);
		LCD_I2C_Delay(5);
		LCD_I2C_Write(LCD, LCD_RETURN_HOME, LCD_COMMAND);
		LCD_I2C_Delay(5);

}



 void LCD_I2C_SetCursor(LCD_I2C_DEVICE* LCD, uint8_t Xpos, uint8_t Ypos){
	uint8_t DRAM_ADDRESS = 0x00;
	// if x position is greater than colums number so set position is (col-1);
	if(Xpos >= LCD->colums)
	{
		Xpos = LCD->colums - 1;
	}
	// if x position is greater than rows number so set position is (row-1);
	if(Ypos >= LCD->rows)
	{
		Ypos = LCD->rows -1;
	}
	// assign DRAM_ADDRESS to address of cell(row 1: 0x00 -> 0x0F, row2 : 0x40 -> 0x4F) in LCD
	if(Ypos == 0)
	{
		DRAM_ADDRESS = 0x00 + Xpos;
	}
	else if(Ypos == 1)
	{
		DRAM_ADDRESS = 0x40 + Xpos;
	}


	LCD_I2C_Write(LCD,  LCD_SET_DDRAM_ADDR | DRAM_ADDRESS, LCD_COMMAND);
}
void LCD_I2C_Clear(LCD_I2C_DEVICE* LCD){
	LCD_I2C_Write(LCD, LCD_CLEAR_DISPLAY, LCD_COMMAND);
	LCD_I2C_Delay(5);
}
void LCD_I2C_Return_Home(LCD_I2C_DEVICE* LCD){
	LCD_I2C_Write(LCD, LCD_RETURN_HOME, LCD_COMMAND);
	LCD_I2C_Delay(5);
}
void LCD_I2C_WriteChar(  LCD_I2C_DEVICE* LCD,char character){
	LCD_I2C_Write(LCD, character, LCD_DATA);
}

void LCD_I2C_WriteString( LCD_I2C_DEVICE* LCD, char *String){
	while(*String)   LCD_I2C_WriteChar(LCD, *String++);
}
