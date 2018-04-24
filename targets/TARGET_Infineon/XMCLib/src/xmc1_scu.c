/**
 * @file xmc1_scu.c
 * @date 2017-06-24
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
 *     - XMC_SCU_StartTempMeasurement API is modified
 *     - XMC_ASSERT statements are added in XMC_SCU_INTERRUPT_SetEventHandler
 *
 * 2015-06-20:
 *     - XMC_SCU_INTERRUPT_EnableEvent,XMC_SCU_INTERRUPT_DisableEvent,
 *     - XMC_SCU_INTERRUPT_TriggerEvent,XMC_SCU_INTERUPT_GetEventStatus,
 *     - XMC_SCU_INTERRUPT_ClearEventStatus APIs are added
 *
 * 2015-09-23:
 *     - XMC1400 support added
 *
 * 2015-11-30:
 *     - Documentation improved
 *
 * 2016-02-29:
 *     - Fixed XMC_SCU_CLOCK_ScaleMCLKFrequency
 *       It solves issues with down clock frequency scaling
 *
 * 2016-04-15:
 *     - Fixed XMC_SCU_CLOCK_Init for XMC1400
 *       It solves issues when trying to disable the OSCHP and use the XTAL pins as GPIO
 *
 * 2017-02-09
 *     - At XMC_SCU_CLOCK_Init() fixed issue while reading oscillator watchdog status
 *
 * 2017-04-11:
 *     - Added XMC_SCU_SetBMI()
 *
 * 2017-06-24
 *     - Changed XMC_SCU_SetBMI() for XMC11/XMC12/XMC13 to set to 1 the bit 11 of BMI
 *
 * @endcond
 *
 */

/**
 *
 * @brief SCU low level driver API prototype definition for XMC1 family of microcontrollers <br>
 *
 * <b>Detailed description of file</b> <br>
 * APIs provided in this file cover the following functional blocks of SCU: <br>
 * -- GCU (APIs prefixed with XMC_SCU_GEN_) <br>
 * ----Temperature Monitoring, Voltage Monitoring, CCU Start etc
 *
 * -- CCU (APIs prefixed with XMC_SCU_CLOCK_)<br>
 * ---- Clock initialization, Clock Gating, Sleep Management etc
 *
 * -- RCU (APIs prefixed with XMC_SCU_RESET_) <br>
 * ---- Reset Init, Cause, Manual Reset Assert/Deassert etc
 *
 * -- INTERRUPT (APIs prefixed with XMC_SCU_INTERRUPT_)<br>
 * ---- Initialization, Manual Assert/Deassert, Acknowledge etc
 *
 */

/*********************************************************************************************************************
 * HEADER FILES
 ********************************************************************************************************************/
#include <xmc_scu.h>

#if UC_FAMILY == XMC1

/*********************************************************************************************************************
 * MACROS
 ********************************************************************************************************************/
#define SCU_IRQ_NUM             (3U)  /**< array index value for list of events that can generate SCU interrupt */
#define SCU_GCU_PASSWD_PROT_ENABLE  (195UL) /**< Password for enabling protection */
#define SCU_GCU_PASSWD_PROT_DISABLE (192UL) /**< Password for disabling protection */


#define XMC_SCU_CHECK_RTCCLKSRC(source)       ( (source == XMC_SCU_CLOCK_RTCCLKSRC_DCO2) || \
                                                (source == XMC_SCU_CLOCK_RTCCLKSRC_ERU_IOUT0) || \
                                                (source == XMC_SCU_CLOCK_RTCCLKSRC_ACMP0_OUT) || \
                                                (source == XMC_SCU_CLOCK_RTCCLKSRC_ACMP1_OUT) || \
                                                (source == XMC_SCU_CLOCK_RTCCLKSRC_ACMP2_OUT) ) /**< Used to verify
                                                                                                whether provided RTC
                                                                                                clock source is valid
                                                                                                or not */
#define SCU_GENERAL_INTCR_INTSEL_Msk SCU_GENERAL_INTCR0_INTSEL0_Msk /**< Mask value of Interrupt Source Select
                                                                         for Node 0 */
#define SCU_GENERAL_INTCR_INTSEL_Size SCU_GENERAL_INTCR0_INTSEL1_Pos /**< Bit position value of Interrupt Source Select
                                                                         for Node 1 */

#define ANA_TSE_T1   (0x10000F30U) /**< d is a constant data can be retrieved from Flash sector 0 to calculate OFFSET
                                         value for DCO calibration */
#define ANA_TSE_T2   (0x10000F31U) /**< e is a constant data can be retrieved from Flash sector 0 to calculate OFFSET
                                         value for DCO calibration */
#define DCO_ADJLO_T1 (0x10000F32U) /**< b is a constant data can be retrieved from Flash sector 0 to calculate OFFSET
                                         value for DCO calibration */
#define DCO_ADJLO_T2 (0x10000F33U) /**< a is a constant data can be retrieved from Flash sector 0 to calculate OFFSET
                                         value for DCO calibration */

#if UC_SERIES == XMC14
#define XMC_SCU_INTERRUPT_EVENT_MAX (64U) /**< Maximum supported SCU events for XMC14 device. */
#else
#define XMC_SCU_INTERRUPT_EVENT_MAX (32U)  /**< Maximum supported SCU events for XMC11/12/13 device. */
#endif

#if UC_SERIES == XMC14
#define DCO1_DIV2_FREQUENCY_KHZ_Q22_10 (48000U << 10) /**< used to configures main clock (MCLK) frequency to requested
                                                         frequency value during runtime for XMC14 device. */
#else
#define DCO1_DIV2_FREQUENCY_KHZ_Q24_8 (32000U << 8) /**< used to configures main clock (MCLK) frequency to requested
                                                         frequency value during runtime for XMC11/12/13  device. */
#endif

#define ROM_BmiInstallationReq \
        (*((uint32_t (**)(uint32_t requestedBmiValue))0x00000108U)) /**< Pointer to Request BMI installation routine is
                                                                         available inside ROM. */

#define ROM_CalcTemperature \
        (*((uint32_t (**)(void))0x0000010cU)) /**<  Pointer to Calculate chip temperature routine is
                                                    available inside ROM. */

#define ROM_CalcTSEVAR \
        (*((uint32_t (**)(uint32_t temperature))0x00000120U)) /**<  Pointer to Calculate target level for temperature
                                                                    comparison routine is available inside ROM. */
        
/*********************************************************************************************************************
 * LOCAL DATA
 ********************************************************************************************************************/
static XMC_SCU_INTERRUPT_EVENT_HANDLER_t event_handler_list[XMC_SCU_INTERRUPT_EVENT_MAX]; /**< For registering callback
                                                                                        functions on SCU event
                                                                                        occurrence. */

static XMC_SCU_INTERRUPT_EVENT_t event_masks[SCU_IRQ_NUM] =
{
  (XMC_SCU_INTERRUPT_EVENT_FLASH_ERROR |
   XMC_SCU_INTERRUPT_EVENT_FLASH_COMPLETED |
   XMC_SCU_INTERRUPT_EVENT_PESRAM |
   XMC_SCU_INTERRUPT_EVENT_PEUSIC0 |
#if defined(USIC1)
   XMC_SCU_INTERRUPT_EVENT_PEUSIC1 |
#endif
#if defined(CAN)
   XMC_SCU_INTERRUPT_EVENT_PEMCAN |
#endif
#if UC_SERIES == XMC14
   XMC_SCU_INTERRUPT_EVENT_LOSS_EXT_CLOCK |
#endif
   XMC_SCU_INTERRUPT_EVENT_LOCI),

  (XMC_SCU_INTERRUPT_EVENT_STDBYCLKFAIL |
#if UC_SERIES == XMC14
   XMC_SCU_INTERRUPT_EVENT_DCO1_OUT_SYNC |
#endif
   XMC_SCU_INTERRUPT_EVENT_VDDPI |
   XMC_SCU_INTERRUPT_EVENT_VDROP |
   XMC_SCU_INTERRUPT_EVENT_VCLIP |
   XMC_SCU_INTERRUPT_EVENT_TSE_DONE |
   XMC_SCU_INTERRUPT_EVENT_TSE_HIGH |
   XMC_SCU_INTERRUPT_EVENT_TSE_LOW |
   XMC_SCU_INTERRUPT_EVENT_WDT_WARN |
   XMC_SCU_INTERRUPT_EVENT_RTC_PERIODIC |
   XMC_SCU_INTERRUPT_EVENT_RTC_ALARM |
   XMC_SCU_INTERRUPT_EVENT_RTCCTR_UPDATED |
   XMC_SCU_INTERRUPT_EVENT_RTCATIM0_UPDATED |
   XMC_SCU_INTERRUPT_EVENT_RTCATIM1_UPDATED |
   XMC_SCU_INTERRUPT_EVENT_RTCTIM0_UPDATED |
   XMC_SCU_INTERRUPT_EVENT_RTCTIM1_UPDATED),

  (
#if UC_SERIES != XMC11
   XMC_SCU_INTERRUPT_EVENT_ORC0 |
   XMC_SCU_INTERRUPT_EVENT_ORC1 |
   XMC_SCU_INTERRUPT_EVENT_ORC2 |
   XMC_SCU_INTERRUPT_EVENT_ORC3 |
   XMC_SCU_INTERRUPT_EVENT_ORC4 |
   XMC_SCU_INTERRUPT_EVENT_ORC5 |
   XMC_SCU_INTERRUPT_EVENT_ORC6 |
   XMC_SCU_INTERRUPT_EVENT_ORC7 |
#endif
#if defined(COMPARATOR)
   XMC_SCU_INTERRUPT_EVENT_ACMP0 |
   XMC_SCU_INTERRUPT_EVENT_ACMP1 |
   XMC_SCU_INTERRUPT_EVENT_ACMP2 |
#if UC_SERIES == XMC14
   XMC_SCU_INTERRUPT_EVENT_ACMP3 |
#endif
#endif
   0)
}; /**<   Defines list of events that can generate SCU interrupt and also indicates SCU events mapping to corresponding
         service request number. These event mask values can be used to verify which event is triggered that corresponds
         to service request number during runtime. All the event items are tabulated as per service request sources list
         table of SCU. */

/*********************************************************************************************************************
 * LOCAL ROUTINES
 ********************************************************************************************************************/
/* Utility routine to perform frequency up scaling */
static void XMC_SCU_CLOCK_lFrequencyUpScaling(uint32_t curr_idiv, uint32_t idiv);

/* Utility routine to perform frequency down scaling */
static void XMC_SCU_CLOCK_lFrequencyDownScaling(uint32_t curr_idiv, uint32_t idiv);

/* Calculates the value which must be installed in ANATSEIx register to get indication in
   SCU_INTERRUPT->SRRAW  bit when the chip temperature is above/below some target/threshold. */
static uint32_t XMC_SCU_CalcTSEVAR(uint32_t temperature)
{
  uint32_t limit;

  XMC_ASSERT("XMC_SCU_CalcTSEVAR: temperature out of range", (temperature >= 233U) && (temperature <= 388U));

  limit = ROM_CalcTSEVAR(temperature);
  if (limit == 0U)
  {
    limit = ROM_CalcTSEVAR(temperature + 1U);
    if (limit == 0U)
    {
      limit = ROM_CalcTSEVAR(temperature - 1U);
    }
  }
  
  return (limit);
}

#if UC_SERIES == XMC14
/* This is a local function used to generate the delay until register get updated with new configured value.  */
static void delay(uint32_t cycles)
{
  while(cycles > 0U)
  {
    __NOP();
    cycles--;
  }
}
#endif

/*********************************************************************************************************************
 * API IMPLEMENTATION
 ********************************************************************************************************************/
 #ifdef XMC_ASSERT_ENABLE
/* API to verify SCU event weather it is valid event or not */
__STATIC_INLINE bool XMC_SCU_INTERRUPT_IsValidEvent(XMC_SCU_INTERRUPT_EVENT_t event)
{
  return ((event == XMC_SCU_INTERRUPT_EVENT_WDT_WARN) ||
          (event == XMC_SCU_INTERRUPT_EVENT_RTC_PERIODIC) ||
          (event == XMC_SCU_INTERRUPT_EVENT_RTC_ALARM) ||
          (event == XMC_SCU_INTERRUPT_EVENT_VDDPI) ||
#if defined(USIC1)
          (event == XMC_SCU_INTERRUPT_EVENT_PEUSIC1) ||
#endif
#if defined(CAN)
          (event == XMC_SCU_INTERRUPT_EVENT_PEMCAN) ||
#endif
#if UC_SERIES == XMC14
          (event == XMC_SCU_INTERRUPT_EVENT_LOSS_EXT_CLOCK) ||
          (event == XMC_SCU_INTERRUPT_EVENT_DCO1_OUT_SYNC) ||
#endif
#if defined(COMPARATOR)
          (event == XMC_SCU_INTERRUPT_EVENT_ACMP0) ||
          (event == XMC_SCU_INTERRUPT_EVENT_ACMP1) ||
          (event == XMC_SCU_INTERRUPT_EVENT_ACMP2) ||
#if UC_SERIES == XMC14
          (event == XMC_SCU_INTERRUPT_EVENT_ACMP3) ||
#endif
#endif
          (event == XMC_SCU_INTERRUPT_EVENT_VDROP) ||
#if UC_SERIES != XMC11
          (event == XMC_SCU_INTERRUPT_EVENT_ORC0) ||
          (event == XMC_SCU_INTERRUPT_EVENT_ORC1) ||
          (event == XMC_SCU_INTERRUPT_EVENT_ORC2) ||
          (event == XMC_SCU_INTERRUPT_EVENT_ORC3) ||
          (event == XMC_SCU_INTERRUPT_EVENT_ORC4) ||
          (event == XMC_SCU_INTERRUPT_EVENT_ORC5) ||
          (event == XMC_SCU_INTERRUPT_EVENT_ORC6) ||
          (event == XMC_SCU_INTERRUPT_EVENT_ORC7) ||
#endif
          (event == XMC_SCU_INTERRUPT_EVENT_LOCI) ||
          (event == XMC_SCU_INTERRUPT_EVENT_PESRAM) ||
          (event == XMC_SCU_INTERRUPT_EVENT_PEUSIC0) ||
          (event == XMC_SCU_INTERRUPT_EVENT_FLASH_ERROR) ||
          (event == XMC_SCU_INTERRUPT_EVENT_FLASH_COMPLETED) ||
          (event == XMC_SCU_INTERRUPT_EVENT_VCLIP) ||
          (event == XMC_SCU_INTERRUPT_EVENT_STDBYCLKFAIL) ||
          (event == XMC_SCU_INTERRUPT_EVENT_RTCCTR_UPDATED) ||
          (event == XMC_SCU_INTERRUPT_EVENT_RTCATIM0_UPDATED) ||
          (event == XMC_SCU_INTERRUPT_EVENT_RTCATIM1_UPDATED) ||
          (event == XMC_SCU_INTERRUPT_EVENT_RTCTIM0_UPDATED) ||
          (event == XMC_SCU_INTERRUPT_EVENT_RTCTIM1_UPDATED) ||
          (event == XMC_SCU_INTERRUPT_EVENT_TSE_DONE) ||
          (event == XMC_SCU_INTERRUPT_EVENT_TSE_HIGH) ||
          (event == XMC_SCU_INTERRUPT_EVENT_TSE_LOW));
}
 #endif

/* API to enable the SCU event */
void XMC_SCU_INTERRUPT_EnableEvent(const XMC_SCU_INTERRUPT_EVENT_t event)
{
  SCU_INTERRUPT->SRMSK |= (uint32_t)event;
#if UC_SERIES == XMC14
  SCU_INTERRUPT->SRMSK1 |= (uint32_t)(event >> 32U);
#endif
}

/* API to disable the SCU event */
void XMC_SCU_INTERRUPT_DisableEvent(const XMC_SCU_INTERRUPT_EVENT_t event)
{
  SCU_INTERRUPT->SRMSK &= ~(uint32_t)event;
#if UC_SERIES == XMC14
  SCU_INTERRUPT->SRMSK1 &= (uint32_t)~(event >> 32U);
#endif
}

/* API to trigger the SCU event */
void XMC_SCU_INTERRUPT_TriggerEvent(const XMC_SCU_INTERRUPT_EVENT_t event)
{
  SCU_INTERRUPT->SRSET |= (uint32_t)event;
#if UC_SERIES == XMC14
  SCU_INTERRUPT->SRSET1 |= (uint32_t)(event >> 32U);
#endif
}

/* API to get the SCU event status */
XMC_SCU_INTERRUPT_EVENT_t XMC_SCU_INTERUPT_GetEventStatus(void)
{
  XMC_SCU_INTERRUPT_EVENT_t tmp;

  tmp = SCU_INTERRUPT->SRRAW;
#if UC_SERIES == XMC14
  tmp |= ((int64_t)SCU_INTERRUPT->SRRAW1 << 32U);
#endif
  return (tmp);
}

/* API to clear the SCU event status */
void XMC_SCU_INTERRUPT_ClearEventStatus(const XMC_SCU_INTERRUPT_EVENT_t event)
{
  SCU_INTERRUPT->SRCLR |= (uint32_t)event;
#if UC_SERIES == XMC14
  SCU_INTERRUPT->SRCLR1 |= (uint32_t)(event >> 32U);
#endif
}

/* API to lock protected bitfields from being modified */
void XMC_SCU_LockProtectedBits(void)
{
  SCU_GENERAL->PASSWD = SCU_GCU_PASSWD_PROT_ENABLE;
}

/* API to make protected bitfields available for modification */
void XMC_SCU_UnlockProtectedBits(void)
{
  SCU_GENERAL->PASSWD = SCU_GCU_PASSWD_PROT_DISABLE;

  while(((SCU_GENERAL->PASSWD) & SCU_GENERAL_PASSWD_PROTS_Msk))
  {
    /* Loop until the lock is removed */
  }
}

/* API to initialize power supply monitoring unit */
void XMC_SCU_SupplyMonitorInit(const XMC_SCU_SUPPLYMONITOR_t *obj)
{
  uint32_t anavdel;
  uint32_t irqmask;

  anavdel = 0UL;

  anavdel |= (uint32_t)((obj-> ext_supply_threshold) << SCU_ANALOG_ANAVDEL_VDEL_SELECT_Pos);
  anavdel |= (uint32_t)((obj->ext_supply_monitor_speed) << SCU_ANALOG_ANAVDEL_VDEL_TIM_ADJ_Pos);

  if(true == (obj->enable_at_init))
  {
    anavdel |= (uint32_t)SCU_ANALOG_ANAVDEL_VDEL_EN_Msk;
  }
   SCU_ANALOG->ANAVDEL = (uint16_t) anavdel;

  irqmask = 0UL;

  if(true == (obj->enable_prewarning_int))
  {
    irqmask |= (uint32_t)SCU_INTERRUPT_SRMSK_VDDPI_Msk;
  }

  if(true == (obj->enable_vdrop_int))
  {
    irqmask |= (uint32_t)SCU_INTERRUPT_SRMSK_VDROPI_Msk;
  }

  if(true == (obj->enable_vclip_int))
  {
    irqmask |= (uint32_t)SCU_INTERRUPT_SRMSK_VCLIPI_Msk;
  }
  SCU_INTERRUPT->SRMSK |= (uint32_t)irqmask;
}

/* API to program lower temperature limit */
XMC_SCU_STATUS_t XMC_SCU_SetTempLowLimit(uint32_t limit)
{
  XMC_SCU_STATUS_t status = XMC_SCU_STATUS_OK;

  limit = XMC_SCU_CalcTSEVAR(limit);

  if (limit != 0) 
  {
    SCU_ANALOG->ANATSEIL = (uint16_t)limit;
  }
  else
  {
    status = XMC_SCU_STATUS_ERROR;
  }

  return (status);
}
  
/* API to program higher temperature limit */
XMC_SCU_STATUS_t XMC_SCU_SetTempHighLimit(uint32_t limit)
{
  XMC_SCU_STATUS_t status = XMC_SCU_STATUS_OK;

  limit = XMC_SCU_CalcTSEVAR(limit);

  if (limit != 0) 
  {
    SCU_ANALOG->ANATSEIH = (uint16_t)limit;
  }
  else
  {
    status = XMC_SCU_STATUS_ERROR;
  } 

  return (status);
}

/* API to program temperature limits as raw digital values into temperature sensor */
void XMC_SCU_SetRawTempLimits(const uint32_t lower_temp, const uint32_t upper_temp)
{
  SCU_ANALOG->ANATSEIH = (uint16_t)(upper_temp & SCU_ANALOG_ANATSEIH_TSE_IH_Msk);
  SCU_ANALOG->ANATSEIL = (uint16_t)(lower_temp & SCU_ANALOG_ANATSEIL_TSE_IL_Msk);
}

/* API to start temperature measurement */
void XMC_SCU_StartTempMeasurement(void)
{
  SCU_ANALOG->ANATSECTRL |= (uint16_t)SCU_ANALOG_ANATSECTRL_TSE_EN_Msk;
}

/* API to stop temperature measurement */
void XMC_SCU_StopTempMeasurement(void)
{
  SCU_ANALOG->ANATSECTRL &= (uint16_t)~SCU_ANALOG_ANATSECTRL_TSE_EN_Msk;
}

/* API to check if the temperature has gone past the ceiling */
bool XMC_SCU_HighTemperature(void)
{
  return ((SCU_INTERRUPT->SRRAW & SCU_INTERRUPT_SRRAW_TSE_HIGH_Msk) == SCU_INTERRUPT_SRRAW_TSE_HIGH_Msk);
}

/* API to check if the temperature is lower than normal */
bool XMC_SCU_LowTemperature(void)
{
  return ((SCU_INTERRUPT->SRRAW & SCU_INTERRUPT_SRRAW_TSE_LOW_Msk) == SCU_INTERRUPT_SRRAW_TSE_LOW_Msk);
}

/* API to retrieve the device temperature */
uint32_t XMC_SCU_GetTemperature(void)
{
  uint32_t temperature;

  temperature = (uint32_t)(SCU_ANALOG->ANATSEMON);

  return (temperature);
}

/* Calculates the die temperature value using ROM function */
uint32_t XMC_SCU_CalcTemperature(void)
{
  return (ROM_CalcTemperature());
}


/* API which initializes the clock tree ofthe device */
void XMC_SCU_CLOCK_Init(const XMC_SCU_CLOCK_CONFIG_t *const config)
{
  /* Remove protection */
  XMC_SCU_UnlockProtectedBits();

#if (UC_SERIES == XMC14)
  /* OSCHP source selection - OSC mode */

  if (config->oschp_mode != XMC_SCU_CLOCK_OSCHP_MODE_DISABLED)
  {
    if (OSCHP_GetFrequency() > 20000000U)
    {
      SCU_ANALOG->ANAOSCHPCTRL |= (uint16_t)SCU_ANALOG_ANAOSCHPCTRL_HYSCTRL_Msk;
    }

    SCU_ANALOG->ANAOSCHPCTRL = (uint16_t)(SCU_ANALOG->ANAOSCHPCTRL & ~(SCU_ANALOG_ANAOSCHPCTRL_SHBY_Msk | SCU_ANALOG_ANAOSCHPCTRL_MODE_Msk)) |
                               config->oschp_mode;

    do
    {
      /* Restart OSC_HP oscillator watchdog */
      SCU_INTERRUPT->SRCLR1 = SCU_INTERRUPT_SRCLR1_LOECI_Msk;

      /* Enable OSC_HP oscillator watchdog*/
      SCU_CLK->OSCCSR &= ~SCU_CLK_OSCCSR_XOWDEN_Msk;
      SCU_CLK->OSCCSR |= SCU_CLK_OSCCSR_XOWDEN_Msk;

      /* Wait a few DCO2 cycles for the update of the clock detection result */
      delay(2500);

      /* check clock is ok */
    }
    while(SCU_INTERRUPT->SRRAW1 & SCU_INTERRUPT_SRRAW1_LOECI_Msk);
  }
  else /* (config->oschp_mode == XMC_SCU_CLOCK_OSCHP_MODE_DISABLED) */
  {
    SCU_ANALOG->ANAOSCHPCTRL |= SCU_ANALOG_ANAOSCHPCTRL_MODE_Msk;
  }

  SCU_ANALOG->ANAOSCLPCTRL = (uint16_t)config->osclp_mode;

  SCU_CLK->CLKCR1 = (SCU_CLK->CLKCR1 & ~SCU_CLK_CLKCR1_DCLKSEL_Msk) |
                    config->dclk_src;

#endif
  /* Update PCLK selection mux. */
  SCU_CLK->CLKCR = (SCU_CLK->CLKCR & (uint32_t)~(SCU_CLK_CLKCR_PCLKSEL_Msk | SCU_CLK_CLKCR_RTCCLKSEL_Msk)) |
                   config->rtc_src |
                   config->pclk_src;

  /* Close the lock opened above. */
  XMC_SCU_LockProtectedBits();

  /* Update the dividers now */
  XMC_SCU_CLOCK_ScaleMCLKFrequency(config->idiv, config->fdiv);

}

/* API which selects one of the available parent clock nodes for a given child clock node */
void XMC_SCU_CLOCK_SetRtcClockSource(const XMC_SCU_CLOCK_RTCCLKSRC_t source)
{
  XMC_ASSERT("XMC_SCU_CLOCK_SetRtcSourceClock:Wrong Parent Clock", XMC_SCU_CHECK_RTCCLKSRC(source));

  XMC_SCU_UnlockProtectedBits();

  SCU_CLK->CLKCR = (SCU_CLK->CLKCR & (uint32_t)~SCU_CLK_CLKCR_RTCCLKSEL_Msk) |
                   source;

  XMC_SCU_LockProtectedBits();
}

/* API to program the divider placed between fperiph and its parent */
void XMC_SCU_CLOCK_SetFastPeripheralClockSource(const XMC_SCU_CLOCK_PCLKSRC_t source)
{
  XMC_SCU_UnlockProtectedBits();

  SCU_CLK->CLKCR = (SCU_CLK->CLKCR & (uint32_t)~SCU_CLK_CLKCR_PCLKSEL_Msk) |
                   source;

  XMC_SCU_LockProtectedBits();
}

/* API which gates a clock node at its source */
void XMC_SCU_CLOCK_GatePeripheralClock(const XMC_SCU_PERIPHERAL_CLOCK_t peripheral)
{
  XMC_SCU_UnlockProtectedBits();
  SCU_CLK->CGATSET0 |= (uint32_t)peripheral;
  XMC_SCU_LockProtectedBits();
}

/* API which ungates a clock note at its source */
void XMC_SCU_CLOCK_UngatePeripheralClock(const XMC_SCU_PERIPHERAL_CLOCK_t peripheral)
{
  XMC_SCU_UnlockProtectedBits();
  SCU_CLK->CGATCLR0 |= (uint32_t)peripheral;
  while ((SCU_CLK->CLKCR) & SCU_CLK_CLKCR_VDDC2LOW_Msk)
  {
    /* Wait voltage suply stabilization */
  }
  XMC_SCU_LockProtectedBits();
}

/* Checks the status of peripheral clock gating */
bool XMC_SCU_CLOCK_IsPeripheralClockGated(const XMC_SCU_PERIPHERAL_CLOCK_t peripheral)
{
  return (bool)((SCU_CLK->CGATSTAT0 & peripheral) != 0);
}

/* This API configures main clock (MCLK) frequency to requested frequency value. */
void XMC_SCU_CLOCK_SetMCLKFrequency(uint32_t freq_khz)
{
  uint32_t ratio;
  uint32_t ratio_int;
  uint32_t ratio_frac;

#if UC_SERIES == XMC14
  if (((SCU_CLK->CLKCR1) & SCU_CLK_CLKCR1_DCLKSEL_Msk) == 0U)
  {
     ratio = DCO1_DIV2_FREQUENCY_KHZ_Q22_10 / freq_khz;
  }
  else
  {
     ratio = ((OSCHP_GetFrequency() / 1000U) << 10U) / freq_khz;
  }

  /* Manage overflow */
  if (ratio > 0xffffffU)
  {
    ratio = 0xffffffU;
  }

  ratio_int = ratio >> 10U;
  ratio_frac = ratio & 0x3ffU;
#else
    ratio = DCO1_DIV2_FREQUENCY_KHZ_Q24_8 / freq_khz;

  /* Manage overflow */
  if (ratio > 0xffffU)
  {
    ratio = 0xffffU;
  }

  ratio_int = ratio >> 8U;
  ratio_frac = ratio & 0xffU;
#endif

  XMC_SCU_CLOCK_ScaleMCLKFrequency(ratio_int, ratio_frac);
}


/* A utility routine which updates the fractional dividers in steps */
void XMC_SCU_CLOCK_ScaleMCLKFrequency(uint32_t idiv, uint32_t fdiv)
{
  /* Find out current and target value of idiv */
  uint32_t curr_idiv;

  XMC_SCU_UnlockProtectedBits();

  /* Take a snapshot of value already programmed into IDIV */
  curr_idiv = (SCU_CLK->CLKCR & SCU_CLK_CLKCR_IDIV_Msk) >> SCU_CLK_CLKCR_IDIV_Pos;

#if (UC_SERIES == XMC14)
  SCU_CLK->CLKCR1 = (SCU_CLK->CLKCR1 & (uint32_t)~(SCU_CLK_CLKCR1_FDIV_Msk)) |
                   (uint32_t)((fdiv >> 8U) << SCU_CLK_CLKCR1_FDIV_Pos);

  SCU_CLK->CLKCR = (SCU_CLK->CLKCR & (uint32_t)~(SCU_CLK_CLKCR_FDIV_Msk | SCU_CLK_CLKCR_CNTADJ_Msk)) |
                   (uint32_t)((fdiv & 0xffU) << SCU_CLK_CLKCR_FDIV_Pos) |
                   (uint32_t)(1023UL << SCU_CLK_CLKCR_CNTADJ_Pos);
#else
  SCU_CLK->CLKCR = (SCU_CLK->CLKCR & (uint32_t)~(SCU_CLK_CLKCR_FDIV_Msk | SCU_CLK_CLKCR_CNTADJ_Msk)) |
                   (uint32_t)(fdiv << SCU_CLK_CLKCR_FDIV_Pos) |
                   (uint32_t)(1023UL << SCU_CLK_CLKCR_CNTADJ_Pos);
#endif

  while ((SCU_CLK->CLKCR)& SCU_CLK_CLKCR_VDDC2LOW_Msk)
  {
    /* Spin until the core supply stabilizes */
  }

  if(curr_idiv <= idiv)
  {
    /* Requested IDIV is greater than currently programmed IDIV. So downscale the frequency */
    XMC_SCU_CLOCK_lFrequencyDownScaling(curr_idiv, idiv);
  }
  else
  {
    /* Requested IDIV is lower than currently programmed IDIV. So upscale the frequency */
    XMC_SCU_CLOCK_lFrequencyUpScaling(curr_idiv, idiv);
  }

  SCU_CLK->CLKCR = (SCU_CLK->CLKCR & (uint32_t)~(SCU_CLK_CLKCR_IDIV_Msk | SCU_CLK_CLKCR_CNTADJ_Msk)) |
                   (uint32_t)(idiv << SCU_CLK_CLKCR_IDIV_Pos) | (uint32_t)(1023UL << SCU_CLK_CLKCR_CNTADJ_Pos);

  while ((SCU_CLK->CLKCR) & SCU_CLK_CLKCR_VDDC2LOW_Msk)
  {
    /* Wait voltage suply stabilization */
  }

  XMC_SCU_LockProtectedBits();

  SystemCoreClockUpdate();

}

/* Utility routine to perform frequency up scaling */
static void XMC_SCU_CLOCK_lFrequencyUpScaling(uint32_t curr_idiv, uint32_t target_idiv)
{
  while (curr_idiv > (target_idiv * 4UL))
  {
    curr_idiv = (uint32_t)(curr_idiv >> 2UL);   /* Divide by 4. */

    SCU_CLK->CLKCR = (SCU_CLK->CLKCR & (uint32_t)~(SCU_CLK_CLKCR_IDIV_Msk | SCU_CLK_CLKCR_CNTADJ_Msk)) |
                     (uint32_t)(curr_idiv << SCU_CLK_CLKCR_IDIV_Pos) | 
                     (uint32_t)(1023UL << SCU_CLK_CLKCR_CNTADJ_Pos);

    while (SCU_CLK->CLKCR & SCU_CLK_CLKCR_VDDC2LOW_Msk)
    {
    /* Wait voltage suply stabilization */
    }
  }
}

/* Utility routine to perform frequency down scaling */
static void XMC_SCU_CLOCK_lFrequencyDownScaling(uint32_t curr_idiv, uint32_t target_idiv)
{

  while ((curr_idiv * 4UL) < target_idiv)
  {
    if(0U == curr_idiv)
    {
      curr_idiv = 1U;
    }
    curr_idiv  = (uint32_t)(curr_idiv << 2UL);   /* Multiply by 4. */
    SCU_CLK->CLKCR = (SCU_CLK->CLKCR & (uint32_t)~(SCU_CLK_CLKCR_IDIV_Msk | SCU_CLK_CLKCR_CNTADJ_Msk)) |
                     (uint32_t)(curr_idiv << SCU_CLK_CLKCR_IDIV_Pos) |
                     (uint32_t)(1023UL << SCU_CLK_CLKCR_CNTADJ_Pos);

    while (SCU_CLK->CLKCR & SCU_CLK_CLKCR_VDDC2LOW_Msk)
    {
    /* Wait voltage suply stabilization */
    }
  }
}

/*
 * API to retrieve clock frequency of peripherals on the peripheral bus using a shared functional clock
 */
uint32_t XMC_SCU_CLOCK_GetPeripheralClockFrequency(void)
{
  return (SystemCoreClock);
}

/* Provides the clock frequency of peripherals on the peripheral bus that are using a shared functional clock */
uint32_t XMC_SCU_CLOCK_GetFastPeripheralClockFrequency(void)
{
  return (SystemCoreClock << ((SCU_CLK->CLKCR & SCU_CLK_CLKCR_PCLKSEL_Msk) >> SCU_CLK_CLKCR_PCLKSEL_Pos));
}

/* DCO1 clock frequency can be calibrated during runtime to achieve a better accuracy */
void XMC_SCU_CLOCK_CalibrateOscillatorOnTemperature(int32_t temperature)
{
  int32_t a;
  int32_t b;
  int32_t d;
  int32_t e;
  int32_t offset;

  a = *((uint8_t*)DCO_ADJLO_T2);
  b = *((uint8_t*)DCO_ADJLO_T1);
  d = *((uint8_t*)ANA_TSE_T1);
  e = *((uint8_t*)ANA_TSE_T2);

  offset = b + (((a - b) * (temperature - d)) / (e - d));
  offset &= SCU_ANALOG_ANAOFFSET_ADJL_OFFSET_Msk;

  XMC_SCU_UnlockProtectedBits();
  SCU_ANALOG->ANAOFFSET = (uint16_t)offset;
  XMC_SCU_LockProtectedBits();
}

/*
 * API to assign the event handler function to be executed on occurrence of the selected event
 */
XMC_SCU_STATUS_t XMC_SCU_INTERRUPT_SetEventHandler(XMC_SCU_INTERRUPT_EVENT_t event, XMC_SCU_INTERRUPT_EVENT_HANDLER_t handler)
{
  uint32_t index;
  XMC_SCU_STATUS_t status;

  XMC_ASSERT("XMC_SCU_INTERRUPT_SetEventHandler: Invalid event", XMC_SCU_INTERRUPT_IsValidEvent(event));
  XMC_ASSERT("XMC_SCU_INTERRUPT_SetEventHandler: Invalid handler", handler != NULL);

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
 * A common function to execute callback functions for multiple events
 */
void XMC_SCU_IRQHandler(uint32_t sr_num)
{
  XMC_ASSERT("XMC_SCU_IRQHandler: Invalid sr_num", sr_num < SCU_IRQ_NUM);

  uint32_t index;
  XMC_SCU_INTERRUPT_EVENT_t event;
  XMC_SCU_INTERRUPT_EVENT_HANDLER_t event_handler;

  index = 0U;
  event = XMC_SCU_INTERUPT_GetEventStatus() & event_masks[sr_num];
  XMC_SCU_INTERRUPT_ClearEventStatus(event);

  while ((event != 0) && (index < XMC_SCU_INTERRUPT_EVENT_MAX))
  {
    if ((event & ((XMC_SCU_INTERRUPT_EVENT_t)1 << index)) != 0U)
    {
      event &= ~((XMC_SCU_INTERRUPT_EVENT_t)1 << index);
      event_handler = event_handler_list[index];
      if (event_handler != NULL)
      {
        event_handler();
      }

      /* break; XMC1: Only PULSE interrupts */
    }
    index++;
  }
}

/* Install BMI */
uint32_t XMC_SCU_SetBMI(uint32_t flags, uint8_t timeout)
{
#if (UC_SERIES == XMC14)
  return ROM_BmiInstallationReq((flags & 0x0fffU) | ((timeout << 12) & 0xf000U));
#else
  return ROM_BmiInstallationReq((flags & 0x07ffU) | ((timeout << 12) & 0xf000U) | 0x0800U);
#endif
}

#if (UC_SERIES == XMC14)
/* DCO1 clock frequency can be calibrated during runtime to achieve a better accuracy */
void XMC_SCU_CLOCK_EnableDCO1ExtRefCalibration(XMC_SCU_CLOCK_SYNC_CLKSRC_t sync_clk, uint32_t prescaler, uint32_t syn_preload)
{

  XMC_SCU_UnlockProtectedBits();
  SCU_ANALOG->ANASYNC2 = (uint16_t)(prescaler << SCU_ANALOG_ANASYNC2_PRESCALER_Pos);
  SCU_ANALOG->ANASYNC1 = (uint16_t)(syn_preload |
                         sync_clk |
                         SCU_ANALOG_ANASYNC1_SYNC_DCO_EN_Msk);

  XMC_SCU_LockProtectedBits();

}

/* This function stops the automatic DCO1 calibration based on the selected clock source */
void XMC_SCU_CLOCK_DisableDCO1ExtRefCalibration(void)
{
  XMC_SCU_UnlockProtectedBits();
  SCU_ANALOG->ANASYNC2 = 0U;
  SCU_ANALOG->ANASYNC1 = 0U;
  XMC_SCU_LockProtectedBits();
}

/* This functions checks the status of the synchronisation */
bool XMC_SCU_CLOCK_IsDCO1ExtRefCalibrationReady(void)
{
  return (bool)((SCU_ANALOG->ANASYNC2 & SCU_ANALOG_ANASYNC2_SYNC_READY_Msk) != 0U);
}

/**
 * This function enables the watchdog on the DCO1 frequency
 */
void XMC_SCU_CLOCK_EnableDCO1OscillatorWatchdog(void)
{
  SCU_CLK->OSCCSR |= SCU_CLK_OSCCSR_OWDEN_Msk;
}

/**
 * This function disables the watchdog on the DCO1 frequency
 */
void XMC_SCU_CLOCK_DisableDCO1OscillatorWatchdog(void)
{
  SCU_CLK->OSCCSR &= ~SCU_CLK_OSCCSR_OWDEN_Msk;
}

/**
 * This function clears the status of the watchdog on the DCO1 frequency
 */
void XMC_SCU_CLOCK_ClearDCO1OscillatorWatchdogStatus(void)
{
  SCU_CLK->OSCCSR |= SCU_CLK_OSCCSR_OWDRES_Msk;
}

/**
 * This function checks if the DCO1 frequency is in the limits of the watchdog.
 */
bool XMC_SCU_CLOCK_IsDCO1ClockFrequencyUsable(void)
{
  return ((SCU_CLK->OSCCSR & (SCU_CLK_OSCCSR_OSC2L_Msk | SCU_CLK_OSCCSR_OSC2H_Msk)) == 0U);
}

/* This function selects service request source for a NVIC interrupt node */
void XMC_SCU_SetInterruptControl(uint8_t irq_number, XMC_SCU_IRQCTRL_t source)
{
  XMC_ASSERT("XMC_SCU_SetInterruptControl: Invalid irq_number", irq_number < 32);
  XMC_ASSERT("XMC_SCU_SetInterruptControl: Invalid source", (source >> 8) == irq_number);

  source &= 0x3U;
  if (irq_number < 16U)
  {
    SCU_GENERAL->INTCR0 = (SCU_GENERAL->INTCR0 & ~(SCU_GENERAL_INTCR_INTSEL_Msk << (irq_number * SCU_GENERAL_INTCR_INTSEL_Size))) |
                          (source << (irq_number * SCU_GENERAL_INTCR_INTSEL_Size));
  }
  else
  {
    irq_number &= 0x0fU;
    SCU_GENERAL->INTCR1 = (SCU_GENERAL->INTCR1 & ~(SCU_GENERAL_INTCR_INTSEL_Msk << (irq_number  * SCU_GENERAL_INTCR_INTSEL_Size))) |
                          (source << (irq_number  * SCU_GENERAL_INTCR_INTSEL_Size));
  }
}

#endif
#endif /* UC_FAMILY == XMC1 */
