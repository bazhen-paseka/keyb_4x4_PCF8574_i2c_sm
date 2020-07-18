#ifndef KEYBOARD_4X4_PCF8574_I2C_SM_H_INCLUDED
#define KEYBOARD_4X4_PCF8574_I2C_SM_H_INCLUDED

/******************************************************/

	#include <string.h>
	#include <stdio.h>
	#include "main.h"
	#include "i2c_techmaker_sm.h"

/******************************************************/

	#define I2C_ADR_PCF8574 0x38

/******************************************************/

	void PCF8574_struct_init 		(I2C_HandleTypeDef * _i2c, UART_HandleTypeDef * _uart, uint8_t _addr)	;
	void PCF8574_start_keyboard 	(void)																	;
	char PCF8574_scan_keyboard 		(void)																	;
	void PCF8574_debug_print_key 	(char _key_char)														;
	void PCF8574_IRQ_enable 		(void)																	;
	void PCF8574_update_IRQ_flag	(uint8_t _flag)															;
	uint8_t PCF8574_get_IRQ_flag	(void)																	;

/******************************************************/

#endif 	//	KEYBOARD_4X4_PCF8574_I2C_SM_H_INCLUDED
