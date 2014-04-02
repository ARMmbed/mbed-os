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
#if !defined(__FSL_RTC_HAL_ACCESS_CONTROL_H__)
#define __FSL_RTC_HAL_ACCESS_CONTROL_H__


#include "fsl_rtc_features.h"
#include "fsl_device_registers.h"
#include <stdint.h>
#include <stdbool.h>



/*! @addtogroup rtc_hal*/
/*! @{*/


/*******************************************************************************
 * Definitions
 ******************************************************************************/

typedef struct rtc_hal_access_control_config
{
  /*! Set/clear any of the following bitfields to enable/disable the
   * respective interrupts.\n\n
   *    IERW:   Interrupt Enable Register Write \n 
   *    LRW:    Lock Register Write              \n 
   *    SRW:    Status Register Write            \n 
   *    CRW:    Control Register Write           \n 
   *    TCRW:   Time Compensation Register Write \n 
   *    TARW:   Time Alarm Register Write       \n 
   *    TPRW:   Time Prescaler Register Write   \n 
   *    TSRW:   Time Seconds Register Write     \n 
   * \n
   *  For MCUs that have Tamper feature: \n
   *    TIRW:   Tamper Interrupt Register Write \n 
   *    TTRW:   Tamper Trim Register Write      \n 
   *    TDRW:   Tamper Detect Register Write    \n 
   *    TERW:   Tamper Enable Register Write    \n 
   *    TTSW:   Tamper Time Seconds Write       \n 
   * \n
   *  For MCUs that have Monotonic Counter: \n
   *    MCHW:   Monotonic Counter High Write    \n 
   *    MCLW:   Monotonic Counter Low Write     \n 
   *    MERW:   Monotonic Enable Register Write \n 
   */
  hw_rtc_war_t writeFlags;

  /*! Set/clear any of the following bitfields to enable/disable the
   * respective interrupts.\n\n
   *    IERR:   Interrupt Enable Register Read \n 
   *    LRR:    Lock Register Read \n 
   *    SRR:    Status Register Read \n 
   *    CRR:    Control Register Read \n 
   *    TCRR:   Time Compensation Register Read \n 
   *    TARR:   Time Alarm Register Read \n 
   *    TPRR:   Time Prescaler Register Read \n 
   *    TSRR:   Time Seconds Register Read \n 
   * \n
   *  For MCUs that have Tamper feature: \n
   *    TIRR:   Tamper Interrupt Register Read \n 
   *    TTRR:   Tamper Trim Register Read \n 
   *    TDRR:   Tamper Detect Register Read \n 
   *    TERR:   Tamper Enable Register Read \n 
   *    TTSR:   Tamper Time Seconds Read \n 
   * \n
   *  For MCUs that have Monotonic Counter: \n
   *    MCHR:   Monotonic Counter High Read \n 
   *    MCLR:   Monotonic Counter Low Read \n 
   *    MERR:   Monotonic Enable Register Read \n 
   */
  hw_rtc_rar_t readFlags;
  
} rtc_hal_access_control_config_t;


/*******************************************************************************
 * API
 ******************************************************************************/

#if FSL_FEATURE_RTC_HAS_ACCESS_CONTROL

#if defined(__cplusplus)
extern "C" {
#endif

  
/*-------------------------------------------------------------------------------------------*/
/* RTC Access Control Register Reset Functions*/
/*-------------------------------------------------------------------------------------------*/

/*! @brief      Resets the RTC Write Access Register (RTC_WAR).*/
static inline void rtc_hal_reset_reg_WAR(void)
{
  HW_RTC_WAR_WR((uint32_t)0x0000FFFFU);
}

/*! @brief      Resets the RTC Read Access Register (RTC_RAR).*/
static inline void rtc_hal_reset_reg_RAR(void)
{
  HW_RTC_RAR_WR((uint32_t)0x0000FFFFU);
}

/*-------------------------------------------------------------------------------------------*/
/* RTC Access Control Configuration*/
/*-------------------------------------------------------------------------------------------*/

/*! @brief      Configures the read and write access controls to other module fields
 *  @param      flags [in] pointer to structure where configuration flags are
 *              found. See the related structure for details.
 */
static inline void rtc_hal_access_control_configuration(const rtc_hal_access_control_config_t * flags)
{
  /* check for null pointer*/
  if(NULL == flags)
  {
    return;
  }
  
  uint32_t valid_flags;

  /* Set/clear any of the following bitfields to enable/disable the
   * respective interrupts.\n\n
   *    IERW:   Interrupt Enable Register Write \n 
   *    LRW:    Lock Register Write              \n 
   *    SRW:    Status Register Write            \n 
   *    CRW:    Control Register Write           \n 
   *    TCRW:   Time Compensation Register Write \n 
   *    TARW:   Time Alarm Register Write       \n 
   *    TPRW:   Time Prescaler Register Write   \n 
   *    TSRW:   Time Seconds Register Write     \n 
   * \n
   *  For MCUs that have Tamper feature: \n
   *    TIRW:   Tamper Interrupt Register Write \n 
   *    TTRW:   Tamper Trim Register Write      \n 
   *    TDRW:   Tamper Detect Register Write    \n 
   *    TERW:   Tamper Enable Register Write    \n 
   *    TTSW:   Tamper Time Seconds Write       \n 
   * \n
   *  For MCUs that have Monotonic Counter: \n
   *    MCHW:   Monotonic Counter High Write    \n 
   *    MCLW:   Monotonic Counter Low Write     \n 
   *    MERW:   Monotonic Enable Register Write \n 
   */
    valid_flags = 0;
    
#if FSL_FEATURE_RTC_HAS_MONOTONIC
    valid_flags |= (BM_RTC_WAR_MCHW | BM_RTC_WAR_MCLW | BM_RTC_WAR_MERW);
#endif
    valid_flags |= (BM_RTC_WAR_IERW | BM_RTC_WAR_LRW | BM_RTC_WAR_SRW |
      BM_RTC_WAR_CRW | BM_RTC_WAR_TCRW | BM_RTC_WAR_TARW | BM_RTC_WAR_TPRW |
      BM_RTC_WAR_TSRW);
      
    HW_RTC_WAR_WR((flags->writeFlags.U) & valid_flags);
    
  /* Set/clear any of the following bitfields to enable/disable the
   * respective interrupts.\n\n
   *    IERR:   Interrupt Enable Register Read \n 
   *    LRR:    Lock Register Read \n 
   *    SRR:    Status Register Read \n 
   *    CRR:    Control Register Read \n 
   *    TCRR:   Time Compensation Register Read \n 
   *    TARR:   Time Alarm Register Read \n 
   *    TPRR:   Time Prescaler Register Read \n 
   *    TSRR:   Time Seconds Register Read \n 
   * \n
   *  For MCUs that have Tamper feature: \n
   *    TIRR:   Tamper Interrupt Register Read \n 
   *    TTRR:   Tamper Trim Register Read \n 
   *    TDRR:   Tamper Detect Register Read \n 
   *    TERR:   Tamper Enable Register Read \n 
   *    TTSR:   Tamper Time Seconds Read \n 
   * \n
   *  For MCUs that have Monotonic Counter: \n
   *    MCHR:   Monotonic Counter High Read \n 
   *    MCLR:   Monotonic Counter Low Read \n 
   *    MERR:   Monotonic Enable Register Read \n 
   */
    valid_flags = 0;

#if FSL_FEATURE_RTC_HAS_MONOTONIC
    valid_flags |= (BM_RTC_RAR_MCHR | BM_RTC_RAR_MCLR | BM_RTC_RAR_MERR);
#endif
    valid_flags |= (BM_RTC_RAR_IERR | BM_RTC_RAR_LRR | BM_RTC_RAR_SRR |
      BM_RTC_RAR_CRR | BM_RTC_RAR_TCRR | BM_RTC_RAR_TARR | BM_RTC_RAR_TPRR |
      BM_RTC_RAR_TSRR);
    
    HW_RTC_RAR_WR((flags->readFlags.U) & valid_flags);

}


#if FSL_FEATURE_RTC_HAS_MONOTONIC
/*! @brief      Reads the value of the RTC Write Access Register (RTC_WAR),
 *              field Monotonic Counter High Write (MCHW). Once cleared,
 *              this bit is only set by the system reset. It is not affected by the
 *              VBAT POR or by the software reset.
 *  @return     true: Writes to the monotonic counter high register complete  normally.
 *              false: Writes to the monotonic counter high register are ignored.
 */
static inline bool rtc_hal_get_monotonic_hcount_wreg(void)
{
  return (bool)BR_RTC_WAR_MCHW;
}

/*! @brief      Writes to the RTC Write Access Register (RTC_WAR),
 *              field Monotonic Counter High Write (MCHW). Once cleared,
 *              this bit is only set by the system reset. It is not affected by the
 *              VBAT POR or software reset.
 *  @param      enable true: Writes to the monotonic counter high register complete  normally.
 *              false: Writes to the monotonic counter high register are ignored.
 */
static inline void rtc_hal_set_monotonic_hcount_wreg(bool enable)
{
  BW_RTC_WAR_MCHW((uint32_t) enable);
}

/*! @brief      Reads the value of the RTC Write Access Register (RTC_WAR),
 *              field Monotonic Counter Low Write (MCLW). Once cleared,
 *              this bit is only set by system reset. It is not affected by the
 *              VBAT POR or software reset.
 *  @return     true: Writes to the monotonic counter low register complete  normally.
 *              false: Writes to the monotonic counter low register are ignored.
 */
static inline bool rtc_hal_get_monotonic_lcount_wreg(void)
{
  return (bool)BR_RTC_WAR_MCLW;
}

/*! @brief      Writes to the RTC Write Access Register (RTC_WAR),
 *              field Monotonic Counter Low Write (MCLW). Once cleared,
 *              this bit is only set by the system reset. It is not affected by the
 *              VBAT POR or the software reset.
 *  @param      enable true: Writes to the monotonic counter low register complete  normally.
 *              false: Writes to the monotonic counter low register are ignored.
 */
static inline void rtc_hal_set_monotonic_lcount_wreg(bool enable)
{
  BW_RTC_WAR_MCLW((uint32_t) enable);
}

/*! @brief      Reads the value of the RTC Write Access Register (RTC_WAR),
 *              field Monotonic Enable Register Write (MERW). Once cleared,
 *              this bit is only set by the system reset. It is not affected by the
 *              VBAT POR or the software reset.
 *  @return     true: Writes to the monotonic enable register complete  normally.
 *              false: Writes to the monotonic enable register are ignored.
 */
static inline bool rtc_hal_get_monotonic_enable_wreg(void)
{
  return (bool)BR_RTC_WAR_MERW;
}

/*! @brief      Writes to the RTC Write Access Register (RTC_WAR),
 *              field Monotonic Enable Register Write (MERW). Once cleared,
 *              this bit is only set by the system reset. It is not affected by the
 *              VBAT POR or the software reset.
 *  @param      enable true: Writes to the monotonic enable register complete  normally.
 *              false: Writes to the monotonic enable register are ignored.
 */
static inline void rtc_hal_set_monotonic_enable_wreg(bool enable)
{
  BW_RTC_WAR_MERW((uint32_t) enable);
}
#endif

/*! @brief      Reads the value of the RTC Write Access Register (RTC_WAR),
 *              field Interrupt Enable Register Write (IERW). Once cleared,
 *              this bit is only set by the system reset. It is not affected by the
 *              VBAT POR or the software reset.
 *  @return     true: Writes to the interrupt enable register complete  normally.
 *              false: Writes to the interrupt enable register are ignored.
 */
static inline bool rtc_hal_get_interrupt_enable_wreg(void)
{
  return (bool)BR_RTC_WAR_IERW;
}

/*! @brief      Writes to the RTC Write Access Register (RTC_WAR),
 *              field Interrupt Enable Register Write (IERW). Once cleared,
 *              this bit is only set by the system reset. It is not affected by the
 *              VBAT POR or the software reset.
 *  @param      enable true: Writes to the interrupt enable register complete  normally.
 *              false: Writes to the interrupt enable register are ignored.
 */
static inline void rtc_hal_set_interrupt_enable_wreg(bool enable)
{
  BW_RTC_WAR_IERW((uint32_t) enable);
}

/*! @brief      Reads the value of the RTC Write Access Register (RTC_WAR),
 *              field Lock Register Write (LRW).
 *  @return     true: Writes to the lock register complete  normally.
 *              false: Writes to the lock register are ignored.
 */
static inline bool rtc_hal_get_lock_wreg(void)
{
  return (bool)BR_RTC_WAR_LRW;
}

/*! @brief      Writes to the RTC Write Access Register (RTC_WAR),
 *              field Lock Register Write (LRW). Once cleared,
 *              this bit is only set by the system reset. It is not affected by the
 *              VBAT POR or the software reset.
 *  @param      enable true: Writes to the lock register complete  normally.
 *              false: Writes to the lock register are ignored.
 */
static inline void rtc_hal_set_lock_wreg(bool enable)
{
  BW_RTC_WAR_LRW((uint32_t) enable);
}

/*! @brief      Reads the value of the RTC Write Access Register (RTC_WAR),
 *              field Status Register Write (SRW). Once cleared,
 *              this bit is only set by the system reset. It is not affected by the
 *              VBAT POR or the software reset.
 *  @return     true: Writes to the status register complete  normally.
 *              false: Writes to the status register are ignored.
 */
static inline bool rtc_hal_get_status_wreg(void)
{
  return (bool)BR_RTC_WAR_SRW;
}

/*! @brief      Writes to the RTC Write Access Register (RTC_WAR),
 *              field Status Register Write (SRW). Once cleared,
 *              this bit is only set by the system reset. It is not affected by the
 *              VBAT POR or the software reset.
 *  @param      enable true: Writes to the status register complete  normally.
 *              false: Writes to the status register are ignored.
 */
static inline void rtc_hal_set_status_wreg(bool enable)
{
  BW_RTC_WAR_SRW((uint32_t) enable);
}

/*! @brief      Reads the value of the RTC Write Access Register (RTC_WAR),
 *              field Control Register Write (CRW). Once cleared,
 *              this bit is only set by the system reset. It is not affected by the
 *              VBAT POR or the software reset.
 *  @return     true: Writes to the control register complete  normally.
 *              false: Writes to the control register are ignored.
 */
static inline bool rtc_hal_get_control_wreg(void)
{
  return (bool)BR_RTC_WAR_CRW;
}

/*! @brief      Writes to the RTC Write Access Register (RTC_WAR),
 *              field Control Register Write (CRW). Once cleared,
 *              this bit is only set by the system reset. It is not affected by the
 *              VBAT POR or the software reset.
 *  @param      enable true: Writes to the control register complete  normally.
 *              false: Writes to the control register are ignored.
 */
static inline void rtc_hal_set_control_wreg(bool enable)
{
  BW_RTC_WAR_CRW((uint32_t) enable);
}

/*! @brief      Reads the value of the RTC Write Access Register (RTC_WAR),
 *              field Time Compensation Register Write (TCRW).
 *  @return     true: Writes to the time compensation register complete  normally.
 *              false: Writes to the time compensation register are ignored.
 */
static inline bool rtc_hal_get_compensation_wreg(void)
{
  return (bool)BR_RTC_WAR_TCRW;
}

/*! @brief      Writes to the RTC Write Access Register (RTC_WAR),
 *              field Time Compensation Register Write (TCRW). Once cleared,
 *              this bit is only set by the system reset. It is not affected by the
 *              VBAT POR or the software reset.
 *  @param      enable true: Writes to the time compensation register complete  normally.
 *              false: Writes to the time compensation register are ignored.
 */
static inline void rtc_hal_set_compensation_wreg(bool enable)
{
  BW_RTC_WAR_TCRW((uint32_t) enable);
}

/*! @brief      Reads the value of the RTC Write Access Register (RTC_WAR),
 *              field Time Alarm Register Write (TARW). Once cleared,
 *              this bit is only set by the system reset. It is not affected by the
 *              VBAT POR or the software reset.
 *  @return     true: Writes to the time alarm register complete  normally.
 *              false: Writes to the time alarm register are ignored.
 */
static inline bool rtc_hal_get_alarm_wreg(void)
{
  return (bool)BR_RTC_WAR_TARW;
}

/*! @brief      Writes to the RTC Write Access Register (RTC_WAR),
 *              field Time Alarm Register Write (TARW). Once cleared,
 *              this bit is only set by the system reset. It is not affected by the
 *              VBAT POR or the software reset.
 *  @param      enable true: Writes to the time alarm register complete  normally.
 *              false: Writes to the time alarm register are ignored.
 */
static inline void rtc_hal_set_alarm_wreg(bool enable)
{
  BW_RTC_WAR_TARW((uint32_t) enable);
}

/*! @brief      Reads the value of the RTC Write Access Register (RTC_WAR),
 *              field Time Prescaler Register Write (TPRW).
 *  @return     true: Writes to the time prescaler register complete  normally.
 *              false: Writes to the time prescaler register are ignored.
 */
static inline bool rtc_hal_get_prescaler_wreg(void)
{
  return (bool)BR_RTC_WAR_TPRW;
}

/*! @brief      Writes to the RTC Write Access Register (RTC_WAR),
 *              field Time Prescaler Register Write (TPRW). Once cleared,
 *              this bit is only set by the system reset. It is not affected by the
 *              VBAT POR or the software reset.
 *  @param      enable true: Writes to the time prescaler register complete  normally.
 *              false: Writes to the time prescaler register are ignored.
 */
static inline void rtc_hal_set_prescaler_wreg(bool enable)
{
  BW_RTC_WAR_TPRW((uint32_t) enable);
}

/*! @brief      Reads the value of the RTC Write Access Register (RTC_WAR),
 *              field Time Seconds Register Write (TSRW). Once cleared,
 *              this bit is only set by the system reset. It is not affected by the
 *              VBAT POR or the software reset.
 *  @return     true: Writes to the time seconds register complete  normally.
 *              false: Writes to the time seconds register are ignored.
 */
static inline bool rtc_hal_get_seconds_wreg(void)
{
  return (bool)BR_RTC_WAR_TSRW;
}

/*! @brief      Writes to the RTC Write Access Register (RTC_WAR),
 *              field Time Seconds Register Write (TSRW). Once cleared,
 *              this bit is only set by the system reset. It is not affected by the
 *              VBAT POR or the software reset.
 *  @param      enable true: Writes to the time seconds register complete  normally.
 *              false: Writes to the time seconds register are ignored.
 */
static inline void rtc_hal_set_seconds_wreg(bool enable)
{
  BW_RTC_WAR_TSRW((uint32_t) enable);
}

#if (FSL_FEATURE_RTC_HAS_MONOTONIC == 1)
/*! @brief      Reads the value of the RTC Read Access Register (RTC_RAR),
 *              field Monotonic Counter High Read (MCHR). Once cleared,
 *              this bit is only set by the system reset. It is not affected by the
 *              VBAT POR or the software reset.
 *  @return     true: Reads to the monotonic counter high register complete  normally.
 *              false: Reads to the monotonic counter high register are ignored.
 */
static inline bool rtc_hal_get_monotonic_hcount_rreg(void)
{
  return (bool)BR_RTC_RAR_MCHR;
}

/*! @brief      Writes to the RTC Read Access Register (RTC_RAR),
 *              field Monotonic Counter High Read (MCHR). Once cleared,
 *              this bit is only set by the system reset. It is not affected by the
 *              VBAT POR or the software reset.
 *  @param      enable true: Reads to the monotonic counter high register complete  normally.
 *              false: Reads to the monotonic counter high register are ignored.
 */
static inline void rtc_hal_set_monotonic_hcount_rreg(bool enable)
{
  BW_RTC_RAR_MCHR((uint32_t) enable);
}

/*! @brief      Reads the value of the RTC Read Access Register (RTC_RAR),
 *              field Monotonic Counter Low Read (MCLR). Once cleared,
 *              this bit is only set by the system reset. It is not affected by the
 *              VBAT POR or the software reset.
 *  @return     true: Reads to the monotonic counter low register complete  normally.
 *              false: Reads to the monotonic counter low register are ignored.
 */
static inline bool rtc_hal_get_monotonic_lcount_rreg(void)
{
  return (bool)BR_RTC_RAR_MCLR;
}

/*! @brief      Writes to the RTC Read Access Register (RTC_RAR),
 *              field Monotonic Counter Low Read (MCLR). Once cleared,
 *              this bit is only set by the system reset. It is not affected by the
 *              VBAT POR or the software reset.
 *  @param      enable true: Reads to the monotonic counter low register complete  normally.
 *              false: Reads to the monotonic counter low register are ignored.
 */
static inline void rtc_hal_set_monotonic_lcount_rreg(bool enable)
{
  BW_RTC_RAR_MCLR((uint32_t) enable);
}

/*! @brief      Reads the value of the RTC Read Access Register (RTC_RAR),
 *              field Monotonic Enable Register Read (MERR). Once cleared,
 *              this bit is only set by the system reset. It is not affected by the
 *              VBAT POR or the software reset.
 *  @return     true: Reads to the monotonic enable register complete  normally.
 *              false: Reads to the monotonic enable register are ignored.
 */
static inline bool rtc_hal_get_monotonic_enable_rreg(void)
{
  return (bool)BR_RTC_RAR_MERR;
}

/*! @brief      Writes to the RTC Read Access Register (RTC_RAR),
 *              field Monotonic Enable Register Read (MERR). Once cleared,
 *              this bit is only set by the system reset. It is not affected by the
 *              VBAT POR or the software reset.
 *  @param      enable true: Reads to the monotonic enable register complete  normally.
 *              false: Reads to the monotonic enable register are ignored.
 */
static inline void rtc_hal_set_monotonic_enable_rreg(bool enable)
{
  BW_RTC_RAR_MERR((uint32_t) enable);
}
#endif

/*! @brief      Reads the value of the RTC Read Access Register (RTC_RAR),
 *              field Interrupt Enable Register Read (IERR). Once cleared,
 *              this bit is only set by the system reset. It is not affected by the
 *              VBAT POR or the software reset.
 *  @return     true: Reads to the interrupt enable register complete  normally.
 *              false: Reads to the interrupt enable register are ignored.
 */
static inline bool rtc_hal_get_interrupt_enable_rreg(void)
{
  return (bool)BR_RTC_RAR_IERR;
}

/*! @brief      Writes to the RTC Read Access Register (RTC_RAR),
 *              field Interrupt Enable Register Read (IERR). Once cleared,
 *              this bit is only set by the system reset. It is not affected by the
 *              VBAT POR or the software reset.
 *  @param      enable true: Reads to the interrupt enable register complete  normally.
 *              false: Reads to the interrupt enable register are ignored.
 */
static inline void rtc_hal_set_interrupt_enable_rreg(bool enable)
{
  BW_RTC_RAR_IERR((uint32_t) enable);
}

/*! @brief      reads the value of the RTC Read Access Register (RTC_RAR),
 *              field Lock Register Read (LRR). Once cleared,
 *              this bit is only set by system reset. It is not affected by
 *              VBAT POR or software reset.
 *  @return     true: Reads to the lock register complete as normal.
 *              false: Reads to the lock register are ignored.
 */
static inline bool rtc_hal_get_lock_rreg(void)
{
  return (bool)BR_RTC_RAR_LRR;
}

/*! @brief      Writes to the RTC Read Access Register (RTC_RAR),
 *              field Lock Register Read (LRR). Once cleared,
 *              this bit is only set by the system reset. It is not affected by the
 *              VBAT POR or the software reset.
 *  @param      enable true: Reads to the lock register complete  normally.
 *              false: Reads to the lock register are ignored.
 */
static inline void rtc_hal_set_lock_rreg(bool enable)
{
  BW_RTC_RAR_LRR((uint32_t) enable);
}

/*! @brief      Reads the value of the RTC Read Access Register (RTC_RAR),
 *              field Status Register Read (SRR). Once cleared,
 *              this bit is only set by the system reset. It is not affected by the
 *              VBAT POR or the software reset.
 *  @return     true: Reads to the status register complete  normally.
 *              false: Reads to the status register are ignored.
 */
static inline bool rtc_hal_get_status_rreg(void)
{
  return (bool)BR_RTC_RAR_SRR;
}

/*! @brief      Writes to the RTC Read Access Register (RTC_RAR),
 *              field Status Register Read (SRR). Once cleared,
 *              this bit is only set by the system reset. It is not affected by the
 *              VBAT POR or software reset.
 *  @param      enable true: Reads to the status register complete  normally.
 *              false: Reads to the status register are ignored.
 */
static inline void rtc_hal_set_status_rreg(bool enable)
{
  BW_RTC_RAR_SRR((uint32_t) enable);
}

/*! @brief      Reads the value of the RTC Read Access Register (RTC_RAR),
 *              field Control Register Read (CRR). Once cleared,
 *              this bit is only set by the system reset. It is not affected by the
 *              VBAT POR or software reset.
 *  @return     true: Reads to the control register complete  normally.
 *              false: Reads to the control register are ignored.
 */
static inline bool rtc_hal_get_control_rreg(void)
{
  return (bool)BR_RTC_RAR_CRR;
}

/*! @brief      Writes to the RTC Read Access Register (RTC_RAR),
 *              field Control Register Read (CRR). Once cleared,
 *              this bit is only set by the system reset. It is not affected by the
 *              VBAT POR or software reset.
 *  @param      enable true: Reads to the control register complete  normally.
 *              false: Reads to the control register are ignored.
 */
static inline void rtc_hal_set_control_rreg(bool enable)
{
  BW_RTC_RAR_CRR((uint32_t) enable);
}

/*! @brief      Reads the value of the RTC Read Access Register (RTC_RAR),
 *              field Time Compensation Register Read (TCRR). Once cleared,
 *              this bit is only set by the system reset. It is not affected by the
 *              VBAT POR or software reset.
 *  @return     true: Reads to the time compensation register complete  normally.
 *              false: Reads to the time compensation register are ignored.
 */
static inline bool rtc_hal_get_compensation_rreg(void)
{
  return (bool)BR_RTC_RAR_TCRR;
}

/*! @brief      Writes to the RTC Read Access Register (RTC_RAR),
 *              field Time Compensation Register Read (TCRR). Once cleared,
 *              this bit is only set by the system reset. It is not affected by the
 *              VBAT POR or software reset.
 *  @param      enable true: Reads to the time compensation register complete  normally.
 *              false: Reads to the time compensation register are ignored.
 */
static inline void rtc_hal_set_compensation_rreg(bool enable)
{
  BW_RTC_RAR_TCRR((uint32_t) enable);
}

/*! @brief      Reads the value of the RTC Read Access Register (RTC_RAR),
 *              field Time Alarm Register Read (TARR). Once cleared,
 *              this bit is only set by the system reset. It is not affected by the
 *              VBAT POR or software reset.
 *  @return     true: Reads to the time alarm register complete  normally.
 *              false: Reads to the time alarm register are ignored.
 */
static inline bool rtc_hal_get_alarm_rreg(void)
{
  return (bool)BR_RTC_RAR_TARR;
}

/*! @brief      Writes to the RTC Read Access Register (RTC_RAR),
 *              field Time Alarm Register Read (TARR). Once cleared,
 *              this bit is only set by the system reset. It is not affected by the
 *              VBAT POR or the software reset.
 *  @param      enable true: Reads to the time alarm register complete  normally.
 *              false: Reads to the time alarm register are ignored.
 */
static inline void rtc_hal_set_alarm_rreg(bool enable)
{
  BW_RTC_RAR_TARR((uint32_t) enable);
}

/*! @brief      Reads the value of the RTC Read Access Register (RTC_RAR),
 *              field Time Prescaler Register Read (TPRR). Once cleared,
 *              this bit is only set by the system reset. It is not affected by
 *              VBAT POR or software reset.
 *  @return     true: Reads to the time prescaler register complete  normally.
 *              false: Reads to the time prescaler register are ignored.
 */
static inline bool rtc_hal_get_prescaler_rreg(void)
{
  return (bool)BR_RTC_RAR_TPRR;
}

/*! @brief      Writes to the RTC Read Access Register (RTC_RAR),
 *              field Time Prescaler Register Read (TPRR). Once cleared,
 *              this bit is only set by the system reset. It is not affected by the
 *              VBAT POR or the software reset.
 *  @param      enable true: Reads to the time prescaler register complete  normally.
 *              false: Reads to the time prescaler register are ignored.
 */
static inline void rtc_hal_set_prescaler_rreg(bool enable)
{

  BW_RTC_RAR_TPRR((uint32_t) enable);
}

/*! @brief      Reads the value of the RTC Read Access Register (RTC_RAR),
 *              field Time Seconds Register Read (TSRR). Once cleared,
 *              this bit is only set by the system reset. It is not affected by the
 *              VBAT POR or the software reset.
 *  @return     true: Reads to the time seconds register complete  normally.
 *              false: Reads to the time seconds register are ignored.
 */
static inline bool rtc_hal_get_seconds_rreg(void)
{
  return (bool)BR_RTC_RAR_TSRR;
}

/*! @brief      Writes to the RTC Read Access Register (RTC_RAR),
 *              field Time Seconds Register Read (TSRR). Once cleared,
 *              this bit is only set by the system reset. It is not affected by the
 *              VBAT POR or the software reset.
 *  @param      enable true: Reads to the time seconds register complete  normally.
 *              false: Reads to the time seconds register are ignored.
 */
static inline void rtc_hal_set_seconds_rreg(bool enable)
{
  BW_RTC_RAR_TSRR((uint32_t) enable);
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
