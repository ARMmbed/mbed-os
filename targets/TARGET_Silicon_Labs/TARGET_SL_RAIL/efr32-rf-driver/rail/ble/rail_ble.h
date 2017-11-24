/***************************************************************************//**
 * @file rail_ble.h
 * @brief The BLE specific header file for the RAIL library.
 * @copyright Copyright 2016 Silicon Laboratories, Inc. http://www.silabs.com
 ******************************************************************************/

#ifndef __RAIL_BLE_H__
#define __RAIL_BLE_H__

// Get the standard include types
#include <stdint.h>
#include <stdbool.h>

// Get the RAIL specific structures and types
#include "rail_types.h"

/**
 * @addtogroup BLE
 * @ingroup Protocol_Specific
 * Accelerator routines for Bluetooth Low Energy (BLE).
 *
 * The APIs in this module help take care of configuring the radio for BLE
 * operation and provide some additional helper routines necessary for
 * normal BLE send/receive that aren't available directly in RAIL. To initialize
 * the radio you will still have to call RAIL_Init(). However
 * RAIL_ConfigChannels(), and RAIL_ConfigRadio() will be taken care of for you.
 *
 * To implement a standard BLE link layer you will also need to handle tight
 * turnaround times and send packets at specific instants. This can all be
 * managed through general RAIL functions like RAIL_ScheduleTx(),
 * RAIL_ScheduleRx(), and RAIL_SetStateTiming(). See the full RAIL API for more
 * useful functions.
 *
 * A simple example of how to setup your application to be in BLE mode is shown
 * below. Note that this will put the radio on the first advertising channel
 * with the advertising Access Address. In any full featured BLE application you
 * will need to use the RAIL_BLE_ConfigChannelRadioParams() function to change
 * the sync word and other parameters as needed based on your connection.
 *
 * @code{.c}
 *
 * // Put the radio into receive on the first BLE advertising channel
 * int bleAdvertiseEnable(void)
 * {
 *   // Call the BLE initialization function to load the right radio config
 *   RAIL_BLE_Init();
 *
 *   // Configure us for the first advertising channel (Physical: 0, Logical: 37)
 *   // The CRC init value and Access Address come from the BLE specification.
 *   RAIL_BLE_ConfigChannelRadioParams(0x555555, 0x8E89BED6, 37, false);
 *
 *   // Start receiving on this channel (Physical: 0, Logical: 37)
 *   RAIL_StartRx(0);
 *  }
 * @endcode
 *
 * @{
 */

/**
 * @enum RAIL_BLE_Coding_t
 * @brief The variant of the BLE Coded PHY
 */
RAIL_ENUM(RAIL_BLE_Coding_t) {
  RAIL_BLE_Coding_125kbps = 0,
  RAIL_BLE_Coding_125kbps_DSA = 1,
  RAIL_BLE_Coding_500kbps = 2,
  RAIL_BLE_Coding_500kbps_DSA = 3,
};

/**
 * @struct RAIL_BLE_State_t
 * @brief State structure for BLE.
 *
 * This structure must be allocated in application global read-write memory
 * that persists for the duration of BLE usage. It cannot be allocated
 * in read-only memory or on the call stack.
 */
typedef struct RAIL_BLE_State {
  uint32_t crcInit; /**< The value used for CRC initialization. */
  uint32_t accessAddress; /**< The access address used for the connection. */
  uint16_t channel; /**< The logical channel used. */
  bool disableWhitening; /**< Whether the whitening engine should be off. */
} RAIL_BLE_State_t;

/**
 * Configure RAIL to run in BLE mode.
 *
 * @param[in] railHandle Handle for RAIL instance.
 * This function will change your radio and channel configuration and other
 * parameters to match what is needed for BLE. If you need to switch back to a
 * default RAIL mode then you must call RAIL_BLE_Deinit() first. This function
 * will configure the protocol output on PTI to \ref RAIL_PTI_PROTOCOL_BLE.
 */
void RAIL_BLE_Init(RAIL_Handle_t railHandle);

/**
 * Take RAIL out of BLE mode.
 *
 * @param[in] railHandle Handle for RAIL instance.
 * This function will undo some of the configuration that happens when you call
 * RAIL_BLE_Init(). After this you can safely run your normal radio
 * initialization code to use a non-BLE configuration. This function will \b
 * not change back your radio or channel configurations so you must do this by
 * manually reinitializing. This also resets the protocol output on PTI to \ref
 * RAIL_PTI_PROTOCOL_CUSTOM.
 */
void RAIL_BLE_Deinit(RAIL_Handle_t railHandle);

/**
 * Determine whether BLE mode is enabled or not.
 *
 * @param[in] railHandle Handle for RAIL instance.
 * @return True if BLE mode is enabled and false otherwise.
 * This function returns the current status of RAIL's BLE mode. It is enabled by
 * a call to RAIL_BLE_Init() and disabled by a call to RAIL_BLE_Deinit().
 */
bool RAIL_BLE_IsEnabled(RAIL_Handle_t railHandle);

/**
 * Switch the Viterbi 1Mbps BLE PHY.
 *
 * @param[in] railHandle Handle for RAIL instance.
 * @return Status code indicating success of the function call.
 *
 * You can use this function to switch back to the defualt BLE 1Mbps PHY if you
 * have switched to the 2Mbps or another configuration. You may only call this
 * function after initializing BLE and while the radio is idle.
 */
RAIL_Status_t RAIL_BLE_ConfigPhy1MbpsViterbi(RAIL_Handle_t railHandle);

/**
 * Switch the legacy non-Viterbi 1Mbps BLE PHY.
 *
 * @param[in] railHandle Handle for RAIL instance.
 * @return Status code indicating success of the function call.
 *
 * You can use this function to switch back to the legacy BLE 1Mbps PHY if you
 * have switched to the 2Mbps or another configuration. You may only call this
 * function after initializing BLE and while the radio is idle.
 */
RAIL_Status_t RAIL_BLE_ConfigPhy1Mbps(RAIL_Handle_t railHandle);

/**
 * Switch the Viterbi 2Mbps BLE PHY.
 *
 * @param[in] railHandle Handle for RAIL instance.
 * @return Status code indicating success of the function call.
 *
 * You can use this function to switch back to the BLE 2Mbps PHY from the
 * default 1Mbps option. You may only call this function after initializing BLE
 * and while the radio is idle.
 *
 * @note Not all chips support the 2Mbps PHY. Consult your part's reference
 * manual to be sure that it does before trying this.
 */
RAIL_Status_t RAIL_BLE_ConfigPhy2MbpsViterbi(RAIL_Handle_t railHandle);

/**
 * Switch the legacy non-Viterbi 2Mbps BLE PHY.
 *
 * @param[in] railHandle Handle for RAIL instance.
 * @return Status code indicating success of the function call.
 *
 * You can use this function to switch back to legacy BLE 2Mbps PHY from the
 * default 1Mbps option. You may only call this function after initializing BLE
 * and while the radio is idle.
 *
 * @note Not all chips support the 2Mbps PHY. Consult your part's reference
 * manual to be sure that it does before trying this.
 */
RAIL_Status_t RAIL_BLE_ConfigPhy2Mbps(RAIL_Handle_t railHandle);

/**
 * Switch to the BLE Coded PHY.
 *
 * @param[in] railHandle Handle for RAIL instance.
 * @param[in] ble_coding The RAIL_BLE_Coding_t to use
 * @return Status code indicating success of the function call.
 *
 * You can use this function to switch back to BLE Coded PHY from the default
 * 1Mbps option. You may only call this function after initializing BLE and
 * while the radio is idle. When using a BLE Coded PHY, the subPhy in
 * RAIL_AppendedInfo_t marks the coding of the received packet. A subPhy of 0
 * marks a 500kbps packet, and a subPhy of 1 marks a 125kbps packet.
 *
 * @note Not all chips support the BLE Coded PHY. Consult your part's reference
 * manual to be sure that it does before trying this.
 */
RAIL_Status_t RAIL_BLE_ConfigPhyCoded(RAIL_Handle_t railHandle,
                                      RAIL_BLE_Coding_t ble_coding);

/**
 * Helper function to change BLE radio parameters.
 *
 * @param[in] railHandle Handle for RAIL instance.
 * @param[in] crcInit The value to use for CRC initialization.
 * @param[in] accessAddress The access address to use for the connection.
 * @param[in] channel The logical channel that you're changing to. This is used to
 * initialize the whitener if you're using whitening.
 * @param[in] disableWhitening This can turn off the whitening engine and is useful
 * for sending BLE test mode packets that don't have this turned on.
 * @return Status code indicating success of the function call.
 *
 * This function can be used to switch radio parameters on every connection
 * and/or channel change. It is BLE-aware and will set the access address,
 * preamble, CRC initialization value, and whitening configuration without
 * requiring you to load a new radio config.
 */
RAIL_Status_t RAIL_BLE_ConfigChannelRadioParams(RAIL_Handle_t railHandle,
                                                uint32_t crcInit,
                                                uint32_t accessAddress,
                                                uint16_t channel,
                                                bool disableWhitening);

/** @} */ // end of BLE

#endif // __RAIL_BLE_H__
