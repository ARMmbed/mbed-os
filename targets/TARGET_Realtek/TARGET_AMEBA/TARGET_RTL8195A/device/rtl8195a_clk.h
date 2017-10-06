/*
 * Copyright (c) 2013-2016 Realtek Semiconductor Corp.
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
#ifndef MBED_RTL8195A_CLK_H
#define MBED_RTL8195A_CLK_H

#define PLATFORM_CLK             (200000000UL/6*5) // 166MHz
//#define SYSTEM_CLK               PLATFORM_CLK
//#define TIMER_CLK                (32*1000)

#define __BUILD_CCTRL_MACRO(name,ctrl)                           \
static inline void                                               \
__##name##_ACTCK_Enable(void)                                    \
{                                                                \
    __RTK_PERI_SETBIT(ctrl, BIT_ACTCK_##name);                   \
}                                                                \
static inline void                                               \
__##name##_SLPCK_Enable(void)                                    \
{                                                                \
    __RTK_PERI_SETBIT(ctrl, BIT_SLPCK_##name);                   \
}                                                                \
static inline void                                               \
__##name##_ACTCK_Disable(void)                                   \
{                                                                \
    __RTK_PERI_CLRBIT(ctrl, BIT_ACTCK_##name);                   \
}                                                                \
static inline void                                               \
__##name##_SLPCK_Disable(void)                                   \
{                                                                \
    __RTK_PERI_CLRBIT(ctrl, BIT_SLPCK_##name);                   \
}                                                                \

//enum clk_idx {
//    CLK_ANACK = 0,
//    CLK_A33CK = 1,
//};

// Interface to ROM functions
extern __longcall uint32_t HalGetCpuClk(void);

#define __CLK_GetCPUClk    HalGetCpuClk

// Interface for HAL functions
static inline uint32_t CLK_GetCPUClk(void)
{
    return __CLK_GetCPUClk();
}

extern void CLK_BackupCPUClk(void);
extern void CLK_ReFillCPUClk(void);
extern uint32_t CLK_Calculate(uint8_t clksel);

#endif
