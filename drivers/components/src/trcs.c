/*
 * trcs.c
 *
 *  Created on: 30 dec. 2018
 *      Author: Ludo
 */

#include "trcs.h"

#include "analog.h"
#include "error.h"
#include "exti.h"
#include "gpio.h"
#include "gpio_mapping.h"
#include "math.h"
#include "mode.h"
#include "mode.h"
#include "nvic.h"

/*** TRCS local macros ***/

#define TRCS_ADC_SAMPLE_BUFFER_LENGTH		10

#define TRCS_RANGE_RECOVERY_DELAY_MS		100		// Recovery time when switching ranges.
#define TRCS_RANGE_STABILIZATION_DELAY_MS	100		// Delay applied after switching operation.

#define TRCS_RANGE_UP_THRESHOLD_MV		    3100	// Range will be increased if TRCS output voltage is higher.
#define TRCS_RANGE_DOWN_THRESHOLD_MV	    25		// Range will be decreased if TRCS output voltage is lower.

#define TRCS_LT6105_VOLTAGE_GAIN			59		// LT6105 with 100R and 5.9k resistors.

#define TRCS_NUMER_OF_ACTIVE_RANGES			(TRCS_RANGE_LAST - 1)

/*** TRCS local structures ***/

/*******************************************************************/
typedef struct {
	const GPIO_pin_t* gpio;
	const uint32_t resistor_mohm;
	uint8_t switch_request_pending;
	uint32_t switch_timer_ms;
} TRCS_range_info_t;

/*******************************************************************/
typedef struct {
	TRCS_range_t current_range;
	TRCS_range_t previous_range;
	uint8_t switch_pending;
	int32_t iout_mv_buf[TRCS_ADC_SAMPLE_BUFFER_LENGTH];
	uint8_t iout_mv_buf_idx;
	int32_t iout_mv;
	uint8_t bypass_flag;
} TRCS_context_t;

/*** TRCS local global variables ***/

static volatile TRCS_context_t trcs_ctx;

static const uint32_t trcs_low_range_resistor_mohms[PSFE_NUMBER_OF_BOARDS] = {52000, 52000, 53000, 51000, 53000, 53000, 50000, 53000, 52000, 50000};
static const uint32_t trcs_middle_range_resistor_mohms[PSFE_NUMBER_OF_BOARDS] = {510, 510, 510, 510, 530, 520, 500, 540, 520, 500};
static const uint32_t trcs_high_range_resistor_mohms[PSFE_NUMBER_OF_BOARDS] = 	{6, 5, 6, 6, 6, 6, 5, 7, 6, 5};

static TRCS_range_info_t trcs_range_table[TRCS_NUMER_OF_ACTIVE_RANGES] = {
	{&GPIO_TRCS_RANGE_LOW, trcs_low_range_resistor_mohms[PSFE_BOARD_INDEX], 0, 0},
	{&GPIO_TRCS_RANGE_MIDDLE, trcs_middle_range_resistor_mohms[PSFE_BOARD_INDEX], 0, 0},
	{&GPIO_TRCS_RANGE_HIGH, trcs_high_range_resistor_mohms[PSFE_BOARD_INDEX], 0, 0}
};

/*** TRCS local functions ***/

/*******************************************************************/
static TRCS_status_t _TRCS_update_adc_data(void) {
	// Local variables.
	TRCS_status_t status = TRCS_SUCCESS;
	ANALOG_status_t analog_status = ANALOG_SUCCESS;
	MATH_status_t math_status = MATH_SUCCESS;
	// Get raw data in mV.
	analog_status = ANALOG_convert_channel(ANALOG_CHANNEL_IOUT_MV, (int32_t*) &trcs_ctx.iout_mv_buf[trcs_ctx.iout_mv_buf_idx]);
	ANALOG_exit_error(TRCS_ERROR_BASE_ADC);
	// Update average.
	math_status = MATH_average((int32_t*) trcs_ctx.iout_mv_buf, TRCS_ADC_SAMPLE_BUFFER_LENGTH, (int32_t*) &trcs_ctx.iout_mv);
	MATH_exit_error(TRCS_ERROR_BASE_MATH);
	// Manage index.
	trcs_ctx.iout_mv_buf_idx++;
	if (trcs_ctx.iout_mv_buf_idx >= TRCS_ADC_SAMPLE_BUFFER_LENGTH) {
		trcs_ctx.iout_mv_buf_idx = 0;
	}
errors:
	return status;
}

/*** TRCS functions ***/

/*******************************************************************/
void TRCS_init(void) {
	// Local variables.
	uint8_t idx = 0;
	// Init GPIOs.
	GPIO_configure(&GPIO_TRCS_RANGE_LOW, GPIO_MODE_OUTPUT, GPIO_TYPE_PUSH_PULL, GPIO_SPEED_HIGH, GPIO_PULL_NONE);
	GPIO_configure(&GPIO_TRCS_RANGE_MIDDLE, GPIO_MODE_OUTPUT, GPIO_TYPE_PUSH_PULL, GPIO_SPEED_HIGH, GPIO_PULL_NONE);
	GPIO_configure(&GPIO_TRCS_RANGE_HIGH, GPIO_MODE_OUTPUT, GPIO_TYPE_PUSH_PULL, GPIO_SPEED_HIGH, GPIO_PULL_NONE);
	GPIO_configure(&GPIO_TRCS_BYPASS, GPIO_MODE_INPUT, GPIO_TYPE_PUSH_PULL, GPIO_SPEED_LOW, GPIO_PULL_NONE);
	// Set board in high range by default to ensure power continuity.
	GPIO_write(&GPIO_TRCS_RANGE_HIGH, 1);
	GPIO_write(&GPIO_TRCS_RANGE_MIDDLE, 0);
	GPIO_write(&GPIO_TRCS_RANGE_LOW, 0);
	// Init context.
	trcs_ctx.current_range = TRCS_RANGE_HIGH;
	trcs_ctx.previous_range = TRCS_RANGE_HIGH;
	trcs_ctx.switch_pending = 0;
	for (idx=0 ; idx<TRCS_ADC_SAMPLE_BUFFER_LENGTH ; idx++) trcs_ctx.iout_mv_buf[idx] = 0;
	trcs_ctx.iout_mv_buf_idx = 0;
	trcs_ctx.iout_mv = 0;
	trcs_ctx.bypass_flag = GPIO_read(&GPIO_TRCS_BYPASS);
}

/*******************************************************************/
void TRCS_de_init(void) {
	// Disable all ranges.
	GPIO_write(&GPIO_TRCS_RANGE_HIGH, 0);
	GPIO_write(&GPIO_TRCS_RANGE_MIDDLE, 0);
	GPIO_write(&GPIO_TRCS_RANGE_LOW, 0);
}

/*******************************************************************/
TRCS_status_t TRCS_process(uint32_t process_period_ms) {
	// Local variables.
	TRCS_status_t status = TRCS_SUCCESS;
	uint8_t idx = 0;
	// Increment recovery timer.
	for (idx=0 ; idx<TRCS_NUMER_OF_ACTIVE_RANGES; idx++) {
		trcs_range_table[idx].switch_timer_ms += (process_period_ms * trcs_range_table[idx].switch_request_pending);
	}
	// Read bypass switch.
	trcs_ctx.bypass_flag = GPIO_read(&GPIO_TRCS_BYPASS);
	// Update ADC result and output current.
	status = _TRCS_update_adc_data();
	if (status != TRCS_SUCCESS) goto errors;
	// Compute range.
	if (trcs_ctx.bypass_flag != 0) {
		// Keep high range to ensure power continuity when bypass will be disabled.
		trcs_ctx.current_range = TRCS_RANGE_HIGH;
	}
	else {
		// Check ADC result.
		if (trcs_ctx.iout_mv > TRCS_RANGE_UP_THRESHOLD_MV) {
			trcs_ctx.current_range += (!trcs_ctx.switch_pending);
		}
		if ((trcs_ctx.iout_mv < TRCS_RANGE_DOWN_THRESHOLD_MV) && (trcs_ctx.current_range > 0)) {
			trcs_ctx.current_range -= (!trcs_ctx.switch_pending);
		}
	}
	// Check if GPIO control is required.
	if (trcs_ctx.current_range > TRCS_RANGE_HIGH) {
		// Disable all range (protection mode).
		TRCS_de_init();
		status = TRCS_ERROR_OVERFLOW;
		goto errors;
	}
	else {
		if (trcs_ctx.current_range != trcs_ctx.previous_range) {
			// Enable new range.
			GPIO_write(trcs_range_table[trcs_ctx.current_range].gpio, 1);
			// Start off timer.
			trcs_range_table[trcs_ctx.previous_range].switch_timer_ms = 0;
			trcs_range_table[trcs_ctx.previous_range].switch_request_pending = 1;
			trcs_ctx.switch_pending++;
		}
		for (idx=0 ; idx<TRCS_NUMER_OF_ACTIVE_RANGES ; idx++) {
			// Check recovery timer.
			if (trcs_range_table[idx].switch_timer_ms >= TRCS_RANGE_RECOVERY_DELAY_MS) {
				// Disable range.
				GPIO_write(trcs_range_table[idx].gpio, 0);
			}
			if (trcs_range_table[idx].switch_timer_ms >= (TRCS_RANGE_RECOVERY_DELAY_MS + TRCS_RANGE_STABILIZATION_DELAY_MS)) {
				// Stop timer.
				trcs_range_table[idx].switch_timer_ms = 0;
				trcs_range_table[idx].switch_request_pending = 0;
				trcs_ctx.switch_pending--;
			}
		}
	}
errors:
	// Update previous index.
	trcs_ctx.previous_range = trcs_ctx.current_range;
	return status;
}

/*******************************************************************/
uint8_t TRCS_get_bypass_switch_state(void) {
	return (trcs_ctx.bypass_flag);
}

/*******************************************************************/
TRCS_range_t TRCS_get_range(void) {
	return (trcs_ctx.current_range);
}

/*******************************************************************/
int32_t TRCS_get_iout(void) {
	// Local variables.
	int32_t iout_ua = 0;
	int64_t num = 0;
	int64_t den = 0;
	// Current conversion.
	num = (((int64_t) trcs_ctx.iout_mv) * ((int64_t) MATH_POWER_10[6]));
	den = (((int64_t) TRCS_LT6105_VOLTAGE_GAIN) * ((int64_t) trcs_range_table[trcs_ctx.current_range].resistor_mohm));
	// Compute IOUT.
	iout_ua = (den == 0) ? 0 : (uint32_t) ((num) / (den));
	return iout_ua;
}
