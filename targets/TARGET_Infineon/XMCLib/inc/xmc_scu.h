/**
 * @file xmc_scu.h
 * @date 2016-03-09
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
 *     - Documentation improved <br>
 *     - XMC_ASSERT() hanging issues have fixed for XMC4 devices. <br>
 *
 * 2015-06-20:
 *     - Removed version macros and declaration of GetDriverVersion API
 *     - Removed STATIC_INLINE property for the below APIs and declared as void
 *       XMC_SCU_INTERRUPT_EnableEvent, XMC_SCU_INTERRUPT_DisableEvent,
 *       XMC_SCU_INTERRUPT_TriggerEvent, XMC_SCU_INTERUPT_GetEventStatus,
 *       XMC_SCU_INTERUPT_ClearEventStatus
 *
 * 2015-11-30:
 *     - Documentation improved <br>
 *      
 * 2016-03-09:
 *     - Optimization of write only registers
 *      
 * @endcond 
 *
 */
#ifndef XMC_SCU_H
#define XMC_SCU_H
 
/*********************************************************************************************************************
 * HEADER FILES
 ********************************************************************************************************************/
#include <xmc_common.h>

/**
 * @addtogroup XMClib XMC Peripheral Library
 * @{
 */
 
/**
 * @addtogroup SCU
 * @brief System Control Unit(SCU) driver for XMC microcontroller family.
 *
 * System control unit is the SoC power, reset and a clock manager with additional responsibility of
 * providing system stability protection and other auxiliary functions.<br>
 * SCU provides the following features,
 * -# Power control
 \if XMC4 
 * -# Hibernate control 
 \endif
 * -# Reset control
 * -# Clock control
 * -# Miscellaneous control(boot mode, system interrupts etc.)<br><br>
 *
 * The SCU driver is divided in to clock control logic, reset control logic, system interrupt control logic
 \if XMC4
 * , hibernate control logic, trap control logic, parity control logic 
 \endif
 * and miscellaneous control logic.<br>
 *
 * Clock driver features:
 * -# Allows clock configuration using the structure XMC_SCU_CLOCK_CONFIG_t and API XMC_SCU_CLOCK_Init()
 \if XMC4
 * -# Provides structure XMC_SCU_CLOCK_SYSPLL_CONFIG_t for configuring the system PLL
 * -# Allows selection of clock source for system PLL, XMC_SCU_CLOCK_GetSystemPllClockSource()
 * -# Provides APIs for configuring different module clock frequencies XMC_SCU_CLOCK_SetWdtClockDivider(), XMC_SCU_CLOCK_SetUsbClockDivider()
 * -# Allows selection of clock source for external output, XMC_SCU_CLOCK_SetExternalOutputClockSource()
 * -# Provides APIs for enabling external high power oscillator and ultra low power oscillator, XMC_SCU_CLOCK_EnableHighPerformanceOscillator(), XMC_SCU_CLOCK_EnableLowPowerOscillator()
 * -# Provides APIs for getting various clock frequencies XMC_SCU_CLOCK_GetPeripheralClockFrequency(), 
 XMC_SCU_CLOCK_GetCpuClockFrequency(), XMC_SCU_CLOCK_GetSystemClockFrequency()<br>
 \endif
 \if XMC1
 * -# Allows selection of peripheral clock frequency, XMC_SCU_CLOCK_SetFastPeripheralClockSource()
 * -# Provides API to get the peripheral clock frequency, XMC_SCU_CLOCK_GetFastPeripheralClockFrequency()
 \endif
 *
 * Reset driver features:
 \if XMC4
 * -# Allows to handle peripheral reset XMC_SCU_RESET_AssertPeripheralReset(), XMC_SCU_RESET_DeassertPeripheralReset()
 * -# Allows configuration of NMI generation for selected events, XMC_SCU_INTERRUPT_EnableNmiRequest()
 \endif
 \if XMC1
 * -# Allows to trigger device reset XMC_SCU_RESET_AssertMasterReset()
 * -# Allows to configure multiple sources for reset, XMC_SCU_RESET_EnableResetRequest()
 \endif <br>
 *
 * Interrupt driver features:
 * -# Provides APIs for enabling/ disabling interrupt event generation XMC_SCU_INTERRUPT_EnableEvent(), 
 XMC_SCU_INTERRUPT_DisableEvent()
 * -# Provides API for registering callback function for events XMC_SCU_INTERRUPT_SetEventHandler()<br>
 *
 \if XMC4
 * Hibernate driver features:
 * -# Allows configuration of hibernate domain XMC_SCU_HIB_EnableHibernateDomain(), XMC_SCU_HIB_DisableHibernateDomain()
 * -# Allows selection of standby clock source, XMC_SCU_HIB_SetStandbyClockSource()
 * -# Allows selection of RTC clock source, XMC_SCU_HIB_SetRtcClockSource()
 * -# Provides API for enabling slow internal clock used for backup clock, XMC_SCU_HIB_EnableInternalSlowClock()<br>
 *
 * Trap driver features:
 * -# Allows handling of trap XMC_SCU_TRAP_Enable(), XMC_SCU_TRAP_GetStatus(), XMC_SCU_TRAP_Trigger()<br>
 *
 * Parity driver features:
 * -# Parity error generated by on-chip RAM can be monitored, XMC_SCU_PARITY_Enable(), XMC_SCU_PARITY_GetStatus()
 * -# Allows configuration of trap generation on detection of parity error, XMC_SCU_PARITY_EnableTrapGeneration()
 *
 * Power driver features:
 * -# Allows to power the USB module XMC_SCU_POWER_EnableUsb(), XMC_SCU_POWER_DisableUsb()
 \endif
 *
 * Miscellaneous features:
 * -# Allows to trigger multiple capture compare unit(CCU) channels to be started together XMC_SCU_SetCcuTriggerHigh()
 \if XMC4
 * -# Enables configuration of out of range comparator (ORC) XMC_SCU_EnableOutOfRangeComparator()
 * -# Enables configuration of die temperature sensor XMC_SCU_EnableTemperatureSensor(), XMC_SCU_CalibrateTemperatureSensor()
 * -# Enables configuration of device boot mode XMC_SCU_SetBootMode()<br>
 \endif
 \if XMC1
 * -# Enables configuration of die temperature sensor XMC_SCU_StartTempMeasurement(), XMC_SCU_SetRawTempLimits()
 * -# Allows configuring supply monitor unit using the structure XMC_SCU_SUPPLYMONITOR_t and API XMC_SCU_SupplyMonitorInit()
 * -# Allows handling of protected bits XMC_SCU_LockProtectedBits(), XMC_SCU_UnlockProtectedBits()<br>
 \endif
 * @{
 */
 
/*********************************************************************************************************************
 * MACROS
 ********************************************************************************************************************/

/*********************************************************************************************************************
 * ENUMS
 ********************************************************************************************************************/
/**
 *  Defines the status of SCU API execution, used to verify the SCU related API calls.
 */
typedef enum XMC_SCU_STATUS 
{
  XMC_SCU_STATUS_OK   = 0UL, /**< SCU related operation successfully completed.*/
  XMC_SCU_STATUS_ERROR,      /**< SCU related operation failed. When API cannot fulfill request, this value is returned. */
  XMC_SCU_STATUS_BUSY,       /**< Cannot execute the SCU related operation request because
                                  another operation is in progress. \a XMC_SCU_STATUS_BUSY is returned when API is busy
                                  processing another request. */
} XMC_SCU_STATUS_t;


/*********************************************************************************************************************
 * DATA TYPES
 ********************************************************************************************************************/

/**
 * Function pointer type used for registering callback functions on SCU event occurrence.
 */
typedef void (*XMC_SCU_INTERRUPT_EVENT_HANDLER_t)(void);

/*********************************************************************************************************************
 * DEVICE EXTENSIONS
 ********************************************************************************************************************/

#if (UC_FAMILY == XMC1)
#include <xmc1_scu.h>
#elif (UC_FAMILY == XMC4)
#include <xmc4_scu.h>
#else
#error "Unspecified chipset"
#endif

/*********************************************************************************************************************
 * API Prototypes
 ********************************************************************************************************************/

#ifdef __cplusplus
extern "C" {
#endif


/**
 *
 * @param trigger    CCU slices to be triggered synchronously via software. The value is a bitmask of CCU slice bits
 *                    in the register CCUCON. <br>
 *                    \b Range: Use type @ref XMC_SCU_CCU_TRIGGER_t for bitmask of individual CCU slices. Multiple slices can be 
 *                    combined using \a OR operation.
 *
 * @return None
 *
 * \par<b>Description</b><br>
 * Generates active edge(low to high) trigger for multiple CCU units at the same time.\n\n
 * Before executing this API, all the required CCU timers should configure external start.
 * The edge of the start signal should be selected as active edge.
 * The input signal for the CCU slice should be selected as SCU input.
 * The above mentioned configurations can be made using the CCU LLD API XMC_CCU4_SLICE_StartConfig(). 
 * CCU timer slice should be started using XMC_CCU4_SLICE_StartTimer() before triggering 
 * the timer using this API.<BR>
 * \par<b>Related APIs:</b><BR>
 * XMC_CCU4_SLICE_StartConfig(), XMC_CCU4_SLICE_SetInput(), XMC_SCU_SetCcuTriggerLow()\n\n\n
 */
__STATIC_INLINE void XMC_SCU_SetCcuTriggerHigh(const uint32_t trigger)
{
  SCU_GENERAL->CCUCON |= (uint32_t)trigger;
}

/**
 *
 * @param trigger  CCU slices to be triggered synchronously via software. The value is a bitmask of CCU slice bits
 *                    in the register CCUCON. <br>
 *                    \b Range: Use type @ref XMC_SCU_CCU_TRIGGER_t for bitmask of individual CCU slices. Multiple slices can be 
 *                    combined using \a OR operation.
 *
 * @return None
 *
 * \par<b>Description</b><br>
 * Generates passive edge(high to low) trigger for multiple CCU units at the same time.\n\n
 * Before executing this API, all the required CCU timers should configure external start.
 * The edge of the start signal should be selected as passive edge.
 * The input signal for the CCU slice should be selected as SCU input.
 * The above mentioned configurations can be made using the CCU LLD API XMC_CCU4_SLICE_StartConfig(). 
 * CCU timer slice should be started using XMC_CCU4_SLICE_StartTimer() before triggering 
 * the timer using this API.<BR>
 * \par<b>Related APIs:</b><BR>
 * XMC_CCU4_SLICE_StartConfig(), XMC_CCU4_SLICE_SetInput(), XMC_SCU_SetCcuTriggerHigh()\n\n\n
 */
__STATIC_INLINE void XMC_SCU_SetCcuTriggerLow(const uint32_t trigger)
{
  SCU_GENERAL->CCUCON &= (uint32_t)~trigger;
}

/**
 *
 * @param config Pointer to structure holding the clock prescaler values and divider values for 
 *                configuring clock generators and clock tree.\n
 *                \b Range: Configure the members of structure @ref XMC_SCU_CLOCK_CONFIG_t for various
 *                parameters of clock setup.
 *
 * @return None
 *
 * \par<b>Description</b><br>
 * Initializes clock generators and clock tree.\n\n
 * \if XMC1
 * Peripheral clock and system clock are configured based on the input configuration \a config.
 * The system clock frequency is tuned by configuring the FDIV and IDIV values of CLKCR register.
 * The values of FDIV and IDIV can be provided as part of input configuration.
 * The PCLK divider determines the ratio of peripheral clock to the system clock. 
 * The source of RTC clock is set based on the input configuration. 
 * \a SystemCoreClock variable will be updated with the value of
 * system clock frequency. Access to protected bit fields are handled internally.
 * \endif
 * \if XMC4
 * Enables the high precision oscillator(fOHP) input and configures the system and peripheral clock frequencies.
 * Based on the system clock source selected in \a config, either fPLL or fOFI will be chosen as system clock.
 * Based on PLL mode(normal or prescaler mode) used, PLL ramps up in steps to achieve target frequency.
 * The clock dividers for CPU, CCU and peripheral clocks will be set based on the input configuration.
 * The \a SystemCoreClock variable is set with the value of system clock frequency.
 * \endif
 * \par<b>Related APIs:</b><BR>
 * XMC_SCU_CLOCK_GetPeripheralClockFrequency(), XMC_SCU_CLOCK_GetCpuClockFrequency() \n\n\n
 */
void XMC_SCU_CLOCK_Init(const XMC_SCU_CLOCK_CONFIG_t *const config);

/**
 *
 * @param event  Bit mask of the event to enable. \b Range: Use type @ref XMC_SCU_INTERRUPT_EVENT_t
 *                for providing the input value. Multiple events can be combined using the \a OR operation.
 *
 * @return None
 *
 * \par<b>Description</b><br>
 * Enables the generation of interrupt for the input events.\n\n
 * The events are enabled by setting the respective bit fields in the SRMSK register. \n
 * Note: User should separately enable the NVIC node responsible for handling the SCU interrupt.
 * The interrupt will be generated when the respective event occurs.
 * \par<b>Related APIs:</b><BR>
 * NVIC_EnableIRQ(), XMC_SCU_INTERRUPT_DisableEvent()\n\n\n
 */
void XMC_SCU_INTERRUPT_EnableEvent(const XMC_SCU_INTERRUPT_EVENT_t event);


/**
 *
 * @param event  Bit mask of the event to disable. \b Range: Use type @ref XMC_SCU_INTERRUPT_EVENT_t
 *                for providing the input value. Multiple events can be combined using the \a OR operation.
 *
 * @return None
 *
 * \par<b>Description</b><br>
 * Disables generation of interrupt on occurrence of the input event.\n\n
 * The events are disabled by resetting the respective bit fields in the SRMSK register. \n
 * \par<b>Related APIs:</b><BR>
 * NVIC_DisableIRQ(), XMC_SCU_INTERRUPT_EnableEvent()\n\n\n
 */
void XMC_SCU_INTERRUPT_DisableEvent(const XMC_SCU_INTERRUPT_EVENT_t event);

/**
 *
 * @param event Bit mask of the event to be triggered. \b Range: Use type @ref XMC_SCU_INTERRUPT_EVENT_t
 *                for providing the input value. Multiple events can be combined using the \a OR operation.
 *
 * @return None
 *
 * \par<b>Description</b><br>
 * Triggers the event as if the hardware raised it.\n\n
 * Event will be triggered by setting the respective bitfield in the SRSET register.\n
 * Note: User should enable the NVIC node that handles the respective event for interrupt generation.
 * \par<b>Related APIs:</b><BR>
 * NVIC_EnableIRQ(), XMC_SCU_INTERUPT_GetEventStatus(), XMC_SCU_INTERRUPT_ClearEventStatus() \n\n\n
 */
void XMC_SCU_INTERRUPT_TriggerEvent(const XMC_SCU_INTERRUPT_EVENT_t  event);

/**
 * @return uint32_t  Status of the SCU events.
 *
 * \par<b>Description</b><br>
 * Provides the status of all SCU events.\n\n
 * The status is read from the SRRAW register. To check the status of a particular 
 * event, the returned value should be masked with the bit mask of the event. The bitmask
 * of events can be obtained using the type @ref XMC_SCU_INTERRUPT_EVENT_t. Multiple events'
 * status can be checked by combining the bit masks using \a OR operation. 
 * After detecting the event, the event status should be cleared using software to detect the event again.
 * \par<b>Related APIs:</b><BR>
 * XMC_SCU_INTERRUPT_ClearEventStatus(), XMC_SCU_INTERRUPT_TriggerEvent(), XMC_SCU_INTERRUPT_SetEventHandler() \n\n\n
 */
XMC_SCU_INTERRUPT_EVENT_t XMC_SCU_INTERUPT_GetEventStatus(void);

/**
 *
 * @param event  Bit mask of the events to clear. \b Range: Use type @ref XMC_SCU_INTERRUPT_EVENT_t
 *                for providing the input value. Multiple events can be combined using the \a OR operation.
 *
 * @return None
 *
 * \par<b>Description</b><br>
 * Clears the event status bit in SRRAW register.\n\n
 * The events are cleared by writing value 1 to their bit positions in the SRCLR register.
 * The API can be used when polling method is used. After detecting the event, the event status
 * should be cleared using software to detect the event again.
 *
 * \par<b>Related APIs:</b><BR>
 * XMC_SCU_INTERUPT_GetEventStatus(), XMC_SCU_INTERRUPT_TriggerEvent() \n\n\n
 */
void XMC_SCU_INTERRUPT_ClearEventStatus(const XMC_SCU_INTERRUPT_EVENT_t event);

/**
 *
 * @return uint32_t  Status representing the reason for device reset.
 *
 * \par<b>Description</b><br>
 * Provides the value representing the reason for device reset.\n\n
 * The return value is an encoded word, which can indicate multiple reasons for the last reset. Each bit position of the
 * returned word is representative of a last reset cause. The returned value should be appropriately masked to check
 * the cause of reset. 
 * The cause of the last reset gets automatically stored in 
 * the \a SCU_RSTSTAT register. The reset status shall be reset after each
 * startup in order to ensure consistent source indication after the next reset.
 * \b Range: The type @ref XMC_SCU_RESET_REASON_t can be used to get the bit masks of the reset cause.
 *
 * \par<b>Related APIs:</b><BR>
 * XMC_SCU_RESET_ClearDeviceResetReason() \n\n\n
 */
__STATIC_INLINE uint32_t XMC_SCU_RESET_GetDeviceResetReason(void)
{
  return ((SCU_RESET->RSTSTAT) & SCU_RESET_RSTSTAT_RSTSTAT_Msk);
}
/**
 * @return None 
 *
 * \par<b>Description</b><br>
 * Clears the reset reason bits in the reset status register. \n\n
 * Clearing of the reset status information in the \a SCU_RSTSTAT register via register bit \a RSTCLR.RSCLR is strongly
 * recommended to ensure a clear indication of the cause of next reset.
 *
 * \par<b>Related APIs:</b><BR>
 * XMC_SCU_RESET_GetDeviceResetReason() \n\n\n
 */
__STATIC_INLINE void XMC_SCU_RESET_ClearDeviceResetReason(void)
{
  /* Clear RSTSTAT.RSTSTAT bitfield */
  SCU_RESET->RSTCLR = (uint32_t)SCU_RESET_RSTCLR_RSCLR_Msk;
}
 
/**
 * @return uint32_t  Value of CPU clock frequency.
 *
 * \par<b>Description</b><br>
 * Provides the vlaue of CPU clock frequency.\n\n
 * The value is stored in a global variable \a \b SystemCoreClock.
 * It is updated when the clock configuration is done using the SCU LLD APIs.
 * The value represents the frequency of clock used for CPU operation.
 * \b Range: Value is of type uint32_t, and gives the value of frequency in Hertz.
 *
 * \par<b>Related APIs:</b><BR>
 * XMC_SCU_CLOCK_GetPeripheralClockFrequency(), XMC_SCU_CLOCK_GatePeripheralClock() \n\n\n
 */
__STATIC_INLINE uint32_t XMC_SCU_CLOCK_GetCpuClockFrequency(void)
{
  return SystemCoreClock;
}

/**
 * @return uint32_t  Value of peripheral clock frequency in Hertz.
 *
 * \par<b>Description</b><br>
 * Provides the vlaue of clock frequency at which the peripherals are working.\n\n
 * The value is derived from the CPU frequency. \b Range: Value is of type uint32_t. It is represented in Hertz.
 * \par<b>Related APIs:</b><BR>
 * XMC_SCU_CLOCK_GetCpuClockFrequency(),XMC_SCU_CLOCK_GatePeripheralClock() \n\n\n
 */
uint32_t XMC_SCU_CLOCK_GetPeripheralClockFrequency(void); 

#if(UC_SERIES != XMC45)
 
/**
 *
 * @param  peripheral The peripheral for which the clock has to be gated. \b Range: Use type @ref XMC_SCU_PERIPHERAL_CLOCK_t
 *                    to identify the peripheral clock to be gated.
 *
 * @return None
 *
 * \par<b>Description</b><br>
 * Blocks the supply of clock to the selected peripheral.\n\n
 * Clock gating helps in reducing the power consumption. User can selectively gate the clocks of unused peripherals.
 * \if XMC1
 * fPCLK is the source of clock to various peripherals. Some peripherals support clock gate. Such a gate blocks
 * the clock supply for the selected peripheral.
 * Software can request for individual gating of such peripheral clocks by enabling the \a SCU_CGATSET0
 * register bit field. Every bit in \a SCU_CGATSET0 register is protected by the bit protection scheme. Access to protected
 * bit fields are handled internally.
 * \endif
 * \if XMC4
 * fPERI is the source of clock to various peripherals. Some peripherals support clock gate. Such a gate blocks
 * the clock supply for the selected peripheral.
 * Software can request for individual gating of such peripheral clocks by enabling one of the \a
 * SCU_CGATSET0, \a SCU_CGATSET1 or \a SCU_CGATSET2 register bitfields.
 * 
 * \endif
 * Note: Clock gating shall not be activated unless the module is in reset state. So use \a
 * XMC_SCU_CLOCK_IsPeripheralClockGated() API before enabling the gating of any peripheral.
 * \par<b>Related APIs:</b><BR>
 * XMC_SCU_CLOCK_IsPeripheralClockGated(), XMC_SCU_CLOCK_UngatePeripheralClock() \n\n\n
 */
void XMC_SCU_CLOCK_GatePeripheralClock(const XMC_SCU_PERIPHERAL_CLOCK_t peripheral);
 
/**
 *
 * @param  peripheral  The peripheral for which the clock has to be ungated. \b Range: Use type @ref XMC_SCU_PERIPHERAL_CLOCK_t
 *                    to identify the peripheral.
 *
 * @return None
 *
 * \par<b>Description</b><br>
 * Enables the supply of clock to the selected peripheral.\n\n
 * By default when the device powers on, the peripheral clock will be gated for the 
 * peripherals that support clock gating.
 * The peripheral clock should be enabled before using it for any functionality.
 * \if XMC1
 * fPCLK is the source of clock to various peripherals. Some peripherals support clock gate.
 * Software can request for individual ungating of such peripheral clocks by setting respective bits
 * in the \a SCU_CGATCLR0 register.
 * \endif
 * \if XMC4
 * fPERI is the source of clock to various peripherals. Some peripherals support clock gate.
 * Software can request for individual ungating of such peripheral clocks by setting the respective bits in one of \a
 * SCU_CGATCLR0, \a SCU_CGATCLR1 or \a SCU_CGATCLR2 registers.
 * \endif
 *
 * \par<b>Related APIs:</b><BR>
 * XMC_SCU_CLOCK_IsPeripheralClockGated(), XMC_SCU_CLOCK_GatePeripheralClock() \n\n\n
 */
void XMC_SCU_CLOCK_UngatePeripheralClock(const XMC_SCU_PERIPHERAL_CLOCK_t peripheral);

/**
 *
 * @param  peripheral  The peripheral for which the check for clock gating has to be done. 
 *                    \b Range: Use type @ref XMC_SCU_PERIPHERAL_CLOCK_t to identify the peripheral.
 *
 * @return bool Status of the peripheral clock gating. \b Range: true if the peripheral clock is gated.
 *              false if the peripheral clock ungated(gate de-asserted).
 *
 * \par<b>Description</b><br>
 * Gives the status of peripheral clock gating.\n\n
 * \if XMC1
 * Checks the status of peripheral clock gating using the register CGATSTAT0.
 * \endif
 * \if XMC4
 * Checks the status of peripheral clock gating using one of CGATSTAT0, CGATSTAT1 or CGATSTAT2 registers.
 * \endif
 * It is recommended to use this API before
 * enabling the gating of any peripherals through \a XMC_SCU_CLOCK_GatePeripheralClock() API.
 *
 * \par<b>Related APIs:</b><BR>
 * XMC_SCU_CLOCK_UngatePeripheralClock(), XMC_SCU_CLOCK_GatePeripheralClock() \n\n\n
 */
bool XMC_SCU_CLOCK_IsPeripheralClockGated(const XMC_SCU_PERIPHERAL_CLOCK_t peripheral);
#endif


/**
 * @return  uint32_t  Status of the register mirror update.\n
 *                    \b Range: Use the bit mask of the SCU_GENERAL_MIRRSTS register for the mirror update event of 
 *                    interest. e.g.: SCU_GENERAL_MIRRSTS_RTC_CTR_Msk. Multiple update events can be combined 
 *                    using \a OR operation.
 *
 * \par<b>Description</b><br>
 * Provides the status of hibernate domain register update, when the respective mirror registers are changed. \n\n
 * The hibernate domain is connected to the core domain via SPI serial communication. MIRRSTS is a status register 
 * representing the communication of changed value of a mirror register to its corresponding register in the 
 * hibernate domain. The bit fields of the register indicate
 * that a corresponding register of the hibernate domain is ready to accept a write or that the communication interface
 * is busy with executing the previous operation.\n
 * Note: There is no hibernate domain in XMC1x devices. This register is retained for legacy purpose.
 */
__STATIC_INLINE uint32_t XMC_SCU_GetMirrorStatus(void)
{
  return(SCU_GENERAL->MIRRSTS);
}

/**
 * @param event The event for which the interrupt handler is to be configured. \n
 *        \b Range: Use type @ref XMC_SCU_INTERRUPT_EVENT_t for identifying the event.
 * @param handler Name of the function to be executed when the event if detected. \n
 *          \b Range: The function accepts no arguments and returns no value.
 * @return XMC_SCU_STATUS_t Status of configuring the event handler function for the selected event.\n
 *          \b Range: \a XMC_SCU_STATUS_OK if the event handler is successfully configured.\n
 *            \a XMC_SCU_STATUS_ERROR if the input event is invalid.\n
 * \par<b>Description</b><br>
 * Assigns the event handler function to be executed on occurrence of the selected event.\n\n
 * If the input event is valid, the handler function will be assigned to a table to be executed
 * when the interrupt is generated and the event status is set in the event status register. By using this API,
 * polling for a particular event can be avoided. This way the CPU utilization will be optimized. Multiple SCU events
 * can generate a common interrupt. When the interrupt is generated, a common interrupt service routine is executed.
 * It checks for status flags of events which can generate the interrupt. The handler function will be executed if the 
 * event flag is set.
 *
 * \par<b>Related APIs:</b><BR>
 * XMC_SCU_INTERRUPT_TriggerEvent(), XMC_SCU_INTERUPT_GetEventStatus() \n\n\n
 */
XMC_SCU_STATUS_t XMC_SCU_INTERRUPT_SetEventHandler(const XMC_SCU_INTERRUPT_EVENT_t event, const XMC_SCU_INTERRUPT_EVENT_HANDLER_t handler);

/**
 * @param sr_num Service request number identifying the SCU interrupt generated.\n
 *                \b Range: 0 to 2. XMC4x devices have one common SCU interrupt, so the value should be 0.\n
 *                                  But XMC1x devices support 3 interrupt nodes.
 * @return None
 * \par<b>Description</b><br>
 * A common function to execute callback functions for multiple events.\n\n
 * It checks for the status of events which can generate the interrupt with the selected service request.
 * If the event is set, the corresponding callback function will be executed. It also clears the event status bit.\n
 * \b Note: This is an internal function. It should not be called by the user application.
 *
 * \par<b>Related APIs:</b><BR>
 * XMC_SCU_INTERRUPT_SetEventHandler() \n\n\n
 */
void XMC_SCU_IRQHandler(uint32_t sr_num);

#ifdef __cplusplus
}
#endif

/**
 * @}
 */
 
/**
 * @}
 */
 
#endif /* SCU_H */
