#ifndef KEYBOARD_4X4_PCF8574_I2C_SM_H_INCLUDED
#define KEYBOARD_4X4_PCF8574_I2C_SM_H_INCLUDED

/******************************************************/

	#include <string.h>
	#include <stdio.h>
	#include "main.h"
	#include "usart.h"

	#include "i2c_techmaker_sm.h"
	#include "keyb_4x4_pcf8574_i2c_sm.h"
	#include "pcf8574_local_config.h"
	#include "3Dbox-keyboards-local-config.h"

/******************************************************/

	typedef struct {
		uint8_t				channel 	;
		I2C_HandleTypeDef  	i2c			;
		uint8_t 			devAddr_u8	;
		uint8_t				key_current	;
		uint8_t				key_previous;
//		uint8_t				IRQ_flag	;
	} 		PCF8574_Struct				;

/******************************************************/

	void PCF8574_struct_init(		PCF8574_Struct 		*_pcf		,
									I2C_HandleTypeDef	*_i2c		,
									uint8_t 			_channel	,
									uint8_t 			_addr		) ;

	uint8_t PCF8574_scan_keyboard(	PCF8574_Struct		*_pcf		) ;


	void PCF8574_IRQ_enable(		PCF8574_Struct		*_pcf		) ;
	void PCF8574_update_IRQ_flag(	PCF8574_Struct		*_pcf		,
									uint8_t				_flag		) ;

	uint8_t PCF8574_get_IRQ_flag(	PCF8574_Struct		*_pcf		) ;
	void PCF8574_set_IRQ_flag(		uint8_t 			_channel	) ;

/******************************************************/

#endif 	//	KEYBOARD_4X4_PCF8574_I2C_SM_H_INCLUDED
