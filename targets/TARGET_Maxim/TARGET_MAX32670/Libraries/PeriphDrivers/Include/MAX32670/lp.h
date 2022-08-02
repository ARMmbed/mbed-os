/**
 * @file    lp.h
 * @brief   Low Power(LP) function prototypes and data types.
 */

/* ****************************************************************************
 * Copyright (C) 2022 Maxim Integrated Products, Inc., All Rights Reserved.
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
 *************************************************************************** */

/* Define to prevent redundant inclusion */
#ifndef _LP_H_
#define _LP_H_

/* **** Includes **** */
#include <stdint.h>
#include "pwrseq_regs.h"
#include "mcr_regs.h"
#include "gcr_regs.h"
#include "gpio.h"
#include "tmr.h"

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @defgroup pwrseq Low Power (LP)
 * @ingroup periphlibs
 * @{
 */

/**
 * @brief   Enumeration type for voltage selection
 *
 */
typedef enum {
    MXC_LP_V0_9 = 0,
    MXC_LP_V1_0,
    MXC_LP_V1_1
} mxc_lp_ovr_t;

/**
 * @brief   Enumeration type for PM Mode
 *
 */
typedef enum {
    MXC_LP_IPO  = MXC_F_GCR_PM_IPO_PD,
    MXC_LP_IBRO = MXC_F_GCR_PM_IBRO_PD,
    MXC_LP_XRFO = MXC_F_GCR_PM_ERFO_PD
} mxc_lp_cfg_ds_pd_t;

/**
 * @brief      Places the device into SLEEP mode.  This function returns once an RTC or external interrupt occur.
 */
void MXC_LP_EnterSleepMode (void);

/**
 * @brief      Places the device into DEEPSLEEP mode.  This function returns once an RTC or external interrupt occur.
 */
void MXC_LP_EnterDeepSleepMode (void);

/**
 * @brief      Places the device into BACKUP mode.  CPU state is not maintained in this mode, so this function never returns.
 *             Instead, the device will restart once an RTC or external interrupt occur.
 */
void MXC_LP_EnterBackupMode (void);

/**
 * @brief      Places the device into Storage mode.  CPU state is not maintained in this mode, so this function never returns.
 *             Instead, the device will restart once an RTC or external interrupt occur.
 */
void MXC_LP_EnterStorageMode (void);

/**
 * @brief      Places the device into Shutdown mode.  CPU state is not maintained in this mode, so this function never returns.
 *             Instead, the device will restart once an RTC, USB wakeup, or external interrupt occur.
 */
void MXC_LP_EnterShutDownMode (void);

/**
 * @brief      Set ovr bits to set the voltage the micro will run at.
 *
 * @param[in]  ovr   The ovr options are only 0.9V, 1.0V, and 1.1V use enum mxc_lp_ovr_t
 */
void MXC_LP_SetOVR (mxc_lp_ovr_t ovr);

/**
 * @brief      Enable retention regulator
 */
void MXC_LP_RetentionRegEnable (void);

/**
 * @brief      Disable retention regulator
 */
void MXC_LP_RetentionRegDisable (void);

/**
 * @brief      Is the retention regulator enabled
 *
 * @return     1 = enabled 0 =  disabled
 */
int  MXC_LP_RetentionRegIsEnabled (void);

/**
 * @brief      Turn bandgap on
 */
void MXC_LP_BandgapOn (void);

/**
 * @brief      Turn bandgap off
 */
void MXC_LP_BandgapOff (void);

/**
 * @brief      Is the bandgap on or off
 *
 * @return     1 = bandgap on , 0 = bandgap off
 */
int MXC_LP_BandgapIsOn (void);

/**
 * @brief      Enable Power on Reset VDD Core Monitor
 */
void MXC_LP_PORVCOREoreMonitorEnable (void);

/**
 * @brief      Disable Power on Reset VDD Core Monitor
 */
void MXC_LP_PORVCOREoreMonitorDisable (void);

/**
 * @brief      Is Power on Reset VDD Core Monitor enabled
 *
 * @return     1 = enabled , 0 = disabled
 */
int MXC_LP_PORVCOREoreMonitorIsEnabled (void);

/**
 * @brief      Enable LDO
 */
void MXC_LP_LDOEnable (void);

/**
 * @brief      Disable LDO
 */
void MXC_LP_LDODisable (void);

/**
 * @brief      Is LDO enabled
 *
 * @return     1 = enabled , 0 = disabled
 */
int  MXC_LP_LDOIsEnabled (void);

/**
 * @brief      Enable Fast wakeup
 */
void MXC_LP_FastWakeupEnable (void);

/**
 * @brief      Disable Fast wakeup
 */
void MXC_LP_FastWakeupDisable (void);

/**
 * @brief      Is Fast wake up is Enabled
 *
 * @return     1 = enabled , 0 = disabled
 */
int  MXC_LP_FastWakeupIsEnabled (void);

/**
 * @brief      clear all wake up status
 */
void MXC_LP_ClearWakeStatus (void);

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
void MXC_LP_DisableGPIOWakeup (unsigned int port, unsigned int mask);

/**
 * @brief      Enables the RTC alarm to wake up the device from any low power mode.
 */
void MXC_LP_EnableRTCAlarmWakeup (void);

/**
 * @brief      Disables the RTC alarm from waking up the device.
 */
void MXC_LP_DisableRTCAlarmWakeup (void);

/**
 * @brief      Enables Timer to wakeup from any low power mode. 
 * 
 * @param      tmr  Pointer to timer module.
 */
void MXC_LP_EnableTimerWakeup(mxc_tmr_regs_t* tmr);

/**
 * @brief      Disables Timer from waking up device.  
 * 
 * @param      tmr  Pointer to timer module.
 */
void MXC_LP_DisableTimerWakeup(mxc_tmr_regs_t* tmr);

/**
 * @brief      Enables the USB to wake up the device from any low power mode.
 */
void MXC_LP_EnableUSBWakeup (void);

/**
 * @brief      Disables the USB from waking up the device.
 */
void MXC_LP_DisableUSBWakeup (void);

/**
 * @brief      Enables the HA0 to wake up the device from any low power mode.
 */
void MXC_LP_EnableHA0Wakeup (void);

/**
 * @brief      Disables the HA)0 from waking up the device.
 */
void MXC_LP_DisableHA0Wakeup (void);
/**
 * @brief      Enables the HA1 to wake up the device from any low power mode.
 */
void MXC_LP_EnableHA1Wakeup (void);

/**
 * @brief      Disables the HA1 from waking up the device.
 */
void MXC_LP_DisableHA1Wakeup (void);

/**
 * @brief      Configure which clocks are powered down at deep sleep and which are not affected.
 *
 * @note       Need to configure all clocks at once any clock not passed in the mask will be unaffected by Deepsleep.  This will
 *             always overwrite the previous settings of ALL clocks.
 *
 * @param[in]  mask  The mask of the clocks to power down when part goes into deepsleep
 *
 * @return     #E_NO_ERROR or error based on \ref MXC_Error_Codes
 */
int MXC_LP_ConfigDeepSleepClocks (uint32_t mask);

/**
 * @brief      Enable NFC Oscilator Bypass
 */
void MXC_LP_NFCOscBypassEnable (void);

/**
 * @brief      Disable NFC Oscilator Bypass
 */
void MXC_LP_NFCOscBypassDisable (void);

/**
 * @brief      Is NFC Oscilator Bypass Enabled
 *
 * @return     1 = enabled, 0 = disabled
 */
int MXC_LP_NFCOscBypassIsEnabled (void);

/**
 * @brief Enable System Ram 0 in light sleep
 */
void MXC_LP_SysRam0LightSleepEnable (void);

/**
 * @brief Enable System Ram 1 in light sleep
 */
void MXC_LP_SysRam1LightSleepEnable (void);

/**
 * @brief Enable System Ram 2 in light sleep
 */
void MXC_LP_SysRam2LightSleepEnable (void);

/**
 * @brief Enable System Ram 3 in light sleep
 */
void MXC_LP_SysRam3LightSleepEnable (void);

/**
 * @brief Enable System Ram 4 in light sleep
 */
void MXC_LP_SysRam4LightSleepEnable (void);

/**
 * @brief Enable System Ram 5 in light sleep
 */
void MXC_LP_SysRam5LightSleepEnable (void);

/**
 * @brief Enable Icache 0 in light sleep
 */
void MXC_LP_ICache0LightSleepEnable (void);

/**
 * @brief Enable Icache XIP in light sleep
 */
void MXC_LP_ICacheXIPLightSleepEnable (void);

/**
 * @brief Enable System Cache in light sleep
 */
void MXC_LP_SRCCLightSleepEnable (void);

/**
 * @brief Enable Crypto in light sleep
 */
void MXC_LP_CryptoLightSleepEnable (void);

/**
 * @brief Enable USB in light sleep
 */
void MXC_LP_USBFIFOLightSleepEnable (void);

/**
 * @brief Enable ROM 0 in light sleep
 */
void MXC_LP_ROMLightSleepEnable (void);

/**
 * @brief Disable System Ram 0 in light sleep
 */
void MXC_LP_SysRam0LightSleepDisable (void);

/**
 * @brief Disable System Ram 1 in light sleep
 */
void MXC_LP_SysRam1LightSleepDisable (void);

/**
 * @brief Disable System Ram 2 in light sleep
 */
void MXC_LP_SysRam2LightSleepDisable (void);

/**
 * @brief Disable System Ram 3 in light sleep
 */
void MXC_LP_SysRam3LightSleepDisable (void);

/**
 * @brief Disable System Ram 4 in light sleep
 */
void MXC_LP_SysRam4LightSleepDisable (void);

/**
 * @brief Disable System Ram 5 in light sleep
 */
void MXC_LP_SysRam5LightSleepDisable (void);

/**
 * @brief Disable Icache 0 in light sleep
 */
void MXC_LP_ICache0LightSleepDisable (void);

/**
 * @brief Disable Icache XIP in light sleep
 */
void MXC_LP_ICacheXIPLightSleepDisable (void);

/**
 * @brief Disable System Cache in light sleep
 */
void MXC_LP_SRCCLightSleepDisable (void);

/**
 * @brief Disable Crypto in light sleep
 */
void MXC_LP_CryptoLightSleepDisable (void);

/**
 * @brief Disable USB in light sleep
 */
void MXC_LP_USBFIFOLightSleepDisable (void);

/**
 * @brief Disable ROM 0 in light sleep
 */
void MXC_LP_ROMLightSleepDisable (void);

/**
 * @brief Shutdown System Ram 0
 */
void MXC_LP_SysRam0Shutdown (void);

/**
 * @brief Wakeup System Ram 0
 */
void MXC_LP_SysRam0PowerUp (void);

/**
 * @brief Shutdown System Ram 1
 */
void MXC_LP_SysRam1Shutdown (void);

/**
 * @brief PowerUp System Ram 1
 */
void MXC_LP_SysRam1PowerUp (void);

/**
 * @brief Shutdown System Ram 2
 */
void MXC_LP_SysRam2Shutdown (void);

/**
 * @brief PowerUp System Ram 2
 */
void MXC_LP_SysRam2PowerUp (void);

/**
 * @brief Shutdown System Ram 3
 */
void MXC_LP_SysRam3Shutdown (void);

/**
 * @brief PowerUp System Ram 3
 */
void MXC_LP_SysRam3PowerUp (void);

/**
 * @brief Shutdown System Ram 4
 */
void MXC_LP_SysRam4Shutdown (void);

/**
 * @brief PowerUp System Ram 4
 */
void MXC_LP_SysRam4PowerUp (void);

/**
 * @brief Shutdown System Ram 5
 */
void MXC_LP_SysRam5Shutdown (void);

/**
 * @brief PowerUp System Ram 5
 */
void MXC_LP_SysRam5PowerUp (void);

/**
 * @brief Shutdown Internal Cache
 */
void MXC_LP_ICache0Shutdown (void);

/**
 * @brief PowerUp Internal Cache
 */
void MXC_LP_ICache0PowerUp (void);

/**
 * @brief Shutdown Internal Cache XIP
 */
void MXC_LP_ICacheXIPShutdown (void);

/**
 * @brief PowerUp Internal Cache XIP
 */
void MXC_LP_ICacheXIPPowerUp (void);

/**
 * @brief Shutdown SRCC
 */
void MXC_LP_SRCCShutdown (void);

/**
 * @brief PowerUp SRCC
 */
void MXC_LP_SRCCPowerUp (void);

/**
 * @brief Shutdown USB FIFO
 */
void MXC_LP_USBFIFOShutdown (void);

/**
 * @brief PowerUp USB FIFO
 */
void MXC_LP_USBFIFOPowerUp (void);

/**
 * @brief Shutdown ROM
 */
void MXC_LP_ROMShutdown (void);

/**
 * @brief PowerUp ROM
 */
void MXC_LP_ROMPowerUp (void);

/**@} end of group pwrseq */

#ifdef __cplusplus
}
#endif

#endif /* _LP_H_ */
