/*
 * rcc.h
 *
 *  Created on: 29 dec. 2018
 *      Author: Ludo
 */

#ifndef RCC_H
#define RCC_H

/*** RCC macros ***/

#define RCC_LSI_FREQUENCY_HZ	38000
#define RCC_HSI_FREQUENCY_KHZ	16000

/*** RCC functions ***/

void RCC_init(void);
unsigned int RCC_get_sysclk_khz(void);
unsigned char RCC_switch_to_hsi(void);
unsigned char RCC_enable_lsi(void);

#endif /* RCC_H */
