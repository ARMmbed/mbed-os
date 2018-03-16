/***************************************************************************//**
 * @file em_smu.h
 * @brief Security Management Unit (SMU) peripheral API
 * @version 5.3.3
 *******************************************************************************
 * # License
 * <b>Copyright 2016 Silicon Laboratories, Inc. http://www.silabs.com</b>
 *******************************************************************************
 *
 * Permission is granted to anyone to use this software for any purpose,
 * including commercial applications, and to alter it and redistribute it
 * freely, subject to the following restrictions:
 *
 * 1. The origin of this software must not be misrepresented; you must not
 *    claim that you wrote the original software.
 * 2. Altered source versions must be plainly marked as such, and must not be
 *    misrepresented as being the original software.
 * 3. This notice may not be removed or altered from any source distribution.
 *
 * DISCLAIMER OF WARRANTY/LIMITATION OF REMEDIES: Silicon Labs has no
 * obligation to support this Software. Silicon Labs is providing the
 * Software "AS IS", with no express or implied warranties of any kind,
 * including, but not limited to, any implied warranties of merchantability
 * or fitness for any particular purpose or warranties against infringement
 * of any proprietary rights of a third party.
 *
 * Silicon Labs will not be liable for any consequential, incidental, or
 * special damages, or any other relief, or for any claim by any third party,
 * arising from your use of this Software.
 *
 ******************************************************************************/

#ifndef EM_SMU_H
#define EM_SMU_H

#include "em_device.h"
#if defined(SMU_COUNT) && (SMU_COUNT > 0)

#include "em_assert.h"
#include "em_bus.h"

#include <stdint.h>
#include <stdbool.h>

#ifdef __cplusplus
extern "C" {
#endif

/***************************************************************************//**
 * @addtogroup emlib
 * @{
 ******************************************************************************/

/***************************************************************************//**
 * @addtogroup SMU
 * @brief Security Management Unit (SMU) Peripheral API
 *
 * @details
 *   The Security Management Unit (SMU) forms the control and status/reporting
 *   component of bus-level security in EFM32/EFR32 devices.
 *
 *   Peripheral-level protection is provided via the peripheral protection unit
 *   (PPU). The PPU provides a hardware access barrier to any peripheral that is
 *   configured to be protected. When an attempt is made to access a peripheral
 *   without the required privilege/security level, the PPU detects the fault
 *   and intercepts the access. No write or read of the peripheral register
 *   space occurs, and an all-zero value is returned if the access is a read.
 *
 *   @subsection Usage example
 *   @include em_smu_init.c
 * @{
 ******************************************************************************/

/*******************************************************************************
 ********************************   ENUMS   ************************************
 ******************************************************************************/

/** SMU peripheral identifiers. */
typedef enum {
#if defined(_SILICON_LABS_GECKO_INTERNAL_SDID_84)
  smuPeripheralACMP0      = _SMU_PPUPATD0_ACMP0_SHIFT,         /**< SMU peripheral identifier for ACMP0     */
  smuPeripheralACMP1      = _SMU_PPUPATD0_ACMP1_SHIFT,         /**< SMU peripheral identifier for ACMP1     */
  smuPeripheralADC0       = _SMU_PPUPATD0_ADC0_SHIFT,          /**< SMU peripheral identifier for ADC0      */
  smuPeripheralCMU        = _SMU_PPUPATD0_CMU_SHIFT,           /**< SMU peripheral identifier for CMU       */
  smuPeripheralCRYOTIMER  = _SMU_PPUPATD0_CRYOTIMER_SHIFT,     /**< SMU peripheral identifier for CRYOTIMER */
  smuPeripheralCRYPTO0    = _SMU_PPUPATD0_CRYPTO0_SHIFT,       /**< SMU peripheral identifier for CRYPTO0   */
  smuPeripheralCRYPTO1    = _SMU_PPUPATD0_CRYPTO1_SHIFT,       /**< SMU peripheral identifier for CRYPTO1   */
  smuPeripheralCSEN       = _SMU_PPUPATD0_CSEN_SHIFT,          /**< SMU peripheral identifier for CSEN      */
  smuPeripheralVDAC0      = _SMU_PPUPATD0_VDAC0_SHIFT,         /**< SMU peripheral identifier for VDAC0     */
  smuPeripheralPRS        = _SMU_PPUPATD0_PRS_SHIFT,           /**< SMU peripheral identifier for PRS       */
  smuPeripheralEMU        = _SMU_PPUPATD0_EMU_SHIFT,           /**< SMU peripheral identifier for EMU       */
  smuPeripheralFPUEH      = _SMU_PPUPATD0_FPUEH_SHIFT,         /**< SMU peripheral identifier for FPUEH     */
  smuPeripheralGPCRC      = _SMU_PPUPATD0_GPCRC_SHIFT,         /**< SMU peripheral identifier for GPCRC     */
  smuPeripheralGPIO       = _SMU_PPUPATD0_GPIO_SHIFT,          /**< SMU peripheral identifier for GPIO      */
  smuPeripheralI2C0       = _SMU_PPUPATD0_I2C0_SHIFT,          /**< SMU peripheral identifier for I2C0      */
  smuPeripheralI2C1       = _SMU_PPUPATD0_I2C1_SHIFT,          /**< SMU peripheral identifier for I2C1      */
  smuPeripheralIDAC0      = _SMU_PPUPATD0_IDAC0_SHIFT,         /**< SMU peripheral identifier for IDAC0     */
  smuPeripheralMSC        = _SMU_PPUPATD0_MSC_SHIFT,           /**< SMU peripheral identifier for MSC       */
  smuPeripheralLDMA       = _SMU_PPUPATD0_LDMA_SHIFT,          /**< SMU peripheral identifier for LDMA      */
  smuPeripheralLESENSE    = _SMU_PPUPATD0_LESENSE_SHIFT,       /**< SMU peripheral identifier for LESENSE   */
  smuPeripheralLETIMER0   = _SMU_PPUPATD0_LETIMER0_SHIFT,      /**< SMU peripheral identifier for LETIMER0  */
  smuPeripheralLEUART0    = _SMU_PPUPATD0_LEUART0_SHIFT,       /**< SMU peripheral identifier for LEUART0   */
  smuPeripheralPCNT0      = _SMU_PPUPATD0_PCNT0_SHIFT,         /**< SMU peripheral identifier for PCNT0     */
  smuPeripheralPCNT1      = _SMU_PPUPATD0_PCNT1_SHIFT,         /**< SMU peripheral identifier for PCNT1     */
  smuPeripheralPCNT2      = _SMU_PPUPATD0_PCNT2_SHIFT,         /**< SMU peripheral identifier for PCNT2     */
  smuPeripheralRMU        = 32 + _SMU_PPUPATD1_RMU_SHIFT,      /**< SMU peripheral identifier for RMU       */
  smuPeripheralRTCC       = 32 + _SMU_PPUPATD1_RTCC_SHIFT,     /**< SMU peripheral identifier for RTCC      */
  smuPeripheralSMU        = 32 + _SMU_PPUPATD1_SMU_SHIFT,      /**< SMU peripheral identifier for SMU       */
  smuPeripheralTIMER0     = 32 + _SMU_PPUPATD1_TIMER0_SHIFT,   /**< SMU peripheral identifier for TIMER0    */
  smuPeripheralTIMER1     = 32 + _SMU_PPUPATD1_TIMER1_SHIFT,   /**< SMU peripheral identifier for TIMER1    */
  smuPeripheralTRNG0      = 32 + _SMU_PPUPATD1_TRNG0_SHIFT,    /**< SMU peripheral identifier for TRNG0     */
  smuPeripheralUSART0     = 32 + _SMU_PPUPATD1_USART0_SHIFT,   /**< SMU peripheral identifier for USART0    */
  smuPeripheralUSART1     = 32 + _SMU_PPUPATD1_USART1_SHIFT,   /**< SMU peripheral identifier for USART1    */
  smuPeripheralUSART2     = 32 + _SMU_PPUPATD1_USART2_SHIFT,   /**< SMU peripheral identifier for USART2    */
  smuPeripheralUSART3     = 32 + _SMU_PPUPATD1_USART3_SHIFT,   /**< SMU peripheral identifier for USART3    */
  smuPeripheralWDOG0      = 32 + _SMU_PPUPATD1_WDOG0_SHIFT,    /**< SMU peripheral identifier for WDOG0     */
  smuPeripheralWDOG1      = 32 + _SMU_PPUPATD1_WDOG1_SHIFT,    /**< SMU peripheral identifier for WDOG1     */
  smuPeripheralWTIMER0    = 32 + _SMU_PPUPATD1_WTIMER0_SHIFT,  /**< SMU peripheral identifier for WTIMER0   */
  smuPeripheralWTIMER1    = 32 + _SMU_PPUPATD1_WTIMER1_SHIFT,  /**< SMU peripheral identifier for WTIMER1   */

#elif defined(_SILICON_LABS_GECKO_INTERNAL_SDID_89)
  smuPeripheralACMP0      = _SMU_PPUPATD0_ACMP0_SHIFT,         /**< SMU peripheral identifier for ACMP0     */
  smuPeripheralACMP1      = _SMU_PPUPATD0_ACMP1_SHIFT,         /**< SMU peripheral identifier for ACMP1     */
  smuPeripheralADC0       = _SMU_PPUPATD0_ADC0_SHIFT,          /**< SMU peripheral identifier for ADC0      */
  smuPeripheralCMU        = _SMU_PPUPATD0_CMU_SHIFT,           /**< SMU peripheral identifier for CMU       */
  smuPeripheralCRYOTIMER  = _SMU_PPUPATD0_CRYOTIMER_SHIFT,     /**< SMU peripheral identifier for CRYOTIMER */
  smuPeripheralCRYPTO0    = _SMU_PPUPATD0_CRYPTO0_SHIFT,       /**< SMU peripheral identifier for CRYPTO0   */
  smuPeripheralCRYPTO1    = _SMU_PPUPATD0_CRYPTO1_SHIFT,       /**< SMU peripheral identifier for CRYPTO1   */
#if defined(_SMU_PPUPATD0_CSEN_SHIFT)
  smuPeripheralCSEN       = _SMU_PPUPATD0_CSEN_SHIFT,          /**< SMU peripheral identifier for CSEN      */
#endif
#if defined(_SMU_PPUPATD0_VDAC0_SHIFT)
  smuPeripheralVDAC0      = _SMU_PPUPATD0_VDAC0_SHIFT,         /**< SMU peripheral identifier for VDAC0     */
#endif
  smuPeripheralPRS        = _SMU_PPUPATD0_PRS_SHIFT,           /**< SMU peripheral identifier for PRS       */
  smuPeripheralEMU        = _SMU_PPUPATD0_EMU_SHIFT,           /**< SMU peripheral identifier for EMU       */
  smuPeripheralFPUEH      = _SMU_PPUPATD0_FPUEH_SHIFT,         /**< SMU peripheral identifier for FPUEH     */
  smuPeripheralGPCRC      = _SMU_PPUPATD0_GPCRC_SHIFT,         /**< SMU peripheral identifier for GPCRC     */
  smuPeripheralGPIO       = _SMU_PPUPATD0_GPIO_SHIFT,          /**< SMU peripheral identifier for GPIO      */
  smuPeripheralI2C0       = _SMU_PPUPATD0_I2C0_SHIFT,          /**< SMU peripheral identifier for I2C0      */
  smuPeripheralI2C1       = _SMU_PPUPATD0_I2C1_SHIFT,          /**< SMU peripheral identifier for I2C1      */
#if defined(_SMU_PPUPATD0_IDAC0_SHIFT)
  smuPeripheralIDAC0      = _SMU_PPUPATD0_IDAC0_SHIFT,         /**< SMU peripheral identifier for IDAC0     */
#endif
  smuPeripheralMSC        = _SMU_PPUPATD0_MSC_SHIFT,           /**< SMU peripheral identifier for MSC       */
  smuPeripheralLDMA       = _SMU_PPUPATD0_LDMA_SHIFT,          /**< SMU peripheral identifier for LDMA      */
  smuPeripheralLESENSE    = _SMU_PPUPATD0_LESENSE_SHIFT,       /**< SMU peripheral identifier for LESENSE   */
  smuPeripheralLETIMER0   = _SMU_PPUPATD0_LETIMER0_SHIFT,      /**< SMU peripheral identifier for LETIMER0  */
  smuPeripheralLEUART0    = _SMU_PPUPATD0_LEUART0_SHIFT,       /**< SMU peripheral identifier for LEUART0   */
  smuPeripheralPCNT0      = _SMU_PPUPATD0_PCNT0_SHIFT,         /**< SMU peripheral identifier for PCNT0     */
  smuPeripheralRMU        = 32 + _SMU_PPUPATD1_RMU_SHIFT,      /**< SMU peripheral identifier for RMU       */
  smuPeripheralRTCC       = 32 + _SMU_PPUPATD1_RTCC_SHIFT,     /**< SMU peripheral identifier for RTCC      */
  smuPeripheralSMU        = 32 + _SMU_PPUPATD1_SMU_SHIFT,      /**< SMU peripheral identifier for SMU       */
  smuPeripheralTIMER0     = 32 + _SMU_PPUPATD1_TIMER0_SHIFT,   /**< SMU peripheral identifier for TIMER0    */
  smuPeripheralTIMER1     = 32 + _SMU_PPUPATD1_TIMER1_SHIFT,   /**< SMU peripheral identifier for TIMER1    */
  smuPeripheralTRNG0      = 32 + _SMU_PPUPATD1_TRNG0_SHIFT,    /**< SMU peripheral identifier for TRNG0     */
  smuPeripheralUSART0     = 32 + _SMU_PPUPATD1_USART0_SHIFT,   /**< SMU peripheral identifier for USART0    */
  smuPeripheralUSART1     = 32 + _SMU_PPUPATD1_USART1_SHIFT,   /**< SMU peripheral identifier for USART1    */
  smuPeripheralUSART2     = 32 + _SMU_PPUPATD1_USART2_SHIFT,   /**< SMU peripheral identifier for USART2    */
  smuPeripheralWDOG0      = 32 + _SMU_PPUPATD1_WDOG0_SHIFT,    /**< SMU peripheral identifier for WDOG0     */
  smuPeripheralWDOG1      = 32 + _SMU_PPUPATD1_WDOG1_SHIFT,    /**< SMU peripheral identifier for WDOG1     */
  smuPeripheralWTIMER0    = 32 + _SMU_PPUPATD1_WTIMER0_SHIFT,  /**< SMU peripheral identifier for WTIMER0   */

#elif defined(_SILICON_LABS_GECKO_INTERNAL_SDID_95)
#if defined(_SMU_PPUPATD0_ACMP0_SHIFT)
  smuPeripheralACMP0      = _SMU_PPUPATD0_ACMP0_SHIFT,         /**< SMU peripheral identifier for ACMP0     */
#endif
#if defined(_SMU_PPUPATD0_ACMP1_SHIFT)
  smuPeripheralACMP1      = _SMU_PPUPATD0_ACMP1_SHIFT,         /**< SMU peripheral identifier for ACMP1     */
#endif
  smuPeripheralADC0       = _SMU_PPUPATD0_ADC0_SHIFT,          /**< SMU peripheral identifier for ADC0      */
  smuPeripheralCMU        = _SMU_PPUPATD0_CMU_SHIFT,           /**< SMU peripheral identifier for CMU       */
  smuPeripheralCRYOTIMER  = _SMU_PPUPATD0_CRYOTIMER_SHIFT,     /**< SMU peripheral identifier for CRYOTIMER */
  smuPeripheralCRYPTO     = _SMU_PPUPATD0_CRYPTO0_SHIFT,       /**< SMU peripheral identifier for CRYPTO0   */
#if defined(_SMU_PPUPATD0_VDAC0_SHIFT)
  smuPeripheralVDAC0      = _SMU_PPUPATD0_VDAC0_SHIFT,         /**< SMU peripheral identifier for VDAC0     */
#endif
  smuPeripheralPRS        = _SMU_PPUPATD0_PRS_SHIFT,           /**< SMU peripheral identifier for PRS       */
  smuPeripheralEMU        = _SMU_PPUPATD0_EMU_SHIFT,           /**< SMU peripheral identifier for EMU       */
  smuPeripheralFPUEH      = _SMU_PPUPATD0_FPUEH_SHIFT,         /**< SMU peripheral identifier for FPUEH     */
  smuPeripheralGPCRC      = _SMU_PPUPATD0_GPCRC_SHIFT,         /**< SMU peripheral identifier for GPCRC     */
  smuPeripheralGPIO       = _SMU_PPUPATD0_GPIO_SHIFT,          /**< SMU peripheral identifier for GPIO      */
  smuPeripheralI2C0       = _SMU_PPUPATD0_I2C0_SHIFT,          /**< SMU peripheral identifier for I2C0      */
#if defined(_SMU_PPUPATD0_IDAC0_SHIFT)
  smuPeripheralIDAC0      = _SMU_PPUPATD0_IDAC0_SHIFT,         /**< SMU peripheral identifier for IDAC0     */
#endif
  smuPeripheralMSC        = _SMU_PPUPATD0_MSC_SHIFT,           /**< SMU peripheral identifier for MSC       */
  smuPeripheralLDMA       = _SMU_PPUPATD0_LDMA_SHIFT,          /**< SMU peripheral identifier for LDMA      */
#if defined(_SMU_PPUPATD0_LESENSE_SHIFT)
  smuPeripheralLESENSE    = _SMU_PPUPATD0_LESENSE_SHIFT,       /**< SMU peripheral identifier for LESENSE   */
#endif
  smuPeripheralLETIMER0   = _SMU_PPUPATD0_LETIMER0_SHIFT,      /**< SMU peripheral identifier for LETIMER0  */
  smuPeripheralLEUART     = _SMU_PPUPATD0_LEUART0_SHIFT,       /**< SMU peripheral identifier for LEUART0   */
#if defined(_SMU_PPUPATD0_PCNT0_SHIFT)
  smuPeripheralPCNT0      = _SMU_PPUPATD0_PCNT0_SHIFT,         /**< SMU peripheral identifier for PCNT0     */
#endif
  smuPeripheralRMU        = _SMU_PPUPATD0_RMU_SHIFT,           /**< SMU peripheral identifier for RMU       */
  smuPeripheralRTCC       = _SMU_PPUPATD0_RTCC_SHIFT,          /**< SMU peripheral identifier for RTCC      */
  smuPeripheralSMU        = _SMU_PPUPATD0_SMU_SHIFT,           /**< SMU peripheral identifier for SMU       */
  smuPeripheralTIMER0     = 32 + _SMU_PPUPATD1_TIMER0_SHIFT,   /**< SMU peripheral identifier for TIMER0    */
  smuPeripheralTIMER1     = 32 + _SMU_PPUPATD1_TIMER1_SHIFT,   /**< SMU peripheral identifier for TIMER1    */
  smuPeripheralTRNG0      = 32 + _SMU_PPUPATD1_TRNG0_SHIFT,    /**< SMU peripheral identifier for TRNG0     */
  smuPeripheralUSART0     = 32 + _SMU_PPUPATD1_USART0_SHIFT,   /**< SMU peripheral identifier for USART0    */
  smuPeripheralUSART1     = 32 + _SMU_PPUPATD1_USART1_SHIFT,   /**< SMU peripheral identifier for USART1    */
  smuPeripheralWDOG0      = 32 + _SMU_PPUPATD1_WDOG0_SHIFT,    /**< SMU peripheral identifier for WDOG0     */
  smuPeripheralWDOG1      = 32 + _SMU_PPUPATD1_WDOG1_SHIFT,    /**< SMU peripheral identifier for WDOG1     */
  smuPeripheralWTIMER0    = 32 + _SMU_PPUPATD1_WTIMER0_SHIFT,  /**< SMU peripheral identifier for WTIMER0   */

#elif defined(_SILICON_LABS_GECKO_INTERNAL_SDID_100)
  smuPeripheralACMP0     = _SMU_PPUPATD0_ACMP0_SHIFT,          /**< SMU peripheral identifier for ACMP0     */
  smuPeripheralACMP1     = _SMU_PPUPATD0_ACMP1_SHIFT,          /**< SMU peripheral identifier for ACMP1     */
  smuPeripheralACMP2     = _SMU_PPUPATD0_ACMP2_SHIFT,          /**< SMU peripheral identifier for ACMP2     */
  smuPeripheralACMP3     = _SMU_PPUPATD0_ACMP3_SHIFT,          /**< SMU peripheral identifier for ACMP3     */
  smuPeripheralADC0      = _SMU_PPUPATD0_ADC0_SHIFT,           /**< SMU peripheral identifier for ADC0      */
  smuPeripheralADC1      = _SMU_PPUPATD0_ADC1_SHIFT,           /**< SMU peripheral identifier for ADC1      */
  smuPeripheralCAN0      = _SMU_PPUPATD0_CAN0_SHIFT,           /**< SMU peripheral identifier for CAN0      */
  smuPeripheralCAN1      = _SMU_PPUPATD0_CAN1_SHIFT,           /**< SMU peripheral identifier for CAN1      */
  smuPeripheralCMU       = _SMU_PPUPATD0_CMU_SHIFT,            /**< SMU peripheral identifier for CMU       */
  smuPeripheralCRYOTIMER = _SMU_PPUPATD0_CRYOTIMER_SHIFT,      /**< SMU peripheral identifier for CRYOTIMER */
  smuPeripheralCRYPTO0   = _SMU_PPUPATD0_CRYPTO0_SHIFT,        /**< SMU peripheral identifier for CRYPTO0   */
  smuPeripheralCSEN      = _SMU_PPUPATD0_CSEN_SHIFT,           /**< SMU peripheral identifier for CSEN      */
  smuPeripheralVDAC0     = _SMU_PPUPATD0_VDAC0_SHIFT,          /**< SMU peripheral identifier for VDAC0     */
  smuPeripheralPRS       = _SMU_PPUPATD0_PRS_SHIFT,            /**< SMU peripheral identifier for PRS       */
  smuPeripheralEBI       = _SMU_PPUPATD0_EBI_SHIFT,            /**< SMU peripheral identifier for EBI       */
  smuPeripheralEMU       = _SMU_PPUPATD0_EMU_SHIFT,            /**< SMU peripheral identifier for EMU       */
#if defined(_SMU_PPUPATD0_ETH_SHIFT)
  smuPeripheralETH       = _SMU_PPUPATD0_ETH_SHIFT,            /**< SMU peripheral identifier for ETH       */
#endif
  smuPeripheralFPUEH     = _SMU_PPUPATD0_FPUEH_SHIFT,          /**< SMU peripheral identifier for FPUEH     */
  smuPeripheralGPCRC     = _SMU_PPUPATD0_GPCRC_SHIFT,          /**< SMU peripheral identifier for GPCRC     */
  smuPeripheralGPIO      = _SMU_PPUPATD0_GPIO_SHIFT,           /**< SMU peripheral identifier for GPIO      */
  smuPeripheralI2C0      = _SMU_PPUPATD0_I2C0_SHIFT,           /**< SMU peripheral identifier for I2C0      */
  smuPeripheralI2C1      = _SMU_PPUPATD0_I2C1_SHIFT,           /**< SMU peripheral identifier for I2C1      */
  smuPeripheralI2C2      = _SMU_PPUPATD0_I2C2_SHIFT,           /**< SMU peripheral identifier for I2C2      */
  smuPeripheralIDAC0     = _SMU_PPUPATD0_IDAC0_SHIFT,          /**< SMU peripheral identifier for IDAC0     */
  smuPeripheralMSC       = _SMU_PPUPATD0_MSC_SHIFT,            /**< SMU peripheral identifier for MAC       */
#if defined(_SMU_PPUPATD0_LCD_SHIFT)
  smuPeripheralLCD       = _SMU_PPUPATD0_LCD_SHIFT,            /**< SMU peripheral identifier for LCD       */
#endif
  smuPeripheralLDMA      = _SMU_PPUPATD0_LDMA_SHIFT,           /**< SMU peripheral identifier for LDMA      */
  smuPeripheralLESENSE   = _SMU_PPUPATD0_LESENSE_SHIFT,        /**< SMU peripheral identifier for LESENSE   */
  smuPeripheralLETIMER0  = _SMU_PPUPATD0_LETIMER0_SHIFT,       /**< SMU peripheral identifier for LETIMER0  */
  smuPeripheralLETIMER1  = _SMU_PPUPATD0_LETIMER1_SHIFT,       /**< SMU peripheral identifier for LETIMER1  */
  smuPeripheralLEUART0   = _SMU_PPUPATD0_LEUART0_SHIFT,        /**< SMU peripheral identifier for LEUART0   */
  smuPeripheralLEUART1   = _SMU_PPUPATD0_LEUART1_SHIFT,        /**< SMU peripheral identifier for LEUART1   */
  smuPeripheralPCNT0     = 32 + _SMU_PPUPATD1_PCNT0_SHIFT,     /**< SMU peripheral identifier for PCNT0     */
  smuPeripheralPCNT1     = 32 + _SMU_PPUPATD1_PCNT1_SHIFT,     /**< SMU peripheral identifier for PCNT1     */
  smuPeripheralPCNT2     = 32 + _SMU_PPUPATD1_PCNT2_SHIFT,     /**< SMU peripheral identifier for PCNT2     */
#if defined(_SMU_PPUPATD1_QSPI0_SHIFT)
  smuPeripheralQSPI0     = 32 + _SMU_PPUPATD1_QSPI0_SHIFT,     /**< SMU peripheral identifier for QSPI0     */
#endif
  smuPeripheralRMU       = 32 + _SMU_PPUPATD1_RMU_SHIFT,       /**< SMU peripheral identifier for RMU       */
  smuPeripheralRTC       = 32 + _SMU_PPUPATD1_RTC_SHIFT,       /**< SMU peripheral identifier for RTC       */
  smuPeripheralRTCC      = 32 + _SMU_PPUPATD1_RTCC_SHIFT,      /**< SMU peripheral identifier for RTCC      */
#if defined(_SMU_PPUPATD1_SDIO_SHIFT)
  smuPeripheralSDIO      = 32 + _SMU_PPUPATD1_SDIO_SHIFT,      /**< SMU peripheral identifier for SDIO      */
#endif
  smuPeripheralSMU       = 32 + _SMU_PPUPATD1_SMU_SHIFT,       /**< SMU peripheral identifier for SMU       */
  smuPeripheralTIMER0    = 32 + _SMU_PPUPATD1_TIMER0_SHIFT,    /**< SMU peripheral identifier for TIMER0    */
  smuPeripheralTIMER1    = 32 + _SMU_PPUPATD1_TIMER1_SHIFT,    /**< SMU peripheral identifier for TIMER1    */
  smuPeripheralTIMER2    = 32 + _SMU_PPUPATD1_TIMER2_SHIFT,    /**< SMU peripheral identifier for TIMER2    */
  smuPeripheralTIMER3    = 32 + _SMU_PPUPATD1_TIMER3_SHIFT,    /**< SMU peripheral identifier for TIMER3    */
  smuPeripheralTIMER4    = 32 + _SMU_PPUPATD1_TIMER4_SHIFT,    /**< SMU peripheral identifier for TIMER4    */
  smuPeripheralTIMER5    = 32 + _SMU_PPUPATD1_TIMER5_SHIFT,    /**< SMU peripheral identifier for TIMER5    */
  smuPeripheralTIMER6    = 32 + _SMU_PPUPATD1_TIMER6_SHIFT,    /**< SMU peripheral identifier for TIMER6    */
  smuPeripheralTRNG0     = 32 + _SMU_PPUPATD1_TRNG0_SHIFT,     /**< SMU peripheral identifier for TRNG0     */
  smuPeripheralUART0     = 32 + _SMU_PPUPATD1_UART0_SHIFT,     /**< SMU peripheral identifier for UART0     */
  smuPeripheralUART1     = 32 + _SMU_PPUPATD1_UART1_SHIFT,     /**< SMU peripheral identifier for UART1     */
  smuPeripheralUSART0    = 32 + _SMU_PPUPATD1_USART0_SHIFT,    /**< SMU peripheral identifier for USART0    */
  smuPeripheralUSART1    = 32 + _SMU_PPUPATD1_USART1_SHIFT,    /**< SMU peripheral identifier for USART1    */
  smuPeripheralUSART2    = 32 + _SMU_PPUPATD1_USART2_SHIFT,    /**< SMU peripheral identifier for USART2    */
  smuPeripheralUSART3    = 32 + _SMU_PPUPATD1_USART3_SHIFT,    /**< SMU peripheral identifier for USART3    */
  smuPeripheralUSART4    = 32 + _SMU_PPUPATD1_USART4_SHIFT,    /**< SMU peripheral identifier for USART4    */
  smuPeripheralUSART5    = 32 + _SMU_PPUPATD1_USART5_SHIFT,    /**< SMU peripheral identifier for USART5    */
#if defined(_SMU_PPUPATD1_USB_SHIFT)
  smuPeripheralUSB       = 32 + _SMU_PPUPATD1_USB_SHIFT,       /**< SMU peripheral identifier for USB       */
#endif
  smuPeripheralWDOG0     = 32 + _SMU_PPUPATD1_WDOG0_SHIFT,     /**< SMU peripheral identifier for WDOG0     */
  smuPeripheralWDOG1     = 32 + _SMU_PPUPATD1_WDOG1_SHIFT,     /**< SMU peripheral identifier for WDOG1     */
  smuPeripheralWTIMER0   = 32 + _SMU_PPUPATD1_WTIMER0_SHIFT,   /**< SMU peripheral identifier for WTIMER0   */
  smuPeripheralWTIMER1   = 32 + _SMU_PPUPATD1_WTIMER1_SHIFT,   /**< SMU peripheral identifier for WTIMER1   */
  smuPeripheralWTIMER2   = 32 + _SMU_PPUPATD1_WTIMER2_SHIFT,   /**< SMU peripheral identifier for WTIMER2   */
  smuPeripheralWTIMER3   = 32 + _SMU_PPUPATD1_WTIMER3_SHIFT,   /**< SMU peripheral identifier for WTIMER3   */

#elif defined(_SILICON_LABS_GECKO_INTERNAL_SDID_103)
  smuPeripheralACMP0     = _SMU_PPUPATD0_ACMP0_SHIFT,          /**< SMU peripheral identifier for ACMP0     */
  smuPeripheralACMP1     = _SMU_PPUPATD0_ACMP1_SHIFT,          /**< SMU peripheral identifier for ACMP1     */
  smuPeripheralADC0      = _SMU_PPUPATD0_ADC0_SHIFT,           /**< SMU peripheral identifier for ADC0      */
  smuPeripheralCAN0      = _SMU_PPUPATD0_CAN0_SHIFT,           /**< SMU peripheral identifier for CAN0      */
  smuPeripheralCMU       = _SMU_PPUPATD0_CMU_SHIFT,            /**< SMU peripheral identifier for CMU       */
  smuPeripheralCRYOTIMER = _SMU_PPUPATD0_CRYOTIMER_SHIFT,      /**< SMU peripheral identifier for CRYOTIMER */
  smuPeripheralCRYPTO0   = _SMU_PPUPATD0_CRYPTO0_SHIFT,        /**< SMU peripheral identifier for CRYPTO0   */
  smuPeripheralCSEN      = _SMU_PPUPATD0_CSEN_SHIFT,           /**< SMU peripheral identifier for CSEN      */
  smuPeripheralVDAC0     = _SMU_PPUPATD0_VDAC0_SHIFT,          /**< SMU peripheral identifier for VDAC0     */
  smuPeripheralPRS       = _SMU_PPUPATD0_PRS_SHIFT,            /**< SMU peripheral identifier for PRS       */
  smuPeripheralEMU       = _SMU_PPUPATD0_EMU_SHIFT,            /**< SMU peripheral identifier for EMU       */
  smuPeripheralGPCRC     = _SMU_PPUPATD0_GPCRC_SHIFT,          /**< SMU peripheral identifier for GPCRC     */
  smuPeripheralGPIO      = _SMU_PPUPATD0_GPIO_SHIFT,           /**< SMU peripheral identifier for GPIO      */
  smuPeripheralI2C0      = _SMU_PPUPATD0_I2C0_SHIFT,           /**< SMU peripheral identifier for I2C0      */
  smuPeripheralI2C1      = _SMU_PPUPATD0_I2C1_SHIFT,           /**< SMU peripheral identifier for I2C1      */
  smuPeripheralMSC       = _SMU_PPUPATD0_MSC_SHIFT,            /**< SMU peripheral identifier for MAC       */
#if defined(_SMU_PPUPATD0_LCD_SHIFT)
  smuPeripheralLCD       = _SMU_PPUPATD0_LCD_SHIFT,            /**< SMU peripheral identifier for LCD       */
#endif
  smuPeripheralLDMA      = _SMU_PPUPATD0_LDMA_SHIFT,           /**< SMU peripheral identifier for LDMA      */
  smuPeripheralLESENSE   = _SMU_PPUPATD0_LESENSE_SHIFT,        /**< SMU peripheral identifier for LESENSE   */
  smuPeripheralLETIMER0  = _SMU_PPUPATD0_LETIMER0_SHIFT,       /**< SMU peripheral identifier for LETIMER0  */
  smuPeripheralLEUART0   = _SMU_PPUPATD0_LEUART0_SHIFT,        /**< SMU peripheral identifier for LEUART0   */
  smuPeripheralPCNT0     = _SMU_PPUPATD0_PCNT0_SHIFT,          /**< SMU peripheral identifier for PCNT0     */
  smuPeripheralRMU       = _SMU_PPUPATD0_RMU_SHIFT,            /**< SMU peripheral identifier for RMU       */
  smuPeripheralRTCC      = _SMU_PPUPATD0_RTCC_SHIFT,           /**< SMU peripheral identifier for RTCC      */
  smuPeripheralSMU       = _SMU_PPUPATD0_SMU_SHIFT,            /**< SMU peripheral identifier for SMU       */
  smuPeripheralTIMER0    = _SMU_PPUPATD0_TIMER0_SHIFT,         /**< SMU peripheral identifier for TIMER0    */
  smuPeripheralTIMER1    = _SMU_PPUPATD0_TIMER1_SHIFT,         /**< SMU peripheral identifier for TIMER0    */
  smuPeripheralTRNG0     = _SMU_PPUPATD0_TRNG0_SHIFT,          /**< SMU peripheral identifier for TRNG0     */
  smuPeripheralUART0     = _SMU_PPUPATD0_UART0_SHIFT,          /**< SMU peripheral identifier for UART0     */
  smuPeripheralUSART0    = _SMU_PPUPATD0_USART0_SHIFT,         /**< SMU peripheral identifier for USART0    */
  smuPeripheralUSART1    = _SMU_PPUPATD0_USART1_SHIFT,         /**< SMU peripheral identifier for USART1    */
  smuPeripheralUSART2    = _SMU_PPUPATD0_USART2_SHIFT,         /**< SMU peripheral identifier for USART2    */
  smuPeripheralUSART3    = 32 + _SMU_PPUPATD1_USART3_SHIFT,    /**< SMU peripheral identifier for USART3    */
  smuPeripheralWDOG0     = 32 + _SMU_PPUPATD1_WDOG0_SHIFT,     /**< SMU peripheral identifier for WDOG0     */
  smuPeripheralWTIMER0   = 32 + _SMU_PPUPATD1_WTIMER0_SHIFT,   /**< SMU peripheral identifier for WTIMER0   */
  smuPeripheralWTIMER1   = 32 + _SMU_PPUPATD1_WTIMER1_SHIFT,   /**< SMU peripheral identifier for WTIMER1   */

#else
#error "No peripherals defined for SMU for this device configuration."
#endif
  smuPeripheralEnd
} SMU_Peripheral_TypeDef;

/** SMU peripheral privileged access enablers. */
typedef struct {
#if defined(_SILICON_LABS_GECKO_INTERNAL_SDID_84)
  bool privilegedACMP0      : 1;     /**< Privileged access enabler for ACMP0     */
  bool privilegedACMP1      : 1;     /**< Privileged access enabler for ACMP1     */
  bool privilegedADC0       : 1;     /**< Privileged access enabler for ADC0      */
  bool privilegedReserved0  : 1;     /**< Reserved privileged access enabler      */
  bool privilegedReserved1  : 1;     /**< Reserved privileged access enabler      */
  bool privilegedCMU        : 1;     /**< Privileged access enabler for CMU       */
  bool privilegedReserved2  : 1;     /**< Reserved privileged access enabler      */
  bool privilegedCRYOTIMER  : 1;     /**< Privileged access enabler for CRYOTIMER */
  bool privilegedCRYPTO0    : 1;     /**< Privileged access enabler for CRYPTO0   */
  bool privilegedCRYPTO1    : 1;     /**< Privileged access enabler for CRYPTO1   */
  bool privilegedCSEN       : 1;     /**< Privileged access enabler for CSEN      */
  bool privilegedVDAC0      : 1;     /**< Privileged access enabler for VDAC0     */
  bool privilegedPRS        : 1;     /**< Privileged access enabler for PRS       */
  bool privilegedEMU        : 1;     /**< Privileged access enabler for EMU       */
  bool privilegedFPUEH      : 1;     /**< Privileged access enabler for FPUEH     */
  bool privilegedReserved3  : 1;     /**< Reserved privileged access enabler      */
  bool privilegedGPCRC      : 1;     /**< Privileged access enabler for GPCRC     */
  bool privilegedGPIO       : 1;     /**< Privileged access enabler for GPIO      */
  bool privilegedI2C0       : 1;     /**< Privileged access enabler for I2C0      */
  bool privilegedI2C1       : 1;     /**< Privileged access enabler for I2C1      */
  bool privilegedIDAC0      : 1;     /**< Privileged access enabler for IDAC0     */
  bool privilegedMSC        : 1;     /**< Privileged access enabler for MSC       */
  bool privilegedLDMA       : 1;     /**< Privileged access enabler for LDMA      */
  bool privilegedLESENSE    : 1;     /**< Privileged access enabler for LESENSE   */
  bool privilegedLETIMER0   : 1;     /**< Privileged access enabler for LETIMER0  */
  bool privilegedLEUART0    : 1;     /**< Privileged access enabler for LEUART0   */
  bool privilegedReserved4  : 1;     /**< Reserved privileged access enabler      */
  bool privilegedPCNT0      : 1;     /**< Privileged access enabler for PCNT0     */
  bool privilegedPCNT1      : 1;     /**< Privileged access enabler for PCNT1     */
  bool privilegedPCNT2      : 1;     /**< Privileged access enabler for PCNT2     */
  bool privilegedReserved5  : 1;     /**< Reserved privileged access enabler      */
  bool privilegedReserved6  : 1;     /**< Reserved privileged access enabler      */
  bool privilegedReserved7  : 1;     /**< Reserved privileged access enabler      */
  bool privilegedRMU        : 1;     /**< Privileged access enabler for RMU       */
  bool privilegedRTCC       : 1;     /**< Privileged access enabler for RTCC      */
  bool privilegedSMU        : 1;     /**< Privileged access enabler for SMU       */
  bool privilegedReserved8  : 1;     /**< Reserved privileged access enabler      */
  bool privilegedTIMER0     : 1;     /**< Privileged access enabler for TIMER0    */
  bool privilegedTIMER1     : 1;     /**< Privileged access enabler for TIMER1    */
  bool privilegedTRNG0      : 1;     /**< Privileged access enabler for TRNG0     */
  bool privilegedUSART0     : 1;     /**< Privileged access enabler for USART0    */
  bool privilegedUSART1     : 1;     /**< Privileged access enabler for USART1    */
  bool privilegedUSART2     : 1;     /**< Privileged access enabler for USART2    */
  bool privilegedUSART3     : 1;     /**< Privileged access enabler for USART3    */
  bool privilegedWDOG0      : 1;     /**< Privileged access enabler for WDOG0     */
  bool privilegedWDOG1      : 1;     /**< Privileged access enabler for WDOG1     */
  bool privilegedWTIMER0    : 1;     /**< Privileged access enabler for WTIMER0   */
  bool privilegedWTIMER1    : 1;     /**< Privileged access enabler for WTIMER1   */

#elif defined(_SILICON_LABS_GECKO_INTERNAL_SDID_89)
  bool privilegedACMP0      : 1;     /**< Privileged access enabler for ACMP0     */
  bool privilegedACMP1      : 1;     /**< Privileged access enabler for ACMP1     */
  bool privilegedADC0       : 1;     /**< Privileged access enabler for ADC0      */
  bool privilegedReserved0  : 1;     /**< Reserved privileged access enabler      */
  bool privilegedReserved1  : 1;     /**< Reserved privileged access enabler      */
  bool privilegedCMU        : 1;     /**< Privileged access enabler for CMU       */
  bool privilegedReserved2  : 1;     /**< Reserved privileged access enabler      */
  bool privilegedCRYOTIMER  : 1;     /**< Privileged access enabler for CRYOTIMER */
  bool privilegedCRYPTO0    : 1;     /**< Privileged access enabler for CRYPTO0   */
  bool privilegedCRYPTO1    : 1;     /**< Privileged access enabler for CRYPTO1   */
  bool privilegedCSEN       : 1;     /**< Privileged access enabler for CSEN      */
  bool privilegedVDAC0      : 1;     /**< Privileged access enabler for VDAC0     */
  bool privilegedPRS        : 1;     /**< Privileged access enabler for PRS       */
  bool privilegedEMU        : 1;     /**< Privileged access enabler for EMU       */
  bool privilegedFPUEH      : 1;     /**< Privileged access enabler for FPUEH     */
  bool privilegedReserved3  : 1;     /**< Reserved privileged access enabler      */
  bool privilegedGPCRC      : 1;     /**< Privileged access enabler for GPCRC     */
  bool privilegedGPIO       : 1;     /**< Privileged access enabler for GPIO      */
  bool privilegedI2C0       : 1;     /**< Privileged access enabler for I2C0      */
  bool privilegedI2C1       : 1;     /**< Privileged access enabler for I2C1      */
  bool privilegedIDAC0      : 1;     /**< Privileged access enabler for IDAC0     */
  bool privilegedMSC        : 1;     /**< Privileged access enabler for MSC       */
  bool privilegedLDMA       : 1;     /**< Privileged access enabler for LDMA      */
  bool privilegedLESENSE    : 1;     /**< Privileged access enabler for LESENSE   */
  bool privilegedLETIMER0   : 1;     /**< Privileged access enabler for LETIMER0  */
  bool privilegedLEUART0    : 1;     /**< Privileged access enabler for LEUART0   */
  bool privilegedReserved4  : 1;     /**< Reserved privileged access enabler      */
  bool privilegedPCNT0      : 1;     /**< Privileged access enabler for PCNT0     */
  bool privilegedReserved5  : 1;     /**< Reserved privileged access enabler      */
  bool privilegedReserved6  : 1;     /**< Reserved privileged access enabler      */
  bool privilegedReserved7  : 1;     /**< Reserved privileged access enabler      */
  bool privilegedReserved8  : 1;     /**< Reserved privileged access enabler      */
  bool privilegedRMU        : 1;     /**< Privileged access enabler for RMU       */
  bool privilegedRTCC       : 1;     /**< Privileged access enabler for RTCC      */
  bool privilegedSMU        : 1;     /**< Privileged access enabler for SMU       */
  bool privilegedReserved9  : 1;     /**< Reserved privileged access enabler      */
  bool privilegedTIMER0     : 1;     /**< Privileged access enabler for TIMER0    */
  bool privilegedTIMER1     : 1;     /**< Privileged access enabler for TIMER1    */
  bool privilegedTRNG0      : 1;     /**< Privileged access enabler for TRNG0     */
  bool privilegedUSART0     : 1;     /**< Privileged access enabler for USART0    */
  bool privilegedUSART1     : 1;     /**< Privileged access enabler for USART1    */
  bool privilegedUSART2     : 1;     /**< Privileged access enabler for USART2    */
  bool privilegedWDOG0      : 1;     /**< Privileged access enabler for WDOG0     */
  bool privilegedWDOG1      : 1;     /**< Privileged access enabler for WDOG1     */
  bool privilegedWTIMER0    : 1;     /**< Privileged access enabler for WTIMER0   */

#elif defined(_SILICON_LABS_GECKO_INTERNAL_SDID_95)
  bool privilegedACMP0      : 1;     /**< Privileged access enabler for           */
  bool privilegedACMP1      : 1;     /**< Privileged access enabler for           */
  bool privilegedADC0       : 1;     /**< Privileged access enabler for           */
  bool privilegedReserved0  : 1;
  bool privilegedReserved1  : 1;
  bool privilegedCMU        : 1;     /**< Privileged access enabler for           */
  bool privilegedReserved2  : 1;
  bool privilegedCRYOTIMER  : 1;     /**< Privileged access enabler for           */
  bool privilegedCRYPTO     : 1;     /**< Privileged access enabler for           */
  bool privilegedVDAC0      : 1;     /**< Privileged access enabler for           */
  bool privilegedPRS        : 1;     /**< Privileged access enabler for           */
  bool privilegedEMU        : 1;     /**< Privileged access enabler for           */
  bool privilegedFPUEH      : 1;     /**< Privileged access enabler for           */
  bool privilegedReserved3  : 1;
  bool privilegedGPCRC      : 1;     /**< Privileged access enabler for           */
  bool privilegedGPIO       : 1;     /**< Privileged access enabler for           */
  bool privilegedI2C0       : 1;     /**< Privileged access enabler for           */
  bool privilegedIDAC0      : 1;     /**< Privileged access enabler for           */
  bool privilegedMSC        : 1;     /**< Privileged access enabler for           */
  bool privilegedLDMA       : 1;     /**< Privileged access enabler for           */
  bool privilegedLESENSE    : 1;     /**< Privileged access enabler for           */
  bool privilegedLETIMER0   : 1;     /**< Privileged access enabler for           */
  bool privilegedLEUART     : 1;     /**< Privileged access enabler for           */
  bool privilegedReserved4  : 1;
  bool privilegedPCNT0      : 1;     /**< Privileged access enabler for           */
  bool privilegedReserved5  : 1;
  bool privilegedReserved6  : 1;
  bool privilegedReserved7  : 1;
  bool privilegedReserved8  : 1;
  bool privilegedRMU        : 1;     /**< Privileged access enabler for           */
  bool privilegedRTCC       : 1;     /**< Privileged access enabler for           */
  bool privilegedSMU        : 1;     /**< Privileged access enabler for           */

  bool privilegedReserved9  : 1;
  bool privilegedTIMER0     : 1;     /**< Privileged access enabler for           */
  bool privilegedTIMER1     : 1;     /**< Privileged access enabler for           */
  bool privilegedTRNG0      : 1;     /**< Privileged access enabler for           */
  bool privilegedUSART0     : 1;     /**< Privileged access enabler for           */
  bool privilegedUSART1     : 1;     /**< Privileged access enabler for           */
  bool privilegedWDOG0      : 1;     /**< Privileged access enabler for           */
  bool privilegedWDOG1      : 1;     /**< Privileged access enabler for           */
  bool privilegedWTIMER0    : 1;     /**< Privileged access enabler for           */

#elif defined(_SILICON_LABS_GECKO_INTERNAL_SDID_100)
  bool privilegedACMP0      : 1;     /**< Privileged access enabler for ACMP0     */
  bool privilegedACMP1      : 1;     /**< Privileged access enabler for ACMP1     */
  bool privilegedACMP2      : 1;     /**< Privileged access enabler for ACMP2     */
  bool privilegedACMP3      : 1;     /**< Privileged access enabler for ACMP3     */
  bool privilegedADC0       : 1;     /**< Privileged access enabler for ADC0      */
  bool privilegedADC1       : 1;     /**< Privileged access enabler for ADC1      */
  bool privilegedCAN0       : 1;     /**< Privileged access enabler for CAN0      */
  bool privilegedCAN1       : 1;     /**< Privileged access enabler for CAN1      */
  bool privilegedCMU        : 1;     /**< Privileged access enabler for CMU       */
  bool privilegedCRYOTIMER  : 1;     /**< Privileged access enabler for CRYOTIMER */
  bool privilegedCRYPTO0    : 1;     /**< Privileged access enabler for CRYPTO0   */
  bool privilegedCSEN       : 1;     /**< Privileged access enabler for CSEN      */
  bool privilegedVDAC0      : 1;     /**< Privileged access enabler for VDAC0     */
  bool privilegedPRS        : 1;     /**< Privileged access enabler for PRS       */
  bool privilegedEBI        : 1;     /**< Privileged access enabler for EBI       */
  bool privilegedEMU        : 1;     /**< Privileged access enabler for EMU       */
  bool privilegedETH        : 1;     /**< Privileged access enabler for ETH       */
  bool privilegedFPUEH      : 1;     /**< Privileged access enabler for FPUEH     */
  bool privilegedGPCRC      : 1;     /**< Privileged access enabler for GPCRC     */
  bool privilegedGPIO       : 1;     /**< Privileged access enabler for GPIO      */
  bool privilegedI2C0       : 1;     /**< Privileged access enabler for I2C0      */
  bool privilegedI2C1       : 1;     /**< Privileged access enabler for I2C1      */
  bool privilegedI2C2       : 1;     /**< Privileged access enabler for I2C2      */
  bool privilegedIDAC0      : 1;     /**< Privileged access enabler for IDAC0     */
  bool privilegedMSC        : 1;     /**< Privileged access enabler for MAC       */
  bool privilegedLCD        : 1;     /**< Privileged access enabler for LCD       */
  bool privilegedLDMA       : 1;     /**< Privileged access enabler for LDMA      */
  bool privilegedLESENSE    : 1;     /**< Privileged access enabler for LESENSE   */
  bool privilegedLETIMER0   : 1;     /**< Privileged access enabler for LETIMER0  */
  bool privilegedLETIMER1   : 1;     /**< Privileged access enabler for LETIMER1  */
  bool privilegedLEUART0    : 1;     /**< Privileged access enabler for LEUART0   */
  bool privilegedLEUART1    : 1;     /**< Privileged access enabler for LEUART1   */
  bool privilegedPCNT0      : 1;     /**< Privileged access enabler for PCNT0     */
  bool privilegedPCNT1      : 1;     /**< Privileged access enabler for PCNT1     */
  bool privilegedPCNT2      : 1;     /**< Privileged access enabler for PCNT2     */
  bool privilegedQSPI0      : 1;     /**< Privileged access enabler for QSPI0     */
  bool privilegedRMU        : 1;     /**< Privileged access enabler for RMU       */
  bool privilegedRTC        : 1;     /**< Privileged access enabler for RTC       */
  bool privilegedRTCC       : 1;     /**< Privileged access enabler for RTCC      */
  bool privilegedSDIO       : 1;     /**< Privileged access enabler for SDIO      */
  bool privilegedSMU        : 1;     /**< Privileged access enabler for SMU       */
  bool privilegedTIMER0     : 1;     /**< Privileged access enabler for TIMER0    */
  bool privilegedTIMER1     : 1;     /**< Privileged access enabler for TIMER1    */
  bool privilegedTIMER2     : 1;     /**< Privileged access enabler for TIMER2    */
  bool privilegedTIMER3     : 1;     /**< Privileged access enabler for TIMER3    */
  bool privilegedTIMER4     : 1;     /**< Privileged access enabler for TIMER4    */
  bool privilegedTIMER5     : 1;     /**< Privileged access enabler for TIMER5    */
  bool privilegedTIMER6     : 1;     /**< Privileged access enabler for TIMER6    */
  bool privilegedTRNG0      : 1;     /**< Privileged access enabler for TRNG0     */
  bool privilegedUART0      : 1;     /**< Privileged access enabler for UART0     */
  bool privilegedUART1      : 1;     /**< Privileged access enabler for UART1     */
  bool privilegedUSART0     : 1;     /**< Privileged access enabler for USART0    */
  bool privilegedUSART1     : 1;     /**< Privileged access enabler for USART1    */
  bool privilegedUSART2     : 1;     /**< Privileged access enabler for USART2    */
  bool privilegedUSART3     : 1;     /**< Privileged access enabler for USART3    */
  bool privilegedUSART4     : 1;     /**< Privileged access enabler for USART4    */
  bool privilegedUSART5     : 1;     /**< Privileged access enabler for USART5    */
  bool privilegedUSB        : 1;     /**< Privileged access enabler for USB       */
  bool privilegedWDOG0      : 1;     /**< Privileged access enabler for WDOG0     */
  bool privilegedWDOG1      : 1;     /**< Privileged access enabler for WDOG1     */
  bool privilegedWTIMER0    : 1;     /**< Privileged access enabler for WTIMER0   */
  bool privilegedWTIMER1    : 1;     /**< Privileged access enabler for WTIMER1   */
  bool privilegedWTIMER2    : 1;     /**< Privileged access enabler for WTIMER2   */
  bool privilegedWTIMER3    : 1;     /**< Privileged access enabler for WTIMER3   */

#elif defined(_SILICON_LABS_GECKO_INTERNAL_SDID_103)
  bool privilegedACMP0      : 1;     /**< Privileged access enabler for ACMP0     */
  bool privilegedACMP1      : 1;     /**< Privileged access enabler for ACMP1     */
  bool privilegedADC0       : 1;     /**< Privileged access enabler for ADC0      */
  bool privilegedCAN0       : 1;     /**< Privileged access enabler for CAN0      */
  bool privilegedCMU        : 1;     /**< Privileged access enabler for CMU       */
  bool privilegedCRYOTIMER  : 1;     /**< Privileged access enabler for CRYOTIMER */
  bool privilegedCRYPTO0    : 1;     /**< Privileged access enabler for CRYPTO0   */
  bool privilegedCSEN       : 1;     /**< Privileged access enabler for CSEN      */
  bool privilegedVDAC0      : 1;     /**< Privileged access enabler for VDAC0     */
  bool privilegedPRS        : 1;     /**< Privileged access enabler for PRS       */
  bool privilegedEMU        : 1;     /**< Privileged access enabler for EMU       */
  bool privilegedGPCRC      : 1;     /**< Privileged access enabler for GPCRC     */
  bool privilegedGPIO       : 1;     /**< Privileged access enabler for GPIO      */
  bool privilegedI2C0       : 1;     /**< Privileged access enabler for I2C0      */
  bool privilegedI2C1       : 1;     /**< Privileged access enabler for I2C1      */
  bool privilegedMSC        : 1;     /**< Privileged access enabler for MAC       */
  bool privilegedLCD        : 1;     /**< Privileged access enabler for LCD       */
  bool privilegedLDMA       : 1;     /**< Privileged access enabler for LDMA      */
  bool privilegedLESENSE    : 1;     /**< Privileged access enabler for LESENSE   */
  bool privilegedLETIMER0   : 1;     /**< Privileged access enabler for LETIMER0  */
  bool privilegedLEUART0    : 1;     /**< Privileged access enabler for LEUART0   */
  bool privilegedPCNT0      : 1;     /**< Privileged access enabler for PCNT0     */
  bool privilegedRMU        : 1;     /**< Privileged access enabler for RMU       */
  bool privilegedRTCC       : 1;     /**< Privileged access enabler for RTCC      */
  bool privilegedSMU        : 1;     /**< Privileged access enabler for SMU       */
  bool privilegedTIMER0     : 1;     /**< Privileged access enabler for TIMER0    */
  bool privilegedTIMER1     : 1;     /**< Privileged access enabler for TIMER1    */
  bool privilegedTRNG0      : 1;     /**< Privileged access enabler for TRNG0     */
  bool privilegedUART0      : 1;     /**< Privileged access enabler for UART0     */
  bool privilegedUSART0     : 1;     /**< Privileged access enabler for USART0    */
  bool privilegedUSART1     : 1;     /**< Privileged access enabler for USART1    */
  bool privilegedUSART2     : 1;     /**< Privileged access enabler for USART2    */
  bool privilegedUSART3     : 1;     /**< Privileged access enabler for USART3    */
  bool privilegedWDOG0      : 1;     /**< Privileged access enabler for WDOG0     */
  bool privilegedWTIMER0    : 1;     /**< Privileged access enabler for WTIMER0   */
  bool privilegedWTIMER1    : 1;     /**< Privileged access enabler for WTIMER1   */

#else
#error "No peripherals defined for SMU for this device configuration"
#endif
} SMU_PrivilegedAccess_TypeDef;

/*******************************************************************************
 ******************************   STRUCTS   ************************************
 ******************************************************************************/

/** SMU initialization structure. */
typedef struct {
  union {
    uint32_t reg[2];                      /**< Periperal access control array.*/
    SMU_PrivilegedAccess_TypeDef access;  /**< Periperal access control array.*/
  } ppu;
  bool enable;                            /**< SMU enable flag, when set SMU_Init() will enable SMU.*/
} SMU_Init_TypeDef;

/** Default SMU initialization struct settings. */
#define SMU_INIT_DEFAULT {                                        \
    { { 0 } },             /* No peripherals acsess protected. */ \
    true                   /* Enable SMU.*/                       \
}

/*******************************************************************************
 *****************************   PROTOTYPES   **********************************
 ******************************************************************************/

/***************************************************************************//**
 * @brief
 *   Enable or disable the Peripheral Protection Unit of the SMU.
 *
 * @param[in] enable
 *   True if the PPU should be enabled, false if it should be disabled.
 ******************************************************************************/
__STATIC_INLINE void SMU_EnablePPU(bool enable)
{
  BUS_RegBitWrite(&SMU->PPUCTRL, _SMU_PPUCTRL_ENABLE_SHIFT, enable);
}

/***************************************************************************//**
 * @brief
 *   Initialize the Peripheral Protection Unit of the SMU.
 *
 * @param[in] init
 *   Pointer to initialization struct defining which peripherals should only
 *   be accessed from privileged mode, and whether the PPU should be enabled.
 ******************************************************************************/
__STATIC_INLINE void SMU_Init(const SMU_Init_TypeDef *init)
{
  SMU->PPUPATD0 = init->ppu.reg[0];
  SMU->PPUPATD1 = init->ppu.reg[1];

  SMU_EnablePPU(init->enable);
}

/***************************************************************************//**
 * @brief
 *   Change the access settings for a peripheral
 *
 * @details
 *   Set whether the peripheral can only be accessed from privileged mode
 *
 * @param[in] peripheral
 *   ID of the peripheral to change access settings for
 *
 * @param[in] privileged
 *   True if the peripheral should only be allowed to be accessed from
 *   privileged mode, false if the peripheral can be accessed from unprivileged
 *   mode.
 ******************************************************************************/
__STATIC_INLINE void SMU_SetPrivilegedAccess(SMU_Peripheral_TypeDef peripheral,
                                             bool privileged)
{
  EFM_ASSERT(peripheral < smuPeripheralEnd);

  if (peripheral < 32) {
    BUS_RegBitWrite(&SMU->PPUPATD0, peripheral, privileged);
  } else {
    BUS_RegBitWrite(&SMU->PPUPATD1, peripheral - 32, privileged);
  }
}

/***************************************************************************//**
 * @brief
 *   Get the ID of the peripheral that caused an access fault.
 *
 * @note
 *   The return value is only valid if the @ref SMU_IF_PPUPRIV interrupt flag
 *   is set.
 *
 * @return
 *   ID of the faulting peripheral.
 ******************************************************************************/
__STATIC_INLINE SMU_Peripheral_TypeDef SMU_GetFaultingPeripheral(void)
{
  return (SMU_Peripheral_TypeDef)SMU->PPUFS;
}

/***************************************************************************//**
 * @brief
 *   Clear one or more pending SMU interrupts.
 *
 * @param[in] flags
 *   Bitwise logic OR of SMU interrupt sources to clear.
 ******************************************************************************/
__STATIC_INLINE void SMU_IntClear(uint32_t flags)
{
  SMU->IFC = flags;
}

/***************************************************************************//**
 * @brief
 *   Disable one or more SMU interrupts.
 *
 * @param[in] flags
 *   SMU interrupt sources to disable.
 ******************************************************************************/
__STATIC_INLINE void SMU_IntDisable(uint32_t flags)
{
  SMU->IEN &= ~flags;
}

/***************************************************************************//**
 * @brief
 *   Enable one or more SMU interrupts.
 *
 * @note
 *   Depending on the use, a pending interrupt may already be set prior to
 *   enabling the interrupt. Consider using SMU_IntClear() prior to enabling
 *   if such a pending interrupt should be ignored.
 *
 * @param[in] flags
 *   SMU interrupt sources to enable.
 ******************************************************************************/
__STATIC_INLINE void SMU_IntEnable(uint32_t flags)
{
  SMU->IEN |= flags;
}

/***************************************************************************//**
 * @brief
 *   Get pending SMU interrupts.
 *
 * @return
 *   SMU interrupt sources pending.
 ******************************************************************************/
__STATIC_INLINE uint32_t SMU_IntGet(void)
{
  return SMU->IF;
}

/***************************************************************************//**
 * @brief
 *   Get enabled and pending SMU interrupt flags.
 *   Useful for handling more interrupt sources in the same interrupt handler.
 *
 * @note
 *   Interrupt flags are not cleared by the use of this function.
 *
 * @return
 *   Pending and enabled SMU interrupt sources.
 *   The return value is the bitwise AND combination of
 *   - the OR combination of enabled interrupt sources in SMU_IEN register
 *     and
 *   - the OR combination of valid interrupt flags in SMU_IF register.
 ******************************************************************************/
__STATIC_INLINE uint32_t SMU_IntGetEnabled(void)
{
  uint32_t tmp;

  // Store SMU->IEN in temporary variable in order to define explicit order
  // of volatile accesses.
  tmp = SMU->IEN;

  // Bitwise AND of pending and enabled interrupts
  return SMU->IF & tmp;
}

/************************************************************************Æ**//**
 * @brief
 *   Set one or more pending SMU interrupts from SW.
 *
 * @param[in] flags
 *   SMU interrupt sources to set to pending.
 *************************************************************************Æ****/
__STATIC_INLINE void SMU_IntSet(uint32_t flags)
{
  SMU->IFS = flags;
}

/** @} (end addtogroup SMU) */
/** @} (end addtogroup emlib) */

#ifdef __cplusplus
}
#endif

#endif // defined(SMU_COUNT) && (SMU_COUNT > 0)
#endif // EM_SMU_H
