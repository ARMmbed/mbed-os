/**
  ******************************************************************************
  * @file    sleep.h
  * @author  AMG - RF Application team
  * @version V1.1.0
  * @date    27-March-2018
  * @brief   Header file for BlueNRG Sleep management
  ******************************************************************************
  * @attention
  *
  * THE PRESENT FIRMWARE WHICH IS FOR GUIDANCE ONLY AIMS AT PROVIDING CUSTOMERS
  * WITH CODING INFORMATION REGARDING THEIR PRODUCTS IN ORDER FOR THEM TO SAVE
  * TIME. AS A RESULT, STMICROELECTRONICS SHALL NOT BE HELD LIABLE FOR ANY
  * DIRECT, INDIRECT OR CONSEQUENTIAL DAMAGES WITH RESPECT TO ANY CLAIMS ARISING
  * FROM THE CONTENT OF SUCH FIRMWARE AND/OR THE USE MADE BY CUSTOMERS OF THE
  * CODING INFORMATION CONTAINED HEREIN IN CONNECTION WITH THEIR PRODUCTS.
  *
  * <h2><center>&copy; COPYRIGHT 2018 STMicroelectronics</center></h2>
  ******************************************************************************
  */
/**
 * @file  sleep.h
 * @brief BlueNRG Sleep management
 * <!-- Copyright 2018 by STMicroelectronics.  All rights reserved.       *80*-->

* \section Library BlueNRG-1, BlueNRG-2 Low Power management 

  - The BlueNRG-1, BlueNRG-2 low power software combines the low power requests coming from 
    the application with the radio operating mode, choosing the best low power mode 
    applicable in the current scenario.
    - This negotiation between the radio module and the application requests prevents data loss
    and it is performed by the low power software APIs. User is requested to call the BlueNRG_Sleep(SleepModes sleepMode, uint8_t gpioWakeBitMask, uint8_t gpioWakeLevelMask) API.
    - Refer to AN4820 "BlueNRG-1 and BlueNRG-2 low power modes" document for detailed description about the devices HW low power modes and SW support. 

* \section  Sleep_API How BlueNRG_Sleep() API works?  

  - BlueNRG_Sleep(SleepModes sleepMode, uint8_t gpioWakeBitMask, uint8_t gpioWakeLevelMask) must be 
    called on main application while loop, with selected sleep mode (SLEEPMODE_RUNNING, SLEEPMODE_CPU_HALT,SLEEPMODE_WAKETIMER,SLEEPMODE_NOTIMER),
    gpio bit masks and level for selecting the external wakup source if applicable.
    - BlueNRG_Sleep() API sets the actual sleep mode value (sleepMode_allowed variable) as combination of following components: 
        - BlueNRG_Sleep() sleepMode parameter value
        - user sleep mode provided within App_SleepMode_Check() function
	- BLE Stack sleep mode returned from BlueNRG_Stack_Perform_Deep_Sleep_Check() function.

      - sleepMode_allowed value determines actual sleep mode and associated actions:
        - SLEEPMODE_RUNNING: 
          - It just reenables global interrupt operations (nothing else to be done);
        - SLEEPMODE_CPU_HALT: 
          - __WFI() is called: it executes a wait for interrupt instruction and suspends the core execution until one of a number of events occurs.
          - Global interrupt operations are re-enabled. 
        - SLEEPMODE_WAKETIMER, SLEEPMODE_NOTIMER:
          - BlueNRG_InternalSleep(sleepMode_allowed, gpioWakeBitMask) is called, after wakeup sources setup. 
     
   * \section Sleep_Modes  How BlueNRG_InternalSleep() API works? (SLEEPMODE_WAKETIMER, SLEEPMODE_NOTIMER sleep modes)   

      - When the BlueNRG-1 and BlueNRG-2 exit from SLEEPMODE_WAKETIMER, SLEEPMODE_NOTIMER low power mode a reset occurs: all the
          peripherals configuration and the application context are lost.
        - The BlueNRG_InternalSleep() API implements a mechanism to save and restore all the peripheral configuration and the application context when a power save procedure is called.

        - BlueNRG_InternalSleep(sleepMode_allowed, gpioWakeBitMask) performs the following main operations in order to enter in sleep mode (SLEEPMODE_WAKETIMER, SLEEPMODE_NOTIMER)  and restore context:
          - Save the peripherals registers configurations on dedicated variables: System Control, FLASH CONFIG, NVIC, CKGEN, GPIO, UART, SPI, I2C, RNG, RNG, RTC, MFTX, SysTick, WDT, DMA, ADC, PKA.
         
          - Enable deep sleep: SystemSleepCmd(ENABLE);
         
          - Save the context by calling the calling the CS_contextSave() function on context_switch.s file.

        - When BlueNRG-1 and BlueNRG-2 exit from SLEEPMODE_WAKETIMER, SLEEPMODE_NOTIMER low power mode due to a wakeup reason, a reset occurs. The following main operations are done: 
          - On reset the __low_level_init() function is called which checks the reset reason:
             - Since the reset reason is a wakeup from sleep, it restores the context by calling the CS_contextRestore() function on context_switch.s file.  
                             
          - Disable deep sleep (SystemSleepCmd(DISABLE))
           
          - Restore the peripherals registers configuration previously saved on dedicated variables  FLASH CONFIG, NVIC, CKGEN, UART, SPI, I2C, RNG, RNG, RTC, MFTX, SysTick, WDT, DMA, ADC, PKA.
         
          - NOTE: From the application point of view, the exit from a low power procedure is fully transparent: when a wakeup from low power occurs, the CPU executes the next instruction after the low power function call.

  * \section Wakeup_WFI How to properly handle device entering in the Sleep mode with a wakeup source active?

      If the user application wants to enter in sleep mode (SLEEPMODE_WAKETIMER or SLEEPMODE_NOTIMER) and starts the sleep procedure with a wakeup source
      already active, the system does not switch the power management off and remains blocked in the WFI instruction until an interrupt occurs. In this context, if other wakeup sources, set in the BlueNRG_Sleep() API, occur, they are ignored. The only way to restart the code execution is through an interrupt occurring from any peripheral. 
      So, it is mandatory  to enable, at NVIC level, the interrupts associated with the wakeup sources.

      - Example: let's assume the wakeup source is IO13 and no specific interrupt is enabled and associated to this IO at application level. 
                 User must enable, at NVIC level, the interrupt on IO13.

        - while (1) {
             BTLE_StackTick();

              wakeup_source = WAKEUP_IO13;
              wakeup_level = (WAKEUP_IO_LOW << WAKEUP_IO13);  

              <enable IO13 interrupt>

              BlueNRG_Sleep(SLEEPMODE_NOTIMER, wakeup_source, wakeup_level, 0);

              <disable IO13 interrupt>
            }

  * \section Wakeup_Reason How to properly handle a wakeup source from a wakeup GPIO (IO9, 10,11,12,13) for which an interrupt is also expected ?

      When in sleep mode, if a wakeup with associated IRQ handler occurs on a wakeup IO, the device is waked up, but the related IO user operations on associated IRQ handler cannot be properly handled.
      This is due to the fact that an event on such IO allows to wake up the device in sleep mode, but, since the GPIO peripheral is in sleep mode, the associated IT pending bit is not set. 
      As consequence, on associated GpioHandler(), user application is not able to correctly check the IT pending bit and perform the related operations. 

      After existing from sleep mode, the following checks on associated IO handler (GpioHandler()) must be performed:

      - use wakeupFromSleepFlag flag to verify that device actually exits from sleep mode and check if the wakeup source is the one associated to the specific IO (by using BlueNRG_WakeupSource() API). 

      - Example: let's assume the wakeup source is IO12 and user application requires to perform specific operations on associated IRQ function (GPIO_handler()).

          - On main application user enables the  sleep mode by setting IO12 as wakeup source: 
              
            BlueNRG_Sleep(SLEEPMODE_WAKETIMER,WAKEUP_I12,WAKEUP_IOx_LOW<< WAKEUP_I12_SHIFT_MASK);    

          - On BlueNRG1_it.c, the following changes must be applied on GPIO_Handler() function:

             void GPIO_Handler(void)
             {
                if ((GPIO_GetITPendingBit(GPIO_Pin_12) == SET) || ((wakeupFromSleepFlag) && (BlueNRG_WakeupSource() == WAKEUP_IO12)))
                {
                    GPIO_ClearITPendingBit(GPIO_Pin_12);

                    ... //User code to be executed when IRQ on IO12 occurs
                }
             }
    
**/

#ifndef __SLEEP_H__
#define __SLEEP_H__

#include "BlueNRG_x_device.h"

/** @brief Enumerations for the possible microcontroller sleep modes.
 * - SLEEPMODE_RUNNING
 *     Everything is active and running.  In practice this mode is not
 *     used, but it is defined for completeness of information.
 * - SLEEPMODE_CPU_HALT
 *     Only the CPU is halted.  The rest of the chip continues running
 *     normally.  The chip will wake from any interrupt.
 * - SLEEPMODE_WAKETIMER
 *     The device is in deep sleep and the timer clock sources remain running. 
 *     Wakeup is possible from both GPIO and the Hal Virtual Timers.
 * - SLEEPMODE_NOTIMER
 *     The device is in deep sleep. All the peripherals and clock sources are turned off.
 *     Wakeup is possible only from GPIOs IO9-IO10-IO11-IO12-IO13.
 */
typedef enum {
  SLEEPMODE_RUNNING       = 0,
  SLEEPMODE_CPU_HALT      = 1,
  SLEEPMODE_WAKETIMER     = 2,
  SLEEPMODE_NOTIMER       = 3,
} SleepModes; 

/** @brief Wakeup source IO9 Mask */
#define WAKEUP_IO9   0x01

/** @brief Wakeup source IO10 Mask */
#define WAKEUP_IO10  0x02

/** @brief Wakeup source IO11 Mask */
#define WAKEUP_IO11  0x04

/** @brief Wakeup source IO12 Mask */
#define WAKEUP_IO12  0x08

/** @brief Wakeup source IO13 Mask */
#define WAKEUP_IO13  0x10

/** @brief Wakeup source Blue block Sleep Timer 1 */
#define WAKEUP_SLEEP_TIMER1  0x20

/** @brief Wakeup source Blue block Sleep Timer 2 */
#define WAKEUP_SLEEP_TIMER2  0x40

/** @brief The Reset reason is not caused from a wakeup source but from a BOR reset */
#define WAKEUP_BOR 0xF0

/** @brief The Reset reason is not caused from a wakeup source but from a POR reset */
#define WAKEUP_POR 0xF1

/** @brief The Reset reason is not caused from a wakeup source but from a System reset request */
#define WAKEUP_SYS_RESET_REQ 0xF2

/** @brief The Reset reason is not caused from a wakeup source but from a watchdog timer reset */
#define WAKEUP_RESET_WDG 0xF3

/** @brief The Reset reason is not caused from a known wakeup source */
#define NO_WAKEUP_RESET 0xFF

/** @brief The system wakes up when IOx level is low */
#define WAKEUP_IOx_LOW 0x00

/** @brief The system wakes up when IOx level is high */
#define WAKEUP_IOx_HIGH 0x01

/** @brief IO9 wake up shift mask */
#define WAKEUP_IO9_SHIFT_MASK  0x00

/** @brief IO10 wake up shift mask */
#define WAKEUP_IO10_SHIFT_MASK  0x01

/** @brief IO11 wake up shift mask */
#define WAKEUP_IO11_SHIFT_MASK  0x02

/** @brief IO12 wakeup shift mask */
#define WAKEUP_IO12_SHIFT_MASK  0x03

/** @brief IO13 wake up shift mask */
#define WAKEUP_IO13_SHIFT_MASK 0x04

/** 
 * @brief This function allows to put the BlueNRG in low power state.
 *
 * This function allows to enter a desired sleep mode that is negotiated between BTLE stack needs and application needs.
 * The application can provide the desired sleep mode using the parameter sleepMode. In addition to this, the application can
 * optionally use the @ref App_SleepMode_Check to provide its desired sleep mode.
 * The function will compute the sleep mode by combining the different requests and choosing the lowest possible power mode.
 *
 * The device can be configured with different wake sources
 * according to the sleepMode parameter. The lowest power mode is obtained configuring the device in 
 * SLEEPMODE_NOTIMER.
 * For all sleep modes, BlueNRG_Sleep() will return when the wakeup occours.
 * 
 * @param sleepMode Sleep mode (see SleepModes enum)
 * 
 * @param gpioWakeBitMask  A bit mask of the GPIO that are allowed to wake
 * the chip from deep sleep.  A high bit in the mask will enable waking
 * the chip if the corresponding GPIO changes state.  bit0 is IO9, bit1 is
 * IO10, bit2 is IO11, bit3 is IO12, bit4 is IO13, bits[5:7] are ignored.
 * If this param is 0 the wakeup GPIO are ignored
 * @param gpioWakeLevelMask  A mask used to setup the active wakeup level:
 * - 0: the system wakes up when IO is low
 * - 1: the system wakes up when IO is high
 * The level sensitive bit mask is the same of the gpioWakeBitMask parameter.
 * 
 * @retval Status of the call
 */
uint8_t BlueNRG_Sleep(SleepModes sleepMode, 
                      uint8_t gpioWakeBitMask, 
                      uint8_t gpioWakeLevelMask);


/** 
 * @brief This function allows the application to define its desired sleep mode.
 * 
 * The App_SleepMode_Check allows the application to set its desired sleep mode based on the application power management policy.
 * When user calls @ref BlueNRG_Sleep, a negotiation occurs to define the sleep mode and this function is called to get
 * inputs from application.
 * It is important to notice that this function is executed with interrupts disabled
 *
 * @param sleepMode Sleep mode (see SleepModes enum)
 *
 * @retval Return the sleep mode possible and safe from the application point of view
 *
 * @note A weak implementation always returning SLEEPMODE_NOTIMER is provided as default
 * when no application specifc behaviour is required.
 */

SleepModes App_SleepMode_Check(SleepModes sleepMode);


/**
 * @brief Return the wakeup source from reset. Possible value are:
 * - WAKEUP_IO9
 * - WAKEUP_IO10
 * - WAKEUP_IO11
 * - WAKEUP_IO12
 * - WAKEUP_IO13
 * - WAKEUP_SLEEP_TIMER1
 * - WAKEUP_SLEEP_TIMER2
 * - WAKEUP_BOR
 * - WAKEUP_POR
 * - WAKEUP_SYS_RESET_REQ
 * - WAKEUP_RESET_WDG
 * - NO_WAKEUP_RESET
 */ 
uint8_t BlueNRG_WakeupSource(void);

#endif
