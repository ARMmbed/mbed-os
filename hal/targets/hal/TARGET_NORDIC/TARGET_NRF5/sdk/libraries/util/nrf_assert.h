/*
 * Copyright (c) 2006 Nordic Semiconductor. All Rights Reserved.
 *
 * The information contained herein is confidential property of Nordic Semiconductor. The use,
 * copying, transfer or disclosure of such information is prohibited except by express written
 * agreement with Nordic Semiconductor.
 *
 */

/** @file
 * @brief Utilities for verifying program logic
 */

#ifndef NRF_ASSERT_H_
#define NRF_ASSERT_H_

#include <stdint.h>
#include "nrf.h"
#include "app_error.h"

#if defined(DEBUG_NRF) || defined(DEBUG_NRF_USER)

/** @brief Function for handling assertions.
 *
 *
 * @note
 * This function is called when an assertion has triggered.
 *
 *
 * @post
 * All hardware is put into an idle non-emitting state (in particular the radio is highly
 * important to switch off since the radio might be in a state that makes it send
 * packets continiously while a typical final infinit ASSERT loop is executing).
 *
 *
 * @param line_num The line number where the assertion is called
 * @param file_name Pointer to the file name
 */
void assert_nrf_callback(uint16_t line_num, const uint8_t *file_name);

/*lint -emacro(506, ASSERT) */ /* Suppress "Constant value Boolean */ 
/*lint -emacro(774, ASSERT) */ /* Suppress "Boolean within 'if' always evaluates to True" */ \

/** @brief Function for checking intended for production code.
 *
 * Check passes if "expr" evaluates to true. */
#define ASSERT(expr) \
if (expr)                                                                     \
{                                                                             \
}                                                                             \
else                                                                          \
{                                                                             \
    assert_nrf_callback((uint16_t)__LINE__, (uint8_t *)__FILE__);             \
}
#else
#define ASSERT(expr) //!< Assert empty when disabled
__WEAK void assert_nrf_callback(uint16_t line_num, const uint8_t *file_name);
#endif /* defined(DEBUG_NRF) || defined(DEBUG_NRF_USER) */

#endif /* NRF_ASSERT_H_ */
