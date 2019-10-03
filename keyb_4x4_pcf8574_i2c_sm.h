#ifndef KEYBOARD_4X4_PCF8574_I2C_SM_H_INCLUDED
#define KEYBOARD_4X4_PCF8574_I2C_SM_H_INCLUDED

	#include <string.h>
	#include <stdio.h>
	#include "main.h"
	#include "i2c_techmaker_sm.h"

/***********************************/

/***********************************/

	char key[4][4];
	char DataChar[100];

	typedef struct
	{
		UART_HandleTypeDef uart;
		I2C_HandleTypeDef  i2c;
		uint8_t devAddr;
	} keyboard_struct;

	keyboard_struct KEY;
/***********************************/

	void Init_keyboard_struct(I2C_HandleTypeDef * _i2c, UART_HandleTypeDef * _uart, uint8_t _addr);
	void Start_keyboard(void);
	char Scan_keyboard(void);
	uint8_t Get_keyboard_int_flag(void);
	void Update_keyboard_int_flag(uint8_t _flag);


#endif 	//	KEYBOARD_4X4_PCF8574_I2C_SM_H_INCLUDED
