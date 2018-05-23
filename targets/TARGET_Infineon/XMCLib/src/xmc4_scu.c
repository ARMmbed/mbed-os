/**
 * @file xmc4_scu.c
 * @date 2016-06-15
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
 *     - Initial <br>
 *      
 * 2015-05-20:
 *     - XMC_ASSERT() hanging issues have fixed.  <br>
 *     - Line indentation aligned with 120 characters. <br>
 *     
 * 2015-06-20:
 *     - XMC_SCU_INTERRUPT_EnableEvent,XMC_SCU_INTERRUPT_DisableEvent,
 *     - XMC_SCU_INTERRUPT_TriggerEvent,XMC_SCU_INTERUPT_GetEventStatus,
 *     - XMC_SCU_INTERRUPT_ClearEventStatus are added
 *     - Added Weak implementation for OSCHP_GetFrequency()
 *
 * 2015-11-30:
 *     - Documentation improved <br>
 *     - Following API functionalities are improved
 *       XMC_SCU_CLOCK_GatePeripheralClock, XMC_SCU_CLOCK_UngatePeripheralClock, XMC_SCU_CLOCK_IsPeripheralClockGated
 *       XMC_SCU_RESET_AssertPeripheralReset, XMC_SCU_RESET_DeassertPeripheralReset, XMC_SCU_RESET_IsPeripheralResetAsserted
 *
 * 2015-12-08:
 *     - XMC_SCU_GetTemperature renamed to XMC_SCU_GetTemperatureMeasurement
 *
 * 2016-03-09:
 *     - Optimize write only registers
 *     - Added XMC_SCU_HIB_SetPinMode
 *     - Added XMC_SCU_HIB_GetHibernateControlStatus,
 *             XMC_SCU_HIB_GetEventStatus, XMC_SCU_HIB_ClearEventStatus, XMC_SCU_HIB_TriggerEvent, 
 *             XMC_SCU_HIB_EnableEvent, XMC_SCU_HIB_DisableEvent
 *     - Added XMC_SCU_HIB_SetWakeupTriggerInput, XMC_SCU_HIB_SetPinMode, XMC_SCU_HIB_SetOutputPinLevel, 
 *             XMC_SCU_HIB_SetInput0, XMC_SCU_HIB_EnterHibernateState
 *
 * 2016-04-06:
 *     - Fixed XMC_SCU_ReadFromRetentionMemory functionality
 *
 * 2016-05-19:
 *     - Changed XMC_SCU_CLOCK_StartSystemPll to avoid using floating point calculation which might have an impact on interrupt latency if ISR uses also the FPU
 *     - Added XMC_SCU_CLOCK_IsLowPowerOscillatorStable() and XMC_SCU_CLOCK_IsHighPerformanceOscillatorStable()
 *     - Added XMC_SCU_CLOCK_EnableLowPowerOscillatorGeneralPurposeInput(), 
 *             XMC_SCU_CLOCK_DisableLowPowerOscillatorGeneralPurposeInput(),
 *             XMC_SCU_CLOCK_GetLowPowerOscillatorGeneralPurposeInputStatus()
 *     - Added XMC_SCU_CLOCK_EnableHighPerformanceOscillatorGeneralPurposeInput(), 
 *             XMC_SCU_CLOCK_DisableHighPerformanceOscillatorGeneralPurposeInput(),
 *             XMC_SCU_CLOCK_GetHighPerformanceOscillatorGeneralPurposeInputStatus()
 *
 * 2016-06-15:
 *     - Added XMC_SCU_HIB_EnterHibernateStateEx() which allows to select between external or internal hibernate mode. This last mode only available in XMC44, XMC42 and XMC41 series.
 *     - Extended wakeup hibernate events using LPAC wakeup on events. Only available in XMC44, XMC42 and XMC41 series
 *     - Added LPAC APIs. Only available in XMC44, XMC42 and XMC41 series.
 *
 * @endcond 
 *
 */

/**
 *
 * @brief SCU low level driver API prototype definition for XMC4 family of microcontrollers. 
 *
 * <b>Detailed description of file:</b> <br>
 * APIs provided in this file cover the following functional blocks of SCU: <br>
 * -- GCU (APIs prefixed with XMC_SCU_GEN_) <br>
 * ------ Temperature Monitoring, Bootmode selection, CCU Start, Comparator configuration etc  <br>
 * -- CCU (APIs prefixed with XMC_SCU_CLOCK_) <br>
 * ------ Clock sources init, Clock tree init, Clock gating, Sleep Management etc <br>
 * -- RCU (APIs prefixed with XMC_SCU_RESET_) <br>
 * ------ Reset Init, Cause, Manual Reset Assert/Deassert <br>
 * -- INTERRUPT (APIs prefixed with XMC_SCU_INTERRUPT_) <br>
 * ------ Init, Manual Assert/Deassert, Acknowledge etc <br>
 * -- PARITY (APIs prefixed with XMC_SCU_PARITY_) <br>
 * ------ Init, Acknowledge etc <br>
 * -- HIBERNATION (APIs prefixed with XMC_SCU_HIB_) <br>
 * ------ Hibernation entry/exit config, entry/wakeup sequences, LPAC configuration etc <br>
 * -- TRAP (APIs prefixed with XMC_SCU_TRAP_) <br>
 * ------ Init, Enable/Disable, Acknowledge etc <br>
 *
 */

/*********************************************************************************************************************
 * HEADER FILES
 ********************************************************************************************************************/
#include <xmc_scu.h>

#if UC_FAMILY == XMC4

/*********************************************************************************************************************
 * MACROS
 ********************************************************************************************************************/
#define FOSCREF   (2500000UL)    /**< Oscillator reference frequency (fOSCREF) monitored by Oscillator watchdog  */
#define FREQ_1MHZ (1000000UL)    /**< Used during calculation. */

#ifndef OFI_FREQUENCY
#define OFI_FREQUENCY (24000000UL)    /**< Fast internal backup clock source. */
#endif

#ifndef OSI_FREQUENCY
#define OSI_FREQUENCY (32768UL)    /**< Internal slow clock source. */
#endif

#ifndef OSCHP_FREQUENCY
#define OSCHP_FREQUENCY (12000000U)    /**< External crystal High Precision Oscillator. */
#endif

#define XMC_SCU_PLL_PLLSTAT_OSC_USABLE  (SCU_PLL_PLLSTAT_PLLHV_Msk | \
                                         SCU_PLL_PLLSTAT_PLLLV_Msk | \
                                         SCU_PLL_PLLSTAT_PLLSP_Msk)  /**< Used to verify the OSC frequency is
                                                                          usable or not.*/

#define XMC_SCU_ORC_ADC_START_GROUP    (0UL)    /**< The ADC group whose channel input is compared by Out of Range  
                                                     Comparator (ORC) to serves the purpose of overvoltage monitoring
                                                     for analog input pins of the chip and ORC start measurements from 
                                                     this group number. */
#define XMC_SCU_ORC_ADC_END_GROUP      (1UL)    /**< The ADC group whose channel input is compared by Out of Range  
                                                     Comparator (ORC) to serves the purpose of overvoltage monitoring
                                                     for analog input pins of the chip and ORC end measurements at 
                                                     this group number. */
#define XMC_SCU_ORC_START_ADC_CHANNEL  (6UL)    /**< The ADC channel whose channel input is compared by Out of Range  
                                                     Comparator (ORC) to serves the purpose of overvoltage monitoring
                                                     for analog input pins of the chip and ORC start measurements from 
                                                     this channel number. */
#define XMC_SCU_ORC_END_ADC_CHANNEL    (7UL)    /**< The ADC channel whose channel input is compared by Out of Range  
                                                     Comparator (ORC) to serves the purpose of overvoltage monitoring
                                                     for analog input pins of the chip and ORC ends measurements at 
                                                     this channel number. */

#define XMC_SCU_CHECK_GRPNUM(GROUP_NUM)   (((GROUP_NUM) == XMC_SCU_ORC_ADC_START_GROUP) || \
                                        ((GROUP_NUM) == XMC_SCU_ORC_ADC_END_GROUP) ) /**< Used to verify whether
                                                                                      provided ADC group number lies 
                                                                                      within specified ADC start and 
                                                                                      end group number or not. */
                                        
#define XMC_SCU_CHECK_CHNUM(CH_NUM)       (((CH_NUM) == XMC_SCU_ORC_START_ADC_CHANNEL) || \
                                        ((CH_NUM) == XMC_SCU_ORC_END_ADC_CHANNEL) )  /**< Used to verify whether
                                                                                      provided ADC channel number lies 
                                                                                      within specified ADC start and 
                                                                                      end channel number or not. */

#define XMC_SCU_INTERRUPT_EVENT_MAX            (32U)      /**< Maximum supported SCU events. */

#define SCU_HIBERNATE_HDCR_HIBIOSEL_Size (4U)

#define SCU_HIBERNATE_OSCULCTRL_MODE_OSC_POWER_DOWN (0x2U)

#define XMC_SCU_POWER_LSB13V (0.0058F)
#define XMC_SCU_POWER_LSB33V (0.0225F)

/*********************************************************************************************************************
 * LOCAL DATA
 ********************************************************************************************************************/
XMC_SCU_INTERRUPT_EVENT_HANDLER_t event_handler_list[XMC_SCU_INTERRUPT_EVENT_MAX]; /**< For registering callback
                                                                                        functions on SCU event
                                                                                        occurrence. */

/*********************************************************************************************************************
 * LOCAL ROUTINES
 ********************************************************************************************************************/
 #if defined(UC_ID)
/* This is a non-weak function, which retrieves high precision external oscillator frequency. */
__WEAK uint32_t OSCHP_GetFrequency(void)
{
  return (OSCHP_FREQUENCY);
}
#endif

/* This is a local function used to generate the delay until register get updated with new configured value.  */
static void XMC_SCU_lDelay(uint32_t cycles);

/*********************************************************************************************************************
 * API IMPLEMENTATION
 ********************************************************************************************************************/
/* This is a local function used to generate the delay until register get updated with new configured value.  */
void XMC_SCU_lDelay(uint32_t delay)
{
  uint32_t i;

  SystemCoreClockUpdate();
  delay =  delay * (uint32_t)(SystemCoreClock / FREQ_1MHZ);

  for (i = 0U; i < delay; ++i)
  {
    __NOP();
  }
}

/* API to enable the SCU event */
void XMC_SCU_INTERRUPT_EnableEvent(const XMC_SCU_INTERRUPT_EVENT_t event)
{
  SCU_INTERRUPT->SRMSK |= (uint32_t)event;
}

/* API to disable the SCU event */
void XMC_SCU_INTERRUPT_DisableEvent(const XMC_SCU_INTERRUPT_EVENT_t event)
{
  SCU_INTERRUPT->SRMSK &= (uint32_t)~event;
}

/* API to trigger the SCU event */
void XMC_SCU_INTERRUPT_TriggerEvent(const XMC_SCU_INTERRUPT_EVENT_t event)
{
  SCU_INTERRUPT->SRSET |= (uint32_t)event;
}

/* API to retrieve the SCU event status */
XMC_SCU_INTERRUPT_EVENT_t XMC_SCU_INTERUPT_GetEventStatus(void)
{
  return (SCU_INTERRUPT->SRRAW);
}

/* API to clear the SCU event status */
void XMC_SCU_INTERRUPT_ClearEventStatus(const XMC_SCU_INTERRUPT_EVENT_t event)
{
  SCU_INTERRUPT->SRCLR = (uint32_t)event;
}


/* API to retrieve the currently deployed device bootmode */
uint32_t XMC_SCU_GetBootMode(void)
{
  return (uint32_t)(SCU_GENERAL->STCON & SCU_GENERAL_STCON_SWCON_Msk);
}

/* API to program a new device bootmode */
void XMC_SCU_SetBootMode(const XMC_SCU_BOOTMODE_t bootmode)
{
  SCU_GENERAL->STCON = (uint32_t)bootmode;
}

/* API to read from General purpose register */
uint32_t XMC_SCU_ReadGPR(const uint32_t index)
{
  return (SCU_GENERAL->GPR[index]);
}

/* API to write to GPR */
void XMC_SCU_WriteGPR(const uint32_t index, const uint32_t data)
{
  SCU_GENERAL->GPR[index] = data;
}

/* API to enable Out of Range Comparator(ORC) for a desired group and a desired channel input */
void XMC_SCU_EnableOutOfRangeComparator(const uint32_t group, const uint32_t channel)
{
  XMC_ASSERT("XMC_SCU_EnableOutOfangeComparator:Wrong Group Number",XMC_SCU_CHECK_GRPNUM(group));
  XMC_ASSERT("XMC_SCU_EnableOutOfangeComparator:Wrong Channel Number",XMC_SCU_CHECK_CHNUM(channel));
   
  SCU_GENERAL->GORCEN[group] |= (uint32_t)(1UL << channel);
}

/* API to enable Out of Range Comparator(ORC) for a desired group and a desired channel input */
void XMC_SCU_DisableOutOfRangeComparator(const uint32_t group, const uint32_t channel)
{
  XMC_ASSERT("XMC_SCU_DisableOutOfRangeComparator:Wrong Group Number",XMC_SCU_CHECK_GRPNUM(group));
  XMC_ASSERT("XMC_SCU_DisableOutOfRangeComparator:Wrong Channel Number",XMC_SCU_CHECK_CHNUM(channel));
   
  SCU_GENERAL->GORCEN[group] &= (uint32_t)~(1UL << channel);
}

/* API to calibrate temperature sensor */
void XMC_SCU_CalibrateTemperatureSensor(uint32_t offset, uint32_t gain)
{
  SCU_GENERAL->DTSCON = ((uint32_t)(offset << SCU_GENERAL_DTSCON_OFFSET_Pos) |
                        (uint32_t)(gain << SCU_GENERAL_DTSCON_GAIN_Pos) |
                        (uint32_t)(0x4UL << SCU_GENERAL_DTSCON_REFTRIM_Pos) |
                        (uint32_t)(0x8UL << SCU_GENERAL_DTSCON_BGTRIM_Pos));
}
/* API to enable die temperature measurement by powering the DTS module. */
void XMC_SCU_EnableTemperatureSensor(void)
{
  SCU_GENERAL->DTSCON &= (uint32_t)~(SCU_GENERAL_DTSCON_PWD_Msk);
}

/* API to disable die temperature measurement by powering the DTS module off. */
void XMC_SCU_DisableTemperatureSensor(void)
{
  SCU_GENERAL->DTSCON |= (uint32_t)SCU_GENERAL_DTSCON_PWD_Msk;
}

/* API to provide the die temperature sensor power status. */
bool XMC_SCU_IsTemperatureSensorEnabled(void)
{
  return ((SCU_GENERAL->DTSCON & SCU_GENERAL_DTSCON_PWD_Msk) == 0U);
}

/* API to check if the die temperature sensor is ready to start a measurement. */
bool XMC_SCU_IsTemperatureSensorReady(void)
{
  return ((SCU_GENERAL->DTSSTAT & SCU_GENERAL_DTSSTAT_RDY_Msk) != 0U);
}
/* API to start device temperature measurements */
XMC_SCU_STATUS_t XMC_SCU_StartTemperatureMeasurement(void)
{
  XMC_SCU_STATUS_t status = XMC_SCU_STATUS_OK;

  if (XMC_SCU_IsTemperatureSensorEnabled() == false)
  {
    status = XMC_SCU_STATUS_ERROR;
  }
   
  if (XMC_SCU_IsTemperatureSensorBusy() == true)
  {
    status = XMC_SCU_STATUS_BUSY;
  }

  /* And start the measurement */
  SCU_GENERAL->DTSCON |= (uint32_t)SCU_GENERAL_DTSCON_START_Msk;
  
  return (status);
}

/* API to retrieve the temperature measured */
uint32_t XMC_SCU_GetTemperatureMeasurement(void)
{
  uint32_t temperature;

  if (XMC_SCU_IsTemperatureSensorEnabled() == false)
  {
    temperature = 0x7FFFFFFFUL;
  }
  else
  {
    temperature = (uint32_t)((SCU_GENERAL->DTSSTAT & SCU_GENERAL_DTSSTAT_RESULT_Msk) >> SCU_GENERAL_DTSSTAT_RESULT_Pos);
  }
  
  return ((uint32_t)temperature);
}

/* API to know whether Die temperature sensor is busy */
bool XMC_SCU_IsTemperatureSensorBusy(void)
{
  return ((SCU_GENERAL->DTSSTAT & SCU_GENERAL_DTSSTAT_BUSY_Msk) != 0U);
}


#if defined(SCU_GENERAL_DTEMPLIM_LOWER_Msk) && defined(SCU_GENERAL_DTEMPLIM_UPPER_Msk)
/* API to determine if device temperature has gone past the ceiling */
bool XMC_SCU_HighTemperature(void)
{
  bool ret_val;
  uint32_t   dtscon;
  uint32_t   dtempalarm;
  dtscon = SCU_GENERAL->DTSCON;
  dtscon = dtscon & SCU_GENERAL_DTSCON_PWD_Msk;

  ret_val = false;

  /* Any audit makes sense only if the DTS were powered up */  
  if(dtscon)
  {
    /* Powered down - return false */
    ret_val = false;
  }
  else
  {
    /* Powered up - Read the overflow bit and decide accordingly*/
    dtempalarm = SCU_GENERAL->DTEMPALARM;
    dtempalarm = dtempalarm & SCU_GENERAL_DTEMPALARM_OVERFL_Msk;
    
    if(dtempalarm)
    {
      ret_val = true;
    }
    else
    {
      ret_val = false;
    }
  }
  return (ret_val);
}

/* API to program raw values of temperature limits into the DTS */
void XMC_SCU_SetRawTempLimits(const uint32_t lower_temp, const uint32_t upper_temp)
{
  /* Power up the DTS module */
  SCU_GENERAL->DTSCON &= (uint32_t)~SCU_GENERAL_DTSCON_PWD_Msk;
  SCU_GENERAL->DTEMPLIM = 0;
  SCU_GENERAL->DTEMPLIM = (lower_temp & SCU_GENERAL_DTEMPLIM_LOWER_Msk);
  SCU_GENERAL->DTEMPLIM |= (uint32_t)((upper_temp & SCU_GENERAL_DTEMPLIM_LOWER_Msk) << SCU_GENERAL_DTEMPLIM_UPPER_Pos);
}

/* API to determine if device temperature has gone below the stipulated limit */
bool XMC_SCU_LowTemperature(void)
{
  bool ret_val;
  uint32_t   dtscon;
  uint32_t   dtempalarm;
  dtscon = SCU_GENERAL->DTSCON;
  dtscon = dtscon & SCU_GENERAL_DTSCON_PWD_Msk;

  ret_val = false;

  /* Any audit makes sense only if the DTS were powered up */  
  if(dtscon)
  {
    /* Powered down - return false */
    ret_val = false;
  }
  else
  {
    /* Powered up - Read the overflow bit and decide accordingly*/
    dtempalarm = SCU_GENERAL->DTEMPALARM;
    dtempalarm = dtempalarm & SCU_GENERAL_DTEMPALARM_UNDERFL_Msk;
    
    if(dtempalarm)
    {
      ret_val = true;
    }
    else
    {
      ret_val = false;
    }
  }
  
  return (ret_val);
}
#endif  

/* API to write into Retention memory in hibernate domain */
void XMC_SCU_WriteToRetentionMemory(uint32_t address, uint32_t data)
{
  uint32_t rmacr;
  
  /* Get the address right */  
  rmacr = (uint32_t)((address << SCU_GENERAL_RMACR_ADDR_Pos) & (uint32_t)SCU_GENERAL_RMACR_ADDR_Msk);
  
  /* Transfer from RMDATA to Retention memory */
  rmacr |= (uint32_t)(SCU_GENERAL_RMACR_RDWR_Msk);
  
  /* Write desired data into RMDATA register */  
  SCU_GENERAL->RMDATA = data;
  
  /* Write address & direction of transfer into RMACR register */  
  SCU_GENERAL->RMACR = rmacr;
  
  /* Wait until the update of RMX register in hibernate domain is completed */
  while((SCU_GENERAL->MIRRSTS) & SCU_GENERAL_MIRRSTS_RMX_Msk)
  {
  }
}

/* API to read from Retention memory in hibernate domain */
uint32_t XMC_SCU_ReadFromRetentionMemory(uint32_t address)
{
  uint32_t rmacr;

  /* Get the address right */  
  rmacr = ((uint32_t)(address << SCU_GENERAL_RMACR_ADDR_Pos) & (uint32_t)SCU_GENERAL_RMACR_ADDR_Msk);
  
  /* Transfer from RMDATA to Retention memory */
  rmacr &= ~((uint32_t)(SCU_GENERAL_RMACR_RDWR_Msk));
  
  /* Writing an adress & direction of transfer into RMACR register */  
  SCU_GENERAL->RMACR = rmacr;
  
  /* Wait until the update of RMX register in hibernate domain is completed */
  while((SCU_GENERAL->MIRRSTS) & SCU_GENERAL_MIRRSTS_RMX_Msk)
  {
  }

  return (SCU_GENERAL->RMDATA);
}

/* API to initialize the clock tree */
void XMC_SCU_CLOCK_Init(const XMC_SCU_CLOCK_CONFIG_t *const config)
{
  XMC_ASSERT("", config->fsys_clkdiv != 0);
  XMC_ASSERT("", config->fcpu_clkdiv != 0);
  XMC_ASSERT("", config->fccu_clkdiv != 0);
  XMC_ASSERT("", config->fperipheral_clkdiv != 0);
  XMC_ASSERT("", ((config->syspll_config.p_div != 0) &&
                  (config->syspll_config.mode == XMC_SCU_CLOCK_SYSPLL_MODE_NORMAL)) ||
                  (config->syspll_config.mode == XMC_SCU_CLOCK_SYSPLL_MODE_PRESCALAR));
  XMC_ASSERT("", ((config->syspll_config.n_div != 0) &&
                  (config->syspll_config.mode == XMC_SCU_CLOCK_SYSPLL_MODE_NORMAL)) ||
                  (config->syspll_config.mode == XMC_SCU_CLOCK_SYSPLL_MODE_PRESCALAR));
  XMC_ASSERT("", (config->syspll_config.k_div != 0) &&
                 ((config->syspll_config.mode == XMC_SCU_CLOCK_SYSPLL_MODE_NORMAL) ||
                  (config->syspll_config.mode == XMC_SCU_CLOCK_SYSPLL_MODE_PRESCALAR)));
  XMC_ASSERT("", ((config->fsys_clksrc == XMC_SCU_CLOCK_SYSCLKSRC_PLL) ||
                 (config->fsys_clksrc == XMC_SCU_CLOCK_SYSCLKSRC_OFI)) &&
                 ((config->syspll_config.mode == XMC_SCU_CLOCK_SYSPLL_MODE_NORMAL) ||
                  (config->syspll_config.mode == XMC_SCU_CLOCK_SYSPLL_MODE_PRESCALAR)));
  XMC_ASSERT("", ((config->fstdby_clksrc == XMC_SCU_HIB_STDBYCLKSRC_OSCULP) && (config->enable_osculp == true)) ||
                 (config->fstdby_clksrc != XMC_SCU_HIB_STDBYCLKSRC_OSCULP));
  XMC_ASSERT("", ((config->syspll_config.clksrc == XMC_SCU_CLOCK_SYSPLLCLKSRC_OSCHP) &&
                 (config->enable_oschp == true)) || (config->syspll_config.clksrc != XMC_SCU_CLOCK_SYSPLLCLKSRC_OSCHP));

  XMC_SCU_CLOCK_SetSystemClockSource(XMC_SCU_CLOCK_SYSCLKSRC_OFI);

  XMC_SCU_HIB_EnableHibernateDomain();

  if (config->enable_osculp == true)
  {
    XMC_SCU_CLOCK_EnableLowPowerOscillator();
    while (XMC_SCU_CLOCK_IsLowPowerOscillatorStable() == false);
  }
  
  XMC_SCU_HIB_SetStandbyClockSource(config->fstdby_clksrc);  
  while (XMC_SCU_GetMirrorStatus() != 0)
  {
    /* Wait until update of the stanby clock source is done in the HIB domain */    
  }

  XMC_SCU_CLOCK_SetBackupClockCalibrationMode(config->calibration_mode);

  XMC_SCU_CLOCK_SetSystemClockDivider((uint32_t)config->fsys_clkdiv);
  XMC_SCU_CLOCK_SetCpuClockDivider((uint32_t)config->fcpu_clkdiv);
  XMC_SCU_CLOCK_SetCcuClockDivider((uint32_t)config->fccu_clkdiv);
  XMC_SCU_CLOCK_SetPeripheralClockDivider((uint32_t)config->fperipheral_clkdiv);

  if (config->enable_oschp == true)
  {
    XMC_SCU_CLOCK_EnableHighPerformanceOscillator();
    while(XMC_SCU_CLOCK_IsHighPerformanceOscillatorStable() == false);
  }

  if (config->syspll_config.mode == XMC_SCU_CLOCK_SYSPLL_MODE_DISABLED)
  {
    XMC_SCU_CLOCK_DisableSystemPll();
  }
  else
  {

    XMC_SCU_CLOCK_EnableSystemPll();
    XMC_SCU_CLOCK_StartSystemPll(config->syspll_config.clksrc,
                                 config->syspll_config.mode,
                                 (uint32_t)config->syspll_config.p_div,
                                 (uint32_t)config->syspll_config.n_div,
                                 (uint32_t)config->syspll_config.k_div);
  }

  /* use SYSPLL? */
  if (config->fsys_clksrc == XMC_SCU_CLOCK_SYSCLKSRC_PLL)
  {
    XMC_SCU_CLOCK_SetSystemClockSource(XMC_SCU_CLOCK_SYSCLKSRC_PLL);    
  }
  SystemCoreClockUpdate();
}

/* API to enable a trap source */
void XMC_SCU_TRAP_Enable(const uint32_t trap)
{
  SCU_TRAP->TRAPDIS &= (uint32_t)~trap;
}

/* API to disable a trap source */
void XMC_SCU_TRAP_Disable(const uint32_t trap)
{
  SCU_TRAP->TRAPDIS |= (uint32_t)trap;
}

/* API to determine if a trap source has generated event */
uint32_t XMC_SCU_TRAP_GetStatus(void)
{
  return (SCU_TRAP->TRAPRAW);
}

/* API to manually trigger a trap event */
void XMC_SCU_TRAP_Trigger(const uint32_t trap)
{
  SCU_TRAP->TRAPSET = (uint32_t)trap;
}

/* API to clear a trap event */
void XMC_SCU_TRAP_ClearStatus(const uint32_t trap)
{
  SCU_TRAP->TRAPCLR = (uint32_t)trap;
}

/* API to clear parity error event */
void XMC_SCU_PARITY_ClearStatus(const uint32_t memory)
{
  SCU_PARITY->PEFLAG |= (uint32_t)memory; 
}

/* API to determine if the specified parity error has occured or not */
uint32_t XMC_SCU_PARITY_GetStatus(void)
{
  return (SCU_PARITY->PEFLAG);
} 

/* API to enable parity error checking for the selected on-chip RAM type */
void XMC_SCU_PARITY_Enable(const uint32_t memory)
{
  SCU_PARITY->PEEN |= (uint32_t)memory; 
}

/* API to disable parity error checking for the selected on-chip RAM type */
void XMC_SCU_PARITY_Disable(const uint32_t memory)
{
  SCU_PARITY->PEEN &= (uint32_t)~memory; 
}

/* API to enable trap assertion for the parity error source */
void XMC_SCU_PARITY_EnableTrapGeneration(const uint32_t memory)
{
  SCU_PARITY->PETE |= (uint32_t)memory; 
}

/* API to disable the assertion of trap for the parity error source */
void XMC_SCU_PARITY_DisableTrapGeneration(const uint32_t memory)
{
  SCU_PARITY->PETE &= (uint32_t)~memory; 
}

/* Enables a NMI source */
void XMC_SCU_INTERRUPT_EnableNmiRequest(const uint32_t request)
{
  SCU_INTERRUPT->NMIREQEN |= (uint32_t)request;
}

/* Disables a NMI source */
void XMC_SCU_INTERRUPT_DisableNmiRequest(const uint32_t request)
{
  SCU_INTERRUPT->NMIREQEN &= (uint32_t)~request;
}

/* API to manually assert a reset request */
void XMC_SCU_RESET_AssertPeripheralReset(const XMC_SCU_PERIPHERAL_RESET_t peripheral)
{
  uint32_t index = (uint32_t)((((uint32_t)peripheral) & 0xf0000000UL) >> 28UL);
  uint32_t mask = (((uint32_t)peripheral) & ((uint32_t)~0xf0000000UL));

  *(uint32_t *)(&(SCU_RESET->PRSET0) + (index * 3U)) = (uint32_t)mask;
}

/* API to manually de-assert a reset request */
void XMC_SCU_RESET_DeassertPeripheralReset(const XMC_SCU_PERIPHERAL_RESET_t peripheral)
{
  uint32_t index = (uint32_t)((((uint32_t)peripheral) & 0xf0000000UL) >> 28UL);
  uint32_t mask = (((uint32_t)peripheral) & ((uint32_t)~0xf0000000UL));

  *(uint32_t *)(&(SCU_RESET->PRCLR0) + (index * 3U)) = (uint32_t)mask;
}

/* Find out if the peripheral reset is asserted */
bool XMC_SCU_RESET_IsPeripheralResetAsserted(const XMC_SCU_PERIPHERAL_RESET_t peripheral)
{
  uint32_t index = (uint32_t)((((uint32_t)peripheral) & 0xf0000000UL) >> 28UL);
  uint32_t mask = (((uint32_t)peripheral) & ((uint32_t)~0xf0000000UL));

  return ((*(uint32_t *)(&(SCU_RESET->PRSTAT0) + (index * 3U)) & mask) != 0U);
}

/*
 * API to retrieve frequency of System PLL output clock
 */
uint32_t XMC_SCU_CLOCK_GetSystemPllClockFrequency(void)
{
  uint32_t clock_frequency;
  uint32_t p_div;
  uint32_t n_div;
  uint32_t k2_div;

  clock_frequency = XMC_SCU_CLOCK_GetSystemPllClockSourceFrequency();
  if(SCU_PLL->PLLSTAT & SCU_PLL_PLLSTAT_VCOBYST_Msk)
  {
    /* Prescalar mode - fOSC is the parent*/
    clock_frequency = (uint32_t)(clock_frequency / 
                      ((((SCU_PLL->PLLCON1) & SCU_PLL_PLLCON1_K1DIV_Msk) >> SCU_PLL_PLLCON1_K1DIV_Pos) + 1UL));
  }
  else
  {
    p_div  = (uint32_t)((((SCU_PLL->PLLCON1) & SCU_PLL_PLLCON1_PDIV_Msk) >> SCU_PLL_PLLCON1_PDIV_Pos) + 1UL);
    n_div  = (uint32_t)((((SCU_PLL->PLLCON1) & SCU_PLL_PLLCON1_NDIV_Msk) >> SCU_PLL_PLLCON1_NDIV_Pos) + 1UL);
    k2_div = (uint32_t)((((SCU_PLL->PLLCON1) & SCU_PLL_PLLCON1_K2DIV_Msk) >> SCU_PLL_PLLCON1_K2DIV_Pos) + 1UL);
       
    clock_frequency = (clock_frequency * n_div) / (p_div * k2_div);
  }

  return (clock_frequency);
}

/**
 * API to retrieve frequency of System PLL VCO input clock
 */
uint32_t XMC_SCU_CLOCK_GetSystemPllClockSourceFrequency(void)
{
  uint32_t clock_frequency;

  /* Prescalar mode - fOSC is the parent*/
  if((SCU_PLL->PLLCON2 & SCU_PLL_PLLCON2_PINSEL_Msk) == (uint32_t)XMC_SCU_CLOCK_SYSPLLCLKSRC_OSCHP)
  {
    clock_frequency = OSCHP_GetFrequency();
  }
  else
  {
    clock_frequency = OFI_FREQUENCY;
  }
  
  return (clock_frequency);
}

/*
 * API to retrieve frequency of USB PLL output clock
 */
uint32_t XMC_SCU_CLOCK_GetUsbPllClockFrequency(void)
{
  uint32_t clock_frequency;
  uint32_t n_div;
  uint32_t p_div;
  
  clock_frequency = OSCHP_GetFrequency();
  if((SCU_PLL->USBPLLSTAT & SCU_PLL_USBPLLSTAT_VCOBYST_Msk) == 0U)
  {
    /* Normal mode - fVCO is the parent*/
    n_div = (uint32_t)((((SCU_PLL->USBPLLCON) & SCU_PLL_USBPLLCON_NDIV_Msk) >> SCU_PLL_USBPLLCON_NDIV_Pos) + 1UL);
    p_div = (uint32_t)((((SCU_PLL->USBPLLCON) & SCU_PLL_USBPLLCON_PDIV_Msk) >> SCU_PLL_USBPLLCON_PDIV_Pos) + 1UL);
    clock_frequency = (uint32_t)((clock_frequency * n_div)/ (uint32_t)(p_div * 2UL));
  }
  return (clock_frequency);
}

/*
 * API to retrieve frequency of CCU clock frequency
 */
uint32_t XMC_SCU_CLOCK_GetCcuClockFrequency(void)
{
  uint32_t frequency = 0UL;
  frequency = XMC_SCU_CLOCK_GetSystemClockFrequency();
  
  return (uint32_t)(frequency >> ((uint32_t)((SCU_CLK->CCUCLKCR & SCU_CLK_CCUCLKCR_CCUDIV_Msk) >>
                                              SCU_CLK_CCUCLKCR_CCUDIV_Pos)));
}

/*
 * API to retrieve USB and SDMMC clock frequency
 */
uint32_t XMC_SCU_CLOCK_GetUsbClockFrequency(void)
{
  uint32_t frequency = 0UL;
  XMC_SCU_CLOCK_USBCLKSRC_t clksrc;

  clksrc = XMC_SCU_CLOCK_GetUsbClockSource();

  if (clksrc == XMC_SCU_CLOCK_USBCLKSRC_SYSPLL)
  {
    frequency = XMC_SCU_CLOCK_GetSystemPllClockFrequency();
  }
  else if (clksrc == XMC_SCU_CLOCK_USBCLKSRC_USBPLL)
  {
    frequency = XMC_SCU_CLOCK_GetUsbPllClockFrequency();
  }
  else
  {
  }

  return (uint32_t)(frequency / (((SCU_CLK->USBCLKCR & SCU_CLK_USBCLKCR_USBDIV_Msk) >>
                                   SCU_CLK_USBCLKCR_USBDIV_Pos) + 1UL));
}

#if defined(EBU)
/*
 * API to retrieve EBU clock frequency
 */
uint32_t XMC_SCU_CLOCK_GetEbuClockFrequency(void)
{
  uint32_t frequency = XMC_SCU_CLOCK_GetSystemPllClockFrequency();
  
  return (uint32_t)((frequency /(((SCU_CLK->EBUCLKCR & SCU_CLK_EBUCLKCR_EBUDIV_Msk) >>
                                   SCU_CLK_EBUCLKCR_EBUDIV_Pos) + 1UL)));
}
#endif

#if defined(ECAT0)
/*
 * API to retrieve ECAT clock frequency
 */
uint32_t XMC_SCU_CLOCK_GetECATClockFrequency(void)
{
  uint32_t frequency;

  if ((SCU_CLK->ECATCLKCR & SCU_CLK_ECATCLKCR_ECATSEL_Msk) != 0U)
  {
    frequency =  XMC_SCU_CLOCK_GetSystemPllClockFrequency();
  }
  else
  {
    frequency =  XMC_SCU_CLOCK_GetUsbPllClockFrequency();
  }

  return (uint32_t)((frequency / (XMC_SCU_CLOCK_GetECATClockDivider() + 1UL)));
}
#endif

/*
 * API to retrieve WDT clock frequency
 */
uint32_t XMC_SCU_CLOCK_GetWdtClockFrequency(void)
{
  uint32_t frequency = 0UL;
  XMC_SCU_CLOCK_WDTCLKSRC_t clksrc;

  clksrc = XMC_SCU_CLOCK_GetWdtClockSource();

  if (clksrc == XMC_SCU_CLOCK_WDTCLKSRC_PLL)
  {
    frequency = XMC_SCU_CLOCK_GetSystemPllClockFrequency();
  }
  else if (clksrc == XMC_SCU_CLOCK_WDTCLKSRC_OFI)
  {
    frequency = OFI_FREQUENCY;
  }
  else if (clksrc == XMC_SCU_CLOCK_WDTCLKSRC_STDBY)
  {
    frequency = OSI_FREQUENCY;
  }
  else
  {

  }

  return (uint32_t)((frequency / (((SCU_CLK->WDTCLKCR & SCU_CLK_WDTCLKCR_WDTDIV_Msk) >>
                                    SCU_CLK_WDTCLKCR_WDTDIV_Pos) + 1UL)));
}

/**
 * @brief API to retrieve EXTERNAL-OUT clock frequency
 * @retval Clock frequency
 */
uint32_t XMC_SCU_CLOCK_GetExternalOutputClockFrequency(void)
{
  uint32_t frequency = 0UL;
  XMC_SCU_CLOCK_EXTOUTCLKSRC_t clksrc;

  clksrc = XMC_SCU_CLOCK_GetExternalOutputClockSource();

  if (clksrc == XMC_SCU_CLOCK_EXTOUTCLKSRC_PLL)
  {
    frequency = XMC_SCU_CLOCK_GetSystemPllClockFrequency();
    
    frequency = (uint32_t)((frequency / ((((SCU_CLK->EXTCLKCR) & SCU_CLK_EXTCLKCR_ECKDIV_Msk) >>
                 SCU_CLK_EXTCLKCR_ECKDIV_Pos)+ 1UL)));
  }
  else if (clksrc == XMC_SCU_CLOCK_EXTOUTCLKSRC_SYS)
  {
    frequency = XMC_SCU_CLOCK_GetSystemClockFrequency();
  }
  else if (clksrc == XMC_SCU_CLOCK_EXTOUTCLKSRC_USB)
  {
    frequency = XMC_SCU_CLOCK_GetUsbPllClockFrequency();
    
    frequency = (uint32_t)((frequency / ((((SCU_CLK->EXTCLKCR) & SCU_CLK_EXTCLKCR_ECKDIV_Msk) >>
                 SCU_CLK_EXTCLKCR_ECKDIV_Pos)+ 1UL)));
  }
  else
  {

  }

  return (frequency);
}

/*
 * API to retrieve clock frequency of peripherals on the peripheral bus using a shared functional clock
 */
uint32_t XMC_SCU_CLOCK_GetPeripheralClockFrequency(void)
{
  return (uint32_t)(XMC_SCU_CLOCK_GetCpuClockFrequency() >>
         ((SCU_CLK->PBCLKCR & SCU_CLK_PBCLKCR_PBDIV_Msk) >> SCU_CLK_PBCLKCR_PBDIV_Pos));  
}

/* API to select fSYS */
void XMC_SCU_CLOCK_SetSystemClockSource(const XMC_SCU_CLOCK_SYSCLKSRC_t source)
{
  SCU_CLK->SYSCLKCR = (SCU_CLK->SYSCLKCR & ((uint32_t)~SCU_CLK_SYSCLKCR_SYSSEL_Msk)) |
                      ((uint32_t)source);
}

/* API to select fUSB */
void XMC_SCU_CLOCK_SetUsbClockSource(const XMC_SCU_CLOCK_USBCLKSRC_t source)
{
  SCU_CLK->USBCLKCR = (SCU_CLK->USBCLKCR & ((uint32_t)~SCU_CLK_USBCLKCR_USBSEL_Msk)) |
                      ((uint32_t)source);
}

/* API to select fWDT */
void XMC_SCU_CLOCK_SetWdtClockSource(const XMC_SCU_CLOCK_WDTCLKSRC_t source)
{
  SCU_CLK->WDTCLKCR = (SCU_CLK->WDTCLKCR & ((uint32_t)~SCU_CLK_WDTCLKCR_WDTSEL_Msk)) |
                      ((uint32_t)source);
}

/* API to select fEXT */
void XMC_SCU_CLOCK_SetExternalOutputClockSource(const XMC_SCU_CLOCK_EXTOUTCLKSRC_t source)
{
  SCU_CLK->EXTCLKCR = (SCU_CLK->EXTCLKCR & ((uint32_t)~SCU_CLK_EXTCLKCR_ECKSEL_Msk)) |
                      ((uint32_t)source);
}

/* API to select fPLL */
void XMC_SCU_CLOCK_SetSystemPllClockSource(const XMC_SCU_CLOCK_SYSPLLCLKSRC_t source)
{
  /* Check input clock */
  if (source == XMC_SCU_CLOCK_SYSPLLCLKSRC_OSCHP) /* Select PLLClockSource */
  {
    SCU_PLL->PLLCON2 &= (uint32_t)~(SCU_PLL_PLLCON2_PINSEL_Msk | SCU_PLL_PLLCON2_K1INSEL_Msk);
  }
  else
  {
    SCU_PLL->PLLCON2 |= (uint32_t)(SCU_PLL_PLLCON2_PINSEL_Msk | SCU_PLL_PLLCON2_K1INSEL_Msk);
  }
}

/* API to select fRTC */
void XMC_SCU_HIB_SetRtcClockSource(const XMC_SCU_HIB_RTCCLKSRC_t source)
{ 
  /* Wait until the update of HDCR register in hibernate domain is completed */
  while((SCU_GENERAL->MIRRSTS) & SCU_GENERAL_MIRRSTS_HDCR_Msk)
  {
  }

  SCU_HIBERNATE->HDCR = (SCU_HIBERNATE->HDCR & ((uint32_t)~SCU_HIBERNATE_HDCR_RCS_Msk)) |
                        ((uint32_t)source);
}

/* API to select fSTDBY */
void XMC_SCU_HIB_SetStandbyClockSource(const XMC_SCU_HIB_STDBYCLKSRC_t source)
{
  while((SCU_GENERAL->MIRRSTS) & SCU_GENERAL_MIRRSTS_HDCR_Msk)
  {
    /* check SCU_MIRRSTS to ensure that no transfer over serial interface is pending */
  }
  SCU_HIBERNATE->HDCR = (SCU_HIBERNATE->HDCR & ((uint32_t)~SCU_HIBERNATE_HDCR_STDBYSEL_Msk)) |
                        ((uint32_t)source);
}

/* API to program the divider placed between fsys and its parent */
void XMC_SCU_CLOCK_SetSystemClockDivider(const uint32_t divider)
{
  XMC_ASSERT("XMC_SCU_CLOCK_SetSystemClockDivider:Wrong clock divider value",
              (divider <= (SCU_CLK_SYSCLKCR_SYSDIV_Msk + 1UL)) );

  SCU_CLK->SYSCLKCR = (SCU_CLK->SYSCLKCR & ((uint32_t)~SCU_CLK_SYSCLKCR_SYSDIV_Msk)) |
                      ((uint32_t)(((uint32_t)(divider - 1UL)) << SCU_CLK_SYSCLKCR_SYSDIV_Pos));
}

/* API to program the divider placed between fccu and its parent */
void XMC_SCU_CLOCK_SetCcuClockDivider(const uint32_t divider)
{
  XMC_ASSERT("XMC_SCU_CLOCK_SetCapcomClockDivider:Wrong clock divider value", (divider <= 2UL) );

  SCU_CLK->CCUCLKCR = (SCU_CLK->CCUCLKCR & ((uint32_t)~SCU_CLK_CCUCLKCR_CCUDIV_Msk)) |
                      (uint32_t)((uint32_t)(divider - 1UL) << SCU_CLK_CCUCLKCR_CCUDIV_Pos);
}

/* API to program the divider placed between fcpu and its parent */
void XMC_SCU_CLOCK_SetCpuClockDivider(const uint32_t divider)
{
  XMC_ASSERT("XMC_SCU_CLOCK_SetCpuClockDivider:Wrong clock divider value", (divider <= 2UL) );

  SCU_CLK->CPUCLKCR = (SCU_CLK->CPUCLKCR & ((uint32_t)~SCU_CLK_CPUCLKCR_CPUDIV_Msk)) |
                      (uint32_t)((uint32_t)(divider - 1UL) << SCU_CLK_CPUCLKCR_CPUDIV_Pos);
}

/* API to program the divider placed between fperiph and its parent */
void XMC_SCU_CLOCK_SetPeripheralClockDivider(const uint32_t divider)
{
  XMC_ASSERT("XMC_SCU_CLOCK_SetPeripheralClockDivider:Wrong clock divider value", (divider <= 2UL) );

  SCU_CLK->PBCLKCR = (SCU_CLK->PBCLKCR & ((uint32_t)~SCU_CLK_PBCLKCR_PBDIV_Msk)) |
                     ((uint32_t)((uint32_t)(divider - 1UL) << SCU_CLK_PBCLKCR_PBDIV_Pos));
}

/* API to program the divider placed between fsdmmc and its parent */
void XMC_SCU_CLOCK_SetUsbClockDivider(const uint32_t divider)
{
  XMC_ASSERT("XMC_SCU_CLOCK_SetSdmmcClockDivider:Wrong clock divider value",
              (divider <= (SCU_CLK_USBCLKCR_USBDIV_Msk + 1UL)) );

  SCU_CLK->USBCLKCR = (SCU_CLK->USBCLKCR & ((uint32_t)~SCU_CLK_USBCLKCR_USBDIV_Msk)) |
                      (uint32_t)((uint32_t)(divider - 1UL) << SCU_CLK_USBCLKCR_USBDIV_Pos); 
}

#if defined(EBU)
/* API to program the divider placed between febu and its parent */
void XMC_SCU_CLOCK_SetEbuClockDivider(const uint32_t divider)
{
  XMC_ASSERT("XMC_SCU_CLOCK_SetEbuClockDivider:Wrong clock divider value",
              (divider <= (SCU_CLK_EBUCLKCR_EBUDIV_Msk + 1UL) ) );

  SCU_CLK->EBUCLKCR = (SCU_CLK->EBUCLKCR & ((uint32_t)~SCU_CLK_EBUCLKCR_EBUDIV_Msk)) |
                      (uint32_t)(((uint32_t)(divider - 1UL)) << SCU_CLK_EBUCLKCR_EBUDIV_Pos);
}
#endif

/* API to program the divider placed between fwdt and its parent */
void XMC_SCU_CLOCK_SetWdtClockDivider(const uint32_t divider)
{
  XMC_ASSERT("XMC_SCU_CLOCK_SetWdtClockDivider:Wrong clock divider value",
              (divider <= (SCU_CLK_WDTCLKCR_WDTDIV_Msk + 1UL) ) );

  SCU_CLK->WDTCLKCR = (SCU_CLK->WDTCLKCR & ((uint32_t)~SCU_CLK_WDTCLKCR_WDTDIV_Msk)) |
                      (uint32_t)(((uint32_t)(divider - 1UL)) << SCU_CLK_WDTCLKCR_WDTDIV_Pos);
}

/* API to program the divider placed between fext and its parent */
void XMC_SCU_CLOCK_SetExternalOutputClockDivider(const uint32_t divider)
{
  XMC_ASSERT("XMC_SCU_CLOCK_SetExternalOutputClockDivider:Wrong clock divider value",
              (divider <= (SCU_CLK_EXTCLKCR_ECKDIV_Msk + 1UL) ) );

  SCU_CLK->EXTCLKCR = (SCU_CLK->EXTCLKCR & ((uint32_t)~SCU_CLK_EXTCLKCR_ECKDIV_Msk)) |
                      (uint32_t)(((uint32_t)(divider - 1UL)) << SCU_CLK_EXTCLKCR_ECKDIV_Pos);
}

#if defined(ECAT0)
/* API to configure the ECAT clock by setting the clock divider for the ECAT clock source */
void XMC_SCU_CLOCK_SetECATClockDivider(const uint32_t divider)
{
  SCU_CLK->ECATCLKCR = (SCU_CLK->ECATCLKCR & ~SCU_CLK_ECATCLKCR_ECADIV_Msk) |
                       (uint32_t)(((uint32_t)(divider - 1UL)) << SCU_CLK_ECATCLKCR_ECADIV_Pos);
}
#endif

/* API to enable a given module clock */
void XMC_SCU_CLOCK_EnableClock(const XMC_SCU_CLOCK_t clock)
{
  SCU_CLK->CLKSET = ((uint32_t)clock);
}

/* API to disable a given module clock */
void XMC_SCU_CLOCK_DisableClock(const XMC_SCU_CLOCK_t clock)
{
  SCU_CLK->CLKCLR = ((uint32_t)clock);
}

/* API to determine if module clock of the given peripheral is enabled */
bool XMC_SCU_CLOCK_IsClockEnabled(const XMC_SCU_CLOCK_t clock)
{
  return (bool)(SCU_CLK->CLKSTAT & ((uint32_t)clock));
}

#if defined(CLOCK_GATING_SUPPORTED)
/* API to gate a given module clock */
void XMC_SCU_CLOCK_GatePeripheralClock(const XMC_SCU_PERIPHERAL_CLOCK_t peripheral)
{
  uint32_t index = (peripheral & 0xf0000000UL) >> 28UL;
  uint32_t mask = (peripheral & (uint32_t)~0xf0000000UL);

  *(uint32_t *)((&(SCU_CLK->CGATSET0)) + (index * 3U)) = (uint32_t)mask;
}

/* API to ungate a given module clock */
void XMC_SCU_CLOCK_UngatePeripheralClock(const XMC_SCU_PERIPHERAL_CLOCK_t peripheral)
{
  uint32_t index = (uint32_t)((peripheral & 0xf0000000UL) >> 28UL);
  uint32_t mask = (peripheral & (uint32_t)~0xf0000000UL);

  *(uint32_t *)(&(SCU_CLK->CGATCLR0) + (index * 3U)) = (uint32_t)mask;
}

/* API to ungate a given module clock */
bool XMC_SCU_CLOCK_IsPeripheralClockGated(const XMC_SCU_PERIPHERAL_CLOCK_t peripheral)
{
  uint32_t index = ((peripheral & 0xf0000000UL) >> 28UL);
  uint32_t mask = (peripheral & (uint32_t)~0xf0000000UL);

  return ((*(uint32_t *)(&(SCU_CLK->CGATSTAT0) + (index * 3U)) & mask) != 0U);
}
#endif

float XMC_SCU_POWER_GetEVR13Voltage(void)
{
  return (SCU_POWER->EVRVADCSTAT & SCU_POWER_EVRVADCSTAT_VADC13V_Msk) * XMC_SCU_POWER_LSB13V;
}

float XMC_SCU_POWER_GetEVR33Voltage(void)
{
  return ((SCU_POWER->EVRVADCSTAT & SCU_POWER_EVRVADCSTAT_VADC33V_Msk) >> SCU_POWER_EVRVADCSTAT_VADC33V_Pos) * XMC_SCU_POWER_LSB33V;
}

/* API to enable USB PLL for USB clock */
void XMC_SCU_CLOCK_EnableUsbPll(void)
{
  SCU_PLL->USBPLLCON &= (uint32_t)~(SCU_PLL_USBPLLCON_VCOPWD_Msk | SCU_PLL_USBPLLCON_PLLPWD_Msk);
}

/* API to disable USB PLL for USB clock */
void XMC_SCU_CLOCK_DisableUsbPll(void)
{
  SCU_PLL->USBPLLCON |= (uint32_t)(SCU_PLL_USBPLLCON_VCOPWD_Msk | SCU_PLL_USBPLLCON_PLLPWD_Msk);
}

/* API to configure USB PLL */
void XMC_SCU_CLOCK_StartUsbPll(uint32_t pdiv, uint32_t ndiv)
{
  /* Go to bypass the USB PLL */
  SCU_PLL->USBPLLCON |= (uint32_t)SCU_PLL_USBPLLCON_VCOBYP_Msk;

  /* disconnect Oscillator from USB PLL */
  SCU_PLL->USBPLLCON |= (uint32_t)SCU_PLL_USBPLLCON_FINDIS_Msk;

  /* Setup Divider settings for USB PLL */
  SCU_PLL->USBPLLCON = (uint32_t)((uint32_t)((ndiv -1U) << SCU_PLL_USBPLLCON_NDIV_Pos) |
                       (uint32_t)((pdiv - 1U) << SCU_PLL_USBPLLCON_PDIV_Pos));

  /* Set OSCDISCDIS */
  SCU_PLL->USBPLLCON |= (uint32_t)SCU_PLL_USBPLLCON_OSCDISCDIS_Msk;

  /* connect Oscillator to USB PLL */
  SCU_PLL->USBPLLCON &= (uint32_t)~SCU_PLL_USBPLLCON_FINDIS_Msk;

  /* restart PLL Lock detection */
  SCU_PLL->USBPLLCON |= (uint32_t)SCU_PLL_USBPLLCON_RESLD_Msk;

  while ((SCU_PLL->USBPLLSTAT & SCU_PLL_USBPLLSTAT_VCOLOCK_Msk) == 0U)
  {
    /* wait for PLL Lock */
  }

}

/* API to disable USB PLL operation */
void XMC_SCU_CLOCK_StopUsbPll(void)
{
  SCU_PLL->USBPLLCON = (uint32_t)(SCU_PLL_USBPLLCON_VCOPWD_Msk | SCU_PLL_USBPLLCON_PLLPWD_Msk |
                                  SCU_PLL_USBPLLCON_VCOBYP_Msk);
}

/* API to onfigure the calibration mode for internal oscillator */
void XMC_SCU_CLOCK_SetBackupClockCalibrationMode(XMC_SCU_CLOCK_FOFI_CALIBRATION_MODE_t mode)
{
  /* Enable factory calibration based trimming */
  SCU_PLL->PLLCON0 |= (uint32_t)SCU_PLL_PLLCON0_FOTR_Msk;

  if (mode == XMC_SCU_CLOCK_FOFI_CALIBRATION_MODE_AUTOMATIC)
  {
    /* Disable factory calibration based trimming */
    SCU_PLL->PLLCON0 &= (uint32_t)~SCU_PLL_PLLCON0_FOTR_Msk;
    XMC_SCU_lDelay(100UL);

    /* Enable automatic calibration */
    SCU_PLL->PLLCON0 |= (uint32_t)SCU_PLL_PLLCON0_AOTREN_Msk;
  }

  XMC_SCU_lDelay(100UL);
}



/* API to enable USB Phy and comparator */
void XMC_SCU_POWER_EnableUsb(void)
{
#if defined(USB_OTG_SUPPORTED)
  SCU_POWER->PWRSET = (uint32_t)(SCU_POWER_PWRSET_USBOTGEN_Msk | SCU_POWER_PWRSET_USBPHYPDQ_Msk);
#else
  SCU_POWER->PWRSET = (uint32_t)SCU_POWER_PWRSET_USBPHYPDQ_Msk;
#endif
}

/* API to power down USB Phy and comparator */
void XMC_SCU_POWER_DisableUsb(void)
{
#if defined(USB_OTG_SUPPORTED)
  SCU_POWER->PWRCLR = (uint32_t)(SCU_POWER_PWRCLR_USBOTGEN_Msk | SCU_POWER_PWRSET_USBPHYPDQ_Msk);
#else
  SCU_POWER->PWRCLR = (uint32_t)SCU_POWER_PWRCLR_USBPHYPDQ_Msk;
#endif    
}

/* API to check USB PLL is locked or not */
bool XMC_SCU_CLOCK_IsUsbPllLocked(void)
{
  return (bool)((SCU_PLL->USBPLLSTAT & SCU_PLL_USBPLLSTAT_VCOLOCK_Msk) != 0UL);
}

/* API to power up the hibernation domain */
void XMC_SCU_HIB_EnableHibernateDomain(void)
{
  /* Power up HIB domain if and only if it is currently powered down */
  if((SCU_POWER->PWRSTAT & SCU_POWER_PWRSTAT_HIBEN_Msk) == 0UL)
  {
    SCU_POWER->PWRSET = (uint32_t)SCU_POWER_PWRSET_HIB_Msk;
    
    while((SCU_POWER->PWRSTAT & SCU_POWER_PWRSTAT_HIBEN_Msk) == 0UL)
    {
      /* wait until HIB domain is enabled */
    }    
  }
  
  /* Remove the reset only if HIB domain were in a state of reset */
  if((SCU_RESET->RSTSTAT) & SCU_RESET_RSTSTAT_HIBRS_Msk)
  {
    SCU_RESET->RSTCLR = (uint32_t)SCU_RESET_RSTCLR_HIBRS_Msk;
    while((SCU_RESET->RSTSTAT & SCU_RESET_RSTSTAT_HIBRS_Msk) != 0UL)
    {
      /* wait until HIB domain is enabled */
    }
  }
}

/* API to power down the hibernation domain */
void XMC_SCU_HIB_DisableHibernateDomain(void)
{
  /* Disable hibernate domain */   
  SCU_POWER->PWRCLR = (uint32_t)SCU_POWER_PWRCLR_HIB_Msk;
  /* Reset of hibernate domain reset */  
  SCU_RESET->RSTSET = (uint32_t)SCU_RESET_RSTSET_HIBRS_Msk;
}

/* API to check the hibernation domain is enabled or not */
bool XMC_SCU_HIB_IsHibernateDomainEnabled(void)
{
  return ((bool)(SCU_POWER->PWRSTAT & SCU_POWER_PWRSTAT_HIBEN_Msk) && 
          !(bool)(SCU_RESET->RSTSTAT & SCU_RESET_RSTSTAT_HIBRS_Msk));
}

/* API to enable internal slow clock - fOSI (32.768kHz) in hibernate domain */
void XMC_SCU_HIB_EnableInternalSlowClock(void)
{
  while((SCU_GENERAL->MIRRSTS) & SCU_GENERAL_MIRRSTS_OSCSICTRL_Msk)
  {
    /* Wait until OSCSICTRL register in hibernate domain is ready to accept a write */  
  }
  SCU_HIBERNATE->OSCSICTRL &= (uint32_t)~(SCU_HIBERNATE_OSCSICTRL_PWD_Msk);
}

/* API to disable internal slow clock - fOSI (32.768kHz) in hibernate domain */
void XMC_SCU_HIB_DisableInternalSlowClock(void)
{
  while((SCU_GENERAL->MIRRSTS) & SCU_GENERAL_MIRRSTS_OSCSICTRL_Msk)
  {
    /* Wait until OSCSICTRL register in hibernate domain is ready to accept a write */  
  }
  SCU_HIBERNATE->OSCSICTRL |= (uint32_t)SCU_HIBERNATE_OSCSICTRL_PWD_Msk;
}

void XMC_SCU_HIB_ClearEventStatus(int32_t event)
{
  while((SCU_GENERAL->MIRRSTS) & SCU_GENERAL_MIRRSTS_HDCLR_Msk)
  {
    /* Wait until HDCLR register in hibernate domain is ready to accept a write */  
  }
  SCU_HIBERNATE->HDCLR = event;
}

void XMC_SCU_HIB_TriggerEvent(int32_t event)
{
  while((SCU_GENERAL->MIRRSTS) & SCU_GENERAL_MIRRSTS_HDSET_Msk)
  {
    /* Wait until HDSET register in hibernate domain is ready to accept a write */    
  }
  SCU_HIBERNATE->HDSET = event;
}

void XMC_SCU_HIB_EnableEvent(int32_t event)
{
#if (defined(DOXYGEN) || (UC_SERIES == XMC44) || (UC_SERIES == XMC42) || (UC_SERIES == XMC41))
  event = ((event & XMC_SCU_HIB_EVENT_LPAC_VBAT_POSEDGE) << (SCU_HIBERNATE_HDCR_VBATHI_Pos - SCU_HIBERNATE_HDSTAT_VBATPEV_Pos)) | (event & (uint32_t)~XMC_SCU_HIB_EVENT_LPAC_VBAT_POSEDGE);
  event = ((event & XMC_SCU_HIB_EVENT_LPAC_VBAT_NEGEDGE) << (SCU_HIBERNATE_HDCR_VBATLO_Pos - SCU_HIBERNATE_HDSTAT_VBATNEV_Pos)) | (event & (uint32_t)~XMC_SCU_HIB_EVENT_LPAC_VBAT_NEGEDGE);
  event = ((event & XMC_SCU_HIB_EVENT_LPAC_HIB_IO_0_POSEDGE) << (SCU_HIBERNATE_HDCR_AHIBIO0HI_Pos - SCU_HIBERNATE_HDSTAT_AHIBIO0PEV_Pos)) | (event & (uint32_t)~XMC_SCU_HIB_EVENT_LPAC_HIB_IO_0_POSEDGE);
  event = ((event & XMC_SCU_HIB_EVENT_LPAC_HIB_IO_0_NEGEDGE) << (SCU_HIBERNATE_HDCR_AHIBIO0LO_Pos - SCU_HIBERNATE_HDSTAT_AHIBIO0NEV_Pos)) | (event & (uint32_t)~XMC_SCU_HIB_EVENT_LPAC_HIB_IO_0_NEGEDGE);
#if (defined(DOXYGEN) || ((UC_SERIES == XMC44) && (UC_PACKAGE == LQFP100)))
  event = ((event & XMC_SCU_HIB_EVENT_LPAC_HIB_IO_1_POSEDGE) << (SCU_HIBERNATE_HDCR_AHIBIO1HI_Pos - SCU_HIBERNATE_HDSTAT_AHIBIO1PEV_Pos)) | (event & (uint32_t)~XMC_SCU_HIB_EVENT_LPAC_HIB_IO_1_POSEDGE);
  event = ((event & XMC_SCU_HIB_EVENT_LPAC_HIB_IO_1_NEGEDGE) << (SCU_HIBERNATE_HDCR_AHIBIO1LO_Pos - SCU_HIBERNATE_HDSTAT_AHIBIO1NEV_Pos)) | (event & (uint32_t)~XMC_SCU_HIB_EVENT_LPAC_HIB_IO_1_NEGEDGE);
#endif
#endif

  while((SCU_GENERAL->MIRRSTS) & SCU_GENERAL_MIRRSTS_HDCR_Msk)
  {
    /* Wait until HDCR register in hibernate domain is ready to accept a write */    
  }
  SCU_HIBERNATE->HDCR |= event;
}

void XMC_SCU_HIB_DisableEvent(int32_t event)
{
#if (defined(DOXYGEN) || (UC_SERIES == XMC44) || (UC_SERIES == XMC42) || (UC_SERIES == XMC41))
  event = ((event & XMC_SCU_HIB_EVENT_LPAC_VBAT_POSEDGE) << (SCU_HIBERNATE_HDCR_VBATHI_Pos - SCU_HIBERNATE_HDSTAT_VBATPEV_Pos)) | (event & (uint32_t)~XMC_SCU_HIB_EVENT_LPAC_VBAT_POSEDGE);
  event = ((event & XMC_SCU_HIB_EVENT_LPAC_VBAT_NEGEDGE) << (SCU_HIBERNATE_HDCR_VBATLO_Pos - SCU_HIBERNATE_HDSTAT_VBATNEV_Pos)) | (event & (uint32_t)~XMC_SCU_HIB_EVENT_LPAC_VBAT_NEGEDGE);
  event = ((event & XMC_SCU_HIB_EVENT_LPAC_HIB_IO_0_POSEDGE) << (SCU_HIBERNATE_HDCR_AHIBIO0HI_Pos - SCU_HIBERNATE_HDSTAT_AHIBIO0PEV_Pos)) | (event & (uint32_t)~XMC_SCU_HIB_EVENT_LPAC_HIB_IO_0_POSEDGE);
  event = ((event & XMC_SCU_HIB_EVENT_LPAC_HIB_IO_0_NEGEDGE) << (SCU_HIBERNATE_HDCR_AHIBIO0LO_Pos - SCU_HIBERNATE_HDSTAT_AHIBIO0NEV_Pos)) | (event & (uint32_t)~XMC_SCU_HIB_EVENT_LPAC_HIB_IO_0_NEGEDGE);
#if (defined(DOXYGEN) || ((UC_SERIES == XMC44) && (UC_PACKAGE == LQFP100)))
  event = ((event & XMC_SCU_HIB_EVENT_LPAC_HIB_IO_1_POSEDGE) << (SCU_HIBERNATE_HDCR_AHIBIO1HI_Pos - SCU_HIBERNATE_HDSTAT_AHIBIO1PEV_Pos)) | (event & (uint32_t)~XMC_SCU_HIB_EVENT_LPAC_HIB_IO_1_POSEDGE);
  event = ((event & XMC_SCU_HIB_EVENT_LPAC_HIB_IO_1_NEGEDGE) << (SCU_HIBERNATE_HDCR_AHIBIO1LO_Pos - SCU_HIBERNATE_HDSTAT_AHIBIO1NEV_Pos)) | (event & (uint32_t)~XMC_SCU_HIB_EVENT_LPAC_HIB_IO_1_NEGEDGE);
#endif
#endif

  while((SCU_GENERAL->MIRRSTS) & SCU_GENERAL_MIRRSTS_HDCR_Msk)
  {
    /* Wait until HDCR register in hibernate domain is ready to accept a write */    
  }
  SCU_HIBERNATE->HDCR &= ~event;
}

void XMC_SCU_HIB_EnterHibernateState(void) 
{
  while((SCU_GENERAL->MIRRSTS) & SCU_GENERAL_MIRRSTS_HDCR_Msk)
  {
    /* Wait until HDCR register in hibernate domain is ready to accept a write */    
  }
  SCU_HIBERNATE->HDCR |= SCU_HIBERNATE_HDCR_HIB_Msk;
}

void XMC_SCU_HIB_EnterHibernateStateEx(XMC_SCU_HIB_HIBERNATE_MODE_t mode)
{
  if (mode == XMC_SCU_HIB_HIBERNATE_MODE_EXTERNAL)
  {
    XMC_SCU_HIB_EnterHibernateState();
  }
#if ((UC_SERIES == XMC44) || (UC_SERIES == XMC42) || (UC_SERIES == XMC41))
  if (mode == XMC_SCU_HIB_HIBERNATE_MODE_INTERNAL)
  {
    while((SCU_GENERAL->MIRRSTS) & SCU_GENERAL_MIRRSTS_HINTSET_Msk)
    {
      /* Wait until HDCR register in hibernate domain is ready to accept a write */
    }
    SCU_HIBERNATE->HINTSET = SCU_HIBERNATE_HINTSET_HIBNINT_Msk;
  }
#endif
}

void XMC_SCU_HIB_SetWakeupTriggerInput(XMC_SCU_HIB_IO_t pin)
{
  while((SCU_GENERAL->MIRRSTS) & SCU_GENERAL_MIRRSTS_HDCR_Msk)
  {
    /* Wait until HDCR register in hibernate domain is ready to accept a write */    
  }

  if (pin == XMC_SCU_HIB_IO_0)
  {
    SCU_HIBERNATE->HDCR |= SCU_HIBERNATE_HDCR_WKUPSEL_Msk;
  }
  else
  {
    SCU_HIBERNATE->HDCR &= ~SCU_HIBERNATE_HDCR_WKUPSEL_Msk; 
  }
}

void XMC_SCU_HIB_SetPinMode(XMC_SCU_HIB_IO_t pin, XMC_SCU_HIB_PIN_MODE_t mode)
{
  while((SCU_GENERAL->MIRRSTS) & SCU_GENERAL_MIRRSTS_HDCR_Msk)
  {
    /* Wait until HDCR register in hibernate domain is ready to accept a write */    
  }
  SCU_HIBERNATE->HDCR = (SCU_HIBERNATE->HDCR & ~(SCU_HIBERNATE_HDCR_HIBIO0SEL_Msk << (SCU_HIBERNATE_HDCR_HIBIOSEL_Size * pin))) |
                        (mode << (SCU_HIBERNATE_HDCR_HIBIOSEL_Size * pin));
}

void XMC_SCU_HIB_SetPinOutputLevel(XMC_SCU_HIB_IO_t pin, XMC_SCU_HIB_IO_OUTPUT_LEVEL_t level)
{
  while((SCU_GENERAL->MIRRSTS) & SCU_GENERAL_MIRRSTS_HDCR_Msk)
  {
    /* Wait until HDCR register in hibernate domain is ready to accept a write */    
  }
  SCU_HIBERNATE->HDCR = (SCU_HIBERNATE->HDCR & ~(SCU_HIBERNATE_HDCR_HIBIO0POL_Msk << pin)) |
                        (level << pin);
}

void XMC_SCU_HIB_SetInput0(XMC_SCU_HIB_IO_t pin)
{
  while((SCU_GENERAL->MIRRSTS) & SCU_GENERAL_MIRRSTS_HDCR_Msk)
  {
    /* Wait until HDCR register in hibernate domain is ready to accept a write */    
  }

  if (pin == XMC_SCU_HIB_IO_0)
  {
    SCU_HIBERNATE->HDCR |= SCU_HIBERNATE_HDCR_GPI0SEL_Msk;
  }
  else
  {
    SCU_HIBERNATE->HDCR &= ~SCU_HIBERNATE_HDCR_GPI0SEL_Msk; 
  }
}

void XMC_SCU_HIB_SetSR0Input(XMC_SCU_HIB_SR0_INPUT_t input)
{
  while((SCU_GENERAL->MIRRSTS) & SCU_GENERAL_MIRRSTS_HDCR_Msk)
  {
    /* Wait until HDCR register in hibernate domain is ready to accept a write */
  }
#if ((UC_SERIES == XMC44) || (UC_SERIES == XMC42) || (UC_SERIES == XMC41))
  SCU_HIBERNATE->HDCR = (SCU_HIBERNATE->HDCR & (uint32_t)~(SCU_HIBERNATE_HDCR_GPI0SEL_Msk | SCU_HIBERNATE_HDCR_ADIG0SEL_Msk)) | 
#else
  SCU_HIBERNATE->HDCR = (SCU_HIBERNATE->HDCR & (uint32_t)~(SCU_HIBERNATE_HDCR_GPI0SEL_Msk)) | 
#endif  
                        input;
}

#if ((UC_SERIES == XMC44) || (UC_SERIES == XMC42) || (UC_SERIES == XMC41))

#if ((UC_SERIES == XMC44) && (UC_PACKAGE == LQFP100))
void XMC_SCU_HIB_SetSR1Input(XMC_SCU_HIB_SR1_INPUT_t input)
{
  while((SCU_GENERAL->MIRRSTS) & SCU_GENERAL_MIRRSTS_HDCR_Msk)
  {
    /* Wait until HDCR register in hibernate domain is ready to accept a write */
  }
  SCU_HIBERNATE->HDCR = (SCU_HIBERNATE->HDCR & (uint32_t)~(SCU_HIBERNATE_HDCR_GPI0SEL_Msk | SCU_HIBERNATE_HDCR_ADIG0SEL_Msk | SCU_HIBERNATE_HDCR_XTALGPI1SEL_Msk)) | 
                        input;
}
#endif

void XMC_SCU_HIB_LPAC_SetInput(XMC_SCU_HIB_LPAC_INPUT_t input)
{
  while((SCU_GENERAL->MIRRSTS) & SCU_GENERAL_MIRRSTS_LPACCONF_Msk)
  {
    /* Wait until HDCR register in hibernate domain is ready to accept a write */
  }
  SCU_HIBERNATE->LPACCONF = (SCU_HIBERNATE->LPACCONF &  (uint32_t)~SCU_HIBERNATE_LPACCONF_CMPEN_Msk) |
                            input;
}

void XMC_SCU_HIB_LPAC_SetTrigger(XMC_SCU_HIB_LPAC_TRIGGER_t trigger)
{
  while((SCU_GENERAL->MIRRSTS) & SCU_GENERAL_MIRRSTS_LPACCONF_Msk)
  {
    /* Wait until HDCR register in hibernate domain is ready to accept a write */
  }
  SCU_HIBERNATE->LPACCONF = (SCU_HIBERNATE->LPACCONF &  (uint32_t)~SCU_HIBERNATE_LPACCONF_TRIGSEL_Msk) |
                            trigger;
}

void XMC_SCU_HIB_LPAC_SetTiming(bool enable_delay, uint16_t interval_count, uint8_t settle_count)
{
  uint32_t config = 0;

  if (enable_delay)
  {
    config = SCU_HIBERNATE_LPACCONF_CONVDEL_Msk;
  }

  config |= interval_count << SCU_HIBERNATE_LPACCONF_INTERVCNT_Pos;
  config |= settle_count << SCU_HIBERNATE_LPACCONF_SETTLECNT_Pos;

  while((SCU_GENERAL->MIRRSTS) & SCU_GENERAL_MIRRSTS_LPACCONF_Msk)
  {
    /* Wait until HDCR register in hibernate domain is ready to accept a write */
  }

  SCU_HIBERNATE->LPACCONF = (SCU_HIBERNATE->LPACCONF &  (uint32_t)~(SCU_HIBERNATE_LPACCONF_CONVDEL_Msk |
                                                                    SCU_HIBERNATE_LPACCONF_INTERVCNT_Msk |
                                                                    SCU_HIBERNATE_LPACCONF_SETTLECNT_Msk)) |
                            config;

}

void XMC_SCU_HIB_LPAC_SetVBATThresholds(uint8_t lower, uint8_t upper)
{

  while((SCU_GENERAL->MIRRSTS) & SCU_GENERAL_MIRRSTS_LPACTH0_Msk)
  {
    /* Wait until HDCR register in hibernate domain is ready to accept a write */
  }

  SCU_HIBERNATE->LPACTH0 = (lower << SCU_HIBERNATE_LPACTH0_VBATLO_Pos) | (upper << SCU_HIBERNATE_LPACTH0_VBATHI_Pos);



}

void XMC_SCU_HIB_LPAC_SetHIBIO0Thresholds(uint8_t lower, uint8_t upper)
{

  while((SCU_GENERAL->MIRRSTS) & SCU_GENERAL_MIRRSTS_LPACTH1_Msk)
  {
    /* Wait until HDCR register in hibernate domain is ready to accept a write */
  }

  SCU_HIBERNATE->LPACTH1 = (SCU_HIBERNATE->LPACTH1 & (uint32_t)~(SCU_HIBERNATE_LPACTH1_AHIBIO0LO_Msk | SCU_HIBERNATE_LPACTH1_AHIBIO0HI_Msk)) |
                           (lower << SCU_HIBERNATE_LPACTH1_AHIBIO0LO_Pos) |
                           (upper << SCU_HIBERNATE_LPACTH1_AHIBIO0HI_Pos);

}
#if ((UC_SERIES == XMC44) && (UC_PACKAGE == LQFP100))
void XMC_SCU_HIB_LPAC_SetHIBIO1Thresholds(uint8_t lower, uint8_t upper)
{

  while((SCU_GENERAL->MIRRSTS) & SCU_GENERAL_MIRRSTS_LPACTH1_Msk)
  {
    /* Wait until HDCR register in hibernate domain is ready to accept a write */
  }

  SCU_HIBERNATE->LPACTH1 = (SCU_HIBERNATE->LPACTH1 & (uint32_t)~(SCU_HIBERNATE_LPACTH1_AHIBIO1LO_Msk | SCU_HIBERNATE_LPACTH1_AHIBIO1HI_Msk)) |
                           (lower << SCU_HIBERNATE_LPACTH1_AHIBIO1LO_Pos) |
                           (upper << SCU_HIBERNATE_LPACTH1_AHIBIO1HI_Pos);

}
#endif
int32_t XMC_SCU_HIB_LPAC_GetStatus(void)
{
  return SCU_HIBERNATE->LPACST;
}

void XMC_SCU_HIB_LPAC_ClearStatus(int32_t status)
{
  while((SCU_GENERAL->MIRRSTS) & SCU_GENERAL_MIRRSTS_LPACCLR_Msk)
  {
    /* Wait until LPACCLR register in hibernate domain is ready to accept a write */
  }
  SCU_HIBERNATE->LPACCLR = status;;
}

void XMC_SCU_HIB_LPAC_TriggerCompare(XMC_SCU_HIB_LPAC_INPUT_t input)
{
  while((SCU_GENERAL->MIRRSTS) & SCU_GENERAL_MIRRSTS_LPACSET_Msk)
  {
    /* Wait until LPACSET register in hibernate domain is ready to accept a write */
  }

  SCU_HIBERNATE->LPACSET = input;
}

#endif

bool XMC_SCU_CLOCK_IsLowPowerOscillatorStable(void)
{
  return ((SCU_HIBERNATE->HDSTAT & SCU_HIBERNATE_HDSTAT_ULPWDG_Msk) == 0UL);
}

/* API to configure the 32khz Ultra Low Power oscillator */
void XMC_SCU_CLOCK_EnableLowPowerOscillator(void)
{
  /* Enable OSC_ULP */
  while((SCU_GENERAL->MIRRSTS) & SCU_GENERAL_MIRRSTS_OSCULCTRL_Msk)
  {
    /* Wait until no pending update to OSCULCTRL register in hibernate domain */
  }
  SCU_HIBERNATE->OSCULCTRL &= ~SCU_HIBERNATE_OSCULCTRL_MODE_Msk;

  /* Enable OSC_ULP Oscillator Watchdog*/
  while (SCU_GENERAL->MIRRSTS & SCU_GENERAL_MIRRSTS_HDCR_Msk)
  {
    /* check SCU_MIRRSTS to ensure that no transfer over serial interface is pending */
  }
  SCU_HIBERNATE->HDCR |= (uint32_t)SCU_HIBERNATE_HDCR_ULPWDGEN_Msk;

  /* Enable OSC_ULP Oscillator Watchdog*/
  while (SCU_GENERAL->MIRRSTS & SCU_GENERAL_MIRRSTS_HDSET_Msk)
  {
    /* check SCU_MIRRSTS to ensure that no transfer over serial interface is pending */
  }
  SCU_HIBERNATE->HDSET = (uint32_t)SCU_HIBERNATE_HDSET_ULPWDG_Msk;

  while (XMC_SCU_GetMirrorStatus() != 0)
  {
    /* Wait until update of the stanby clock source is done in the HIB domain */    
  }
}

/* API to configure the 32khz Ultra Low Power oscillator */
void XMC_SCU_CLOCK_DisableLowPowerOscillator(void)
{
  while((SCU_GENERAL->MIRRSTS) & SCU_GENERAL_MIRRSTS_OSCULCTRL_Msk)
  {
    /* Wait until OSCULCTRL register in hibernate domain is ready to accept a write */    
  }
  SCU_HIBERNATE->OSCULCTRL |= (uint32_t)SCU_HIBERNATE_OSCULCTRL_MODE_Msk;
}

void XMC_SCU_CLOCK_EnableLowPowerOscillatorGeneralPurposeInput(void)
{
  while((SCU_GENERAL->MIRRSTS) & SCU_GENERAL_MIRRSTS_OSCULCTRL_Msk)
  {
    /* Wait until OSCULCTRL register in hibernate domain is ready to accept a write */    
  }
  SCU_HIBERNATE->OSCULCTRL |= SCU_HIBERNATE_OSCULCTRL_X1DEN_Msk | SCU_HIBERNATE_OSCULCTRL_MODE_Msk;
}

void XMC_SCU_CLOCK_DisableLowPowerOscillatorGeneralPurposeInput(void)
{
  while((SCU_GENERAL->MIRRSTS) & SCU_GENERAL_MIRRSTS_OSCULCTRL_Msk)
  {
    /* Wait until OSCULCTRL register in hibernate domain is ready to accept a write */    
  }
  SCU_HIBERNATE->OSCULCTRL = (SCU_HIBERNATE->OSCULCTRL & ~(uint32_t)(SCU_HIBERNATE_OSCULCTRL_X1DEN_Msk | SCU_HIBERNATE_OSCULCTRL_MODE_Msk)) |
                             (SCU_HIBERNATE_OSCULCTRL_MODE_OSC_POWER_DOWN << SCU_HIBERNATE_OSCULCTRL_MODE_Pos);                       
}

uint32_t XMC_SCU_CLOCK_GetLowPowerOscillatorGeneralPurposeInputStatus(void)
{
  return (SCU_HIBERNATE->OSCULSTAT & SCU_HIBERNATE_OSCULSTAT_X1D_Msk);
}

/* API to enable High Precision High Speed oscillator */
void XMC_SCU_CLOCK_EnableHighPerformanceOscillator(void)
{
  SCU_PLL->PLLCON0 &= (uint32_t)~SCU_PLL_PLLCON0_PLLPWD_Msk;

  SCU_OSC->OSCHPCTRL = (uint32_t)((SCU_OSC->OSCHPCTRL & ~(SCU_OSC_OSCHPCTRL_MODE_Msk | SCU_OSC_OSCHPCTRL_OSCVAL_Msk)) |
                                  (((OSCHP_GetFrequency() / FOSCREF) - 1UL) << SCU_OSC_OSCHPCTRL_OSCVAL_Pos));

  /* restart OSC Watchdog */
  SCU_PLL->PLLCON0 &= (uint32_t)~SCU_PLL_PLLCON0_OSCRES_Msk;
}

bool XMC_SCU_CLOCK_IsHighPerformanceOscillatorStable(void)
{
  return ((SCU_PLL->PLLSTAT & XMC_SCU_PLL_PLLSTAT_OSC_USABLE) == XMC_SCU_PLL_PLLSTAT_OSC_USABLE);
}

/* API to disable High Precision High Speed oscillator */
void XMC_SCU_CLOCK_DisableHighPerformanceOscillator(void)
{
  SCU_OSC->OSCHPCTRL |= (uint32_t)SCU_OSC_OSCHPCTRL_MODE_Msk;
}

void XMC_SCU_CLOCK_EnableHighPerformanceOscillatorGeneralPurposeInput(void)
{
  SCU_OSC->OSCHPCTRL |= SCU_OSC_OSCHPCTRL_X1DEN_Msk;
}

void XMC_SCU_CLOCK_DisableHighPerformanceOscillatorGeneralPurposeInput(void)
{
  SCU_OSC->OSCHPCTRL &= ~SCU_OSC_OSCHPCTRL_X1DEN_Msk;
}

uint32_t XMC_SCU_CLOCK_GetHighPerformanceOscillatorGeneralPurposeInputStatus(void)
{
  return (SCU_OSC->OSCHPSTAT & SCU_OSC_OSCHPSTAT_X1D_Msk);
}

/* API to enable main PLL */
void XMC_SCU_CLOCK_EnableSystemPll(void)
{
  SCU_PLL->PLLCON0 &= (uint32_t)~(SCU_PLL_PLLCON0_VCOPWD_Msk | SCU_PLL_PLLCON0_PLLPWD_Msk);
}

/* API to disable main PLL */
void XMC_SCU_CLOCK_DisableSystemPll(void)
{
  SCU_PLL->PLLCON0 |= (uint32_t)(SCU_PLL_PLLCON0_VCOPWD_Msk | SCU_PLL_PLLCON0_PLLPWD_Msk);
}

/* API to configure main PLL */
void XMC_SCU_CLOCK_StartSystemPll(XMC_SCU_CLOCK_SYSPLLCLKSRC_t source,
                                  XMC_SCU_CLOCK_SYSPLL_MODE_t mode,
                                  uint32_t pdiv,
                                  uint32_t ndiv,
                                  uint32_t kdiv)
{

  uint32_t vco_frequency; /* Q10.22, max VCO frequency = 520MHz */
  uint32_t kdiv_temp;

  XMC_SCU_CLOCK_SetSystemPllClockSource(source);

  if (mode == XMC_SCU_CLOCK_SYSPLL_MODE_NORMAL)
  {
    /* Calculate initial step to be close to fOFI */
    if (source == XMC_SCU_CLOCK_SYSPLLCLKSRC_OSCHP)
    {
      vco_frequency = (OSCHP_GetFrequency() / 1000000U) << 22;
    }
    else
    {
      vco_frequency = (OFI_FREQUENCY / 1000000U) << 22;
    }
    vco_frequency = ((vco_frequency * ndiv) / pdiv);
    kdiv_temp = (vco_frequency / (OFI_FREQUENCY / 1000000U)) >> 22;

    /* Switch to prescaler mode */
    SCU_PLL->PLLCON0 |= (uint32_t)SCU_PLL_PLLCON0_VCOBYP_Msk;

    /* disconnect Oscillator from PLL */
    SCU_PLL->PLLCON0 |= (uint32_t)SCU_PLL_PLLCON0_FINDIS_Msk;

    /* Setup divider settings for main PLL */
    SCU_PLL->PLLCON1 = (uint32_t)((SCU_PLL->PLLCON1 & ~(SCU_PLL_PLLCON1_NDIV_Msk | SCU_PLL_PLLCON1_K2DIV_Msk |
                                   SCU_PLL_PLLCON1_PDIV_Msk)) | ((ndiv - 1UL) << SCU_PLL_PLLCON1_NDIV_Pos) |
                                   ((kdiv_temp - 1UL) << SCU_PLL_PLLCON1_K2DIV_Pos) |
                                   ((pdiv - 1UL)<< SCU_PLL_PLLCON1_PDIV_Pos));

    /* Set OSCDISCDIS, OSC clock remains connected to the VCO in case of loss of lock */
    SCU_PLL->PLLCON0 |= (uint32_t)SCU_PLL_PLLCON0_OSCDISCDIS_Msk;

    /* connect Oscillator to PLL */
    SCU_PLL->PLLCON0 &= (uint32_t)~SCU_PLL_PLLCON0_FINDIS_Msk;

    /* restart PLL Lock detection */
    SCU_PLL->PLLCON0 |= (uint32_t)SCU_PLL_PLLCON0_RESLD_Msk;
    while ((SCU_PLL->PLLSTAT & SCU_PLL_PLLSTAT_VCOLOCK_Msk) == 0U)
    {
      /* wait for PLL Lock */
    }

    /* Switch to normal mode */
    SCU_PLL->PLLCON0 &= (uint32_t)~SCU_PLL_PLLCON0_VCOBYP_Msk;
    while ((SCU_PLL->PLLSTAT & SCU_PLL_PLLSTAT_VCOBYST_Msk) != 0U)
    {
      /* wait for normal mode */
    }

    /* Ramp up PLL frequency in steps */
    kdiv_temp = (vco_frequency / 60UL) >> 22;
    if (kdiv < kdiv_temp)
    {
      XMC_SCU_CLOCK_StepSystemPllFrequency(kdiv_temp);
    }

    kdiv_temp = (vco_frequency / 90UL) >> 22;
    if (kdiv < kdiv_temp)
    {
      XMC_SCU_CLOCK_StepSystemPllFrequency(kdiv_temp);
    }

    XMC_SCU_CLOCK_StepSystemPllFrequency(kdiv);
  }
  else
  {
    SCU_PLL->PLLCON1 = (uint32_t)((SCU_PLL->PLLCON1 & ~SCU_PLL_PLLCON1_K1DIV_Msk) |
                       ((kdiv -1UL) << SCU_PLL_PLLCON1_K1DIV_Pos));

    /* Switch to prescaler mode */
    SCU_PLL->PLLCON0 |= (uint32_t)SCU_PLL_PLLCON0_VCOBYP_Msk;

    while ((SCU_PLL->PLLSTAT & SCU_PLL_PLLSTAT_VCOBYST_Msk) == 0U)
    {
      /* wait for prescaler mode */
    }
  }
}

/* API to stop main PLL operation */
void XMC_SCU_CLOCK_StopSystemPll(void)
{
  SCU_PLL->PLLCON0 |= (uint32_t)SCU_PLL_PLLCON0_PLLPWD_Msk;
}

/* API to step up/down the main PLL frequency */
void XMC_SCU_CLOCK_StepSystemPllFrequency(uint32_t kdiv)
{
  SCU_PLL->PLLCON1 = (uint32_t)((SCU_PLL->PLLCON1 & ~SCU_PLL_PLLCON1_K2DIV_Msk) |
                     ((kdiv - 1UL) << SCU_PLL_PLLCON1_K2DIV_Pos));

  XMC_SCU_lDelay(50U);
}

/* API to check main PLL is locked or not */
bool XMC_SCU_CLOCK_IsSystemPllLocked(void)
{
  return (bool)((SCU_PLL->PLLSTAT & SCU_PLL_PLLSTAT_VCOLOCK_Msk) != 0UL);
}

/*
 * API to assign the event handler function to be executed on occurrence of the selected event.
 */
XMC_SCU_STATUS_t XMC_SCU_INTERRUPT_SetEventHandler(const XMC_SCU_INTERRUPT_EVENT_t event,
                                                   const XMC_SCU_INTERRUPT_EVENT_HANDLER_t handler)
{
  uint32_t index;
  XMC_SCU_STATUS_t status;
  
  index = 0U;
  while (((event & ((XMC_SCU_INTERRUPT_EVENT_t)1 << index)) == 0U) && (index < XMC_SCU_INTERRUPT_EVENT_MAX))
  {
    index++;
  }
  
  if (index == XMC_SCU_INTERRUPT_EVENT_MAX)
  {
    status = XMC_SCU_STATUS_ERROR;
  }
  else
  {
    event_handler_list[index] = handler;
    status = XMC_SCU_STATUS_OK;      
  }
  
  return (status);
}

/*
 * API to execute callback functions for multiple events.
 */
void XMC_SCU_IRQHandler(uint32_t sr_num)
{
  uint32_t index;
  XMC_SCU_INTERRUPT_EVENT_t event;
  XMC_SCU_INTERRUPT_EVENT_HANDLER_t event_handler;
  
  XMC_UNUSED_ARG(sr_num);
  
  index = 0U;
  event = XMC_SCU_INTERUPT_GetEventStatus();
  while (index < XMC_SCU_INTERRUPT_EVENT_MAX)
  {    
    if ((event & ((XMC_SCU_INTERRUPT_EVENT_t)1 << index)) != 0U)
    {
      event_handler = event_handler_list[index];
      if (event_handler != NULL)
      {
          (event_handler)();
      }
      
      XMC_SCU_INTERRUPT_ClearEventStatus((uint32_t)(1UL << index));
      
      break;
    }   
    index++;    
  }
}

#endif /* UC_FAMILY == XMC4 */
