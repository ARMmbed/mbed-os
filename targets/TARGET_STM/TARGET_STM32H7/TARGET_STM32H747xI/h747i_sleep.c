/* mbed Microcontroller Library
 *******************************************************************************
 * <h2><center>&copy; Copyright (c) 2019 STMicroelectronics.
 * All rights reserved.</center></h2>
 * SPDX-License-Identifier: BSD-3-Clause
 *
 * This software component is licensed by ST under BSD 3-Clause license,
 * the "License"; You may not use this file except in compliance with the
 * License. You may obtain a copy of the License at:
 *                        opensource.org/licenses/BSD-3-Clause
 *******************************************************************************
 */
#if DEVICE_SLEEP

/*  Deepsleep temporarily not supported on STM32H747I
 *  wrap it to sleep
 */
void hal_deepsleep(void)
{
    hal_sleep();
}

#endif
