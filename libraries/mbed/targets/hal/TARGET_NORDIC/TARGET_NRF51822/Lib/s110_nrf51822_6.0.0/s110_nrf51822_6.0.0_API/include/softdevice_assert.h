/*
 * Copyright (c) 2012 Nordic Semiconductor. All Rights Reserved.
 *
 * The information contained herein is confidential property of Nordic Semiconductor. The use,
 * copying, transfer or disclosure of such information is prohibited except by express written
 * agreement with Nordic Semiconductor.
 *
 */

/** @brief Utilities for verifying program logic
 */

#ifndef SOFTDEVICE_ASSERT_H_
#define SOFTDEVICE_ASSERT_H_

#include <stdint.h>

/** @brief This function handles assertions.
 *
 *
 * @note
 * This function is called when an assertion has triggered.
 * 
 *
 * @param line_num The line number where the assertion is called
 * @param file_name Pointer to the file name
 */
void assert_softdevice_callback(uint16_t line_num, const uint8_t *file_name);


/*lint -emacro(506, ASSERT) */ /* Suppress "Constant value Boolean */ 
/*lint -emacro(774, ASSERT) */ /* Suppress "Boolean within 'if' always evaluates to True" */ \
/** @brief Check intended for production code
 *
 * Check passes if "expr" evaluates to true. */
#define ASSERT(expr) \
if (expr)                                                                     \
{                                                                             \
}                                                                             \
else                                                                          \
{                                                                             \
  assert_softdevice_callback((uint16_t)__LINE__, (uint8_t *)__FILE__);        \
  /*lint -unreachable */                                                      \
}

#endif /* SOFTDEVICE_ASSERT_H_ */
