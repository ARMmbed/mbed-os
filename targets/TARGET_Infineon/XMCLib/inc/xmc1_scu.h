/**
 * @file xmc1_scu.h
 * @date 2017-08-03
 *
 * @cond
 *********************************************************************************************************************
 * XMClib v2.1.16 - XMC Peripheral Driver Library 
 *
 * Copyright (c) 2015-2017, Infineon Technologies AG
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without modification,are permitted provided that the
 * following conditions are met:
 *
 * Redistributions of source code must retain the above copyright notice, this list of conditions and the following
 * disclaimer.
 *
 * Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the following
 * disclaimer in the documentation and/or other materials provided with the distribution.
 *
 * Neither the name of the copyright holders nor the names of its contributors may be used to endorse or promote
 * products derived from this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES,
 * INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE  FOR ANY DIRECT, INDIRECT, INCIDENTAL,
 * SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
 * SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
 * WHETHER IN CONTRACT, STRICT LIABILITY,OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
 * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *
 * To improve the quality of the software, users are encouraged to share modifications, enhancements or bug fixes with
 * Infineon Technologies AG dave@infineon.com).
 *********************************************************************************************************************
 *
 * Change History
 * --------------
 *
 * 2015-02-20:
 *     - Initial
 *
 * 2015-05-20:
 *     - Description updated <br>
 *
 * 2015-06-20:
 *     - XMC_SCU_INTERRUPT_EVENT enum elements are typecasted to int64_t
 *
 * 2015-06-20:
 *     - Added support for XMC1400
 *
 * 2015-11-30:
 *     - Documentation improved
 *
 * 2015-12-09:
 *     - Added XMC_SCU_EnablePrefetchUnit and XMC_SCU_DisablePrefetchUnit
 * 
 * 2016-03-09:
 *     - Added XMC_SCU_POWER_EnableMonitor/XMC_SCU_POWER_DisableMonitor
 *
 * 2017-03-28:
 *     - Fixed prescaler formula comments for XMC_SCU_CLOCK_EnableDCO1ExtRefCalibration()
 *     - Added XMC_SCU_SetBMI()
 *
 * 2017-08-03:
 *     - Removed unused XMC_SCU_CLOCK_DEEP_SLEEP_t
 *     - Added XMC_SCU_CLOCK_EnableFlashPowerDown() and XMC_SCU_CLOCK_DisableFlashPowerDown()
 *  
 * @endcond
 *
 */

#ifndef XMC1_SCU_H
#define XMC1_SCU_H

/*********************************************************************************************************************
 * HEADER FILES
 ********************************************************************************************************************/
#include "xmc_common.h"

#if UC_FAMILY == XMC1

/**
 * @addtogroup XMClib XMC Peripheral Library
 * @{
 */

/**
 * @addtogroup SCU
 * @{
 */

/*********************************************************************************************************************
 * MACROS
 ********************************************************************************************************************/

/*
 * List of events
 */
#define XMC_SCU_INTERRUPT_EVENT_WDT_WARN         SCU_INTERRUPT_SRMSK_PRWARN_Msk /**< WDT pre-warning event. */
#define XMC_SCU_INTERRUPT_EVENT_RTC_PERIODIC     SCU_INTERRUPT_SRCLR_PI_Msk     /**< RTC periodic event. */
#define XMC_SCU_INTERRUPT_EVENT_RTC_ALARM        SCU_INTERRUPT_SRCLR_AI_Msk     /**< RTC alarm event. */
#define XMC_SCU_INTERRUPT_EVENT_VDDPI            SCU_INTERRUPT_SRMSK_VDDPI_Msk  /**< VDDP pre-warning event. */
#if defined(COMPARATOR)
#define XMC_SCU_INTERRUPT_EVENT_ACMP0            SCU_INTERRUPT_SRMSK_ACMP0I_Msk /**< Analog comparator-0 output event. */
#define XMC_SCU_INTERRUPT_EVENT_ACMP1            SCU_INTERRUPT_SRMSK_ACMP1I_Msk /**< Analog comparator-1 output event. */
#define XMC_SCU_INTERRUPT_EVENT_ACMP2            SCU_INTERRUPT_SRMSK_ACMP2I_Msk /**< Analog comparator-2 output event. */
#if (UC_SERIES == XMC14) || defined(DOXYGEN)
#define XMC_SCU_INTERRUPT_EVENT_ACMP3            (((int64_t)SCU_INTERRUPT_SRMSK1_ACMP3I_Msk) << 32U) /**< Analog comparator-3 output event. @note Only available for XMC1400 series */
#endif
#endif
#define XMC_SCU_INTERRUPT_EVENT_VDROP            SCU_INTERRUPT_SRMSK_VDROPI_Msk  /**< VDROP event. */
#if UC_SERIES != XMC11
#define XMC_SCU_INTERRUPT_EVENT_ORC0             SCU_INTERRUPT_SRMSK_ORC0I_Msk   /**< Out of range comparator-0 event. */
#define XMC_SCU_INTERRUPT_EVENT_ORC1             SCU_INTERRUPT_SRMSK_ORC1I_Msk   /**< Out of range comparator-1 event. */
#define XMC_SCU_INTERRUPT_EVENT_ORC2             SCU_INTERRUPT_SRMSK_ORC2I_Msk   /**< Out of range comparator-2 event. */
#define XMC_SCU_INTERRUPT_EVENT_ORC3             SCU_INTERRUPT_SRMSK_ORC3I_Msk   /**< Out of range comparator-3 event. */
#define XMC_SCU_INTERRUPT_EVENT_ORC4             SCU_INTERRUPT_SRMSK_ORC4I_Msk   /**< Out of range comparator-4 event. */
#define XMC_SCU_INTERRUPT_EVENT_ORC5             SCU_INTERRUPT_SRMSK_ORC5I_Msk   /**< Out of range comparator-5 event. */
#define XMC_SCU_INTERRUPT_EVENT_ORC6             SCU_INTERRUPT_SRMSK_ORC6I_Msk   /**< Out of range comparator-6 event. */
#define XMC_SCU_INTERRUPT_EVENT_ORC7             SCU_INTERRUPT_SRMSK_ORC7I_Msk   /**< Out of range comparator-7 event. */
#endif
#define XMC_SCU_INTERRUPT_EVENT_LOCI             SCU_INTERRUPT_SRMSK_LOCI_Msk    /**< Loss of clock event. */
#define XMC_SCU_INTERRUPT_EVENT_PESRAM           SCU_INTERRUPT_SRMSK_PESRAMI_Msk /**< PSRAM Parity error event. */
#define XMC_SCU_INTERRUPT_EVENT_PEUSIC0          SCU_INTERRUPT_SRMSK_PEU0I_Msk   /**< USIC0 Parity error event. */
#if defined(USIC1)
#define XMC_SCU_INTERRUPT_EVENT_PEUSIC1          (((int64_t)SCU_INTERRUPT_SRMSK1_PEU1I_Msk) << 32U)   /**< USIC1 Parity error event. */
#endif
#define XMC_SCU_INTERRUPT_EVENT_FLASH_ERROR      SCU_INTERRUPT_SRMSK_FLECC2I_Msk /**< Flash ECC double bit error event. */
#define XMC_SCU_INTERRUPT_EVENT_FLASH_COMPLETED  SCU_INTERRUPT_SRCLR_FLCMPLTI_Msk /**< Flash operation completion event. */
#define XMC_SCU_INTERRUPT_EVENT_VCLIP            SCU_INTERRUPT_SRMSK_VCLIPI_Msk  /**< VCLIP event. */
#define XMC_SCU_INTERRUPT_EVENT_STDBYCLKFAIL     SCU_INTERRUPT_SRMSK_SBYCLKFI_Msk  /**< Standby clock failure event. */
#define XMC_SCU_INTERRUPT_EVENT_RTCCTR_UPDATED   SCU_INTERRUPT_SRMSK_RTC_CTR_Msk  /**< RTCCTR register update event. */
#define XMC_SCU_INTERRUPT_EVENT_RTCATIM0_UPDATED SCU_INTERRUPT_SRMSK_RTC_ATIM0_Msk  /**< RTCATIM0 register update event. */
#define XMC_SCU_INTERRUPT_EVENT_RTCATIM1_UPDATED SCU_INTERRUPT_SRMSK_RTC_ATIM1_Msk  /**< RTCATIM1 register update event. */
#define XMC_SCU_INTERRUPT_EVENT_RTCTIM0_UPDATED  SCU_INTERRUPT_SRMSK_RTC_TIM0_Msk  /**< RTCTIM0 register update event. */
#define XMC_SCU_INTERRUPT_EVENT_RTCTIM1_UPDATED  SCU_INTERRUPT_SRMSK_RTC_TIM1_Msk  /**< RTCTIM1 register update event. */
#define XMC_SCU_INTERRUPT_EVENT_TSE_DONE         SCU_INTERRUPT_SRMSK_TSE_DONE_Msk  /**< Temperature measurement Completion event. */
#define XMC_SCU_INTERRUPT_EVENT_TSE_HIGH         SCU_INTERRUPT_SRMSK_TSE_HIGH_Msk  /**< Temperature too high event. */
#define XMC_SCU_INTERRUPT_EVENT_TSE_LOW          SCU_INTERRUPT_SRMSK_TSE_LOW_Msk /**< Temperature too low event. */
#if defined(CAN)
#define XMC_SCU_INTERRUPT_EVENT_PEMCAN           (((int64_t)SCU_INTERRUPT_SRMSK1_PEMCI_Msk) << 32U)   /**< MultiCAN SRAM Parity Error Event. */
#endif
#if (UC_SERIES == XMC14) || defined(DOXYGEN)
#define XMC_SCU_INTERRUPT_EVENT_LOSS_EXT_CLOCK   (((int64_t)SCU_INTERRUPT_SRMSK1_LOECI_Msk) << 32U)  /**< Loss of external OSC_HP clock event. @note Only available for XMC1400 series */
#define XMC_SCU_INTERRUPT_EVENT_DCO1_OUT_SYNC    (((int64_t)SCU_INTERRUPT_SRMSK1_DCO1OFSI_Msk) << 32U)   /**< DCO1 Out of SYNC Event.  @note Only available for XMC1400 series */
#endif

/*
 *  These are the flags which may be passed to XMC_SCU_SetBMI().
 */
#if defined(CAN) || defined(DOXYGEN)
#define XMC_SCU_BMI_HWCFG_CAN_BSL      (0x0080U) /**< CAN Bootstrap Loader Start-up Mode */
#define XMC_SCU_BMI_HWCFG_CAN_BSLTO    (0x0090U) /**< CAN Bootstrap Loader Start-up Mode with time-out */
#define XMC_SCU_BMI_HWCFG_SBSL_CANOPEN (0x00A0U) /**< Secure Bootstrap Loader Start-up Mode over CANopen */
#endif

#define XMC_SCU_BMI_HWCFG_ASC_BSL      (0x00C0U) /**< ASC Bootstrap Loader Start-up Mode */
#define XMC_SCU_BMI_HWCFG_UPM          (0x00C1U) /**< User productive Start-up Mode */
#define XMC_SCU_BMI_HWCFG_UMD          (0x00C3U) /**< User Start-up Mode with debug enabled */
#define XMC_SCU_BMI_HWCFG_UMHAR        (0x00C7U) /**< User Start-up Mode with debug enabled and halt after reset (HAR) */
#define XMC_SCU_BMI_HWCFG_SSC_BSL      (0x00C8U) /**< SSC Bootstrap Loader Start-up Mode */
#define XMC_SCU_BMI_HWCFG_ASC_BSLTO    (0x00D0U) /**< ASC BSL Start-up Mode with time-out */
#define XMC_SCU_BMI_HWCFG_SSC_BSLTO    (0x00D8U) /**< SSC BSL Start-up Mode with time-out */
#define XMC_SCU_BMI_HWCFG_SBSL         (0x00FAU) /**< Secure Bootstrap Loader Start-up Mode over ASC */

#define XMC_SCU_BMI_DAPTYP_SWD         (0U << 8) /**< Serial wire debug (SWD) interface is selected */
#define XMC_SCU_BMI_DAPTYP_SPD         (1U << 8) /**< Single pin debug (SPD) interface is selected */

#define XMC_SCU_BMI_DAPDIS_CHANNEL_0   (0U << 9) /**< SWD/SPD_0 pin is selected */
#define XMC_SCU_BMI_DAPDIS_CHANNEL_1   (1U << 9) /**< SWD/SPD_1 pin is selected */

#if defined(CAN) || defined(DOXYGEN)
#define XMC_SCU_BMI_CANCLK_DCO1        (0U << 11) /**< Synchronous CAN clock via internal oscillator (DCO1) with enabled trimming via external reference is selected */
#define XMC_SCU_BMI_CANCLK_OSCHP       (1U << 11) /**< Synchronous CAN clock via external oscillator (OSC_HP) is selected */
#endif

/*********************************************************************************************************************
 * ENUMS
 ********************************************************************************************************************/

/**
 *  Defines the cause of last reset. The cause of last reset gets automatically stored in
 *  the \a SCU_RSTSTAT register and can be checked by user software to determine the state of the system and for debug
 *  purpose. All the enum items are tabulated as per bits present in \a SCU_RSTSTAT register.
 *  Use type \a XMC_SCU_RESET_REASON_t for accessing these enum parameters.
 */
typedef enum XMC_SCU_RESET_REASON
{
  XMC_SCU_RESET_REASON_PORST        = (1UL << SCU_RESET_RSTSTAT_RSTSTAT_Pos),  /**< Reset due to Power On reset. */
  XMC_SCU_RESET_REASON_MASTER       = (2UL << SCU_RESET_RSTSTAT_RSTSTAT_Pos),  /**< Reset due to Master reset. */
  XMC_SCU_RESET_REASON_SW           = (4UL << SCU_RESET_RSTSTAT_RSTSTAT_Pos),  /**< Reset due to Software initiated reset. */
  XMC_SCU_RESET_REASON_LOCKUP       = (8UL << SCU_RESET_RSTSTAT_RSTSTAT_Pos),  /**< Reset due to CPU lockup. */
  XMC_SCU_RESET_REASON_FLASH        = (16UL << SCU_RESET_RSTSTAT_RSTSTAT_Pos), /**< Reset due to flash error. */
  XMC_SCU_RESET_REASON_WATCHDOG     = (32UL << SCU_RESET_RSTSTAT_RSTSTAT_Pos), /**< Reset due to watchdog. */
  XMC_SCU_RESET_REASON_CLOCK_LOSS   = (64UL << SCU_RESET_RSTSTAT_RSTSTAT_Pos), /**< Reset due to clock loss. */
  XMC_SCU_RESET_REASON_PARITY_ERROR = (128UL << SCU_RESET_RSTSTAT_RSTSTAT_Pos) /**< Reset due to RAM parity error. */
} XMC_SCU_RESET_REASON_t;

/**
 *  Defines the reset sources that can cause device reset. These enums can be used to configure reset source in reset
 *  control \a RSTCON register which enables different reset sources to identify the reset cause. The \a SCU_RSTSTAT
 *  register can be checked by user software to determine the state of the system and for debug
 *  purpose. Use type \a XMC_SCU_SYSTEM_RESET_REQUEST_t for accessing these enum parameters.
 */
typedef enum XMC_SCU_SYSTEM_RESET_REQUEST
{
  XMC_SCU_RESET_REQUEST_FLASH_ECC_ERROR = SCU_RESET_RSTCON_ECCRSTEN_Msk,        /**< Reset when ECC double bit error occurs.*/
  XMC_SCU_RESET_REQUEST_CLOCK_LOSS = SCU_RESET_RSTCON_LOCRSTEN_Msk,             /**< Reset when loss of clock occurs.*/
  XMC_SCU_RESET_REQUEST_SRAM_PARITY_ERROR = SCU_RESET_RSTCON_SPERSTEN_Msk,      /**< Reset when SRAM parity error occurs.*/
  XMC_SCU_RESET_REQUEST_USIC_SRAM_PARITY_ERROR = SCU_RESET_RSTCON_U0PERSTEN_Msk /**< Reset when USIC0 memory parity error occurs.*/
} XMC_SCU_SYSTEM_RESET_REQUEST_t;

/**
 *  Defines list of events that can generate SCU interrupt. These enums can be used to configure events
 *  in \a SRMSK register for assertion of interrupt. All the enum items are tabulated as per bits present
 *  in \a SRMSK register. Use type \a XMC_SCU_INTERRUPT_EVENT_t for accessing these enum parameters. These
 *  enums can also be used for checking the status of events from the \a SRSTAT register.
 */
#if (UC_SERIES == XMC14)
typedef uint64_t XMC_SCU_INTERRUPT_EVENT_t;
#else
typedef uint32_t XMC_SCU_INTERRUPT_EVENT_t;
#endif

/**
 *  Defines possible sources of RTC clock. These enums can be used to configure \a RTCCLKSEL bits of \a CLKCR Clock Control
 *  Register. Use type \a XMC_SCU_CLOCK_RTCCLKSRC_t for accessing these enum parameters.
 */
typedef enum XMC_SCU_CLOCK_RTCCLKSRC
{
  XMC_SCU_CLOCK_RTCCLKSRC_DCO2 = (0x0UL << SCU_CLK_CLKCR_RTCCLKSEL_Pos), /**< RTC clock source is standby clock. */
  XMC_SCU_CLOCK_RTCCLKSRC_ERU_IOUT0 = (0x1UL << SCU_CLK_CLKCR_RTCCLKSEL_Pos), /**< RTC clock source is external clock from
                                                                                   ERU0.IOUT0. */
  XMC_SCU_CLOCK_RTCCLKSRC_ACMP0_OUT = (0x2UL << SCU_CLK_CLKCR_RTCCLKSEL_Pos), /**< RTC clock source is external clock from
                                                                                   ACMP0.OUT. */
  XMC_SCU_CLOCK_RTCCLKSRC_ACMP1_OUT = (0x3UL << SCU_CLK_CLKCR_RTCCLKSEL_Pos), /**< RTC clock source is external clock from
                                                                                  ACMP1.OUT. */
  XMC_SCU_CLOCK_RTCCLKSRC_ACMP2_OUT = (0x4UL << SCU_CLK_CLKCR_RTCCLKSEL_Pos), /**< RTC clock source is external clock from
                                                                                  ACMP2.OUT. */
#if (UC_SERIES == XMC14) || defined(DOXYGEN)
  XMC_SCU_CLOCK_RTCCLKSRC_OSCLP = (0x5UL << SCU_CLK_CLKCR_RTCCLKSEL_Pos) /**< 32.768kHz XTAL clock via OSC_LP. @note Only available for XMC1400 series */
#endif
} XMC_SCU_CLOCK_RTCCLKSRC_t;

/**
 *  Defines possible sources of peripheral clock (PCLK). These enums can be used to configure \a PCLKSEL bits of \a CLKCR
 *  Clock Control Register. Use type \a XMC_SCU_CLOCK_PCLKSRC_t for accessing these enum parameters.
 */
typedef enum XMC_SCU_CLOCK_PCLKSRC
{
  XMC_SCU_CLOCK_PCLKSRC_MCLK = (0UL << SCU_CLK_CLKCR_PCLKSEL_Pos), /**< MCLK as the source for PCLK. */
  XMC_SCU_CLOCK_PCLKSRC_DOUBLE_MCLK = (1UL << SCU_CLK_CLKCR_PCLKSEL_Pos) /**< Source of PCLK is twice the MCLK. */
} XMC_SCU_CLOCK_PCLKSRC_t;

#if (UC_SERIES == XMC14) || defined(DOXYGEN)
/**
 * DCLK clock source selection
 * @note Only available in XMC1400 series
 */
typedef enum XMC_SCU_CLOCK_DCLKSRC
{
  XMC_SCU_CLOCK_DCLKSRC_DCO1 = 0UL << SCU_CLK_CLKCR1_DCLKSEL_Pos, /**< Internal oscillator DCO1 (48MHz) */
  XMC_SCU_CLOCK_DCLKSRC_EXT_XTAL = 1UL << SCU_CLK_CLKCR1_DCLKSEL_Pos, /**< External crystal oscillator */
} XMC_SCU_CLOCK_DCLKSRC_t;

/**
 * OSCHP mode
 * @note Only available in XMC1400 series
 */
typedef enum XMC_SCU_CLOCK_OSCHP_MODE
{
  XMC_SCU_CLOCK_OSCHP_MODE_OSC = 0UL << SCU_ANALOG_ANAOSCHPCTRL_MODE_Pos, /**< Oscillator is enabled and in active power mode with shaper enabled */
  XMC_SCU_CLOCK_OSCHP_MODE_DIRECT = 1UL << SCU_ANALOG_ANAOSCHPCTRL_MODE_Pos, /**< Oscillator in power down mode with shaper enabled */
  XMC_SCU_CLOCK_OSCHP_MODE_DISABLED = 3UL << SCU_ANALOG_ANAOSCHPCTRL_MODE_Pos, /**< Oscillator in power down mode with shaper enabled */
} XMC_SCU_CLOCK_OSCHP_MODE_t;

/**
 * OSCLP mode
 * @note Only available in XMC1400 series
 */
typedef enum XMC_SCU_CLOCK_OSCLP_MODE
{
  XMC_SCU_CLOCK_OSCLP_MODE_OSC = 0UL << SCU_ANALOG_ANAOSCLPCTRL_MODE_Pos, /**< Oscillator is enabled and in active power mode with shaper enabled */
  XMC_SCU_CLOCK_OSCLP_MODE_DISABLED = 3UL << SCU_ANALOG_ANAOSCLPCTRL_MODE_Pos, /**< Oscillator in power down mode with shaper enabled */
} XMC_SCU_CLOCK_OSCLP_MODE_t;

/**
 * Clock source for synchronization
 * @note Only available in XMC1400 series
 */
typedef enum XMC_SCU_CLOCK_SYNC_CLKSRC
{
  XMC_SCU_CLOCK_SYNC_CLKSRC_OSCLP = 0U << SCU_ANALOG_ANASYNC1_XTAL_SEL_Pos,
  XMC_SCU_CLOCK_SYNC_CLKSRC_OSCHP = 1U << SCU_ANALOG_ANASYNC1_XTAL_SEL_Pos,
} XMC_SCU_CLOCK_SYNC_CLKSRC_t;

#endif /* (UC_SERIES == XMC14) */

/**
 *  Defines the list of peripherals that support clock gating. After a master reset, only core, memories, SCU
 *  and PORT peripheral are not clock gated. The rest of the peripherals are by default clock gated.
 *  All the enum items are tabulated as per bits present in \a CGATSTAT0 register.
 *  Use type \a XMC_SCU_PERIPHERAL_CLOCK_t for accessing these enum parameters.
 *  @note Peripherals availability depends on device
 */
typedef enum XMC_SCU_PERIPHERAL_CLOCK
{
#if defined(VADC)
  XMC_SCU_PERIPHERAL_CLOCK_VADC   = SCU_CLK_CGATSTAT0_VADC_Msk, /**< VADC peripheral clock gate. */
#endif
#if defined(CCU80)
  XMC_SCU_PERIPHERAL_CLOCK_CCU80  = SCU_CLK_CGATSTAT0_CCU80_Msk, /**< CCU80 peripheral clock gate. */
#endif
#if defined(CCU40)
  XMC_SCU_PERIPHERAL_CLOCK_CCU40  = SCU_CLK_CGATSTAT0_CCU40_Msk, /**< CCU40 peripheral clock gate. */
#endif
#if defined(USIC0)
  XMC_SCU_PERIPHERAL_CLOCK_USIC0  = SCU_CLK_CGATSTAT0_USIC0_Msk, /**< USIC0 peripheral clock gate. */
#endif
#if defined(BCCU0)
  XMC_SCU_PERIPHERAL_CLOCK_BCCU0  = SCU_CLK_CGATSTAT0_BCCU0_Msk, /**< BCCU0 peripheral clock gate. */
#endif
#if defined(LEDTS0)
  XMC_SCU_PERIPHERAL_CLOCK_LEDTS0 = SCU_CLK_CGATSTAT0_LEDTS0_Msk, /**< LEDTS0 peripheral clock gate. */
#endif
#if defined(LEDTS1)
  XMC_SCU_PERIPHERAL_CLOCK_LEDTS1 = SCU_CLK_CGATSTAT0_LEDTS1_Msk, /**< LEDTS1 peripheral clock gate. */
#endif
#if defined(POSIF0)
  XMC_SCU_PERIPHERAL_CLOCK_POSIF0 = SCU_CLK_CGATSTAT0_POSIF0_Msk, /**< POSIF0 peripheral clock gate. */
#endif
#if defined(MATH)
  XMC_SCU_PERIPHERAL_CLOCK_MATH   = SCU_CLK_CGATSTAT0_MATH_Msk, /**< MATH peripheral clock gate. */
#endif
  XMC_SCU_PERIPHERAL_CLOCK_WDT    = SCU_CLK_CGATSTAT0_WDT_Msk, /**< WDT peripheral clock gate. */
  XMC_SCU_PERIPHERAL_CLOCK_RTC    = SCU_CLK_CGATSTAT0_RTC_Msk, /**< RTC peripheral clock gate. */
#if defined(CCU81)
  XMC_SCU_PERIPHERAL_CLOCK_CCU81  = SCU_CLK_CGATSTAT0_CCU81_Msk, /**< CCU80 peripheral clock gate. */
#endif
#if defined(CCU41)
  XMC_SCU_PERIPHERAL_CLOCK_CCU41  = SCU_CLK_CGATSTAT0_CCU41_Msk, /**< CCU80 peripheral clock gate. */
#endif
#if defined(USIC1)
  XMC_SCU_PERIPHERAL_CLOCK_USIC1  = SCU_CLK_CGATSTAT0_USIC1_Msk, /**< USIC0 peripheral clock gate. */
#endif
#if defined(LEDTS2)
  XMC_SCU_PERIPHERAL_CLOCK_LEDTS2 = SCU_CLK_CGATSTAT0_LEDTS2_Msk, /**< LEDTS1 peripheral clock gate. */
#endif
#if defined(POSIF1)
  XMC_SCU_PERIPHERAL_CLOCK_POSIF1 = SCU_CLK_CGATSTAT0_POSIF1_Msk, /**< POSIF0 peripheral clock gate. */
#endif
#if defined(CAN)
  XMC_SCU_PERIPHERAL_CLOCK_MCAN = SCU_CLK_CGATSTAT0_MCAN0_Msk, /**< POSIF0 peripheral clock gate. */
#endif
} XMC_SCU_PERIPHERAL_CLOCK_t;


/**
 *  Defines options for Capture/Compare unit timer slice trigger that enables synchronous start function available on the \a SCU,
 *  \a CCUCON register. Use type \a XMC_SCU_CCU_TRIGGER_t for accessing these enum parameters.
 */
typedef enum XMC_SCU_CCU_TRIGGER
{
  XMC_SCU_CCU_TRIGGER_CCU40 = SCU_GENERAL_CCUCON_GSC40_Msk, /**< Trigger CCU40 peripheral. */
#if defined(CCU80)
  XMC_SCU_CCU_TRIGGER_CCU80 = SCU_GENERAL_CCUCON_GSC80_Msk, /**< Trigger CCU80 peripheral. */
#endif
#if defined(CCU41)
  XMC_SCU_CCU_TRIGGER_CCU41 = SCU_GENERAL_CCUCON_GSC41_Msk, /**< Trigger CCU40 peripheral. */
#endif
#if defined(CCU81)
  XMC_SCU_CCU_TRIGGER_CCU81 = SCU_GENERAL_CCUCON_GSC81_Msk, /**< Trigger CCU80 peripheral. */
#endif
} XMC_SCU_CCU_TRIGGER_t;

#if (UC_SERIES == XMC14) || defined(DOXYGEN)
/**
 * Selects the service request connected to the interrupt node.
 * @image html "xmc1400_irqmux.png"
 * @note Only available for XMC1400 series
 */
typedef enum XMC_SCU_IRQCTRL
{
  XMC_SCU_IRQCTRL_SCU_SR0_IRQ0              = (0U << 8U) | 0U, /**< SCU_SR0 connected to IRQ0 */
#if defined(CAN)
  XMC_SCU_IRQCTRL_CAN0_SR0_IRQ0             = (0U << 8U) | 1U, /**< CAN0_SR0 connected to IRQ0 */
#endif
  XMC_SCU_IRQCTRL_CCU40_SR0_IRQ0            = (0U << 8U) | 2U, /**< CCU40_SR0 connected to IRQ0 */
#if defined(CAN)
  XMC_SCU_IRQCTRL_SCU_SR0_OR_CAN0_SR0_IRQ0  = (0U << 8U) | 3U, /**< SCU_SR0 and CAN_SR0 are both connected to IRQ0*/
#endif

  XMC_SCU_IRQCTRL_SCU_SR1_IRQ1              = (1U << 8U) | 0U, /**< SCU_SR1 connected to IRQ1 */
#if defined(CAN)
  XMC_SCU_IRQCTRL_CAN0_SR1_IRQ1             = (1U << 8U) | 1U, /**< CAN0_SR1 connected to IRQ1 */
#endif
#if defined(CCU80)
  XMC_SCU_IRQCTRL_CCU80_SR0_IRQ1            = (1U << 8U) | 2U, /**< CCU80_SR0 connected to IRQ1 */
#endif
#if defined(CAN)
  XMC_SCU_IRQCTRL_SCU_SR1_OR_CAN0_SR1_IRQ1  = (1U << 8U) | 3U, /**< SCU_SR1 and CAN0_SR1 connected to IRQ1 */
#endif

  XMC_SCU_IRQCTRL_SCU_SR2_IRQ2              = (2U << 8U) | 0U, /**< SCU_SR2 connected to IRQ2 */
#if defined(CAN)
  XMC_SCU_IRQCTRL_CAN0_SR2_IRQ2             = (2U << 8U) | 1U, /**< CAN0_SR2 connected to IRQ2 */
#endif
#if defined(CCU80)
  XMC_SCU_IRQCTRL_CCU80_SR1_IRQ2            = (2U << 8U) | 2U, /**< CCU80_SR1 connected to IRQ2 */
#endif
#if defined(CAN)
  XMC_SCU_IRQCTRL_SCU_SR2_OR_CAN0_SR2_IRQ2  = (2U << 8U) | 3U, /**< SCU_SR2 and CAN0_SR2 connected to IRQ2 */
#endif

  XMC_SCU_IRQCTRL_ERU0_SR0_IRQ3             = (3U << 8U) | 0U, /**< ERU0_SR0 connected to IRQ3 */
  XMC_SCU_IRQCTRL_ERU1_SR0_IRQ3             = (3U << 8U) | 1U, /**< ERU1_SR0 connected to IRQ3 */
#if defined(CAN)
  XMC_SCU_IRQCTRL_CAN0_SR0_IRQ3             = (3U << 8U) | 2U, /**< CAN0_SR0 connected to IRQ3 */
#endif
  XMC_SCU_IRQCTRL_ERU0_SR0_OR_ERU1_SR0_IRQ3 = (3U << 8U) | 3U, /**< ERU0_SR0 and ERU1_SR0 connected to IRQ3 */

  XMC_SCU_IRQCTRL_ERU0_SR1_IRQ4             = (4U << 8U) | 0U, /**< ERU0_SR1 connected to IRQ4 */
  XMC_SCU_IRQCTRL_ERU1_SR1_IRQ4             = (4U << 8U) | 1U, /**< ERU1_SR1 connected to IRQ4 */
#if defined(CAN)
  XMC_SCU_IRQCTRL_CAN0_SR1_IRQ4             = (4U << 8U) | 2U, /**< CAN0_SR1 connected to IRQ4 */
#endif
  XMC_SCU_IRQCTRL_ERU0_SR1_OR_ERU1_SR1_IRQ4 = (4U << 8U) | 3U, /**< ERU0_SR1 and ERU1_SR1 connected to IRQ4 */

  XMC_SCU_IRQCTRL_ERU0_SR2_IRQ5             = (5U << 8U) | 0U, /**< ERU0_SR2 connected to IRQ5 */
  XMC_SCU_IRQCTRL_ERU1_SR2_IRQ5             = (5U << 8U) | 1U, /**< ERU1_SR2 connected to IRQ5 */
#if defined(CAN)
  XMC_SCU_IRQCTRL_CAN0_SR2_IRQ5             = (5U << 8U) | 2U, /**< CAN0_SR2 connected to IRQ5 */
#endif
  XMC_SCU_IRQCTRL_ERU0_SR2_OR_ERU1_SR2_IRQ5 = (5U << 8U) | 3U, /**< ERU0_SR2 and ERU1_SR2 connected to IRQ5 */

  XMC_SCU_IRQCTRL_ERU0_SR3_IRQ6             = (6U << 8U) | 0U, /**< ERU0_SR3 connected to IRQ6 */
  XMC_SCU_IRQCTRL_ERU1_SR3_IRQ6             = (6U << 8U) | 1U, /**< ERU1_SR3 connected to IRQ6 */
#if defined(CAN)
  XMC_SCU_IRQCTRL_CAN0_SR3_IRQ6             = (6U << 8U) | 2U, /**< CAN0_SR3 connected to IRQ6 */
#endif
  XMC_SCU_IRQCTRL_ERU0_SR3_OR_ERU1_SR3_IRQ6 = (6U << 8U) | 3U, /**< ERU0_SR3 and ERU1_SR3 connected to IRQ6 */

#if defined(MATH)
  XMC_SCU_IRQCTRL_MATH_SR0_IRQ7                = (7U << 8U) | 0U, /**< MATH_SR0 connected to IRQ7 */
#endif
#if defined(CAN)
  XMC_SCU_IRQCTRL_CAN0_SR3_IRQ7                = (7U << 8U) | 1U, /**< CAN0_SR3 connected to IRQ7 */
#endif
  XMC_SCU_IRQCTRL_CCU40_SR1_IRQ7               = (7U << 8U) | 2U, /**< CCU40_SR1 connected to IRQ7 */
#if defined(MATH) && defined(CAN)
  XMC_SCU_IRQCTRL_MATH_SR0_OR_CAN0_SR3_IRQ7    = (7U << 8U) | 3U, /**< MATH_SR0 and CAN0_SR3 connected to IRQ7 */
#endif

#if defined(LEDTS2)
  XMC_SCU_IRQCTRL_LEDTS2_SR0_IRQ8              = (8U << 8U) | 0U, /**< LEDTS2_SR0 connected to IRQ8 */
#endif
  XMC_SCU_IRQCTRL_CCU40_SR0_IRQ8               = (8U << 8U) | 1U, /**< CCU40_SR0 connected to IRQ8 */
#if defined(CCU80)
  XMC_SCU_IRQCTRL_CCU80_SR0_IRQ8               = (8U << 8U) | 2U, /**< CCU80_SR0 connected to IRQ8 */
#endif
#if defined(LEDTS2)
  XMC_SCU_IRQCTRL_LEDTS2_SR0_OR_CCU40_SR0_IRQ8 = (8U << 8U) | 3U, /**< LEDTS2_SR0 and CCU40_SR0 connected to IRQ8 */
#endif

  XMC_SCU_IRQCTRL_USIC0_SR0_IRQ9               = (9U << 8U) | 0U, /**< USIC0_SR0 connected to IRQ9 */
#if defined(USIC1)
  XMC_SCU_IRQCTRL_USIC1_SR0_IRQ9               = (9U << 8U) | 1U, /**< USIC1_SR0 connected to IRQ9 */
#endif
  XMC_SCU_IRQCTRL_ERU0_SR0_IRQ9                = (9U << 8U) | 2U, /**< ERU0_SR0 connected to IRQ9 */
#if defined(USIC1)
  XMC_SCU_IRQCTRL_USIC0_SR0_OR_USIC1_SR0_IRQ9  = (9U << 8U) | 3U, /**< USIC0_SR0 and USIC1_SR0 connected to IRQ9 */
#endif

  XMC_SCU_IRQCTRL_USIC0_SR1_IRQ10              = (10U << 8U) | 0U, /**< USIC0_SR1 connected to IRQ10 */
#if defined(USIC1)
  XMC_SCU_IRQCTRL_USIC1_SR1_IRQ10              = (10U << 8U) | 1U, /**< USIC1_SR1 connected to IRQ10 */
#endif
  XMC_SCU_IRQCTRL_ERU0_SR1_IRQ10               = (10U << 8U) | 2U, /**< ERU0_SR1 connected to IRQ10 */
#if defined(USIC1)
  XMC_SCU_IRQCTRL_USIC0_SR1_OR_USIC1_SR1_IRQ10 = (10U << 8U) | 3U, /**< USIC0_SR1 and USIC1_SR1 connected to IRQ10 */
#endif

  XMC_SCU_IRQCTRL_USIC0_SR2_IRQ11              = (11U << 8U) | 0U, /**< USIC0_SR2 connected to IRQ11 */
#if defined(USIC1)
  XMC_SCU_IRQCTRL_USIC1_SR2_IRQ11              = (11U << 8U) | 1U, /**< USIC1_SR2 connected to IRQ11 */
#endif
  XMC_SCU_IRQCTRL_ERU0_SR2_IRQ11               = (11U << 8U) | 2U, /**< ERU0_SR2 connected to IRQ11 */
#if defined(USIC1)
  XMC_SCU_IRQCTRL_USIC0_SR2_OR_USIC1_SR2_IRQ11 = (11U << 8U) | 3U, /**< USIC0_SR2 and USIC1_SR2 connected to IRQ11 */
#endif

  XMC_SCU_IRQCTRL_USIC0_SR3_IRQ12              = (12U << 8U) | 0U, /**< USIC0_SR3 connected to IRQ12 */
#if defined(USIC1)
  XMC_SCU_IRQCTRL_USIC1_SR3_IRQ12              = (12U << 8U) | 1U, /**< USIC1_SR3 connected to IRQ12 */
#endif
  XMC_SCU_IRQCTRL_ERU0_SR3_IRQ12               = (12U << 8U) | 2U, /**< ERU0_SR3 connected to IRQ12 */
#if defined(USIC1)
  XMC_SCU_IRQCTRL_USIC0_SR3_OR_USIC1_SR3_IRQ12 = (12U << 8U) | 3U, /**< USIC0_SR3 and USIC1_SR3 connected to IRQ12 */
#endif

  XMC_SCU_IRQCTRL_USIC0_SR4_IRQ13              = (13U << 8U) | 0U, /**< USIC0_SR4 connected to IRQ13 */
#if defined(USIC1)
  XMC_SCU_IRQCTRL_USIC1_SR4_IRQ13              = (13U << 8U) | 1U, /**< USIC1_SR4 connected to IRQ13 */
#endif
#if defined(CCU80)
  XMC_SCU_IRQCTRL_CCU80_SR1_IRQ13              = (13U << 8U) | 2U, /**< CCU80_SR1 connected to IRQ13 */
#endif
#if defined(USIC1)
  XMC_SCU_IRQCTRL_USIC0_SR4_OR_USIC1_SR4_IRQ13 = (13U << 8U) | 3U, /**< USIC0_SR4 and USIC1_SR4 connected to IRQ13 */
#endif

  XMC_SCU_IRQCTRL_USIC0_SR5_IRQ14              = (14U << 8U) | 0U, /**< USIC0_SR5 connected to IRQ14 */
#if defined(USIC1)
  XMC_SCU_IRQCTRL_USIC1_SR5_IRQ14              = (14U << 8U) | 1U, /**< USIC1_SR5 connected to IRQ14 */
#endif
#if defined(POSIF0)
  XMC_SCU_IRQCTRL_POSIF0_SR0_IRQ14             = (14U << 8U) | 2U, /**< POSIF0_SR0 connected to IRQ14 */
#endif
#if defined(USIC1)
  XMC_SCU_IRQCTRL_USIC0_SR5_OR_USIC1_SR5_IRQ14 = (14U << 8U) | 3U, /**< USIC0_SR5 and USIC1_SR5 connected to IRQ14 */
#endif

  XMC_SCU_IRQCTRL_VADC0_C0SR0_IRQ15              = (15U << 8U) | 0U, /**< VADC0_C0SR0 connected to IRQ15 */
  XMC_SCU_IRQCTRL_USIC0_SR0_IRQ15                = (15U << 8U) | 1U, /**< USIC0_SR0 connected to IRQ15 */
#if defined(POSIF0)
  XMC_SCU_IRQCTRL_POSIF0_SR1_IRQ15               = (15U << 8U) | 2U, /**< POSIF0_SR1 connected to IRQ15 */
#endif
  XMC_SCU_IRQCTRL_VADC0_C0SR0_OR_USIC0_SR0_IRQ15 = (15U << 8U) | 3U, /**< VADC0_C0SR0 and USIC0_SR0 connected to IRQ15 */

  XMC_SCU_IRQCTRL_VADC0_C0SR1_IRQ16              = (16U << 8U) | 0U, /**< VADC0_C0SR1 connected to IRQ16 */
  XMC_SCU_IRQCTRL_USIC0_SR1_IRQ16                = (16U << 8U) | 1U, /**< USIC0_SR1 connected to IRQ16 */
  XMC_SCU_IRQCTRL_CCU40_SR2_IRQ16                = (16U << 8U) | 2U, /**< CCU40_SR2 connected to IRQ16 */
  XMC_SCU_IRQCTRL_VADC0_C0SR1_OR_USIC0_SR1_IRQ16 = (16U << 8U) | 3U, /**< VADC0_C0SR1 and USIC0_SR1 connected to IRQ16 */

  XMC_SCU_IRQCTRL_VADC0_G0SR0_IRQ17              = (17U << 8U) | 0U, /**< VADC0_G0SR0 connected to IRQ17 */
  XMC_SCU_IRQCTRL_USIC0_SR2_IRQ17                = (17U << 8U) | 1U, /**< USIC0_SR2 connected to IRQ17 */
#if defined(CAN)
  XMC_SCU_IRQCTRL_CAN0_SR0_IRQ17                 = (17U << 8U) | 2U, /**< CAN0_SR0 connected to IRQ17 */
#endif
  XMC_SCU_IRQCTRL_VADC0_G0SR0_OR_USIC0_SR2_IRQ17 = (17U << 8U) | 3U, /**< VADC0_G0SR0 and USIC0_SR2 connected to IRQ17 */

  XMC_SCU_IRQCTRL_VADC0_G0SR1_IRQ18              = (18U << 8U) | 0U, /**< VADC0_G0SR1 connected to IRQ18 */
  XMC_SCU_IRQCTRL_USIC0_SR3_IRQ18                = (18U << 8U) | 1U, /**< USIC0_SR3 connected to IRQ18 */
#if defined(CAN)
  XMC_SCU_IRQCTRL_CAN0_SR1_IRQ18                 = (18U << 8U) | 2U, /**< CAN0_SR1 connected to IRQ18 */
#endif
  XMC_SCU_IRQCTRL_VADC0_G0SR1_OR_USIC0_SR3_IRQ18 = (18U << 8U) | 3U, /**< VADC0_G0SR1 and USIC0_SR3 connected to IRQ18 */

  XMC_SCU_IRQCTRL_VADC0_G1SR0_IRQ19              = (19U << 8U) | 0U, /**< VADC0_G1SR0 connected to IRQ19 */
  XMC_SCU_IRQCTRL_USIC0_SR4_IRQ19                = (19U << 8U) | 1U, /**< USIC0_SR4 connected to IRQ19 */
#if defined(CAN)
  XMC_SCU_IRQCTRL_CAN0_SR2_IRQ19                 = (19U << 8U) | 2U, /**< CAN0_SR2 connected to IRQ19 */
#endif
  XMC_SCU_IRQCTRL_VADC0_G1SR0_OR_USIC0_SR4_IRQ19 = (19U << 8U) | 3U, /**< VADC0_G1SR0 and USIC0_SR4 connected to IRQ19 */

  XMC_SCU_IRQCTRL_VADC0_G1SR1_IRQ20              = (20U << 8U) | 0U, /**< VADC0_G1SR1 connected to IRQ20 */
  XMC_SCU_IRQCTRL_USIC0_SR5_IRQ20                = (20U << 8U) | 1U, /**< USIC0_SR5 connected to IRQ20 */
#if defined(CAN)
  XMC_SCU_IRQCTRL_CAN0_SR4_IRQ20                 = (20U << 8U) | 2U, /**< CAN0_SR4 connected to IRQ20 */
#endif
  XMC_SCU_IRQCTRL_VADC0_G1SR1_OR_USIC0_SR5_IRQ20 = (20U << 8U) | 3U, /**< VADC0_G1SR1 and USIC0_SR5 connected to IRQ20 */

  XMC_SCU_IRQCTRL_CCU40_SR0_IRQ21                = (21U << 8U) | 0U, /**< CCU40_SR0 connected to IRQ21 */
#if defined(CCU41)
  XMC_SCU_IRQCTRL_CCU41_SR0_IRQ21                = (21U << 8U) | 1U, /**< CCU41_SR0 connected to IRQ21 */
#endif
  XMC_SCU_IRQCTRL_USIC0_SR0_IRQ21                = (21U << 8U) | 2U, /**< USIC0_SR0 connected to IRQ21 */
#if defined(CCU41)
  XMC_SCU_IRQCTRL_CCU40_SR0_OR_CCU41_SR0_IRQ21   = (21U << 8U) | 3U, /**< CCU40_SR0 and CCU41_SR0 connected to IRQ21 */
#endif

  XMC_SCU_IRQCTRL_CCU40_SR1_IRQ22                = (22U << 8U) | 0U, /**< CCU40_SR1 connected to IRQ22 */
#if defined(CCU41)
  XMC_SCU_IRQCTRL_CCU41_SR1_IRQ22                = (22U << 8U) | 1U, /**< CCU41_SR1 connected to IRQ22 */
#endif
  XMC_SCU_IRQCTRL_USIC0_SR1_IRQ22                = (22U << 8U) | 2U, /**< USIC0_SR1 connected to IRQ22 */
#if defined(CCU41)
  XMC_SCU_IRQCTRL_CCU40_SR0_OR_CCU41_SR0_IRQ22   = (22U << 8U) | 3U, /**< CCU40_SR0 and CCU41_SR0 connected to IRQ22 */
#endif

  XMC_SCU_IRQCTRL_CCU40_SR2_IRQ23                = (23U << 8U) | 0U, /**< CCU40_SR2 connected to IRQ23 */
#if defined(CCU41)
  XMC_SCU_IRQCTRL_CCU41_SR2_IRQ23                = (23U << 8U) | 1U, /**< CCU41_SR2 connected to IRQ23 */
#endif
  XMC_SCU_IRQCTRL_USIC0_SR2_IRQ23                = (23U << 8U) | 2U, /**< USIC0_SR2 connected to IRQ23 */
#if defined(CCU41)
  XMC_SCU_IRQCTRL_CCU40_SR2_OR_CCU41_SR2_IRQ23   = (23U << 8U) | 3U, /**< CCU40_SR2 and CCU41_SR2 connected to IRQ23 */
#endif

  XMC_SCU_IRQCTRL_CCU40_SR3_IRQ24                = (24U << 8U) | 0U, /**< CCU40_SR3 connected to IRQ24 */
#if defined(CCU41)
  XMC_SCU_IRQCTRL_CCU41_SR3_IRQ24                = (24U << 8U) | 1U, /**< CCU41_SR3 connected to IRQ24 */
#endif
  XMC_SCU_IRQCTRL_USIC0_SR3_IRQ24                = (24U << 8U) | 2U, /**< USIC0_SR3 connected to IRQ24 */
#if defined(CCU41)
  XMC_SCU_IRQCTRL_CCU40_SR3_OR_CCU41_SR3_IRQ24   = (24U << 8U) | 3U, /**< CCU40_SR3 and CCU41_SR3 connected to IRQ24 */
#endif

#if defined(CCU80)
  XMC_SCU_IRQCTRL_CCU80_SR0_IRQ25                = (25U << 8U) | 0U, /**< CCU80_SR0 connected to IRQ25 */
#endif
#if defined(CCU81)
  XMC_SCU_IRQCTRL_CCU81_SR0_IRQ25                = (25U << 8U) | 1U, /**< CCU81_SR0 connected to IRQ25 */
#endif
  XMC_SCU_IRQCTRL_USIC0_SR4_IRQ25                = (25U << 8U) | 2U, /**< USIC0_SR4 connected to IRQ25 */
#if defined(CCU80) && defined(CCU81)
  XMC_SCU_IRQCTRL_CCU80_SR0_OR_CCU81_SR0_IRQ25   = (25U << 8U) | 3U, /**< CCU80_SR0 and CCU81_SR0 connected to IRQ25 */
#endif

#if defined(CCU80)
  XMC_SCU_IRQCTRL_CCU80_SR1_IRQ26                = (26U << 8U) | 0U, /**< CCU80_SR1 connected to IRQ26 */
#endif
#if defined(CCU81)
  XMC_SCU_IRQCTRL_CCU81_SR1_IRQ26                = (26U << 8U) | 1U, /**< CCU81_SR1 connected to IRQ26 */
#endif
  XMC_SCU_IRQCTRL_USIC0_SR5_IRQ26                = (26U << 8U) | 2U, /**< USIC0_SR5 connected to IRQ26 */
#if defined(CCU80) && defined(CCU81)
  XMC_SCU_IRQCTRL_CCU80_SR1_OR_CCU81_SR1_IRQ26   = (26U << 8U) | 3U, /**< CCU80_SR1 and CCU81_SR1 connected to IRQ26 */
#endif

#if defined(POSIF0)
  XMC_SCU_IRQCTRL_POSIF0_SR0_IRQ27               = (27U << 8U) | 0U, /**< POSIF0_SR0 connected to IRQ27 */
#endif
#if defined(POSIF1)
  XMC_SCU_IRQCTRL_POSIF1_SR0_IRQ27               = (27U << 8U) | 1U, /**< POSIF1_SR0 connected to IRQ27 */
#endif
  XMC_SCU_IRQCTRL_CCU40_SR3_IRQ27                = (27U << 8U) | 2U, /**< CCU40_SR3 connected to IRQ27 */
#if defined(POSIF0) && defined(POSIF1)
  XMC_SCU_IRQCTRL_POSIF0_SR0_OR_POSIF1_SR0_IRQ27 = (27U << 8U) | 3U, /**< POSIF0_SR0 and POSIF1_SR0 connected to IRQ27 */
#endif

#if defined(POSIF0)
  XMC_SCU_IRQCTRL_POSIF0_SR1_IRQ28               = (28U << 8U) | 0U, /**< POSIF0_SR1 connected to IRQ28 */
#endif
#if defined(POSIF1)
  XMC_SCU_IRQCTRL_POSIF1_SR1_IRQ28               = (28U << 8U) | 1U, /**< POSIF1_SR1 connected to IRQ28 */
#endif
  XMC_SCU_IRQCTRL_ERU0_SR0_IRQ28                 = (28U << 8U) | 2U, /**< ERU0_SR0 connected to IRQ28 */
#if defined(POSIF0) && defined(POSIF1)
  XMC_SCU_IRQCTRL_POSIF0_SR1_OR_POSIF1_SR1_IRQ28 = (28U << 8U) | 3U, /**< POSIF0_SR1 and POSIF1_SR1 connected to IRQ28 */
#endif

#if defined(LEDTS0)
  XMC_SCU_IRQCTRL_LEDTS0_SR0_IRQ29               = (29U << 8U) | 0U, /**< LEDTS0_SR0 connected to IRQ29 */
#endif
  XMC_SCU_IRQCTRL_CCU40_SR1_IRQ29                = (29U << 8U) | 1U, /**< CCU40_SR1 connected to IRQ29 */
  XMC_SCU_IRQCTRL_ERU0_SR1_IRQ29                 = (29U << 8U) | 2U, /**< ERU0_SR1 connected to IRQ29 */
#if defined(LEDTS0)
  XMC_SCU_IRQCTRL_LEDTS0_SR0_OR_CCU40_SR1_IRQ29  = (29U << 8U) | 3U, /**< LEDTS0_SR0 and CCU40_SR1 connected to IRQ29 */
#endif

#if defined(LEDTS1)
  XMC_SCU_IRQCTRL_LEDTS1_SR0_IRQ30               = (30U << 8U) | 0U, /**< LEDTS1_SR0 connected to IRQ30 */
#endif
  XMC_SCU_IRQCTRL_CCU40_SR2_IRQ30                = (30U << 8U) | 1U, /**< CCU40_SR2 connected to IRQ30 */
  XMC_SCU_IRQCTRL_ERU0_SR2_IRQ30                 = (30U << 8U) | 2U, /**< ERU0_SR2 connected to IRQ30 */
#if defined(LEDTS1)
  XMC_SCU_IRQCTRL_LEDTS0_SR0_OR_CCU40_SR1_IRQ30  = (30U << 8U) | 3U, /**< LEDTS0_SR0 and CCU40_SR1 connected to IRQ30 */
#endif

#if defined(BCCU0)
  XMC_SCU_IRQCTRL_BCCU0_SR0_IRQ31                = (31U << 8U) | 0U, /**< BCCU0_SR0 connected to IRQ31 */
#endif
  XMC_SCU_IRQCTRL_CCU40_SR3_IRQ31                = (31U << 8U) | 1U, /**< CCU40_SR3 connected to IRQ31 */
  XMC_SCU_IRQCTRL_ERU0_SR3_IRQ31                 = (31U << 8U) | 2U, /**< ERU0_SR3 connected to IRQ31 */
#if defined(BCCU0)
  XMC_SCU_IRQCTRL_BCCU0_SR0_OR_CCU40_SR3_IRQ31   = (31U << 8U) | 3U, /**< BCCU0_SR0 and CCU40_SR3 connected to IRQ31 */
#endif
} XMC_SCU_IRQCTRL_t;
#endif /* XMC_SERIES = XMC14 */

/** TODO
 *
 */
typedef enum XMC_SCU_POWER_MONITOR_RANGE
{
  XMC_SCU_POWER_MONITOR_RANGE_2_25V = 0U << SCU_ANALOG_ANAVDEL_VDEL_SELECT_Pos, /**< */
  XMC_SCU_POWER_MONITOR_RANGE_3_00V = 1U << SCU_ANALOG_ANAVDEL_VDEL_SELECT_Pos, /**< */
  XMC_SCU_POWER_MONITOR_RANGE_4_40V = 2U << SCU_ANALOG_ANAVDEL_VDEL_SELECT_Pos  /**< */
} XMC_SCU_POWER_MONITOR_RANGE_t;

/** TODO
 *
 */
typedef enum XMC_SCU_POWER_MONITOR_DELAY
{
  XMC_SCU_POWER_MONITOR_DELAY_1US = 0U << SCU_ANALOG_ANAVDEL_VDEL_TIM_ADJ_Pos,   /**< */
  XMC_SCU_POWER_MONITOR_DELAY_500NS = 1U << SCU_ANALOG_ANAVDEL_VDEL_TIM_ADJ_Pos, /**< */
  XMC_SCU_POWER_MONITOR_DELAY_250NS = 2U << SCU_ANALOG_ANAVDEL_VDEL_TIM_ADJ_Pos, /**< */
  XMC_SCU_POWER_MONITOR_DELAY_NONE = 3U << SCU_ANALOG_ANAVDEL_VDEL_TIM_ADJ_Pos   /**< */
} XMC_SCU_POWER_MONITOR_DELAY_t;

/*********************************************************************************************************************
 * DATA STRUCTURES
 ********************************************************************************************************************/
/**
 *  Defines a data structure for initializing the data of the supply voltage monitoring block.
 *  Supply voltage monitoring block consists of 2 detectors namely External voltage detector (VDEL) and External brownout
 *  detector (BDE) in the EVR that are used to monitor the VDDP. \a VDEL detector compares the supply voltage against a
 *  pre-warning threshold voltage.
 *  Use type \a XMC_SCU_SUPPLYMONITOR_t for accessing these structure parameters.
 */
typedef struct XMC_SCU_SUPPLYMONITOR
{
  uint32_t ext_supply_threshold;   /**<  External supply range (VDEL Range Select).\n
                                         \b Range:
                                         <ul>
                                         <li>00B sets threshold value to 2.25V
                                         <li>01B sets threshold value to 3.0V
                                         <li>10B sets threshold value to 4.4V 
                                         </ul>*/
  uint32_t ext_supply_monitor_speed; /**<  Speed of the voltage monitor(VDEL Timing Setting).\n
                                           \b Range:
                                           <ul>
                                           <li>00B sets monitor speed typ 1us - slowest response time
                                           <li>01B sets monitor speed typ 500n
                                           <li>10B sets monitor speed typ 250n
                                           <li>11B sets monitor speed with no delay - fastest response time.
                                           </ul>*/
  bool     enable_prewarning_int;    /**< Configure pre-warning interrupt generation.\n
                                          \b Range: 
                                          <ul>
                                          <li>\a true to enable the interrupt.
                                          <li>\a false to disable the interrupt.
                                          </ul>*/
  bool     enable_vdrop_int;         /**< Configure VDROP interrupt generation.\n
                                          \b Range: 
                                          <ul>
                                          <li>\a true to enable the interrupt.
                                          <li>\a false to disable the interrupt.                                          
                                          </ul>*/
  bool     enable_vclip_int;         /**< Configure VCLIP interrupt.\n
                                          \b Range: 
                                          <ul>
                                          <li>\a true to enable the interrupt.
                                          <li>\a false to disable the interrupt.                                          
                                          </ul>*/
  bool     enable_at_init;           /**< Whether the monitor has to be enabled (VDEL unit Enable) after initialization.\n
                                          \b Range: 
                                          <ul>
                                          <li>\a true to enable after initialization.
                                          <li>\a false to enable after initialization.
                                          </ul>*/
} XMC_SCU_SUPPLYMONITOR_t;

/**
 *  Defines a data structure for initializing the data of the clock functional block.
 *  Clock functional block configures clock dividers, peripheral and RTC clock source by configuring corresponding
 *  bits in \a CLKCR clock control register.
 *  Use type \a XMC_SCU_CLOCK_CONFIG_t for accessing these structure parameters.
 */
typedef struct XMC_SCU_CLOCK_CONFIG
{
#if (UC_SERIES == XMC14)
  uint16_t fdiv; /**<  Fractional clock divider (FDIV). \b Range: 0 to 1023. */
#else
  uint8_t fdiv; /**<  Fractional clock divider (FDIV). \b Range: 0 to 255. @note XMC1400 series extends the range to 1023 */
#endif
  uint8_t idiv; /**<  Integer clock divider (IDIV). \b Range: 0 to 255. */
#if (UC_SERIES == XMC14) || defined(DOXYGEN)
  XMC_SCU_CLOCK_DCLKSRC_t dclk_src; /**< DCLK clock source selection. @note Only available in XMC1400 series */
  XMC_SCU_CLOCK_OSCHP_MODE_t oschp_mode; /**< OSCHP mode. @note Only available in XMC1400 series */
  XMC_SCU_CLOCK_OSCLP_MODE_t osclp_mode; /**< OSCLP mode. @note Only available in XMC1400 series */
#endif
  XMC_SCU_CLOCK_PCLKSRC_t pclk_src; /**<  Source of PCLK Clock */
  XMC_SCU_CLOCK_RTCCLKSRC_t rtc_src; /**<  Source of RTC Clock */
} XMC_SCU_CLOCK_CONFIG_t;


/*********************************************************************************************************************
 * API PROTOTYPES
 ********************************************************************************************************************/

#ifdef __cplusplus
extern "C" {
#endif

/**
 *
 * @param freq_khz   Required MCLK frequency value in kHz.\n
 *                   \b Range: XMC11/XMC12/XMC13 Device: 125 to 32000.
 *                             XMC14 Device: 188 to 48000 when DCO1 is clock source for clock control unit.
 *                                            79 to 48000 when OSC_HP is clock source for clock control unit.\n
 *
 * @return None
 *
 * \par<b>Description</b><br>
 * This API configures main clock (MCLK) frequency to requested frequency value.\n\n
 * The API configures main clock by setting \a IDIV and \a FDIV bit's of the \a CLKCR register for
 * XMC11/XMC12/XMC13/XMC14 Device and with additional \a FDIV bit (FDIV[9:8]) of the \a CLKCR1 register settings
 * for XMC14 device.
 * \par<b>Related APIs:</b><BR>
 * XMC_SCU_CLOCK_ScaleMCLKFrequency()\n\n\n
 */
void XMC_SCU_CLOCK_SetMCLKFrequency(uint32_t freq_khz);

/**
 *
 * @param idiv  Divider value.\n
 *              \b Range: 0 to 255.\n
 * @param fdiv  Fractional Divider value.\n
 *              \b Range: XMC11/XMC12/XMC13 Device: 0 to 255.
 *                        XMC14 Device: 0 to 1023.\n
 *
 * @return None
 *
 * \par<b>Description</b><br>
 * This API configures main clock (MCLK) frequency by updating user provided divider values.\n\n
 * The API configures main clock by setting \a IDIV and \a FDIV bit's of the \a CLKCR register for
 * XMC11/XMC12/XMC13/XMC14 Device and with additional \a FDIV bit (FDIV[9:8]) of the \a CLKCR1 register settings
 * for XMC14 device.
 * \par<b>Related APIs:</b><BR>
 * XMC_SCU_CLOCK_SetMCLKFrequency()\n\n\n
 */
void XMC_SCU_CLOCK_ScaleMCLKFrequency(uint32_t idiv, uint32_t fdiv);

/**
 *
 * @return None
 *
 * \par<b>Description</b><br>
 * Locks access to protected bit fields of the SCU.\n\n
 * The bit protection scheme prevents changing selected register bits by unauthorized code.
 * Bit protection scheme is enabled by writing 000000C3H to \a PASSWD register. By writing
 * this value, the API is setting the \a MODE bit field to bit protection enabled state.\n
 * List of Protected Register Bit Fields are mentioned below. \n
 * <table>
 *   <tr><td> \a Register    </td> <td>\a Bit fields                  </td></tr>
 *   <tr><td> SCU_CLKCR      </td> <td> FDIV, IDIV, PCLKSEL, RTCLKSEL </td></tr>
 *   <tr><td> SCU_CGATSET0   </td> <td> All bits                      </td></tr>
 *   <tr><td> SCU_CGATCLR0   </td> <td> All bits                      </td></tr>
 *   <tr><td> SCU_ANAOFFSET  </td> <td> ADJL_OFFSET                   </td></tr>
 *   <tr><td> VADC0_ACCPROT0 </td> <td> All bits                      </td></tr>
 *   <tr><td> VADC0_ACCPROT1 </td> <td> All bits                      </td></tr>
 * </table>
 *
 * \par<b>Related APIs:</b><BR>
 * XMC_SCU_UnlockProtectedBits() \n\n\n
 */
void XMC_SCU_LockProtectedBits(void);

/**
 *
 * @return None
 *
 * \par<b>Description</b><br>
 * Unlocks access to protected bit fields of the SCU.\n\n
 * The bit protection scheme prevents changing selected register bits by unauthorized code.
 * Bit protection scheme can be temporarily(for 32 MCLK cycles) disabled by writing 000000C0H to \a PASSWD register.
 * By writing this value, the API is setting the \a MODE bit field to bit protection disabled state. The API
 * waits for the protection to be disabled after changing the \a MODE.\n
 * User can change the values of the protected bit fields within 32 MCLK cycles. After 32 MCLK cycles the lock will
 * be enabled automatically.
 * List of Protected Register Bit Fields are mentioned below. \n
 * <table>
 *   <tr><td> \a Register    </td> <td>\a Bit fields                  </td></tr>
 *   <tr><td> SCU_CLKCR      </td> <td> FDIV, IDIV, PCLKSEL, RTCLKSEL </td></tr>
 *   <tr><td> SCU_CGATSET0   </td> <td> All bits                      </td></tr>
 *   <tr><td> SCU_CGATCLR0   </td> <td> All bits                      </td></tr>
 *   <tr><td> SCU_ANAOFFSET  </td> <td> ADJL_OFFSET                   </td></tr>
 *   <tr><td> VADC0_ACCPROT0 </td> <td> All bits                      </td></tr>
 *   <tr><td> VADC0_ACCPROT1 </td> <td> All bits                      </td></tr>
 * </table>
 *
 * \par<b>Related APIs:</b><BR>
 * XMC_SCU_LockProtectedBits() \n\n\n
 */
void XMC_SCU_UnlockProtectedBits(void);

/**
 *
 * @param obj   Pointer to data structure consisting voltage monitoring block configuration.\n
 *               \b Range: Use type @ref XMC_SCU_SUPPLYMONITOR_t for detailed description of structure members.
 *
 * @return None
 *
 * \par<b>Description</b><br>
 * Initializes power supply monitoring unit.\n\n
 * Supply voltage monitoring block consist of 2 detectors namely External voltage detector (VDEL) and External brownout
 * detector (BDE) in the EVR that are used to monitor the VDDP. \a VDEL detector compares the supply voltage against a
 * pre-warning threshold voltage \a ext_supply_threshold. The threshold level is programmable via register \a ANAVDEL.VDEL_SELECT. An interrupt
 * if enabled via \a enable_prewarning_int, will be triggered if a level below this threshold is detected and the flag, VDDPI,
 * in SRRAW register bit is set. Similarly interrupts can be enabled for the events of VCLIP and prewarning, using the structure members,
 * \a enable_vclip_int and \a enable_prewarning_int.  The handlers for these interrupts have to be explicitly defined using
 * the API XMC_SCU_INTERRUPT_SetEventHandler().
 * \par<b>Related APIs:</b><BR>
 * XMC_SCU_INTERRUPT_SetEventHandler()\n\n\n
 */
void XMC_SCU_SupplyMonitorInit(const XMC_SCU_SUPPLYMONITOR_t *obj);

/**
 * @param limit Kelvin degree temperature lower compare limit in range [233,388]
 * @return XMC_SCU_STATUS_t status of limit installation
 *
 * \par<b>Description</b><br>
 * Set lower temperature compare limit. 
 * A low temperature interrupt (SCU_IRQ1) is triggered if Tchip < limit and the event and interrupt are enabled. 
 * Alternatively XMC_SCU_LowTemperature() can be used to check the status.
 * \par<b>Related APIs:</b><BR>
 * XMC_SCU_LowTemperature()\n\n\n
 *
 **/
XMC_SCU_STATUS_t XMC_SCU_SetTempLowLimit(uint32_t limit);

/**
 * @param limit Kelvin degree temperature higher compare limit in range [233,388]
 * @return XMC_SCU_STATUS_t status of limit installation
 *
 * \par<b>Description</b><br>
 * Set higher temperature compare limit. 
 * A high temperature interrupt (SCU_IRQ1) is triggered if Tchip > limit and the event and interrupt are enabled. 
 * Alternatively XMC_SCU_HighTemperature() can be used to check the status.
 * \par<b>Related APIs:</b><BR>
 * XMC_SCU_HighTemperature()\n\n\n
 *
 **/
XMC_SCU_STATUS_t XMC_SCU_SetTempHighLimit(uint32_t limit);

/**
 *
 * @param lower_temp  Lower threshold value for the die temperature.\n
 *          \b Range: 0 to 65535(16 bit unsigned value).
 * @param upper_temp  Upper threshold value for the die temperature.\n
 *          \b Range: 0 to 65535(16 bit unsigned value).
 *
 * @return None
 *
 * \par<b>Description</b><br>
 * Configures upper and lower thresholds of die temperature as raw digital values into temperature sensor.\n\n
 * The API configures \a ANATSEIH and \a ANATSEIL registers for upper and lower die temperature threshold limits
 * respectively.\n
 * It is recommended to use following steps:\n
 * - Call \a XMC_SCU_StopTempMeasurement to stop temperature measurement if it was started previously.\n
 * - Call \a XMC_SCU_SetRawTempLimits with desired lower and upper temperature threshold limit values.\n
 * - Finally call \a XMC_SCU_StartTempMeasurement to start temperature measurement.\n
 *
 * \par<b>Related APIs:</b><BR>
 * XMC_SCU_StopTempMeasurement(), XMC_SCU_StartTempMeasurement() \n\n\n
 */
void XMC_SCU_SetRawTempLimits(const uint32_t lower_temp, const uint32_t upper_temp);

// /* API to program temperature limits in centigrade into temperature sensor unit */ // need to implement in future
// void XMC_SCU_SetTempLimits(const uint32_t lower_temp, const uint32_t upper_temp);
/**
 *
 * @return XMC_SCU_STATUS_t   Status of starting the temperature measurement.\n
 *                  \b Range: Use type @ref XMC_SCU_STATUS_t to identify the result.\n
 *                  XMC_SCU_STATUS_OK- Temperature measurement started successfully.\n
 *                  Always returns the above status.
 *
 * \par<b>Description</b><br>
 * Starts die temperature measurement using internal temperature sensor.\n\n
 * The API, enables die temperature measurement and waits for about 10000 cycles until
 * temperature measurement result is available on \a SCU_ANALOG->ANATSEMON bit fields.\n
 * It is recommended to use following steps:\n
 * - Call \a XMC_SCU_StopTempMeasurement to stop temperature measurement if it was started previously.\n
 * - Call \a XMC_SCU_SetRawTempLimits with desired lower and upper temperature threshold limit values if it is needed.\n
 * - Call \a XMC_SCU_StartTempMeasurement to start temperature measurement.\n
 * - Read die temperature value using \a XMC_SCU_GetTemperature API.\n
 * \par<b>Related APIs:</b><BR>
 * XMC_SCU_StopTempMeasurement(), XMC_SCU_SetRawTempLimits(), XMC_SCU_GetTemperature() \n\n\n
 */
void XMC_SCU_StartTempMeasurement(void);

/**
 * @return None
 *
 * \par<b>Description</b><br>
 * Stops the die temperature measurement.\n\n
 * Die temperature measurement is stopped by disabling the sensor using \a TSE_EN bit of
 * \a ANATSECTRL register.
 * \par<b>Related APIs:</b><BR>
 * XMC_SCU_StartTempMeasurement(), XMC_SCU_SetRawTempLimits(), XMC_SCU_GetTemperature() \n\n\n
 */
void XMC_SCU_StopTempMeasurement(void);

/**
 *
 * @return bool  Result of checking whether the die temperature is more than the upper threshold.\n
 *            \b Range: \a false if temperature is below the upper threshold. \a true if temperature
 *            has exceeded the upper threshold configured in \a ANATSEIH register.
 *
 * \par<b>Description</b><br>
 * Check if the temperature has exceeded the upper threshold value.\n\n
 * The API checks for \a TSE_HIGH bit (TSE Compare High Temperature Event Status bit) of \a SRRAW register.
 * The bit will be set when the \a TSE_MON value in \a ANATSEMON register exceeds the value of
 * \a TSE_IH value in \a ANATSEIH register.
 * \par<b>Related APIs:</b><BR>
 * XMC_SCU_StartTempMeasurement(), XMC_SCU_SetRawTempLimits(), XMC_SCU_GetTemperature(), XMC_SCU_LowTemperature() \n\n\n
 */
bool XMC_SCU_HighTemperature(void);

/**
 *
 * @return bool  Result of checking whether the die temperature is less than the lower threshold.\n
 *            \b Range: \a false if temperature is higher than the lower threshold. \a true if temperature
 *            has dropped below the lower threshold configured in \a ANATSEIL register.
 *
 * \par<b>Description</b><br>
 * Check if the temperature has dropped below the lower threshold value.\n\n
 * The API checks for \a TSE_LOW bit (TSE Compare Low Temperature Event Status bit) of \a SRRAW register.
 * The bit will be set when the \a TSE_MON value in \a ANATSEMON register drops below the value of
 * \a TSE_IL value in \a ANATSEIL register.
 * \par<b>Related APIs:</b><BR>
 * XMC_SCU_StartTempMeasurement(), XMC_SCU_SetRawTempLimits(), XMC_SCU_GetTemperature(), XMC_SCU_HighTemperature() \n\n\n
 */
bool XMC_SCU_LowTemperature(void);

/**
 * @return uint32_t  Raw die temperature value. \b Range: 16 bit value.
 *
 * \par<b>Description</b><br>
 * Provides the raw die temperature value.\n\n
 * The API reads temperature measurement result from \a SCU_ANALOG->ANATSEMON bit fields.
 * \par<b>Related APIs:</b><BR>
 * XMC_SCU_StartTempMeasurement() \n\n\n
 */
uint32_t XMC_SCU_GetTemperature(void);

/**
 * @return uint32_t  Calculate die temperature value. \b Range: 16 bit value.
 *
 * \par<b>Description</b><br>
 * Calculates the die temperature value using ROM function.\n\n
 * \par<b>Related APIs:</b><BR>
 * XMC_SCU_StartTempMeasurement() \n\n\n
 */
uint32_t XMC_SCU_CalcTemperature(void);

/**
 *
 * @return true DTS Measurement Done
 * @return false DTS Measurement not Done
 *
 * \par<b>Description</b><br>
 * This functions checks the status of the DTS Measurement completion.\n\n
 * \par<b>Related APIs:</b><BR>
 * XMC_SCU_StartTempMeasurement() \n\n\n
 */
__STATIC_INLINE bool XMC_SCU_IsTempMeasurementDone(void)
{
  return ((SCU_INTERRUPT->SRRAW & SCU_INTERRUPT_SRRAW_TSE_DONE_Msk) != 0U);
}

/**
 * @return None
 *
 * \par<b>Description</b><br>
 * Trigger device master reset.\n\n
 * The API triggers master reset by setting the \a MRSTEN bit of \a RSTCON register.
 * It also internally triggers system reset. Almost all the logics of the device are affected by this reset.
 * \par<b>Related APIs:</b><BR>
 * XMC_SCU_RESET_EnableResetRequest() \n\n\n
 */
__STATIC_INLINE void XMC_SCU_RESET_AssertMasterReset(void)
{
  SCU_RESET->RSTCON |= SCU_RESET_RSTCON_MRSTEN_Msk;
}
/**
 *
 * @param request  Reset source to trigger the device reset.\n
 *          \b Range: Use type @ref XMC_SCU_SYSTEM_RESET_REQUEST_t to identify the reset source.\n
 *          XMC_SCU_RESET_REQUEST_FLASH_ECC_ERROR- Reset when flash memory double bit error is detected.\n
 *          XMC_SCU_RESET_REQUEST_CLOCK_LOSS- Reset when loss of clock is detected.\n
 *          XMC_SCU_RESET_REQUEST_SRAM_PARITY_ERROR- Reset when SRAM parity error is detected.\n
 *          XMC_SCU_RESET_REQUEST_USIC_SRAM_PARITY_ERROR- Reset when USIC0 SRAM parity error is detected.\n
 *
 * @return None
 *
 * \par<b>Description</b><br>
 * Configures trigger for system reset from the selected source.\n\n
 * The API configures the reset source specific bit in the \a RSTCON register.
 * Multiple reset sources can be combined using \a OR operation. By enabling
 * the reset using this API will not trigger the reset. The reset will happen when
 * the configured source event is detected.
 * \par<b>Related APIs:</b><BR>
 * XMC_SCU_RESET_AssertMasterReset() \n\n\n
 */
__STATIC_INLINE void XMC_SCU_RESET_EnableResetRequest(uint32_t request)
{
  SCU_RESET->RSTCON |= request;
}

/**
 *
 * @return uint32_t  Fast peripheral clock frequency in Hertz.
 *
 * \par<b>Description</b><br>
 * Provides the clock frequency of peripherals on the peripheral bus that are using a shared functional clock.\n\n
 * The value is derived using the bitfield \a PCLKSEL from \a CLKCR register. Peripheral clock
 * can have 2 times the frequency of system clock if the \a PCLKSEL is set.
 * \par<b>Related APIs:</b><BR>
 * XMC_SCU_CLOCK_SetFastPeripheralClockSource() \n\n\n
 */
uint32_t XMC_SCU_CLOCK_GetFastPeripheralClockFrequency(void);

/**
 *
 * @param source  Fast peripheral clock source.\n
 *          \b Range: Use type @ref XMC_SCU_CLOCK_PCLKSRC_t to identify the clock source.\n
 *          XMC_SCU_CLOCK_PCLKSRC_MCLK- Use MCLK as the peripheral clock.\n
 *          XMC_SCU_CLOCK_PCLKSRC_DOUBLE_MCLK- peripheral clock will be 2 times the MCLK frequency.
 *
 * @return None
 *
 * \par<b>Description</b><br>
 * Configures the source of peripheral clock. \n\n
 * The peripheral clock can be either same as MCLK or twice the frequency of MCLK.
 * \par<b>Related APIs:</b><BR>
 * XMC_SCU_CLOCK_GetFastPeripheralClockFrequency() \n\n\n
 */
void XMC_SCU_CLOCK_SetFastPeripheralClockSource(const XMC_SCU_CLOCK_PCLKSRC_t source);

/**
 *
 * @param temperature  measured temperature using the on-chip temperature sensor
 *
 * @return None
 *
 * \par<b>Description</b><br>
 * DCO1 clock frequency can be calibrated during runtime to achieve a better accuracy.
 * This function start the DCO1 calibration based on temperature.
 * 
 */
void XMC_SCU_CLOCK_CalibrateOscillatorOnTemperature(int32_t temperature);

#if (UC_SERIES == XMC14) || defined(DOXYGEN)
/**
 *
 * @param sync_clk Clock source selected as external reference. @ref XMC_SCU_CLOCK_SYNC_CLKSRC_t
 * @param prescaler integer(\f$\frac{3000 \times f_{OSC}[MHz]}{48}\f$)
 * @param syn_preload integer(\f$\frac{48 \times prescaler}{f_{OSC}[MHz]}\f$)
 *
 * @return None
 * 
 * \par<b>Description</b><br>
 * DCO1 clock frequency can be calibrated during runtime to achieve a better accuracy.
 * This function starts the automatic DCO1 calibration based on the selected clock source.
 * @note Only available for XMC1400 series
 */
void XMC_SCU_CLOCK_EnableDCO1ExtRefCalibration(XMC_SCU_CLOCK_SYNC_CLKSRC_t sync_clk, uint32_t prescaler, uint32_t syn_preload);

/**
 *
 * @return None
 * 
 * \par<b>Description</b><br>
 * This function stops the automatic DCO1 calibration based on the selected clock source.
 * @note Only available for XMC1400 series
 */
void XMC_SCU_CLOCK_DisableDCO1ExtRefCalibration(void);

/**
 *
 * @return true DCO1 is synchronized to the selected XTAL frequency
 * @return false Actual DCO1 frequency is out of target
 * 
 * \par<b>Description</b><br>
 * This functions checks the status of the synchronisation
 * @note Only available for XMC1400 series
 */
bool XMC_SCU_CLOCK_IsDCO1ExtRefCalibrationReady(void);

/**
 *
 * @return None
 * 
 * \par<b>Description</b><br>
 * This function enables the watchdog on the DCO1 frequency
 * @note Only available for XMC1400 series
 */
void XMC_SCU_CLOCK_EnableDCO1OscillatorWatchdog(void);

/**
 *
 * @return None
 * 
 * \par<b>Description</b><br>
 * This function disables the watchdog on the DCO1 frequency
 * @note Only available for XMC1400 series
 */
void XMC_SCU_CLOCK_DisableDCO1OscillatorWatchdog(void);

/**
 *
 * @return None
 * 
 * \par<b>Description</b><br>
 * This function clears the status of the watchdog on the DCO1 frequency
 * @note Only available for XMC1400 series
 */
void XMC_SCU_CLOCK_ClearDCO1OscillatorWatchdogStatus(void);

/*
 *
 * @return true The OSC frequency is usable
 * @return false The OSC frequency is not usable. Frequency is too high or too low
 * 
 * \par<b>Description</b><br>
 * This function checks if the DCO1 frequency is in the limits of the watchdog
 * @note Only available for XMC1400 series
 */
bool XMC_SCU_CLOCK_IsDCO1ClockFrequencyUsable(void);

/*
 *
 * @return None
 * 
 * \par<b>Description</b><br>
 * This function enables flash power down when entering power save mode (SLEEP or DEEPSLEEP modes). 
 * Upon wake-up, CPU is able to fetch code from flash.
 *
 * @usage
 * @code
 *
 * // The clock of the peripherals that are not needed during sleep state can be gated before entering sleep state
 * XMC_SCU_CLOCK_GatePeripheralClock(SCU_CLK_CGATSTAT0_MATH_Msk);
 *
 * // Enable FLASH power down during SLEEP and DEEPSLEEP mode 
 * XMC_SCU_CLOCK_EnableFlashPowerDown();
 *
 * // Make sure that SLEEPDEEP bit is set
 * SCB->SCR |= SCB_SCR_DEEPSLEEP_Msk;
 *
 * // Return to SLEEP mode after handling the wakeup event
 * SCB->SCR |= SCB_SCR_SLEEPONEXIT_Msk; 
 * 
 * // Put system in DEEPSLEEP state
 * __WFI();
 *
 * @endcode
 *
 * \par<b>Related APIs:</b><BR>
 * XMC_FLASH_EnterSleepMode()
 */
__STATIC_INLINE void XMC_SCU_CLOCK_EnableFlashPowerDown(void)
{
  SCU_CLK->PWRSVCR = SCU_CLK_PWRSVCR_FPD_Msk;
}

/*
 *
 * @return None
 * 
 * \par<b>Description</b><br>
 * This function disables flash power down when entering power save mode (SLEEP or DEEPSLEEP modes). 
 * \par<b>Related APIs:</b><BR>
 * XMC_FLASH_EnterSleepMode()
 */
__STATIC_INLINE void XMC_SCU_CLOCK_DisableFlashPowerDown(void)
{
  SCU_CLK->PWRSVCR = 0;
}

/**
 * This function selects service request source for a NVIC interrupt node.
 * The XMC1400 series has up to 54 peripheral service requests.
 * The Cortex M0 however has 32 interrupts available for peripherals.
 * This function allows you to select which 32 peripheral requests of the 54 the CPU should react on.
 *
 * @param irq_number Interrupt number, 0 to 31
 * @param source Peripheral service request. See @ref XMC_SCU_IRQCTRL_t
 *
 * @note Only available for XMC1400 series
 */
void XMC_SCU_SetInterruptControl(uint8_t irq_number, XMC_SCU_IRQCTRL_t source);

/**
 * This function enables the Prefetch Unit (PFU).
 * The purpose of the Prefetch unit is to reduce the Flash latency gap at higher system
 * frequencies to increase the instruction per cycle performance.
 *
 * @note Only available for XMC1400 series
 */
__STATIC_INLINE void XMC_SCU_EnablePrefetchUnit(void)
{
  SCU_GENERAL->PFUCR &= ~SCU_GENERAL_PFUCR_PFUBYP_Msk;
}

/**
 * This function disables the Prefetch Unit (PFU).
 * The purpose of the Prefetch unit is to reduce the Flash latency gap at higher system
 * frequencies to increase the instruction per cycle performance.
 *
 * @note Only available for XMC1400 series
 */
__STATIC_INLINE void XMC_SCU_DisablePrefetchUnit(void)
{
  SCU_GENERAL->PFUCR |= SCU_GENERAL_PFUCR_PFUBYP_Msk;
}

#endif

/** 
 *
 * @param range VDEL Range Select ::XMC_SCU_POWER_MONITOR_RANGE_t
 * @param delay VDEL Timing Setting ::XMC_SCU_POWER_MONITOR_DELAY_t
 * @return None
 *
 * \par<b>Description</b><br>
 * Enables VDEL detector. VDEL detector compares the supply voltage against a pre-warning threshold voltage
 *
 * @note Brown Out Trap need to be enabled previously
 */
__STATIC_INLINE void XMC_SCU_POWER_EnableMonitor(XMC_SCU_POWER_MONITOR_RANGE_t range, XMC_SCU_POWER_MONITOR_DELAY_t delay)
{
  SCU_ANALOG->ANAVDEL = SCU_ANALOG_ANAVDEL_VDEL_EN_Msk | 
                        (uint32_t)range | 
                        (uint32_t)delay;
}

/**
 *
 * @return None
 *
 * \par<b>Description</b><br>
 * Disables VDEL detector
 */
__STATIC_INLINE void XMC_SCU_POWER_DisableMonitor(void)
{
  SCU_ANALOG->ANAVDEL &= ~SCU_ANALOG_ANAVDEL_VDEL_EN_Msk; 
}

/**
 *
 * @param flags may be any of: <br>
 *             \ref XMC_SCU_BMI_HWCFG_CAN_BSL (only available for XMC1400 series with CAN Module)<br> 
 *             \ref XMC_SCU_BMI_HWCFG_CAN_BSLTO (only available for XMC1400 series with CAN Module)<br> 
 *             \ref XMC_SCU_BMI_HWCFG_SBSL_CANOPEN (only available for XMC1400 series with CAN Module)<br> 
 *             \ref XMC_SCU_BMI_HWCFG_ASC_BSL<br> 
 *             \ref XMC_SCU_BMI_HWCFG_UPM<br> 
 *             \ref XMC_SCU_BMI_HWCFG_UMD<br> 
 *             \ref XMC_SCU_BMI_HWCFG_UMHAR<br> 
 *             \ref XMC_SCU_BMI_HWCFG_SSC_BSL<br> 
 *             \ref XMC_SCU_BMI_HWCFG_ASC_BSLTO<br> 
 *             \ref XMC_SCU_BMI_HWCFG_SSC_BSLTO<br> 
 *             \ref XMC_SCU_BMI_HWCFG_SBSL<br><br>
 *        optionally OR'd together with any of: <br> 
 *             \ref XMC_SCU_BMI_DAPTYP_SWD<br> 
 *             \ref XMC_SCU_BMI_DAPTYP_SPD<br><br> 
 *        optionally OR'd together with any of: <br> 
 *             \ref XMC_SCU_BMI_DAPDIS_CHANNEL_0<br> 
 *             \ref XMC_SCU_BMI_DAPDIS_CHANNEL_1<br><br>
 * @image html "xmc1000_debugif.png"
 *        optionally OR'd together with any of (only available for XMC1400 series with CAN Module): <br> 
 *             \ref XMC_SCU_BMI_CANCLK_DCO1<br> 
 *             \ref XMC_SCU_BMI_CANCLK_OSCHP<br><br>
 *
 * @param timeout Only relevant if a start up mode is selected that uses timeout. The time-out duration is BSLTO*2664000 MCLK cycles, the supported time-out range is 0.3-5s (333...4995ms)
 * @return false only upon error, if OK the procedure triggers a reset and does not return to calling routine
 *
 * \par<b>Description</b><br>
 * This procedure initiates installation of a new BMI value. In particular, it can be used as
 * well as to restore the state upon delivery for a device already in User Productive mode.
 * 
 * @code
 *   // Switch to ASC Bootstrap Loader
 *   XMC_SCU_SetBMI(XMC_SCU_BMI_HWCFG_ASC_BSL, 0);
 *
 *   // Switch to Debug user mode SWD1 (pins P1.3 and P1.2)
 *   XMC_SCU_SetBMI(XMC_SCU_BMI_HWCFG_UMD | XMC_SCU_BMI_DAPTYP_SWD | XMC_SCU_BMI_DAPDIS_CHANNEL_1, 0);
 * @endcode
 */
uint32_t XMC_SCU_SetBMI(uint32_t flags, uint8_t timeout);

#ifdef __cplusplus
}
#endif

/**
 * @}
 */

/**
 * @}
 */

#endif /* UC_FAMILY == XMC1 */

#endif /* XMC1_SCU_H */
