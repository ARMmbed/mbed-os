/*---------------------------------------------------------------------------
 * Copyright (c) 2016, u-blox Malmö, All Rights Reserved
 * SPDX-License-Identifier: LicenseRef-PBL
 *
 * This file and the related binary are licensed under the
 * Permissive Binary License, Version 1.0 (the "License");
 * you may not use these files except in compliance with the License.
 *
 * You may obtain a copy of the License here:
 * LICENSE-permissive-binary-license-1.0.txt and at
 * https://www.mbed.com/licenses/PBL-1.0
 *
 * See the License for the specific language governing permissions and
 * limitations under the License.
 *
 * Component: Main for WiFi-driver and BT stack
 * File     : cb_main.h
 *
 * Description : 
 *-------------------------------------------------------------------------*/

#ifndef _CB_MAIN_H_
#define _CB_MAIN_H_

#include "bt_types.h"
#include "cb_bt_man.h"
#include "cb_wlan.h"
#include "mbed_events.h"

/*===========================================================================
 * DEFINES
 *=========================================================================*/
#define cbMAIN_TARGET_INVALID_ID -1

/*===========================================================================
 * TYPES
 *=========================================================================*/
typedef struct
{
    TBdAddr         address;                    /** Bluetooth address that shall be assigned to controller. Pass invalidBdAddress to use controller default address*/
    cbBM_LeRole     leRole;                     /** Bluetooth low energy role */
    cb_int8         maxOutputPower;             /** Maximum output power. */
    cb_uint32       maxLinkKeysClassic;         /** Max number of CLASSIC link keys */
    cb_uint32       maxLinkKeysLe;              /** Max number of link keys BLE*/
} cbMAIN_BtInitParams;

typedef void(*cbMAIN_ErrorHandler)(
    cb_int32            errorCode,
    const cb_char*      filename,
    cb_uint32           line);

typedef struct
{
    cbWLAN_MACAddress      mac;                 /**< MAC of WLAN interface, set to all zeros if hardware programmed address should be used. */
    cbWM_TxPowerSettings   txPowerSettings;     /**< Transmission power settings. */
} cbMAIN_WlanStartParams;

#if (BLE_STACK_UBX != true)

typedef void (*vs_cmd_send_t)(uint16_t opcode);

typedef struct {
    vs_cmd_send_t   vs_command_callback;
    char            *Service_pack;
    uint32_t        service_pack_size;
} cordio_callback_s;

#endif /* !BLE_STACK_UBX */

/*---------------------------------------------------------------------------
 * Callback to indicate that initialization of BT stack is completed.
 *-------------------------------------------------------------------------*/
typedef void(*cbMAIN_initBtComplete)(void);

/*===========================================================================
 * FUNCTIONS
 *=========================================================================*/

/**
 * Initialize OS, timers, GPIO's, heap and OTP.
 *
 * @return void
 */
extern void cbMAIN_initOS(void);

/**
 * Start Bluetooth HW.
 *
 * @param pInitParameters Initial configuration parameters. These parameters can
 *                        not be changed once Bluetooth has been started.
 * @param callback Will be invoked when initialisation is done.
 * @return void
 */
extern void cbMAIN_initBt(cbMAIN_BtInitParams *pInitParameters, cbMAIN_initBtComplete callback);

/**
 * Initialize WLAN component.
 * @return Port specific TARGET identifier
 */
extern cb_int32 cbMAIN_initWlan(void);

/**
 * Start WLAN component.
 * Create WLAN driver instance, bind it to targetId and start the driver.
 *
 * @param targetId Port specific TARGET identifier.
 * @param params Start parameters passed to WLAN driver instance.
 * @return cbSTATUS_OK if successful, otherwise cbSTATUS_ERROR.
 */
extern cb_int32 cbMAIN_startWlan(cb_int32 targetId, cbMAIN_WlanStartParams *params);

/**
 * Register error handler function.
 *
 * @param errHandler Function to be invoked in case of error.
 * @return void
 */
extern void cbMAIN_registerErrorHandler(cbMAIN_ErrorHandler errHandler);

/**
 * Start driver OS. This must be called after all cbMAIN_initOS/cbMAIN_initBt/cbMAIN_initWlan
 * to start the driver thread.
 *
 * @return void
 */
extern void cbMAIN_startOS(void);

/**
 * Get event queue. Used for running a function in the same thread context as the driver.
 * Can not be called before cbMAIN_initOS/cbMAIN_initBt/cbMAIN_initWlan.
 * Use cbMAIN_dispatchEventQueue to trigger the driver to call the queued up functions.
 * @return EventQueue     Pointer to the event queue where function calls can be enqueued.
 */
extern EventQueue* cbMAIN_getEventQueue(void);

/**
 * Lock driver from usage. This must be used if a C API function is used outside of the driver thread context.
 * The driver should only be locked for as small time as possible.
 * @return void
 */
extern void cbMAIN_driverLock(void);

/**
 * Unlock driver. used when the C API function has finished executing to release the driver for others to use.
 *
 * @return void
 */
extern void cbMAIN_driverUnlock(void);

/**
 * Dispatch event queue. Should be called to trigger calls that have been queued up in the driver context
 *
 * @return void
 */
extern void cbMAIN_dispatchEventQueue(void);

#if (BLE_STACK_UBX != true)
/**
* Initialize BT Hardware by detecting if external LPO is connected else Emulate LPO by using TIMER1 and
* configuring PORTA in alternate mode as source of external clk(LPO for BT).
*
* @return void
*/
void cbCordio_Btinit(cordio_callback_s *bt_callback_cordio);

/**
* Get BT address saved in OTP memory and provide it to stack for assignment.
*
* @param BdAddress      Pointer to be initialized with  BT address saved in OTP
* @return void
*/
void cbCordio_Retreive_Btaddr(cb_uint8 *BdAddress);


/**
* Update HCI H4 UART baud-rate to 3Mbps to achieve minimum setup time.
*
* @return void
*/
void update_uart_baud_rate(void);

#endif /* BLE_STACK_UBX */

#endif /*_CB_MAIN_H_*/
