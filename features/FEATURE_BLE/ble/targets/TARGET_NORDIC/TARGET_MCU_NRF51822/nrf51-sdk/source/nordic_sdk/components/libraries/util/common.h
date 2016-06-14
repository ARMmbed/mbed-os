 /*
 * Copyright (c) Nordic Semiconductor ASA
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without modification,
 * are permitted provided that the following conditions are met:
 *
 *   1. Redistributions of source code must retain the above copyright notice, this
 *   list of conditions and the following disclaimer.
 *
 *   2. Redistributions in binary form must reproduce the above copyright notice, this
 *   list of conditions and the following disclaimer in the documentation and/or
 *   other materials provided with the distribution.
 *
 *   3. Neither the name of Nordic Semiconductor ASA nor the names of other
 *   contributors to this software may be used to endorse or promote products
 *   derived from this software without specific prior written permission.
 *
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR
 * ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON
 * ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
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
