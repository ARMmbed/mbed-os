 /* Copyright (c) 2009 Nordic Semiconductor. All Rights Reserved.
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

#ifndef COMMON_H
#define COMMON_H

/*lint ++flb "Enter library region" */

#include <stdbool.h>
#include <stdint.h>

/* @file
* @brief Common header file for generic macros and definitions
 *
 */

/*
 * GPIO glue macros, this can be used to define a pin number in source/header file and use that macro for pin
 * configuration using this expansion.
 * example:
 * #define RESET_PIN    8
 * NRF_GPIO->PINCNF(RESET_PIN) = XXX  ;  // Expanded NRF_GPIO->PIN_CNF[8] = XXX
 */
#define PINX_GLUE(x, y, z)       x##y##_##z            /*!< first level glue for pin macros */
#define PINCNF(p)                PINX_GLUE(PIN,p,CNF)  /*!< gpio configure pin number 'p' */
#define PINOUT(p)                PINX_GLUE(PIN,p,OUT)  /*!< gpio out pin number 'p' */

/*lint --flb "Leave library region" */
#endif
