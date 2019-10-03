
#include "keyb_4x4_pcf8574_i2c_sm.h"

/******************************************************/

	uint8_t keyboard_int_flag = 0;
	uint8_t	row = 0;
	uint8_t	col = 0;
	char previous_char = '-';
	/******************************************************/

void init_struct(keyboard_struct *_key, I2C_HandleTypeDef * _i2c, UART_HandleTypeDef * _uart)
{
	_key->devAddr = I2C_ADR_PCF8574;
	_key->i2c = *_i2c;
	_key->uart = *_uart;
}
/******************************************************/

void init_keyboard(keyboard_struct *_key)
{
	char DataChar[100];
	sprintf(DataChar,"\r\nKeyBord 4x4 over PCF8574 v2.0.0\r\nUART1 for debug started on speed 9600\r\n");
	HAL_UART_Transmit(&_key->uart, (uint8_t *)DataChar, strlen(DataChar), 100);

	I2Cdev_init(&_key->i2c);
	I2C_ScanBusFlow(&_key->i2c, &_key->uart);


	keyboard_u8 = 0b00001111;
	HAL_I2C_Master_Transmit(&_key->i2c, _key->devAddr<<1, &keyboard_u8,  1, 100 );

	key[0][0] = '1';
	key[0][1] = '2';
	key[0][2] = '3';
	key[0][3] = 'A';

	key[1][0] = '4';
	key[1][1] = '5';
	key[1][2] = '6';
	key[1][3] = 'B';

	key[2][0] = '7';
	key[2][1] = '8';
	key[2][2] = '9';
	key[2][3] = 'C';

	key[3][0] = '*';
	key[3][1] = '0';
	key[3][2] = '\r';
	key[3][3] = '\n';
}
/******************************************************/

void scan_keyboard(keyboard_struct * _key)
{
	uint8_t scan=0;
	for ( uint8_t i = 0; i<8; i++)
	{
		keyboard_u8 = (1UL<<i);
		HAL_I2C_Master_Transmit(&_key->i2c, _key->devAddr<<1, &keyboard_u8,  1, 100 );
		HAL_I2C_Master_Receive(&_key->i2c, _key->devAddr<<1, &keyboard_u8,  1, 100 );

		if (keyboard_u8 == 0)
		{
			if ((scan==0) && (i<4))
			{
				row = i;
			}
			if ((scan==1) && (i>=4) && (i<8))
			{
				col = i-4;
			}
			scan++;
		}
	}

	if ( previous_char != key[row][col] )
	{
		//sprintf(DataChar,"[%d][%d] btn %c\r\n",  row, col, key[row][col]);
		sprintf(DataChar,"%c", key[row][col]);
		HAL_UART_Transmit(&_key->uart, (uint8_t *)DataChar, strlen(DataChar), 100);
		previous_char = key[row][col];
	}

	keyboard_u8 = 0b00001111;
	HAL_I2C_Master_Transmit(&_key->i2c, _key->devAddr<<1, &keyboard_u8,  1, 100 );
}
/******************************************************/

uint8_t Get_keyboard_int_flag(void)
{
	return keyboard_int_flag;
}
/******************************************************/

void Update_keyboard_int_flag(uint8_t _flag)
{
	keyboard_int_flag = _flag;
}
/******************************************************/
/******************************************************/
/******************************************************/
/******************************************************/
