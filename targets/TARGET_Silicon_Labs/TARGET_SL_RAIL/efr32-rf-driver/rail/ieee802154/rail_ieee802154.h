/***************************************************************************//**
 * @file rail_ieee802154.h
 * @brief The IEEE 802.15.4 specific header file for the RAIL library.
 * @copyright Copyright 2016 Silicon Laboratories, Inc. http://www.silabs.com
 ******************************************************************************/

#ifndef __RAIL_IEEE802154_H__
#define __RAIL_IEEE802154_H__

/**
 * @addtogroup Protocol_Specific
 * @{
 */

/**
 * @addtogroup IEEE802_15_4
 * @brief IEEE 802.15.4 configuration routines
 *
 * The functions in this group configure RAIL IEEE 802.15.4 hardware
 * acceleration. To configure 802.15.4 functionality, call
 * RAIL_IEEE802154_Init(). Make note that this function calls many other RAIL
 * functions; the application is advised to not reconfigure any of these
 * functions.  When using 802.15.4 functionality in the 2.4 GHz band, consider
 * using RAIL_IEEE802154_2p4GHzRadioConfig() instead of RAIL_RadioConfig() and
 * RAIL_ChannelConfig().
 *
 * @code{.c}
 * RAIL_IEEE802154_Config_t config = { false, false,
 *                                     RAIL_IEEE802154_ACCEPT_STANDARD_FRAMES,
 *                                     RAIL_RF_STATE_RX, 100, 192, 894, NULL };
 * RAIL_IEEE802154_2p4GHzRadioConfig();
 * RAIL_IEEE802154_Init(&config);
 * @endcode
 *
 * The application can configure the node's address by using
 * RAIL_IEEE802154_SetAddresses(). Inidividual members can be changed with
 * RAIL_IEEE802154_SetPanId(), RAIL_IEEE802154_SetShortAddress(),
 * RAIL_IEEE802154_SetLongAddress(). RAIL only supports one set of addresses at
 * a time. Beacon addresses are supported by default, without additional
 * configuration.
 *
 * @code{.c}
 * uint8_t longAddress[8] = { 0x11, 0x22, 0x33, 0x44,
 *                            0x55, 0x66, 0x77, 0x88};
 * // PanID OTA value of 0x34 0x12
 * // Short Address OTA byte order of 0x78 0x56
 * // Long address with OTA byte order of 0x11 0x22 0x33 0x44 0x55 0x66 0x77 0x88
 * RAIL_IEEE802154_AddrConfig_t nodeAddress = { 0x1234, 0x5678,
 *                                              &longAddress[0] };
 *
 * bool status = RAIL_IEEE802154_SetAddresses(&nodeAddress);
 *
 * // Alternative methods:
 * status = RAIL_IEEE802154_SetPanId(nodeAddress.panId);
 * status = RAIL_IEEE802154_SetShortAddress(nodeAddress.shortAddr);
 * status = RAIL_IEEE802154_SetLongAddress(nodeAddress.longAddr);
 * @endcode
 *
 * Auto ack is initialized through RAIL_IEEE802154_Init(). It is not advised
 * to call RAIL_AutoAckConfig() while 802.15.4 hardware acceleration is
 * enabled. The default IEEE 802.15.4 ack will have a 5 byte length. The frame
 * type will be an ack. The frame pending bit will be set based on the
 * RAIL_IEEE802154_SetFramePending() function. The sequence number will be set to
 * match the packet being acknowledged. All other frame control fields will be
 * set to 0, in compliance with IEEE Std 802.15.4-2011 section 5.2.2.3.
 * However, the auto ack modification function can be used to control auto
 * acking. Documentation for these functions can be found in \ref Auto_Ack.
 * @{
 */

/**
 * @enum RAIL_IEEE802154_AddressLength_t
 * @brief Different lengths that an 802.15.4 address can have
 */
typedef enum RAIL_IEEE802154_AddressLength
{
  RAIL_IEEE802154_ShortAddress = 2, /**< 2 byte short address. */
  RAIL_IEEE802154_LongAddress = 3, /**< 8 byte extended address. */
} RAIL_IEEE802154_AddressLength_t;

/**
 * @struct RAIL_IEEE802154_Address_t
 * @brief Representation of 802.15.4 address
 * This structure is only used for a received address, which needs to be parsed
 * to discover the type.
 */
typedef struct RAIL_IEEE802154_Address
{
  /**
   * Enum of the received address length
   */
  RAIL_IEEE802154_AddressLength_t length;
  union
  {
    uint16_t shortAddress; /**< Present for 2 byte addresses. */
    uint8_t longAddress[8]; /**< Present for 8 byte addresses. */
  };
} RAIL_IEEE802154_Address_t;

/**
 * @struct RAIL_IEEE802154_AddrConfig_t
 * @brief Configuration structure for IEEE 802.15.4 Address Filtering. The
 * broadcast addresses are handled separately, and do not need to be specified
 * here. Any address which is NULL will be ignored.
 */
typedef struct RAIL_IEEE802154_AddrConfig
{
  uint16_t panId; /**< PAN ID for destination filtering. */
  uint16_t shortAddr; /**< Network address for destination filtering. */
  uint8_t *longAddr; /**< 64 bit address for destination filtering. In OTA byte order.*/
} RAIL_IEEE802154_AddrConfig_t;

/**
 * @struct RAIL_IEEE802154_Config_t
 * @brief Configuration structure for IEEE 802.15.4 in RAIL
 */
typedef struct RAIL_IEEE802154_Config {
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
  /**
   * Set which 802.15.4 frame types will be received, of Beacon, Data, Ack, and
   * Command. This setting can be overridden via RAIL_IEEE802154_AcceptFrames().
   */
  uint8_t framesMask;
  /**
   * Defines the default radio state after a transmit operation (transmit
   * packet, wait for ack) or a receive operation (receive packet, transmit
   * ack) finishes.
   */
  RAIL_RadioState_t defaultState;
  /**
   * Define the idleToRx and idleToTx time
   * This defines the time it takes for the radio to go into RX or TX from an
   * idle radio state
   */
  uint16_t idleTime;
  /**
   * Define the turnaround time after receiving a packet and transmitting an
   * ack and vice versa
   */
  uint16_t turnaroundTime;
  /**
   * Define the ack timeout time in microseconds
   */
  uint16_t ackTimeout;
  /**
   * Configure the RAIL Address Filter to allow the given destination
   * addresses. If addresses is NULL, defer destination address configuration.
   * If a member of addresses is NULL, defer configuration of just that member.
   * This can be overridden via RAIL_IEEE802154_SetAddresses(), or the
   * individual members can be changed via RAIL_IEEE802154_SetPanId(),
   * RAIL_IEEE802154_SetShortAddress(), and RAIL_IEEE802154_SetLongAddress().
   */
  RAIL_IEEE802154_AddrConfig_t *addresses;
} RAIL_IEEE802154_Config_t;

/**
 * Initialize RAIL for IEEE802.15.4 features
 *
 * @param[in] config IEEE802154 configuration struct
 * @return \ref RAIL_STATUS_NO_ERROR if successfully configured.
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
 * - RAIL_AutoAckConfig()
 * - RAIL_SetRxTransitions()
 * - RAIL_SetTxTransitions()
 * - RAIL_SetStateTiming()
 * - RAIL_AddressFilterConfig()
 * - RAIL_AddressFilterEnable()
 */
RAIL_Status_t RAIL_IEEE802154_Init(RAIL_IEEE802154_Config_t *config);

/**
 * Configures the radio for 2.4GHz 802.15.4 operation
 *
 * @return \ref RAIL_STATUS_NO_ERROR if successfully configured.
 *
 * This initializes the radio for 2.4GHz operation. It takes the place of
 * calling \ref RAIL_RadioConfig and \ref RAIL_ChannelConfig. After this call,
 * channels 11-26 will be available, giving the frequencies of those channels
 * on channel page 0, as defined by IEEE 802.15.4-2011 section 8.1.2.2.
 */
RAIL_Status_t RAIL_IEEE802154_2p4GHzRadioConfig(void);

/**
 * De-initializes IEEE802.15.4 hardware acceleration
 *
 * @return 0 if IEEE802.15.4 hardware acceleration is successfully
 * deinitialized. Error code on failure
 *
 * Disables and resets all IEE802.15.4 hardware acceleration features. This
 * function should only be called when the radio is IDLE. This calls the
 * following:
 * - RAIL_AutoAckDisable(), which resets the state transitions to IDLE
 * - RAIL_SetStateTiming(), to reset all timings to 100 us
 * - RAIL_AddressFilterDisable()
 * - RAIL_AddressFilterReset()
 */
RAIL_Status_t RAIL_IEEE802154_Deinit(void);

/**
 * Return whether IEEE802.15.4 hardware accelertion is currently enabled.
 *
 * @return True if IEEE802.15.4 hardware acceleration was enabled to start with
 * and false otherwise
 */
bool RAIL_IEEE802154_IsEnabled(void);

/**
 * Configure the RAIL Address Filter for 802.15.4 filtering
 *
 * @param[in] addresses The address information that should be used
 * @return True if addresses were successfully set, false otherwise
 *
 * Set up the 802.15.4 address filter to accept messages to the given
 * addresses. This will return true if at least one address was successfully
 * stored to be used.
 */
bool RAIL_IEEE802154_SetAddresses(RAIL_IEEE802154_AddrConfig_t *addresses);

/**
 * Set a PAN ID for 802.15.4 address filtering
 *
 * @param[in] panId The 16-bit PAN ID information.
 * This will be matched against the destination PAN ID of incoming messages.
 * The PAN ID is sent little endian over the air meaning panId[7:0] is first in
 * the payload followed by panId[15:8].
 * @return True if the PAN ID was successfully set, false otherwise
 *
 * Set up the 802.15.4 address filter to accept messages to the given PAN ID.
 */
bool RAIL_IEEE802154_SetPanId(uint16_t panId);

/**
 * Set a short address for 802.15.4 address filtering
 *
 * @param[in] shortAddr 16 bit short address value. This will be matched against the
 * destination short address of incoming messages. The short address is sent
 * little endian over the air meaning shortAddr[7:0] is first in the payload
 * followed by shortAddr[15:8].
 * @return True if the short address was successfully set, false otherwise
 *
 * Set up the 802.15.4 address filter to accept messages to the given short
 * address.
 */
bool RAIL_IEEE802154_SetShortAddress(uint16_t shortAddr);

/**
 * Set a long address for 802.15.4 address filtering
 *
 * @param[in] longAddr Pointer to a 8 byte array containing the long address
 * information. The long address must be in over the air byte order. This will
 * be matched against the destination long address of incoming messages.
 * @return True if the long address was successfully set, false otherwise
 *
 * Set up the 802.15.4 address filter to accept messages to the given long
 * address.
 */
bool RAIL_IEEE802154_SetLongAddress(uint8_t *longAddr);

/**
 * Set whether the current node is a PAN coordinator
 *
 * @param[in] isPanCoordinator True if this device is a PAN coordinator
 * @return Returns zero on success and an error code on error
 *
 * If the device is a PAN Coordinator, then it will accept data and command
 * frames with no destination address. This function will fail if 802.15.4
 * hardware acceleration is not currently enabled. This setting may be changed
 * at any time when 802.15.4 hardwarea acceleration is enabled.
 */
RAIL_Status_t RAIL_IEEE802154_SetPanCoordinator(bool isPanCoordinator);

/**
 * Set whether to enable 802.15.4 promiscuous mode
 *
 * @param[in] enable True if all frames and addresses should be accepted
 * @return Returns zero on success and an error code on error
 *
 * If promiscuous mode is enabled, then no frame or address filtering steps
 * will be performed, other than checking the CRC. This function will fail if
 * 802.15.4 hardware acceleration is not currently enabled. This setting may be
 * changed at any time when 802.15.4 hardware acceleration is enabled.
 */
RAIL_Status_t RAIL_IEEE802154_SetPromiscuousMode(bool enable);

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
#define RAIL_IEEE802154_ACCEPT_STANDARD_FRAMES (RAIL_IEEE802154_ACCEPT_BEACON_FRAMES | \
                                                RAIL_IEEE802154_ACCEPT_DATA_FRAMES | \
                                                RAIL_IEEE802154_ACCEPT_COMMAND_FRAMES)

/**
 * Set which 802.15.4 frame types to accept
 *
 * @param[in] framesMask Mask containing which 802.15.4 frame types to receive
 * @return Returns zero on success and an error code on error
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
RAIL_Status_t RAIL_IEEE802154_AcceptFrames(uint8_t framesMask);

/**
 * Callback for when a Data Request is being received
 *
 * @param address The source address of the data request command
 *
 * This function is called when the command byte of an incoming frame is for a
 * data request, which requests an ACK. This callback will be called before the
 * packet is fully received, to allow the node to have more time to decide
 * whether to set frame pending in the outgoing ACK.
 */
void RAILCb_IEEE802154_DataRequestCommand(RAIL_IEEE802154_Address_t *address);

/**
 * Set the frame pending bit on the outgoing ACK
 *
 * @return Returns zero on success and an error code on error
 *
 * This function should be called after receiving
 * RAILCb_IEEE802154_DataRequestCommand(), if the given source address has a
 * pending frame. This will return \ref RAIL_STATUS_INVALID_STATE if it is too
 * late to modify the ACK.
 */
RAIL_Status_t RAIL_IEEE802154_SetFramePending(void);

/**
 * @}
 * end of IEEE802.15.4
 */

/**
 * @}
 * end of Protocol_Specific
 */

#endif // __RAIL_IEEE802154_H__
