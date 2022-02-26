/*
 * lcd.c
 *
 *  Created on: 29 dec. 2018
 *      Author: Ludo
 */

#include "lcd.h"

#include "gpio.h"
#include "gpio_reg.h"
#include "lptim.h"
#include "mapping.h"
#include "string.h"
#include "tim.h"

/*** LCD local macros ***/

#define LCD_ROW_IDX_MAX			1
#define LCD_COLUMN_IDX_MAX		7

/*** LCD local functions ***/

/* CREATE A PULSE ON ENABLE SIGNAL TO WRITE LCD REGISTERS.
 * @param:	None.
 * @return:	None.
 */
static void LCD_enable_pulse(void) {
	// Local variables.
	unsigned int loop_count = 0;
	// Pulse width > 460ns.
	for (loop_count=0 ; loop_count<5 ; loop_count++) {
		GPIO_write(&GPIO_LCD_E, ((loop_count > 0x01) ? 1 : 0));
	}
	GPIO_write(&GPIO_LCD_E, 0);
}

/* SEND A COMMAND TO LCD SCREEN.
 * @param lcd_command:	Command to send.
 * @return:				None.
 */
static void LCD_command(unsigned char lcd_command) {
	// Put command on output port.
	GPIOA -> ODR &= 0xFFFFFE01;
	GPIOA -> ODR |= (lcd_command << 1);
	// Send command.
	GPIO_write(&GPIO_LCD_RS, 0);
	LCD_enable_pulse();
}

/* SEND DATA TO LCD SCREEN.
 * @param lcd_data:	Data to send.
 * @return:			None.
 */
static void LCD_data(unsigned char lcd_data) {
	// Put data on output port.
	GPIOA -> ODR &= 0xFFFFFE01;
	GPIOA -> ODR |= (lcd_data << 1);
	// Send command.
	GPIO_write(&GPIO_LCD_RS, 1);
	LCD_enable_pulse();
}

/*** LCD functions ***/

void LCD_init(void) {
	// Init GPIOs.
	GPIO_configure(&GPIO_LCD_E, GPIO_MODE_OUTPUT, GPIO_TYPE_PUSH_PULL, GPIO_SPEED_LOW, GPIO_PULL_NONE);
	GPIO_configure(&GPIO_LCD_RS, GPIO_MODE_OUTPUT, GPIO_TYPE_PUSH_PULL, GPIO_SPEED_LOW, GPIO_PULL_NONE);
	GPIO_configure(&GPIO_LCD_DB0, GPIO_MODE_OUTPUT, GPIO_TYPE_PUSH_PULL, GPIO_SPEED_LOW, GPIO_PULL_NONE);
	GPIO_configure(&GPIO_LCD_DB1, GPIO_MODE_OUTPUT, GPIO_TYPE_PUSH_PULL, GPIO_SPEED_LOW, GPIO_PULL_NONE);
	GPIO_configure(&GPIO_LCD_DB2, GPIO_MODE_OUTPUT, GPIO_TYPE_PUSH_PULL, GPIO_SPEED_LOW, GPIO_PULL_NONE);
	GPIO_configure(&GPIO_LCD_DB3, GPIO_MODE_OUTPUT, GPIO_TYPE_PUSH_PULL, GPIO_SPEED_LOW, GPIO_PULL_NONE);
	GPIO_configure(&GPIO_LCD_DB4, GPIO_MODE_OUTPUT, GPIO_TYPE_PUSH_PULL, GPIO_SPEED_LOW, GPIO_PULL_NONE);
	GPIO_configure(&GPIO_LCD_DB5, GPIO_MODE_OUTPUT, GPIO_TYPE_PUSH_PULL, GPIO_SPEED_LOW, GPIO_PULL_NONE);
	GPIO_configure(&GPIO_LCD_DB6, GPIO_MODE_OUTPUT, GPIO_TYPE_PUSH_PULL, GPIO_SPEED_LOW, GPIO_PULL_NONE);
	GPIO_configure(&GPIO_LCD_DB7, GPIO_MODE_OUTPUT, GPIO_TYPE_PUSH_PULL, GPIO_SPEED_LOW, GPIO_PULL_NONE);
	// Initialization sequence.
	GPIO_write(&GPIO_LCD_E, 0);
	LPTIM1_delay_milliseconds(100);
	LCD_command(0x30);
	LPTIM1_delay_milliseconds(30);
	LCD_command(0x30);
	LPTIM1_delay_milliseconds(10);
	LCD_command(0x30);
	LPTIM1_delay_milliseconds(10);
	LCD_command(0x38); // 8-bits / 2 lines mode.
	LCD_command(0x08); // Display off.
	LCD_command(0x0C); // Display on.
	// Clear.
	LCD_clear();
}

/* PRINT A STRING ON LCD SCREEN.
 * @param row :     		Current row (0 or 1).
 * @param position  		Cursor position (0 to 7).
 * @param string:			Char array to print.
 * @param string_length:	Number of characters to print.
 * @return:					None.
 */
void LCD_print(unsigned char row, unsigned char column, char* string, unsigned char string_length) {
	// Check parameters.
	if ((row <= LCD_ROW_IDX_MAX) && (column <= LCD_COLUMN_IDX_MAX)) {
		// Set position.
		LCD_command(((row * 0x40) + column) + 0x80);
		// Print string.
		unsigned char column_idx = column;
		unsigned char string_idx = 0;
		// Loop until string is printed or screen edge is reached.
		while ((column_idx <= LCD_COLUMN_IDX_MAX) && (column_idx < (column + string_length))) {
			LCD_data(string[string_idx]);
			string_idx++;
			column_idx++;
		}
	}
}

/* CLEAR LCD SCREEN.
 * @param:	None.
 * @return:	None.
 */
void LCD_clear(void) {
	LCD_command(0x01);
	LPTIM1_delay_milliseconds(2);
}

/* PRINT A SIGFOX DEVICE ID.
 * @param sigfox_id:	ID to print.
 * @return:				None.
 */
void LCD_print_sigfox_id(unsigned char row, unsigned char sigfox_id[SIGFOX_DEVICE_ID_LENGTH_BYTES]) {
	// Build corresponding string.
	char sigfox_id_string[2 * SIGFOX_DEVICE_ID_LENGTH_BYTES];
	unsigned char byte_idx = 0;
	for (byte_idx=0 ; byte_idx<SIGFOX_DEVICE_ID_LENGTH_BYTES ; byte_idx++) {
		sigfox_id_string[2 * byte_idx] = STRING_hexa_to_ascii((sigfox_id[byte_idx] & 0xF0) >> 4);
		sigfox_id_string[2 * byte_idx + 1] = STRING_hexa_to_ascii((sigfox_id[byte_idx] & 0x0F) >> 0);
	}
	// Print ID.
	LCD_print(row, 0, sigfox_id_string, (2 * SIGFOX_DEVICE_ID_LENGTH_BYTES));
}

/* PRINT A VALUE ON 5 DIGITS.
 * @param value:	(value * 1000) to print.
 * @return:			None.
 */
void LCD_print_value_5_digits(unsigned char row, unsigned char column, unsigned int value) {
	// Local variables.
	unsigned char u1, u2, u3, u4, u5;
	unsigned char d1, d2, d3;
	char value_string[5] = {0};
	// Convert value to message.
	if (value < 10000) {
		// Format = u.ddd
		u1 = (value) / (1000);
		d1 = (value - (u1 * 1000)) / (100);
		d2 = (value - (u1 * 1000) - (d1 * 100)) / (10);
		d3 = value - (u1 * 1000) - (d1 * 100) - (d2 * 10);
		value_string[0] = STRING_decimal_to_ascii(u1);
		value_string[1] = '.';
		value_string[2] = STRING_decimal_to_ascii(d1);
		value_string[3] = STRING_decimal_to_ascii(d2);
		value_string[4] = STRING_decimal_to_ascii(d3);
	}
	else if (value < 100000) {
		// Format = uu.dd
		u1 = (value) / (10000);
		u2 = (value - (u1 * 10000)) / (1000);
		d1 = (value - (u1 * 10000) - (u2 * 1000)) / (100);
		d2 = (value - (u1 * 10000) - (u2 * 1000) - (d1 * 100)) / (10);
		value_string[0] = STRING_decimal_to_ascii(u1);
		value_string[1] = STRING_decimal_to_ascii(u2);
		value_string[2] = '.';
		value_string[3] = STRING_decimal_to_ascii(d1);
		value_string[4] = STRING_decimal_to_ascii(d2);
	}
	else if (value < 1000000) {
		// Format = uuu.d
		u1 = (value) / (100000);
		u2 = (value - (u1 * 100000)) / (10000);
		u3 = (value - (u1 * 100000) - (u2 * 10000)) / (1000);
		d1 = (value - (u1 * 100000) - (u2 * 10000) - (u3 * 1000)) / (100);
		value_string[0] = STRING_decimal_to_ascii(u1);
		value_string[1] = STRING_decimal_to_ascii(u2);
		value_string[2] = STRING_decimal_to_ascii(u3);
		value_string[3] = '.';
		value_string[4] = STRING_decimal_to_ascii(d1);
	}
	else {
		// Format = uuuuu
		u1 = (value) / (10000000);
		u2 = (value - (u1 * 10000000)) / (1000000);
		u3 = (value - (u1 * 10000000) - (u2 * 1000000)) / (100000);
		u4 = (value - (u1 * 10000000) - (u2 * 1000000) - (u3 * 100000)) / (10000);
		u5 = (value - (u1 * 10000000) - (u2 * 1000000) - (u3 * 100000) - (u4 * 10000)) / (1000);
		value_string[0] = STRING_decimal_to_ascii(u1);
		value_string[1] = STRING_decimal_to_ascii(u2);
		value_string[2] = STRING_decimal_to_ascii(u3);
		value_string[3] = STRING_decimal_to_ascii(u4);
		value_string[4] = STRING_decimal_to_ascii(u5);
	}
	// Print value.
	LCD_print(row, column, value_string, 5);
}
