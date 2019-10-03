#ifndef KEYBOARD_4X4_PCF8574_I2C_SM_H_INCLUDED
#define KEYBOARD_4X4_PCF8574_I2C_SM_H_INCLUDED

	#include <string.h>
	#include <stdio.h>
	#include "main.h"
	#include "i2c_techmaker_sm.h"

/***********************************/
	#define I2C_ADR_PCF8574 0x38
	//uint8_t devAddr = I2C_ADR_PCF8574;
	uint8_t keyboard_u8 ;

	char key[4][4];

	typedef struct
	{
		UART_HandleTypeDef uart;
		I2C_HandleTypeDef  i2c;
		uint8_t devAddr;
	} keyboard_struct;
	/***********************************/



	char DataChar[100];

	keyboard_struct KEY;

	void init_struct(keyboard_struct *_key,I2C_HandleTypeDef * _i2c, UART_HandleTypeDef * _uart);
	void init_keyboard(keyboard_struct * _key);
	void scan_keyboard(keyboard_struct * _key);
	uint8_t Get_keyboard_int_flag(void);
	void Update_keyboard_int_flag(uint8_t _flag);


#endif 	//	KEYBOARD_4X4_PCF8574_I2C_SM_H_INCLUDED
