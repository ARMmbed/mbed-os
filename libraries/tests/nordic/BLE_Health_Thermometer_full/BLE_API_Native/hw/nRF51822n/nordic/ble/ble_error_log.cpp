/* Copyright (c) 2012 Nordic Semiconductor. All Rights Reserved.
 *
 * The information contained herein is property of Nordic Semiconductor ASA.
 * Terms and conditions of usage are described in detail in NORDIC
 * SEMICONDUCTOR STANDARD SOFTWARE LICENSE AGREEMENT.
 *
 * Licensees are granted free, non-transferable use of the information. NO
 * WARRANTY of ANY KIND is provided. This heading must NOT be removed from
 * the file.
 *
 */

#include <string.h>
#include <stdint.h>
#include <stdbool.h>
#include <nrf51.h>
#include "ble_error_log.h"
#include "app_util.h"
#include "app_error.h"
#include "nrf_gpio.h"
#include "pstorage.h"


// Made static to avoid the error_log to go on the stack.
static ble_error_log_data_t   m_ble_error_log;            /**< . */
//lint -esym(526,__Vectors)
extern uint32_t             * __Vectors;                  /**< The initialization vector holds the address to __initial_sp that will be used when fetching the stack. */

static void fetch_stack(ble_error_log_data_t * error_log)
{
    // KTOWN: Temporarily removed 06022014
    /*
    uint32_t * p_stack;
    uint32_t * initial_sp;
    uint32_t   length;
  
    initial_sp = (uint32_t *) __Vectors;
    p_stack    = (uint32_t *) GET_SP();
  
    length = ((uint32_t) initial_sp) - ((uint32_t) p_stack);
    memcpy(error_log->stack_info, 
           p_stack, 
           (length > STACK_DUMP_LENGTH) ? STACK_DUMP_LENGTH : length);
    */
}

uint32_t ble_error_log_write(uint32_t err_code, const uint8_t * p_message, uint16_t line_number)
{
    m_ble_error_log.failure     = true;
    m_ble_error_log.err_code    = err_code;
    m_ble_error_log.line_number = line_number;

    strncpy((char *)m_ble_error_log.message, (const char *)p_message, ERROR_MESSAGE_LENGTH - 1);
    m_ble_error_log.message[ERROR_MESSAGE_LENGTH - 1] = '\0';

    fetch_stack(&m_ble_error_log);
    
    // Write to flash removed, to be redone.

    return NRF_SUCCESS;
}
