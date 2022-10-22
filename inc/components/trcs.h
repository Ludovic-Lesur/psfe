/*
 * trcs.h
 *
 *  Created on: 30 dec. 2018
 *      Author: Ludo
 */

#ifndef __TRCS_H__
#define __TRCS_H__

#include "adc.h"
#include "math.h"

/*** TRCS structures ***/

typedef enum {
	TRCS_SUCCESS = 0,
	TRCS_ERROR_NULL_PARAMETER,
	TRCS_ERROR_OVERFLOW,
	TRCS_ERROR_BASE_ADC = 0x0100,
	TRCS_ERROR_BASE_MATH = (TRCS_ERROR_BASE_ADC + ADC_ERROR_BASE_LAST),
	TRCS_ERROR_BASE_LAST = (TRCS_ERROR_BASE_MATH + MATH_ERROR_BASE_LAST)
} TRCS_status_t;

typedef enum {
	TRCS_RANGE_LOW = 0,
	TRCS_RANGE_MIDDLE,
	TRCS_RANGE_HIGH,
	TRCS_RANGE_NONE,
	TRCS_RANGE_LAST
} TRCS_range_t;

/*** TRCS functions ***/

void TRCS_init(void);
TRCS_status_t TRCS_task(void);
void TRCS_set_bypass_flag(uint8_t bypass_state);
TRCS_status_t TRCS_get_range(volatile TRCS_range_t* range);
TRCS_status_t TRCS_get_iout(volatile uint32_t* iout_ua);
void TRCS_off(void);

#define TRCS_status_check(error_base) { if (trcs_status != TRCS_SUCCESS) { status = error_base + trcs_status; goto errors; }}
#define TRCS_error_check() { ERROR_status_check(trcs_status, TRCS_SUCCESS, ERROR_BASE_TRCS); }
#define TRCS_error_check_print() { ERROR_status_check_print(trcs_status, TRCS_SUCCESS, ERROR_BASE_TRCS); }

#endif /* __TRCS_H__ */
