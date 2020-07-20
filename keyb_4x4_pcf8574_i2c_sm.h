#ifndef KEYBOARD_4X4_PCF8574_I2C_SM_H_INCLUDED
#define KEYBOARD_4X4_PCF8574_I2C_SM_H_INCLUDED

/******************************************************/

	#include <string.h>
	#include <stdio.h>
	#include "main.h"
	#include "i2c_techmaker_sm.h"
	#include "pcf8574_local_config.h"

/******************************************************/

	typedef struct {
		uint8_t				channel ;
		I2C_HandleTypeDef  	i2c		;
		uint8_t 			devAddr	;
		UART_HandleTypeDef 	uart	;
	} PCF8574_Struct		;

/******************************************************/

	void PCF8574_struct_init 		(	PCF8574_Struct 		*_pcf,
										uint8_t 			_channel,
										I2C_HandleTypeDef	*_i2c,
										UART_HandleTypeDef	*_uart,
										uint8_t 			_addr)	;

	void PCF8574_start_keyboard 	(PCF8574_Struct * _pcf)					;
	uint8_t PCF8574_scan_keyboard 	(PCF8574_Struct * _pcf)					;
	void PCF8574_debug_print_key 	(PCF8574_Struct * _pcf, uint8_t _button);
	void PCF8574_IRQ_enable 		(PCF8574_Struct * _pcf)					;
	void PCF8574_update_IRQ_flag	(PCF8574_Struct * _pcf, uint8_t _flag)	;
	uint8_t PCF8574_get_IRQ_flag	(PCF8574_Struct * _pcf)					;
	void PCF8574_set_IRQ_flag 		(uint8_t _channel) 						;

/******************************************************/

#endif 	//	KEYBOARD_4X4_PCF8574_I2C_SM_H_INCLUDED
