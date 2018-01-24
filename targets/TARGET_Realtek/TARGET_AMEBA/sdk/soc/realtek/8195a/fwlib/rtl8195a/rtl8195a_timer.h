/*******************************************************************************
 *Copyright (c) 2013-2016 Realtek Semiconductor Corp, All Rights Reserved
 * SPDX-License-Identifier: LicenseRef-PBL
 * 
 * Licensed under the Permissive Binary License, Version 1.0 (the "License"); 
 * you may not use this file except in compliance with the License.
 * 
 * You may obtain a copy of the License at https://www.mbed.com/licenses/PBL-1.0
 * 
 * See the License for the specific language governing permissions and limitations under the License.
 *******************************************************************************
 */

#ifndef _RTL8195A_TIMER_H_
#define _RTL8195A_TIMER_H_


#define TIMER_TICK_US               32

#define TIMER_LOAD_COUNT_OFF        0x00
#define TIMER_CURRENT_VAL_OFF       0x04
#define TIMER_CTL_REG_OFF           0x08
#define TIMER_EOI_OFF               0x0c
#define TIMER_INT_STATUS_OFF        0x10
#define TIMER_INTERVAL              0x14
#define TIMERS_INT_STATUS_OFF       0xa0
#define TIMERS_EOI_OFF              0xa4
#define TIMERS_RAW_INT_STATUS_OFF   0xa8
#define TIMERS_COMP_VER_OFF         0xac

#define MAX_TIMER_VECTOR_TABLE_NUM                  6

#define HAL_TIMER_READ32(addr)            (*((volatile u32*)(TIMER_REG_BASE + addr)))//HAL_READ32(TIMER_REG_BASE, addr)
#define HAL_TIMER_WRITE32(addr, value)    ((*((volatile u32*)(TIMER_REG_BASE + addr))) = value)//HAL_WRITE32(TIMER_REG_BASE, addr, value)
#define HAL_TIMER_READ16(addr)            (*((volatile u16*)(TIMER_REG_BASE + addr)))//HAL_READ16(TIMER_REG_BASE, addr)
#define HAL_TIMER_WRITE16(addr, value)    ((*((volatile u16*)(TIMER_REG_BASE + addr))) = value)//HAL_WRITE16(TIMER_REG_BASE, addr, value)
#define HAL_TIMER_READ8(addr)             (*((volatile u8*)(TIMER_REG_BASE + addr)))//HAL_READ8(TIMER_REG_BASE, addr)
#define HAL_TIMER_WRITE8(addr, value)     ((*((volatile u8*)(TIMER_REG_BASE + addr))) = value)//HAL_WRITE8(TIMER_REG_BASE, addr, value)

_LONG_CALL_ u32
HalGetTimerIdRtl8195a(
    IN  u32     *TimerID
);

_LONG_CALL_ BOOL
HalTimerInitRtl8195a(
    IN  VOID    *Data
);

_LONG_CALL_ u32
HalTimerReadCountRtl8195a(
    IN  u32     TimerId
);

_LONG_CALL_ VOID
HalTimerIrqClearRtl8195a(
    IN  u32 TimerId
);

_LONG_CALL_ VOID
HalTimerDisRtl8195a(
    IN  u32 TimerId
);

_LONG_CALL_ VOID
HalTimerEnRtl8195a(
    IN  u32 TimerId
);

_LONG_CALL_ VOID
HalTimerDumpRegRtl8195a(
    IN  u32 TimerId
);

// ROM Code patch
HAL_Status
HalTimerInitRtl8195a_Patch(
    IN  VOID    *Data
);

u32
HalTimerReadCountRtl8195a_Patch(
    IN  u32 TimerId
);

VOID
HalTimerReLoadRtl8195a_Patch(
    IN  u32 TimerId,
    IN  u32 LoadUs
);

u32
HalTimerReadCountRtl8195a_Patch(
    IN  u32 TimerId
);

VOID 
HalTimerSync(
    IN u32 TimerId
);

VOID
HalTimerIrqEnRtl8195a(
    IN  u32 TimerId
);

VOID
HalTimerIrqDisRtl8195a(
    IN  u32 TimerId
);

VOID
HalTimerClearIsrRtl8195a(
    IN  u32 TimerId
);

VOID
HalTimerEnRtl8195a_Patch(
    IN  u32 TimerId
);

VOID
HalTimerDisRtl8195a_Patch(
    IN  u32 TimerId
);

VOID
HalTimerDeInitRtl8195a_Patch(
    IN  VOID    *Data
);

#if defined(CONFIG_CHIP_C_CUT) || defined(CONFIG_CHIP_E_CUT)

__weak _LONG_CALL_
VOID
HalTimerIrq2To7HandleV02(
    IN  VOID    *Data
);

__weak _LONG_CALL_ROM_
HAL_Status
HalTimerIrqRegisterRtl8195aV02(
    IN  VOID    *Data
);

__weak _LONG_CALL_
HAL_Status
HalTimerInitRtl8195aV02(
    IN  VOID    *Data
);

__weak _LONG_CALL_
u32
HalTimerReadCountRtl8195aV02(
    IN  u32 TimerId
);

__weak _LONG_CALL_
VOID
HalTimerReLoadRtl8195aV02(
    IN  u32 TimerId,
    IN  u32 LoadUs
);

__weak _LONG_CALL_ROM_
HAL_Status
HalTimerIrqUnRegisterRtl8195aV02(
    IN  VOID    *Data
);

__weak _LONG_CALL_
VOID
HalTimerDeInitRtl8195aV02(
    IN  VOID    *Data
);

#endif  // end of "#ifdef CONFIG_CHIP_C_CUT"

#ifdef CONFIG_CHIP_E_CUT
_LONG_CALL_ VOID
HalTimerReLoadRtl8195a_V04(
    IN  u32 TimerId,
    IN  u32 LoadUs
);

_LONG_CALL_ HAL_Status
HalTimerInitRtl8195a_V04(
    IN  VOID    *Data
);
#endif  // #ifdef CONFIG_CHIP_E_CUT

// HAL functions wrapper
#ifndef CONFIG_RELEASE_BUILD_LIBRARIES
static __inline HAL_Status
HalTimerInit(
    IN  VOID    *Data
)
{
#ifdef CONFIG_CHIP_E_CUT
    return (HalTimerInitRtl8195a_V04(Data));
#else
    return (HalTimerInitRtl8195a_Patch(Data));
#endif
}

static __inline VOID
HalTimerEnable(
    IN  u32 TimerId
)
{
    HalTimerIrqEnRtl8195a(TimerId);
    HalTimerEnRtl8195a_Patch(TimerId);
}

static __inline VOID
HalTimerDisable(
    IN  u32 TimerId
)
{
    HalTimerDisRtl8195a_Patch(TimerId);
}

static __inline VOID
HalTimerClearIsr(
    IN  u32 TimerId
)
{
    HalTimerClearIsrRtl8195a(TimerId);
}

static __inline VOID
HalTimerReLoad(
    IN  u32 TimerId,
    IN  u32 LoadUs
)
{
#ifdef CONFIG_CHIP_E_CUT
    HalTimerReLoadRtl8195a_V04(TimerId, LoadUs);
#else
    HalTimerReLoadRtl8195a_Patch(TimerId, LoadUs);
#endif
}

#if defined(CONFIG_CHIP_A_CUT) || defined(CONFIG_CHIP_B_CUT)

static __inline VOID
HalTimerDeInit(
    IN  VOID    *Data
)
{
    HalTimerDeInitRtl8195a_Patch(Data);
}

#else

static __inline VOID
HalTimerDeInit(
    IN  VOID    *Data
)
{
    HalTimerDeInitRtl8195aV02(Data);
}

#endif      // end of "#ifndef CONFIG_CHIP_C_CUT"
#endif  // #ifndef CONFIG_RELEASE_BUILD_LIBRARIES
#endif //_RTL8195A_TIMER_H_
