/* mbed Microcontroller Library
 *******************************************************************************
 * Copyright (c) 2016, STMicroelectronics
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 * 1. Redistributions of source code must retain the above copyright notice,
 *    this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright notice,
 *    this list of conditions and the following disclaimer in the documentation
 *    and/or other materials provided with the distribution.
 * 3. Neither the name of STMicroelectronics nor the names of its contributors
 *    may be used to endorse or promote products derived from this software
 *    without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
 * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
 * SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
 * CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
 * OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
 * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *******************************************************************************
 */
#ifndef MBED_PINNAMESTYPES_H
#define MBED_PINNAMESTYPES_H

#include "cmsis.h"

#ifdef __cplusplus
extern "C" {
#endif

#define GPIO_AF_NONE 0

/*  STM PIN data as used in pin_function is coded on 32 bits as below
 *   [2:0]  Function (like in MODER reg) : Input / Output / Alt / Analog
 *     [3]  Output Push-Pull / Open Drain (as in OTYPER reg)
 *   [5:4]  as in PUPDR reg: No Pull, Pull-up, Pull-Donc
 *   [9:6]  speed config (as in OSPEEDR)
 * [13:10]  Alternate Num (as in AFRL/AFRG reg)
 * [17:14]  Channel (Analog/Timer specific)
 *    [18]  Inverted (Analog/Timer specific)
 *    [19]  Analog ADC control - Only valid for specific families
 * [32:21]  Reserved
 */

#define STM_PIN_FUNCTION_MASK 0x07
#define STM_PIN_FUNCTION_SHIFT 0
#define STM_PIN_FUNCTION_BITS (STM_PIN_FUNCTION_MASK << STM_PIN_FUNCTION_SHIFT)

#define STM_PIN_OD_MASK 0x01
#define STM_PIN_OD_SHIFT 3
#define STM_PIN_OD_BITS (STM_PIN_OD_MASK << STM_PIN_OD_SHIFT)

#define STM_PIN_PUPD_MASK 0x03
#define STM_PIN_PUPD_SHIFT 4
#define STM_PIN_PUPD_BITS (STM_PIN_PUPD_MASK << STM_PIN_PUPD_SHIFT)

#define STM_PIN_SPEED_MASK 0x0F
#define STM_PIN_SPEED_SHIFT 6
#define STM_PIN_SPEED_BITS (STM_PIN_SPEED_MASK << STM_PIN_SPEED_SHIFT)

#define STM_PIN_AFNUM_MASK 0x0F
#define STM_PIN_AFNUM_SHIFT 10
#define STM_PIN_AFNUM_BITS (STM_PIN_AFNUM_MASK << STM_PIN_AFNUM_SHIFT)

#define STM_PIN_CHAN_MASK 0x1F
#define STM_PIN_CHAN_SHIFT 14
#define STM_PIN_CHANNEL_BIT (STM_PIN_CHAN_MASK << STM_PIN_CHAN_SHIFT)

#define STM_PIN_INV_MASK 0x01
#define STM_PIN_INV_SHIFT 19
#define STM_PIN_INV_BIT (STM_PIN_INV_MASK << STM_PIN_INV_SHIFT)

#define STM_PIN_AN_CTRL_MASK 0x01
#define STM_PIN_AN_CTRL_SHIFT 20
#define STM_PIN_ANALOG_CONTROL_BIT (STM_PIN_AN_CTRL_MASK << STM_PIN_AN_CTRL_SHIFT)

#define STM_PIN_FUNCTION(X)         (((X) >> STM_PIN_FUNCTION_SHIFT) & STM_PIN_FUNCTION_MASK)
#define STM_PIN_OD(X)               (((X) >> STM_PIN_OD_SHIFT) & STM_PIN_OD_MASK)
#define STM_PIN_PUPD(X)             (((X) >> STM_PIN_PUPD_SHIFT) & STM_PIN_PUPD_MASK)
#define STM_PIN_SPEED(X)            (((X) >> STM_PIN_SPEED_SHIFT) & STM_PIN_SPEED_MASK)
#define STM_PIN_AFNUM(X)            (((X) >> STM_PIN_AFNUM_SHIFT) & STM_PIN_AFNUM_MASK)
#define STM_PIN_CHANNEL(X)          (((X) >> STM_PIN_CHAN_SHIFT) & STM_PIN_CHAN_MASK)
#define STM_PIN_INVERTED(X)         (((X) >> STM_PIN_INV_SHIFT) & STM_PIN_INV_MASK)
#define STM_PIN_ANALOG_CONTROL(X)   (((X) >> STM_PIN_AN_CTRL_SHIFT) & STM_PIN_AN_CTRL_MASK)

#define STM_PIN_DEFINE(FUNC_OD, PUPD, AFNUM)  ((int)(FUNC_OD) |\
            ((STM_PIN_SPEED_MASK & STM_PIN_SPEED_MASK) << STM_PIN_SPEED_SHIFT) |\
            (((PUPD) & STM_PIN_PUPD_MASK) << STM_PIN_PUPD_SHIFT) |\
            (((AFNUM) & STM_PIN_AFNUM_MASK) << STM_PIN_AFNUM_SHIFT))

#define STM_PIN_DEFINE_SPEED(FUNC_OD, PUPD, AFNUM, SPEEDV)  ((int)(FUNC_OD) |\
            (((SPEEDV) & STM_PIN_SPEED_MASK) << STM_PIN_SPEED_SHIFT) |\
            (((PUPD) & STM_PIN_PUPD_MASK) << STM_PIN_PUPD_SHIFT) |\
            (((AFNUM) & STM_PIN_AFNUM_MASK) << STM_PIN_AFNUM_SHIFT))

#define STM_PIN_DEFINE_EXT(FUNC_OD, PUPD, AFNUM, CHAN, INV) \
            ((int)(FUNC_OD) |\
            ((STM_PIN_SPEED_MASK & STM_PIN_SPEED_MASK) << STM_PIN_SPEED_SHIFT) |\
            (((PUPD) & STM_PIN_PUPD_MASK) << STM_PIN_PUPD_SHIFT) |\
            (((AFNUM) & STM_PIN_AFNUM_MASK) << STM_PIN_AFNUM_SHIFT) |\
            (((CHAN) & STM_PIN_CHAN_MASK) << STM_PIN_CHAN_SHIFT) |\
            (((INV) & STM_PIN_INV_MASK) << STM_PIN_INV_SHIFT))

#define STM_PIN_DEFINE_SPEED_EXT(FUNC_OD, PUPD, AFNUM, CHAN, INV, SPEEDV) \
            ((int)(FUNC_OD) |\
            (((SPEEDV) & STM_PIN_SPEED_MASK) << STM_PIN_SPEED_SHIFT) |\
            (((PUPD) & STM_PIN_PUPD_MASK) << STM_PIN_PUPD_SHIFT) |\
            (((AFNUM) & STM_PIN_AFNUM_MASK) << STM_PIN_AFNUM_SHIFT) |\
            (((CHAN) & STM_PIN_CHAN_MASK) << STM_PIN_CHAN_SHIFT) |\
            (((INV) & STM_PIN_INV_MASK) << STM_PIN_INV_SHIFT))

/*
 * MACROS to support the legacy definition of PIN formats
 * The STM_MODE_ defines contain the function and the Push-pull/OpenDrain
 * configuration (legacy inheritance).
 */
#define STM_PIN_DATA(FUNC_OD, PUPD, AFNUM) \
            STM_PIN_DEFINE(FUNC_OD, PUPD, AFNUM)
#define STM_PIN_DATA_EXT(FUNC_OD, PUPD, AFNUM, CHANNEL, INVERTED) \
            STM_PIN_DEFINE_EXT(FUNC_OD, PUPD, AFNUM, CHANNEL, INVERTED)

typedef enum {
    STM_PIN_INPUT = 0,
    STM_PIN_OUTPUT = 1,
    STM_PIN_ALTERNATE = 2,
    STM_PIN_ANALOG = 3,
} StmPinFunction;

#define STM_MODE_INPUT               (STM_PIN_INPUT)
#define STM_MODE_OUTPUT_PP           (STM_PIN_OUTPUT)
#define STM_MODE_OUTPUT_OD           (STM_PIN_OUTPUT | STM_PIN_OD_BITS)
#define STM_MODE_AF_PP               (STM_PIN_ALTERNATE)
#define STM_MODE_AF_OD               (STM_PIN_ALTERNATE | STM_PIN_OD_BITS)
#define STM_MODE_ANALOG              (STM_PIN_ANALOG)
#define STM_MODE_ANALOG_ADC_CONTROL  (STM_PIN_ANALOG | STM_PIN_ANALOG_CONTROL_BIT)

// High nibble = port number (0=A, 1=B, 2=C, 3=D, 4=E, 5=F, 6=G, 7=H)
// Low nibble  = pin number
#define STM_PORT(X) (((uint32_t)(X) >> 4) & 0xF)
#define STM_PIN(X)  ((uint32_t)(X) & 0xF)

/*  Defines to be used by application */
typedef enum {
    PIN_INPUT = 0,
    PIN_OUTPUT
} PinDirection;

typedef enum {
    PullNone          = 0,
    PullUp            = 1,
    PullDown          = 2,
    OpenDrainPullUp   = 3,
    OpenDrainNoPull   = 4,
    OpenDrainPullDown = 5,
    PushPullNoPull    = PullNone,
    PushPullPullUp    = PullUp,
    PushPullPullDown  = PullDown,
    OpenDrain         = OpenDrainPullUp,
    PullDefault       = PullNone
} PinMode;

#ifdef __cplusplus
}
#endif

#endif
