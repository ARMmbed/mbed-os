/*
 *****************************************************************************
 * @file:    adi_pwr.h
 * @brief:   System clock and power management driver.
 *-----------------------------------------------------------------------------
Copyright (c) 2016 Analog Devices, Inc.

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


/*! \addtogroup Power_Driver Power Driver
 *  @{
 */

#ifndef ADI_PWR_H
#define ADI_PWR_H

#include <adi_callback.h>
#include <adi_processor.h>

#ifdef __ICCARM__
/* IAR MISRA C 2004 error suppressions.
 *
 * Pm009 (rule 5.1): identifiers shall not rely on significance of more than 31 characters.
 *   IAR compiler supports longer identifiers.
 * Pm011 (rule 6.3): The basic types of char, int, long, float cannot be used.
 *   bool is used in the APIs as it is not affending the rule. Disabling this as IAR treats it as an error. 
 */
#pragma diag_suppress=Pm009,Pm011
#endif /* __ICCARM__ */

#ifdef __cplusplus
 extern "C" {
#endif
   
/*! Enumeration of clock sources for various peripherals. */
typedef enum {
    /*! Source for all peripherals SPI, SPORT, SIP, CRC, AES, SIP interface, I2C, UART, optionally for timers. */  
    ADI_CLOCK_PCLK,    
    /*! Source for Core,Bus etc. */    
    ADI_CLOCK_HCLK,
    /*! Source for the ADC. */
    ADI_CLOCK_ACLK
    
} ADI_CLOCK_ID;

/*! Enumeration of input clock sources */
typedef enum {
   /*! Clock ID for 16 MHz or 26 MHz external crystal oscillator called  HFXTAL. */    
    ADI_CLOCK_SOURCE_HFXTAL,
   /*! Clock ID 32 kHz external crystal oscillator called  LFXTAL. */
    ADI_CLOCK_SOURCE_LFXTAL,
   /*! Clock ID for 26 MHz internal oscillator called  HFOSC. */
    ADI_CLOCK_SOURCE_HFOSC,
   /*! Clock ID 32 kHz a 32 kHz internal oscillator called  LFXTAL. */
    ADI_CLOCK_SOURCE_LFOSC,
   /*! Clock ID for output clock for System PLL. */
    ADI_CLOCK_SOURCE_SPLL,
   /*! Clock ID for external clock from GPIO. */               
    ADI_CLOCK_SOURCE_GPIO
} ADI_CLOCK_SOURCE_ID;


/*! 
 * Enumeration of clock sources for each clock multiplexer.
 * The processor has the following clock multiplexers.
 * - SPLL Mux (System PLL).
 * - Reference clock Mux.
 * - Root Clock Mux.
 */
typedef enum  {

    /*! Input clock for  system PLL mux is HFOSC. */
    ADI_CLOCK_MUX_SPLL_HFOSC, 
    /*! Input clock for  system PLL mux is HFXTAL. */
    ADI_CLOCK_MUX_SPLL_HFXTAL,
    /*! Input clock for  system PLL mux is provided through GPIO. */
    ADI_CLOCK_MUX_SPLL_GPIO,
    
    /*! Input clock for  low frequency clock mux is LFOSC. */
    ADI_CLOCK_MUX_LFCLK_LFOSC,
    /*! Input clock for  low frequency clock mux is LFXTAL. */    
    ADI_CLOCK_MUX_LFCLK_LFXTAL,

    /*! Input clock to the multiplexer which provides reference clock for Flash 
      and HPBUCK clock is HFOSC. */
    ADI_CLOCK_MUX_REF_HFOSC_CLK,  
    /*! Reserved. */
    ADI_CLOCK_MUX_REF_RESERVED,    
    /*! Input clock to the multiplexer which provides reference clock for Flash 
      and HPBUCK clock is 26 MHz HFXTAL. */
    ADI_CLOCK_MUX_REF_HFXTAL_26MHZ_CLK,
    /*! Input clock to the multiplexer which provides reference clock for Flash 
      and HPBUCK clock is 16 MHz HFXTAL. */
    ADI_CLOCK_MUX_REF_HFXTAL_16MHZ_CLK,
    
    /*! Input clock to root multiplexer is HFOSC. */                                            
    ADI_CLOCK_MUX_ROOT_HFOSC,
    /*! Input clock to root multiplexer is HFXTAL. */                                                
    ADI_CLOCK_MUX_ROOT_HFXTAL,
    /*! Input clock to root multiplexer is SPLL. */                                                
    ADI_CLOCK_MUX_ROOT_SPLL,
    /*! Input clock to root multiplexer is from GPIO. */                                                
    ADI_CLOCK_MUX_ROOT_GPIO
    
} ADI_CLOCK_MUX_ID;


/*! 
 * Enumeration of clock source status. 
 */
typedef enum {
    /*! Specified clock source is disabled. */
    ADI_CLOCK_SOURCE_DISABLED = 0,
    /*! Specified clock source is not stable. */    
    ADI_CLOCK_SOURCE_ENABLED_NOT_STABLE,
    /*! Specified clock source is enabled and  stable. */        
    ADI_CLOCK_SOURCE_ENABLED_STABLE,
    /*! Invalid clock ID. */
    ADI_CLOCK_SOURCE_ID_NOT_VALID
    
} ADI_CLOCK_SOURCE_STATUS;

/*! Clock output options through GPIO pin.
    The GPIO clock output pin can be driven through one of these clocks.
*/
typedef enum
{
    /*! Root Clock (ROOT_CLK). */
    ADI_CLOCK_OUTPUT_ROOT_CLK,
    
    /*! Low Frequency Clock (LF_CLK). */
    ADI_CLOCK_OUTPUT_LF_CLK,
    
    /*! ADC Clock (ACLK). */
    ADI_CLOCK_OUTPUT_ACLK,
    
    /*! HCLK_BUS. */
    ADI_CLOCK_OUTPUT_HCLK_BUS,
    
    /*! HCLK_CORE. */
    ADI_CLOCK_OUTPUT_HCLK_CORE,
    
    /*! Peripheral Clock (PCLK). */
    ADI_CLOCK_OUTPUT_PCLK,
    
    /*! Reference Clock for Flash controller timer (RCLK). */
    ADI_CLOCK_OUTPUT_RCLK,
    
    /*! Mux of HFOSC, HFXTAL clock (RHP_CLK). */
    ADI_CLOCK_OUTPUT_RHP_CLK,
    
    /*! GP Timer 0 clock (GPT0_CLK). */
    ADI_CLOCK_OUTPUT_GPT0_CLK,
    
    /*! GP Timer 1 clock (GPT1_CLK). */
    ADI_CLOCK_OUTPUT_GPT1_CLK,
    
    /*! Peripherals operating at HCLK (HCLK_P). */
    ADI_CLOCK_OUTPUT_HCLK_PERIPHERAL,
    
    /*! PLL Clock out. */
    ADI_CLOCK_OUTPUT_PLL_OUTPUT,
    
    /*! RTC0 Clock. */
    ADI_CLOCK_OUTPUT_RTC0_CLK,
    
    /*! HP Buck Clock (HPBUCK_CLK). */
    ADI_CLOCK_OUTPUT_HPBUCK_CLK,
    
    /*! HP Buck Non overlap clock. */
    ADI_CLOCK_OUTPUT_HPBUCK_NO_OVERLAP_CLK,
    
    /*! RTC1 generated clock. */
    ADI_CLOCK_OUTPUT_RTC1_CLK
    
}ADI_CLOCK_OUTPUT_ID;
 

/*! Enumeration of clock gates using which the clocks can be gated. */
typedef enum {
    /*! Clock Gate for the GP Timer-0. */    
    ADI_CLOCK_GATE_GPT0_CLK     = 1 << BITP_CLKG_CLK_CTL5_GPTCLK0OFF,
    /*! Clock Gate for the GP Timer-1. */        
    ADI_CLOCK_GATE_GPT1_CLK     = 1 << BITP_CLKG_CLK_CTL5_GPTCLK1OFF,
    /*! Clock Gate for the GP Timer-2. */        
    ADI_CLOCK_GATE_GPT2_CLK     = 1 << BITP_CLKG_CLK_CTL5_GPTCLK2OFF,
    /*! Clock Gate for the I2C. */        
    ADI_CLOCK_GATE_I2C_CLK      = 1 << BITP_CLKG_CLK_CTL5_UCLKI2COFF,
    /*! Clock Gate for the GPIO. */        
    ADI_CLOCK_GATE_GPIO_CLK     = 1 << BITP_CLKG_CLK_CTL5_GPIOCLKOFF,    
    /*! Clock Gate for the PCLK. */                
    ADI_CLOCK_GATE_PCLK         = 1 << BITP_CLKG_CLK_CTL5_PERCLKOFF, 
    /*! Clock Gate for the RGB Timer. */                
    ADI_CLOCK_GATE_TMR_RGB_CLK  = 1 << BITP_CLKG_CLK_CTL5_TMRRGBCLKOFF 

} ADI_CLOCK_GATE;

/*!
 *  Enumeration of HF oscillator clock divide factor.
 */
typedef enum 
{
    /*! Divide by 1. */
    ADI_PWR_HFOSC_DIV_BY_1,
    /*! Divide by 2. */
    ADI_PWR_HFOSC_DIV_BY_2,
    /*! Divide by 4. */
    ADI_PWR_HFOSC_DIV_BY_4,
    /*! Divide by 8. */
    ADI_PWR_HFOSC_DIV_BY_8,
    /*! Divide by 16. */
    ADI_PWR_HFOSC_DIV_BY_16,
    /*! Divide by 32. */
    ADI_PWR_HFOSC_DIV_BY_32

} ADI_PWR_HFOSC_DIV;

 /*!
 *****************************************************************************
 * Power driver API return codes
 *****************************************************************************/
typedef enum
{
    /*! No error detected.                                    */  
    ADI_PWR_SUCCESS = 0,              
    /*! Generic unknown error occurred.                       */    
    ADI_PWR_FAILURE,                  
    /*! If the given pointer is pointing to NULL.             */    
    ADI_PWR_NULL_POINTER,                  
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
    ADI_PWR_INVALID_PARAM,   
    /*! System not initialized, call the API SystemInit.      */
    ADI_PWR_SYSTEM_NOT_INITIALIZED
    
} ADI_PWR_RESULT;

/*!
 *   Enumeration of the power modes supported by the processor.
 */
typedef enum
{
    /*! Core Sleep power-down mode. */    
    ADI_PWR_MODE_FLEXI          = 0 << BITP_PMG_PWRMOD_MODE,      
    /*! Fully Active. (piggy-back on bitmode value "1", normally reserved) */    
    ADI_PWR_MODE_ACTIVE         = 1 << BITP_PMG_PWRMOD_MODE,  
    /*! Full Hibernate power-down mode. */    
    ADI_PWR_MODE_HIBERNATE      = 2 << BITP_PMG_PWRMOD_MODE,        
    /*! System Sleep power-down mode. */    
    ADI_PWR_MODE_SHUTDOWN       = 3 << BITP_PMG_PWRMOD_MODE
    
} ADI_PWR_POWER_MODE;


/*!
 *   Enumeration of power management interrupts.
 */
typedef enum
{
    /*! Interrupt when battery voltage drops below 1.8V.*/    
    ADI_PWR_LOW_BATTERY_VOLTAGE_IEN     = 1 << BITP_PMG_IEN_VBAT,       
    /*! Interrupt when VREG under-voltage: below 1V. */    
    ADI_PWR_UNDER_VOLATAGE_IEN          = 1 << BITP_PMG_IEN_VREGUNDR,   
    /*! Interrupt when VREG over-voltage: over- 1.32V. */    
    ADI_PWR_OVER_VOLATAGE_IEN           = 1 << BITP_PMG_IEN_VREGOVR,    
    /*! Interrupt when battery voltage falls to the specified range.Please see #adi_pwr_SetVoltageRange.*/     
    ADI_PWR_BATTERY_VOLTAGE_RANGE_IEN   = 1 << BITP_PMG_IEN_IENBAT
    
} ADI_PWR_PMG_IRQ;


/*!
 *   Enumeration of system clock module interrupts.
 */
typedef enum
{
    /*! Interrupt for root clock monitor and Clock Fail. */
    ADI_PWR_ROOT_CLOCK_MON_IEN      = 1 << BITP_CLKG_OSC_CTL_ROOT_MON_EN,
    /*! Interrupt for LFXTAL clock monitor and Clock Fail. */
    ADI_PWR_LFXTAL_CLOCK_MON_IEN    = 1 << BITP_CLKG_OSC_CTL_LFX_MON_EN,
    /*! Interrupt when LFXTAL clock becomes stable/unstable. */
    ADI_PWR_LFXTAL_STATUS_IEN       = 1 << BITP_CLKG_CLK_CTL0_LFXTALIE,
    /*! Interrupt when HFXTAL clock becomes stable/unstable. */     
    ADI_PWR_HFXTAL_STATUS_IEN       = 1 << BITP_CLKG_CLK_CTL0_HFXTALIE,
    /*! Interrupt when PLL-LOCK/PLL-UNLOCK.  */
    ADI_PWR_PLL_STATUS_IEN          = 1 << BITP_CLKG_CLK_CTL3_SPLLIE
      
} ADI_PWR_CLOCK_IRQ;

/**
 * Enumeration of the power driver events notified through the callback.
 */
typedef enum 
{
    /*! Event for indicating Over voltage VREG  > 1.32v. */    
    ADI_PWR_EVENT_VREG_OVER_VOLTAGE,            
    /*! Event for indicating  under voltage VREG < 1V. */    
    ADI_PWR_EVENT_VREG_UNDER_VOLTAGE,           
    
    /*! Event for indicating battery voltage below  1.8V. */
    ADI_PWR_EVENT_BATTERY_VOLTAGE_LOW,
    /*! Event for indicating battery voltage in specified range-1.VBAT range1 (> 2.75v). */ 
    ADI_PWR_EVENT_BATTERY_VOLTAGE_RANGE_1,
    /*! Event for indicating battery voltage in specified range-2.VBAT range2 (2.75v - 2.3v).   */    
    ADI_PWR_EVENT_BATTERY_VOLTAGE_RANGE_2,
    /*! Event for indicating battery voltage in specified range-3.VBAT range3 (2.3v - 1.6v).   */    
    ADI_PWR_EVENT_BATTERY_VOLTAGE_RANGE_3,

    /*! Event to indicate that LFXTAL failed and hardware automatically switched to LFOSC. */
    ADI_PWR_EVENT_OSC_LFXTAL_AUTO_SWITCH,
    /*! Event to indicate the LFXTAL clock is not stable. */
    ADI_PWR_EVENT_OSC_LFXTAL_MON_FAIL,
    /*! Event to indicate the Root clock is not stable. */
    ADI_PWR_EVENT_OSC_ROOT_CLOCK_MON_FAIL,
    /*! Event to indicate the Root clock failed and hardware automatically switched to HFOSC. */
    ADI_PWR_EVENT_OSC_ROOT_CLOCK_FAIL_AUTO_SWITCH,
 
    /*! Event to indicate HF crystal stable. */    
    ADI_PWR_EVENT_OSC_HFXTAL_CLOCK_OK,
    /*! Event to indicate HF crystal is not stable. */    
    ADI_PWR_EVENT_OSC_HFXTAL_CLOCK_NO_OK,
    /*! Event to indicate LF crystal is  stable. */    
    ADI_PWR_EVENT_OSC_LFXTAL_CLOCK_OK,
    /*! Event to indicate LF crystal is not stable. */    
    ADI_PWR_EVENT_OSC_LFXTAL_CLOCK_NO_OK,
    /*! Event for indicating PLL is locked. */    

    ADI_PWR_EVENT_PLLC_LOCK,
    /*! Event for indicating PLL is unlocked.  */    
    ADI_PWR_EVENT_PLLC_UNLOCK
    
} ADI_PWR_EVENT;


/*!
 *   Enumeration of processor wake up status. 
*/
typedef enum
{
    /*! Interrupt from External Interrupt 0. */    
    ADI_PWR_INT_EXT0,    
    /*! Interrupt from External Interrupt 1. */    
    ADI_PWR_INT_EXT1,    
    /*! Interrupt from External Interrupt 2. */        
    ADI_PWR_INT_EXT2,   
    /*! Interrupt from RTC. */
    ADI_PWR_INT_RTC     
      
} ADI_PWR_WAKEUP_STATUS;

/*!
 *   Enumeration of the battery voltage ranges for voltage monitoring interrupt generation.
*/
typedef enum
{
    /*! Voltage range is in safe region.  */
    ADI_PWR_BAT_VOLTAGE_RANGE_SAFE,     
    /*! Battery voltage is in the range of 2.2 to 2.75 V. */    
    ADI_PWR_VOLTAGE_RANGE_2_2_TO_2_75,  
    /*! Battery voltage is in the range of 1.6 to 2.2 V.  */        
    ADI_PWR_VOLTAGE_RANGE_1_6_TO_2_2    
} ADI_PWR_VOLTAGE_RANGE;

/*!
 *  Enumeration of LFXTAL Robust Mode Load select. The amount of loading tolerated when 
 *  LFXTAL robust mode is selected, that is when LFXTAL robust mode is enabled.
 */
typedef enum
{
    /*! No Trim, and big resistive loads not tolerated. */
    ADI_PWR_LFXTAL_LOAD_NONE,
    /*! 20 MOHM Load mode, greater than 20 MOHM load allowed. */
    ADI_PWR_LFXTAL_LOAD_20MOHM,
    /*! 10 MOHM Load mode, greater than 10 MOHM load allowed. */
    ADI_PWR_LFXTAL_LOAD_10MOHM,
    /*! 5 MOHM load resistance allowed on both IO pins, the user can scale the current 
        down if the load is expected to be smaller than 5 MOHM. */
    ADI_PWR_LFXTAL_LOAD_5MOHM
    
}ADI_PWR_LFXTAL_LOAD;

/*!
* Enumeration of HP Buck load modes. The modes can be used to choose the loading capability 
* of the HPBUCK. The low load mode and high load mode are based on the loading in the system.
*/
typedef enum
{
    /*! HPBUCK Low load mode.  This mode can be set if the maximum system clock(HCLK) frequency 
        is 26 MHz. */   
    ADI_PWR_HPBUCK_LD_MODE_LOW,
    
    /*! HPBUCK High load mode. This mode can be set if the  system clock(HCLK) frequency is greater 
        than 26 MHz. */
    ADI_PWR_HPBUCK_LD_MODE_HIGH
    
}ADI_PWR_HPBUCK_LD_MODE;

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
               const uint32_t ExtClkFreq
               );

/* 
 * To Configure the root clock muxing 
 */
ADI_PWR_RESULT adi_pwr_SetRootClockMux(
               const ADI_CLOCK_MUX_ID eClockID
               );

/*
 * To Configure the root clock muxing 
 */
ADI_PWR_RESULT adi_pwr_SetPLLClockMux(
               const ADI_CLOCK_MUX_ID eClockID
               );

/*
 *  To Configure the root clock muxing 
 */
ADI_PWR_RESULT adi_pwr_SetLFClockMux(
               const ADI_CLOCK_MUX_ID eClockID
               );

               
/*
 * To Enable/Disable the LFXTAL robust mode.
 */
ADI_PWR_RESULT adi_pwr_EnableLFXTALRobustMode(
                const bool  bEnable
                );

/*
 * To configure the LFXTAL robust mode load.
 */
ADI_PWR_RESULT adi_pwr_SetLFXTALRobustModeLoad(
                const ADI_PWR_LFXTAL_LOAD eLoad
                );

/*
 * To Enable/Disable the LFXTAL Fail Auto switch.
 */
ADI_PWR_RESULT adi_pwr_EnableLFXTALFailAutoSwitch(
                const bool  bEnable
                );                

/*
 *  To enable/disable auto switching of root clock to HFOSC upon detection
 *  of Root clock failure.
 */ 
ADI_PWR_RESULT adi_pwr_EnableRootClockFailAutoSwitch(
               const bool bEnable
               );

/*
 *  To set the HF Oscillator divide factor
 */ 
ADI_PWR_RESULT adi_pwr_SetHFOscDivFactor(
               const ADI_PWR_HFOSC_DIV    eDivFactor
               );

/*
 *  To set the HF oscillator automatic divide by 1 during wakeup from Flexi mode
 */ 
ADI_PWR_RESULT adi_pwr_EnableHFOscAutoDivBy1(
               const bool   bEnable
               );
               
/* 
 * To Configure the reference clock muxing 
 */
ADI_PWR_RESULT adi_pwr_SetRefClockMux(
               const ADI_CLOCK_MUX_ID eClockID
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
               const ADI_CLOCK_ID eClockId,
               uint32_t *pClock
               );
/* 
 * To enable/disable the specific clock. 
 */
ADI_PWR_RESULT adi_pwr_EnableClock(
               const ADI_CLOCK_GATE eClockGate, 
               const bool bEnable
               );

/* 
 * To enable/disable the specific clock source. 
 */
ADI_PWR_RESULT adi_pwr_EnableClockSource(
               const ADI_CLOCK_SOURCE_ID eClockSource, 
               const bool bEnable
               );
/* 
 * To set the specific clock divider. 
*/
ADI_PWR_RESULT adi_pwr_SetClockDivider(
               const ADI_CLOCK_ID eClockId,
               const uint16_t nDiv
               );
/* 
 * To Get the clock status. 
*/
ADI_PWR_RESULT adi_pwr_GetClockStatus(
               const ADI_CLOCK_SOURCE_ID eClockSource,
               ADI_CLOCK_SOURCE_STATUS *peStatus
               );
/* 
 * To configure the PLL to generate the SPLL
*/
ADI_PWR_RESULT adi_pwr_SetPll(
                uint8_t  nDivFactor, 
                const uint8_t  nMulFactor, 
                const bool bDiv2,
                const bool bMul2
                );
                
/* To enable the interrupt for clock monitoring LFXTAL/HFXTAL/PLL.*/
ADI_PWR_RESULT adi_pwr_EnableClockInterrupt(
                const ADI_PWR_CLOCK_IRQ eIrq, 
                const bool bEnable
                );

/* Enabling the LFXTAL bypass mode */
ADI_PWR_RESULT  adi_pwr_EnableLFXTALBypass(
                const bool bEnable
                );


/* Set the clock output through the GPIO */
ADI_PWR_RESULT  adi_pwr_SetGPIOClockOutput(
                const ADI_CLOCK_OUTPUT_ID eClockOutput
                );
                
/* 
 * =================================================================
 *  Power Management related  APIs 
 * =================================================================
*/
/* To enable the interrupt for voltage monitoring.*/
ADI_PWR_RESULT adi_pwr_EnablePMGInterrupt(
                const ADI_PWR_PMG_IRQ eIrq, 
                const bool bEnable
                );

/*
 * To know which is interrupt caused the processor to wake up from SHUTDOWN mode.
 */
ADI_PWR_RESULT adi_pwr_GetWakeUpStatus(
               ADI_PWR_WAKEUP_STATUS *peStatus
               );

/*
 * To select the voltage range of the battery for monitoring.
*/
ADI_PWR_RESULT adi_pwr_SetVoltageRange(
               const ADI_PWR_VOLTAGE_RANGE eRange
               );

/*
 *  For entering the low power mode.
*/
ADI_PWR_RESULT adi_pwr_EnterLowPowerMode(
                const ADI_PWR_POWER_MODE PowerMode,
                uint32_t volatile *        pnInterruptOccurred,
                const uint8_t            PriorityMask
                );

/*
 *  For exiting the low power mode.
*/
ADI_PWR_RESULT adi_pwr_ExitLowPowerMode(
                uint32_t volatile *  pnInterruptOccurred
                );

/* To enable the HPBUCK */
ADI_PWR_RESULT adi_pwr_EnableHPBuck(
               const bool bEnable
               );
               

/* To enable the HPBUCK Low Power mode */
ADI_PWR_RESULT adi_pwr_EnableHPBuckLowPowerMode(
               const bool bEnable
               );
               
/* To enable the HPBUCK Load mode */
ADI_PWR_RESULT adi_pwr_SetHPBuckLoadMode(
               const ADI_PWR_HPBUCK_LD_MODE   eLoadMode
               );
               
/* 
 * For registering the call back function .
*/
ADI_PWR_RESULT adi_pwr_RegisterCallback(
               const ADI_CALLBACK pfCallback,
               void *pcbParam
              );

#ifdef __cplusplus
}
#endif

#endif /* ADI_PWR_H */


/*@}*/

/*
** EOF
*/
