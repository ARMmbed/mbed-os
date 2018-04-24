/**
 * @file xmc_wdt.h
 * @date 2015-08-06
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
 *     - Documentation updates <br>
 *
 * 2015-06-20:
 *     - Removed version macros and declaration of GetDriverVersion API <br>
 *
 * 2015-08-06:
 *     - Bug fix in XMC_WDT_SetDebugMode() API, Wrong register is being configured.<br>
 * @endcond
 */

#ifndef XMC_WDT_H
#define XMC_WDT_H

/*********************************************************************************************************************
 * HEADER FILES
 ********************************************************************************************************************/

#include "xmc_common.h"
#include "xmc_scu.h"
/**
 * @addtogroup XMClib XMC Peripheral Library
 * @{
 */

/**
 * @addtogroup WDT
 * @brief Watchdog driver for the XMC microcontroller family.
 *
 * The watchdog unit (WDT) improves the system integrity, by triggering the system reset request to bring the system 
 * back from the unresponsive state to normal operation.
 * 
 * This LLD provides the Configuration structure XMC_WDT_CONFIG_t and initialization function XMC_WDT_Init().\n
 * It can be used to:
 * -# Start or Stop the watchdog timer. (XMC_WDT_Start() and XMC_WDT_Stop())
 * -# Service the watchdog timer. (XMC_WDT_Service())
 * -# Configure the service window upper bound and lower bound timing values. (XMC_WDT_SetWindowBounds())
 * -# Enable the generation of the pre-warning event for the first overflow of the timer. (XMC_WDT_SetMode())
 * -# Clear the pre-warning alarm event. It is mandatory to clear the flag during pre-warning alarm ISR, to stop 
      generating reset request for the second overflow of the timer. (XMC_WDT_ClearAlarm())
 * -# Suspend the watchdog timer during Debug HALT mode. (XMC_WDT_SetDebugMode())
 * -# Configure service indication pulse width.(XMC_WDT_SetServicePulseWidth())
 * 
 * @{
 */

/*********************************************************************************************************************
 * MACROS
  ********************************************************************************************************************/

#define XMC_WDT_MAGIC_WORD             (0xABADCAFEU) /* Magic word to be written in Service Register (SRV), 
                                                          to service or feed the watchdog. */

/*********************************************************************************************************************
 * ENUMS
  ********************************************************************************************************************/

/**
 *  Defines working modes for watchdog. Use type XMC_WDT_MODE_t for this enum.
 */
typedef enum XMC_WDT_MODE 
{
  XMC_WDT_MODE_TIMEOUT    = (uint32_t)0x0 << WDT_CTR_PRE_Pos, /**< Generates reset request as soon as the timer overflow 
                                                                   occurs. */
  XMC_WDT_MODE_PREWARNING = (uint32_t)0x1 << WDT_CTR_PRE_Pos  /**< Generates an alarm event for the first overflow. And 
                                                                   reset request after subsequent overflow, if not 
                                                                   serviced after first overflow. */
} XMC_WDT_MODE_t;

/**
 *  Defines debug behaviour of watchdog when the CPU enters HALT mode. Use type XMC_WDT_DEBUG_MODE_t for this enum.
 */
typedef enum XMC_WDT_DEBUG_MODE 
{
  XMC_WDT_DEBUG_MODE_STOP = (uint32_t)0x0 << WDT_CTR_DSP_Pos, /**< Watchdog counter is paused during debug halt. */
  XMC_WDT_DEBUG_MODE_RUN  = (uint32_t)0x1 << WDT_CTR_DSP_Pos  /**< Watchdog counter is not paused during debug halt. */
} XMC_WDT_DEBUG_MODE_t;

/*********************************************************************************************************************
 * DATA STRUCTURES
  ********************************************************************************************************************/
 /* Anonymous structure/union guard start */
#if defined(__CC_ARM)
  #pragma push
  #pragma anon_unions
#elif defined(__TASKING__)
  #pragma warning 586
#endif
 
/**
 * Structure for initializing watchdog timer. Use type XMC_WDT_CONFIG_t for this structure.
 */
typedef struct XMC_WDT_CONFIG
{
  uint32_t window_upper_bound; /**< Upper bound for service window (WUB). Reset request is generated up on overflow of 
                                    timer. ALways upper bound value has to be more than lower bound value. If it is set
                                    lower than WLB, triggers a system reset after timer crossed upper bound value.\n
                                    Range: [0H to FFFFFFFFH] */
  uint32_t window_lower_bound; /**< Lower bound for servicing window (WLB). Setting the lower bound to 0H disables the 
                                    window mechanism.\n 
                                    Range: [0H to FFFFFFFFH] */
  union
  {
    struct
    {
      uint32_t : 1;
      uint32_t prewarn_mode : 1;        /**< Pre-warning mode (PRE). This accepts boolean values as input. */
      uint32_t : 2;
      uint32_t run_in_debug_mode : 1;   /**< Watchdog timer behaviour during debug (DSP). This accepts boolean values as input. */
      uint32_t : 3;
      uint32_t service_pulse_width : 8; /**< Service Indication Pulse Width (SPW). Generated Pulse width is of (SPW+1), 
                                             in fwdt cycles.\n
                                             Range: [0H to FFH] */
      uint32_t : 16;
    };
    uint32_t wdt_ctr; /* Value of operation mode control register (CTR). It’s bit fields are represented by above 
                           union members. */
  };
} XMC_WDT_CONFIG_t;
/* Anonymous structure/union guard end */
#if defined(__CC_ARM)
  #pragma pop
#elif defined(__TASKING__)
  #pragma warning restore
#endif
/*********************************************************************************************************************
 * API PROTOTYPES
  ********************************************************************************************************************/

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @param None
 *
 * @return None
 *
 * \par<b>Description:</b><br>
 * \if XMC4
 * Enables watchdog clock and releases watchdog reset.\n
 * \endif
 * \if XMC1
 * Enables watchdog clock.\n
 * \endif
 * \par
 * This API is invoked by XMC_WDT_Init() and therefore no need to call it explicitly during watchdog initialization
 * sequence. Invoke this API to enable watchdog once again if the watchdog is disabled by invoking XMC_WDT_Disable().
 * 
 * \par<b>Note:</b><br>
 * \if XMC4
 * 1. It is required to configure the watchdog, again after invoking XMC_WDT_Disable(). Since all the registers are
 *    reset with default values.
 * \endif
 * \if XMC1
 * 1. Not required to configure the watchdog again after invoking XMC_WDT_Disable(). Since the registers retains with
 *    the configured values.
 * \endif
 * \par<b>Related APIs:</b><BR>
 * XMC_WDT_Init(), XMC_WDT_Disable()
 */
void XMC_WDT_Enable(void);

/**
 * @param None
 *
 * @return None
 *
 * \par<b>Description:</b><br>
 * \if XMC4
 * Disables the clock and resets watchdog timer.\n
 * \endif
 * \if XMC1
 * Disables the clock to the watchdog timer.\n
 * \endif
 *
 * \par<b>Note:</b><br>
 * \if XMC4
 * 1. Resets the registers with default values. So XMC_WDT_Init() has to be invoked again to configure the watchdog.
 * \endif
 * \if XMC1
 * 1. After invoking XMC_WDT_Disable(), all register values are displayed with 0F in debugger. Once enabled by 
      calling XMC_WDT_Enable(), previous configured register values are displayed. No need to invoke XMC_WDT_Init()
      again.
 * \endif
 * \par<b>Related APIs:</b><BR>
 * XMC_WDT_Enable()
 */
void XMC_WDT_Disable(void);

/**
 * @param config pointer to a constant watchdog configuration data structure. Refer data structure XMC_WDT_CONFIG_t
 *               for detail.
 *
 * @return None
 *
 * \par<b>Description:</b><br>
 * Initializes and configures watchdog with configuration data pointed by \a config.\n
 * \par
 * It invokes XMC_WDT_Enable() to enable clock and release reset. Then configures the lower and upper window bounds,
 * working mode (timeout/pre-warning), debug behaviour and service request indication pulse width.
 * 
 * \par<b>Note:</b><br> 
 * 1. With out invoking this XMC_WDT_Init() or XMC_WDT_Enable(), invocation of other APIs like XMC_WDT_SetWindowBounds(),
 *    XMC_WDT_SetMode(), XMC_WDT_SetServicePulseWidth(), XMC_WDT_SetDebugMode(), XMC_WDT_Start(), XMC_WDT_GetCounter(),
 *    XMC_WDT_Service(), XMC_WDT_ClearAlarm() has no affect.
 */
void XMC_WDT_Init(const XMC_WDT_CONFIG_t *const config);

/**
 * @param lower_bound specifies watchdog window lower bound in terms of watchdog clock (fWDT) cycles. 
 *                    Range: [0H to FFFFFFFFH].
 * @param upper_bound specifies watchdog window upper bound in terms of watchdog clock (fWDT) cycles. 
 *                    Range: [0H to FFFFFFFFH]. 
 *
 * @return None
 *
 * \par<b>Description:</b><br>
 * Sets watchdog window lower and upper bounds by updating WLB and WUB registers.\n
 * \par
 * Window lower and upper bounds are set during initialization in XMC_WDT_Init(). Invoke this API to alter the values as
 * needed later in the program. This upper bound and lower bound can be calculated by using the below formula\n
 *       upper_bound or lower_bound = desired_boundary_time(sec) * fwdt(hz)
 *
 * \par<b>Note:</b>
 * 1. Always ensure that upper_bound is greater than the lower_bound value. If not, whenever timer crosses the 
 *    upper_bound value it triggers the reset(wdt_rst_req) of the controller.
 */
__STATIC_INLINE void XMC_WDT_SetWindowBounds(uint32_t lower_bound, uint32_t upper_bound)
{
  WDT->WLB = lower_bound;
  WDT->WUB = upper_bound;
}

/**
 * @param mode is one of the working modes of the watchdog timer, i.e timeout or pre-warning. Refer @ref XMC_WDT_MODE_t
 *             for valid values.
 *
 * @return None
 *
 * \par<b>Description:</b><br>
 * Sets watchdog working mode (timeout or pre-warning) by updating PRE bit of CTR register.\n
 * \par
 * The working mode is set during initialization in XMC_WDT_Init(). Invoke this API to alter the mode as needed later in
 * the program.
 */
__STATIC_INLINE void XMC_WDT_SetMode(XMC_WDT_MODE_t mode)
{
  WDT->CTR = (WDT->CTR & (uint32_t)~WDT_CTR_PRE_Msk) | (uint32_t)mode;
}

/**
 * @param service_pulse_width specifies Service indication pulse width in terms of fwdt. 
 *                            Range: [0H – FFH].
 * @return None
 *
 * \par<b>Description:</b><br>
 * Sets service indication pulse width by updating SPW bit field of CTR register.\n
 * \par
 * The service indication pulse (with width service_pulse_width + 1 in fwdt cycles) is generated on successful servicing
 * or feeding of watchdog. The pulse width is initially set during initialization in XMC_WDT_Init(). Invoke this API to
 * alter the width as needed later in the program.
 */
__STATIC_INLINE void XMC_WDT_SetServicePulseWidth(uint8_t service_pulse_width)
{
  WDT->CTR = (WDT->CTR & (uint32_t)~WDT_CTR_SPW_Msk) | ((uint32_t)service_pulse_width << WDT_CTR_SPW_Pos);
}

/**
 * @param debug_mode running state of watchdog during debug halt mode. Refer @ref XMC_WDT_DEBUG_MODE_t for 
 *                   valid values.
 *
 * @return None
 *
 * \par<b>Description:</b><br>
 * Sets debug behaviour of watchdog by modifying DSP bit of CTR register.\n
 * \par
 * Depending upon DSP bit, the watchdog timer stops when CPU is in HALT mode. The debug behaviour is initially set as 
 * XMC_WDT_DEBUG_MODE_STOP during initialization in XMC_WDT_Init(). Invoke this API to change the debug behaviour as 
 * needed later in the program.
 */
__STATIC_INLINE void XMC_WDT_SetDebugMode(const XMC_WDT_DEBUG_MODE_t debug_mode)
{
  WDT->CTR = (WDT->CTR & (uint32_t)~WDT_CTR_DSP_Msk) | (uint32_t)debug_mode;
}

/**
 * @param None
 *
 * @return None
 *
 * \par<b>Description:</b><br>
 * Start the watchdog timer by setting ENB bit of CTR register.\n
 * \par
 * Invoke this API to start the watchdog after initialization, or to resume the watchdog when
 * paused by invoking XMC_WDT_Stop().
 *
 * \par<b>Related APIs:</b><BR>
 * XMC_WDT_Init(), XMC_WDT_Stop()
 */
__STATIC_INLINE void XMC_WDT_Start(void)
{
  WDT->CTR |= (uint32_t)WDT_CTR_ENB_Msk;
}

/**
 * @param None
 *
 * @return None
 *
 * \par<b>Description:</b><br>
 * Pauses watchdog timer by resetting ENB bit of CTR register.\n
 * \par
 * Invoke this API to pause the watchdog as needed in the program e.g. debugging through software control.
 *
 * \par<b>Related APIs:</b><BR>
 * XMC_WDT_Init(), XMC_WDT_Stop()
 */
__STATIC_INLINE void XMC_WDT_Stop(void)
{
  WDT->CTR &= (uint32_t)~WDT_CTR_ENB_Msk;
}

/**
 * @param None
 *
 * @return uint32_t Current count value of watchdog timer register (TIM).
 *                  Range: [0H to FFFFFFFFH]
 *
 * \par<b>Description:</b><br>
 * Reads current count of timer register (TIM).\n
 * \par
 * Invoke this API before servicing or feeding the watchdog to check whether count is between lower and upper
 * window bounds.
 *
 * \par<b>Related APIs:</b><BR>
 * XMC_WDT_Service()
 */
__STATIC_INLINE uint32_t XMC_WDT_GetCounter(void)
{
  return WDT->TIM;
}

/**
 * @param None
 *
 * @return None
 *
 * \par<b>Description:</b><br>
 * Services or feeds the watchdog by writing the Magic word in SRV register.\n
 * \par
 * Service watchdog when count value of watchdog timer is between lower and upper window bounds. Successful servicing
 * will reset watchdog timer (TIM register) to 0H and generate service indication pulse.
 *
 * \par<b>Note:</b><br>
 * 1. invoking this API when count value of watchdog timer is less than window lower bound results
 *    wrong servicing and immediately triggers reset request.
 *
 * \par<b>Related APIs:</b><BR>
 * XMC_WDT_GetCounter(), XMC_WDT_SetWindowBounds(), XMC_WDT_SetServicePulseWidth()
 */
__STATIC_INLINE void XMC_WDT_Service(void)
{
  WDT->SRV = XMC_WDT_MAGIC_WORD;
}

/**
 * @param None
 *
 * @return None
 *
 * \par<b>Description:</b><br>
 * Clears pre-warning alarm by setting ALMC bit in WDTCLR register.\n
 * \par
 * In pre-warning mode, first overflow of the timer upper window bound fires the pre-warning alarm. XMC_WDT_ClearAlarm() 
 * must be invoked to clear the alarm alarm. After clearing of the alarm, watchdog timer must be serviced within valid 
 * time window. Otherwise watchdog timer triggers the reset request up on crossing the upper bound value in a subsequent
 * cycle.
 *
 * \par<b>Related APIs:</b><BR>
 * XMC_WDT_Service(), XMC_WDT_SetMode()
 */
__STATIC_INLINE void XMC_WDT_ClearAlarm(void)
{
  WDT->WDTCLR = WDT_WDTCLR_ALMC_Msk;
}

#ifdef __cplusplus
}
#endif

/**
 * @}
 */

/**
 * @}
 */

#endif /* XMC_WDT_H */
