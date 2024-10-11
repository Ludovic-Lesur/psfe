/*
 * print_hw.c
 *
 *  Created on: 29 sep. 2024
 *      Author: Ludo
 */

#include "terminal_hw.h"

#ifndef EMBEDDED_UTILS_DISABLE_FLAGS_FILE
#include "embedded_utils_flags.h"
#endif
#include "error.h"
#include "gpio_mapping.h"
#include "lpuart.h"
#include "mode.h"
#include "nvic_priority.h"
#include "types.h"

/*** TERMINAL HW local macros ***/

#define TERMINAL_LOG_BAUD_RATE  9600

/*** TERMINAL HW functions ***/

/*******************************************************************/
TERMINAL_status_t TERMINAL_HW_init(uint8_t instance, TERMINAL_HW_rx_irq_cb_t rx_irq_callback) {
    // Local variables.
    TERMINAL_status_t status = TERMINAL_SUCCESS;
    // Unused parameter.
    UNUSED(instance);
#ifdef DEBUG
    UNUSED(rx_irq_callback);
#else
    LPUART_status_t lpuart_status = LPUART_SUCCESS;
    LPUART_configuration_t lpuart_config;
    // Init print interface.
    lpuart_config.baud_rate = TERMINAL_LOG_BAUD_RATE;
    lpuart_config.nvic_priority = NVIC_PRIORITY_LOG;
    lpuart_config.rxne_callback = rx_irq_callback;
    lpuart_status = LPUART_init(&GPIO_LOG_LPUART, &lpuart_config);
    LPUART_exit_error(TERMINAL_ERROR_BASE_HW_INTERFACE);
    // Start reception if needed.
    if (rx_irq_callback != NULL) {
        lpuart_status = LPUART_enable_rx();
        LPUART_exit_error(TERMINAL_ERROR_BASE_HW_INTERFACE);
    }
errors:
#endif
    return status;
}

/*******************************************************************/
TERMINAL_status_t TERMINAL_HW_de_init(uint8_t instance) {
    // Local variables.
    TERMINAL_status_t status = TERMINAL_SUCCESS;
    // Unused parameter.
    UNUSED(instance);
#ifndef DEBUG
    LPUART_status_t lpuart_status = LPUART_SUCCESS;
    // Release print interface.
    lpuart_status = LPUART_de_init(&GPIO_LOG_LPUART);
    LPUART_exit_error(TERMINAL_ERROR_BASE_HW_INTERFACE);
errors:
#endif
    return status;
}

/*******************************************************************/
TERMINAL_status_t TERMINAL_HW_write(uint8_t instance, uint8_t* data, uint32_t data_size_bytes) {
    // Local variables.
    TERMINAL_status_t status = TERMINAL_SUCCESS;
    // Unused parameter.
    UNUSED(instance);
#ifndef DEBUG
    LPUART_status_t lpuart_status = LPUART_SUCCESS;
    // Write data.
    lpuart_status = LPUART_write(data, data_size_bytes);
    LPUART_exit_error(TERMINAL_ERROR_BASE_HW_INTERFACE);
errors:
#else
    UNUSED(data);
    UNUSED(data_size_bytes);
#endif
    return status;
}
