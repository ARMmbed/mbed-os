/***************************************************************************//**
 * @file rail_ieee802154.h
 * @brief The IEEE 802.15.4 specific header file for the RAIL library.
 * @copyright Copyright 2016 Silicon Laboratories, Inc. www.silabs.com
 ******************************************************************************/

#ifndef __RAIL_IEEE802154_H__
#define __RAIL_IEEE802154_H__

#include "rail_types.h"

#ifdef __cplusplus
extern "C" {
#endif

/// @addtogroup IEEE802_15_4 IEEE 802.15.4
/// @ingroup Protocol_Specific
/// @brief IEEE 802.15.4 configuration routines
///
/// The functions in this group configure RAIL IEEE 802.15.4 hardware
/// acceleration which includes IEEE 802.15.4 format filtering, address
/// filtering, acking, and filtering based on the frame type.
///
/// To configure IEEE 802.15.4 functionality, the application must first setup
/// a RAIL instance as normal with RAIL_Init() and other setup functions.
/// Instead of RAIL_ConfigChannels() and RAIL_ConfigRadio(), however, an
/// application may use RAIL_IEEE802154_Config2p4GHzRadio() to setup the
/// official IEEE 2.4GHz 802.15.4 PHY. This configuration is shown below.
///
/// @code{.c}
/// static RAIL_Handle_t railHandle = NULL; // Initialized somewhere else
///
/// static const RAIL_IEEE802154_Config_t rail154Config = {
///   .addresses = NULL,
///   .ackConfig = {
///     .enable = true,     // Turn on auto ACK for IEEE 802.15.4
///     .ackTimeout = 864,  // 54 symbols * 16 us/symbol = 864 us
///     .rxTransitions = {
///       .success = RAIL_RF_STATE_TX,  // Go to Tx to send the ACK
///       .error = RAIL_RF_STATE_RX,    // For an always on device stay in Rx
///     },
///     .txTransitions = {
///       .success = RAIL_RF_STATE_RX,  // Go to Rx for receiving the ACK
///       .error = RAIL_RF_STATE_RX,    // For an always on device stay in Rx
///     },
///   },
///   .timings = {
///     .idleToRx = 100,
///     .idleToTx = 100,
///     .rxToTx = 192,    // 12 symbols * 16 us/symbol = 192 us
///     .txToRx = 192,    // 12 symbols * 16 us/symbol = 192 us
///     .rxSearchTimeout = 0, // not used
///     .txToRxSearchTimeout = 0, // not used
///   },
///   .framesMask = RAIL_IEEE802154_ACCEPT_STANDARD_FRAMES,
///   .promiscuousMode = false,  // Enable format and address filtering
///   .isPanCoordinator = false,
/// };
///
/// void config154(void)
/// {
///   // Configure the radio and channels for 2.4GHz IEEE 802.15.4
///   RAIL_IEEE802154_Config2p4GHzRadio(railHandle);
///   // Initialize the IEEE 802.15.4 config using the static config above
///   RAIL_IEEE802154_Init(railHandle, &rail154Config);
/// }
/// @endcode
///
/// To configure address filtering the application can call
/// RAIL_IEEE802154_SetAddresses() with a structure containing all addresses or
/// can call the individual RAIL_IEEE802154_SetPanId(),
/// RAIL_IEEE802154_SetShortAddress(), and RAIL_IEEE802154_SetLongAddress()
/// APIs. RAIL supports \ref RAIL_IEEE802154_MAX_ADDRESSES number of address
/// pairs for situations where you want to receive packets from multiple IEEE
/// 802.15.4 networks at the same time. Broadcast addresses are supported by
/// default without any additional configuration so they do not consume one of
/// these slots. If the application does not require all address pairs be sure
/// to set unused ones to the proper disabled value for each type. These can
/// be found in the \ref RAIL_IEEE802154_AddrConfig_t documentation. Below is
/// an example of setting filtering for one set of addresses.
///
/// @code{.c}
/// // PanID OTA value of 0x34 0x12
/// // Short Address OTA byte order of 0x78 0x56
/// // Long address with OTA byte order of 0x11 0x22 0x33 0x44 0x55 0x66 0x77 0x88
///
/// // Setup all address simultaneously
/// RAIL_Status_t setup1(void)
/// {
///   RAIL_IEEE802154_AddrConfig_t nodeAddress = {
///     { 0x1234, 0xFFFF, 0xFFFF },
///     { 0x5678, 0xFFFF, 0xFFFF },
///     { { 0x11, 0x22, 0x33, 0x44, 0x55, 0x66, 0x77, 0x88 },
///       { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 },
///       { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 } }
///   };
///   return RAIL_IEEE802154_SetAddresses(railHandle, &nodeAddress);
/// }
///
/// // Alternatively the addresses can be setup individually as follows:
/// RAIL_Status_t setup2(void)
/// {
///   RAIL_Status_t status;
///   const uint8_t longAddress[] = { 0x11, 0x22, 0x33, 0x44, 0x55, 0x66, 0x77, 0x88 };
///
///   status = RAIL_IEEE802154_SetPanId(railHandle, 0x1234, 0);
///   if (status != RAIL_STATUS_NO_ERROR) {
///     return status
///   }
///   status = RAIL_IEEE802154_SetShortAddress(railHandle, 0x5678, 0);
///   if (status != RAIL_STATUS_NO_ERROR) {
///     return status
///   }
///   status = RAIL_IEEE802154_SetLongAddress(railHandle, longAddress, 0);
///   if (status != RAIL_STATUS_NO_ERROR) {
///     return status
///   }
///
///   return RAIL_STATUS_NO_ERROR;
/// }
/// @endcode
///
/// Address filtering will be enabled except when in promiscuous mode which can
/// be set with RAIL_IEEE802154_SetPromiscuousMode(). The addresses may be
/// changed at runtime but if you are receiving a packet while reconfiguring the
/// address filters you may get undesired behavior so it's safest to do this
/// while not in receive.
///
/// Auto ACK is controlled by the ackConfig and timings fields passed to
/// RAIL_IEEE802154_Init(). After initialization though they may be controlled
/// using the normal \ref Auto_Ack and \ref State_Transitions APIs. When in IEEE
/// 802.15.4 mode the ACK will have a 5 byte length, the frame type will be set
/// to ack, and the frame pending bit will be set if
/// RAIL_IEEE802154_SetFramePending() is called when the \ref
/// RAIL_EVENT_IEEE802154_DATA_REQUEST_COMMAND event is triggered. This event
/// must be turned on by the user and will fire whenever a data request is being
/// received so that the stack can determine whether there is pending data. Be
/// aware that the frame pending bit must be set quickly after receiving the
/// event or the ACK may already have been transmitted. Check the return code of
/// RAIL_IEEE802154_SetFramePending() to be sure that the bit was set in time.
///
/// Transmit and receive operations are all done using the standard RAIL APIs in
/// IEEE 802.15.4 mode. To send packets using the correct CSMA configuration
/// there is a \ref RAIL_CSMA_CONFIG_802_15_4_2003_2p4_GHz_OQPSK_CSMA define
/// that can initialize the csmaConfig structure passed to \ref
/// RAIL_StartCcaCsmaTx().
/// @{

/**
 * @enum RAIL_IEEE802154_AddressLength_t
 * @brief Different lengths that an 802.15.4 address can have
 */
RAIL_ENUM(RAIL_IEEE802154_AddressLength_t) {
  RAIL_IEEE802154_ShortAddress = 2, /**< 2 byte short address. */
  RAIL_IEEE802154_LongAddress = 3, /**< 8 byte extended address. */
};

/**
 * @struct RAIL_IEEE802154_Address_t
 * @brief Representation of 802.15.4 address
 * This structure is only used for a received address, which needs to be parsed
 * to discover the type.
 */
typedef struct RAIL_IEEE802154_Address{
  /** Convenient storage for different address types */
  union {
    uint16_t shortAddress; /**< Present for 2 byte addresses. */
    uint8_t longAddress[8]; /**< Present for 8 byte addresses. */
  };
  /**
   * Enum of the received address length
   */
  RAIL_IEEE802154_AddressLength_t length;
} RAIL_IEEE802154_Address_t;

/** The maximum number of allowed addresses of each type. */
#define RAIL_IEEE802154_MAX_ADDRESSES 3

/**
 * @struct RAIL_IEEE802154_AddrConfig_t
 * @brief Configuration structure for IEEE 802.15.4 Address Filtering. The
 * broadcast addresses are handled separately, and do not need to be specified
 * here. Any address to be ignored should be set with all bits high.
 *
 * This structure allows configuration of dual-PAN functionality, by specifying
 * multiple PAN IDs and short addresses. A packet will be received if it
 * matches either PAN ID and the long address. The short addresses are specific
 * to a given PAN, so the first short address goes with the first PAN ID, and
 * not with the second PAN ID. The broadcast PAN ID and address will work with
 * any address or PAN ID, respectively.
 */
typedef struct RAIL_IEEE802154_AddrConfig{
  /**
   * PAN IDs for destination filtering. Both must be specified.
   * To disable a PAN ID, set it to the broadcast value, 0xFFFF.
   */
  uint16_t panId[RAIL_IEEE802154_MAX_ADDRESSES];
  /**
   * Short network addresses for destination filtering. Both must be specified.
   * To disable a short address, set it to the broadcast value, 0xFFFF.
   */
  uint16_t shortAddr[RAIL_IEEE802154_MAX_ADDRESSES];
  /**
   * 64 bit address for destination filtering. Both must be specified.
   * This field is parsed in over-the-air (OTA) byte order. To disable a long
   * address, set it to the reserved value of 0x00 00 00 00 00 00 00 00.
   */
  uint8_t longAddr[RAIL_IEEE802154_MAX_ADDRESSES][8];
} RAIL_IEEE802154_AddrConfig_t;

/**
 * @struct RAIL_IEEE802154_Config_t
 * @brief Configuration structure for IEEE 802.15.4 in RAIL
 */
typedef struct RAIL_IEEE802154_Config {
  /**
   * Configure the RAIL Address Filter to allow the given destination
   * addresses. If addresses is NULL, defer destination address configuration.
   * If a member of addresses is NULL, defer configuration of just that member.
   * This can be overridden via RAIL_IEEE802154_SetAddresses(), or the
   * individual members can be changed via RAIL_IEEE802154_SetPanId(),
   * RAIL_IEEE802154_SetShortAddress(), and RAIL_IEEE802154_SetLongAddress().
   */
  const RAIL_IEEE802154_AddrConfig_t *addresses;
  /**
   * Defines the acking configuration for the IEEE 802.15.4 implementation
   */
  RAIL_AutoAckConfig_t ackConfig;
  /**
   * Defines state timings for the IEEE 802.15.4 implementation
   */
  RAIL_StateTiming_t timings;
  /**
   * Set which 802.15.4 frame types will be received, of Beacon, Data, Ack, and
   * Command. This setting can be overridden via RAIL_IEEE802154_AcceptFrames().
   */
  uint8_t framesMask;
  /**
   * Enable promiscuous mode during configuration. This can be overridden via
   * RAIL_IEEE802154_SetPromiscuousMode() afterwards.
   */
  bool promiscuousMode;
  /**
   * Set whether the device is a PAN Coordinator during configuration. This can
   * be overridden via RAIL_IEEE802154_SetPanCoordinator() afterwards.
   */
  bool isPanCoordinator;
} RAIL_IEEE802154_Config_t;

/**
 * Initialize RAIL for IEEE802.15.4 features
 *
 * @param[in] railHandle Handle of RAIL instance
 * @param[in] fifteenFourConfig IEEE802154 configuration struct
 * @return Status code indicating success of the function call.
 *
 * This function calls the following RAIL functions to configure the radio for
 * IEEE802.15.4 features.
 *
 * Initializes the following:
 *   - Enables IEEE802154 hardware acceleration
 *   - Configures RAIL Auto Ack functionality
 *   - Configures RAIL Address Filter for 802.15.4 address filtering
 *
 * It calls the following functions:
 * - RAIL_ConfigAutoAck()
 * - RAIL_SetRxTransitions()
 * - RAIL_SetTxTransitions()
 * - RAIL_SetStateTiming()
 * - RAIL_ConfigAddressFilter()
 * - RAIL_EnableAddressFilter()
 */
RAIL_Status_t RAIL_IEEE802154_Init(RAIL_Handle_t railHandle,
                                   const RAIL_IEEE802154_Config_t *fifteenFourConfig);

/**
 * Configures the radio for 2.4GHz 802.15.4 operation
 *
 * @param[in] railHandle Handle of RAIL instance
 * @return Status code indicating success of the function call.
 *
 * This initializes the radio for 2.4GHz operation. It takes the place of
 * calling \ref RAIL_ConfigRadio and \ref RAIL_ConfigChannels. After this call,
 * channels 11-26 will be available, giving the frequencies of those channels
 * on channel page 0, as defined by IEEE 802.15.4-2011 section 8.1.2.2.
 */
RAIL_Status_t RAIL_IEEE802154_Config2p4GHzRadio(RAIL_Handle_t railHandle);

/**
 * De-initializes IEEE802.15.4 hardware acceleration
 *
 * @param[in] railHandle Handle of RAIL instance
 * @return Status code indicating success of the function call.
 *
 * Disables and resets all IEE802.15.4 hardware acceleration features. This
 * function should only be called when the radio is IDLE. This calls the
 * following:
 * - RAIL_SetStateTiming(), to reset all timings to 100 us
 * - RAIL_EnableAddressFilter(false)
 * - RAIL_ResetAddressFilter()
 */
RAIL_Status_t RAIL_IEEE802154_Deinit(RAIL_Handle_t railHandle);

/**
 * Return whether IEEE802.15.4 hardware acceleration is currently enabled.
 *
 * @param[in] railHandle Handle of RAIL instance
 * @return True if IEEE802.15.4 hardware acceleration was enabled to start with
 * and false otherwise
 */
bool RAIL_IEEE802154_IsEnabled(RAIL_Handle_t railHandle);

/**
 * Configure the RAIL Address Filter for 802.15.4 filtering
 *
 * @param[in] railHandle Handle of RAIL instance
 * @param[in] addresses The address information that should be used
 * @return Status code indicating success of the function call. If this returns
 * an error, then the 802.15.4 address filter is in an undefined state.
 *
 * Set up the 802.15.4 address filter to accept messages to the given
 * addresses. This will return false if any of the addresses failed to be set.
 * If NULL is passed in for addresses, then all addresses will be set to their
 * reset value.
 */
RAIL_Status_t RAIL_IEEE802154_SetAddresses(RAIL_Handle_t railHandle,
                                           const RAIL_IEEE802154_AddrConfig_t *addresses);

/**
 * Set a PAN ID for 802.15.4 address filtering
 *
 * @param[in] railHandle Handle of RAIL instance
 * @param[in] panId The 16-bit PAN ID information.
 * This will be matched against the destination PAN ID of incoming messages.
 * The PAN ID is sent little endian over the air meaning panId[7:0] is first in
 * the payload followed by panId[15:8]. Set to 0xFFFF to disable for this index.
 * @param[in] index Which PAN ID to set. Must be below
 * RAIL_IEEE802154_MAX_ADDRESSES.
 * @return Status code indicating success of the function call.
 *
 * Set up the 802.15.4 address filter to accept messages to the given PAN ID.
 */
RAIL_Status_t RAIL_IEEE802154_SetPanId(RAIL_Handle_t railHandle,
                                       uint16_t panId,
                                       uint8_t index);

/**
 * Set a short address for 802.15.4 address filtering
 *
 * @param[in] railHandle Handle of RAIL instance
 * @param[in] shortAddr 16 bit short address value. This will be matched against the
 * destination short address of incoming messages. The short address is sent
 * little endian over the air meaning shortAddr[7:0] is first in the payload
 * followed by shortAddr[15:8]. Set to 0xFFFF to disable for this index.
 * @param[in] index Which short address to set. Must be below
 * RAIL_IEEE802154_MAX_ADDRESSES.
 * @return Status code indicating success of the function call.
 *
 * Set up the 802.15.4 address filter to accept messages to the given short
 * address.
 */
RAIL_Status_t RAIL_IEEE802154_SetShortAddress(RAIL_Handle_t railHandle,
                                              uint16_t shortAddr,
                                              uint8_t index);

/**
 * Set a long address for 802.15.4 address filtering
 *
 * @param[in] railHandle Handle of RAIL instance
 * @param[in] longAddr Pointer to a 8 byte array containing the long address
 * information. The long address must be in over the air byte order. This will
 * be matched against the destination long address of incoming messages. Set to
 * 0x00 00 00 00 00 00 00 00 to disable for this index.
 * @param[in] index Which long address to set. Must be below
 * RAIL_IEEE802154_MAX_ADDRESSES.
 * @return Status code indicating success of the function call.
 *
 * Set up the 802.15.4 address filter to accept messages to the given long
 * address.
 */
RAIL_Status_t RAIL_IEEE802154_SetLongAddress(RAIL_Handle_t railHandle,
                                             const uint8_t *longAddr,
                                             uint8_t index);

/**
 * Set whether the current node is a PAN coordinator
 *
 * @param[in] railHandle Handle of RAIL instance
 * @param[in] isPanCoordinator True if this device is a PAN coordinator
 * @return Status code indicating success of the function call.
 *
 * If the device is a PAN Coordinator, then it will accept data and command
 * frames with no destination address. This function will fail if 802.15.4
 * hardware acceleration is not currently enabled. This setting may be changed
 * at any time when 802.15.4 hardware acceleration is enabled.
 */
RAIL_Status_t RAIL_IEEE802154_SetPanCoordinator(RAIL_Handle_t railHandle,
                                                bool isPanCoordinator);

/**
 * Set whether to enable 802.15.4 promiscuous mode
 *
 * @param[in] railHandle Handle of RAIL instance
 * @param[in] enable True if all frames and addresses should be accepted
 * @return Status code indicating success of the function call.
 *
 * If promiscuous mode is enabled, then no frame or address filtering steps
 * will be performed, other than checking the CRC. This function will fail if
 * 802.15.4 hardware acceleration is not currently enabled. This setting may be
 * changed at any time when 802.15.4 hardware acceleration is enabled.
 */
RAIL_Status_t RAIL_IEEE802154_SetPromiscuousMode(RAIL_Handle_t railHandle,
                                                 bool enable);

/// When receiving packets, accept 802.15.4 BEACON frame types
#define RAIL_IEEE802154_ACCEPT_BEACON_FRAMES      (0x01)
/// When receiving packets, accept 802.15.4 DATA frame types
#define RAIL_IEEE802154_ACCEPT_DATA_FRAMES        (0x02)
/// When receiving packets, accept 802.15.4 ACK frame types
/// If this is not enabled, ACK frame types will only be accepted while waiting
/// for an ack
#define RAIL_IEEE802154_ACCEPT_ACK_FRAMES         (0x04)
/// When receiving packets, accept 802.15.4 COMMAND frame types
#define RAIL_IEEE802154_ACCEPT_COMMAND_FRAMES     (0x08)

/// In standard operation, accept BEACON, DATA and COMMAND frames.
/// Only receive ACK frames while waiting for ack
#define RAIL_IEEE802154_ACCEPT_STANDARD_FRAMES (RAIL_IEEE802154_ACCEPT_BEACON_FRAMES \
                                                | RAIL_IEEE802154_ACCEPT_DATA_FRAMES \
                                                | RAIL_IEEE802154_ACCEPT_COMMAND_FRAMES)

/**
 * Set which 802.15.4 frame types to accept
 *
 * @param[in] railHandle Handle of RAIL instance
 * @param[in] framesMask Mask containing which 802.15.4 frame types to receive
 * @return Status code indicating success of the function call.
 *
 * This function will fail if 802.15.4 hardware acceleration is not currently
 * enabled. This setting may be changed at any time when 802.15.4 hardware
 * acceleration is enabled. Only Beacon, Data, Ack, and Command frames may
 * be received. The RAIL_IEEE802154_ACCEPT_XXX_FRAMES defines may be combined
 * to create a bitmask to pass into this function.
 *
 * \ref RAIL_IEEE802154_ACCEPT_ACK_FRAMES behaves slightly different than the
 * other defines. If \ref RAIL_IEEE802154_ACCEPT_ACK_FRAMES is set, the radio
 * will accept an ACK frame during normal packet reception. If \ref
 * RAIL_IEEE802154_ACCEPT_ACK_FRAMES is not set, ACK frames will be filtered
 * unless the radio is waiting for an ACK.
 */
RAIL_Status_t RAIL_IEEE802154_AcceptFrames(RAIL_Handle_t railHandle,
                                           uint8_t framesMask);

/**
 * Set the frame pending bit on the outgoing ACK
 *
 * @param[in] railHandle Handle of RAIL instance
 * @return Status code indicating success of the function call.
 *
 * This function should be called after receiving \ref
 * RAIL_EVENT_IEEE802154_DATA_REQUEST_COMMAND, if the given source address has
 * a pending frame. This will return \ref RAIL_STATUS_INVALID_STATE if it is
 * too late to modify the ACK.
 */
RAIL_Status_t RAIL_IEEE802154_SetFramePending(RAIL_Handle_t railHandle);

/**
 * Get the source address of the incoming data request.
 *
 * @param[in] railHandle A RAIL instance handle.
 * @param[out] pAddress Pointer to \ref RAIL_IEEE802154_Address_t structure
 *   to populate with address information.
 * @return Status code indicating success of the function call.
 *
 * This function should only be called when handling the \ref
 * RAIL_EVENT_IEEE802154_DATA_REQUEST_COMMAND event.
 */
RAIL_Status_t RAIL_IEEE802154_GetAddress(RAIL_Handle_t railHandle,
                                         RAIL_IEEE802154_Address_t *pAddress);

/** @} */ // end of IEEE802.15.4

#ifdef __cplusplus
}
#endif

#endif // __RAIL_IEEE802154_H__
