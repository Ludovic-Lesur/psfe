/*
 * main.c
 *
 *  Created on: 29 dec. 2018
 *      Author: Ludo
 */

// Peripherals.
#include "adc.h"
#include "gpio.h"
#include "iwdg.h"
#include "lptim.h"
#include "lpuart.h"
#include "mode.h"
#include "nvic.h"
#include "rcc.h"
#include "tim.h"
#include "usart.h"
// Components.
#include "lcd.h"
#include "td1208.h"
#include "trcs.h"
// Applicative.
#include "psfe.h"

/*** MAIN function ***/

/* MAIN FUNCTION.
 * @param:	None.
 * @return:	None.
 */
int main(void) {
	// Init memory.
	NVIC_Init();
	// Init GPIOs.
	GPIO_Init();
	// Init clock.
	RCC_Init();
	RCC_SwitchToHsi();
	RCC_EnableLsi();
	// Init watchdog.
#ifndef DEBUG
	IWDG_Init();
#endif
	IWDG_Reload();
	// Get effective LSI frequency.
	unsigned int lsi_frequency_hz = 0;
	TIM21_Init();
	TIM21_GetLsiFrequency(&lsi_frequency_hz);
	TIM21_Disable();
	// Init peripherals.
	TIM2_Init(PSFE_ADC_CONVERSION_PERIOD_MS);
	TIM22_Init(PSFE_LCD_UART_PRINT_PERIOD_MS);
	LPTIM1_Init(lsi_frequency_hz);
	ADC1_Init();
	USART2_Init();
	LPUART1_Init();
	// Init components.
	LCD_Init();
	TRCS_Init();
	TD1208_Init();
	// Init applicative layer.
	PSFE_Init();
	TIM2_Start();
	// Main loop.
	while (1) {
		PSFE_Task();
		IWDG_Reload();
	}
	return 0;
}
