/*
 * atxfox.h
 *
 *  Created on: 25 july 2020
 *      Author: Ludo
 */

#ifndef __PSFE_H__
#define __PSFE_H__

#include "mode.h"
#include "types.h"

/*** PSFE macros ***/

#define PSFE_NUMBER_OF_BOARDS			10
#define PSFE_BOARD_INDEX				(PSFE_BOARD_NUMBER - 1)

#define PSFE_ADC_CONVERSION_PERIOD_MS	100
#define PSFE_LCD_UART_PRINT_PERIOD_MS	300

/*** PSFE global variables ***/

static const uint8_t psfe_trcs_number[PSFE_NUMBER_OF_BOARDS] = {5, 6, 1, 2, 7, 8, 9, 3, 4, 10};
static const uint8_t psfe_vout_voltage_divider_ratio[PSFE_NUMBER_OF_BOARDS] = {2, 2, 6, 6, 2, 2, 2, 6, 6, 2};

/*** PSFE functions ***/

void PSFE_init_hw(void);
void PSFE_init_context(void);
void PSFE_start(void);
void PSFE_task(void);
void PSFE_set_bypass_flag(uint8_t bypass_state);
void PSFE_adc_callback(void);
void PSFE_lcd_uart_callback(void);

#endif /* __PSFE_H__ */
