/**
 ******************************************************************************
 * @file gpio.h
 * @brief (API) Public header of GPIO driver
 * @internal
 * @author   ON Semiconductor
 * $Rev: 3724 $
 * $Date: 2015-09-14 14:35:42 +0530 (Mon, 14 Sep 2015) $
 ******************************************************************************
 * @copyright (c) 2012 ON Semiconductor. All rights reserved.
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
 * @ingroup gpio
 *
 * @details
 *
 * <h1> General description </h1>
 * <p>
 * The APB GPIO is a configurable module allowing the use of 14 I/O lines.
 * Each line can be configured independently.
 * The GPIO module supports a wide variety of features concerning interrupts,
 * which can be triggered by a low level, high level, positive or negative edge,
 * or both edges.
 * </p>
 *
 */

#ifndef GPIO_H_
#define GPIO_H_

#ifdef __cplusplus
extern "C" {
#endif

#include "memory_map.h"
#include "gpio_map.h"
#include "pad_map.h"
#include "crossbar.h"
#include "clock.h"
#include "pad.h"


/** output configuration push/pull */
#define PAD_OUTCFG_PUSHPULL	                (uint8_t)0x00

/** output configuration open drain */
#define PAD_OOUTCFG_OPENDRAIN                   (uint8_t)0x01

/** no pull up nor pull down */
#define PAD_PULL_NONE	                        (uint8_t)0x01

/** pull down */
#define PAD_PULL_DOWN	                        (uint8_t)0x00

/** pull up */
#define PAD_PULL_UP		                (uint8_t)0x03

/* Number of DIO lines supported by NCS36510 */
#define NUMBER_OF_GPIO		        ((uint8_t)0x12)

/* All DIO lines set to 1 */
#define IO_ALL				((uint32_t)0x3FFFF)
#define IO_NONE				((uint32_t)0x00000)

/* Gpio handler */
void fGpioHandler(void);

#ifdef __cplusplus
}
#endif

#endif /* GPIO_H_ */
