
#include "keyb_4x4_pcf8574_i2c_sm.h"

/******************************************************/

			uint8_t keyboard_IRQ_flag 	= 0	;
	const 	char 	keyboard_char[4][4]	= { { '1', '2', '3', 'A' },
											{ '4', '5', '6', 'B' },
											{ '7', '8', '9', 'C' },
											{ '*', '0', '#', 'D' } } ;

/******************************************************/

	//	for activate IRQ:
	#define 	IRQ_WORD			(0b00001111)
	#define		DEBUG_CHARS_SIZE	100

/******************************************************/

	typedef struct {
		I2C_HandleTypeDef  	i2c		;
		uint8_t 			devAddr	;
		UART_HandleTypeDef 	uart	;
	} PCF8574_keyboard_struct		;

/******************************************************/

	PCF8574_keyboard_struct 	PCF8574_struct ;

/******************************************************/
/******************************************************/

void PCF8574_struct_init ( I2C_HandleTypeDef * _i2c, UART_HandleTypeDef * _uart, uint8_t _addr ) {
	PCF8574_struct.i2c 		= *_i2c 	;
	PCF8574_struct.devAddr	= _addr 	;
	PCF8574_struct.uart 	= *_uart	;
}
/******************************************************/

void PCF8574_start_keyboard (void) {
	char Debug_Char[DEBUG_CHARS_SIZE] = { 0 }	;
	sprintf(Debug_Char,"\r\nKeyBoard 4x4 over PCF8574 v2.2.0\r\n for debug USART1 on 115200/8-N-1 \r\n") ;
	HAL_UART_Transmit(&PCF8574_struct.uart, (uint8_t *)Debug_Char, strlen(Debug_Char), 100) ;

	I2C_ScanBusFlow(&PCF8574_struct.i2c, &PCF8574_struct.uart) ;
}
/******************************************************/

char PCF8574_scan_keyboard (void) {
	HAL_GPIO_TogglePin(LED_GPIO_Port, LED_Pin) ;

	static 	uint8_t	keyboard_Row_u8 	= 0		;
	static 	uint8_t	keyboard_Col_u8		= 0		;
			uint8_t I2C_RW_register_u8 	= 0 	;
			uint8_t magic_var_u8		= 0 	;

	for ( uint8_t IO_port_u8 = 0; IO_port_u8<8; IO_port_u8++) 	{	//	8 Input/Output lines
		I2C_RW_register_u8 = ( 1UL<<IO_port_u8 ) ;
		HAL_I2C_Master_Transmit(&PCF8574_struct.i2c, PCF8574_struct.devAddr<<1, &I2C_RW_register_u8,  1, 100 ) ;
		HAL_I2C_Master_Receive (&PCF8574_struct.i2c, PCF8574_struct.devAddr<<1, &I2C_RW_register_u8,  1, 100 ) ;

		if (I2C_RW_register_u8 == 0) {	// якщо "1" скинулась в "0" значить це та лінія, де натиснута кнопка
			if ((magic_var_u8==0) && ( IO_port_u8 <4 )) {
				keyboard_Row_u8 = IO_port_u8 ;		//	спочатку знаходимо ряд
			}
			if ((magic_var_u8==1) && ( IO_port_u8 >= 4) && ( IO_port_u8 < 8 )) {
				keyboard_Col_u8 = IO_port_u8 - 4 ;	//	тепер колонку
			}
			magic_var_u8 = 1 ;
		}
	}
	return keyboard_char[keyboard_Row_u8][keyboard_Col_u8] ;
}
/******************************************************/

void PCF8574_debug_print_key (char _key_char) {
	static 	char previous_char					= '-'	;
			char Debug_Char[DEBUG_CHARS_SIZE]	= { 0 }	;

	if ( previous_char == _key_char ) {
		return;
	}

	snprintf(Debug_Char, 4, "%c\r\n", _key_char) ;
	HAL_UART_Transmit(&PCF8574_struct.uart, (uint8_t *)Debug_Char, strlen(Debug_Char), 100) ;
	previous_char = _key_char ;
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

void PCF8574_IRQ_enable (void) {
	uint8_t I2C_RW_register_u8 = IRQ_WORD ;
	HAL_I2C_Master_Transmit(&PCF8574_struct.i2c, PCF8574_struct.devAddr<<1, &I2C_RW_register_u8,  1, 100 )	;
}
/******************************************************/
