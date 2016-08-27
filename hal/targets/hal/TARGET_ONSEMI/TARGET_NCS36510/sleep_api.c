/**
 *******************************************************************************
 * @file    sleep_api.c
 * @brief Implementation of a sleep functionality
 * @internal
 * @author  ON Semiconductor
 * $Rev:  $
 * $Date: $
 ******************************************************************************
 * @copyright (c) 2015 ON Semiconductor. All rights reserved.
 * ON Semiconductor is supplying this software for use with ON Semiconductor
 * processor based microcontrollers only.
 *
 * THIS SOFTWARE IS PROVIDED "AS IS".  NO WARRANTIES, WHETHER EXPRESS, IMPLIED
 * OR STATUTORY, INCLUDING, BUT NOT LIMITED TO, IMPLIED WARRANTIES OF
 * MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE APPLY TO THIS SOFTWARE.
 * ON SEMICONDUCTOR SHALL NOT, IN ANY CIRCUMSTANCES, BE LIABLE FOR SPECIAL,
 * INCIDENTAL, OR CONSEQUENTIAL DAMAGES, FOR ANY REASON WHATSOEVER.
 * @endinternal
 *
 * @ingroup sleep
 *
 * @details
 * Sleep implementation TBD - Dummy function is fine for first release
 *
 */

#if DEVICE_SLEEP
#include "sleep.h"
#include "sleep_api.h"
#include "cmsis_nvic.h"

void mbed_enter_sleep(sleep_t *obj)
{
    /* Empty implementation, this will be implemented for mbed5.0 */
}

void mbed_exit_sleep(sleep_t *obj)
{
    (void)obj;
}
#endif /* DEVICE_SLEEP */