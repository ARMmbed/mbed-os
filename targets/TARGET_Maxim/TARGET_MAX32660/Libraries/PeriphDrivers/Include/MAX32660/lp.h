/**
 * @file    lp.h
 * @brief   Low power function prototypes and data types.
 */


/* ****************************************************************************
 * Copyright (C) Maxim Integrated Products, Inc., All Rights Reserved.
 *
 * Permission is hereby granted, free of charge, to any person obtaining a
 * copy of this software and associated documentation files (the "Software"),
 * to deal in the Software without restriction, including without limitation
 * the rights to use, copy, modify, merge, publish, distribute, sublicense,
 * and/or sell copies of the Software, and to permit persons to whom the
 * Software is furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included
 * in all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS
 * OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
 * MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
 * IN NO EVENT SHALL MAXIM INTEGRATED BE LIABLE FOR ANY CLAIM, DAMAGES
 * OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE,
 * ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR
 * OTHER DEALINGS IN THE SOFTWARE.
 *
 * Except as contained in this notice, the name of Maxim Integrated
 * Products, Inc. shall not be used except as stated in the Maxim Integrated
 * Products, Inc. Branding Policy.
 *
 * The mere transfer of this software does not imply any licenses
 * of trade secrets, proprietary technology, copyrights, patents,
 * trademarks, maskwork rights, or any other form of intellectual
 * property whatsoever. Maxim Integrated Products, Inc. retains all
 * ownership rights.
 *
 * $Date: 2018-09-26 08:48:30 -0500 (Wed, 26 Sep 2018) $
 * $Revision: 38105 $
 *
 *************************************************************************** */

// Define to prevent redundant inclusion
#ifndef _LP_H_
#define _LP_H_

/***** Includes *****/
#include "pwrseq_regs.h"

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @defgroup pwrseq Low Power (LP)
 * @ingroup periphlibs
 * @{
 */

/** @brief System reset0 enumeration. Used in SYS_PeriphReset0 function */
typedef enum {
    MXC_LP_OVR_0_9       = MXC_S_PWRSEQ_LP_CTRL_OVR_0_9V,         /**< Reset DMA */
    MXC_LP_OVR_1_0       = MXC_S_PWRSEQ_LP_CTRL_OVR_1_0V,         /**< Reset DMA */
    MXC_LP_OVR_1_1       = MXC_S_PWRSEQ_LP_CTRL_OVR_1_1V,         /**< Reset DMA */
} mxc_lp_ovr_t;

/**
 * @brief 	   Clears the low power wakeup flags
 */
void MXC_LP_ClearWakeStatus(void);

/**
 * @brief      Enables power to RAM addresses 0x20010000-0x20017FFF.
 */
void MXC_LP_EnableSRAM3(void);

/**
 * @brief      Enables power to RAM addresses 0x20008000-0x2000FFFF.
 */
void MXC_LP_EnableSRAM2(void);

/**
 * @brief      Enables power to RAM addresses 0x20004000-0x20007FFF.
 */
void MXC_LP_EnableSRAM1(void);

/**
 * @brief      Enables power to RAM addresses 0x20000000-0x20003FFF.
 */
void MXC_LP_EnableSRAM0(void);

/**
 * @brief      Disables power to RAM addresses 0x20010000-0x20017FFF. The contents of the RAM are destroyed.
 */
void MXC_LP_DisableSRAM3(void);

/**
 * @brief      Disables power to RAM addresses 0x20008000-0x2000FFFF. The contents of the RAM are destroyed.
 */
void MXC_LP_DisableSRAM2(void);

/**
 * @brief      Disables power to RAM addresses 0x20004000-0x20007FFF. The contents of the RAM are destroyed.
 */
void MXC_LP_DisableSRAM1(void);

/**
 * @brief      Disables power to RAM addresses 0x20000000-0x20003FFF. The contents of the RAM are destroyed.
 */
void MXC_LP_DisableSRAM0(void);

/**
 * @brief      Places the instruction cache in light sleep mode. Data will be unavailable for read/write operations but will be retained.
 */
void MXC_LP_EnableICacheLightSleep(void);

/**
 * @brief      Places addresses 0x20010000 to 0x20017FFF of the RAM in light sleep mode. Data will be unavailable for read/write operations but will be retained.
 */
void MXC_LP_EnableSysRAM3LightSleep(void);

/**
 * @brief      Places addresses 0x20008000 to 0x2000FFFF of the RAM in light sleep mode. Data will be unavailable for read/write operations but will be retained.
 */
void MXC_LP_EnableSysRAM2LightSleep(void);

/**
 * @brief      Places addresses 0x20004000 to 0x20007FFF of the RAM in light sleep mode. Data will be unavailable for read/write operations but will be retained.
 */
void MXC_LP_EnableSysRAM1LightSleep(void);

/**
 * @brief      Places addresses 0x20000000 to 0x20003FFF of the RAM in light sleep mode. Data will be unavailable for read/write operations but will be retained.
 */
void MXC_LP_EnableSysRAM0LightSleep(void);

/**
 * @brief      Places the instruction cache in active mode.
 */
void MXC_LP_DisableICacheLightSleep(void);

/**
 * @brief      Places addresses 0x20010000 to 0x20017FFF of the RAM in active mode.
 */
void MXC_LP_DisableSysRAM3LightSleep(void);

/**
 * @brief      Places addresses 0x20008000 to 0x2000FFFF of the RAM in active mode.
 */
void MXC_LP_DisableSysRAM2LightSleep(void);

/**
 * @brief      Places addresses 0x20004000 to 0x20007FFF of the RAM in active mode.
 */
void MXC_LP_DisableSysRAM1LightSleep(void);

/**
 * @brief      Places addresses 0x20000000 to 0x20003FFF of the RAM in active mode.
 */
void MXC_LP_DisableSysRAM0LightSleep(void);

/**
 * @brief      Enables the selected GPIO port and its selected pins to wake up the device from any low power mode.
 *             Call this function multiple times to enable pins on multiple ports.  This function does not configure
 *             the GPIO pins nor does it setup their interrupt functionality.
 * @param      port      The port to configure as wakeup sources.
 * @param      mask      The pins to configure as wakeup sources.
 */
void MXC_LP_EnableGPIOWakeup(unsigned int port, unsigned int mask);

/**
 * @brief      Disables the selected GPIO port and its selected pins as a wake up source.
 *             Call this function multiple times to disable pins on multiple ports.
 * @param      port      The port to configure as wakeup sources.
 * @param      mask      The pins to configure as wakeup sources.
 */
void MXC_LP_DisableGPIOWakeup(unsigned int port, unsigned int mask);

/**
 * @brief      Enables the RTC alarm to wake up the device from any low power mode.
 */
void MXC_LP_EnableRTCAlarmWakeup(void);

/**
 * @brief      Disables the RTC alarm from waking up the device.
 */
void MXC_LP_DisableRTCAlarmWakeup(void);

/**
 * @brief      Places the device into SLEEP mode.  This function returns once any interrupt occurs.
 * @note 	   MXC_LP_ClearWakeStatus should be called before this function, to avoid immediately waking up again
 */
void MXC_LP_EnterSleepMode(void);

/**
 * @brief      Places the device into DEEPSLEEP mode.  This function returns once an RTC or external interrupt occur.
 * @note      MXC_LP_ClearWakeStatus should be called before this function, to avoid immediately waking up again
*/
void MXC_LP_EnterDeepSleepMode(void);

/**
 * @brief      Places the device into BACKUP mode.  CPU state is not maintained in this mode, so this function never returns.
 *             Instead, the device will restart once an RTC or external interrupt occur.
 * @note       MXC_LP_ClearWakeStatus should be called before this function, to avoid immediately waking up again
 */
void MXC_LP_EnterBackupMode(void);

/**
 * @brief      Places the device into Shutdown mode.  CPU state is not maintained in this mode, so this function never returns.
 *             Instead, the device will restart once an RTC, USB wakeup, or external interrupt occur.
 */
void MXC_LP_EnterShutDownMode(void);

/**
 * @brief      Set operating voltage and change the clock to match the new voltage.
 * @param	   system reset configuration struct
 */
int MXC_LP_SetOperatingVoltage(mxc_lp_ovr_t ovr);

/**
 * @brief      Enables Data Retention to RAM addresses 0x20000000-0x20003FFF.
 */
void MXC_LP_EnableSRamRet0(void);

/**
 * @brief      Disables Data Retention to RAM addresses 0x20000000-0x20003FFF.
 */
void MXC_LP_DisableSRamRet0(void);

/**
 * @brief      Enables Data Retention to RAM addresses 0x20004000-0x20007FFF.
 */
void MXC_LP_EnableSRamRet1(void);

/**
 * @brief      Disables Data Retention to RAM addresses 0x20004000-0x20007FFF.
 */
void MXC_LP_DisableSRamRet1(void);

/**
 * @brief      Enables Data Retention to RAM addresses 0x20008000-0x2000FFFF.
 */
void MXC_LP_EnableSRamRet2(void);

/**
 * @brief      Disables Data Retention to RAM addresses 0x20008000-0x2000FFFF.
 */
void MXC_LP_DisableSRamRet2(void);

/**
 * @brief      Enables Data Retention to RAM addresses 0x20010000-0x20017FFF.
 */
void MXC_LP_EnableSRamRet3(void);

/**
 * @brief      Disables Data Retention to RAM addresses 0x20010000-0x20017FFF.
 */
void MXC_LP_DisableSRamRet3(void);

/**
 * @brief      Enables Bypassing the hardware detection of an external supply on V CORE enables a faster wakeup time.
 */
void MXC_LP_EnableBlockDetect(void);

/**
 * @brief      Disables Bypassing the hardware detection of an external supply on V CORE enables a faster wakeup time
 */
void MXC_LP_DisableBlockDetect(void);

/**
 * @brief      RAM Retention Regulator Enable for BACKUP Mode
 */
void MXC_LP_EnableRamRetReg(void);

/**
 * @brief      RAM Retention Regulator Disabels for BACKUP Mode
 */
void MXC_LP_DisableRamRetReg(void);

/**
 * @brief      Enables Fast wake up from deepsleep
 */
void MXC_LP_EnableFastWk(void);

/**
 * @brief      Disables Fast wake up from deepsleep
 */
void MXC_LP_DisableFastWk(void);

/**
 * @brief      Turns on band gap during deepsleep and backup mode.
 */
void MXC_LP_EnableBandGap(void);

/**
 * @brief      Turns off band gap during deepsleep and backup mode.
 */
void MXC_LP_DisableBandGap(void);

/**
 * @brief     Enables signal for power on reset when the device is int DEEPSLEEP or BACKUP mode
 */
void MXC_LP_EnableVCorePORSignal(void);

/**
 * @brief     Disables signal for power on reset when the device is int DEEPSLEEP or BACKUP mode
 */
void MXC_LP_DisableVCorePORSignal(void);

/**
 * @brief     Enables signal for power on reset when the device is int DEEPSLEEP or BACKUP mode
 */
void MXC_LP_EnableLDO(void);

/**
 * @brief     Disables signal for power on reset when the device is int DEEPSLEEP or BACKUP mode
 */
void MXC_LP_DisableLDO(void);

/**
 * @brief     Enables V CORE Supply Voltage Monitor
 */
void MXC_LP_EnableVCoreSVM(void);

/**
 * @brief     Disables V CORE Supply Voltage Monitor
 */
void MXC_LP_DisableVCoreSVM(void);


/**
 * @brief     Enables VDDIO Power-On-Reset Monitor
 */
void MXC_LP_EnableVDDIOPorMonitor(void);

/**
 * @brief     Disables VDDIO Power-On-Reset Monitor
 */
void MXC_LP_DisableVDDIOPorMonitor(void);
/**@} end of group pwrseq */

#ifdef __cplusplus
}
#endif

#endif /* _LP_H_ */
