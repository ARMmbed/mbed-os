/*
 * Copyright (c) 2013 - 2014, Freescale Semiconductor, Inc.
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without modification,
 * are permitted provided that the following conditions are met:
 *
 * o Redistributions of source code must retain the above copyright notice, this list
 *   of conditions and the following disclaimer.
 *
 * o Redistributions in binary form must reproduce the above copyright notice, this
 *   list of conditions and the following disclaimer in the documentation and/or
 *   other materials provided with the distribution.
 *
 * o Neither the name of Freescale Semiconductor, Inc. nor the names of its
 *   contributors may be used to endorse or promote products derived from this
 *   software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR
 * ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON
 * ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */
#if !defined(__FSL_RTC_HAL_MONOTONIC_H__)
#define __FSL_RTC_HAL_MONOTONIC_H__


#include "fsl_rtc_features.h"
#include "device/fsl_device_registers.h"
#include <stdint.h>
#include <stdbool.h>



/*! @addtogroup rtc_hal*/
/*! @{*/


/*******************************************************************************
 * Definitions
 ******************************************************************************/



/*******************************************************************************
 * API
 ******************************************************************************/

#if FSL_FEATURE_RTC_HAS_MONOTONIC

#if defined(__cplusplus)
extern "C" {
#endif

  
/*-------------------------------------------------------------------------------------------*/
/* RTC Monotonic Counter Register Reset Functions*/
/*-------------------------------------------------------------------------------------------*/

/*! @brief      Resets the RTC Monotonic Enable Register (RTC_MER).*/
static inline void rtc_hal_reset_reg_MER(void)
{
  HW_RTC_MER_WR((uint32_t)0x00000000U);
}

/*! @brief      Resets the RTC Monotonic Counter Low Register (RTC_MCLR).*/
static inline void rtc_hal_reset_reg_MCLR(void)
{
  HW_RTC_MCLR_WR((uint32_t)0x00000000U);
}

/*! @brief      Resets the RTC Monotonic Counter High Register (RTC_MCHR).*/
static inline void rtc_hal_reset_reg_MCHR(void)
{
  HW_RTC_MCHR_WR((uint32_t)0x00000000U);
}

/*-------------------------------------------------------------------------------------------*/
/* RTC Monotonic Enable*/
/*-------------------------------------------------------------------------------------------*/

/*! @brief      For the monotonic counter reads the mode writing.
 *  @return     true: Enables writing to the monotonic counter increment the counter.
 *              false: Enables writing to the monotonic counter load the counter with the
 *              value written.
 */
static inline bool rtc_hal_read_monotonic_enable(void)
{
 /* reads the value of the RTC Monotonic Enable Register (RTC_MER), field
  * Monotonic Counter Enable (MCE). */
  return (bool)BR_RTC_MER_MCE;
}

/*! @brief      Changes the state of the Monotonic Counter Enable.
 *  @param      enable true: Writes to the monotonic counter increment the counter.
 *              false: Writes to the monotonic counter load the counter with the
 *              value written.
 */
static inline void rtc_hal_config_monotonic_enable(bool enable)
{
 /* writes to the RTC Monotonic Enable Register (RTC_MER), field Monotonic 
  * Counter Enable (MCE).*/
  BW_RTC_MER_MCE((uint32_t) enable);
}

/*-------------------------------------------------------------------------------------------*/
/* RTC Monotonic Counter*/
/*-------------------------------------------------------------------------------------------*/

/*! @brief      Reads the values of the Monotonic Counter High and the Monotonic Counter
 *              Low and returns them into a single value.
 *  @param      counter [out] pointer to variable where the value  is stored.
 */
static inline void rtc_hal_get_monotonic_counter(uint64_t * counter)
{
  /* check for null pointer*/
  if(NULL == counter)
  {
    return;
  }
  
  uint64_t tmpCountHigh = 0;
  
  /* store in temporal variable to avoid compile warning*/
  tmpCountHigh = BR_RTC_MCHR_MCH;
  
  *counter = (((uint64_t)(tmpCountHigh) << 32) | ((uint64_t)(BR_RTC_MCLR_MCL)));
}

/*! @brief      Writes values Monotonic Counter High and Monotonic Counter Low
 *              by decomposing the given single value.
 *  @param      counter [in] pointer to the variable where the value  is stored.
 */
static inline void rtc_hal_set_monotonic_counter(const uint64_t * counter)
{
  /* check for null pointer*/
  if(NULL == counter)
  {
    return;
  }
  
  /* enable writing to the counter*/
  BW_RTC_MER_MCE(0U);
  
  BW_RTC_MCLR_MCL((uint32_t)(*counter));
  
  /* enable writing to the counter*/
  BW_RTC_MER_MCE(0U);
  
  BW_RTC_MCHR_MCH((uint32_t)((*counter) >> 32));
}

/*! @brief      Increments one to the Monotonic Counter.*/
/*! @return     true: success; false: error, time invalid flag found.*/
static inline bool rtc_hal_monotonic_counter_increment(void)
{
  bool result = false;
  
 /* Increments the Monotonic Counter (registers RTC_MCLR and RTC_MCHR
  * accordingly); by setting the monotonic counter enable (MER[MCE]) and then
  * writing to RTC_MCLR register. A write to monotonic counter low that causes
  * it to overflow  also increments the monotonic counter high. */
  
  if((!(BR_RTC_SR_TIF)) && (!(BR_RTC_SR_MOF)))
  {
    /* prepare for incrementing after write*/
    BW_RTC_MER_MCE(1U);
    
    /* write anything so the counter increments*/
    BW_RTC_MCLR_MCL(1U);
    
    result = true;
  }
  
  return result;
}

/*-------------------------------------------------------------------------------------------*/
/* RTC Status*/
/*-------------------------------------------------------------------------------------------*/

/*! @brief      Reads the value of the Monotonic Overflow Flag (MOF). This flag is
 *              set when the monotonic counter is enabled and the monotonic
 *              counter high overflows. The monotonic counter does not increment
 *              and  reads as zero when this bit is set. This bit is cleared
 *              by writing the monotonic counter high register when the monotonic
 *              counter is disabled.
 *  @return     true: monotonic counter overflow has occurred and monotonic
                counter is read as zero.
 *              false: NO monotonic counter overflow has occurred.
 */
static inline bool rtc_hal_is_monotonic_counter_overflow(void)
{
  return (bool)BR_RTC_SR_MOF;
}

/*-------------------------------------------------------------------------------------------*/
/* RTC Lock*/
/*-------------------------------------------------------------------------------------------*/

/*! @brief      Reads the value of the Monotonic Counter High Lock.
 *  @return     true: Monotonic counter high register is not locked and writes
 *              complete  normally.
 *              false: Monotonic counter high register is locked and writes are
 *              ignored.
 */
static inline bool rtc_hal_read_monotonic_hcounter_lock(void)
{
 /* Reads the RTC Lock Register (RTC_LR), field Monotonic Counter High Lock
  * (MCHL). */
  return (bool)BR_RTC_LR_MCHL;
}

/*! @brief      Clears the Monotonic Counter High Lock. Once done, this
 *              flag can only be set by VBAT POR orthe software reset.
 */
static inline void rtc_hal_clear_monotonic_hcounter_lock(void)
{
 /* Writes 0 to the RTC Lock Register (RTC_LR), field Monotonic Counter High Lock
  * (MCHL). Once cleared, this flag can only be set by VBAT POR or software
  * reset. */
  BW_RTC_LR_MCHL(0U);
}

/*! @brief      Reads the value of the Monotonic Counter Low Lock.
 *  @return     true: Monotonic counter low register is not locked and writes
 *              complete  normally.
 *              false: Monotonic counter low register is locked and writes are
 *              ignored.
 */
static inline bool rtc_hal_read_lcounter_lock(void)
{
 /* Reads the value of the RTC Lock Register (RTC_LR), field Monotonic Counter
  * Low Lock (MCLL). */
  return (bool)BR_RTC_LR_MCLL;
}

/*! @brief      Clears the Monotonic Counter Low Lock. Once done, this flag
 *              can only be set by the VBAT POR or the software reset.
 */
static inline void rtc_hal_clear_lcounter_lock(bool set_to)
{
  BW_RTC_LR_MCLL(0U);
}

/*! @brief      Reads the value of the Monotonic Enable Lock.
 *  @return     true: Monotonic enable register is not locked and writes
 *              complete  normally.
 *              false: Monotonic enable register is locked and writes are
 *              ignored.
 */
static inline bool rtc_hal_read_monotonic_enable_lock(void)
{
 /* Reads the value of the RTC Lock Register (RTC_LR), field Monotonic Enable
  * Lock (MEL). Once cleared, this flag can only be set by the VBAT POR or the software
  * reset. */
  return (bool)BR_RTC_LR_MEL;
}

/*! @brief      Clears the Monotonic Enable Lock. Once done, this flag can
 *              only be set by the VBAT POR or the software reset.
 */
static inline void rtc_hal_clear_monotonic_enable_lock(void)
{
 /* Writes to the RTC Lock Register (RTC_LR), field Monotonic Enable Lock (MEL).
  * Once cleared, this flag can only be set by the VBAT POR or the software reset. */
  BW_RTC_LR_MEL(0U);
}

/*-------------------------------------------------------------------------------------------*/
/* RTC Interrupt Enable*/
/*-------------------------------------------------------------------------------------------*/

/*! @brief      Checks whether the Monotonic Overflow Interrupt is enabled/disabled.
 *  @return     true: Monotonic overflow flag generates an interrupt.
 *              false: Monotonic overflow flag does not generate an interrupt.
 */
static inline bool rtc_hal_read_monot_overflow_int_enable(void)
{
 /* Reads the value of the RTC Interrupt Enable Register (RTC_IER), field 
  * Monotonic Overflow Interrupt Enable (MOIE). */
  return (bool)BR_RTC_IER_MOIE;
}

/*! @brief      Enables/disables the Monotonic Overflow Interrupt Enable.
 *  @param      enable true: Monotonic overflow flag  generates an interrupt.
 *              false: Monotonic overflow flag does not generate an interrupt.
 */
static inline void rtc_hal_config_monot_overflow_int_enable(bool enable)
{
  BW_RTC_IER_MOIE((uint32_t) enable);
}


#if defined(__cplusplus)
}
#endif

#endif

/*! @}*/

#endif /* __FSL_RTC_HAL_MONOTONIC_H__*/

/*******************************************************************************
 * EOF
 ******************************************************************************/
