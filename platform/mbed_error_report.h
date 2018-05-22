
/** \addtogroup platform */
/** @{*/
/* mbed Microcontroller Library
 * Copyright (c) 2006-2013 ARM Limited
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
#ifndef MBED_ERROR_REPORT_H
#define MBED_ERROR_REPORT_H

#ifdef __cplusplus
extern "C" {
#endif
    
#ifndef MBED_CONF_ERROR_REPORT_INTERFACE
#define MBED_CONF_ERROR_REPORT_INTERFACE            DEVICE_SERIAL
#endif       

#ifdef MBED_CONF_RTOS_PRESENT 
#include "rtx_os.h"

/* Prints thread info from a list
 * @param  threads                    Pointer to the list of osRtxThread_t structs for which the thread info will be printed
 *
 */
void print_threads_info(osRtxThread_t *threads);

/* Prints info of a thread(using osRtxThread_t struct)
 * @param  thread                     Pointer to the osRtxThread_t struct for which the thread info will be printed
 *
 */
void print_thread(osRtxThread_t *thread);
#endif

/* Routine to report the error 
 * @param  error_ctx                   This is the error context struct containing the error info
 * @param  error_msg                   Error message to be used when reporting the error   
 *
 */
void mbed_report_error(mbed_error_ctx *error_ctx, char *error_msg);
    
/* Limited print functionality which prints the string out to 
   stdout/uart without using stdlib by directly calling serial-api 
   and also uses less resources 
   The fmtstr contains the format string for printing and for every %
   found in that it fetches a uint32 value from values buffer
   and prints it in hex format.
 * @param  fmstr                   format string to be used, currently we support %x(hex) %d(decimal) %s(string) formatters only.
 * @param  values                  This should be a pointer to array of values used for printing corresponding to the format specifiers(%x,%d,%s)) mentioned          
 *
 */
void mbed_error_print(char *fmtstr, uint32_t *values);
    
/* Initializes the data structures and interfaces for printing the error output
 *
 */
void mbed_error_init(void);

/* Routine which putc into serial/itm interface
 * @param  ch                   character to print
 *
 */
void mbed_error_putc(char ch);

#ifdef __cplusplus
}
#endif

#endif

/** @}*/
