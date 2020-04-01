/***************************************************************************//**
 * @file
 * @brief Real Time Counter (RTC) peripheral API
 *******************************************************************************
 * # License
 * <b>Copyright 2018 Silicon Laboratories Inc. www.silabs.com</b>
 *******************************************************************************
 *
 * SPDX-License-Identifier: Zlib
 *
 * The licensor of this software is Silicon Laboratories Inc.
 *
 * This software is provided 'as-is', without any express or implied
 * warranty. In no event will the authors be held liable for any damages
 * arising from the use of this software.
 *
 * Permission is granted to anyone to use this software for any purpose,
 * including commercial applications, and to alter it and redistribute it
 * freely, subject to the following restrictions:
 *
 * 1. The origin of this software must not be misrepresented; you must not
 *    claim that you wrote the original software. If you use this software
 *    in a product, an acknowledgment in the product documentation would be
 *    appreciated but is not required.
 * 2. Altered source versions must be plainly marked as such, and must not be
 *    misrepresented as being the original software.
 * 3. This notice may not be removed or altered from any source distribution.
 *
 ******************************************************************************/

#ifndef EM_RTC_H
#define EM_RTC_H

#include "em_device.h"
#if defined(RTC_COUNT) && (RTC_COUNT > 0)

#include <stdbool.h>

#ifdef __cplusplus
extern "C" {
#endif

/***************************************************************************//**
 * @addtogroup emlib
 * @{
 ******************************************************************************/

/***************************************************************************//**
 * @addtogroup RTC
 * @{
 ******************************************************************************/

/*******************************************************************************
 *******************************   DEFINES   ***********************************
 ******************************************************************************/

/** The RTC peripheral on series 0 devices support 2 compare channels while
 *  the RTC peripheral on series 1 devices support 6 compare channels. */
#if defined(_SILICON_LABS_32B_SERIES_0)
#define NUM_RTC_CHANNELS   2U
#else
#define NUM_RTC_CHANNELS   6U
#endif

/*******************************************************************************
 *******************************   STRUCTS   ***********************************
 ******************************************************************************/

/** RTC initialization structure. */
typedef struct {
  bool enable;   /**< Start counting when initialization is completed. */
  bool debugRun; /**< Counter shall keep running during debug halt. */
  bool comp0Top; /**< Use compare register 0 as max count value. */
} RTC_Init_TypeDef;

/** Suggested default configuration for RTC initialization structure. */
#define RTC_INIT_DEFAULT                                      \
  {                                                           \
    true,  /* Start counting when initialization is done. */  \
    false, /* Disable updating during debug halt. */          \
    true   /* Restart counting from 0 when reaching COMP0. */ \
  }

/*******************************************************************************
 *****************************   PROTOTYPES   **********************************
 ******************************************************************************/

uint32_t RTC_CompareGet(unsigned int comp);
void RTC_CompareSet(unsigned int comp, uint32_t value);

/***************************************************************************//**
 * @brief
 *   Get RTC counter value.
 *
 * @return
 *   Current RTC counter value.
 ******************************************************************************/
__STATIC_INLINE uint32_t RTC_CounterGet(void)
{
  return RTC->CNT;
}

#if !defined(_EFM32_GECKO_FAMILY)
/***************************************************************************//**
 * @brief
 *   Set the RTC counter value.
 *
 * @param[in] value
 *   The new RTC counter value.
 ******************************************************************************/
__STATIC_INLINE void RTC_CounterSet(uint32_t value)
{
  RTC->CNT = value;
}
#endif

void RTC_CounterReset(void);
void RTC_Enable(bool enable);
#if defined(_RTC_FREEZE_MASK)
void RTC_FreezeEnable(bool enable);
#endif
void RTC_Init(const RTC_Init_TypeDef *init);

/***************************************************************************//**
 * @brief
 *   Clear one or more pending RTC interrupts.
 *
 * @param[in] flags
 *   RTC interrupt sources to clear. Use a set of interrupt flags OR-ed
 *   together to clear multiple interrupt sources for the RTC module
 *   (RTC_IFS_nnn).
 ******************************************************************************/
__STATIC_INLINE void RTC_IntClear(uint32_t flags)
{
  RTC->IFC = flags;
}

/***************************************************************************//**
 * @brief
 *   Disable one or more RTC interrupts.
 *
 * @param[in] flags
 *   RTC interrupt sources to disable. Use a set of interrupt flags OR-ed
 *   together to disable multiple interrupt sources for the RTC module
 *   (RTC_IFS_nnn).
 ******************************************************************************/
__STATIC_INLINE void RTC_IntDisable(uint32_t flags)
{
  RTC->IEN &= ~flags;
}

/***************************************************************************//**
 * @brief
 *   Enable one or more RTC interrupts.
 *
 * @note
 *   Depending on the use, a pending interrupt may already be set prior to
 *   enabling the interrupt. To ignore a pending interrupt, consider using
 *   RTC_IntClear() prior to enabling the interrupt.
 *
 * @param[in] flags
 *   RTC interrupt sources to enable. Use a set of interrupt flags OR-ed
 *   together to set multiple interrupt sources for the RTC module
 *   (RTC_IFS_nnn).
 ******************************************************************************/
__STATIC_INLINE void RTC_IntEnable(uint32_t flags)
{
  RTC->IEN |= flags;
}

/***************************************************************************//**
 * @brief
 *   Get pending RTC interrupt flags.
 *
 * @note
 *   Event bits are not cleared by using this function.
 *
 * @return
 *   Pending RTC interrupt sources. Returns a set of interrupt flags OR-ed
 *   together for multiple interrupt sources in the RTC module (RTC_IFS_nnn).
 ******************************************************************************/
__STATIC_INLINE uint32_t RTC_IntGet(void)
{
  return RTC->IF;
}

/***************************************************************************//**
 * @brief
 *   Get enabled and pending RTC interrupt flags.
 *   Useful for handling more interrupt sources in the same interrupt handler.
 *
 * @note
 *   Interrupt flags are not cleared by using this function.
 *
 * @return
 *   Pending and enabled RTC interrupt sources.
 *   The return value is the bitwise AND of
 *   - the enabled interrupt sources in RTC_IEN and
 *   - the pending interrupt flags RTC_IF.
 ******************************************************************************/
__STATIC_INLINE uint32_t RTC_IntGetEnabled(void)
{
  uint32_t ien;

  ien = RTC->IEN;
  return RTC->IF & ien;
}

/***************************************************************************//**
 * @brief
 *   Set one or more pending RTC interrupts from SW.
 *
 * @param[in] flags
 *   RTC interrupt sources to set to pending. Use a set of interrupt flags
 *   OR-ed together to set multiple interrupt sources for the RTC module
 *   (RTC_IFS_nnn).
 ******************************************************************************/
__STATIC_INLINE void RTC_IntSet(uint32_t flags)
{
  RTC->IFS = flags;
}

void RTC_Reset(void);

/** @} (end addtogroup RTC) */
/** @} (end addtogroup emlib) */

#ifdef __cplusplus
}
#endif

#endif /* defined(RTC_COUNT) && (RTC_COUNT > 0) */
#endif /* EM_RTC_H */
