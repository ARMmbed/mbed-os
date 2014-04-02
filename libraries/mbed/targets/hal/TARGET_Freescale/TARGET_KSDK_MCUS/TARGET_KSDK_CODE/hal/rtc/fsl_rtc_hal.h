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
#if !defined(__FSL_RTC_HAL_H__)
#define __FSL_RTC_HAL_H__


#include "fsl_rtc_features.h"
#include "fsl_device_registers.h"

#if FSL_FEATURE_RTC_HAS_MONOTONIC
  #include "fsl_rtc_hal_monotonic.h"
#endif
#if FSL_FEATURE_RTC_HAS_ACCESS_CONTROL
  #include "fsl_rtc_hal_access_control.h"
#endif

#include <stdint.h>
#include <stdbool.h>

/*!
 * @addtogroup rtc_hal
 * @{
 */

/*******************************************************************************
 * Definitions
 ******************************************************************************/

typedef struct rtc_hal_init_config
{

  /*! Configures the oscillator load in pF. \n
   *    Example:\n
   *    value 0   => to be configured: 0pF \n
   *    value 2   => to be configured: 2pF \n
   *    value 4   => to be configured: 4pF \n
   *    value 8   => to be configured: 8pF \n
   *    value 16  => to be configured: 16pF \n
   * Any other value is ignored.
   */
  uint8_t enableOscillatorLoadConfg;
  
  bool disableClockOutToPeripheral; /*!< at register field CR[CLKO]*/
  
  /*! After enabling this,  waits the oscillator startup time before enabling
   * the time counter TSR[TSR] to allow the 32.768 kHz clock time to stabilize.
   */
  bool enable32kOscillator; /*!< at register field CR[OSCE]*/
  
#if FSL_FEATURE_RTC_HAS_WAKEUP_PIN
  /*! For devices that have the wakeup pin, this variable  indicates whether it is
   *  to be enabled (set to 'true') or not (set to 'false') at the initialization
   *  function.\n
   *  See the device's user manual for details depending on each device
   *  specific wakeup pin feature implementation.
   */
  bool enableWakeupPin;
#endif
  
  /*! Sets the register field TSR[TSR]. Set to zero to skip this configuration.*/
  uint32_t startSecondsCounterAt;
  
  /*! Sets the register field TPR[TPR]. Set to zero to skip this configuration.*/
  uint16_t prescalerAt;
  
  /*! Sets the register field TAR[TAR]. Set to zero to skip this configuration.*/
  uint32_t alarmCounterAt;
  
  /*! Configures the compensation interval in seconds from 1 to 256 to control
   *  how frequently the TCR should adjusts the number of 32.768 kHz cycles in
   *  each second. The value written should be one less than the number of
   *  seconds (for example, write zero to configure for a compensation interval
   *  of one second). This register is double-buffered and writes do not take
   *  affect until the end of the current compensation interval.\n
   *  Set to zero to skip this configuration.
   */
  uint8_t compensationInterval;
  
  /*! Configures the number of 32.768 kHz clock cycles in each second. This
   *  register is double-buffered and writes do not take affect until the end
   *  of the current compensation interval.\n 
   *  \n
   *    80h Time prescaler register overflows every 32896 clock cycles.\n
   *    ... ...\n
   *    FFh Time prescaler register overflows every 32769 clock cycles.\n
   *    00h Time prescaler register overflows every 32768 clock cycles.\n
   *    01h Time prescaler register overflows every 32767 clock cycles.\n
   *    ... ...\n
   *    7Fh Time prescaler register overflows every 32641 clock cycles.\n
   */
  uint8_t timeCompensation;
   
  /*! Sets/clears any of the following bitfields to enable/disable the
   * respective interrupts.\n
   *   TSIE:    Time Seconds Interrupt Enable \n
   *   TAIE:    Time Alarm Interrupt Enable \n
   *   TOIE:    Time Overflow Interrupt Enable \n
   *   TIIE:    Time Invalid Interrupt Enable \n
   * \n
   * For MCUs that have the Wakeup Pin only: \n
   *   WPON:    Wakeup Pin On (see the corresponding MCU's reference manual)\n
   * \n
   * For MCUs that have the Monotonic Counter only: \n
   *   MOIE:    Monotonic Overflow Interrupt Enable \n
   */
  hw_rtc_ier_t enableInterrupts;
  
#if FSL_FEATURE_RTC_HAS_MONOTONIC
  /*! Sets the Monotonic Counter to the pointed variable's value.
   *  To skip setting a value or if does not apply set pointer to NULL.
   */
  uint64_t * monotonicCounterAt;
#endif
  
} rtc_hal_init_config_t;

/*******************************************************************************
 * API
 ******************************************************************************/

#if defined(__cplusplus)
extern "C" {
#endif

/*! @brief      Initializes the RTC module.
 *
 *  @param      configs Pointer to a structure where the configuration details are
 *                      stored at. The structure values that do NOT apply to the
 *                      MCU in use are  ignored.
 */
void rtc_hal_init(rtc_hal_init_config_t * configs);


/*-------------------------------------------------------------------------------------------*/
/* RTC Register Reset Functions*/
/*-------------------------------------------------------------------------------------------*/

/*! @brief      Resets the RTC Time Seconds Register (RTC_TSR).*/
static inline void rtc_hal_reset_reg_TSR(void)
{
  HW_RTC_TSR_WR((uint32_t)0x00000000U);
}

/*! @brief      Resets the RTC Time Prescaler Register (RTC_TPR).*/
static inline void rtc_hal_reset_reg_TPR(void)
{
  HW_RTC_TPR_WR((uint32_t)0x00000000U);
}

/*! @brief      Resets the RTC Time Alarm Register (RTC_TAR).*/
static inline void rtc_hal_reset_reg_TAR(void)
{
  HW_RTC_TAR_WR((uint32_t)0x00000000U);
}

/*! @brief      Resets the RTC Time Compensation Register (RTC_TCR).*/
static inline void rtc_hal_reset_reg_TCR(void)
{
  HW_RTC_TCR_WR((uint32_t)0x00000000U);
}

/*! @brief      Resets the RTC Control Register (RTC_CR).*/
static inline void rtc_hal_reset_reg_CR(void)
{
  HW_RTC_CR_WR((uint32_t)0x00000000U);
}

/*! @brief      Resets the RTC Status Register (RTC_SR).*/
static inline void rtc_hal_reset_reg_SR(void)
{
  HW_RTC_SR_WR((uint32_t)0x00000001U);
}

/*! @brief      Resets the RTC Lock Register (RTC_LR).*/
static inline void rtc_hal_reset_reg_LR(void)
{
#if (defined(CPU_MKL25Z32VFM4) ||  defined(CPU_MKL25Z64VFM4) ||  defined(CPU_MKL25Z128VFM4) || \
     defined(CPU_MKL25Z32VFT4) ||  defined(CPU_MKL25Z64VFT4) ||  defined(CPU_MKL25Z128VFT4) || \
     defined(CPU_MKL25Z32VLH4) ||  defined(CPU_MKL25Z64VLH4) ||  defined(CPU_MKL25Z128VLH4) || \
     defined(CPU_MKL25Z32VLK4) ||  defined(CPU_MKL25Z64VLK4) ||  defined(CPU_MKL25Z128VLK4) || \
     defined(CPU_MK22FN512VDC12))

  HW_RTC_LR_WR((uint32_t)0x000000FFU);
  
#elif (defined(CPU_MK70FX512VMJ12) || defined(CPU_MK70FN1M0VMJ12) || \
       defined(CPU_MK70FX512VMJ15) || defined(CPU_MK70FN1M0VMJ15) || \
       defined(CPU_MK63FN1M0VMD12) || defined(CPU_MK63FN1M0VMD12WS) || \
       defined(CPU_MK64FN1M0VMD12) || defined(CPU_MK64FX512VMD12))

  HW_RTC_LR_WR((uint32_t)0x0000FFFFU);
  
#else
  #error "No valid CPU defined"
#endif
}

/*! @brief      Resets the RTC Interrupt Enable Register (RTC_IER).*/
static inline void rtc_hal_reset_reg_IER(void)
{
  HW_RTC_IER_WR((uint32_t)0x00000007U);
}

/*-------------------------------------------------------------------------------------------*/
/* RTC Time Seconds*/
/*-------------------------------------------------------------------------------------------*/

/*! @brief      Reads the value of the time seconds counter.
 *  @param      Seconds [out] pointer to variable where the seconds are  stored.
 */
static inline void rtc_hal_get_seconds(uint32_t * seconds)
{
  /* When the time counter is enabled, the TSR is read only and increments
   * once a second provided SR[TOF] and SR[TIF] are not set. The time counter
   * reads as zero when SR[TOF] or SR[TIF] are set. When the time counter
   * is disabled, the TSR can be read or written. */
  *seconds = BR_RTC_TSR_TSR;

}

/*! @brief      Writes to the time seconds counter.
 *  @param      seconds [in] pointer to a variable from where to write the seconds.
 *  @return     true: write success since time counter is disabled.
 *              false: write error since time counter is enabled.
 */
static inline bool rtc_hal_set_seconds(const uint32_t * seconds)
{
  /* When the time counter is enabled, the TSR is read only and increments
   * once a second provided SR[TOF] or SR[TIF] are not set. The time counter
   * reads as zero when SR[TOF] or SR[TIF] are set. When the time counter
   * is disabled, the TSR can be read or written. Writing to the TSR when the
   * time counter is disabled  clears the SR[TOF] and/or the SR[TIF]. Writing
   * to the TSR register with zero is supported, but not recommended since TSR
   * reads as zero when SR[TIF] or SR[TOF] are set (indicating the time is
   * invalid). */
  
  bool result = false;
  
  if(!(BR_RTC_SR_TCE))
  {
    BW_RTC_TSR_TSR(*seconds); /* jgsp: add write to prescaler with 0x00 before writing to TSR.*/
    result = true;
  }
  
  return result;
}

/*-------------------------------------------------------------------------------------------*/
/* RTC Time Prescaler*/
/*-------------------------------------------------------------------------------------------*/

/*! @brief      Reads the value of the time prescaler.
 *  @param      prescale [out] pointer to variable where the prescaler's value
 *              is stored.
 */
static inline void rtc_hal_get_prescaler(uint16_t * prescale)
{
  /* When the time counter is enabled, the TPR is read only and increments
   * every 32.768 kHz clock cycle. The time counter  reads as zero when
   * SR[TOF] or SR[TIF] are set. When the time counter is disabled, the TPR
   * can be read or written. The TSR[TSR] increments when bit 14 of the TPR
   * transitions from a logic one to a logic zero. */
  *prescale = BR_RTC_TPR_TPR;

}

/*! @brief      Sets the time prescaler.
 *  @param      prescale [in] pointer to variable from where to write the
 *              seconds.
 *  @return     true: set successfull; false: error, unable to set prescaler
 *              since the the time counter is enabled.
 */
static inline bool rtc_hal_set_prescaler(const uint16_t * prescale)
{
  /* When the time counter is enabled, the TPR is read only and increments
   * every 32.768 kHz clock cycle. The time counter  reads as zero when the
   * SR[TOF] or SR[TIF] are set. When the time counter is disabled, the TPR
   * can be read or written. The TSR[TSR] increments when bit 14 of the TPR
   * transitions from a logic one to a logic zero. */
    bool result = false;
    
    if(!(BR_RTC_SR_TCE))
    {
      BW_RTC_TPR_TPR(*prescale);
      result = true;
    }
    
    return result;
    
}

/*-------------------------------------------------------------------------------------------*/
/* RTC Time Alarm*/
/*-------------------------------------------------------------------------------------------*/

/*! @brief      Reads the value of the time alarm.
 *  @param      seconds [out] pointer to a variable where the alarm value in seconds
 *              will be stored.
 */
static inline void rtc_hal_get_alarm(uint32_t * seconds)
{
  *seconds = BR_RTC_TAR_TAR;
}

/*! @brief      Sets the time alarm, this  clears the time alarm flag.
 *  @param      seconds [in] pointer to variable from where to write alarm value
 *              in seconds.
 */
static inline void rtc_hal_set_alarm(const uint32_t * seconds)
{
 /* When the time counter is enabled, the SR[TAF] is set whenever the TAR[TAR]
  * equals the TSR[TSR] and the TSR[TSR] increments. Writing to the TAR
  * clears the SR[TAF]. */
  BW_RTC_TAR_TAR(*seconds);
}

/*-------------------------------------------------------------------------------------------*/
/* RTC Time Compensation*/
/*-------------------------------------------------------------------------------------------*/

/*! @brief      Reads the compensation interval counter value.*/
/*! @param      counter [out] pointer to variable where the value  is stored.*/
static inline void rtc_hal_get_comp_intrvl_counter(uint8_t * counter)
{
  *counter = BR_RTC_TCR_CIC;
}

/*! @brief      Reads the current time compensation interval counter value.
 *  @param      tcValue [out] pointer to variable where the value  is stored.
 */
static inline void rtc_hal_get_current_time_compensation(uint8_t * tcValue)
{
  *tcValue = BR_RTC_TCR_TCV;
}

/*! @brief      Reads the compensation interval. The value is the configured
 *              compensation interval in seconds from 1 to 256 to control
 *              how frequently the time compensation register  should adjust the
 *              number of 32.768 kHz cycles in each second. The value is one
 *              less than the number of seconds (for example. Zero means a
 *              configuration for a compensation interval of one second). 
 *  @param      value [out] pointer to variable where the value  is stored.
 */
static inline void rtc_hal_get_compensation_interval(uint8_t * value)
{
  *value = BR_RTC_TCR_CIR;
}

/*! @brief      Writes the compensation interval. This configures the
 *              compensation interval in seconds from 1 to 256 to control
 *              how frequently the TCR should adjust the number of 32.768 kHz
 *              cycles in each second. The value written should be one less than
 *              the number of seconds (for example, write zero to configure for
 *              a compensation interval of one second). This register is double
 *              buffered and writes do not take affect until the end of the
 *              current compensation interval.
 *  @param      value [in] pointer to a variable from where to write the value.
 */
static inline void rtc_hal_set_compensation_interval(const uint8_t * value)
{
  BW_RTC_TCR_CIR(*value);
}

/*! @brief      Reads the time compensation value which is the configured number
 *              of the 32.768 kHz clock cycles in each second.
 *  @param      value [out] pointer to variable where the value is stored.
 */
static inline void rtc_hal_get_time_compensation(uint8_t * value)
{
  *value = BR_RTC_TCR_TCR;
}

/*! @brief      Writes to the RTC Time Compensation Register (RTC_TCR), field
 *              Time Compensation Register (TCR). Configuring the number of
 *              32.768 kHz clock cycles in each second. This register is double
 *              buffered and writes do not take affect until the end of the
 *              current compensation interval.\n
 *              80h Time prescaler register overflows every 32896 clock cycles.\n
 *              ... ...\n
 *              FFh Time prescaler register overflows every 32769 clock cycles.\n
 *              00h Time prescaler register overflows every 32768 clock cycles.\n
 *              01h Time prescaler register overflows every 32767 clock cycles.\n
 *              ... ...\n
 *              7Fh Time prescaler register overflows every 32641 clock cycles.\n
 *  @param      enable [in] pointer to variable from where to write the value.
 */
static inline void rtc_hal_set_time_compensation(const uint8_t * enable)
{
  BW_RTC_TCR_TCR(*enable);
}

/*-------------------------------------------------------------------------------------------*/
/* RTC Control*/
/*-------------------------------------------------------------------------------------------*/

/*! @brief      Enables/disables oscillator configuration for 2pF load.*/
/*! @param      enable true: enables load; false: disables load.*/
static inline void rtc_hal_config_osc_2pf_load(bool enable)
{
  BW_RTC_CR_SC2P(enable);
}

/*! @brief      Enables/disables oscillator configuration for 4pF load.*/
/*! @param      enable true: enables load; false: disables load.*/
static inline void rtc_hal_config_osc_4pf_load(bool enable)
{
  BW_RTC_CR_SC4P(enable);
}

/*! @brief      Enables/disables oscillator configuration for 8pF load.*/
/*! @param      enable true: enables load; false: disables load.*/
static inline void rtc_hal_config_osc_8pf_load(bool enable)
{
  BW_RTC_CR_SC8P(enable);
}

/*! @brief      Enables/disables oscillator configuration for 16pF load.*/
/*! @param      enable true: enables load; false: disables load.*/
static inline void rtc_hal_config_osc_16pf_load(bool enable)
{
  BW_RTC_CR_SC16P(enable);
}

/*! @brief      Enables/disables the 32kHz clock output to other peripherals.*/
/*! @param      enable true: enables clock out; false: disables clock out.*/
static inline void rtc_hal_config_clock_out(bool enable)
{
  BW_RTC_CR_CLKO(!enable);
}

/*! @brief      Enables/disables  the oscillator. After enablement, wait the
 *              oscillator startup time before enabling the time counter to
 *              allow the 32.768 kHz clock time to stabilize.
 *  @param      enable true: enables oscillator; false: disables oscillator.
 */
static inline void rtc_hal_config_oscillator(bool enable)
{
  BW_RTC_CR_OSCE(enable);
}

/*! @brief      Enables/disables the update mode. This mode allows the time counter
 *              enabled to be written even when the status register is locked.
 *              When set, the time counter enable, can always be written if the
 *              TIF (Time Invalid Flag) or TOF (Time Overflow Flag) are set or
 *              if the time counter enable is clear. For devices with the
 *              monotonic counter, it allows the monotonic enable to be written
 *              when it is locked. When set, the monotonic enable can always be
 *              written if the TIF (Time Invalid Flag) or TOF (Time Overflow Flag)
 *              are set or if the monotonic counter enable is clear.
 *              For devices with tamper detect, it allows the it to be written
 *              when it is locked. When set, the tamper detect can always be
 *              written if the TIF (Time Invalid Flag) is clear.
 *              Note: Tamper and Monotonic features are not available in all MCUs.
 *  @param      lock true: enables register lock, registers cannot be written
 *                         when locked; False: disables register lock, registers
 *                         can be written when locked under limited conditions.
 */
static inline void rtc_hal_configure_update_mode(bool lock)
{
  BW_RTC_CR_UM(lock);
}

/*! @brief      Enables/disables the supervisor access, which configures
 *              non-supervisor mode write access to all RTC registers and
 *              non-supervisor mode read access to RTC tamper/monotonic registers.
 *              Note: Tamper and Monotonic features are NOT available in all MCUs.
 *  @param      enable_reg_write true: enables register lock, Non-supervisor
 *              mode write accesses are supported; false: disables register
 *              lock, non-supervisor mode write accesses are not supported and
 *              generate a bus error.
 */
static inline void rtc_hal_configure_supervisor_access(bool enable_reg_write)
{
  BW_RTC_CR_SUP(enable_reg_write);
}

#if FSL_FEATURE_RTC_HAS_WAKEUP_PIN
/*! @brief      Enables/disables the wakeup pin.
 *              Note: The wakeup pin is optional and not available on all devices.
 *  @param      enable_wp true: enables wakeup-pin, wakeup pin asserts if the
 *              RTC interrupt asserts and the chip is powered down;
 *              false: disables wakeup-pin.
 */
static inline void rtc_hal_config_wakeup_pin(bool enable_wp)
{
  BW_RTC_CR_WPE(enable_wp);
}
#endif

/*! @brief      Performs a software reset on the RTC module. This resets all
 *              RTC registers except for the SWR bit and the RTC_WAR and RTC_RAR
 *              registers. The SWR bit is cleared after VBAT POR and by software
 *              explicitly clearing it.
 *              Note: access control features (RTC_WAR and RTC_RAR registers)
 *              are not available in all MCUs.
 */
static inline void rtc_hal_software_reset(void)
{
  BW_RTC_CR_SWR(1u);
}

/*! @brief      Clears the software reset flag.*/
static inline void rtc_hal_software_reset_flag_clear(void)
{
  BW_RTC_CR_SWR(0u);
}

/*-------------------------------------------------------------------------------------------*/
/* RTC Status*/
/*-------------------------------------------------------------------------------------------*/

/*! @brief      Reads the time counter enabled/disabled status.
 *  @return     true: time counter is enabled, time seconds register and time
 *              prescaler register are not writeable, but increment; false: time
 *              counter is disabled, time seconds register and time prescaler
 *              register are writeable, but do not increment. */
static inline bool rtc_hal_is_counter_enabled(void)
{
  return (bool)BR_RTC_SR_TCE;
}

/*! @brief      Changes the time counter enabled/disabled status.
 *  @param      enable true: enables the time counter;
 *              false: disables the time counter.
 */
static inline void rtc_hal_counter_enable(bool enable)
{
  BW_RTC_SR_TCE(enable);
}

/*! @brief      Checks if the configured time alarm  occurred.
 *  @return     true: time alarm has occurred.
 *              false: NO time alarm occurred.
 */
static inline bool rtc_hal_is_alarm_occured(void)
{
  /* Reads time alarm flag (TAF). This flag is set when the time
   * alarm register (TAR) equals the time seconds register (TSR) and
   * the TSR increments. This flag is cleared by writing the TAR register. */
  return (bool)BR_RTC_SR_TAF;
}

/*! @brief      Checks whether a counter overflow  happened.
 *  @return     true: time overflow occurred and time counter is zero.
 *              false: NO time overflow occurred.
 */
static inline bool rtc_hal_is_counter_overflow(void)
{
  /* Reads the value of RTC Status Register (RTC_SR), field Time
   * Overflow Flag (TOF). This flag is set when the time counter is
   * enabled and overflows. The TSR and TPR do not increment and read
   * as zero when this bit is set. This flag is cleared by writing the
   * TSR register when the time counter is disabled. */
  return (bool)BR_RTC_SR_TOF;
}

/*! @brief      Checks whether the time is marked as invalid.
 *  @return     true: time is INVALID and time counter is zero.
 *              false: time is valid.
 */
static inline bool rtc_hal_is_time_invalid(void)
{
  /*! Reads the value of the RTC Status Register (RTC_SR), field Time
   *  Invalid Flag (TIF). This flag is set on the VBAT POR or the software
   *  reset. The TSR and TPR do not increment and read as zero when
   *  this bit is set. This flag is cleared by writing the TSR
   *  register when the time counter is disabled. */
  return (bool)BR_RTC_SR_TIF;
}

/*-------------------------------------------------------------------------------------------*/
/* RTC Lock*/
/*-------------------------------------------------------------------------------------------*/

/*! @brief      Configures the register lock to other module fields.
 *  @param      bitfields [in] configuration flags:\n
 *  Valid bitfields:\n
 *    LRL: Lock Register Lock \n
 *    SRL: Status Register Lock \n
 *    CRL: Control Register Lock \n
 *    TCL: Time Compensation Lock \n
 *
 * For MCUs that have the Tamper Detect only: \n
 *    TIL: Tamper Interrupt Lock \n
 *    TTL: Tamper Trim Lock \n
 *    TDL: Tamper Detect Lock \n
 *    TEL: Tamper Enable Lock \n
 *    TTSL: Tamper Time Seconds Lock \n
 *
 * For MCUs that have the Monotonic Counter only: \n
 *    MCHL: Monotonic Counter High Lock \n
 *    MCLL: Monotonic Counter Low Lock \n
 *    MEL: Monotonic Enable Lock \n
 */
static inline void rtc_hal_config_lock_registers(hw_rtc_lr_t bitfields)
{
  uint32_t valid_flags = 0;

  valid_flags |= (BM_RTC_LR_LRL | BM_RTC_LR_SRL | BM_RTC_LR_CRL |
    BM_RTC_LR_TCL);

#if FSL_FEATURE_RTC_HAS_MONOTONIC
  valid_flags |= (BM_RTC_LR_MCHL | BM_RTC_LR_MCLL | BM_RTC_LR_MEL);
#endif
  HW_RTC_LR_WR((bitfields.U) & valid_flags);
  
}

/*! @brief      Obtains the lock status of the lock register.
 *  @return     true: lock register is not locked and writes complete  normally.
 *              false: lock register is locked and writes are ignored.
 */
static inline bool rtc_hal_get_lock_reg_lock(void)
{
  /* Reads the value of the RTC Lock Register (RTC_LR),
   * field Lock Register Lock (LRL). Once cleared, this flag can
   * only be set by VBAT POR or software reset. */
  return (bool)BR_RTC_LR_LRL;
}

/*! @brief      Changes the lock status of the lock register. Once cleared,
 *              this can only be set by the VBAT POR or the software reset.
 *  @param      set_to true: Lock register is not locked and writes complete  normally.
 *              false: Lock register is locked and writes are ignored.
 */
static inline void rtc_hal_set_lock_reg_lock(bool set_to)
{
 /* Writes to the RTC Lock Register (RTC_LR), field Lock Register Lock (LRL).
  * Once cleared, this flag can only be set by VBAT POR or software reset. */
  BW_RTC_LR_LRL((uint32_t) set_to);
}

/*! @brief      Obtains the state of the status register lock.
 *  @return     true: Status register is not locked and writes complete 
 *              normally.
 *              false: Status register is locked and writes are ignored.
 */
static inline bool rtc_hal_get_status_reg_lock(void)
{
 /* Reads the value of the RTC Lock Register (RTC_LR), field Status Register 
  * Lock (SRL). Once cleared, this flag can only be set by VBAT POR or software
  * reset. */
  return (bool)BR_RTC_LR_SRL;
}

/*! @brief      Changes the state of the status register lock. Once cleared,
 *              this can only be set by the VBAT POR or the software reset.
 *  @param      set_to true: Status register is not locked and writes complete 
 *              normally.
 *              false: Status register is locked and writes are ignored.
 */
static inline void rtc_hal_set_status_reg_lock(bool set_to)
{
  BW_RTC_LR_SRL((uint32_t) set_to);
}

/*! @brief      Obtains the state of the control register lock
 *  @return     true: Control register is not locked and writes complete 
 *              normally.
 *              false: Control register is locked and writes are ignored.
 */
static inline bool rtc_hal_get_control_reg_lock(void)
{
 /* Reads the value of the RTC Lock Register (RTC_LR), field Control Register
  * Lock (CRL). Once cleared, this flag can only be set by the VBAT POR or the software
  * reset. */
  return (bool)BR_RTC_LR_CRL;
}

/*! @brief      Changes the state of the control register lock. Once cleared,
 *              this can only be set by the VBAT POR or the software reset.
 *  @param      set_to true: Control register is not locked and writes complete 
 *              normally.
 *              false: Control register is locked and writes are ignored.
 */
static inline void rtc_hal_set_control_reg_lock(bool set_to)
{
 /* Writes to the RTC Lock Register (RTC_LR), field Control Register Lock (CRL).
  * Once cleared, this flag can only be set by VBAT POR or software reset. */
  BW_RTC_LR_CRL((uint32_t) set_to);
}

/*! @brief      Obtains the state of the time compensation lock.
 *  @return     true: Time compensation register is not locked and writes
 *              complete  normally.
 *              false: Time compensation register is locked and writes are
 *              ignored.
 */
static inline bool rtc_hal_get_time_comp_lock(void)
{
 /* Reads the value of the RTC Lock Register (RTC_LR), field Time Compensation
  * Lock (TCL). Once cleared, this flag can only be set by VBAT POR or software
  * reset. */
  return (bool)BR_RTC_LR_TCL;
}

/*! @brief      Changes the state of the time compensation lock. Once cleared,
 *              this can only be set by the VBAT POR or the software reset.
 *  @param      set_to true: Time compensation register is not locked and writes
 *              complete  normally.
 *              false: Time compensation register is locked and writes are
 *              ignored.
 */
static inline void rtc_hal_set_time_comp_lock(bool set_to)
{
 /* Writes to the RTC Lock Register (RTC_LR), field Time Compensation Lock (TCL).
  * Once cleared, this flag can only be set by VBAT POR or software reset. */
  BW_RTC_LR_TCL((uint32_t) set_to);
}

/*-------------------------------------------------------------------------------------------*/
/* RTC Interrupt Enable*/
/*-------------------------------------------------------------------------------------------*/

/*! @brief      Enables/disables RTC interrupts.
 *  @param      bitfields [in]  set/clear respective bitfields to enabled/disabled
 *                              interrupts. \n
 *                        [out] resulting interrupt enable state. \n
 *              Valid bitfields: \n
 *   TSIE:    Time Seconds Interrupt Enable \n
 *   TAIE:    Time Alarm Interrupt Enable \n
 *   TOIE:    Time Overflow Interrupt Enable \n
 *   TIIE:    Time Invalid Interrupt Enable \n
 * \n
 * For MCUs that have the Wakeup Pin only: \n
 *   WPON:    Wakeup Pin On (see the corresponding MCU's reference manual)\n
 * \n
 * For MCUs that have the Monotonic Counter only: \n
 *   MOIE:    Monotonic Overflow Interrupt Enable \n
 */
static inline void rtc_hal_config_interrupts(hw_rtc_ier_t * bitfields)
{
  HW_RTC_IER_WR((bitfields->U) & ( BM_RTC_IER_TSIE |  BM_RTC_IER_TAIE | BM_RTC_IER_TOIE |
    BM_RTC_IER_TIIE));

#if FSL_FEATURE_RTC_HAS_WAKEUP_PIN
    BW_RTC_IER_WPON(bitfields->B.WPON);
#endif
#if FSL_FEATURE_RTC_HAS_MONOTONIC
    BW_RTC_IER_MOIE(bitfields->B.MOIE);
#endif

  bitfields->U = HW_RTC_IER_RD();
}

/*! @brief      Checks whether the Time Seconds Interrupt is enabled/disabled.
 *  @return     true: Seconds interrupt is enabled.
 *              false: Seconds interrupt is disabled.
 */
static inline bool rtc_hal_read_seconds_int_enable(void)
{
 /* Reads the value of the RTC Interrupt Enable Register (RTC_IER), field Time
  * Seconds Interrupt Enable (TSIE). The seconds interrupt is an edge-sensitive
  * interrupt with a dedicated interrupt vector. It is generated once a second
  * and requires no software overhead (there is no corresponding status flag to 
  * clear). */
  return (bool)BR_RTC_IER_TSIE;
}

/*! @brief      Enables/disables the Time Seconds Interrupt.
 *              Note: The seconds interrupt is an edge-sensitive interrupt with a
 *              dedicated interrupt vector. It is generated once a second and
 *              requires no software overhead (there is no corresponding status
 *              flag to clear).
 *  @param      enable true: Seconds interrupt is enabled.
 *              false: Seconds interrupt is disabled.
 */
static inline void rtc_hal_config_seconds_int(bool enable)
{
 /* Writes to the RTC Interrupt Enable Register (RTC_IER), field Time Seconds 
  * Interrupt Enable (TSIE). The seconds interrupt is an edge-sensitive
  * interrupt with a dedicated interrupt vector. It is generated once a second
  * and requires no software overhead (there is no corresponding status flag to
  * clear). */
  BW_RTC_IER_TSIE((uint32_t) enable);
}

/*! @brief      Checks whether the Time Alarm Interrupt is enabled/disabled.
 *  @return     true: Time alarm flag does generate an interrupt.
 *              false: Time alarm flag does not generate an interrupt.
 */
static inline bool rtc_hal_read_alarm_int_enable(void)
{
 /* Reads the value of the RTC Interrupt Enable Register (RTC_IER),
  * field Time Alarm Interrupt Enable (TAIE). */
  return (bool)BR_RTC_IER_TAIE;
}

/*! @brief      Enables/disables the Time Alarm Interrupt.
 *  @param      enable true: Time alarm flag does generate an interrupt.
 *              false: Time alarm flag does not generate an interrupt.
 */
static inline void rtc_hal_config_alarm_int_enable(bool enable)
{
 /* Writes to the RTC Interrupt Enable Register (RTC_IER), field Time Alarm
  * Interrupt Enable (TAIE). */
  BW_RTC_IER_TAIE((uint32_t) enable);
}

/*! @brief      Checks whether the Time Overflow Interrupt is enabled/disabled .
 *  @return     true: Time overflow flag does generate an interrupt.
 *              false: Time overflow flag does not generate an interrupt.
 */
static inline bool rtc_hal_read_time_overflow_int_enable(void)
{
 /* Reads the value of the RTC Interrupt Enable Register (RTC_IER), field 
  * Time Overflow Interrupt Enable (TOIE). */
  return (bool)BR_RTC_IER_TOIE;
}

/*! @brief      Enables/disables the Time Overflow Interrupt.
 *  @param      enable true: Time overflow flag does generate an interrupt.
 *              false: Time overflow flag does not generate an interrupt.
 */
static inline void rtc_hal_config_time_overflow_int_enable(bool enable)
{
 /* Writes to the RTC Interrupt Enable Register (RTC_IER),
  * field Time Overflow Interrupt Enable (TOIE). */
  BW_RTC_IER_TOIE((uint32_t) enable);
}

/*! @brief      Checks whether the Time Invalid Interrupt is enabled/disabled.
 *  @return     true: Time invalid flag does generate an interrupt.
 *              false: Time invalid flag does not generate an interrupt.
 */
static inline bool rtc_hal_read_time_interval_int_enable(void)
{
 /* Reads the value of the RTC Interrupt Enable Register (RTC_IER), field Time 
  * Invalid Interrupt Enable (TIIE). */
  return (bool)BR_RTC_IER_TIIE;
}

/*! @brief      Enables/disables the Time Invalid Interrupt.
 *  @param      enable true: Time invalid flag does generate an interrupt.
 *              false: Time invalid flag does not generate an interrupt.
 */
static inline void rtc_hal_config_time_interval_int(bool enable)
{
 /* writes to the RTC Interrupt Enable Register (RTC_IER), field Time Invalid 
  * Interrupt Enable (TIIE). */
  BW_RTC_IER_TIIE((uint32_t) enable);
}


#if defined(__cplusplus)
}
#endif
  

/*! @}*/

#endif /* __FSL_RTC_HAL_H__*/

/*******************************************************************************
 * EOF
 ******************************************************************************/

