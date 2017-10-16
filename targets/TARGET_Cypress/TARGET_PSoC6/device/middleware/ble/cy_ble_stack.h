/*******************************************************************************
* \file cy_ble_stack.h
* \version 2.0
*
* \brief
* This file contains the BLE Stack, which will be updated on the Jenkins
* build and the common structures, APIs.
*
*******************************************************************************
* Copyright 2017, Cypress Semiconductor Corporation.  All rights reserved.
* You may use this file only in accordance with the license, terms, conditions,
* disclaimers, and limitations in the end user license agreement accompanying
* the software package with which this file was provided.
******************************************************************************/

#ifndef CY_BLE_STACK_H_
#define CY_BLE_STACK_H_

/***************************************
* Common stack includes
***************************************/
#include "BLE_config.h"

/***************************************
** Constants
***************************************/

/** Stack Build Version Constants - It will be updated by Jenkins during build.*/

/** Major version number for device firmware. */
#define CY_BLE_STACK_MAJOR                                               (5u)

/** Minor version number for device firmware. */
#define CY_BLE_STACK_MINOR                                               (0u)

/** Patch version number for device firmware. */
#define CY_BLE_STACK_PATCH                                               (0u)

/** Firmware build number. */
#define CY_BLE_STACK_BUILD                                               (785u)

/** BT Address Size */
#define CY_BLE_BD_ADDR_SIZE                                              (0x06u)

/** BLE Middleware ID */
#define CY_BLE_ID                                                        (0x05ul << 18u)

/** Memory pool configuration data defines*/

/** Size of internal data buffer structures */
#define CY_BLE_STACK_BUFFER_MGR_UTIL_RAM_SZ                              (0x1Cu)

/** Memory extension offset */
#define CY_BLE_MEM_EXT_SZ                                                (0x08u)

/** Feature mask for 4.2 features */
/** DLE feature */
#define CY_BLE_DLE_FEATURE_MASK                                          (0x01u)
/** Privacy 1.2 feature */
#define CY_BLE_PRIVACY_1_2_FEATURE_MASK                                  (0x02u)
/** Secured connection feature */
#define CY_BLE_SECURE_CONN_FEATURE_MASK                                  (0x04u)

/** Feature mask for Shanghai features */
#define CY_BLE_PHY_UPDATE_FEATURE_MASK                                   (0x08u)

/** Feature mask for persistent Store */
/** Bondlist */
#define CY_BLE_PERSISTENT_STORE_BONDLIST                                 (0x10u)
/** Resolving list */
#define CY_BLE_PERSISTENT_STORE_RESOLVING_LIST                           (0x20u)
/** whiteList */
#define CY_BLE_PERSISTENT_STORE_WHITELIST                                (0x40u)
/** Radio power calibration*/
#define CY_BLE_PERSISTENT_RADIO_CALIBRATION_MASK                         (0x80u)

/** Feature mask bit-mask */
#define CY_BLE_FEATURE_BIT_MASK                                          (0x0FFu)

/** Default RPA List Entry Size */
#define CY_BLE_DEFAULT_RPA_LIST_SZ                                       (0x10u)

/** Default Bonded device List entry size */
#define CY_BLE_DEFAULT_BONDED_DEVICE_LIST_SZ                             (0x10u)

/** Resolving list memory computation 
 * CY_BLE_MAX_RESOLVABLE_DEVICES: 
 *      It is user configured value for size of resolving list using BLE PDL.
 *             Range: 1 to 16, 0 if Privacy 1.2 feature is not used.
 * Memory required =
 * (((CY_BLE_LL_PRIVACY_HEAP_REQ * CY_BLE_MAX_RESOLVABLE_DEVICES) +
                                            CY_BLE_LL_PRIVACY_RETENTION_HEAP_REQ)
 * Note:
 * This configurable memory should be added to 
 * CY_BLE_CONTROLLER_DEVICE_SPECIFIC_HEAP_REQ 
 * static macro value passed to BLE Stack. 
 */
 
/** Internal RAM required for resolving list retention data
 * structure
 */
#define CY_BLE_LL_PRIVACY_RETENTION_HEAP_REQ                             (0x0Cu)

/** Internal RAM required for single resolving list entry
 */
#define CY_BLE_LL_PRIVACY_HEAP_REQ                                       (0x44u)

/** Whitelist list memory computation 
 * CY_BLE_MAX_WHITELIST_LIST_SZ: 
 *        It is a user-configured value for the size of the Whitelist using the BLE PDL.
 *             Range: 1 to 16.
 * Memory required =
 * (((CY_BLE_LL_ONE_WHITELIST_HEAP_REQ * CY_BLE_MAX_WHITELIST_LIST_SZ) +
                                            CY_BLE_LL_WHITELIST_RETENTION_HEAP_REQ)
 * Note:
 * This configurable memory should be added to 
 * the CY_BLE_CONTROLLER_DEVICE_SPECIFIC_HEAP_REQ 
 * static macro value passed to the BLE Stack. 
 */
 
/** Max WhiteList size supported by BLE Stack */
#define CY_BLE_MAX_WHITELIST_LIST_SZ                                     (0x10u)

/** LL Whitelist retention heap requirement */
#define CY_BLE_LL_WHITELIST_RETENTION_HEAP_REQ                           (0x08u)

/** LL whiteList size for one entity */
#define CY_BLE_LL_ONE_WHITELIST_HEAP_REQ                                 (0x08u)

/** Convert Octets to Bits. */
#define CY_BLE_OCTETS_TO_BITS(octets)                                    ((octets) << 3)

/** Convert LL Data Octets to Time in micro-seconds - 1Mbps */
#define CY_BLE_DATA_OCTETS_TO_US_1MBPS(octets)                           (CY_BLE_OCTETS_TO_BITS((octets) + 14))
/** Convert LL Data Octets to Time in micro-seconds - 2Mbps */
#define CY_BLE_DATA_OCTETS_TO_US_2MBPS(octets)                           (CY_BLE_OCTETS_TO_BITS(((octets) + 15) >> 1))


/** Total Command Buffers Chunks for Command Buffer Pool */
#define CY_BLE_BT_FW_LE_CMD_BUFFERS                                      (0x05u)

/** Event Buffer Pool 1 */
#define CY_BLE_BT_FW_LE_EVENT_TYPE1_BUFFERS                              (0x01u)

/** Event Buffer Pool 2 */
#define CY_BLE_BT_FW_LE_EVENT_TYPE2_BUFFERS                              (0x08u)

/** Per Connection LL Control PDU Size for Tx */
#define CY_BLE_BT_FW_TX_LL_PDU_Q_SZ                                      (0x02u)

/** Per Connection LL Control PDU Size for Rx */
#define CY_BLE_BT_FW_RX_LL_PDU_Q_SZ                                      (0x02u)

/** Firmware total LE pdu packets */
#define CY_BLE_BT_FW_LE_TOTAL_PDU_PKTS         (CY_BLE_BT_FW_TX_LL_PDU_Q_SZ + \
                                    CY_BLE_BT_FW_RX_LL_PDU_Q_SZ)

/**  Encryption and Connection Parameter Update  */
#define CY_BLE_BT_FW_LE_TXN_BUFFERS                                      (0x02u)


/** HCI Command Buffers per single Command Buffer Pool*/
#define CY_BLE_HCI_LE_CMD_BUFFER_SZ                                      (0x50u)

/** Event packet size
 * Max size for HCI LE event is 64 bytes
 * Total size = 2 * (64 + 4(Event Header) + 4)
 */
#define CY_BLE_HCI_LE_EVENT_BUFFER_POOL_TYPE1_SZ                         (0x48u)

/** Event buffer pool for rest of the le single mode events.*/
#define CY_BLE_HCI_LE_EVENT_BUFFER_POOL_TYPE2_SZ                         (0x30u)

/**
 * LL Control Data channel PDU =
 * 27 bytes
 * MIC = 4 Bytes
 * 1 byte llh_connection_index
 * Following buffers are reserved for a single connection
 * for Multiple connections deciding the actual number of PDU buffers
 */
#define CY_BLE_LL_PDU_MAX_BUFFER_SZ                                      (0x20u)

/** As of now 64 bytes per txn */
#define CY_BLE_LL_LE_TXN_BUFFER_SZ                                       (0x40u)

/** Memory pool item size*/
#define CY_BLE_MEM_POOL_ITEM_SZ                                          (0x04u)

/** BLE Dle device param size */
#define CY_BLE_DLE_DEVICE_PARAM_SZ                                       (0x10u)

/** controller per connection data struct heap size */
#define CY_BLE_CONTROLLER_PER_CONN_DATA_STRUCT_HEAP_SZ                   (0x0110u)

/** Heap Reserve for Controller Stack */
#define CY_BLE_BT_FW_HEAP_RESERVE_REQ                                    (0x80u)

/** Device Specific Pool Memory for BLESS RX FIFO, HCI Command and
 * HCI Events memory */
#define CY_BLE_BT_FW_DEV_SPECIFIC_POOL_REQ        (CY_BLE_BT_FW_RX_ACL_MEM_POOL_REQ + \
                                                  CY_BLE_BT_FW_CMD_MEM_POOL_REQ + \
                                                   CY_BLE_BT_FW_EVENT_MEM_POOL_REQ)
/** device specific controller heap requirement  */
#define CY_BLE_CONTROLLER_DEVICE_SPECIFIC_HEAP_REQ                             \
                                       ((CY_BLE_BT_FW_LE_EVENT_TYPE1_BUFFERS * \
        (CY_BLE_HCI_LE_EVENT_BUFFER_POOL_TYPE1_SZ + CY_BLE_MEM_POOL_ITEM_SZ)) + \
                                        (CY_BLE_BT_FW_LE_EVENT_TYPE2_BUFFERS * \
        (CY_BLE_HCI_LE_EVENT_BUFFER_POOL_TYPE2_SZ + CY_BLE_MEM_POOL_ITEM_SZ)) + \
                                                (CY_BLE_BT_FW_LE_CMD_BUFFERS * \
                     (CY_BLE_HCI_LE_CMD_BUFFER_SZ + CY_BLE_MEM_POOL_ITEM_SZ)) + \
                                       ((CY_BLE_STACK_BUFFER_MGR_UTIL_RAM_SZ * \
                                            CY_BLE_BT_FW_DEV_SPECIFIC_POOL_REQ) + \
                                                    CY_BLE_BT_FW_HEAP_RESERVE_REQ))


/** Configurable Memory for Stack Manager Buffer Pool Data Structures for
 * buffer pools that are created and maintained.
 */
 /** Tx acl memory requirement */
#define CY_BLE_BT_FW_TX_ACL_MEM_POOL_REQ                                    (0x01u)
/** firmware pdu memory requirement */
#define CY_BLE_BT_FW_PDU_MEM_POOL_REQ                                       (0x02u)
/** firmware transaction memory requirement */
#define CY_BLE_BT_FW_TXN_MEM_POOL_REQ                                       (0x01u)

/** Heap required to for buffer pools that are created per connection
 * Formula for total pools:
 * (4 <Tx, Tx LL, Rx LL, LL Transaction Pool> * no. of connections)
 */
#define CY_BLE_BT_FW_CONN_SPECIFIC_POOL_REQ       (CY_BLE_BT_FW_TX_ACL_MEM_POOL_REQ + \
                                                  CY_BLE_BT_FW_PDU_MEM_POOL_REQ + \
                                                     CY_BLE_BT_FW_TXN_MEM_POOL_REQ)
/** controller connection specific heap requirement */
 #define CY_BLE_CONTROLLER_CONN_SPECIFIC_HEAP_REQ                              \
                                            ((CY_BLE_BT_FW_LE_TOTAL_PDU_PKTS * \
                     (CY_BLE_LL_PDU_MAX_BUFFER_SZ + CY_BLE_MEM_POOL_ITEM_SZ)) + \
                                                (CY_BLE_BT_FW_LE_TXN_BUFFERS * \
                      (CY_BLE_LL_LE_TXN_BUFFER_SZ + CY_BLE_MEM_POOL_ITEM_SZ)) + \
                                           (CY_BLE_BT_FW_CONN_SPECIFIC_POOL_REQ * \
                                        CY_BLE_STACK_BUFFER_MGR_UTIL_RAM_SZ) + \
                               (CY_BLE_CONTROLLER_PER_CONN_DATA_STRUCT_HEAP_SZ))

/** Rx acl memory requirement */
#define CY_BLE_BT_FW_RX_ACL_MEM_POOL_REQ                                    (0x01u)
/** firmware command memory requirement */
#define CY_BLE_BT_FW_CMD_MEM_POOL_REQ                                       (0x01u)
/** firmware event memory requirement */
#define CY_BLE_BT_FW_EVENT_MEM_POOL_REQ                                     (0x02u)

/** Size of the heap when the BLE Stack is built in HCI mode */
#define CY_BLE_DEFAULT_CONTROLLER_RAM_SIZE                                  (0u)

/** Default Max ACL Packet Size for LE-DATA packet both for */
#define CY_BLE_LL_DEFAULT_MAX_SUPPORTED_ACL_BUFFER_SZ                    (0xFCu)

/** Default Max ACL Packet Size for Rx LE-DATA Packet
 * Formulae: Default ACL Size (27 bytes) ~ AlignToWord(27) =  28
 * */
#define CY_BLE_LL_DEFAULT_ACL_MAX_RX_BUFFER_SZ                           (0x1Cu)

/** 8 Additional Bytes = (4 bytes of MIC + 4 bytes of HCI header) */
#define CY_BLE_LL_ACL_DATA_PACKET_OVERHEAD_SZ                            (0x08u)

/** Number of Rx ACL Packet buffers, this shall not change
 * as this should not be user configurable parameter */
#define CY_BLE_LL_DEFAULT_NUM_ACL_RX_PACKETS                             (0x04u)

/** Number of Tx ACL Packet buffers */
#define CY_BLE_LL_DEFAULT_NUM_ACL_TX_PACKETS                             (0x03u)

/** Default Tx capability. If DLE is not enabled, this is Tx capability*/
#define CY_BLE_LL_DEFAULT_TX_CAPABILITY                                  (0x1Bu)

/** Default Rx capability. If DLE is not enabled, this is Rx capability*/
#define CY_BLE_LL_DEFAULT_RX_CAPABILITY                                  (0x1Bu)

/** Default Tx capability. If DLE is not enabled, this is Tx capability*/
#define CY_BLE_MAX_DEFAULT_TX_CAPABILITY                                 (0xFBu)

/** Default Rx capability. If DLE is not enabled, this is Rx capability*/
#define CY_BLE_MAX_DEFAULT_RX_CAPABILITY                                 (0xFBu)

/** Internal heap required for LE Data Length Extension Feature Per
 * Connection */
#define CY_BLE_LL_DLE_HEAP_REQ                                           (0x40u)

/** DLE heap required for device specific parameters */
#define CY_BLE_LL_DLE_DEVICE_PARAM_HEAP_REQ                              (0x10u)

/** Default RPA List Entry Size
 * DKSH: RPA list support is limited to 8 only for time being */
#define CY_BLE_MAX_RPA_LIST_SZ                                           (0x10u)

/** CY pipe client IDs for BLE */
#define CY_BLE_PIPE_BLE_MSG_COMPLETE_ID                                  (0u) /**< msg complete */
#define CY_BLE_PIPE_BLE_MSG_SEND_ID                                      (1u) /**< msg send */

/** Mask to disable all events */
#define CY_BLE_DISABLE_ALL_EVENTS_MASK                                   (0x00u)

/** Mask to Enable connection established events */
#define CY_BLE_CONN_ESTB_EVENT_MASK                                      (0x01u)

/** Length of mask byte array */
#define CY_BLE_EVENT_MASK_LENGTH                                         (1u)

/** Event Mask index 0 */
#define CY_BLE_EVENT_MASK_INDEX_0                                        (0u)

/***************************************
** Enumerated Types
***************************************/

/**
 \addtogroup group_ble_common_api_events
 @{
*/

/** BLE stack events */
typedef enum
{
    /** This event is triggered by the BLE stack when the stack is in a bad state. Restarting the stack is the
        only way to get out of the state */
    CY_BLE_EVT_INVALID = 0x0000u,

    /* Range for Generic events - 0x1000 to 0x1FFF */

    /** This event is received when the BLE stack is initialized and turned ON by invoking the Cy_BLE_StackInit () function.*/
    CY_BLE_EVT_STACK_ON = 0x1000u,

    /** This event is received when there is a timeout and the application must handle the event.
        Event parameter is of type  'cy_stc_ble_timeout_param_t'.
        - If reasonCode is 'CY_BLE_GATT_RSP_TO', then look for connHandle.
        - If reasonCode is 'CY_BLE_GENERIC_APP_TO', then look for timerHandle.
        - All other cases connHandle or timerHandle is ignored*/
    CY_BLE_EVT_TIMEOUT, /* 0x1001u */

    /** This event is triggered by host stack whether the BLE stack is busy or not.
       The Event Parameter corresponding to this event will indicate the state of BLE stack's internal protocol buffers
       for the application to safely initiate data transactions (GATT, GAP Security, and L2CAP transactions)
       with the peer BLE device.
       Event parameter is of type cy_stc_ble_l2cap_state_info_t.
       cy_stc_ble_l2cap_state_info_t contains flowState and Bd handle as parameters
       flowState indicates the following state:
        * CY_BLE_STACK_STATE_BUSY (0x01) = CY_BLE_STACK_STATE_BUSY indicates to the application that the BLE Stack's internal buffers
        *                   are about to be filled, and the remaining buffers are required to respond to the peer BLE device.
        *                   After this event, the application shall not initiate GATT, GAP Security, or L2CAP data transactions.
        *                   However, the application shall respond to peer initiated transactions to prevent BLE protocol timeouts
        *                   from occuring.
        *                   Application-initiated data transactions can be resumed after the CY_BLE_EVT_STACK_BUSY_STATUS
        *                   event with parameter 'CY_BLE_STACK_STATE_FREE' is received.
        *
        * CY_BLE_STACK_STATE_FREE (0x00) = CY_BLE_STACK_STATE_FREE indicates to the application that pending transactions are completed
        *                   and sufficient buffers are available to process application-initiated transactions.
        *                   The 'CY_BLE_EVT_STACK_BUSY_STATUS' event with 'CY_BLE_STACK_STATE_FREE' indicates to the
        *                   application whether the BLE Stack's internal buffer state has transitioned from 'CY_BLE_STACK_STATE_BUSY'
        *                   to 'CY_BLE_STACK_STATE_FREE'.
        *
        Bd handle indicates connection.
       To increase the BLE Stack's default queue depth(CY_BLE_L2CAP_STACK_Q_DEPTH_PER_CONN) and achieve better throughput for the attribute MTU greater than 32,
       use the AddQdepthPerConn parameter in the Expression view of the Advanced tab.
    */
    CY_BLE_EVT_STACK_BUSY_STATUS, /* 0x1002u */

    /** This event is received when the stack wants the application to provide memory to process a remote request.
       The event parameter is of type cy_stc_ble_memory_request_t.
       This event is automatically handled by the component for the CY_BLE_PREPARED_WRITE_REQUEST request.
       The component allocates sufficient memory for the long write request with the assumption that attribute MTU size
       is negotiated to the minimum possible value. The application could use dynamic memory allocation to save static
       RAM memory consumption. To enable this event for the application level, set the EnableExternalPrepWriteBuff parameter
       in the Expression view of the Advanced tab to be true.
    */
    CY_BLE_EVT_MEMORY_REQUEST, /* 0x1003u */

    /** This event is used to inform the application that a flash write is pending.
            Stack internal data structures are modified and require backup. */
    CY_BLE_EVT_PENDING_FLASH_WRITE, /* 0x1004u */

    /** This event is used to inform the application that persistent data stored in flash memory is corrupted
      */
    CY_BLE_EVT_FLASH_CORRUPT,  /* 0x1005u */ 

    /* Range for HCI events - 0x2000 to 0x2FFF */

    /** This event indicates that some internal hardware error has occurred.
      * Reset of the hardware may be required. 
      * Event parameter is (uint8 *). The possible error codes for this event are defined 
      * in cy_ble_stack_host_error.h file.
      */
    CY_BLE_EVT_HARDWARE_ERROR = 0x2000,

    /** This event is triggered on successful setting of Authentication Payload timeout in the BLE Controller for
     * LE_PING feature. Refer to Bluetooth 4.1 core specification, Volume 6, Part B,
     * section 4.6.5 for LE Ping operation.
     * Event parameter is (cy_stc_ble_events_param_generic_t*)
     * eventParams member of cy_stc_ble_events_param_generic_t is bdHandle (uint8_t*)
     * eventParams is valid only if status is success (0x00)
     */
    CY_BLE_EVT_WRITE_AUTH_PAYLOAD_TO_COMPLETE, /* 0x2001u */

    /** This event carries Authentication Payload timeout in the BLE Controller for the LE_PING feature.
     * Refer to Bluetooth 4.1 core specification, Volume 6, Part B, section 4.6.5 for LE Ping operation.
     * Event parameter is (cy_stc_ble_events_param_generic_t*)
     * eventParams member of cy_stc_ble_events_param_generic_t is (cy_stc_ble_auth_payload_info_t*)
     * eventParams is valid only if status is success (0x00)
    */
    CY_BLE_EVT_READ_AUTH_PAYLOAD_TO_COMPLETE, /* 0x2002u */

    /** This event indicates a channel map corresponding to one connection.
     * The event parameter is (cy_stc_ble_events_param_generic_t*)
     * eventParams member of cy_stc_ble_events_param_generic_t is (cy_stc_ble_channel_map_info_t*)
     * eventParams is valid only if status is success (0x00)
     */
    CY_BLE_EVT_GET_CHANNEL_MAP_COMPLETE, /* 0x2003u */

    /** This event indicates completion of the Set LE event mask.
     * The event parameter is (cy_stc_ble_events_param_generic_t*)
     * eventParams member of cy_stc_ble_events_param_generic_t is ignored
     */
    CY_BLE_EVT_LE_SET_EVENT_MASK_COMPLETE, /* 0x2004u */

    /** LE PING Authentication Timeout Event to indicate that the peer device has not responded
        with the valid MIC packet within the application configured ping authentication time. */
    CY_BLE_EVT_LE_PING_AUTH_TIMEOUT, /* 0x2005u */

    /** This event indicates completion of the Set data length command.
     * Event parameter is (cy_stc_ble_events_param_generic_t*)
     * eventParams member of cy_stc_ble_events_param_generic_t is bdHandle (uint8_t*)
     * eventParams is valid only if status is success (0x00)
     */
    CY_BLE_EVT_SET_DATA_LENGTH_COMPLETE, /* 0x2006u */

    /** This event indicates completion of Set suggested data length command.
     * The event parameter is (cy_stc_ble_events_param_generic_t*)
     * eventParams member of cy_stc_ble_events_param_generic_t is ignored
     */
    CY_BLE_EVT_SET_SUGGESTED_DATA_LENGTH_COMPLETE, /* 0x2007u */

    /** This event indicates completion of Cy_BLE_GetDataLength API.
     * The event parameter is (cy_stc_ble_events_param_generic_t*)
     * eventParams member of cy_stc_ble_events_param_generic_t is cy_stc_ble_data_length_param_t*)
     * eventParams is valid only if status is success (0x00)
     */
    CY_BLE_EVT_GET_DATA_LENGTH_COMPLETE, /* 0x2008u */

    /** The LE Data Length Change event notifies the Host of a change to either the maximum Payload length or
      * the maximum transmission time of Data Channel PDUs in either direction. The values reported are the maximum
      * that will actually be used on the connection following the change.
      * The event parameter is of type 'cy_stc_ble_data_length_change_event_param_t'
      */
    CY_BLE_EVT_DATA_LENGTH_CHANGE, /* 0x2009u */

    /** This event indicates peer resolvable private address currently used by the BLE Controller.
     * Event parameter is (*cy_stc_ble_events_param_generic_t)
     * eventParams member of cy_stc_ble_events_param_generic_t is peer resolvable address (uint8_t*)
     * eventParams is valid only if status is success (0x00)
     */
    CY_BLE_EVT_GET_PEER_RPA_COMPLETE, /* 0x200Au */

    /** This event indicates local resolvable private address currently used by the BLE Controller.
     * The event parameter is (*cy_stc_ble_events_param_generic_t)
     * eventParams member of cy_stc_ble_events_param_generic_t is local resolvable address (uint8_t*)
     * eventParams is valid only if status is success (0x00)
     */
    CY_BLE_EVT_GET_LOCAL_RPA_COMPLETE, /* 0x200Bu */

    /** This event indicates completion of the Set RPA timeout command.
     * The event parameter is (cy_stc_ble_events_param_generic_t*)
     * eventParams member of cy_stc_ble_events_param_generic_t is ignored
     */
    CY_BLE_EVT_SET_RPA_TO_COMPLETE, /* 0x200Cu */

    /** This event indicates completion of Set RPA enable command.
     * The event parameter is (cy_stc_ble_events_param_generic_t*)
     * eventParams member of cy_stc_ble_events_param_generic_t is ignored
     */
    CY_BLE_EVT_SET_RPA_ENABLE_COMPLETE, /* 0x200Du */

    /** This event indicates completion of the Set host channel command.
     * the event parameter is (cy_stc_ble_events_param_generic_t*)
     * eventParams member of cy_stc_ble_events_param_generic_t is ignored
     */
    CY_BLE_EVT_SET_HOST_CHANNEL_COMPLETE, /* 0x200Eu */

    /** This event indicates completion of the add device to resolving list command.
     * The event parameter is (cy_stc_ble_events_param_generic_t*)
     * eventParams member of cy_stc_ble_events_param_generic_t is ignored
     */
    CY_BLE_EVT_ADD_DEVICE_TO_RPA_LIST_COMPLETE,  /* 0x200Fu */

    /** This event indicates completion of the remove device from resolving list command.
     * The event parameter is (cy_stc_ble_events_param_generic_t*)
     * eventParams member of cy_stc_ble_events_param_generic_t is ignored
     */
    CY_BLE_EVT_REMOVE_DEVICE_FROM_RPA_LIST_COMPLETE,  /* 0x2010u */

    /** This event indicates completion of the add device to white list command.
      * The event parameter is (cy_stc_ble_events_param_generic_t*)
      * eventParams member of cy_stc_ble_events_param_generic_t is ignored
      */
    CY_BLE_EVT_ADD_DEVICE_TO_WHITE_LIST_COMPLETE, /* 0x2011u */

    /** This event indicates completion of the remove device from the Whitelist command.
         * The event parameter is (cy_stc_ble_events_param_generic_t*)
         * eventParams member of cy_stc_ble_events_param_generic_t is ignored
         */
    CY_BLE_EVT_REMOVE_DEVICE_FROM_WHITE_LIST_COMPLETE, /* 0x2012u */

    /** This event indicates completion of the Cy_BLE_GetPhy API.
     * The event parameter is (cy_stc_ble_events_param_generic_t*)
     * eventParams member of cy_stc_ble_events_param_generic_t is (cy_stc_ble_phy_param_t*)
     * eventParams is valid only if status is Success(0x00)
     */
    CY_BLE_EVT_GET_PHY_COMPLETE, /* 0x2013u */

    /** This event indicates completion of the Cy_BLE_SetDefaultPhy API.
     * The event parameter is (cy_stc_ble_events_param_generic_t*)
     * eventParams member of cy_stc_ble_events_param_generic_t is NULL
     */
    CY_BLE_EVT_SET_DEFAULT_PHY_COMPLETE, /* 0x2014u */

    /** This event indicates completion of the Cy_BLE_SetPhy API.
     * The event parameter is (cy_stc_ble_events_param_generic_t*)
     * eventParams member of cy_stc_ble_events_param_generic_t is NULL
     */
    CY_BLE_EVT_SET_PHY_COMPLETE, /* 0x2015u */

    /** This event indicates that the Controller has changed the transmitter PHY or receiver PHY in use.
     * The event parameter is (cy_stc_ble_events_param_generic_t*)
     * eventParams member of cy_stc_ble_events_param_generic_t is (cy_stc_ble_phy_param_t*)
     * eventParams is valid only if status is Success(0x00)
     */
    CY_BLE_EVT_PHY_UPDATE_COMPLETE,  /* 0x2016u */

    /** This event indicates completion of the set privacy mode command.
     * The event parameter is (cy_stc_ble_events_param_generic_t*)
     * eventParams member of cy_stc_ble_events_param_generic_t is ignored
     */
    CY_BLE_EVT_SET_PRIVACY_MODE_COMPLETE,  /* 0x2017u */

    /* Range for vendor events - 0x3000 to 0x5FFF */

    /** This event indicates completion of the Cy_BLE_isLLControlProcPending API.
     * The event parameter is (cy_stc_ble_events_param_generic_t*)
     * eventParams member of cy_stc_ble_events_param_generic_t is (cy_stc_ble_ll_cntrl_proc_param_t*)
     * eventParams is valid only if status is Success(0x00)
     */
    CY_BLE_EVT_LL_CNTRL_PROC_PENDING_COMPLETE = 0x3000u,

    /** This event indicates a soft reset completed successfully.
      * The event parameter is NULL.
      */
    CY_BLE_EVT_SOFT_RESET_COMPLETE, /* 0x3001u */

    /** This event indicates the set device address command completed.
     * The event parameter is (cy_stc_ble_events_param_generic_t*)
     * eventParams member of cy_stc_ble_events_param_generic_t is ignored
     */
    CY_BLE_EVT_SET_DEVICE_ADDR_COMPLETE, /* 0x3002u */

    /** This event indicates the get device address command completed successfully.
     * The event parameter is (cy_stc_ble_events_param_generic_t*)
     * eventParams member of cy_stc_ble_events_param_generic_t is (cy_stc_ble_bd_addrs_t*)
     * eventParams is valid only if status is Success(0x00)
     */
    CY_BLE_EVT_GET_DEVICE_ADDR_COMPLETE, /* 0x3003u */

    /** This event indicates the get RSSI command completed.
     * The event parameter is (cy_stc_ble_events_param_generic_t*)
     * eventParams member of cy_stc_ble_events_param_generic_t is (cy_stc_ble_rssi_info_t*)
     * eventParams is valid only if status is Success(0x00)
     */
    CY_BLE_EVT_GET_RSSI_COMPLETE, /* 0x3004u */

    /** This event indicates the get Tx Power command completed.
     * The event parameter is (cy_stc_ble_events_param_generic_t*)
     * eventParams member of cy_stc_ble_events_param_generic_t is powerLevel (cy_stc_ble_tx_pwr_lvl_info_t*)
     * eventParams is valid only if status is success (0x00)
     * bdHandle is valid only if bleSsChId is CY_BLE_LL_CONN_CH_TYPE
     */
    CY_BLE_EVT_GET_TX_PWR_COMPLETE, /* 0x3005u */

    /** This event indicates the set Tx Power command completed .
     * The event parameter is (cy_stc_ble_events_param_generic_t*)
     * eventParams member of cy_stc_ble_events_param_generic_t is powerConfig (cy_stc_ble_tx_pwr_config_param_t*)
     * eventParams is valid only if status is success (0x00)
     */
    CY_BLE_EVT_SET_TX_PWR_COMPLETE, /* 0x3006u */

    /** This event indicates the get clockl config command completed.
     * The event parameter is (cy_stc_ble_events_param_generic_t*)
     * eventParams member of cy_stc_ble_events_param_generic_t is powerLevel (cy_stc_ble_bless_clk_cfg_params_t*)
     * eventParams is valid only if status is Success(0x00)
     */
    CY_BLE_EVT_GET_CLK_CONFIG_COMPLETE, /* 0x3007u */

    /** This event indicates the set clock config command completed.
     * The event parameter is (cy_stc_ble_events_param_generic_t*)
     * eventParams member of cy_stc_ble_events_param_generic_t is ignored
     */
    CY_BLE_EVT_SET_CLK_CONFIG_COMPLETE, /* 0x3008u */

    /** This event indicates the random number generation command completed successfully.
        The event parameter is Pointer to a buffer of size 8 bytes */
    CY_BLE_EVT_RANDOM_NUM_GEN_COMPLETE, /* 0x3009u */

    /** This event indicates the AES encrypt command completed.
     * The event parameter is (cy_stc_ble_events_param_generic_t*)
     * eventParams member of cy_stc_ble_events_param_generic_t is pointer to encrypted data (128-bit)(uint8_t*)
     * eventParams is valid only if status is Success(0x00)
     */
    CY_BLE_EVT_AES_ENCRYPT_COMPLETE, /* 0x300Au */

    /** This event indicates the AES CCM encrypt command completed.
     * The event parameter is (cy_stc_ble_events_param_generic_t*)
     * eventParams member of cy_stc_ble_events_param_generic_t is pointer to cy_stc_ble_aes_ccm_param_t
     * eventParams is valid only if status is Success(0x00)
     */
    CY_BLE_EVT_AES_CCM_ENCRYPT_COMPLETE, /* 0x300Bu */

    /** This event indicates the AES CCM decrypt command completed.
     * The event parameter is (cy_stc_ble_events_param_generic_t*)
     * eventParams member of cy_stc_ble_events_param_generic_t is pointer to cy_stc_ble_aes_ccm_param_t
     * eventParams is valid only if status is Success(0x00).eventParams->mic to be ignored.
     */
    CY_BLE_EVT_AES_CCM_DECRYPT_COMPLETE, /* 0x300Cu */

    /* 0x300Du -> Reserved for future*/

    /* 0x300Eu -> Reserved for future*/

    /* 0x300Fu -> Reserved for future*/

    /* 0x3010u -> Reserved for future*/

    /** This event is triggered for Cy_BLE_GetBatteryLevel API.
     * Event parameter is (cy_stc_ble_events_param_generic_t*)
     * eventParams member of cy_stc_ble_events_param_generic_t is bdHandle (uint8_t*)
     * eventParams is valid only if status is Success(0x00)
     */
    CY_BLE_EVT_SET_SLAVE_LATENCY_MODE_COMPLETE = 0x3011u, /* 0x3011u */

    /** This event is used to inform the application that stack shutdown is completed.
    * The event parameter is NULL.
    */
    CY_BLE_EVT_STACK_SHUTDOWN_COMPLETE, /* 0x3012u */

    /** This event is used to inform the application of temperature data as measured. Event parameter is of 'uint16_t' */
    CY_BLE_EVT_RADIO_TEMPERATURE, /* 0x3013u */

    /** This event is used to inform the application of voltage data as measured. Event parameter is of 'uint16_t' */
    CY_BLE_EVT_RADIO_VOLTAGE_LEVEL, /* 0x3014u */

    /** This event is used to inform the application that the AES CMAC generation is completed
     * The event parameter is (cy_stc_ble_events_param_generic_t*)
     * eventParams member of cy_stc_ble_events_param_generic_t is ignored
     */
    CY_BLE_EVT_AES_CMAC_GEN_COMPLETE, /* 0x3015 */

    /** This event is triggered on completion of setting vendor event mask.
    * The event parameter is (cy_stc_ble_events_param_generic_t*)
    * Only status is valid. eventParams is not used
    */
    CY_BLE_EVT_SET_EVENT_MASK_COMPLETE, /* 0x3016u */

    /** This event is triggered on completion of setting vendor event mask.
     * The event parameter is (cy_stc_ble_events_param_generic_t*)
     * eventParams member of cy_stc_ble_events_param_generic_t is bdHandle (uint8_t*)
     * eventParams is valid only if status is Success(0x00)
     */
    CY_BLE_EVT_SET_CE_LENGTH_COMPLETE, /* 0x3017u */

    /** This event is triggered on completion of setting vendor event mask.
     * The event parameter is (cy_stc_ble_events_param_generic_t*)
     * eventParams member of cy_stc_ble_events_param_generic_t is bdHandle (uint8_t*)
     * eventParams is valid only if status is Success(0x00)
     */
    CY_BLE_EVT_SET_CONN_PRIORITY_COMPLETE, /* 0x3018u */

    /* Range for GAP events - 0x4000 to 0x4FFF */

    /** This event is triggered every time a device is discovered. A pointer to structure of type
     * cy_stc_ble_gapc_adv_report_param_t is returned as the event parameter.
     */
    CY_BLE_EVT_GAPC_SCAN_PROGRESS_RESULT = 0x4000u,

    /** This event is received by Peripheral and Central devices. When it is received by a peripheral, that peripheral
        must Call Cy_BLE_GAPP_AuthReqReply() to reply to the authentication request from Central.

        When this event is received by Central, that means the slave has requested Central to initiate authentication
        procedure. Central must call Cy_BLE_GAP_AuthReq() to initiate the authentication procedure.
        A pointer to structure of type cy_stc_ble_gap_auth_info_t is returned as the event parameter. */
    CY_BLE_EVT_GAP_AUTH_REQ, /* 0x4001u */

    /** This event indicates that the device must send a passkey to be used during the pairing procedure.
       Cy_BLE_GAP_AuthPassKeyReply() is required to be called with valid parameters on receiving this event.

       cy_stc_ble_gap_auth_pk_info_t is returned as the event parameter. bdHandle is the only relevant parameter.
       Other parameters should be ignored. */
    CY_BLE_EVT_GAP_PASSKEY_ENTRY_REQUEST, /* 0x4002u */

    /** This event indicates that the device needs to display a passkey during the pairing procedure.
       cy_stc_ble_gap_auth_pk_info_t is returned as the event parameter. bdHandle and passkey are relevant parameters.
       Other parameter(s) should be ignored.
       The passkey can be any 6-decimal-digit value. */
    CY_BLE_EVT_GAP_PASSKEY_DISPLAY_REQUEST, /* 0x4003u */

     /** This event indicates that the authentication procedure has been completed.

         The event parameter contains the security information as defined by cy_stc_ble_gap_auth_info_t.
         This event is generated at the end of the following three operations:
            * Authentication is initiated with a newly connected device
            * Encryption is initiated with a connected device that is already bonded
            * Re-Encryption is initiated with a connected device with link already encrypted
         During encryption/re-encryption, the Encryption Information exchanged during the pairing process
         is used to encrypt/re-encrypt the link. As this does not modify any of the authentication
         parameters with which the devices were paired, this event is generated with NULL event data
         and the result of the encryption operation. */
    CY_BLE_EVT_GAP_AUTH_COMPLETE, /* 0x4004u */

    /** Authentication process failed between two devices.
        cy_stc_ble_gap_auth_info_t is returned as the event parameter. bdHandle provides a handle for the failing device and
        authErr provides cy_en_ble_gap_auth_failed_reason_t indicates the reason for failure.
        Other parameters should be ignored. */
    CY_BLE_EVT_GAP_AUTH_FAILED, /* 0x4005u */

    /** Peripheral device has started/stopped advertising.
       This event is generated after making a call to the Cy_BLE_GAPP_EnterDiscoveryMode and
       Cy_BLE_GAPP_ExitDiscoveryMode functions. The event parameter contains the HCI Status error code,
       which is of type 'uint8_t'.

       If the data is '0x00', it indicates 'success'; anything else indicates 'failure'. */
    CY_BLE_EVT_GAPP_ADVERTISEMENT_START_STOP, /* 0x4006u */

    /** This event is generated at the GAP Peripheral end after the connection is completed with a peer Central device.
        For a GAP Central device, this event is generated as an acknowledgment of receiving this event successfully
        by the BLE Controller. After the connection is complete, no other event is required but if connection establishment fails,
        'CY_BLE_EVT_GAP_DEVICE_DISCONNECTED' is passed to the application. The 'CY_BLE_EVT_GAP_ENHANCE_CONN_COMPLETE'
        event is triggered instead of 'CY_BLE_EVT_GAP_DEVICE_CONNECTED', if Link Layer Privacy is enabled in the component customizer.
        The event parameter is a pointer to a structure of type cy_stc_ble_gap_connected_param_t. */
    CY_BLE_EVT_GAP_DEVICE_CONNECTED, /* 0x4007u */

    /** Disconnected from remote device or failed to establish connection.
        Parameter is of type 'cy_stc_ble_gap_disconnect_param_t' */
    CY_BLE_EVT_GAP_DEVICE_DISCONNECTED, /* 0x4008u */

    /** Encryption change event for active connection.
         * Event parameter is (cy_stc_ble_events_param_generic_t*)
         * eventParams member of cy_stc_ble_events_param_generic_t is (cy_stc_ble_gap_encrypt_change_param_t*)
         * eventParams->encryption is valid only if status is Success(0x00)
         This is an informative event for the application when there is a change in encryption. 
         The application may choose to ignore it. */
    CY_BLE_EVT_GAP_ENCRYPT_CHANGE, /* 0x4009u */

    /** This event is generated at the GAP Central and the Peripheral end after a connection parameter update
        is requested from the host to the controller.
        The event parameter is a pointer to a structure of type cy_stc_ble_gap_conn_param_updated_in_controller_t. */
    CY_BLE_EVT_GAP_CONNECTION_UPDATE_COMPLETE, /* 0x400Au */

    /** The Central device has started/stopped scanning.
       This event is generated after making a call to the Cy_BLE_GAPC_StartDiscovery and
       Cy_BLE_GAPC_StopDiscovery APIs. The event parameter contains the HCI Status error code,
       which is of type 'uint8_t'.

       If the data is '0x00', it indicates 'success'; anything else indicates 'failure'. */
    CY_BLE_EVT_GAPC_SCAN_START_STOP, /* 0x400Bu */

    /** This is an indication that the SMP keys exchange with a peer device is complete. The event handler
       is expected to store the peer device keys, especially IRK, which is used to resolve the
       peer device after the connection establishment.
       The event parameter returns data of type cy_stc_ble_gap_sec_key_param_t containing the peer device keys. */
    CY_BLE_EVT_GAP_KEYINFO_EXCHNGE_CMPLT, /* 0x400Cu */

    /** This event indicates that the device must display a passkey during the
        secure connection pairing procedure. Cy_BLE_GAP_AuthPassKeyReply() is
        required to be called with valid parameters on receiving this event.
        Because no key is entered by the user for Numeric comparison, the
        parameter passkey for the function Cy_BLE_GAP_AuthPassKeyReply will be
        ignored.
        cy_stc_ble_gap_auth_pk_info_t is returned as the event parameter. bdHandle and passkey are relevant parameters.
        Other parameter(s) should be ignored. Passkey can be any 6-decimal-digit value. */
    CY_BLE_EVT_GAP_NUMERIC_COMPARISON_REQUEST, /* 0x400Du */

    /** This event is generated when keypress (Secure connections) is received
       from peer device.
       cy_stc_ble_gap_sc_kp_notif_info_t is returned as event parameter. */
    CY_BLE_EVT_GAP_KEYPRESS_NOTIFICATION, /* 0x400Eu */

    /** This event is generated when OOB generation for Secure connections is complete.
       The event parameter is of type 'cy_stc_ble_gap_oob_data_param_t' */
    CY_BLE_EVT_GAP_OOB_GENERATED_NOTIFICATION, /* 0x400Fu */

    /** The LE Enhanced Connection Complete event indicates to the application that a new connection has been created when
        Link Layer Privacy is enabled in the component customizer.
       The event parameter is of type 'cy_stc_ble_gap_enhance_conn_complete_param_t' */
    CY_BLE_EVT_GAP_ENHANCE_CONN_COMPLETE, /* 0x4010u */

    /** The LE Direct Advertising Report event indicates that directed advertisements have been received where
       the advertiser is using a resolvable private address for the InitA field in the ADV_DIRECT_IND PDU and the
       Scanning_Filter_Policy is equal to 0x02 or 0x03. The event parameter is of type 'cy_stc_ble_gapc_direct_adv_report_param_t' */
    CY_BLE_EVT_GAPC_DIRECT_ADV_REPORT, /* 0x4011u */

    /** The SMP negotiated auth info event is raised as soon as the SMP has completed pairing properties (feature exchange)
        negotiation. The event parameter is cy_stc_ble_gap_auth_info_t. cy_stc_ble_gap_auth_info_t will have the
        negotiated parameter. The pairing should either pass with these negotiated parameters or may fail. */
    CY_BLE_EVT_GAP_SMP_NEGOTIATED_AUTH_INFO, /* 0x4012u */

    /** This event indicates a new Bluetooth device address generated successfully as per an application requirement.
         The event parameter is cy_stc_ble_bd_addr_t */
    CY_BLE_EVT_GAP_DEVICE_ADDR_GEN_COMPLETE, /* 0x4013u */

    /** This event indicates security keys are generated successfully.
         The event parameter is cy_stc_ble_gap_sec_key_param_t  bdHandle in the parameter should be ignored */
    CY_BLE_EVT_GAP_KEYS_GEN_COMPLETE, /* 0x4014u */

    /** This event is triggered on completion of Cy_BLE_GAPC_ResolveDevice API.
          The event parameter is (cy_stc_ble_events_param_generic_t*)*/
    CY_BLE_EVT_GAP_RESOLVE_DEVICE_COMPLETE, /* 0x4015u */

    /** This event is triggered on completion of the Cy_BLE_GAP_GenerateSetLocalP256Keys API.
         The event parameter is of pointer to cy_stc_ble_gap_smp_local_p256_keys_t that has
         generated P256 keys. */
    CY_BLE_EVT_GAP_GEN_SET_LOCAL_P256_KEYS_COMPLETE, /* 0x4016u */

    /** This event is triggered on completion of the Cy_BLE_GAPC_CancelConnection API.
         * The event parameter is (cy_stc_ble_events_param_generic_t*)
         * eventParams member of cy_stc_ble_events_param_generic_t is ignored
         */
    CY_BLE_EVT_GAP_CREATE_CONN_CANCEL_COMPLETE, /* 0x4017u */

    /** This event is triggered on completion of connection establishment.
         * Event parameter is (cy_stc_ble_conn_estb_param_t*)
         */
    CY_BLE_EVT_GAP_CONN_ESTB, /* 0x4018u */

    /** Peripheral device has started/stopped advertising.
       This event is generated after making a call to the Cy_BLE_GAP_UpdateAdvScanData function.
       The event parameter contains the HCI Status error code, which is of type 'uint8_t'.

       If the data is '0x00', it indicates 'success'; anything else indicates 'failure'. */
    CY_BLE_EVT_GAPP_UPDATE_ADV_SCAN_DATA_COMPLETE, /* 0x4019u */

    /* Range for GATT events - 0x5000 to 0x5FFF */

    /** The event is received by the Client when the Server cannot perform the requested
       operation and sends out an error response. The event parameter is a pointer to a structure
       of type cy_stc_ble_gatt_err_param_t. */
    CY_BLE_EVT_GATTC_ERROR_RSP = 0x5000u,

    /** This event is generated at the GAP Peripheral end after a connection is completed with a peer Central device.
        For a GAP Central device, this event is generated as in acknowledgment of receiving this event successfully
        by the BLE Controller. After connection is complete, no other event is required but if connection establishment fails,
        'CY_BLE_EVT_GATT_DISCONNECT_IND' is passed to the application.
        The event parameter is a pointer to a structure of type cy_stc_ble_conn_handle_t. */
    CY_BLE_EVT_GATT_CONNECT_IND, /* 0x5001u */

    /** GATT is disconnected.
        The event parameter is a pointer to a structure of type cy_stc_ble_conn_handle_t. */
    CY_BLE_EVT_GATT_DISCONNECT_IND, /* 0x5002u */

    /** 'GATT MTU Exchange Request' received from GATT client device. The event parameter
       contains the MTU size of type cy_stc_ble_gatt_xchg_mtu_param_t. */
    CY_BLE_EVT_GATTS_XCNHG_MTU_REQ, /* 0x5003u */

    /** 'GATT MTU Exchange Response' received from server device. The event parameter is a
       pointer to a structure of type cy_stc_ble_gatt_xchg_mtu_param_t. */
    CY_BLE_EVT_GATTC_XCHNG_MTU_RSP, /* 0x5004u */

    /** 'Read by Group Type Response' received from server device. Event parameter
       is a pointer to a structure of type cy_stc_ble_gattc_read_by_grp_rsp_param_t. */
    CY_BLE_EVT_GATTC_READ_BY_GROUP_TYPE_RSP, /* 0x5005u */

    /** 'Read by Type Response' received from server device. The event parameter is a
       pointer to a structure of type cy_stc_ble_gattc_read_by_type_rsp_param_t. */
    CY_BLE_EVT_GATTC_READ_BY_TYPE_RSP, /* 0x5006u */

    /** 'Find Information Response' received from server device. The event parameter is
       a pointer to a structure of type 'cy_stc_ble_gattc_find_info_rsp_param_t. */
    CY_BLE_EVT_GATTC_FIND_INFO_RSP, /* 0x5007u */

    /** 'Find by Type Value Response' received from server device. The event parameter is
       a pointer to a structure of type cy_stc_ble_gattc_find_by_type_rsp_param_t. */
    CY_BLE_EVT_GATTC_FIND_BY_TYPE_VALUE_RSP, /* 0x5008u */

    /** 'Read Response' from server device. The event parameter is a pointer to a
       structure of type cy_stc_ble_gattc_read_rsp_param_t. */
    CY_BLE_EVT_GATTC_READ_RSP, /* 0x5009u */

    /** 'Read Blob Response' from server. The event parameter is a pointer to a
       structure of type cy_stc_ble_gattc_read_rsp_param_t. */
    CY_BLE_EVT_GATTC_READ_BLOB_RSP, /* 0x500Au */

    /** 'Read Multiple Responses' from server. The event parameter is a pointer
       to a structure of type cy_stc_ble_gattc_read_rsp_param_t. The 'actualLen' field
       should be ignored as it is unused in this event response. */
    CY_BLE_EVT_GATTC_READ_MULTI_RSP, /* 0x500Bu */

    /** 'Write Request' from client device. The event parameter is a pointer to
       a structure of type cy_stc_ble_gatt_write_param_t. */
    CY_BLE_EVT_GATTS_WRITE_REQ, /* 0x500Cu */

    /** 'Write Response' from server device. The event parameter is a pointer
       to a structure of type cy_stc_ble_conn_handle_t. */
    CY_BLE_EVT_GATTC_WRITE_RSP, /* 0x500Du */

    /** 'Write Command' Request from client device. The event parameter is a
       pointer to a structure of type cy_stc_ble_gatts_write_cmd_req_param_t. */
    CY_BLE_EVT_GATTS_WRITE_CMD_REQ, /* 0x500Eu */

    /** 'Prepare Write' Request from client device. The event parameter is a
       pointer to a structure of type cy_stc_ble_gatts_prep_write_req_param_t. */
    CY_BLE_EVT_GATTS_PREP_WRITE_REQ, /* 0x500Fu */

    /** 'Execute Write' request from client device. The event parameter is a
       pointer to a structure of type 'cy_stc_ble_gatts_exec_write_req_t'.
       This event will be triggered before GATT DB is modified. GATT DB will be updated
       only if there is no error condition provided by the application. In case of an error condition triggered
       during stack validation, a partial write will occur. The write will be cancelled from that handle where
       the error has occurred and an error response corresponding to that handle will be sent to the remote.
       If at any point of time 'CY_BLE_GATT_EXECUTE_WRITE_CANCEL_FLAG' is received in
       execWriteFlag fields of 'cy_stc_ble_gatts_exec_write_req_t' structure, then all previous
       writes are cancelled. For execute cancel scenario, all elements of
       'cy_stc_ble_gatts_exec_write_req_t' should be ignored except execWriteFlag and connHandle.
     */
    CY_BLE_EVT_GATTS_EXEC_WRITE_REQ, /* 0x5010u */

    /** 'Execute Write' response from server device. The event parameter is a
       pointer to a structure of type cy_stc_ble_gattc_exec_write_rsp_param_t. */
    CY_BLE_EVT_GATTC_EXEC_WRITE_RSP, /* 0x5011u */

    /** Notification data received from server device. The event parameter
       is a pointer to a structure of type cy_stc_ble_gattc_handle_value_ntf_param_t. */
    CY_BLE_EVT_GATTC_HANDLE_VALUE_NTF, /* 0x5012u */

    /** Indication data received from server device. The event parameter is
       a pointer to a structure of type cy_stc_ble_gattc_handle_value_ind_param_t. */
    CY_BLE_EVT_GATTC_HANDLE_VALUE_IND, /* 0x5013u */

    /** Confirmation to indication response from client device. The event
       parameter is a pointer to a structure of type cy_stc_ble_conn_handle_t. */
    CY_BLE_EVT_GATTS_HANDLE_VALUE_CNF, /* 0x5014u */

    /** Event indicating GATT signed write command is received from client device. The event
       parameter is a pointer to a structure of type cy_stc_ble_gatts_signed_write_cmd_req_param_t.
       If value.val parameter is set to Zero, then the signature is not matched and is ignored by the stack */
    CY_BLE_EVT_GATTS_DATA_SIGNED_CMD_REQ, /* 0x5015u */

    /** Event indicating that GATT group procedure has stopped or completed. This event occurs
       only if the application has called the Cy_BLE_GATTC_StopCmd API.
       The parameter is a pointer to a structure of type cy_stc_ble_conn_handle_t. */
    CY_BLE_EVT_GATTC_STOP_CMD_COMPLETE, /* 0x5016u */

   /** The event parameter type is cy_stc_ble_gatts_char_val_read_req_t. It is triggered on the server side
       when the client sends a read request and when characteristic has the CY_BLE_GATT_DB_ATTR_CHAR_VAL_RD_EVENT
       property set. This event could be ignored by the application unless it need to respond by an error response, which
       must be set in the gattErrorCode field of the event parameter. */
    CY_BLE_EVT_GATTS_READ_CHAR_VAL_ACCESS_REQ, /* 0x5017u */

    /** Event indicating that the GATT long procedure has ended and the stack will not send any further
       * requests to the peer. The event parameter is of type 'cy_stc_ble_gattc_long_procedure_end_param_t'.
       * Either this event or 'CY_BLE_EVT_GATTC_ERROR_RSP' will be received
       * by the application. This event may be triggered for the below GATT long procedures:
       *                     1.  Cy_BLE_GATTC_DiscoverAllPrimaryServices
       *                     2.  Cy_BLE_GATTC_DiscoverPrimaryServiceByUuid
       *                     3.  Cy_BLE_GATTC_FindIncludedServices
       *                     4.  Cy_BLE_GATTC_DiscoverAllCharacteristics
       *                     5.  Cy_BLE_GATTC_DiscoverCharacteristicByUuid
       *                     6.  Cy_BLE_GATTC_DiscoverAllCharacteristicDescriptors
       *                     7.  Cy_BLE_GATTC_ReadLongCharacteristicValues
       *                     8.  Cy_BLE_GATTC_WriteLongCharacteristicValues
       *                     9.  Cy_BLE_GATTC_ReliableWrites
       *                     10. Cy_BLE_GATTC_ReadLongCharacteristicDescriptors
       *                     11. Cy_BLE_GATTC_WriteLongCharacteristicDescriptors    \n
       *   The event parameter is ATT opcode for the corresponding long GATT Procedure.
       */
    CY_BLE_EVT_GATTC_LONG_PROCEDURE_END, /* 0x5018u */

    /* Range for L2CAP events - 0x6000 to 0x6FFF */

    /** This event indicates the connection parameter update has been received
       from the remote device. The application is expected to reply to L2CAP using the
       Cy_BLE_L2CAP_LeConnectionParamUpdateResponse() function to respond to the remote
       device, whether parameters are accepted or rejected.

       The event parameter pointer points to data of type 'cy_stc_ble_gap_conn_update_param_info_t' */
    CY_BLE_EVT_L2CAP_CONN_PARAM_UPDATE_REQ = 0x6000u,

    /** This event indicates the connection parameter update response has been received
       from the master. The event parameter is a pointer to a structure of type
       cy_stc_ble_l2cap_conn_update_rsp_param_t. */
    CY_BLE_EVT_L2CAP_CONN_PARAM_UPDATE_RSP, /* 0x6001u */

    /** This event indicates that the request to send over l2cap signaling has been
       rejected. The event parameter is a pointer to a structure of type
       cy_stc_ble_l2cap_cmd_rej_param_t. */
    CY_BLE_EVT_L2CAP_COMMAND_REJ, /* 0x6002u */

    /** This event is used to inform the application of the incoming L2CAP CBFC
       Connection Request. The event parameter is a pointer to a structure of type
       cy_stc_ble_l2cap_cbfc_conn_ind_param_t is returned. */
    CY_BLE_EVT_L2CAP_CBFC_CONN_IND, /* 0x6003u */

    /** This event is used to inform application of the L2CAP CBFC Connection
       Response/Confirmation. The event parameter is a pointer to a structure of
       type cy_stc_ble_l2cap_cbfc_conn_cnf_param_t is returned. */
    CY_BLE_EVT_L2CAP_CBFC_CONN_CNF, /* 0x6004u */

    /** This event is used to inform the application of the L2CAP CBFC Disconnection
       Request received from the Peer device. The event parameter is a pointer to
       a Local CID of type unit16. */
    CY_BLE_EVT_L2CAP_CBFC_DISCONN_IND, /* 0x6005u */

    /** This event is used to inform the application of the L2CAP CBFC Disconnection
       confirmation/Response received from the Peer device. The event parameter is a
       pointer to a structure of type cy_stc_ble_l2cap_cbfc_disconn_cnf_param_t. */
    CY_BLE_EVT_L2CAP_CBFC_DISCONN_CNF, /* 0x6006u */

    /** This event is used to inform the application of data received over the L2CAP
       CBFC channel. The event parameter is a pointer to a structure of type
       cy_stc_ble_l2cap_cbfc_rx_param_t. */
    CY_BLE_EVT_L2CAP_CBFC_DATA_READ, /* 0x6007u */

    /** This event is used to inform the application of receive credits has reached the 
       low mark. After receiving L2CAP data/payload from a peer device for a
       specification Channel, the available credits are calculated.

       If the credit count goes below the low mark, this event is called to inform
       the application of the condition, so that if the application wants it can
       send more credits to the peer device.

       The event parameter is a pointer to a structure of type
       cy_stc_ble_l2cap_cbfc_low_rx_credit_param_t. */
    CY_BLE_EVT_L2CAP_CBFC_RX_CREDIT_IND, /* 0x6008u */

    /** This event is used to inform the application of having received transmit
       credits. This event is called on receiving LE Flow Control Credit from a peer
       device.

       The event parameter is a pointer to a structure of type
       cy_stc_ble_l2cap_cbfc_low_tx_credit_param_t.

       If the 'result' field of the received data is non-zero, this indicates an
       error. If the sum of 'credit' field value and the previously available credit
       at the peer device receiving credit information exceeds 65535, it indicates a
       'credit overflow' error.

       If there is an error, the peer device receiving this event should initiate
       disconnection of the L2CAP channel by invoking the Cy_BLE_L2CAP_DisconnectReq ()
       function. */
    CY_BLE_EVT_L2CAP_CBFC_TX_CREDIT_IND, /* 0x6009u */

    /** This event is used to inform the application that L2CAP CBFC data transmission is scheduled
        for transmission in the BLE Controller. The application can send the next data.
        The event parameter is of type 'cy_stc_ble_l2cap_cbfc_rx_data_param_t'.
        The L2CAP CBFC application must wait for this event before transmitting the next CBFC L2CAP data.
        The application can send the next data only when the CY_BLE_EVT_L2CAP_CBFC_DATA_WRITE_IND event is received for 
        previously sent data and CY_BLE_EVT_STACK_BUSY_STATUS is received with status CY_BLE_STACK_STATE_FREE. */
    CY_BLE_EVT_L2CAP_CBFC_DATA_WRITE_IND,  /* 0x600Au */

    /*Range for qualification events - 0x7000 to 0x7FFF*/

#ifdef CY_BLE_HOST_QUALIFICATION
    /** Tester to manipulate pairing request or response PDU. The event parameter is a pointer to 1 byte data.
        The tester can manipulate the bits of the byte. */
    CY_BLE_EVT_QUAL_SMP_PAIRING_REQ_RSP = 0x7000u,

    /** Tester to manipulate local Public Key. The event parameter is a pointer to local public key of size 64 Bytes.
        The tester can manipulate the bits/bytes. */
    CY_BLE_EVT_QUAL_SMP_LOCAL_PUBLIC_KEY, /* 0x7001u */

    /** Tester to assign pairing failed error code. The event parameter is a pointer to 16 bits value.
        The tester should assign an error code to the lower bits. */
    CY_BLE_EVT_QUAL_SMP_PAIRING_FAILED_CMD, /* 0x7002u */

#endif /* CY_BLE_HOST_QUALIFICATION */

    /*##Range for for future use - 0x8000 to 0xFFFE*/


    /** Maximum value of cy_en_ble_event_t type */
    CY_BLE_EVT_MAX = 0xFFFF

} cy_en_ble_event_t;

/** @} */

/**
 \addtogroup group_ble_common_api_definitions
 @{
*/
/** Common error codes received as API result */
typedef enum
{
    /** No Error occurred */
    CY_BLE_SUCCESS = CY_PDL_STATUS_INFO,

    /** Flash operation in progress*/
    CY_BLE_INFO_FLASH_WRITE_IN_PROGRESS = CY_PDL_STATUS_INFO | CY_BLE_ID | 0x0001ul,

    /** At least one of the input parameters is invalid */
    CY_BLE_ERROR_INVALID_PARAMETER = CY_PDL_STATUS_ERROR | CY_BLE_ID | 0x0001ul,

    /** Operation is not permitted */
    CY_BLE_ERROR_INVALID_OPERATION,

    /** An internal error occurred in the stack */
    CY_BLE_ERROR_MEMORY_ALLOCATION_FAILED,

    /** Insufficient resources to perform requested operation */
    CY_BLE_ERROR_INSUFFICIENT_RESOURCES,

    /** OOB data not available */
    CY_BLE_ERROR_OOB_NOT_AVAILABLE,

    /** Connection is required to perform requested operation. Connection not
       present */
    CY_BLE_ERROR_NO_CONNECTION,

    /** No device entity to perform requested operation */
    CY_BLE_ERROR_NO_DEVICE_ENTITY,

    /** Device cannot be added to the Whitelist as it has already been added */
    CY_BLE_ERROR_DEVICE_ALREADY_EXISTS,

    /** Attempted repeat operation is not allowed */
    CY_BLE_ERROR_REPEATED_ATTEMPTS,

    /** GAP role is incorrect */
    CY_BLE_ERROR_GAP_ROLE,

    /** Security operation failed */
    CY_BLE_ERROR_SEC_FAILED,

    /** L2CAP PSM encoding is incorrect */
    CY_BLE_ERROR_L2CAP_PSM_WRONG_ENCODING,

    /** L2CAP PSM has already been registered */
    CY_BLE_ERROR_L2CAP_PSM_ALREADY_REGISTERED,

    /** L2CAP PSM has not been registered */
    CY_BLE_ERROR_L2CAP_PSM_NOT_REGISTERED,

    /** L2CAP connection entity not found */
    CY_BLE_ERROR_L2CAP_CONNECTION_ENTITY_NOT_FOUND,

    /** Specified PSM is out of range */
    CY_BLE_ERROR_L2CAP_PSM_NOT_IN_RANGE,

    /** Unsupported feature or parameter value */
    CY_BLE_ERROR_UNSUPPORTED_FEATURE_OR_PARAMETER_VALUE,

    /** Write to flash is not permitted */
    CY_BLE_ERROR_FLASH_WRITE_NOT_PERMITED,

    /** Error in flash Write */
    CY_BLE_ERROR_FLASH_WRITE,

    /** MIC Authentication failure */
    CY_BLE_ERROR_MIC_AUTH_FAILED,

   /** Controller error codes. These come directly from the controller (not the host stack)*/

    /** Hardware Failure */
    /** Possible reason - BLE ECO failed to start due to one of the below:
      * - LFCLK (WCO, PILO, ILO) not present
      * - Radio bus failure */
    CY_BLE_ERROR_HARDWARE_FAILURE,

    /**GATT DB error codes*/

    /** Invalid attribute handle */
    CY_BLE_ERROR_GATT_DB_INVALID_ATTR_HANDLE,

    /* Profile level API_RESULT codes */
    /** Characteristic notifications disabled */
    CY_BLE_ERROR_NTF_DISABLED,

    /** Characteristic indications disabled */
    CY_BLE_ERROR_IND_DISABLED,

    /** \cond IGNORE */
    CY_BLE_ERROR_CHAR_IS_NOT_DISCOVERED,
    /** \endcond */

    /** Controller Busy */
    CY_BLE_ERROR_CONTROLLER_BUSY = CY_PDL_STATUS_ERROR | CY_BLE_ID | 0x00FEul,

    /** The state is not valid for current operation */
    CY_BLE_ERROR_INVALID_STATE,

    /** All other errors not covered in the above list map to this error code */
    CY_BLE_ERROR_MAX = CY_PDL_STATUS_ERROR | CY_BLE_ID | 0xFFFFul,

} cy_en_ble_api_result_t;


/***************************************
** Exported structures and unions
***************************************/

/** Event callback function prototype to receive events from stack */
typedef void (*cy_ble_app_ev_cb_t)(cy_en_ble_event_t event, void* evParam);


/** This structure is used to hold version information of the BLE Stack Library */
typedef struct
{
    /** The major version of the library */
    uint8_t majorVersion;

    /** The minor version of the library */
    uint8_t minorVersion;

    /** The patch number of the library */
    uint8_t patch;

    /** The build number of the library */
    uint16_t buildNumber;

} cy_stc_ble_stack_lib_version_t;


/** Bluetooth Device Address type */
typedef struct
{
    /** Bluetooth device address */
    uint8_t     bdAddr[CY_BLE_BD_ADDR_SIZE];

    /** public = 0, Random = 1 */
    uint8_t     type;

} cy_stc_ble_bd_addr_t;


/** Configuration structure for Tx and Rx ACL Data Buffers required by the 
  * BLE Controller Stack, all the configuration specified is valid per
  * connection and all the support LL connections will have same capability.
  */
typedef struct
{
    /**
     * Tx buffer size configuration for the BLE Controller per the
     * LL Connection
     */
    uint16_t   dleMaxTxCapability;

    /**
     * Rx buffer size configuration for the BLE Controller per the
     * LL Connection
     */
    uint16_t   dleMaxRxCapability;

    /**
     * Total number of Tx buffer configuration per LL Connection for the
     * BLE Controller
     */
    uint8_t   dleNumTxBuffer;

    /**
     * Total number of Rx buffer configuration per LL Connection for
     * BLE Controller
     */
    uint8_t   dleNumRxBuffer;

} cy_stc_ble_dle_config_param_t;

/** Configuration structure for the L2CAP buffer for data transmission
 *
  */
typedef struct
{
    /**
     * This specifies L2CAP stack queue depth per connection for data tx.
     * The application must provide heap memory based on the following calculation:
     * l2capBufferPerConn * maxBleConnections * CY_BLE_L2CAP_QUEUE_ELEMENT_SIZE
     */
    uint8_t   l2capBufferPerConn;

} cy_stc_ble_l2cap_config_param_t;


/** Configuration structure for LL Privacy feature */
typedef struct
{
    /** Maximum number of possible entries in resolving list */
    uint8_t   resolvingListSize;

} cy_stc_ble_privacy_1_2_config_param_t;

/** Configuration parameter for bonded device list  */
typedef struct
{
    /** Maximum number of possible entries in bonded device list */
    /* The application must provide bonded device list memory as per bondlist size */
    uint8_t   bondListSize;

} cy_stc_ble_bonded_device_list_config_param_t;

/**Configuration parameter for the WhiteList */
typedef struct
{
    /** Maximum number of possible entries in the Whitelist */
    uint8_t   whiteListSize;

} cy_stc_ble_white_list_config_param_t;

/**
 * Configuration structure for enabling selective features
 * and passing associated parameters.
 */
typedef struct
{
    /** Configuration parameter for DLE feature */
    cy_stc_ble_dle_config_param_t            dleConfig;

    /** Configuration parameter for L2CAP buffer */
    cy_stc_ble_l2cap_config_param_t            l2capConfig;

    /** Configuration parameter for privacy 1.2 feature */
    cy_stc_ble_privacy_1_2_config_param_t    privacyConfig;

    /** Configuration parameter for bonded device list  */
    cy_stc_ble_bonded_device_list_config_param_t    bondListConfig;

    /** Configuration parameter for whiteList */
    cy_stc_ble_white_list_config_param_t    whiteListConfig;

    /** The feature set mask used to control usage of
     * specified feature in the BLE stack. If a feature is not selected,
     * then associated parameter pointer can be NULL.
     * Feature Mask Values :
     * B0 : DLE_FEATURE_MASK
     * B1 : PRIVACY_1_2_FEATURE_MASK
     * B2 : SECURE_CONN_FEATURE_MASK
     * B3 : PHY_UPDATE_FEATURE_MASK
     * B4 : PERSISTENT_STORE_BONDLIST
     * B5 : PERSISTENT_STORE_RESOLVING_LIST
     * B6 : PERSISTENT_STORE_WHITELIST
     * B7 : PERSISTENT_RADIO_CALIBRATION_MASK
     */
    uint16_t                              featureMask;

    /**Out parameter for returning memory requirement for selected features*/
    uint16_t                              featureHeapReq;


    /** Configuration parameter for Maximum number of BLE Connection
     * Max possible value is 4 and Min configuration value is 1
     */
    uint8_t                               maxBleConnections;

} cy_stc_ble_stack_config_param_t;


/** Set of buffers to be allocated by stack for stack operation */
typedef struct
{
    /** Size of the buffer chunk */
    uint16_t   bufferSize;

    /** Number of the buffers units of 'bufferSize' */
    uint16_t   bufferUnits;

} cy_stc_ble_stk_app_data_buff_t;

/** Structure for passing the connection established
 *  parameters
 */
typedef struct
{
    /** bd handle of the device */
    uint8_t bdHandle;
    
    /** Link layer access address */
    uint32_t accessAddr;
    
} cy_stc_ble_conn_estb_param_t;

/** BLE Stack Manager configuration structure for allocating memory pools
 *  for host and controller operation.
 */
typedef struct
{
    /** Memory Heap pointer */
    uint8_t                              * memoryHeapPtr;

    /** Set of buffers needed for stack operation */
    cy_stc_ble_stk_app_data_buff_t          * dataBuff;

    /** Memory heap pointer size */
    uint16_t                               totalHeapSz;

    /** Total data buffer pools */
    uint8_t                                totalDataBufferPools;

    /** Pointer to an array of bytes to be allocated by the BLE component for the storing the
         persistent data into the flash. Pointer provided is should be aligned to the flash boundary. */
    const uint8_t                        * bleStackFlashPointer;

    /** Size of the total flash memory pointed by bleStackFlashPointer */
    uint32_t                               bleStackFlashSize;

} cy_stc_ble_stack_mgr_mem_cfg_param_t;


/** BLE Stack init parameter */
typedef struct
{
    /** Application Callback Function Configuration **/
    cy_ble_app_ev_cb_t               CyBleAppCbFunc;

    /** Stack Manager Memory Configuration **/
    cy_stc_ble_stack_mgr_mem_cfg_param_t memParam;

    /** Stack configuration parameter **/
    cy_stc_ble_stack_config_param_t      stackConfig;

} cy_stc_ble_stack_init_info_t;

/**
 * Structure containing the parameters required for AES CMAC Generation 
 */
typedef struct
{

    /** pointer to message for which AES CMAC must be calculated, LSB should be first */
    uint8_t               *buffer;
    /** size of the message buffer */
    uint16_t              size;
    /** AES CMAC 128-bit Key, LSB should be first */
    uint8_t               *key;
    /** output-parameter, Buffer to hold generated MAC of 16 bytes. Output is LSB first */
    uint8_t               *mac;

}cy_stc_ble_aes_cmac_generate_param_t;

/** BLESS Power enum reflecting power level values supported by BLESS radio */
typedef enum
{
    /** ABS PWR = -20 dBm */
    CY_BLE_LL_PWR_LVL_NEG_20_DBM = -20,

    /** ABS PWR = -16 dBm */
    CY_BLE_LL_PWR_LVL_NEG_16_DBM = -16,
    
    /** ABS PWR = -12 dBm */
    CY_BLE_LL_PWR_LVL_NEG_12_DBM = -12,

    /** ABS PWR = -6 dBm */
    CY_BLE_LL_PWR_LVL_NEG_6_DBM = -6,

    /** ABS PWR = 0 dBm */
    CY_BLE_LL_PWR_LVL_0_DBM = 0,

    /** ABS PWR = +4 dBm as per silicon */
    CY_BLE_LL_PWR_LVL_MAX = 4

} cy_en_ble_bless_pwr_lvl_t;

/** BLE channel group ID */
typedef enum
{
    /** Advertisement channel type */
    CY_BLE_LL_ADV_CH_TYPE = 0x00u,

    /** Connection channel type */
    CY_BLE_LL_CONN_CH_TYPE,

    /** Maximum value of the cy_en_ble_bless_phy_ch_grp_id_t type */
    CY_BLE_LL_MAX_CH_TYPE

} cy_en_ble_bless_phy_ch_grp_id_t;

/** BLE WCO sleep clock accuracy configuration */
typedef enum
{
    /** BLE WCO sleep clock accuracy 251 to 500 ppm */
    CY_BLE_LL_SCA_251_TO_500_PPM = 0x00u,

    /** BLE WCO sleep clock accuracy 151 to 250 ppm */
    CY_BLE_LL_SCA_151_TO_250_PPM,

    /** BLE WCO sleep clock accuracy 101 to 150 ppm */
    CY_BLE_LL_SCA_101_TO_150_PPM,

    /** BLE WCO sleep clock accuracy 76 to 100 ppm */
    CY_BLE_LL_SCA_076_TO_100_PPM,

    /** BLE WCO sleep clock accuracy 51 to 75 ppm */
    CY_BLE_LL_SCA_051_TO_075_PPM,

    /** BLE WCO sleep clock accuracy 31 to 50 ppm */
    CY_BLE_LL_SCA_031_TO_050_PPM,

    /** BLE WCO sleep clock accuracy 21 to 30 ppm */
    CY_BLE_LL_SCA_021_TO_030_PPM,

    /** BLE WCO sleep clock accuracy 0 to 20 ppm */
    CY_BLE_LL_SCA_000_TO_020_PPM,

    /** BLE WCO invalid sleep clock accuracy */
    CY_BLE_LL_SCA_IN_PPM_INVALID
} cy_en_ble_bless_wco_sca_cfg_t;

/** BLE ECO clock divider */
typedef enum
{
    /** Link Layer clock divider = 1*/
    CY_BLE_LL_ECO_CLK_DIV_1 = 0x00u,

    /** Link Layer clock divider = 2*/
    CY_BLE_LL_ECO_CLK_DIV_2,

    /** Link Layer clock divider = 4*/
    CY_BLE_LL_ECO_CLK_DIV_4,

    /** Link Layer clock divider = 8*/
    CY_BLE_LL_ECO_CLK_DIV_8,

    /** Invalid Link Layer clock divider*/
    CY_BLE_LL_ECO_CLK_DIV_INVALID

} cy_en_ble_bless_eco_clk_div_t;

/** BLE clock configuration parameters */
typedef struct
{
    /** Sleep Clock accuracy in PPM, 32Khz Cycles */
    cy_en_ble_bless_wco_sca_cfg_t  bleLlSca;

    /** Link Layer clock divider */
    cy_en_ble_bless_eco_clk_div_t  bleLlClockDiv;

} cy_stc_ble_bless_clk_cfg_params_t;

/** BLE power modes */
typedef enum
{
    /** Link Layer engine and Digital modem clocked from ECO.
        The CPU can access the BLE Sub-System (BLESS) registers.
        This mode collectively denotes Tx Mode, Rx Mode, and
        Idle mode of BLESS.
    */
    CY_BLE_BLESS_ACTIVE = 0x01u,

    /** The ECO is stopped and WCO is used to maintain link layer
        timing. RF transceiver is turned off completely to reduce
        leakage current. BLESS logic is kept powered ON from the
        SRSS deep sleep regulator for retention.
    */
    CY_BLE_BLESS_DEEPSLEEP,

    /** Invalid mode */
    CY_BLE_BLESS_INVALID = 0xFFu
} cy_en_ble_lp_mode_t;


/** BLESS Power enum reflecting power states supported by BLESS radio */
typedef enum
{
    CY_BLE_BLESS_STATE_ACTIVE = 0x01,
    CY_BLE_BLESS_STATE_EVENT_CLOSE,
    CY_BLE_BLESS_STATE_ECO_ON,
    CY_BLE_BLESS_STATE_ECO_STABLE,
    CY_BLE_BLESS_STATE_DEEPSLEEP,
    CY_BLE_BLESS_STATE_STOPPED,
    CY_BLE_BLESS_STATE_INVALID = 0xFFu
} cy_en_ble_bless_state_t;

/** Sleep mode which controller core is trying to enter */
typedef enum
{
    CY_BLE_CONTROLLER_SLEEP_MODE_SLEEP = 0x01u,
    CY_BLE_CONTROLLER_SLEEP_MODE_DEEPSLEEP
} cy_en_ble_controller_sleep_mode_t;

/** @} */

/**
 \addtogroup group_ble_common_api_functions
 @{
*/
/******************************************************************************
* Function Name: Cy_BLE_GetStackLibraryVersion
***************************************************************************//**
*
*  This function retrieves the version information of the BLE Stack library. This
*  is a blocking function. No event is generated on calling this function.
*
*  \param param: Pointer to a variable of type cy_stc_ble_stack_lib_version_t
*                 containing the version information of the CY_BLE Stack library.
*
* \return
*  cy_en_ble_api_result_t: Return value indicates whether the function succeeded or
*  failed. Following are the possible error codes.
*
*   Errors codes                            | Description
*   ------------                            | -----------
*   CY_BLE_SUCCESS                          | On successful operation.
*   CY_BLE_ERROR_INVALID_PARAMETER          | param is NULL.
*
******************************************************************************/
cy_en_ble_api_result_t Cy_BLE_GetStackLibraryVersion
(
    cy_stc_ble_stack_lib_version_t   * param
);


/******************************************************************************
* Function Name: Cy_BLE_StackInit
***************************************************************************//**
*
* This function initializes the BLE Stack and must be called from the higher
* layer initialization routine only once before invoking any other BLE Stack
* function. It includes initialization of the BLE Stack Manager, BLE Controller, and
* BLE Host Stack modules that make up the BLE Stack.
*
* For HCI-Mode of operation, this function will not initialize the BLE Host Stack
* module.
*
* This is a non-blocking function. A call to this function results in the generation of following events
*
*   | Event Name                         | Description                                                                    |
*   | ------------------------------     |--------------------------------------------------------------------------      |
*   | CY_BLE_EVT_STACK_ON                | On successful initialization of Stack                                          |
*   | CY_BLE_EVT_HARDWARE_ERROR          | Sent by Host if Controller Initialization is failed in Dual Core Configuration |
*   | CY_BLE_EVT_STACK_SHUTDOWN_COMPLETE | Sent by Host if Controller Initialization is failed in Dual Core Configuration |
*
*  \param param: parameter is of type 'cy_stc_ble_stack_init_info_t'.
*         param->CyBLEApplCbFunc: Event callback function to receive events from BLE Stack.
*                  cy_ble_app_ev_cb_t is a function pointer type. The application is not expected to call
*                  stack APIs in the stack call back context. Stack execution should be allowed to
*                  return unless the Stack API explicitly mentions otherwise.
*
*                  The following APIs should not be called from the BLE Stack callback context but can be called from the
*                   'CY_BLE_EVT_GATTC_ERROR_RSP' or 'CY_BLE_EVT_GATTC_LONG_PROCEDURE_END' events
*                   or any not long procedure events.
*                     1.  Cy_BLE_GATTC_DiscoverPrimaryServices
*                     2.  Cy_BLE_GATTC_DiscoverPrimaryServiceByUuid
*                     3.  Cy_BLE_GATTC_FindIncludedServices
*                     4.  Cy_BLE_GATTC_DiscoverCharacteristics
*                     5.  Cy_BLE_GATTC_DiscoverCharacteristicByUuid
*                     6.  Cy_BLE_GATTC_DiscoverCharacteristicDescriptors
*                     7.  Cy_BLE_GATTC_ReadLongCharacteristicValues
*                     8.  Cy_BLE_GATTC_WriteLongCharacteristicValues
*                     9.  Cy_BLE_GATTC_ReliableWrites
*                     10. Cy_BLE_GATTC_ReadLongCharacteristicDescriptors
*                     11. Cy_BLE_GATTC_WriteLongCharacteristicDescriptors
*                     12. Cy_BLE_GATTC_ReadByTypeReq
*
*         param->memParam.memoryHeapPtr: Pointer to an array of bytes to be allocated by the BLE component
*               (or the application, if the component's initialization function
*                is not used). The size of the memory to be allocated is as
*                given below for non-HCI mode build of the BLE Stack.
*
*                (CY_BLE_DEFAULT_HOST_RAM_SIZE
*                + Accumulative memory for all dataBuff units 
*                + L2CAP heap requirement
*                + CY_BLE_RAM_SECURE_CONNECTIONS_SIZE (Only if Secure Connections feature is enabled)
*                + (CY_BLE_STACK_ONE_BONDED_DEVICE_SIZE + CY_BLE_STACK_ONE_DEVICE_Q_ENTITY_SIZE) 
*                                        * (CY_BLE_MAX_CONNECTION_INSTANCES + param->bondListConfig.bondListSize)
*                + CY_BLE_STACK_BUFFER_MGR_UTIL_RAM_SZ * totalDataBufferPools)
*
*                Where, CY_BLE_DEFAULT_HOST_RAM_SIZE is a define that is available from
*                the BLE Stack. The starting address of the memoryHeapPtr should be four bytes aligned and
*                the size of the total memory should be divisible by four.
*
*                  Accumulative memory for "single databuff instance" =
*                    (dataBuff.bufferSize * dataBuff.bufferUnits)
*
*                  L2CAP heap requirement =
*                 (CY_BLE_L2CAP_QUEUE_ELEMENT_SIZE * stack queue depth per connection *
*                  maxBleConnections ).
*                  - CY_BLE_L2CAP_QUEUE_ELEMENT_SIZE is a define available in the BLE stack.
*                  - Stack queue depth per connection should be equal to or higher than 
*                    CY_BLE_L2CAP_STACK_Q_DEPTH_PER_CONN.
*                  - maxBleConnections speficies number of connections supported.
*
*                  totalDataBufferPools is "numOfDataBuffSet"
*
*                If HCI mode BLE stack is used, the memory to be allocated should
*                be of size 'CY_BLE_DEFAULT_RAM_SIZE_HCI'.
*
*       param->memParam.totalHeapSz: Size of the total heap memory pointer by memoryHeapPtr. totalHeapSz shall be
*              the accumulated value of every dataBuff elements. To compute total size, each element's
*              size shall be rounded to next 4-byte boundary value.
*
*              For example, a typical value can be computed in following manner:
*
*              Size for buffer pool #1 to be added =
*              ((CY_BLE_ALIGN_TO_4(Size of GATT MTU) + CY_BLE_MEM_EXT_SZ + CY_BLE_L2CAP_HDR_SZ)  *\
*              noOfBuffer)
*
*              Note that the buffer pool size for each chunk may not be 4-byte boundary aligned,
*              and it should be as per user configuration. The BLE Stack's buffer management utility
*              internally aligns it to a higher 4-byte boundary.
*
*       param->memParam.dataBuff: array of structures of type cy_stc_ble_stk_app_data_buff_t. Ignored in case of HCI mode.
*
*       param->memParam.totalDataBufferPools: Number of data buffer sets in the array. Ignored in case of HCI mode.
*
*       param->memParam.bleStackFlashPointer: Pointer to the flash memory. Ignored in case of HCI mode.
*                 The size of the memory to be allocated is as given below for a non-HCI mode build of the BLE Stack:
*                 (
*                  (CY_BLE_STACK_ONE_BONDED_DEVICE_SIZE * User configured Bondlist size)
*                    + (CY_BLE_STACK_ONE_DEVICE_Q_ENTITY_SIZE * (CY_BLE_MAX_CONNECTION_INSTANCES + User configured bondlist size))
*                    + ((CY_BLE_LL_ONE_WHITELIST_HEAP_REQ * User Configured Whitelist size) + 
*                                 CY_BLE_LL_WHITELIST_RETENTION_HEAP_REQ)
*                    + ((CY_BLE_LL_PRIVACY_HEAP_REQ * User Configured Resolving list size) +
*                                 CY_BLE_LL_PRIVACY_RETENTION_HEAP_REQ)
*                 )
*
*      param->memParam.bleStackFlashSize: Size of the total flash memory pointed by 'bleStackFlashPointer'. 
*                                     Ignored in case of HCI mode.
*   
*      param->stackConfig: Stack configuration parameters such as featuremask, Bondlist, Whitelist & Resolving list size
*              configurarions, max number of connections, L2CAP configuration.
*
*  Array dataBuff [totalDataBufferPools] shall provide the information to Stack based on the below table -
*
*  Index | Application config. param  | bufferSize                                                                           | noOfBuffer
*  ----- | -------------------------- | ---------------------------------------------------------------------------------    | ---------------
*    0   | GATT MTU                   | (GATT MTU + CY_BLE_MEM_EXT_SZ + CY_BLE_L2CAP_HDR_SZ)                                 | 3
*    1   | Num. of PSM supported      | ((CY_BLE_L2CAP_PSM_SIZE + CY_BLE_MEM_EXT_SZ) * no of PSM supported)                  | No of PSM supported
*    2   | Num. of L2CAP CBFC channels| ((CY_BLE_L2CAP_CBFC_CHANNEL_SIZE + CY_BLE_MEM_EXT_SZ) * No of L2CAP logical channels)| 2 * No of L2cap logical channels
*    3   | L2CAP MTU (0x17 to 0xFFD0u)| (L2CAP MTU + CY_BLE_MEM_EXT_SZ + CY_BLE_L2CAP_HDR_SZ)                                | 2 * No of L2cap logical channels
*
*   Index position is fixed for each data buffer corresponding to configuration parameters.
*   First four buffers of with valid size (shall be > 12) corresponding to each field are required
*   for BLE_SOC mode operation. Otherwise, the stack will throw an error.
*
*   'dataBuff' will be modified by stack. If the application wants to reuse dataBuff, the application should keep a copy of it.
*
*   *No of buffers should be increased from 3 to Stack queue depth. if mtu>32.
*    stack queue depth = (stack queue depth per connection - 1) * maxBleConnection
*
*
* \return
*  cy_en_ble_api_result_t : Return value indicates whether the function succeeded or
*  failed. Following are the possible error codes.
*
*  Error codes                       | Description
*  ------------                      | -----------
*  CY_BLE_SUCCESS                    | On successful operation.
*  CY_BLE_ERROR_MEMORY_ALLOCATION_FAILED | Memory allocation failed.
*  CY_BLE_ERROR_INVALID_PARAMETER    | On passing a "NULL" for 'CyBLEApplCbFunc' parameter if the BLE stack is not built in HCI mode. On passing a "NULL" for 'memoryHeapPtr' parameter. If maxMtuSize is less than 23 or greater than 512 (only if BLE stack is not built in HCI mode).
*  CY_BLE_ERROR_REPEATED_ATTEMPTS    | On invoking this function more than once without calling Cy_BLE_StackShutdown() function between calls to this function if the BLE stack is not built in HCI mode.
*
******************************************************************************/
/* Event callback function prototype to receive events from stack */
cy_en_ble_api_result_t Cy_BLE_StackInit
(
    cy_stc_ble_stack_init_info_t  * param
);


/******************************************************************************
* Function Name: Cy_BLE_StackShutdown
***************************************************************************//**
*  This function stops any ongoing operation on the BLE Stack and forces the BLE
*  Stack to shut down. The only function that can be called after calling this
*  function is Cy_BLE_StackInit. On calling this function, BLE Radio is turned
*  off; all the data loaded in retention memory are retained and can be used on
*  re-initializing the BLE Stack.
*
*  Shutdown complete is informed through 'CY_BLE_EVT_STACK_SHUTDOWN_COMPLETE' event.
*  Cy_BLE_ProcessEvents must be called to receive this event.
*
*  For UART HCI mode:
*  This is a blocking function and no event is generated.
*  Only CY_BLE_SUCCESS will be returned and other error codes are not applicable.
*  The UART interface will be stopped and UART data will not be processed by the stack 
*  until Cy_BLE_StackInit is invoked.
*
* \return
*  cy_en_ble_api_result_t : Return value indicates whether the function succeeded or
*  failed. Following are the possible error codes.
*
*    Errors codes                    | Description
*    ------------                    | -----------
*    CY_BLE_SUCCESS                  | On successful operation.
*    CY_BLE_ERROR_INVALID_OPERATION  | On calling shutdown before calling stack init.
*
******************************************************************************/
cy_en_ble_api_result_t Cy_BLE_StackShutdown
(
    void
);


/******************************************************************************
* Function Name: Cy_BLE_StackSoftReset
***************************************************************************//**
*
*  This function resets the BLE Stack, including BLE sub-system hardware
*  registers. The BLE Stack transitions to idle mode. This function can be used to
*  reset the BLE Stack if the BLE Stack turns unresponsive due to incomplete
*  transfers with the peer BLE device.
*
*  Reset complete is informed through 'CY_BLE_EVT_SOFT_RESET_COMPLETE event'.
*
* \return
*  cy_en_ble_api_result_t : Return value indicates whether the function succeeded or
*  failed. Following are the possible error codes.
*
*    Errors codes                    | Description
*    ------------                    | -----------
*    CY_BLE_SUCCESS                  | On successful operation.
*    CY_BLE_ERROR_INVALID_OPERATION  | On calling this API before calling stack init.
*
******************************************************************************/
cy_en_ble_api_result_t Cy_BLE_StackSoftReset
(
    void
);


/******************************************************************************
* Function Name: Cy_BLE_ProcessEvents
***************************************************************************//**
*
*  This function checks the internal task queue in the BLE Stack, and pending
*  operation of the BLE Stack, if any. This must be called at least once
*  every interval 't' where:
*   1. 't' is equal to connection interval or scan interval, whichever is
*       smaller, if the device is in GAP Central mode of operation, or
*   2. 't' is equal to connection interval or advertisement interval,
*       whichever is smaller, if the device is in GAP Peripheral mode
*       of operation.
*
*  On calling every interval 't', all pending operations of the BLE Stack are
*  processed. This is a blocking function and returns only after processing all
*  pending events of the BLE Stack. Care should be taken to prevent this call
*  from any kind of starvation; on starvation, events may be dropped by the
*  stack. All the events generated will be propagated to higher layers of the
*  BLE Stack and to the Application layer only after making a call to this
*  function.
*
*  Calling this function can wake BLESS from Low Power Mode, and in the process
*  of waking from BLESS Deep Sleep Mode, the BLE Stack puts the CPU into Sleep Mode to
*  save power while polling for a wakeup indication from BLESS. This can occur if
*  the caller function has pending data or control transactions to be performed
*  in the BLE Stack that must be programmed to BLESS in Cy_BLE_ProcessEvents()
*  context when BLESS is in Low Power Mode.
*
* \return
*  None
*
******************************************************************************/
void Cy_BLE_ProcessEvents
(
    void
);


/******************************************************************************
* Function Name: Cy_BLE_StackSetFeatureConfig
***************************************************************************//**
*
* This API sets the configuration for Bluetooth 4.2 features in the BLE Stack to
* initialize the corresponding data structures and data buffers to support
* the features. The BLE Stack will create the data buffers for the Data length extension
* feature, LE Privacy 1_2, and Secure connections as specified in the parameters
* during time of initialization in the initialization API.
*
* This is a blocking function. No event is generated on calling this function.
*
*  \param param: parameter of type 'CY_BLE_STACK_SET_CONFIG_PARAM_T'
*         param->configParam: parameter of type cy_stc_ble_stack_config_param_t. This structure contains
*                     pointers to config params for individual features. Individual
*                     feature configuration structures can be NULL if that feature is
*                     not selected.
*
*         param->featureHeapReq: Out parameter for returning memory requirement for selected features.
*
*
* \return
* cy_en_ble_api_result_t : Return value indicates if the function succeeded or
* failed. Following are the possible error codes.
*
*  Errors codes                     | Description
*  ------------                     | -----------
*  CY_BLE_SUCCESS                   | On successful operation.
*  CY_BLE_ERROR_INVALID_PARAMETER   | Invalid configuration parameters passed or invalid combination of configParam and featureMask.
*  CY_BLE_ERROR_INVALID_OPERATION   | Invoked after successful stack initialization.
*
******************************************************************************/
cy_en_ble_api_result_t Cy_BLE_StackSetFeatureConfig
(
    cy_stc_ble_stack_config_param_t  * param
);


/*******************************************************************************
* Function Name: Cy_BLE_StackGetFeatureConfig
****************************************************************************//**
*
* This API is used to get Bluetooth 4.2 features configuration made to the BLE Stack
* during Stack initialization. For more details about configuration, please
* refer to API Cy_BLE_StackSetFeatureConfig.
*
* This is a blocking function. No event is generated on calling this function.
*
* \param param: pointer to cy_stc_ble_stack_config_param_t. This structure
*                     contains pointers to config params for individual
*                     features. Application shall provide memory for this
*                     configuration structure.
*
* \return
* cy_en_ble_api_result_t : Return value indicates whether the function succeeded or
* failed. Following are the possible error codes.
*
*  Errors codes                      | Description
*  ------------                      | -----------
*  CY_BLE_SUCCESS                    | On successful operation.
*  CY_BLE_ERROR_INVALID_PARAMETER    | If param pointer is NULL.
*
******************************************************************************/
cy_en_ble_api_result_t Cy_BLE_StackGetFeatureConfig
(
    cy_stc_ble_stack_config_param_t  * param
);

/******************************************************************************
* Function Name: Cy_BLE_EnablePrivacyFeature
***************************************************************************//**
*
* This function Enables Privacy features in the
* BLE Stack. When this function is not called in the application,
* privacy features are disabled and memory space used in the BLE Stack
* is relinquished.
*
* \return
*    None.
*
******************************************************************************/
void Cy_BLE_EnablePrivacyFeature
(
    void
);


/******************************************************************************
* Function Name: Cy_BLE_EnableDleFeature
***************************************************************************//**
*
* This function Enables Data Length Extension features in the
* BLE Stack. When this function is not called in the application,
* DLE features are disabled and memory space used in the BLE Stack
* is relinquished.
*
* \return
*    None.
*
******************************************************************************/
void Cy_BLE_EnableDleFeature
(
    void
);


/******************************************************************************
* Function Name: Cy_BLE_EnablePhyUpdateFeature
***************************************************************************//**
*
* This function Enables PHY Update (for 2 Mbps bitrate) feature in the
* BLE Stack. When this function is not called in the application,
* the PHY features are disabled and memory space used in the BLE Stack
* is relinquished.
*
* \return
*    None.
*
******************************************************************************/
void Cy_BLE_EnablePhyUpdateFeature
(
    void
);

/******************************************************************************
* Function Name: Cy_BLE_SetCustomEventMask
***************************************************************************//**
* This API is used to set the mask that will enable/disable custom events
* 
* Set Custom Event mask complete is informed through the
* 'CY_BLE_EVT_SET_EVENT_MASK_COMPLETE' event.
*
* \param param: Pointer to a variable of type UINT8
*                 containing the mask bits.
*               Following are the supported Mask values
*               CY_BLE_DISABLE_ALL_EVENTS_MASK -  Disables all custom events
*               CY_BLE_CONN_ESTB_EVENT_MASK - For connection establishment
*                 events 
* \return
*  cy_en_ble_api_result_t : Return value indicates whether the function succeeded or
*  failed. Following are the possible error codes.
*
*   Errors codes                          | Description
*   ------------                          | -----------
*   CY_BLE_SUCCESS                        | On successful operation.
*   CY_BLE_ERROR_INVALID_PARAMETER        | One of the input parameter is invalid.
*   CY_BLE_ERROR_MEMORY_ALLOCATION_FAILED | If Memory allocation failed.
*   CY_BLE_ERROR_INSUFFICIENT_RESOURCES   | If Stack resources are unavailable.
*
******************************************************************************/

cy_en_ble_api_result_t Cy_BLE_SetCustomEventMask
(
    uint8_t  *param
);

/******************************************************************************
* Function Name: Cy_BLE_StackEnterLPM
***************************************************************************//**
*
* This function requests the BLE Stack (which includes the BLE Controller, BLE
* Host Stack, and BLE Stack manger) to enter into one of the supported low power
* modes. BLE driver/application should use this function to put the Bluetooth Low Energy
* Sub-System (BLESS) register to Low Power Mode (LPM).
*
* The BLE Stack enters and exits low power modes based on its current state. 
* The application should consider the BLE Stack's current LPM state before configuring
* the CPU or the overall device into LPM. Note that the CPU will not be able to 
* access the BLESS registers when BLESS is in deep sleep mode.
*
* BLE Stack has the following power modes:
*  1. Active
*  2. DeepSleep (Low Power Mode)
*
*  Note that certain conditions may prevent the BLE sub system from entering a
*  particular low power mode.
*
* # Active Mode #
*  BLESS has three sub-modes in Active mode:
*   1. Idle
*   2. Transmit Mode, and
*   3. Receive Mode
*  CPU has full access to the BLESS registers in this mode.
*
* # Deep Sleep Mode #
*  The BLE ECO is stopped (optional) and Watch Crystal Oscillator (WCO) is used to maintain
*  link layer timing. All the regulators in the Radio Frequency (RF) transceiver
*  are turned off to reduce the leakage current and BLESS logic is kept powered ON
*  from the System Resources Sub System (SRSS) deep-sleep regulator for retention
*  of current BLESS state information. This mode can be entered from Idle (Active) mode. 
*  It should be entered when the next scheduled BLE activity instant is greater 
*  than the BLESS Deep Sleep total wakeup time (typically 2 ms).
*  
*  NOTE: If application is using the BLE ECO as source of HFCLK for higher clock accuracy
*  and calls this API to move BLESS to Deep Sleep mode, then BLE ECO is kept ON even in BLESS 
*  DeePSleep mode. 
*
*  Our recommendation is that the application switches the HFCLK to IMO before calling this API
*  to achieve lowest possible system DeepSleep current. On system wakeup due to sources other than BLESS, 
*  the application can switch the HFCLK source to ECO only when BLESS is active. 
*  Recommended clock switching pseudo code is given below.
*  
*    Pseudo Code:
*     -> Turn on IMO and switch HFCLK to IMO
*     Cy_BLE_StackEnterLPM(CY_BLE_BLESS_DEEPSLEEP);
*     Cy_SysPm_DeepSleep();
*     -> If exit is not due to BLE, when wait until BLESS is active before switching HFCLK source to ECO.
*     while(Cy_BLE_StackGetBleSsState() != CY_BLE_BLESS_DEEPSLEEP);
*
* The following table indicates the allowed sleep modes for the complete system
* (BLE Sub-system and the micro-controller). Modes marked In 'X' are the allowed
* combinations. The application layer should make sure that the invalid modes
* are not entered in to:
*
* <code>
*  |-----------|------------------------------------------------|
*  |BLE Stack  |PSoC 6 BLE Microcontroller Low Power Modes     |
*  |LPM Modes  |________________________________________________|
*  |           |  Active   |  Sleep   |  DeepSleep  | Hibernate |
*  |___________|___________|__________|_____________|___________|
*  |   Active  |     X     |    X     |             |           |
*  |___________|___________|__________|_____________|___________|
*  | DeepSleep |           |          |             |           |
*  | (ECO OFF) |     X     |    X     |      X      |           |
*  |___________|___________|__________|_____________|___________|
* </code>
*
* The application layer is responsible for configuring the BLE Sub-system and the
* microcontroller in the desired low power modes. Upon entering the requested
* low power mode combination, the BLE Sub-system and the microcontroller are woken
* up by a BLESS interrupt (Advertisement interrupt, connection interrupt, 
* scan interrupt, etc.). On wakeup, if the application must transmit data, 
* the appropriate function(s), including the Stack functions, must be invoked. 
* This must be followed by a call to Cy_BLE_ProcessEvents routine, which 
* handles all pending transmit and receive operations. The application can now configure 
* the complete system back in to one of the low power modes. The application 
* should ensure that the above invalid states are never encountered.
*
* The application shall also ensure that the BLE Sub-system's low power entry and
* low power exit interrupts are processed in realtime and not blocked. The BLE Sub-system interrupt must be of higher priority.
* If the BLE Sub-system interrupts are blocked for a longer time ( > 200 us ),
* the BLE Sub-system can enter an undesired state, resulting in BLE connection failures.
*
* This is a blocking function. In the process of entering into BLESS Deep Sleep Mode, 
* the BLE Stack puts the CPU into Sleep Mode to save power while polling
* for an entry indication to BLESS DSM. No event is generated on calling this function.
* Based on the return code from this function, the application layer should
* decide on the sleep mode for the complete system. For example, if the return
* code is CY_BLE_BLESS_DEEPSLEEP, the application can choose to call system 
* DeepSleep function.
*
* \param pwrMode: The power mode that the BLE Stack is intended to
* enter. The allowed value is CY_BLE_BLESS_DEEPSLEEP
*
* \return
* cy_en_ble_lp_mode_t : The actual power mode to which the BLE Stack is now set.
*
******************************************************************************/
cy_en_ble_lp_mode_t Cy_BLE_StackEnterLPM(cy_en_ble_lp_mode_t pwrMode);

/******************************************************************************
* Function Name: Cy_BLE_StackGetBleSsState
***************************************************************************//**
* 
*  This function returns the BLE Subsystem's current operational mode. This state 
*  can be used to manage system level power modes.
* 
* \return:
* cy_en_ble_bless_state_t : BLESS's operating mode has one of the following values
*
*  BLE Stack Mode                | Description
*  --------------                | -----------
*  CY_BLE_BLESS_STATE_ACTIVE     | BLE Sub System is in active mode, CPU can be in active mode or sleep mode.
*  CY_BLE_BLESS_STATE_EVENT_CLOSE| BLE Sub System radio and Link Layer hardware finished Tx/Rx. In this state, the application can try configuring the stack to Deep Sleep State to save power.
*  CY_BLE_BLESS_STATE_ECO_STABLE | BLE Sub System is in the process of waking up from Deep Sleep Mode and the BLE ECO is stable. The CPU can be configured in sleep mode.
*  CY_BLE_BLESS_STATE_DEEPSLEEP  | BLE Sub System is in Deep Sleep Mode. CPU can be configured in deep sleep mode.
* 
******************************************************************************/
cy_en_ble_bless_state_t Cy_BLE_StackGetBleSsState(void);

/******************************************************************************
* Function Name: Cy_BLE_IsControllerActive
***************************************************************************//**
* 
*  This function checks whether any of the hardware engines of the BLE controller are
*  active. If either of ADV/SCAN/INIT/CONN engine is active, it returns CY_BLE_SUCCESS.
* 
* \param checkForMode: Sleep mode which controller core is trying to enter
*               CY_BLE_CONTROLLER_SLEEP_MODE_SLEEP -  Check whether controller core can enter Sleep mode
*               CY_BLE_CONTROLLER_SLEEP_MODE_DEEPSLEEP -  Check whether controller core can enter DeepSleep mode
*
* \return
*  cy_en_ble_api_result_t : Returns whether the controller is active or not.
*
*   Return value                          | Description
*   ------------                          | -----------
*   CY_BLE_SUCCESS                        | BLE Controller is active.
*   CY_BLE_ERROR_INVALID_OPERATION        | BLE Controller is not active.
* 
******************************************************************************/
cy_en_ble_api_result_t Cy_BLE_IsControllerActive(cy_en_ble_controller_sleep_mode_t checkForMode);

 /** @} */
#endif //CY_BLE_STACK_H_
