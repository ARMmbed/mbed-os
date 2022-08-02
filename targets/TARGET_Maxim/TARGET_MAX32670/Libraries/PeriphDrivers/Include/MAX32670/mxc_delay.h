/**
 * @file    mxc_delay.h
 * @brief   Asynchronous delay routines based on the SysTick Timer.
*/

/* ****************************************************************************
 * Copyright (C) 2022 Maxim Integrated Products, Inc., All Rights Reserved.
 *
 * Permission is hereby granted, free of charge, to any person obtaining a
 * copy of this software and associated documentation files (the "Software"),
 * to deal in the Software without restriction, including without limitation
 * the rights to use, copy, modify, merge, publish, distribute, sublicense,
 * and/or sell copies of the Software, and to permit persons to whom the
 * Software is furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included
 * in all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS
 * OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
 * MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
 * IN NO EVENT SHALL MAXIM INTEGRATED BE LIABLE FOR ANY CLAIM, DAMAGES
 * OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE,
 * ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR
 * OTHER DEALINGS IN THE SOFTWARE.
 *
 * Except as contained in this notice, the name of Maxim Integrated
 * Products, Inc. shall not be used except as stated in the Maxim Integrated
 * Products, Inc. Branding Policy.
 *
 * The mere transfer of this software does not imply any licenses
 * of trade secrets, proprietary technology, copyrights, patents,
 * trademarks, maskwork rights, or any other form of intellectual
 * property whatsoever. Maxim Integrated Products, Inc. retains all
 * ownership rights.
 *
 *************************************************************************** */

/* Define to prevent redundant inclusion */
#ifndef _DELAY_H_
#define _DELAY_H_

/**
 * @ingroup devicelibs
 * @defgroup    MXC_delay Delay Utility Functions
 * @brief       Asynchronous delay routines based on the SysTick Timer
 * @{
 */

/***** Definitions *****/
/**
 * Macro used to specify a microsecond timing parameter in seconds.
 * \code
 * x = SEC(3) // 3 seconds -> x = 3,000,000
 * \endcode
 */
#define MXC_DELAY_SEC(s)            (((unsigned long)s) * 1000000UL)
/**
 * Macro used to specify a microsecond timing parameter in milliseconds.
 * \code
 * x = MSEC(3) // 3ms -> x = 3,000
 * \endcode
 */
#define MXC_DELAY_MSEC(ms)          (ms * 1000UL)
/**
 * Macro used to specify a microsecond timing parameter.
 * \code
 * x = USEC(3) // 3us -> x = 3
 * \endcode
 */
#define MXC_DELAY_USEC(us)          (us)

/**
 * @brief   The callback routine used by MXC_DelayAsync() when the delay is complete
 *          or aborted early.
 *
 * @param   result      See \ref MXC_Error_Codes for the list of error codes.
 */
typedef void (*mxc_delay_complete_t) (int result);

/***** Function Prototypes *****/

/**
 * @brief      Blocks and delays for the specified number of microseconds.
 * @details    Uses the SysTick to create the requested delay. If the SysTick is
 *             running, the current settings will be used. If the SysTick is not
 *             running, it will be started.
 * @param      us    microseconds to delay
 * @return     #E_NO_ERROR if no errors, @ref MXC_Error_Codes "error" if unsuccessful.
 */
int MXC_Delay (unsigned long us);

/**
 * @brief      Starts a non-blocking delay for the specified number of
 *             microseconds.
 * @details    Uses the SysTick to time the requested delay. If the SysTick is
 *             running, the current settings will be used. If the SysTick is not
 *             running, it will be started.
 * @note       MXC_Delay_handler() must be called from the SysTick interrupt service
 *             routine or at a rate greater than the SysTick overflow rate.
 * @param      us    microseconds to delay
 * @param      callback  Function pointer to function called upon delay completion.
 * @return     #E_NO_ERROR if no errors, #E_BUSY if currently servicing another
 *             delay request.
 */
int MXC_DelayAsync (unsigned long us, mxc_delay_complete_t callback);

/**
 * @brief      Returns the status of a non-blocking delay request
 * @pre        Start the asynchronous delay by calling MXC_Delay_start().
 * @return     #E_BUSY until the requested delay time has expired.
 */
int MXC_DelayCheck (void);

/**
 * @brief      Stops an asynchronous delay previously started.
 * @pre        Start the asynchronous delay by calling MXC_Delay_start().
 */
void MXC_DelayAbort (void);

/**
 * @brief      Processes the delay interrupt.
 * @details    This function must be called from the SysTick IRQ or polled at a
 *             rate greater than the SysTick overflow rate.
 */
void MXC_DelayHandler (void);

/**@} end of group MXC_delay */

#endif /* _DELAY_H_ */
