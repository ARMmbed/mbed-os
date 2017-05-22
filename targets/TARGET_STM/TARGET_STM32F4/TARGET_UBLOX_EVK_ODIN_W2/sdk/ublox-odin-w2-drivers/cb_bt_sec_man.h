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
 * Component   : Bluetooth Security Manager
 * File        : cb_bt_sec_man.h
 *
 * Description : Bluetooth security application support
 *-------------------------------------------------------------------------*/

/**
 * @file cb_bt_sec_man.h
 * @brief  Bluetooth security application support. This includes bonding,
 * security modes, passkey and pin code handling.
 */

#ifndef _CB_BT_SEC_MAN_H_
#define _CB_BT_SEC_MAN_H_

#include "cb_comdefs.h"

#ifdef __cplusplus
extern "C" {
#endif

/*===========================================================================
 * DEFINES
 *=========================================================================*/
#define cbBSM_OK                                    (0)
#define cbBSM_ERROR                                 (-1)

#define cbBSM_PASSKEY_MAX_VALUE                     (999999)

/*===========================================================================
 * TYPES
 *=========================================================================*/

/**
 * cbBSM_SECURITY_MODE_1_DISABLED
 * Security disabled
 * - Remote Device BT 2.1: Auto accept (No man-in-the-middle attack protection, encryption enabled) 
 * - Remote Device BT 2.0: Authentication and encryption disabled. 
 * - Bluetooth Low Energy: Auto accept (No man-in-the-middle attack protection, encryption enabled) 
 * 
 * cbBSM_SECURITY_MODE_2_BT_2_0
 * - Enforce BT 2.0 (Service level authentication and encryption enabled) 
 * Please note that the device is not BT 2.1 qualified for this setting. It is included for backward compatibility. Invalid for Bluetooth Low Energy.
 * 
 * cbBSM_SECURITY_MODE_3_FIXED_PIN
 * - Remote Device BT 2.1: Service level authentication and encryption enabled. 
 * - Remote Device BT 2.0: Service level authentication and encryption enabled. 
 * - Bluetooth Low Energy: Service level authentication and encryption enabled. 
 * Please note that this security mode will not work with a general BT 2.1 device. However, it will work between two connectBlue BT 2.1 Serial Port Adapters. Use security mode 4 to make the device work with a general BT 2.1 device. 
 * 
 * cbBSM_SECURITY_MODE_4_JUST_WORKS
 * - Remote Device BT 2.1: Auto accept (no man-in-the-middle attack protection, encryption enabled) 
 * - Remote Device BT 2.0: Service level authentication and encryption enabled. 
 * - Bluetooth Low Energy: Auto accept (no man-in-the-middle attack protection, encryption enabled)
 * This security mode is intended for pairing in safe environments. When this mode is set, pairability (see AT*AGPM) is automatically disabled. In data mode, pairing can be enabled for 60 seconds by pressing the "External Connect" button for at least 5 seconds. When the module is pairable, the LED will blink. If the mode is changed from Just Works to another, pairability must be enabled again using the AT*AGPM command. 
 * 
 * cbBSM_SECURITY_MODE_5_DISPLAY_ONLY 
 * - Remote Device BT 2.1: Service level authentication and encryption enabled. User should be presented a passkey. 
 * - Remote Device BT 2.0: Service level authentication and encryption enabled. No user interaction required. 
 * - Bluetooth Low Energy: Service level authentication and encryption enabled. User should be presented a passkey.
 * This security mode is used when the device has a display that can present a 6-digit value that the user shall enter on the remote device. 
 * 
 * cbBSM_SECURITY_MODE_6_DISPLAY_YES_NO 
 * - Remote Device BT 2.1: Service level authentication and encryption enabled. User should compare two values. 
 * - Remote Device BT 2.0: Service level authentication and encryption enabled. No user interaction required. 
 * This security mode is used when the device has a display that can present a 6-digit value that the user shall verify with yes or no to the remote device's presented value. 
 * Invalid for Bluetooth Low Energy.
 * 
 * cbBSM_SECURITY_MODE_7_KEYBOARD_ONLY
 * - Remote Device BT 2.1: Service level authentication and encryption enabled. User should enter a passkey. 
 * - Remote Device BT 2.0: Service level authentication and encryption enabled. No user interaction required. 
 * - Bluetooth Low Energy: Service level authentication and encryption enabled. User should enter a passkey.
 * This security mode is used when the device only has a keyboard where the user can enter a 6-digit value that is presented on the remote device. 
 */
typedef enum
{
    cbBSM_SECURITY_MODE_1_DISABLED = 1,
    cbBSM_SECURITY_MODE_2_BT_2_0,
    cbBSM_SECURITY_MODE_3_FIXED_PIN,
    cbBSM_SECURITY_MODE_4_JUST_WORKS,
    cbBSM_SECURITY_MODE_5_DISPLAY_ONLY,
    cbBSM_SECURITY_MODE_6_DISPLAY_YES_NO,
    cbBSM_SECURITY_MODE_7_KEYBOARD_ONLY
} cbBSM_SecurityMode;

typedef struct
{
    TPinCode pin;
    cb_uint8   nBytes;
} cbBSM_PinCode;

typedef enum
{
    cbBSM_BOND_TYPE_CLASSIC,
    cbBSM_BOND_TYPE_LE,
    cbBSM_BOND_TYPE_ALL,
} cbBSM_BondTypes;

typedef enum
{
    cbBSM_BOND_STATUS_OK = 0,
    cbBSM_BOND_STATUS_ERR_PAGE_TMO,
    cbBSM_BOND_STATUS_ERR_AUTH_FAIL,
    cbBSM_BOND_STATUS_ERR_NO_MITM
} cbBSM_BondStatus;

/**
 * Callback to indicate that bonding is finished.
 * @param   bdAddress Remote BD address
 * @param   bondStatus Bond status, e.g. cbBSM_BOND_STATUS_OK
 * @return  None
 */
typedef void (*cbBSM_BondCnf)(
    cbBSM_BondStatus status,
    TBdAddr* pBdAddress);

/**
 * Callback to indicate that a pin code is required from upper layer.
 * Respond the pin code request with cbBSM_rspFixedPin/cbBSM_rspNegFixedPin
 * This is only used when either local or remote side does not support 
 * BT 2.1 secure simple pairing.
 * @param   bdAddress Remote BD address
 * @return  None
 */
typedef void (*cbBSM_RequestPinInd)(
    TBdAddr* pBdAddress);

/**
 * Callback to indicate that user confirmation is required. The user should
 * compare numericValues on local and remote side and respond the confirmation 
 * request with cbBSM_rspUserConfirmation if values match and 
 * cbBSM_rspNegUserConfirmation if they do not match or user wants to interrupt
 * the pairing attempt.
 * This is only used when both sides support BT 2.1 secure simple pairing and 
 * security mode cbBSM_SECURITY_MODE_6_DISPLAY_YES_NO is used.
 * @param   bdAddress Remote BD address
 * @param   numericValue The numeric value to be compared
 * @return  None
 */
typedef void (*cbBSM_UserConfirmationInd)(
    TBdAddr* pBdAddress,
    cb_uint32 numericValue);

/**
 * Callback to indicate that a passkey is required from upper layer.
 * Respond the passkey request with cbBSM_rspUserPasskey/cbBSM_rspNegUserPasskey.
 * This is only used when both sides support BT 2.1 secure simple pairing and 
 * security modes cbBSM_SECURITY_MODE_3_FIXED_PIN or cbBSM_SECURITY_MODE_7_KEYBOARD_ONLY is used
 * @param   bdAddress Remote BD address
 * @return  None
 */
typedef void (*cbBSM_UserPasskeyInd)(
    TBdAddr* pBdAddress);

/**
 * Callback to indicate that a passkey is used in the pairing procedure.
 * The passkey should be displayed to the user.
 * This is only used when both sides support BT 2.1 secure simple pairing and
 * security mode cbBSM_SECURITY_MODE_5_DISPLAY_ONLY is used.
 * @param   bdAddress Remote BD address
 * @param   passkey Passkey
 * @return  None
 */
typedef void (*cbBSM_UserPasskeyEvt)(
    TBdAddr* pBdAddress,
    cb_uint32 passkey);

typedef struct  
{
    cbBSM_RequestPinInd requestPinInd;
    cbBSM_UserConfirmationInd userConfirmationInd;
    cbBSM_UserPasskeyInd userPasskeyInd;
    cbBSM_UserPasskeyEvt userPasskeyEvt;
    cbBSM_BondCnf bondConfirmation;
    cbBSM_BondCnf bondEvent;
} cbBSM_Callbacks;

/*===========================================================================
 * FUNCTIONS
 *=========================================================================*/

/**
 * Initialization of BLuetooth security manager. Called during stack
 * initialization. Shall not be called by application.
 * 
 * @return  None
 */
extern void cbBSM_init(void);

/**
 * Register security callbacks. Callbacks in the struct that are not 
 * of any interest can be set to NULL.
 *
 * @param pPairingCallbacks Pointer to the security callback struct
 * @return If the operation is successful cbBSM_OK is returned.
 */
extern cb_int32 cbBSM_registerCallbacks(cbBSM_Callbacks* pPairingCallbacks);

/**
 * Set security mode. See comments on cbBSM_SecurityMode for 
 * description of the different security modes.
 * 
 * @param securityMode Security mode. Default security is cbBSM_SECURITY_MODE_1_DISABLED
 * @param allowPairingInNonBondableMode Normally FALSE. Set to TRUE if pairing should be allowed when not bondable.
 *                                      No link keys will then be stored.
 * @return If the operation is successful cbBSM_OK is returned.
 */
extern cb_int32 cbBSM_setSecurityMode(
    cbBSM_SecurityMode securityMode,
    cb_boolean allowPairingInNonBondableMode);

/**
 * Read current security mode.
 * 
 * @param pSecurityMode Security mode
 * @return If the operation is successful cbBSM_OK is returned.
 */
extern cb_int32 cbBSM_getSecurityMode(cbBSM_SecurityMode* pSecurityMode);

/**
 * Sets the local device pairable mode.
 *
 * @param   pairable TRUE=pairable, FALSE=not pairable (default)
 * @return  If the operation is successful cbBSM_OK is returned.
 */
extern cb_int32 cbBSM_setPairable(boolean pairable);

/**
 * Gets the local device pairable mode.
 * 
 * @param pPairable Pointer to return value
 * @return If the operation is successful cbBSM_OK is returned.
 */
extern cb_int32 cbBSM_getPairable(boolean* pPairable);

/**
 * Performs bonding with a remote device. The cbBSM_BondCnf callback will
 * be called upon success/failure.
 *
 * @param remoteDevice Remote BD address
 * @param type Classic or LE
 * @return If the operation is successful cbBSM_OK is returned.
 */
extern cb_int32 cbBSM_reqBond(
    TBdAddr remoteDevice,
    TBluetoothType type);

/**
 * Responds on the cbBSM_RequestPinInd callback with a pin code
 * This is only used when either local or remote side does not support 
 * BT 2.1 secure simple pairing.
 *
 * @param pBdAddress Pointer to the remote BD address
 * @param pinCodeLength Length of the provided pin code
 * @param pPinCode Pointer to the provided pin code
 * @return If the operation is successful cbBSM_OK is returned.
 */
extern cb_int32 cbBSM_rspFixedPin(
    TBdAddr* pBdAddress,
    cb_uint8 pinCodeLength,
    cb_uint8 *pPinCode);

/**
 * Responds the cbBSM_RequestPinInd callback. Can be used to interrupt a
 * pairing attempt from the remote device.
 * This is only used when either local or remote side does not support 
 * BT 2.1 secure simple pairing.
 *
 * @param   pBdAddress Pointer to the remote BD address
 * @return  If the operation is successful cbBSM_OK is returned.
 */
extern cb_int32 cbBSM_rspNegFixedPin(TBdAddr* pBdAddress);

/**
 * Responds on the cbBSM_UserPasskeyInd callback.
 * This is only used when both sides support BT 2.1 secure simple pairing.
 *
 * @param   pBdAddress Pointer to the remote BD address
 * @param   passkey Passkey, range: 0-999999
 * @return  If the operation is successful cbBSM_OK is returned.
 */
extern cb_int32 cbBSM_rspUserPasskey(
    TBdAddr *pBdAddress,
    uint32 passkey);

/**
 * Responds on the cbBSM_UserPasskeyInd callback. Can be used to interrupt a
 * pairing attempt from the remote device.
 * This is only used when both sides support BT 2.1 secure simple pairing.
 *
 * @param   pBdAddress Pointer to the remote BD address
 * @return  If the operation is successful cbBSM_OK is returned.
 */
extern cb_int32 cbBSM_rspNegUserPasskey(TBdAddr *pBdAddress);

/**
 * Responds on the cbBSM_UserConfirmationInd callback. Accepts the numeric value.
 * This is only used when both sides support BT 2.1 secure simple pairing.
 *
 * @param   pBdAddress Pointer to the remote BD address
 * @return  If the operation is successful cbBSM_OK is returned.
 */
extern cb_int32 cbBSM_rspUserConfirmation(TBdAddr* pBdAddress);

/**
 * Responds on the cbBSM_UserConfirmationInd callback. Rejects the numeric value.
 * This is only used when both sides support BT 2.1 secure simple pairing.
 *
 * @param   pBdAddress Pointer to the remote BD address
 * @return  If the operation is successful cbBSM_OK is returned.
 */
extern cb_int32 cbBSM_rspNegUserConfirmation(TBdAddr* pBdAddress);

/**
 * Get number of bonded devices.
 *
 * @param type Bond type
 * @param pNo Pointer to return value.
 * @return If the operation is successful cbBSM_OK is returned.
 */
extern cb_int32 cbBSM_getAllNumberBondedDevices(
    cbBSM_BondTypes type,
    uint32* pNo);

/**
* Get a bonded devices.
*
* @param deviceIndex Index of the bonded device
* @param pBdAddr Pointer to remote BD address.
* @param pIsLe   Should be TRUE for LE and FALSE for classic
* @return If the operation is successful cbBSM_OK is returned.
*/
extern cb_int32 cbBSM_getBondedDevice(
    uint32 deviceIndex,
    TBdAddr* pBdAddr,
    cb_boolean pIsLe);

/**
 * Delete a bonded device and its link keys.
 * 
 * @param pBdAddress to the address of the device which bond shall be deleted.
 * @return If the operation is successful cbBSM_OK is returned.
 */
extern cb_int32 cbBSM_deleteBondedDevice(TBdAddr* pBdAddress); 

/**
 * Delete all bonded devices and link keys.
 * 
 * @return If the operation is successful cbBSM_OK is returned.
 */
extern cb_int32 cbBSM_deleteAllBondedDevices(void);

#ifdef __cplusplus
}
#endif

#endif /* _CB_BT_SEC_MAN_H_ */






