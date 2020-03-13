/*
 * Copyright (c) 2020, Nuvoton Technology Corporation
 *
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
#ifndef __PIN_NAMES_COMMON_H__
#define __PIN_NAMES_COMMON_H__

#include "cmsis.h"
#include "partition_M2351.h"

#ifdef __cplusplus
extern "C" {
#endif

#define NU_PININDEX_Pos                             0
#define NU_PININDEX_Msk                             (0xFFul << NU_PININDEX_Pos)
#define NU_PINPORT_Pos                              8
#define NU_PINPORT_Msk                              (0xFul << NU_PINPORT_Pos)
#define NU_PIN_MODINDEX_Pos                         12
#define NU_PIN_MODINDEX_Msk                         (0xFul << NU_PIN_MODINDEX_Pos)
#define NU_PIN_BIND_Pos                             16
#define NU_PIN_BIND_Msk                             (0x1ul << NU_PIN_BIND_Pos)

#define NU_PININDEX(PINNAME)                        (((unsigned int)(PINNAME) & NU_PININDEX_Msk) >> NU_PININDEX_Pos)
#define NU_PINPORT(PINNAME)                         (((unsigned int)(PINNAME) & NU_PINPORT_Msk) >> NU_PINPORT_Pos)
#define NU_PIN_BIND(PINNAME)                        (((unsigned int)(PINNAME) & NU_PIN_BIND_Msk) >> NU_PIN_BIND_Pos)
#define NU_PIN_MODINDEX(PINNAME)                    (((unsigned int)(PINNAME) & NU_PIN_MODINDEX_Msk) >> NU_PIN_MODINDEX_Pos)
#define NU_PINNAME(PORT, PIN)                       ((((unsigned int) (PORT)) << (NU_PINPORT_Pos)) | (((unsigned int) (PIN)) << NU_PININDEX_Pos))
#define NU_PINNAME_BIND(PINNAME, modname)           ((PinName) NU_PINNAME_BIND_(NU_PINPORT(PINNAME), NU_PININDEX(PINNAME), modname))
#define NU_PINNAME_BIND_(PORT, PIN, modname)        ((((unsigned int)(PORT)) << NU_PINPORT_Pos) | (((unsigned int)(PIN)) << NU_PININDEX_Pos) | (NU_MODINDEX(modname) << NU_PIN_MODINDEX_Pos) | NU_PIN_BIND_Msk)

/* Revise NU_PORT_BASE to be TrustZone-aware */
__STATIC_INLINE GPIO_T *NU_PORT_BASE(uint32_t PORT)
{
    uint32_t port_base = ((uint32_t) GPIOA_BASE) + 0x40 * PORT;

#if defined(SCU_INIT_IONSSET_VAL)
    if (SCU_INIT_IONSSET_VAL & (1 << (PORT + 0))) {
        port_base += NS_OFFSET;
    } 
#endif

    return ((GPIO_T *) port_base);
}

#define NU_MFP_POS(pin)                             ((pin % 8) * 4)
#define NU_MFP_MSK(pin)                             (0xful << NU_MFP_POS(pin))

/* TrustZone-aware version of GPIO_PIN_DATA to get GPIO pin data */
__STATIC_INLINE uint32_t NU_GET_GPIO_PIN_DATA(uint32_t PORT, uint32_t PIN)
{
#if defined(SCU_INIT_IONSSET_VAL)
    if (SCU_INIT_IONSSET_VAL & (1 << (PORT + 0))) {
        return GPIO_PIN_DATA_NS(PORT, PIN);
    } 
#endif

    return GPIO_PIN_DATA_S(PORT, PIN);
}

/* TrustZone-aware version of GPIO_PIN_DATA to set GPIO pin data */
__STATIC_INLINE void NU_SET_GPIO_PIN_DATA(uint32_t PORT, uint32_t PIN, uint32_t VALUE)
{
#if defined(SCU_INIT_IONSSET_VAL)
    if (SCU_INIT_IONSSET_VAL & (1 << (PORT + 0))) {
        GPIO_PIN_DATA_NS(PORT, PIN) = VALUE;
        return;
    }
#endif

    GPIO_PIN_DATA_S(PORT, PIN) = VALUE;
}

// LEGACY
#define NU_PINNAME_TO_PIN(PINNAME)                  NU_PININDEX(PINNAME)
#define NU_PINNAME_TO_PORT(PINNAME)                 NU_PINPORT(PINNAME)
#define NU_PINNAME_TO_MODSUBINDEX(PINNAME)          NU_PIN_MODINDEX(PINNAME)
#define NU_PORT_N_PIN_TO_PINNAME(PORT, PIN)         NU_PINNAME((PORT), (PIN))

typedef enum {
    PIN_INPUT,
    PIN_OUTPUT
} PinDirection;

typedef enum {
    /* Input pull mode */
    PullNone = 0,
    PullDown,
    PullUp,

    /* I/O mode */
    InputOnly,
    PushPullOutput,
    OpenDrain,
    QuasiBidirectional,

    /* Default input pull mode */
    PullDefault = PullUp
} PinMode;

#ifdef __cplusplus
}
#endif

#endif // __PIN_NAMES_COMMON_H__
