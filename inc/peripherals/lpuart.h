/*
 * lpuart.h
 *
 *  Created on: 9 juil. 2019
 *      Author: Ludo
 */

#ifndef __LPUART_H__
#define __LPUART_H__

#include "lptim.h"

/*** LPUART structures ***/

typedef enum {
	LPUART_SUCCESS = 0,
	LPUART_ERROR_TX_TIMEOUT,
	LPUART_ERROR_STRING_LENGTH,
	LPUART_ERROR_BASE_LAST = 0x0100
} LPUART_status_t;

/*** LPUART functions ***/

void LPUART1_init(void);
LPUART_status_t LPUART1_send_string(char* tx_string);

#define LPUART1_status_check(error_base) { if (lpuart_status != LPUART_SUCCESS) { status = error_base + lpuart_status; goto errors; }}
#define LPUART1_error_check() { ERROR_status_check(lpuart_status, LPUART_SUCCESS, ERROR_BASE_LPUART1); }
#define LPUART1_error_check_print() { ERROR_status_check_print(lpuart_status, LPUART_SUCCESS, ERROR_BASE_LPUART1); }

#endif /* __LPUART_H__ */
