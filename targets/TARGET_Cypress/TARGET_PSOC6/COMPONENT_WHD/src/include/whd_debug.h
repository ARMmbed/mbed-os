/*
 * Copyright 2019 Cypress Semiconductor Corporation
 * SPDX-License-Identifier: Apache-2.0
 * 
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 * 
 *     http://www.apache.org/licenses/LICENSE-2.0
 * 
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include <stdio.h>
#include <stdio.h>
#include <string.h>
#include "whd.h"

#ifndef INCLUDED_WHD_DEBUG_H
#define INCLUDED_WHD_DEBUG_H

#ifdef __cplusplus
extern "C"
{
#endif

/******************************************************
*                      Macros
******************************************************/
#define WPRINT_ENABLE_WHD_ERROR
/* #define WPRINT_ENABLE_WHD_INFO */
/* #define WPRINT_ENABLE_WHD_DEBUG */

#define WHD_ENABLE_STATS
/*#define WHD_LOGGING_BUFFER_ENABLE*/

#if defined (__GNUC__)
#define WHD_TRIGGER_BREAKPOINT( ) do { __asm__ ("bkpt"); } while (0)

#elif defined (__IAR_SYSTEMS_ICC__)
#define WHD_TRIGGER_BREAKPOINT( ) do { __asm("bkpt 0"); } while (0)

#else
#define WHD_TRIGGER_BREAKPOINT( )
#endif

#ifdef WPRINT_ENABLE_ERROR
#define WPRINT_ERROR(args)                      do { WPRINT_MACRO(args); } while (0)
#define whd_assert(error_string, assertion) do { if (!(assertion) ){ WHD_TRIGGER_BREAKPOINT(); } } while (0)
#define whd_minor_assert(error_string, \
                         assertion)   do { if (!(assertion) ) WPRINT_MACRO( (error_string) ); } while (0)
#else
#define whd_assert(error_string, \
                   assertion)         do { if (!(assertion) ){ WPRINT_MACRO( (error_string) ); } } while (0)
#define whd_minor_assert(error_string, assertion)   do { (void)(assertion); } while (0)
#endif

/******************************************************
*             Print declarations
******************************************************/
/* IF MFG TEST is enabled then disable all LOGGING VIA UART as
 + * this interrupts communication between WL TOOL and MFG Test APP
 + * via STDIO UART causing Wrong Message Exchange and failure.
 + */
#ifdef WLAN_MFG_FIRMWARE
#define WPRINT_MACRO(args)
#else
#if defined(WHD_LOGGING_BUFFER_ENABLE)
#define WPRINT_MACRO(args) do { whd_buffer_printf args; } while (0 == 1)
#else
#define WPRINT_MACRO(args) do { printf args;} while (0 == 1)
#endif
#endif


/* WICED printing macros for Wiced Wi-Fi Driver*/
#ifdef WPRINT_ENABLE_WHD_INFO
#define WPRINT_WHD_INFO(args) WPRINT_MACRO(args)
#else
#define WPRINT_WHD_INFO(args)
#endif

#ifdef WPRINT_ENABLE_WHD_DEBUG
#define WPRINT_WHD_DEBUG(args) WPRINT_MACRO(args)
#else
#define WPRINT_WHD_DEBUG(args)
#endif

#ifdef WPRINT_ENABLE_WHD_ERROR
#define WPRINT_WHD_ERROR(args) WPRINT_MACRO(args);
#else
#define WPRINT_WHD_ERROR(args)
#endif

#ifdef WPRINT_ENABLE_WHD_DATA_LOG
#define WPRINT_WHD_DATA_LOG(args) WPRINT_MACRO(args)
#else
#define WPRINT_WHD_DATA_LOG(args)
#endif

#define WHD_STATS_INCREMENT_VARIABLE(whd_driver, var) \
    do { whd_driver->whd_stats.var++; } while (0)

#define WHD_STATS_CONDITIONAL_INCREMENT_VARIABLE(whd_driver, condition, var) \
    do { if (condition){ whd_driver->whd_stats.var++; }} while (0)

#if (defined(__GNUC__) && (__GNUC__ >= 6) )
#define __FUNCTION__ __func__
#endif


void whd_init_stats(whd_driver_t whd_driver);
void whd_print_logbuffer(void);


#ifdef WHD_LOGGING_BUFFER_ENABLE
#define LOGGING_BUFFER_SIZE (4 * 1024)
int whd_buffer_printf(const char *format, ...);

typedef struct
{
    uint32_t buffer_write;
    uint32_t buffer_read;
    char buffer[LOGGING_BUFFER_SIZE + 1];
    whd_bool_t roll_over;
    whd_bool_t over_write;
} whd_logging_t;
#else
#define whd_print_logbuffer()
#endif /* WHD_LOGGING_BUFFER_ENABLE */

#ifdef __cplusplus
} /* extern "C" */
#endif
#endif /* ifndef INCLUDED_WHD_DEBUG_H */

