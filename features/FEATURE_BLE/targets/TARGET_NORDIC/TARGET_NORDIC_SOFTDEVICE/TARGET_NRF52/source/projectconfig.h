/* mbed Microcontroller Library
 * Copyright (c) 2006-2013 ARM Limited
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#ifndef _PROJECTCONFIG_H_
#define _PROJECTCONFIG_H_

#include "ble/GapAdvertisingData.h"

/*=========================================================================
    MCU & BOARD SELCTION

    CFG_BOARD is one of the value defined in board.h
    -----------------------------------------------------------------------*/
    #define CFG_BOARD                                  BOARD_PCA10001
    #define CFG_MCU_STRING                             "nRF51822"
/*=========================================================================*/


/*=========================================================================
    CODE BASE VERSION SETTINGS

    Please do not modify this version number.  To set a version number
    for your project or firmware, change the values in your 'boards/'
    config file.
    -----------------------------------------------------------------------*/
    #define CFG_CODEBASE_VERSION_MAJOR                 0
    #define CFG_CODEBASE_VERSION_MINOR                 1
    #define CFG_CODEBASE_VERSION_REVISION              0
/*=========================================================================*/


/*=========================================================================
    FIRMWARE VERSION SETTINGS
    -----------------------------------------------------------------------*/
    #define CFG_FIRMWARE_VERSION_MAJOR                 0
    #define CFG_FIRMWARE_VERSION_MINOR                 0
    #define CFG_FIRMWARE_VERSION_REVISION              0
/*=========================================================================*/


/*=========================================================================
    DEBUG LEVEL
    -----------------------------------------------------------------------

    CFG_DEBUG                 Level 3: Full debug output, any failed assert
                                       will produce a breakpoint for the
                                       debugger
                              Level 2: ATTR_ALWAYS_INLINE is null, ASSERT
                                       has text
                              Level 1: ATTR_ALWAYS_INLINE is an attribute,
                                       ASSERT has no text
                              Level 0: No debug information generated

    -----------------------------------------------------------------------*/
    #define CFG_DEBUG                                  (1)

    #if (CFG_DEBUG > 3) || (CFG_DEBUG < 0)
      #error "CFG_DEBUG must be a value between 0 (no debug) and 3"
    #endif
/*=========================================================================*/


/*=========================================================================
    GENERAL NRF51 PERIPHERAL SETTINGS
    -----------------------------------------------------------------------

    CFG_SCHEDULER_ENABLE      Set this to 'true' or 'false' depending on
                              if you use the event scheduler or not

    -----------------------------------------------------------------------*/
    #define CFG_SCHEDULER_ENABLE                       false

    /*------------------------------- GPIOTE ------------------------------*/
    #define CFG_GPIOTE_MAX_USERS                       1                        /**< Maximum number of users of the GPIOTE handler. */

    /*-------------------------------- TIMER ------------------------------*/
    #define CFG_TIMER_PRESCALER                        0                        /**< Value of the RTC1 PRESCALER register. freq = (32768/(PRESCALER+1)) */
    #define CFG_TIMER_MAX_INSTANCE                     1                        /**< Maximum number of simultaneously created timers. */
    #define CFG_TIMER_OPERATION_QUEUE_SIZE             2                        /**< Size of timer operation queues. */
/*=========================================================================*/


/*=========================================================================
    BTLE SETTINGS
    -----------------------------------------------------------------------*/

    #define CFG_BLE_TX_POWER_LEVEL                     0                        /**< in dBm (Valid values are -40, -20, -16, -12, -8, -4, 0, 4) */

    /*---------------------------- BOND MANAGER ---------------------------*/
    #define CFG_BLE_BOND_FLASH_PAGE_BOND               (BLE_FLASH_PAGE_END-1)   /**< Flash page used for bond manager bonding information.*/
    #define CFG_BLE_BOND_FLASH_PAGE_SYS_ATTR           (BLE_FLASH_PAGE_END-3)   /**< Flash page used for bond manager system attribute information. TODO check if we can use BLE_FLASH_PAGE_END-2*/
    #define CFG_BLE_BOND_DELETE_BUTTON_NUM             0                        /**< Button to press to delete bond details during init */

    /*------------------------------ SECURITY -----------------------------*/
    #define CFG_BLE_SEC_PARAM_MITM                     0                        /**< Man In The Middle protection not required. */
    #define CFG_BLE_SEC_PARAM_IO_CAPABILITIES          BLE_GAP_IO_CAPS_NONE     /**< No I/O capabilities. */
    #define CFG_BLE_SEC_PARAM_OOB                      0                        /**< Out Of Band data not available. */
    #define CFG_BLE_SEC_PARAM_MIN_KEY_SIZE             7                        /**< Minimum encryption key size. */
    #define CFG_BLE_SEC_PARAM_MAX_KEY_SIZE             16

    /*--------------------------------- GAP -------------------------------*/
    #define CFG_GAP_APPEARANCE                         GapAdvertisingData::GENERIC_TAG
    #define CFG_GAP_LOCAL_NAME                         "nRF5x"

    #define CFG_GAP_CONNECTION_MIN_INTERVAL_MS           50                     /**< Minimum acceptable connection interval */
    #define CFG_GAP_CONNECTION_MAX_INTERVAL_MS          500                     /**< Maximum acceptable connection interval */
    #define CFG_GAP_CONNECTION_SUPERVISION_TIMEOUT_MS  4000                     /**< Connection supervisory timeout */
    #define CFG_GAP_CONNECTION_SLAVE_LATENCY           0                        /**< Slave Latency in number of connection events. */

    #define CFG_GAP_ADV_INTERVAL_MS                    25                       /**< The advertising interval in miliseconds, should be multiply of 0.625 */
    #define CFG_GAP_ADV_TIMEOUT_S                      180                      /**< The advertising timeout in units of seconds. */
/*=========================================================================*/


/*=========================================================================
    VALIDATION
    -----------------------------------------------------------------------*/
    #if CFG_BLE_TX_POWER_LEVEL != -40 && CFG_BLE_TX_POWER_LEVEL != -20 && CFG_BLE_TX_POWER_LEVEL != -16 && CFG_BLE_TX_POWER_LEVEL != -12 && CFG_BLE_TX_POWER_LEVEL != -8  && CFG_BLE_TX_POWER_LEVEL != -4  && CFG_BLE_TX_POWER_LEVEL != 0   && CFG_BLE_TX_POWER_LEVEL != 4
        #error "CFG_BLE_TX_POWER_LEVEL must be -40, -20, -16, -12, -8, -4, 0 or 4"
    #endif
/*=========================================================================*/

#endif /* _PROJECTCONFIG_H_ */
