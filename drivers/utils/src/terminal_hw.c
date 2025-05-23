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
#include "error_base.h"
#include "mcu_mapping.h"
#include "lpuart.h"
#include "nvic_priority.h"
#include "psfe_flags.h"
#include "types.h"

#if (!(defined EMBEDDED_UTILS_TERMINAL_DRIVER_DISABLE) && (EMBEDDED_UTILS_TERMINAL_INSTANCES_NUMBER > 0))

/*** TERMINAL HW functions ***/

/*******************************************************************/
TERMINAL_status_t TERMINAL_HW_init(uint8_t instance, uint32_t baud_rate, TERMINAL_rx_irq_cb_t rx_irq_callback) {
    // Local variables.
    TERMINAL_status_t status = TERMINAL_SUCCESS;
    // Unused parameter.
    UNUSED(instance);
#if ((defined PSFE_SERIAL_MONITORING) && !(defined PSFE_MODE_DEBUG))
    LPUART_status_t lpuart_status = LPUART_SUCCESS;
    LPUART_configuration_t lpuart_config;
    // Init print interface.
    lpuart_config.baud_rate = baud_rate;
    lpuart_config.nvic_priority = NVIC_PRIORITY_SERIAL;
    lpuart_config.rxne_irq_callback = rx_irq_callback;
    lpuart_status = LPUART_init(&LPUART_GPIO_SERIAL, &lpuart_config);
    LPUART_exit_error(TERMINAL_ERROR_BASE_HW_INTERFACE);
errors:
#else
    UNUSED(baud_rate);
    UNUSED(rx_irq_callback);
#endif
    return status;
}

/*******************************************************************/
TERMINAL_status_t TERMINAL_HW_de_init(uint8_t instance) {
    // Local variables.
    TERMINAL_status_t status = TERMINAL_SUCCESS;
    // Unused parameter.
    UNUSED(instance);
#if ((defined PSFE_SERIAL_MONITORING) && !(defined PSFE_MODE_DEBUG))
    LPUART_status_t lpuart_status = LPUART_SUCCESS;
    // Release print interface.
    lpuart_status = LPUART_de_init(&LPUART_GPIO_SERIAL);
    LPUART_stack_error(ERROR_BASE_TERMINAL_SERIAL + TERMINAL_ERROR_BASE_HW_INTERFACE);
#endif
    return status;
}

/*******************************************************************/
TERMINAL_status_t TERMINAL_HW_enable_rx(uint8_t instance) {
    // Local variables.
    TERMINAL_status_t status = TERMINAL_SUCCESS;
    // Unused parameter.
    UNUSED(instance);
#if ((defined PSFE_SERIAL_MONITORING) && !(defined PSFE_MODE_DEBUG))
    LPUART_status_t lpuart_status = LPUART_SUCCESS;
    // Start receiver.
    lpuart_status = LPUART_enable_rx();
    LPUART_exit_error(TERMINAL_ERROR_BASE_HW_INTERFACE);
errors:
#endif
    return status;
}

/*******************************************************************/
TERMINAL_status_t TERMINAL_HW_disable_rx(uint8_t instance) {
    // Local variables.
    TERMINAL_status_t status = TERMINAL_SUCCESS;
    // Unused parameter.
    UNUSED(instance);
#if ((defined PSFE_SERIAL_MONITORING) && !(defined PSFE_MODE_DEBUG))
    LPUART_status_t lpuart_status = LPUART_SUCCESS;
    // Start receiver.
    lpuart_status = LPUART_disable_rx();
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
#if ((defined PSFE_SERIAL_MONITORING) && !(defined PSFE_MODE_DEBUG))
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

#ifdef EMBEDDED_UTILS_TERMINAL_MODE_BUS
/*******************************************************************/
TERMINAL_status_t TERMINAL_HW_set_destination_address(uint8_t instance, uint8_t destination_address) {
    // Local variables.
    TERMINAL_status_t status = TERMINAL_SUCCESS;
    /* To be implemented */
    UNUSED(instance);
    UNUSED(destination_address);
    return status;
}
#endif

#endif /* EMBEDDED_UTILS_TERMINAL_DRIVER_DISABLE */
