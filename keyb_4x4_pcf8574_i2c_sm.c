
#include "keyb_4x4_pcf8574_i2c_sm.h"

/******************************************************/

			uint8_t keyboard_IRQ_flag[2] 	= { 0 }	;
	const 	char 	keyboard_char[4][4]	= { { '1', '2', '3', 'a' },
											{ '4', '5', '6', 'b' },
											{ '7', '8', '9', 'c' },
											{ 'e', '0', 'f', 'd' } } ;

/******************************************************/

	//	for activate IRQ:
	#define 	IRQ_WORD			(0b00001111)
	#define		DEBUG_CHARS_SIZE	100

/******************************************************/
/******************************************************/

void PCF8574_struct_init (PCF8574_Struct * _pcf, uint8_t _channel,  I2C_HandleTypeDef * _i2c, UART_HandleTypeDef * _uart, uint8_t _addr ) {
	_pcf->channel	=	_channel;
	_pcf->i2c 		=	*_i2c 	;
	_pcf->devAddr_u8	=	_addr 	;
	_pcf->uart 		=	*_uart	;
}
/******************************************************/

void PCF8574_start_keyboard (PCF8574_Struct * _pcf) {
	char Debug_Char[DEBUG_CHARS_SIZE] = { 0 }	;
	sprintf(Debug_Char,"\r\nKeyBoard 4x4 over PCF8574 v2.3.0\r\n for debug USART1 on 115200/8-N-1 \r\n") ;
	HAL_UART_Transmit(&_pcf->uart, (uint8_t *)Debug_Char, strlen(Debug_Char), 100) ;

	I2C_ScanBusFlow(&_pcf->i2c, &_pcf->uart) ;
}
/******************************************************/

uint8_t PCF8574_scan_keyboard (PCF8574_Struct * _pcf) {
	HAL_GPIO_TogglePin(LED_GPIO_Port, LED_Pin) ;

	static 	uint8_t	keyboard_Row_u8 	= 0	;
	static 	uint8_t	keyboard_Col_u8		= 0	;
			uint8_t I2C_RW_register_u8 	= 0 ;

	for ( uint8_t IO_port_u8 = 0; IO_port_u8 < 8; IO_port_u8++) 	{	//	8 Input/Output lines
		I2C_RW_register_u8 = ( 1UL<<IO_port_u8 ) ;
		HAL_I2C_Master_Transmit( &_pcf->i2c , (_pcf->devAddr_u8)<<1 , &I2C_RW_register_u8 ,  1 , 100 ) ;
		HAL_I2C_Master_Receive ( &_pcf->i2c , (_pcf->devAddr_u8)<<1 , &I2C_RW_register_u8 ,  1 , 100 ) ;

		if (I2C_RW_register_u8 == 0) {	// якщо "1" скинулась в "0" значить це та лінія, де натиснута кнопка
			if ( IO_port_u8 <  4 ) {
				keyboard_Row_u8 = IO_port_u8 ;		//	спочатку знаходимо ряд
			}
			if ( IO_port_u8 >= 4) {
				keyboard_Col_u8 = IO_port_u8 - 4 ;	//	тепер колонку
			}
		}
	}
	uint8_t button_u8 = 10 * keyboard_Row_u8 + keyboard_Col_u8 ;
	//return keyboard_char[keyboard_Row_u8][keyboard_Col_u8] ;
	return button_u8;
}
/******************************************************/

void PCF8574_debug_print_key (PCF8574_Struct * _pcf, uint8_t _button) {
	static 	uint8_t previous_button[2]					= { 5, 5 }	;
			char Debug_Char[DEBUG_CHARS_SIZE]	= { 0 }	;

	if ( previous_button[_pcf->channel] == _button ) {
		return;
	}

	snprintf(Debug_Char, 6, "%02d%d\r\n", _button, (int)_pcf->channel) ;
	HAL_UART_Transmit(&_pcf->uart, (uint8_t *)Debug_Char, strlen(Debug_Char), 100) ;
	previous_button[_pcf->channel] = _button ;
}
/******************************************************/

uint8_t PCF8574_get_IRQ_flag (PCF8574_Struct * _pcf) {
	return keyboard_IRQ_flag[ _pcf->channel ] ;
}
/******************************************************/

void PCF8574_update_IRQ_flag (PCF8574_Struct * _pcf , uint8_t _flag) {
	if ( _flag == 0 ) {
		keyboard_IRQ_flag[ _pcf->channel ] = 0 ;
	} else {
		keyboard_IRQ_flag[ _pcf->channel ] = 1 ;
	}
}
/******************************************************/

void PCF8574_IRQ_enable (PCF8574_Struct * _pcf) {
	uint8_t I2C_RW_register_u8 = IRQ_WORD ;
	HAL_I2C_Master_Transmit( &_pcf->i2c , (_pcf->devAddr_u8)<<1 , &I2C_RW_register_u8 ,  1 , 100 )	;
}
/******************************************************/

void PCF8574_set_IRQ_flag (uint8_t _channel) {
	keyboard_IRQ_flag[_channel] = 1 ;
}
/******************************************************/
