/*************************************************************************************************/
/*!
 *  \file   main_board.h
 *
 *  \brief  Board services.
 *
 *  Copyright (c) 2015 ARM, Ltd., all rights reserved.
 *  ARM confidential and proprietary.
 *
 *  IMPORTANT.  Your use of this file is governed by a Software License Agreement
 *  ("Agreement") that must be accepted in order to download or otherwise receive a
 *  copy of this file.  You may not use or copy this file for any purpose other than
 *  as described in the Agreement.  If you do not agree to all of the terms of the
 *  Agreement do not use this file and delete all copies in your possession or control;
 *  if you do not have a copy of the Agreement, you must contact ARM, Ltd. prior
 *  to any use, copying or further distribution of this software.
 */
/*************************************************************************************************/

#ifndef __MAIN_BOARD_H
#define __MAIN_BOARD_H

/***************************************************************************************************
** INCLUDES
***************************************************************************************************/

#include "chip.h"
#include "board.h"

#include "wsf_types.h"

#ifdef __cplusplus
extern "C" {
#endif

/***************************************************************************************************
** TYPES
***************************************************************************************************/

/*--------------------------------------------------------------------------------------------------
** enum main_board_mode
**
** DESCRIPTION: Mode of application on board, if detectable at run-time.
--------------------------------------------------------------------------------------------------*/
enum main_board_mode {
    MAIN_BOARD_MODE_DTM                   = 0,
    MAIN_BOARD_MODE_BEACON                = 1,
    MAIN_BOARD_MODE_SENSOR                = 2,
    MAIN_BOARD_MODE_BEACON_NONCONNECTABLE = 3
};

/*--------------------------------------------------------------------------------------------------
** enum main_board_settings
**
** DESCRIPTION: Settings for board, stored in NV data.  These values should not be changed once
**              used.
--------------------------------------------------------------------------------------------------*/
enum main_board_settings {
    /* Major number (for beacon) [2 bytes] */
    MAIN_BOARD_SETTING_MAJOR = 0,
    /* Minor number (for beacon) [2 bytes] */
    MAIN_BOARD_SETTING_MINOR = 1,
    /* RSSI reference (for beacon) [2 bytes] */
    MAIN_BOARD_SETTING_RSSI_REFERENCE = 2,
    /* HCI logging level [1 byte] */
    MAIN_BOARD_SETTING_HCI_LOGGING_LEVEL = 3,
    /* Lock state (for UriBeacon) [1 byte] */
    MAIN_BOARD_SETTING_LOCK_STATE = 4,
    /* URI data (for UriBeacon) [18 bytes] */
    MAIN_BOARD_SETTING_URI_DATA = 5,
    /* URI flags (for UriBeacon) [1 byte] */
    MAIN_BOARD_SETTING_URI_FLAGS = 6,
    /* Advertised tx power levels (for UriBeacon) [4 bytes] */
    MAIN_BOARD_SETTING_ADVERTISED_TX_POWER_LEVELS = 7,
    /* Tx power mode (for UriBeacon) [1 byte] */
    MAIN_BOARD_SETTING_TX_POWER_MODE = 8,
    /* Beacon period (for beacon) [2 bytes] */
    MAIN_BOARD_SETTING_BEACON_PERIOD = 9,
    /* Lock [16 bytes] */
    MAIN_BOARD_SETTING_LOCK = 10,
    /* PIN [3 bytes] */
    MAIN_BOARD_SETTING_PIN = 11,
    /* Beacon type [1 byte] */
    MAIN_BOARD_SETTING_BEACON_TYPE = 12,
};

/*--------------------------------------------------------------------------------------------------
** enum main_board_muxmode
**
** DESCRIPTION: Settings for MUX mode of MCU.
--------------------------------------------------------------------------------------------------*/
enum main_board_muxmode {
    MAIN_BOARD_MUXMODE_NORMAL = 0,
    MAIN_BOARD_MUXMODE_DIGITAL_TEST = 1,
};

/***************************************************************************************************
** FUNCTIONS
***************************************************************************************************/

/*--------------------------------------------------------------------------------------------------
** Main_BoardInit()
**
** DESCRIPTION: Initialize board.
--------------------------------------------------------------------------------------------------*/
void Main_BoardInit(void);

/*--------------------------------------------------------------------------------------------------
** Main_BoardI2cInit()
**
** DESCRIPTION: Initialize I2C devices on board.
--------------------------------------------------------------------------------------------------*/
#ifdef BOARD_HAS_I2C
void Main_BoardI2cInit(void);
#endif

/*--------------------------------------------------------------------------------------------------
** Main_BoardGetMode()
**
** DESCRIPTION: Get board mode.
**
** RETURNS:     Board mode.
--------------------------------------------------------------------------------------------------*/
enum main_board_mode Main_BoardGetMode(void);

/*--------------------------------------------------------------------------------------------------
** Main_BoardNvGetSize()
**
** DESCRIPTION: Called to get size of NV memory.
**
** RETURNS:     Size of NV memory
**------------------------------------------------------------------------------------------------*/
#ifdef BOARD_HAS_SPIFLASH
size_t Main_BoardNvGetSize(void);
#endif /* BOARD_HAS_SPIFLASH */

/*--------------------------------------------------------------------------------------------------
** Main_BoardNvRead()
**
** DESCRIPTION: Called to read NV data.
**
** PARAMETERS:  addr    Address from which to read
**              buf     Pointer to buffer to receive data
**              length  Number of bytes to read
**
** RETURNS:     Number of bytes read
**------------------------------------------------------------------------------------------------*/
#ifdef BOARD_HAS_SPIFLASH
size_t Main_BoardNvRead(size_t addr, uint8_t *buf, size_t length);
#endif /* BOARD_HAS_SPIFLASH */

/*--------------------------------------------------------------------------------------------------
** Main_BoardNvWrite()
**
** DESCRIPTION: Called to write NV data.
**
** PARAMETERS:  addr    Address at which to write
**              buf     Pointer to buffer with data to write
**              length  Number of bytes to write
**
** RETURNS:     Number of bytes written
**------------------------------------------------------------------------------------------------*/
#ifdef BOARD_HAS_SPIFLASH
size_t Main_BoardNvWrite(size_t addr, const uint8_t *buf, size_t length);
#endif /* BOARD_HAS_SPIFLASH */

/*--------------------------------------------------------------------------------------------------
** Main_BoardNvErase()
**
** DESCRIPTION: Called to erase NV data.
**
** RETURNS:     0 or error (if not 0)
**------------------------------------------------------------------------------------------------*/
#ifdef BOARD_HAS_SPIFLASH
int32_t Main_BoardNvErase(void);
#endif /* BOARD_HAS_SPIFLASH */

/*--------------------------------------------------------------------------------------------------
** Main_BoardSettingsInit()
**
** DESCRIPTION: Initialize settings stored in a host blob in the NV data.
**
** RETURNS:     0 or error (if not 0)
**------------------------------------------------------------------------------------------------*/
#ifdef BOARD_HAS_SPIFLASH
int32_t Main_BoardSettingsInit(void);
#endif /* BOARD_HAS_SPIFLASH */

/*--------------------------------------------------------------------------------------------------
** Main_BoardSettingsClear()
**
** DESCRIPTION: Clear settings stored in a host blob in the NV data.
**
** RETURNS:     0 or error (if not 0)
**------------------------------------------------------------------------------------------------*/
#ifdef BOARD_HAS_SPIFLASH
int32_t Main_BoardSettingsClear(void);
#endif /* BOARD_HAS_SPIFLASH */

/*--------------------------------------------------------------------------------------------------
** Main_BoardSettingRead()
**
** DESCRIPTION: Read a setting stored in a host blob in the NV data.
**
** PARAMETERS:  id      ID of the setting
**              buf     Pointer to buffer that will receive setting value
**              length  Length of the buffer; if setting is longer, only this many bytes will be
**                      stored
**
** RETURNS:     length of the setting or error (if less than or equal to zero)
**------------------------------------------------------------------------------------------------*/
#ifdef BOARD_HAS_SPIFLASH
int32_t Main_BoardSettingRead(uint8_t id, uint8_t *buf, uint8_t length);
#endif /* BOARD_HAS_SPIFLASH */

/*--------------------------------------------------------------------------------------------------
** Main_BoardSettingWrite()
**
** DESCRIPTION: Write a setting stored in a host blob in the NV data.
**
** PARAMETERS:  id      ID of the setting
**              buf     Pointer to buffer with setting value
**              length  Length of the buffer; if setting is longer, only this many bytes will be
**                      read
**
** RETURNS:     length of the setting or error (if less than or equal to zero)
**------------------------------------------------------------------------------------------------*/
#ifdef BOARD_HAS_SPIFLASH
int32_t Main_BoardSettingWrite(uint8_t id, const uint8_t *buf, uint8_t length);
#endif /* BOARD_HAS_SPIFLASH */

/*--------------------------------------------------------------------------------------------------
** Main_BoardSetPmuFeature()
**
** DESCRIPTION: Set PMU feature of MCU.
**
** PARAMETERS:  req  Requested feature(s).
**------------------------------------------------------------------------------------------------*/
void Main_BoardSetPmuFeature(uint8_t req);

/*--------------------------------------------------------------------------------------------------
** Main_BoardGetPmuFeature()
**
** DESCRIPTION: Get PMU feature of MCU.
**
** RETURNS:     Requested feature(s).
**------------------------------------------------------------------------------------------------*/
uint8_t Main_BoardGetPmuFeature(void);

/*--------------------------------------------------------------------------------------------------
** Main_BoardSetMuxMode()
**
** DESCRIPTION: Set MUX mode of MCU.
**
** PARAMETERS:  mode  Mode to set
**------------------------------------------------------------------------------------------------*/
void Main_BoardSetMuxMode(uint8_t mode);

/*--------------------------------------------------------------------------------------------------
** Main_BoardWakeLockInc()
**
** DESCRIPTION: Increment counter to keep MCU from entering deep sleep.
**------------------------------------------------------------------------------------------------*/
void Main_BoardWakeLockInc(void);

/*--------------------------------------------------------------------------------------------------
** Main_BoardWakeLockDec()
**
** DESCRIPTION: Decrement counter to allow MCU to enter deep sleep.
**------------------------------------------------------------------------------------------------*/
void Main_BoardWakeLockDec(void);

/*--------------------------------------------------------------------------------------------------
** Main_Board32kTimerSleep()
**
** DESCRIPTION: Deep sleep for a certain number of ticks of the 32k timer.
**
** PARAMETERS:  ticks  Number of ticks to sleep; 0 to sleep forever
**              deep   TRUE if sleep should be deep sleep
**------------------------------------------------------------------------------------------------*/
void Main_Board32kTimerSleep(uint32_t ticks, bool_t deep);

/*--------------------------------------------------------------------------------------------------
** Main_Board32kTimerRead()
**
** DESCRIPTION: Read the 32k timer.
**
** Returns:     The timer value
**------------------------------------------------------------------------------------------------*/
uint32_t Main_Board32kTimerRead(void);

/*--------------------------------------------------------------------------------------------------
** Main_BoardSetWakeupCallback()
**
** DESCRIPTION: Set callback for wakeup interrupt.
**------------------------------------------------------------------------------------------------*/
#ifdef BOARD_HAS_WAKEUP
void Main_BoardSetWakeupCback(void (*wakeupCback)(void));
#endif

/*--------------------------------------------------------------------------------------------------
** Main_BoardWatchdogTimeout()
**
** DESCRIPTION: Set watchdog to expire in certain number of milliseconds, after which the processor
**              will reset.
**
** PARAMETERS:  to_ms  Milliseconds before timeout; 0 to timeout immediately
**------------------------------------------------------------------------------------------------*/
void Main_BoardWatchdogTimeout(uint16_t to_ms);

/*--------------------------------------------------------------------------------------------------
** Main_BoardWatchdogCancel()
**
** DESCRIPTION: Cancel watchdog timeout, so that the processor will not reset if it expires.
**------------------------------------------------------------------------------------------------*/
void Main_BoardWatchdogCancel(void);

/*--------------------------------------------------------------------------------------------------
** Main_BoardGpioSet()
**
** DESCRIPTION: Set GPIO output.
**------------------------------------------------------------------------------------------------*/
#ifdef BOARD_HAS_GPIO_OUTPUTS
void Main_BoardGpioSet(uint8_t num);
#endif

/*--------------------------------------------------------------------------------------------------
** Main_BoardGpioClr()
**
** DESCRIPTION: Clear GPIO output.
**------------------------------------------------------------------------------------------------*/
#ifdef BOARD_HAS_GPIO_OUTPUTS
void Main_BoardGpioClr(uint8_t num);
#endif

/*--------------------------------------------------------------------------------------------------
** Main_BoardGpioToggle()
**
** DESCRIPTION: Toggle GPIO output.
**------------------------------------------------------------------------------------------------*/
#ifdef BOARD_HAS_GPIO_OUTPUTS
void Main_BoardGpioToggle(uint8_t num);
#endif

/*--------------------------------------------------------------------------------------------------
** Main_BoardDelayUsec()
**
** DESCRIPTION: Delay for a certain time, in microseconds.
**
** PARAMETERS:  delay_us  Microseconds to delay.
**------------------------------------------------------------------------------------------------*/
void Main_BoardDelayUsec(uint32_t delay_us);

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_BOARD_H */
