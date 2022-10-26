
#include "keyb_4x4_pcf8574_i2c_sm.h"

/******************************************************/

			uint8_t keyboard_IRQ_flag 	=  0 ;
	const 	char 	keyboard_char[4][4]		= { { '1', '2', '3', 'a' } ,
												{ '4', '5', '6', 'b' } ,
												{ '7', '8', '9', 'c' } ,
												{ 'e', '0', 'f', 'd' } } ;

/******************************************************/

	//	for activate IRQ:
	#define 	IRQ_WORD			(0b00001111)
	#define		DEBUG_CHARS_SIZE	100

/******************************************************/
/******************************************************/

void PCF8574_struct_init(	PCF8574_Struct		*_pcf		,
							I2C_HandleTypeDef	*_i2c		,
							uint8_t				_channel	,
							uint8_t				_addr		) {
	_pcf->channel		=	_channel;
	_pcf->i2c 			=	*_i2c 	;
	_pcf->devAddr_u8	=	_addr 	;
}
/******************************************************/

uint8_t PCF8574_scan_keyboard (PCF8574_Struct * _pcf) {

	static 	uint8_t	keyboard_Row_u8 	= 0	;
	static 	uint8_t	keyboard_Col_u8		= 0	;
			uint8_t I2C_RW_register_u8 	= 0 ;

	for ( uint8_t IO_port_u8 = 0; IO_port_u8 < 8; IO_port_u8++) 	{	//	8 Input/Output lines
		I2C_RW_register_u8 = ( 1UL<<IO_port_u8 ) ;
		HAL_I2C_Master_Transmit( &_pcf->i2c , (_pcf->devAddr_u8)<<1 , &I2C_RW_register_u8 ,  1 , 100 ) ;
		HAL_I2C_Master_Receive ( &_pcf->i2c , (_pcf->devAddr_u8)<<1 , &I2C_RW_register_u8 ,  1 , 100 ) ;

		if (I2C_RW_register_u8 == 0) {	//if "1" switch to "0" so this is the line where you click
			if ( IO_port_u8 <  4 ) {
				keyboard_Row_u8 = IO_port_u8 ;		//	first of all find the ROW
			}
			if ( IO_port_u8 >= 4) {
				keyboard_Col_u8 = IO_port_u8 - 4 ;	//	then find the column
			}
		}
	}
	uint8_t button_u8 = 10 * keyboard_Row_u8 + keyboard_Col_u8 ;
	//return keyboard_char[keyboard_Row_u8][keyboard_Col_u8] ;
	return button_u8;
}
/******************************************************/

uint8_t PCF8574_get_IRQ_flag (void) {
	return keyboard_IRQ_flag ;
}
/******************************************************/

void PCF8574_update_IRQ_flag (uint8_t _flag) {
	if ( _flag == 0 ) {
		keyboard_IRQ_flag = 0 ;
	} else {
		keyboard_IRQ_flag = 1 ;
	}
}
/******************************************************/

void PCF8574_IRQ_enable (PCF8574_Struct * _pcf) {
	uint8_t I2C_RW_register_u8 = IRQ_WORD ;
	HAL_I2C_Master_Transmit( &_pcf->i2c , (_pcf->devAddr_u8)<<1 , &I2C_RW_register_u8 ,  1 , 100 )	;
}
/******************************************************/

void PCF8574_set_IRQ_flag (void) {
	keyboard_IRQ_flag = 1 ;
}
/******************************************************/
