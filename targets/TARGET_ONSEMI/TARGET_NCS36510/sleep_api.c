/**
 *******************************************************************************
 * @file    sleep_api.c
 * @brief Implementation of a sleep functionality
 * @internal
 * @author  ON Semiconductor
 * $Rev:  $
 * $Date: $
 ******************************************************************************
 * Copyright 2016 Semiconductor Components Industries LLC (d/b/a “ON Semiconductor”).
 * All rights reserved.  This software and/or documentation is licensed by ON Semiconductor
 * under limited terms and conditions.  The terms and conditions pertaining to the software
 * and/or documentation are available at http://www.onsemi.com/site/pdf/ONSEMI_T&C.pdf
 * (“ON Semiconductor Standard Terms and Conditions of Sale, Section 8 Software”) and
 * if applicable the software license agreement.  Do not use this software and/or
 * documentation unless you have carefully read and you agree to the limited terms and
 * conditions.  By using this software and/or documentation, you agree to the limited
 * terms and conditions.

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

#include "fncs36510_sleep.h"
#include "sleep_api.h"
#include "cmsis_nvic.h"

void hal_sleep()
{
    fncs36510_sleep();
}

void hal_deepsleep()
{
    fncs36510_deepsleep();
}

#endif /* DEVICE_SLEEP */
