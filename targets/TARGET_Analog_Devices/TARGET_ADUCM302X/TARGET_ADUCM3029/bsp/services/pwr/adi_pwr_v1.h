/*!
 *****************************************************************************
 * @file:    adi_pwr_v1.h
 * @brief:   CMSIS Cortex-M3 Device clock and power management function 
 * @version: $Revision$
 * @date:    $Date$
 *-----------------------------------------------------------------------------
Copyright (c) 2010-2014 Analog Devices, Inc.

All rights reserved.

Redistribution and use in source and binary forms, with or without modification,
are permitted provided that the following conditions are met:
  - Redistributions of source code must retain the above copyright notice,
    this list of conditions and the following disclaimer.
  - Redistributions in binary form must reproduce the above copyright notice,
    this list of conditions and the following disclaimer in the documentation
    and/or other materials provided with the distribution.
  - Modified versions of the software must be conspicuously marked as such.
  - This software is licensed solely and exclusively for use with processors
    manufactured by or for Analog Devices, Inc.
  - This software may not be combined or merged with other code in any manner
    that would cause the software to become subject to terms and conditions
    which differ from those listed here.
  - Neither the name of Analog Devices, Inc. nor the names of its
    contributors may be used to endorse or promote products derived
    from this software without specific prior written permission.
  - The use of this software may or may not infringe the patent rights of one
    or more patent holders.  This license does not release you from the
    requirement that you obtain separate licenses from these patent holders
    to use this software.

THIS SOFTWARE IS PROVIDED BY ANALOG DEVICES, INC. AND CONTRIBUTORS "AS IS"
AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, NON-
INFRINGEMENT, TITLE, MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
DISCLAIMED. IN NO EVENT SHALL ANALOG DEVICES, INC. OR CONTRIBUTORS BE LIABLE
FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, PUNITIVE OR
CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, DAMAGES ARISING OUT OF
CLAIMS OF INTELLECTUAL PROPERTY RIGHTS INFRINGEMENT; PROCUREMENT OF
SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
POSSIBILITY OF SUCH DAMAGE.

*****************************************************************************/


/*! \addtogroup Power_Service Dynamic Power Management Service
 *  @{
 * add result types to doxygen
 */

#ifndef __ADI_PWR_V1_H__
#define __ADI_PWR_V1_H__

#include <services/int/adi_int.h>
#include <adi_types.h>

#ifdef __ICCARM__
/* IAR MISRA C 2004 error suppressions.
 *
 * Pm009 (rule 5.1): identifiers shall not rely on significance of more than 31 characters.
 *   IAR compiler supports longer identifiers.
 */
#pragma diag_suppress=Pm009
#endif /* __ICCARM__ */

#ifdef __cplusplus
 extern "C" {
#endif
   
/*! Offset for clock events  */  
#define CLOCK_EVENT_OFFSET   16u

/*! Enum for clock IDs */
typedef enum {
    /*!Source for all peripherals SPI,SPORT,SIP,CRC,AES,SIP interface, I2C,UART,optionally for timers  */  
    ADI_CLOCK_PCLK,    
    /*!Source for Core,Bus etc */    
    ADI_CLOCK_HCLK
} ADI_CLOCK_ID;

/*! Enum for clock Clock sources */
typedef enum {
   /*! Clock ID for 16 MHz or 26 MHz external crystal oscillator called  HFXTAL */    
    ADI_CLOCK_SOURCE_HFXTAL,
   /*! Clock ID 32 kHz external crystal oscillator called  LFXTL */            
    ADI_CLOCK_SOURCE_LFXTAL,
   /*! Clock ID for 26 MHz internal oscillator called  HFOSC */            
    ADI_CLOCK_SOURCE_HFOSC,
   /*! Clock ID 32 kHz a 32 kHz internal oscillator called  LFXTL */            
    ADI_CLOCK_SOURCE_LFOSC,
   /*! Clock ID for output clock for System PLL */            
    ADI_CLOCK_SOURCE_SPLL,
   /*! Clock ID for external clock from GPIO */                
    ADI_CLOCK_SOURCE_GPIO,
} ADI_CLOCK_SOURCE_ID;

/*! 
 * Clock multiplexer identifiers       
*/
typedef enum {
    /*! SPLL input mux                */  
    ADI_CLOCK_MUX_SPLL,         
    /*! 32kHz (LFLCK) clock mux        */    
    ADI_CLOCK_MUX_LFCLK,        
   /*!  Clock out mux                  */    
    ADI_CLOCK_MUX_REFCLK,       
   /*! root_clk mux                   */    
    ADI_CLOCK_MUX_ROOT,         
} ADI_CLOCK_MUX_GROUP_ID;

/*! 
 * Clock multiplexer selection       
*/
typedef enum {

    /*! Input clock for  system PLL mux is HFOSC */
    ADI_CLOCK_MUX_SPLL_HF_OSC, 
    /*! Input clock for  system PLL mux is HFXTL */    
    ADI_CLOCK_MUX_SPLL_HF_XTAL,

    /*! Input clock for  low frequency clock mux is LFOSC */
    ADI_CLOCK_MUX_LFCLK_LFOSC,
    /*! Input clock for  low frequency clock mux is LFXTL */    
    ADI_CLOCK_MUX_LFCLK_LFXTAL,

    /*! Input clock to the multiplexer which provides reference clock for Flash 
      and HPBUCK clock is HFOSC */
    ADI_CLOCK_MUX_REF_HFOSC_CLK,
    /*! Input clock to the multiplexer which provides reference clock for Flash 
      and HPBUCK clock is HFXTL */
    ADI_CLOCK_MUX_REF_HFXTL_CLK,

    /*! Inoput clock to root multiplexer is HFOSC */                                            
    ADI_CLOCK_MUX_ROOT_HFOSC,
    /*! Inoput clock to root multiplexer is HFXTL */                                                
    ADI_CLOCK_MUX_ROOT_HFXTAL,
    /*! Inoput clock to root multiplexer is SPLL */                                                
    ADI_CLOCK_MUX_ROOT_SPLL,
    /*! Inoput clock to root multiplexer is from GPIO */                                                
    ADI_CLOCK_MUX_ROOT_GPIO,
} ADI_CLOCK_MUX_ID;

/*! \enum ADI_CLOCK_SOURCE_STATUS
 *  Clock source status. 
 */
typedef enum {
    /*! Specified clcock source is disabled */
    ADI_CLOCK_SOURCE_DISABLED = 0,
    /*! Specified clcock source is not stable*/    
    ADI_CLOCK_SOURCE_ENABLED_NOT_STABLE,
    /*! Specified clcock source is enabled and  stable*/        
    ADI_CLOCK_SOURCE_ENABLED_STABLE,
    /*! Invalid clock ID */
    ADI_CLOCK_SOURCE_ID_NOT_VALID
} ADI_CLOCK_SOURCE_STATUS;

/*! Enum for CLOCK GATE IDs */
typedef enum {
    /*! Clock Gate for the GP Timer-0 */    
    ADI_CLOCK_GATE_GPT0_CLK = 1 << BITP_CLKG_CLK_CTL5_GPTCLK0OFF,
    /*! Clock Gate for the GP Timer-1 */        
    ADI_CLOCK_GATE_GPT1_CLK = 1 << BITP_CLKG_CLK_CTL5_GPTCLK1OFF,
    /*! Clock Gate for the GP Timer-2 */        
    ADI_CLOCK_GATE_GPT2_CLK = 1 << BITP_CLKG_CLK_CTL5_GPTCLK2OFF,
    /*! Clock Gate for the I2C */        
    ADI_CLOCK_GATE_I2C_CLK  = 1 << BITP_CLKG_CLK_CTL5_UCLKI2COFF,
    /*! Clock Gate for the PCLK */                
    ADI_CLOCK_GATE_PCLK     = 1 << BITP_CLKG_CLK_CTL5_PERCLKOFF 
} ADI_CLOCK_GATE_ID;

 /*!
 *****************************************************************************
 * \enum ADI_PWR_RESULT
 *
  * System API function return codes
 *
 *****************************************************************************/
typedef enum
{
    /*! No error detected.                                    */  
    ADI_PWR_SUCCESS = 0,              
    /*! Generic unknown error occurred.                       */    
    ADI_PWR_FAILURE,                  
    /*! Requested divide value is out of range.               */    
    ADI_PWR_INVALID_CLOCK_DIVIDER,    
    /*! Invalid ADI_CLOCK_ID specified.                       */    
    ADI_PWR_INVALID_CLOCK_ID,         
    /*! PDIV:HDIV ratio must be integral.                     */    
    ADI_PWR_INVALID_CLOCK_RATIO,      
    /*! Invalid low-power mode requested.                     */    
    ADI_PWR_INVALID_POWER_MODE,       
    /*! Invalid clock speed.                                  */    
    ADI_PWR_INVALID_CLOCK_SPEED,      
    /*! Specified operation is not allowed.                   */    
    ADI_PWR_OPERATION_NOT_ALLOWED,
    /*! Parameter is out of range.                            */    
    ADI_PWR_INVALID_PARAM   
    
} ADI_PWR_RESULT;
/*!
 *   Power modes
 */
typedef enum
{
    /*! Fully Active */    
    ADI_PWR_MODE_ACTIVE         = 0 << BITP_PMG_PWRMOD_MODE,      
    /*! Core Sleep power-down mode */    
    ADI_PWR_MODE_FLEXI          = 1 << BITP_PMG_PWRMOD_MODE,      
    /*! Full Hybernate power-down mode */    
    ADI_PWR_MODE_HIBERNATE      = 2 << BITP_PMG_PWRMOD_MODE,        
    /*! System Sleep power-down mode */    
    ADI_PWR_MODE_SHUTDOWN       = 3 << BITP_PMG_PWRMOD_MODE      
} ADI_PWR_POWER_MODE;

/*!
 *   Power interrupt type
 */
typedef enum
{
    /*! Enable Interrupt when battery vlotage drops below 1.8V.*/    
    ADI_PWR_LOW_BATTERY_VOLTAGE_IEN   = 1 << BITP_PMG_IEN_VBAT,       
    /*! Enable Interrupt when VREG under-voltage: below 1V. */    
    ADI_PWR_UNDER_VOLATAGE_IEN         = 1 << BITP_PMG_IEN_VREGUNDR,   
    /*! Enable Interrupt when VREG over-voltage: over- 1.32V */    
    ADI_PWR_OVER_VOLATAGE_IEN          = 1 << BITP_PMG_IEN_VREGOVR,    
    /*! Enable the interrupt when battery voltage falls to the specified range.Please see #adi_pwr_SetVoltageRange.*/     
    ADI_PWR_BATTERY_VOLTAGE_RANGE_IEN  = 1 << BITP_PMG_IEN_IENBAT
    
} ADI_PWR_IRQ;


/*!
 *   Power interrupt type
 */
typedef enum
{
    ADI_PWR_OSC_LFXTL_STATUS_IEN     = 1 << BITP_CLKG_CLK_CTL0_LFXTALIE,
    /*!Enable Interrupt when HFXTL clock becomes stable/unstable */     
    ADI_PWR_OSC_HFXTL_STATUS_IEN     = 1 << BITP_CLKG_CLK_CTL0_HFXTALIE,
    /*!Enable Interrupt when PLL-LOCK/PLL-UNLOCK  */     
    ADI_PWR_PLL_STATUS_EVENT        = 1 << BITP_CLKG_CLK_CTL3_SPLLIE
} ADI_CLOCK_IRQ;

/**
 * The power service notification events.
 */
typedef enum {
    /*! Event for indicating Over voltage. VREG  > 1.32v */    
    ADI_PWR_EVENT_OVER_VOLTAGE = BITM_PMG_PSM_STAT_VREGOVR ,            
    /*! Event for indicating  under voltage.VREG < 1V */    
    ADI_PWR_EVENT_UNDER_VOLTAGE = BITM_PMG_PSM_STAT_VREGUNDR,           
    /*! Event for indicating battery voltage below  1.8V */
    ADI_PWR_EVENT_LOW_BATTERY_VOLTAGE = BITM_PMG_PSM_STAT_VBATUNDR,    
    /*! Event for indicating battery voltage in specified range-1.VBAT range1 (> 2.75v) */    
    ADI_PWR_EVENT_BATTERY_VOLTAGE = BITM_PMG_PSM_STAT_RANGE1,
    /*! Event for indicating battery voltage in specified range-2.VBAT range2 (2.75v - 2.3v)   */    
    ADI_PWR_EVENT_BATTERY_VOLTAGE_RANGE_2 = BITM_PMG_PSM_STAT_RANGE2,
    /*! Event for indicating battery voltage in specified range-3.VBAT range3 (2.3v - 1.6v)   */    
    ADI_PWR_EVENT_BATTERY_VOLTAGE_RANGE_3 = BITM_PMG_PSM_STAT_RANGE3,

    /*! Event to indicate HF crystal stable. */    
    ADI_PWR_EVENT_OSC_HFXTL_CLOCK_OK     =   BITM_CLKG_CLK_STAT0_HFXTALOK << CLOCK_EVENT_OFFSET,
    /*! Event to indicate HF crystal is not stable. */    
    ADI_PWR_EVENT_OSC_HFXTL_CLOCK_NO_OK  = BITM_CLKG_CLK_STAT0_HFXTALNOK << CLOCK_EVENT_OFFSET,
    /*! Event to indicate LF crystal is  stable. */    
    ADI_PWR_EVENT_OSC_LFXTL_CLOCK_OK     =  BITM_CLKG_CLK_STAT0_LFXTALOK << CLOCK_EVENT_OFFSET,
    /*! Event to indicate LF crystal is not stable. */    
    ADI_PWR_EVENT_OSC_LFXTL_CLOCK_NO_OK  = BITM_CLKG_CLK_STAT0_LFXTALNOK << CLOCK_EVENT_OFFSET,
    /*! Event for indicating PLL is locked . */    
    ADI_PWR_EVENT_PLLC_LOCK =   BITM_CLKG_CLK_STAT0_SPLLLK << CLOCK_EVENT_OFFSET,
    /*! Event for indicating PLL is unlocked .  */    
    ADI_PWR_EVENT_PLLC_UNLOCK = BITM_CLKG_CLK_STAT0_SPLLUNLK << CLOCK_EVENT_OFFSET
    
} ADI_PWR_EVENT;

/*!
 *   Processor wake up status 
*/
typedef enum
{
    /*! Interrupt from External Interrupt 0 */    
    ADI_PWR_INT_EXT0,    
    /*! Interrupt from External Interrupt 1 */    
    ADI_PWR_INT_EXT1,    
    /*! Interrupt from External Interrupt 2 */        
    ADI_PWR_INT_EXT2,   
    /*! Interrupt from RTC */            
    ADI_PWR_INT_RTC     
} ADI_PWR_WAKEUP_STATUS;

/*!
 *   To set the battery voltage range for interrupt generation.
*/
typedef enum
{
    /*! Voltage range is in safe region  */    
    ADI_PWR_BAT_VOLTAGE_RANGE_SAFE,     
    /*! Battery voltage is in the range of 2.2 to 2.75 V */    
    ADI_PWR_VOLTAGE_RANGE_2_2_TO_2_75,  
    /*! Battery voltage is in the range of 1.6 to 2.2 V  */        
    ADI_PWR_VOLTAGE_RANGE_1_6_TO_2_2    
} ADI_PWR_VOLTAGE_RANGE;


/*  Related clock APIs */

/* 
 * Initialize the dynamic power management service 
 */
ADI_PWR_RESULT  adi_pwr_Init(void);

/* 
 * =================================================================
 *  Clock Management related  APIs 
 * =================================================================
*/

/*
 *  Update the internal clock variable based on current configuration 
 */
ADI_PWR_RESULT adi_pwr_UpdateCoreClock(void);

/* 
 * Set the external clock frequency.
 */
ADI_PWR_RESULT adi_pwr_SetExtClkFreq(
               uint32_t ExtClkFreq
               );

/* 
 * To Configure the root clock muxing 
 */
ADI_PWR_RESULT adi_pwr_SetRootClockMux(
               ADI_CLOCK_MUX_ID eClockID
               );

/*
 * To Configure the root clock muxing 
 */
ADI_PWR_RESULT adi_pwr_SetPLLClockMux(
               ADI_CLOCK_MUX_ID eClockID
               );

/*
 *  To Configure the root clock muxing 
 */
ADI_PWR_RESULT adi_pwr_SetLFClockMux(
               ADI_CLOCK_MUX_ID eClockID
               );

/* 
 * To Configure the root clock muxing 
 */
ADI_PWR_RESULT adi_pwr_SetRefClockMux(
               ADI_CLOCK_MUX_ID eClockID
               );

/*
 * Get current muxing configuration for the specified Group . 
*/
ADI_PWR_RESULT adi_pwr_GetClockMux(
               ADI_CLOCK_MUX_GROUP_ID eMuxId, 
               ADI_CLOCK_MUX_ID *peMuxID
               );

/*
 *  Get external clock frequency. 
 */
ADI_PWR_RESULT adi_pwr_GetExtClkFreq(
               uint32_t *pExtClock
               );

/* 
 * Get current clock frequency. This API can be used to know PCLK, HCLK. 
 */
ADI_PWR_RESULT adi_pwr_GetClockFrequency(
               ADI_CLOCK_ID eClockId,
               uint32_t *pClock
               );
/* 
 * To enable/disable the specific clock. 
 */
ADI_PWR_RESULT adi_pwr_EnableClock(
               ADI_CLOCK_GATE_ID eClockGateId, 
               bool_t bEnable
               );

/* 
 * To enable/disable the specific clock source. 
 */
ADI_PWR_RESULT adi_pwr_EnableClockSource(
               ADI_CLOCK_SOURCE_ID eClockSource, 
               bool_t bEnable
               );
/* 
 * To set the specific clock divider. 
*/
ADI_PWR_RESULT adi_pwr_SetClockDivider(
               ADI_CLOCK_ID eClockId,
               uint8_t nDiv
               );
/* 
 * To Get the clock status. 
*/
ADI_PWR_RESULT adi_pwr_GetClockStatus(
               ADI_CLOCK_SOURCE_ID eClockSource,
               ADI_CLOCK_SOURCE_STATUS *peStatus
               );
/* 
 * To configure the PLL to generate the SPLL
*/
ADI_PWR_RESULT adi_pwr_SetPll(
                uint8_t  nDivFactor, 
                uint8_t  nMulFactor, 
                bool_t bDiv2,
                bool_t bMul2
                );
/* To enable the interrupt for clock monitoring LFXTL/HFXTL/PLL.*/
ADI_PWR_RESULT adi_pwr_EnableClockInterrupt(
                ADI_CLOCK_IRQ eIrq, 
                bool_t bEnable
                );

/* Enabling the LFXTL bypass mode */
ADI_PWR_RESULT  adi_pwr_EnableLFXTLBypass(
                bool_t bEnable
                );

/* Enabling the LFXTL monitoring interrupt */
ADI_PWR_RESULT  adi_pwr_EnableLFXTLMonitorInterrupt(
                bool_t bEnable
                );
/* 
 * =================================================================
 *  Power Management related  APIs 
 * =================================================================
*/
/* To enable the interrupt for voltage monitoring.*/
ADI_PWR_RESULT adi_pwr_EnableInterrupt(
                ADI_PWR_IRQ eIrq, 
                bool_t bEnable
                );

/*
 * To know which is interrupt caused the processor to wake up from SHUTDOWN  mode.
 */
ADI_PWR_RESULT adi_pwr_GetWakeUpStatus(
               ADI_PWR_WAKEUP_STATUS *peStatus
               );

/*
 * To select the voltage range of the battery for monitoring.
*/
ADI_PWR_RESULT adi_pwr_SetVoltageRange(
               ADI_PWR_VOLTAGE_RANGE eRange
               );

/*
 *  For entering the low power mode.
*/
ADI_PWR_RESULT adi_pwr_EnterLowPowerMode(
                const ADI_PWR_POWER_MODE PowerMode,
                bool_t volatile *        pbInterruptOccurred,
                const uint8_t            PriorityMask
                );

/*
 *  For exiting the low power mode.
*/
ADI_PWR_RESULT adi_pwr_ExitLowPowerMode(
                bool_t volatile *  pbInterruptOccurred
                );

/* To enable the HPBUCK */
ADI_PWR_RESULT adi_pwr_EnableHPBuck(
               bool_t bEnable
               );
/* 
 * For registering the call back function .
*/
ADI_PWR_RESULT adi_pwr_RegisterCallback(
               ADI_CALLBACK pfCallback,
               void *pcbParam
              );

#ifdef __cplusplus
}
#endif

#endif /* __ADI_PWR_H__ */


/*@}*//*
** EOF
*/
