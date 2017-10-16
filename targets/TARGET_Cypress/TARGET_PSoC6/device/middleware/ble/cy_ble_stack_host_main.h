/*******************************************************************************
* \file cy_ble_stack_host_main.h
* \version 2.0
*
* \brief
*  This file contains declarations of public BLE APIs other than those covered by
*  GAP, GATT, and L2CAP specific APIs. Also specified are the defines, constants,
*  and data structures required for the APIs.
* 
* Related Document:
*  BLE Standard Spec - CoreV4.2, CSS, CSAs, ESR05, ESR06
* 
********************************************************************************
* \copyright
* Copyright 2014-2017, Cypress Semiconductor Corporation.  All rights reserved.
* You may use this file only in accordance with the license, terms, conditions,
* disclaimers, and limitations in the end user license agreement accompanying
* the software package with which this file was provided.
*******************************************************************************/

#ifndef CY_BLE_STACK_HOST_MAIN_H_
#define CY_BLE_STACK_HOST_MAIN_H_


/***************************************
* Common stack includes
***************************************/
#include "BLE_config.h"
#include "cy_ble_stack.h"

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

/***************************************
** Constants
***************************************/
/** Bluetooth Stack busy status */
#define CY_BLE_STACK_STATE_BUSY                  (0x01u) /**< stack busy status */
#define CY_BLE_STACK_STATE_FREE                  (0x00u) /**< stack free status */

/***************************************
**  Memory pool configuration data defines
***************************************/
/** Maximum number of Connections */
#define CY_BLE_MAX_CONNECTION_INSTANCES          (0x04u)

/** Size of the heap when GATT MTU, L2CAP MTU, and MPS sizes are specified as 23 Bytes */
/** An extra 256 bytes is added as we allocate eight chunks of 32 bytes in the buffer pool and
   64 u bytes is the overhead required by mint buf management to store each extra chunk ( 8 chunks * 8 bytes overhead).
   4 bytes of default memory for AES engine, 40u bytes for SMP-AES engine  */ 
#define CY_BLE_DEFAULT_HOST_RAM_SIZE             (16u + 1024u + 824u + 120u + 256u + 64u +4u +40u)

/** L2CAP Header size */
#define CY_BLE_L2CAP_HDR_SZ                      (0x04u)

/** Buffer size needed for L2cap PSM  */
#define CY_BLE_L2CAP_PSM_SIZE                    (0x28u) 
/** Buffer size needed for L2cap CBFC channels */
#define CY_BLE_L2CAP_CBFC_CHANNEL_SIZE           (0x20u)

/** Minimum stack queue depth requirement per connection */
/** The application can choose to give higher queue depth for better throughput. */
#define CY_BLE_L2CAP_STACK_Q_DEPTH_PER_CONN       (0x06u)
/** 
 * L2cap Queue element size specifies the size of single element to hold L2CAP data.
 * The application must provide memory based on the number of queue elements required per connection.
 * Ex: if the application needs a stack queue size of five elements per connection and supports four connections,
 * then the application must provide memory as per the following calculation
 * Memory = CY_BLE_L2CAP_QUEUE_ELEMENT_SIZE * CY_BLE_L2CAP_STACK_Q_DEPTH_PER_CONN * 4;
 */ 
#define CY_BLE_L2CAP_QUEUE_ELEMENT_SIZE           (0x18u)

/**
 * L2cap per connection heap requirement. This memory includes memory for L2cap per connection context alone
 */
#define CY_BLE_RAM_SIZE_L2CAP_SINGLE_CONN             (0x90u)

/**
 * HCI per connecton heap requirement . This memory include memory for Hci remote device structure(8 bytes) and hci command queue 
 * with depth of 6 per connection(0x30 bytes).
 */
#define CY_BLE_RAM_SIZE_HCI_SINGLE_CONN              (0x38u) 
 
 /**
  * GAP module per connection heap requirement 
  */
#define CY_BLE_RAM_SIZE_GAP_SINGLE_CONN              (0x04u)
 
 /**
  * GATT and ATT module per connection heap requirement 
  */
#define CY_BLE_RAM_SIZE_GATT_SINGLE_CONN             (0x104u)  
 
 /**
  * AES module per connection heap requirement 
  */
#define CY_BLE_RAM_SIZE_AES_SINGLE_CONN             (0x08u)  
 
 /**
  * SMP module per connection heap requirement includes memory for SMP Context and
  * SMP platform context
  */
#define CY_BLE_RAM_SIZE_SMP_SINGLE_CONN              (0x132u)   

/**
 *  Device queue heap requirement per connection. This requirement includes Device Queue and Device queue extension RPA
 */
 #define CY_BLE_RAM_SIZE_DEVICE_Q_SINGLE_CONN         (CY_BLE_STACK_ONE_DEVICE_Q_ENTITY_SIZE + CY_BLE_STACK_DEVICE_Q_EXTN_RPA_SIZE )
 
/**
 * Host per connection heap requirement . Application needs to provide memory as per following calculation 
 *  = (CY_BLE_HOST_PER_CONN_HEAP_REQ * CY_BLE_MAX_CONNECTION_INSTANCES)
 */
#define CY_BLE_RAM_SIZE_HOST_SINGLE_CONN             (CY_BLE_RAM_SIZE_HCI_SINGLE_CONN \
                                                      +  CY_BLE_RAM_SIZE_L2CAP_SINGLE_CONN \
                                                      +  CY_BLE_RAM_SIZE_GAP_SINGLE_CONN \
                                                      +  CY_BLE_RAM_SIZE_GATT_SINGLE_CONN \
                                                      +  CY_BLE_RAM_SIZE_AES_SINGLE_CONN \
                                                      +  CY_BLE_RAM_SIZE_SMP_SINGLE_CONN \
                                                      +  CY_BLE_STACK_ONE_BONDED_DEVICE_SIZE \
                                                      +  CY_BLE_RAM_SIZE_DEVICE_Q_SINGLE_CONN)

/** Size of the heap required for Secure connection feature
 * This includes memory for SMP FSM, SMP Crypto tool box, and ECC
 */
#define CY_BLE_RAM_SIZE_SECURE_SINGLE_CONN        (0x3B0u) /**< SC single connection */
/** SC multiconnection */
#define CY_BLE_RAM_SIZE_SECURE_CONNECTIONS        (CY_BLE_MAX_CONNECTION_INSTANCES * CY_BLE_RAM_SIZE_SECURE_SINGLE_CONN)

/** Data Buffer Pool identifiers to support configurable parameters for L2CAP and
 * GATT in the BLE Stack
 * These data buffer pools must be configured during BLE Stack
 * initialization. */

/** Data Buffer index for ATT/GATT Configured MTU Size */
#define CY_BLE_STACK_APP_POOL_1_SZ                (0x00u)

/** Data Buffer index for L2CAP configuration for number of PSM Channels
 * */
#define CY_BLE_STACK_APP_POOL_2_SZ                (0x01u)

/** Data Buffer index for L2CAP configuration for number of CBFC Channels
 * */
#define CY_BLE_STACK_APP_POOL_3_SZ                (0x02u)

/** Data Buffer index for L2CAP configured MTU Size */
#define CY_BLE_STACK_APP_POOL_4_SZ                (0x03u)

/** Maximum Application Pool  */
#define CY_BLE_STACK_APP_MIN_POOL                 (CY_BLE_STACK_APP_POOL_4_SZ + 1u)

/** GATT minimum attribute length */
#define CY_BLE_GATT_MAX_ATTR_LEN_MIN_VAL          (0x00u)

/** GATT minimum attribute length buffers*/
#define CY_BLE_GATT_MIN_NO_OF_ATT_MTU_BUFF        (0x03u)

/** Max attribute size can be of 0xFFFF but the Stack has a limitation
 * on buffer creation as it requires an additional eight bytes and 
 * L2CAP header size (four bytes)
 * */
#define CY_BLE_GATT_MAX_ATTR_LEN_MAX_VAL          (0xFFD0u)

/** L2CAP MTU minimum value */
#define CY_BLE_L2CAP_MTU_MIN_VALUE                (0x17u)

/** Max L2CAP MTU or MPS size can be of 0xFFFF, but the Stack has a limitation
 * on buffer creation as it requires an additional eight bytes and 
 * L2CAP header size (four bytes)
 * */
#define CY_BLE_L2CAP_MTU_MAX_VALUE                (0xFFD0u)

/** L2CAP MPS minimum value */
#define CY_BLE_L2CAP_MPS_MIN_VALUE                CY_BLE_L2CAP_MTU_MIN_VALUE

/** L2CAP MPS maximum value */
#define CY_BLE_L2CAP_MPS_MAX_VALUE                CY_BLE_L2CAP_MTU_MAX_VALUE

/** Default Bonded device size */
#define CY_BLE_GAP_DEFAULT_BONDED_DEVICE_SIZE     (0x10u)

/** Size of single bonded device entry*/
#define CY_BLE_STACK_ONE_BONDED_DEVICE_SIZE       (0xA8u)

/** Size of internal device queue */

#define CY_BLE_STACK_DEVICE_Q_EXTN_RPA_SIZE       (0x1Cu)

/** Size of internal device queue */

#define CY_BLE_STACK_ONE_DEVICE_Q_ENTITY_SIZE    (0x10u)

/** Size of the checksum to be stored in Flash memory  */
#define CY_BLE_STACK_CHECKSUM_SIZE                    (0x2u)

/**  Flash size is configurable by component; configuration is mentioned below.
    * \#define CY_BLE_STACK_FLASH_STORAGE_SIZE       CY_BLE_STACK_CHECKSUM_SIZE + (total memory required for 
    * flash storage based on below calculation)
    *
    * Bonded device list memory computation
    * CY_BLE_GAP_DEFAULT_BONDED_DEVICE_SIZE:
    *       It is a user-configured value for the size of bondlist using BLE PDL
    * Memory required =
    *((CY_BLE_STACK_ONE_BONDED_DEVICE_SIZE * User configured bondlist size)
    *                                        + (CY_BLE_STACK_ONE_DEVICE_Q_ENTITY_SIZE *  User configured bondlist size))
    *
    * Whitelist list memory computation 
    * CY_BLE_MAX_WHITELIST_LIST_SZ: 
    *       It is a user-configured value for the size of whitelist using the BLE PDL.
    *       Range: 1 to 16.
    * Memory required =
    * (((CY_BLE_LL_ONE_WHITELIST_HEAP_REQ * CY_BLE_MAX_WHITELIST_LIST_SZ) +
    *                                           CY_BLE_LL_WHITELIST_RETENTION_HEAP_REQ)
    *
    * Resolving list memory computation 
    * CY_BLE_MAX_RESOLVABLE_DEVICES: 
    *       It is a user-configured value for the size of resolving list using the BLE PDL.
    *       Range: 1 to 16, 0 if Privacy 1.2 feature is not used.
    * Memory required =
    * (((CY_BLE_LL_PRIVACY_HEAP_REQ * CY_BLE_MAX_RESOLVABLE_DEVICES) +
    *                                         CY_BLE_LL_PRIVACY_RETENTION_HEAP_REQ)
    */

/** Encryption information */
/** encryption data size */
#define CY_BLE_ENCRYPTION_DATA_SIZE              (0x10u) 
/** encryption key size */
#define CY_BLE_ENCRYPTION_KEY_SIZE               (0x10u) 
/** encryption nonce size */
#define CY_BLE_ENCRYPTION_NONCE_SIZE             (0x0Du) 
/** BLE MIC size */
#define CY_BLE_MIC_SIZE                          (0x04u) 

/** SMP Key size */
#define CY_BLE_IRK_SIZE                          (0x10u)

/** Bitmask values for Flash write */
#define CY_BLE_PERSISTENT_BOND_LIST_BITMASK      (0x01u) /**< Bondlist mask */
#define CY_BLE_PERSISTENT_RPA_LIST_BITMASK       (0x02u) /**< Resolvablelist mask */
#define CY_BLE_PERSISTENT_WHITELIST_BITMASK      (0x04u) /**< whiteList mask */

/** Application to provide or free memory */
#define CY_BLE_ALLOC_MEMORY                      (0x00u) /**< Allocate memory */
#define CY_BLE_FREE_MEMORY                       (0x01u) /**< Free memory */

/***************************************
** Enumerated Types
***************************************/
/**
 \addtogroup group_ble_common_api_definitions
 @{
*/

/** Timeout happened reason due to stack started times or application stated timers. 
    It is the application's responsibility to disconnect or keep the channel on depending on type of timeout;
    i.e., GATT procedure timeout: the application may choose to disconnect.*/
typedef enum
{
    /** Advertisement time set by application has expired */
    CY_BLE_GAP_ADV_TO = 0x01u,
    
    /** Scan time set by application has expired */
    CY_BLE_GAP_SCAN_TO,

    /** GATT procedure timeout */
    CY_BLE_GATT_RSP_TO,

    /** Application procedure timeout */
    CY_BLE_GENERIC_APP_TO
    
}cy_en_ble_to_reason_code_t;

/** BLE Stack memory request type */
typedef enum
{
    /** Memory requested for prepare write request */
    CY_BLE_PREPARED_WRITE_REQUEST = 0x00u,

    /** Invalid request */
    CY_BLE_INVALID_REQUEST
        
}cy_en_ble_protocol_req_t;

/** Phy used for the connection */
typedef enum
{
    /** The transmitter PHY for the connection is LE 1M */
    CY_BLE_PHY_MASK_LE_1M = 0x01u,

    /** The transmitter PHY for the connection is LE 2M */
    CY_BLE_PHY_MASK_LE_2M,

    /** The transmitter PHY for the connection is LE Coded */
    CY_BLE_PHY_MASK_LE_CODED

}cy_en_ble_phy_mask_t;


/** PHY preference masks used for Cy_BLE_SetPhy API. */
typedef enum
{
    /** All PHY settings are preferred by the host */
    CY_BLE_PHY_NO_PREF_MASK_NONE = 0x00u,

    /** Host has no preference for TX PHY */
    CY_BLE_PHY_NO_PREF_MASK_TX,

    /** Host has no preference for RX PHY */
    CY_BLE_PHY_NO_PREF_MASK_RX,
    
    /** Host has no preference for both TX & RX PHY */
    CY_BLE_PHY_NO_PREF_MASK_BOTH_TX_RX

}cy_en_ble_phy_no_pref_mask_t;


/** Privacy modes used for Cy_BLE_SetPrivacyMode API. */
typedef enum
{
    /* Network privacy mode */
    CY_BLE_PRIVACY_MODE_NETWORK = 0x00u,
    /* Device privacy mode */
    CY_BLE_PRIVACY_MODE_DEVICE,
}cy_en_ble_privacy_mode_t;



/***************************************
** Exported structures and unions
***************************************/

/** callback function prototype to receive DTM packet from Controller */
typedef uint16_t (*cy_ble_hci_rx_cb_t)(uint8_t type, uint8_t  * packet, uint8_t length);


/** Bluetooth Device Addresses */
typedef struct
{
    /** Public Bluetooth device address of size 6 bytes */
    uint8_t     *publicBdAddr;

    /** Private Bluetooth device address of size 6 bytes */
    uint8_t     *privateBdAddr;
}cy_stc_ble_bd_addrs_t;

/** Connection Handle */
typedef struct
{
    /** Identifies the peer device(s) bonded or in current connection. The Stack supports
       CY_BLE_GAP_MAX_BONDED_DEVICE+CY_BLE_MAX_CONNECTION_INSTANCES devices.
       first device connected is assigned a value (CY_BLE_GAP_MAX_BONDED_DEVICE
       +CY_BLE_MAX_CONNECTION_INSTANCES)-1. If one device is present in the bonded list, 
       then the current device will be assigned the value
       (CY_BLE_GAP_MAX_BONDED_DEVICE+CY_BLE_MAX_CONNECTION_INSTANCES)-2,
       else (CY_BLE_GAP_MAX_BONDED_DEVICE+CY_BLE_MAX_CONNECTION_INSTANCES)-1.
    */
    uint8_t           bdHandle;

    /** Identifies the active ATT connection Instance.
    */
    uint8_t           attId;

}cy_stc_ble_conn_handle_t;

/** Store BLE Application Data parameter into flash */
typedef struct
{
    /** BIT field to specify the type of list to be stored in flash
      * CY_BLE_PERSISTENT_BOND_LIST_BITMASK  - B0 Bond List Data
      * CY_BLE_PERSISTENT_RPA_LIST_BITMASK   - B1 Resolving List Data
      * CY_BLE_PERSISTENT_WHITELIST_BITMASK  - B2 Whitelist Data
      * Bit B3 is internally used by the stack and other bits are reserved for
      * future use.
      */
    uint8_t     bitField;
} cy_stc_ble_stack_flash_param_t;

/** Timer parameter */
typedef struct
{
    /** Handle of the started timer */
    uint8_t    timerHandle;
    
    /** Timeout for which timer to be started
      * IF MSBit of timeout == 1, then resolution is in milliseconds,
      * ELSE resolution is seconds.
      * This takes the assumption that no one would call timeout of,
      * say, 32000 seconds.
      */
    uint8_t    timeout;

} cy_stc_ble_timer_info_t;

/**
 * Configuration structure for enabling selective features 
 * and passing associated parameters in BLE Stack.
 */
typedef struct
{
    /** Seed for DRBG */
    uint32_t      seed;

}cy_stc_ble_stack_random_num_param_t;

/** Tx power level configuration parameters */
typedef struct
{
    /** Channel group ID for which power level is to be read/written */
    cy_en_ble_bless_phy_ch_grp_id_t     bleSsChId;

    /** Device handle for which power level is to be read/written */
    uint8_t                           bdHandle;

} cy_stc_ble_tx_pwr_config_param_t;

/** Tx power level parameter */
typedef struct
{
    /** Power level */
    cy_en_ble_bless_pwr_lvl_t           blePwrLevel;

    /** Tx power level configuration parameters */
    cy_stc_ble_tx_pwr_config_param_t     pwrConfigParam;

} cy_stc_ble_tx_pwr_lvl_info_t;

/** RSSI parameter */
typedef struct
{
    /** The RSSI value 
            Range is -85 <= N <= 5 in dBm */
    int8_t      rssi; 

    /** Peer device handle */
    uint8_t     bdHandle;
    
}cy_stc_ble_rssi_info_t;

/** AES encrypt information */
typedef struct
{
    /** data containing plain text (128-bit) that is to be encrypted */
    uint8_t   plainData[CY_BLE_ENCRYPTION_DATA_SIZE];

    /** Pointer to the AES Key (128-bit) that is to be used for AES encryption*/
    uint8_t   aesKey[CY_BLE_ENCRYPTION_KEY_SIZE];
    
}cy_stc_ble_aes_encrypt_info_t;

/** AES CCM encrypt information */
typedef struct
{
    /** array of bytes holding the key */
    uint8_t   key[CY_BLE_ENCRYPTION_KEY_SIZE];

    /** array of bytes */
    uint8_t   nonce[CY_BLE_ENCRYPTION_NONCE_SIZE];

    /** Pointer to an array of bytes to be encrypted. The size of the array should be equal to 
          the value of the 'length' parameter*/
    uint8_t   *data;

    /** length of data */
    uint8_t   length;
    
}cy_stc_ble_aes_ccm_encrypt_info_t;

/** AES CCM decrypt information */
typedef struct
{
    /** array of bytes holding the key */
    uint8_t   key[CY_BLE_ENCRYPTION_KEY_SIZE];

    /** array of bytes */
    uint8_t   nonce[CY_BLE_ENCRYPTION_NONCE_SIZE];

    /** Pointer to an array of bytes to be encrypted. Size of the array should be equal to 
          the value of the 'length' parameter*/
    uint8_t   * data;
    
    /** Pointer to an array of bytes (four Bytes) to provide the MIC value
          generated during encryption */
    uint8_t   * mic;

    /** length of data */
    uint8_t   length;
    
}cy_stc_ble_aes_ccm_decrypt_info_t;

/** authentication payload information */
typedef struct
{
    /** authentication timeout value */
    uint16_t      authPayloadTimeout;

    /** peer device handle */
    uint8_t       bdHandle;
    
}cy_stc_ble_auth_payload_info_t;

/** Slave Latency Parameters */
typedef struct
{
    /** Set or reset the force quick transmit configuration in the BLE Stack.
         '1': Set the quick transmit behavior. It gets set immediately and disables
        over the air slave latency. This quick transmit setting remains true until
        the application gives control to the BLE Stack for controlling the quick transmit bit.
        '0': Reset the force quick transmit behavior in BLESS to allow the BLE Stack to control
        quick transmit behavior when slave latency is applied. */
    uint8_t   setForceQuickTransmit;
    
    /** remote device identifier */
    uint8_t   bdHandle;
    
}cy_stc_ble_slave_latency_info_t;

/** DLE Set Data Length parameter */
typedef struct
{
    /** Peer bdHandle */
    uint8_t       bdHandle;

    /** The maximum number of payload octets in a Link Layer Data Channel
     * PDU that the local Controller will send on the current connection.
     */
    uint16_t      connMaxTxOctets;

    /** The maximum time that the local Controller will take to send a Link
     * Layer Data Channel PDU on the current connection. 
     */
    uint16_t      connMaxTxTime;

    /** The maximum number of payload octets in a Link Layer Data Channel
     * PDU that the local Controller will receive on the current connection.
     */
    uint16_t      connMaxRxOctets;

    /** The maximum time that the local Controller will take to receive a Link
     * Layer Data Channel PDU on the current connection.
     */
    uint16_t      connMaxRxTime;

}cy_stc_ble_set_data_length_info_t;

/** DLE Suggested Data Length parameter */
typedef struct
{
    /** Controller's maximum transmitted number of payload 
     * octets to be used for new connections.
     */
    uint16_t      suggestedTxOctets;

    /** Controller's maximum packet transmission time to be 
     * used for new connections. 
     */
    uint16_t      suggestedTxTime;

}cy_stc_ble_suggested_data_length_info_t;

/** Channel Map Parameters Information  */
typedef struct 
{
    /** Peer bdHandle */
    uint8_t       bdHandle;

    /** This parameter contains five octet byte stream (Least Significant
              Byte having the bit fields 0 to 7, most significant byte having
              the bit fields 32 to 36). The nth such field (in the range 0 to
              36) contains the value for the link layer channel index n. 
              Allowed values and their interpretation are,
              * Channel 'n' is bad = 0x00u
              * Channel 'n' is unknown = 0x01u
              The most significant bits (37 to 39) are reserved and will be 
              set to 0. At least one channel will be marked as unknown. 
                 For example-
                 expected pattern = XX XX XX XX 1F
                 not expected =  XX XX XX XX 10, XX XX XX XX 2f 
                 MSB 3 bits should be not set. (1F is most significant byte in this case) */
    uint8_t       * channelMap;      

}cy_stc_ble_channel_map_info_t;

/** Resolving list information of one device */
typedef struct
{
    /** Peer IRK */
    uint8_t       peerIrk[CY_BLE_IRK_SIZE];

    /** Local IRK */
    uint8_t       localIrk[CY_BLE_IRK_SIZE];

    /** Peer Identity device address  */
    uint8_t       bdAddr[CY_BLE_BD_ADDR_SIZE];

    /** Peer Identity addr type */
    uint8_t       type; /*public = 0, Random = 1*/

}cy_stc_ble_resolving_device_info_t;

/** Configuration Structure for Set Privacy Mode */
typedef struct
{
    /** Peer Identity device address  */
    uint8_t       bdAddr[CY_BLE_BD_ADDR_SIZE];

    /** Peer Identity addr type */
    uint8_t       type; /*public = 0, Random = 1*/

    /** Privacy Mode */
    cy_en_ble_privacy_mode_t privacyMode;

}cy_stc_ble_privacy_mode_info_t;


/** Random number generated from the Stack */
typedef struct
{
    /** public = 0, Random = 1 */
    uint8_t     type;

    /** Bluetooth device address */
    uint8_t     bdAddr[CY_BLE_BD_ADDR_SIZE];

}cy_stc_ble_peer_id_addr_t;

/** Random number generated from the Stack */
typedef struct
{
    /** standard HCI command/data packet */
    uint8_t       * data;

    /** length of data */
    uint16_t      dataLength;

    /** HCI packet type*/
    uint8_t       packetType;

}cy_stc_ble_hci_tx_packet_info_t;

/** Set Phy parameter */
typedef struct
{
    /** Peer bdHandle */
    uint8_t                    bdHandle;

    /** Host preference for PHY setting. Non preferred PHY settings are ignored by the controller */
    cy_en_ble_phy_no_pref_mask_t allPhyMask;

    /** The transmitter PHY for the connection */
    cy_en_ble_phy_mask_t         txPhyMask;

    /** The receiver PHY for the connection */
    cy_en_ble_phy_mask_t         rxPhyMask;

    /** Parameter to specify coding preference*/
    uint16_t                   phyOption;

}cy_stc_ble_set_phy_info_t;

/** Set Default Phy parameter */
typedef struct
{
    /** Host preference for PHY setting. Non preferred PHY settings are ignored by the controller */
    cy_en_ble_phy_no_pref_mask_t    allPhyMask;

    /** The transmitter PHY for the connection */
    cy_en_ble_phy_mask_t    txPhyMask;

    /** The receiver PHY for the connection */
    cy_en_ble_phy_mask_t    rxPhyMask;

}cy_stc_ble_set_suggested_phy_info_t;

/* --------------------------Structure corresponding to events-------------------- */

/** Local suggested or Max 'tx octets' and 'tx time' */
typedef struct
{
    /** Controller's maximum transmitted number of payload 
     * octets to be used for new connections
     */
    uint16_t      suggestedTxOctets;

    /** Controller's maximum packet transmission time to be 
     * used for new connections.
     */
    uint16_t      suggestedTxTime;

    /** Maximum number of payload octets that the local Controller supports
     * for transmission of a single Link Layer Data Channel PDU.
     */
    uint16_t      maxTxOctets;

    /** Maximum time, in microseconds, that the local Controller supports for
     * transmission of a single Link Layer Data Channel PDU. 
     */
    uint16_t      maxTxTime;

    /** Maximum number of payload octets that the local Controller supports
     * for reception of a single Link Layer Data Channel PDU.
     */
    uint16_t      maxRxOctets;

    /** Maximum time, in microseconds, that the local Controller supports for
     * reception of a single Link Layer Data Channel PDU. 
     */
    uint16_t      maxRxTime;

}cy_stc_ble_data_length_param_t;

/** LE Data Length Change event parameter */
typedef struct
{
    /** The maximum number of payload octets in a Link Layer Data Channel
     * PDU that the local Controller will send on the current connection.
     */
    uint16_t      connMaxTxOctets;

    /** The maximum time that the local Controller will take to send a Link
     * Layer Data Channel PDU on the current connection.
     */
    uint16_t      connMaxTxTime;

    /** The maximum number of payload octets in a Link Layer Data Channel
     * PDU that the local controller expects to receive on the current connection.
     */
    uint16_t      connMaxRxOctets;

    /** The maximum time that the local Controller expects to take to receive a
     * Link Layer Data Channel PDU on this connection. 
     */
    uint16_t      connMaxRxTime;
    
    /** Peer bdHandle */
    uint8_t       bdHandle;

}cy_stc_ble_data_length_change_event_param_t;


/** Memory request parameters */
typedef struct
{
    /** Protocol Request type*/
    cy_en_ble_protocol_req_t    request;

    /** This is an output parameter that the application must fill and pass to the BLE Stack as per below table:
    
         request                        | memory                             
         ------------                   | ------------  
         CY_BLE_PREPARED_WRITE_REQUEST   | cy_stc_ble_prepare_write_request_memory_t
    */
    void                    * configMemory;

    /** The event parameter is generated to allocate memory or to free up previously allocated memory
            CY_BLE_ALLOC_MEMORY (0) = to allocate memory for request type, 
            CY_BLE_FREE_MEMORY (1) = free previously allocated memory for the request type*/
    uint8_t                   allocFree;

    /** Connection handle */
    cy_stc_ble_conn_handle_t     connHandle; 

} cy_stc_ble_memory_request_t;

/** AES CCM encrypted data information */
typedef struct
{
    /** MIC value generated during encryption */
    uint8_t   mic[CY_BLE_MIC_SIZE];

    /** Pointer to an array of size 'length' where the encrypted/decrypted data
            is stored */
    uint8_t   *data;

    /** length of data */
    uint8_t   length;
    
}cy_stc_ble_aes_ccm_param_t;

/** Timeout event parameter */
typedef struct
{
    /** Bluetooth device address */
    cy_en_ble_to_reason_code_t      reasonCode;

    /** handle */
    union
    {
        /** Connection Handle */
        cy_stc_ble_conn_handle_t     connHandle;

        /* Peer device handle */
        uint8_t                   bdHandle;

    }CY_BLE_HANDLE;

    /** Handle of the started timer */
    uint8_t                       timerHandle;

}cy_stc_ble_timeout_param_t;

/** Generic Event parameter structure */
typedef struct
{
    /** HCI error code as defined in Bluetooth 4.1 core specification, Volume 2,
             Part D, section 1.3  */
    uint8_t   status;

    /** Event Parameter associated to event */
    void    *eventParams;
    
}cy_stc_ble_events_param_generic_t;

/** Set Phy parameter */
typedef struct
{
    /** Peer bdHandle */
    uint8_t               bdHandle;

    /** The transmitter PHY for the connection */
    cy_en_ble_phy_mask_t    txPhyMask;

    /** The receiver PHY for the connection */
    cy_en_ble_phy_mask_t    rxPhyMask;

}cy_stc_ble_phy_param_t;

/** LL control procedure param structure */
typedef struct
{
    /** Peer bdHandle */
    uint8_t bdHandle;

    /** LL Control procedure status - TRUE/FALSE */
    uint8_t cntrlProcStatus;
    
}cy_stc_ble_ll_cntrl_proc_param_t;

/** Whitelist information param */
typedef struct
{
    /** Peer ID Address type, public = 0, Random = 1 */
    uint8_t type;
    
    /** Peer ID Address */
    uint8_t bdAddr[CY_BLE_BD_ADDR_SIZE];
    
} cy_stc_ble_white_list_t;

/** Whitelist retention structure */
typedef struct
{
    /** Size of the Whitelist */
    uint8_t size;

    /** Pointer to Whitelist Array */ 
    const cy_stc_ble_white_list_t  *whiteList;
    
} cy_stc_ble_white_list_retention_t ;

/** Resolving list information param */
typedef struct
{
    /** Internally used by the Stack */
    uint16_t flags;

    /** Internally used by the Stack */    
    uint16_t extendedFlags;
    
    /** Peer ID Address */
    uint8_t bdAddr[CY_BLE_BD_ADDR_SIZE];
    
    /** Peer ID Address type, public = 0, Random = 1 */
    uint8_t type;
    
    /** Local IRK */
    uint8_t localIrk[CY_BLE_IRK_SIZE];
    
    /** Peer IRK */
    uint8_t peerIrk[CY_BLE_IRK_SIZE];
    
}cy_stc_ble_rpa_list_info_t;

/** Resolving list retention structure */
typedef struct
{
    /** Size of Resolving List */
    uint8_t size;

    /** Pointer to Resolving List Array */
    cy_stc_ble_rpa_list_info_t *resolvingList;
    
}cy_stc_ble_resolving_list_retention_t;
/** @} */

/***************************************
** Exported APIs
***************************************/
/**
 \addtogroup group_ble_common_api_functions
 @{
*/

/******************************************************************************
* Function Name: Cy_BLE_StoreStackData
***************************************************************************//**
* 
*  This function instructs the Stack to backup the Stack internal RAM data into flash.
*  This API must be called by the application to back up Stack data. If this API is not 
*  called appropriately, the Stack internal data structure will not be available on the 
*  power cycle.
*     
*  \param param: Parameter is of type 'cy_stc_ble_stack_flash_param_t'.
*   
* \return
*  cy_en_ble_api_result_t : Return value indicates whether the function succeeded or 
*  failed. Following are the possible error codes.
*
*  Errors codes                              | Description
*  ------------                              | -----------
*   CY_BLE_SUCCESS                           | On successful operation.
*   CY_BLE_INFO_FLASH_WRITE_IN_PROGRESS      | Flash write in progress, application needs to  call Cy_BLE_StoreStackData to complete the operation.
*   CY_BLE_ERROR_FLASH_WRITE                 | Error in flash Write. 
*   CY_BLE_ERROR_INVALID_PARAMETER           | If param pointer is NULL.
******************************************************************************/
cy_en_ble_api_result_t Cy_BLE_StoreStackData
(
    cy_stc_ble_stack_flash_param_t  * param
);


/******************************************************************************
* Function Name: Cy_BLE_StartTimer 
***************************************************************************//**
* 
*  This function provides a timer start utility to the BLE-Component. The BLE-Component can
*  use this timer for certain operations like deferring writing CCCD to flash.
*  This function should not be used for generic purposes as the timer ticks are
*  updated only when there are LL activities. The BLE stack internally takes care of the 
*  timing requirement for GATT, GAP, and L2CAP signaling on-going procedures.
*  Maximum timer support for application is restricted to 4.
*
*  \param param: parameter of type 'cy_stc_ble_timer_info_t'
*  param->timerHandle: output parameter.
* 
* \return
*  cy_en_ble_api_result_t : Return value indicates whether the function succeeded or
*  failed. Following are the possible error codes.
*
*  Errors codes                              | Description
*   ------------                             | -----------
*   CY_BLE_SUCCESS                           | On successful operation.
*   CY_BLE_ERROR_INVALID_PARAMETER           | Timeout is set to zero.   
*   CY_BLE_ERROR_INVALID_OPERATION           | On failed operation.
* 
******************************************************************************/
cy_en_ble_api_result_t Cy_BLE_StartTimer
(
    cy_stc_ble_timer_info_t  * param
);

/******************************************************************************
* Function Name: Cy_BLE_StopTimer 
***************************************************************************//**
* 
*  This function provides a timer stop utility to the BLE-Component. The BLE-Component can  
*  use this timer for certain operations like deferring writing CCCD to flash.  
*  This function should not be used for generic purposes as the timer ticks are  
*  updated only when there are LL activities. The BLE stack internally takes care of the 
*  timing requirement for GATT, GAP, and L2CAP signaling on-going procedures.
* 
*  \param param: parameter of type 'cy_stc_ble_timer_info_t'
*   param->timeout: to be ignored.
*  param->timerHandle: input parameter.
*
* \return
*  cy_en_ble_api_result_t : Return value indicates whether the function succeeded or
*  failed. Following are the possible error codes.
*
*  Errors codes                              | Description
*   ------------                             | -----------
*   CY_BLE_SUCCESS                           | On successful operation.
*   CY_BLE_ERROR_INVALID_OPERATION           | On failed operation (Timer is already stopped or expired).
* 
******************************************************************************/
cy_en_ble_api_result_t Cy_BLE_StopTimer
(
    cy_stc_ble_timer_info_t  * param
);


/******************************************************************************
* Function Name: Cy_BLE_GetRssi
***************************************************************************//**
* 
*  This function reads the recorded Received Signal Strength Indicator (RSSI) 
*  value for the last successfully received packet from the BLE radio 
*  sub-system. 
*  RSSI value is informed thorough 'CY_BLE_EVT_GET_RSSI_COMPLETE'
*     
*  \param param: Parameter is of type 'cy_stc_ble_rssi_info_t'
*   param->rssi: ignore this parameter
* 
* \return
*  cy_en_ble_api_result_t : Return value indicates whether the function succeeded or
*  failed. Following are the possible error codes.
*
*  Errors codes                              | Description
*   ------------                             | -----------
*   CY_BLE_SUCCESS                           | On successful operation.
*   CY_BLE_ERROR_INVALID_PARAMETER           | On specifying NULL as input parameter.
*   CY_BLE_ERROR_NO_DEVICE_ENTITY            | If connection does not exists for corresponding 'bdHandle'.
*   CY_BLE_ERROR_MEMORY_ALLOCATION_FAILED    | If Memory allocation failed.
*   CY_BLE_ERROR_INSUFFICIENT_RESOURCES      | If Stack resources are unavailable.
*
*   Information     | Description
*   -----------     | -----------
*   Range           | -85 <= N <= 5
*   \note: The value is in dBm.
*     
******************************************************************************/
cy_en_ble_api_result_t Cy_BLE_GetRssi
(
    cy_stc_ble_rssi_info_t  * param
);


/******************************************************************************
* Function Name: Cy_BLE_GetTxPowerLevel
***************************************************************************//**
* 
*  This function reads the transmit power of the BLE radio for the given BLE
*  sub-system channel group. 
* 
*  Tx power level is informed thorough 'CY_BLE_EVT_GET_TX_PWR_COMPLETE'
*
*  \param param: Pointer to a variable of type 'cy_stc_ble_tx_pwr_config_param_t' where, 
*                * cy_stc_ble_tx_pwr_config_param_t -> bleSsChId indicates Channel group for which
*                  power level is to be read. The value can be advertisement
*                  channels (CY_BLE_LL_ADV_CH_TYPE) or data channels 
*                  (CY_BLE_LL_CONN_CH_TYPE).
*                * bdHandle indicates peer device handle. This is not applicable for advertisement
*                  channels (CY_BLE_LL_ADV_CH_TYPE).
*
* \return
*  cy_en_ble_api_result_t : Return value indicates whether the function succeeded or
*  failed. Following are the possible error codes.
*
*    Errors codes                          | Description
*    ------------                          | -----------
*    CY_BLE_SUCCESS                        | On successful operation.
*    CY_BLE_ERROR_INVALID_PARAMETER        | On specifying NULL as input parameter.  
*    CY_BLE_ERROR_NO_DEVICE_ENTITY         | If connection does not exists for corresponding 'bdHandle'.
*    CY_BLE_ERROR_MEMORY_ALLOCATION_FAILED | If Memory allocation failed.
*    CY_BLE_ERROR_INSUFFICIENT_RESOURCES   | If Stack resources are unavailable.
* 
******************************************************************************/
cy_en_ble_api_result_t Cy_BLE_GetTxPowerLevel
(
    cy_stc_ble_tx_pwr_config_param_t  * param
);


/******************************************************************************
* Function Name: Cy_BLE_SetTxPowerLevel
***************************************************************************//**
* 
*  This function sets the transmit power of the BLE radio for the given BLE
*  sub-system channel group.
*  Successful operation is informed through 'CY_BLE_EVT_SET_TX_PWR_COMPLETE' event.
*
* \param param: Parameter is of type 'cy_stc_ble_tx_pwr_lvl_info_t' where,
*                * blePwrLevel indicates Output Power level to be set by the function.
*                * cy_stc_ble_tx_pwr_config_param_t -> bleSsChId indicates Channel group for which
*                  power level is to be set. The value can be advertisement
*                  channels (CY_BLE_LL_ADV_CH_TYPE) or data channels 
*                  (CY_BLE_LL_CONN_CH_TYPE).
*                * cy_stc_ble_tx_pwr_config_param_t -> bdHandle indicates peer device handle. This
*                  is not applicable for advertisement channels (CY_BLE_LL_ADV_CH_TYPE). Set bdHandle to 0xFFu 
*                  to use 'blePwrLevel' as the default power level for all connections.
* \return
*  cy_en_ble_api_result_t : Return value indicates whether the function succeeded or
*  failed. Following are the possible error codes.
*
*    Errors codes                          | Description
*    ------------                          | -----------
*    CY_BLE_SUCCESS                        | On successful operation.
*    CY_BLE_ERROR_INVALID_PARAMETER        | On specifying NULL as input parameter.
*    CY_BLE_ERROR_NO_DEVICE_ENTITY         | If connection does not exists for corresponding 'bdHandle'.
*    CY_BLE_ERROR_MEMORY_ALLOCATION_FAILED | If Memory allocation failed.
*    CY_BLE_ERROR_INSUFFICIENT_RESOURCES   | If Stack resources are unavailable.
* 
******************************************************************************/
cy_en_ble_api_result_t Cy_BLE_SetTxPowerLevel
(
    cy_stc_ble_tx_pwr_lvl_info_t  * param
);


/******************************************************************************
* Function Name: Cy_BLE_GetBleClockCfgParam
***************************************************************************//**
* 
*  This function reads the clock configuration parameter of the BLE sub-system. 
*  This is not a blocking function. Clock config paramter is informed through 
*  the 'CY_BLE_EVT_GET_CLK_CONFIG_COMPLETE' event.
* 
*  The following parameters related to the BLE sub-system clock are read by this
*  function:
* 
*  __Sleep Clock accuracy__

*   Sleep clock accuracy (SCA) in PPM. This parameter indicates the sleep clock
*   accuracy in PPM as described in the following table. It is set in the BLE
*   Stack and is used for BLE Connection operation while creating LE connection
*   with the peer device.
* 
*     Sleep Clock Accuracy Enum Field       | PPM Range Translation (PPM)
*     -------------------------------       | ---------------------------
*     CY_BLE_LL_SCA_251_TO_500_PPM          | 251 - 500
*     CY_BLE_LL_SCA_151_TO_250_PPM          | 151 - 250
*     CY_BLE_LL_SCA_101_TO_150_PPM          | 101 - 150
*     CY_BLE_LL_SCA_076_TO_100_PPM          | 76 - 100
*     CY_BLE_LL_SCA_051_TO_075_PPM          | 51 - 75
*     CY_BLE_LL_SCA_031_TO_050_PPM          | 31 - 50
*     CY_BLE_LL_SCA_021_TO_030_PPM          | 21 - 30
*     CY_BLE_LL_SCA_000_TO_020_PPM          |  0 - 20
* 
*   Refer to Bluetooth Core Specification 4.1 Volume 6, Chapter 4.5.7 for more
*   details on how the SCA is used.
* 
*  __Link Layer clock divider__

*   This input decides the frequency of the clock to the link layer. A lower
*   clock frequency results in lower power consumption. Default clock frequency
*   for the operation is 24 MHz. BLESS supports 24 MHz, 12 MHz, and 8 MHz clock
*   configurations. Based on the end application requirement (how frequent the 
*   communication is expected to be), this parameter must be set.
* 
*  cy_en_ble_api_result_t : Return value indicates whether the function succeeded or
*  failed. Following are the possible error codes.
*
*    Errors codes                          | Description
*    ------------                          | -----------
*    CY_BLE_SUCCESS                        | On successful operation.
*    CY_BLE_ERROR_MEMORY_ALLOCATION_FAILED | If Memory allocation failed.
*    CY_BLE_ERROR_INSUFFICIENT_RESOURCES   | If Stack resources are unavailable.

******************************************************************************/   
cy_en_ble_api_result_t Cy_BLE_GetBleClockCfgParam
(
    void
);


/******************************************************************************
* Function Name: Cy_BLE_SetBleClockCfgParam
***************************************************************************//**
* 
*   
*  This function sets the clock configuration parameter of the BLE sub-system. This
*  is a blocking function. 
*  Successful operation is informed through event 'CY_BLE_EVT_SET_CLK_CONFIG_COMPLETE'.
*  The following parameters related to the BLE sub-system clock are set by this
*  function:
* 
*  __Sleep Clock accuracy__

*   Sleep clock accuracy (SCA) in PPM. This parameter indicates the sleep clock
*   accuracy in PPM as described in the following table. It is set in the BLE
*   Stack and is used for BLE Connection operation while creating LE connection
*   with the peer device.
*
*     Sleep Clock Accuracy Enum Field       | PPM Range Translation (PPM)
*     -------------------------------       | ---------------------------
*     CY_BLE_LL_SCA_251_TO_500_PPM          | 251 - 500
*     CY_BLE_LL_SCA_151_TO_250_PPM          | 151 - 250
*     CY_BLE_LL_SCA_101_TO_150_PPM          | 101 - 150
*     CY_BLE_LL_SCA_076_TO_100_PPM          | 76 - 100
*     CY_BLE_LL_SCA_051_TO_075_PPM          | 51 - 75
*     CY_BLE_LL_SCA_031_TO_050_PPM          | 31 - 50
*     CY_BLE_LL_SCA_021_TO_030_PPM          | 21 - 30
*     CY_BLE_LL_SCA_000_TO_020_PPM          |  0 - 20
* 
*   Refer to Bluetooth Core Specification 4.1 Volume 6, Chapter 4.5.7 for more
*   details on how the SCA is used.
* 
*  __Link Layer clock divider__

*   This input decides the frequency of the clock to the link layer. A lower
*   clock frequency results in lower power consumption. Default clock frequency
*   for the operation is 24 MHz. BLESS supports 24 MHz, 12 MHz, and 8 MHz clock
*   configurations. Based on the end application requirement (how frequent the 
*   communication is expected to be), this parameter must be set.
*
*  \param param: Pointer to a variable of type cy_stc_ble_bless_clk_cfg_params_t
*                     from which the existing clock configuration is taken.
* 
* \return
*  cy_en_ble_api_result_t : Return value indicates whether the function succeeded or
*  failed. Following are the possible error codes.
*
*    Errors codes                          | Description
*    ------------                          | -----------
*    CY_BLE_SUCCESS                        | On successful operation.
*    CY_BLE_ERROR_INVALID_PARAMETER        | If param pointer is NULL
*    CY_BLE_ERROR_MEMORY_ALLOCATION_FAILED | If Memory allocation failed.
*    CY_BLE_ERROR_INSUFFICIENT_RESOURCES   | If Stack resources are unavailable.

*   
******************************************************************************/ 
cy_en_ble_api_result_t Cy_BLE_SetBleClockCfgParam
(
    cy_stc_ble_bless_clk_cfg_params_t  * param
); 

/******************************************************************************
* Function Name: Cy_BLE_GenerateRandomNumber
***************************************************************************//**
* 
* This function sets the application-specific seed for DRBG (Deterministic Random number generator). 
* This function generates 8-byte random number that complies with pseudo random
*  number generation in accordance with [FIPS PUB 140-2]. Random number
*  generation function is used during security procedures documented in Bluetooth
*  4.1 core specification, Volume 3, Part H. 
*  
*  Generated random number is informed through event 'CY_BLE_EVT_RANDOM_NUM_GEN_COMPLETE'
*
* \param param: parameter is of type 'cy_stc_ble_stack_random_num_param_t'
*  param->seed: Seed for DRBG. Setting the seed to zero is functionally 
*               equivalent to not setting the application-specific seed.
* 
* \return
*  cy_en_ble_api_result_t : Return value indicates whether the function succeeded or
*  failed. Following are the possible error codes.
*
*    Errors codes                          | Description
*    ------------                          | -----------
*    CY_BLE_SUCCESS                        | On successful operation.
*    CY_BLE_ERROR_MEMORY_ALLOCATION_FAILED | If Memory allocation failed.
*    CY_BLE_ERROR_INSUFFICIENT_RESOURCES   | If Stack resources are unavailable.
* 
******************************************************************************/ 
cy_en_ble_api_result_t Cy_BLE_GenerateRandomNumber
(
    cy_stc_ble_stack_random_num_param_t  * param
);

/******************************************************************************
* Function Name: Cy_BLE_AesEncrypt
***************************************************************************//**
* 
*  This function uses BLE sub-system AES engine to encrypt 128-bit of plain text
*  using the given AES key. The output of AES processing is copied to 
*  encryptedData buffer. Refer to Bluetooth 4.1 core specification, Volume 3,
*  Part H, section 2.2 for more details on usage of AES key.
* 
*  Completion of the API is informed through  event 'CY_BLE_EVT_AES_ENCRYPT_COMPLETE'
* 
*  \param param: Parameter of type 'cy_stc_ble_aes_encrypt_info_t'.
*   param->plainData: Pointer to the data containing plain text (128-bit) that is to be encrypted.
* 
*   param->aesKey: Pointer to the AES Key (128-bit) that is to be used for AES encryption.
* 
* \return
*  cy_en_ble_api_result_t : Return value indicates whether the function succeeded or
*  failed. Following are the possible error codes.
* 
*    Errors codes                               | Description
*    ------------                               | -----------
*    CY_BLE_SUCCESS                             | On successful operation.
*    CY_BLE_ERROR_INVALID_PARAMETER             | On specifying NULL as input parameter.
*    CY_BLE_ERROR_MEMORY_ALLOCATION_FAILED      | If Memory allocation failed.
*    CY_BLE_ERROR_INSUFFICIENT_RESOURCES        | AES engine is being used internally or stack resources are unavailable.
* 
******************************************************************************/ 
cy_en_ble_api_result_t Cy_BLE_AesEncrypt
(   
    cy_stc_ble_aes_encrypt_info_t  * param
);   

/******************************************************************************
* Function Name: Cy_BLE_AesCcmEncrypt
***************************************************************************//**
* 
*  This function encrypts the given data. 
* 
*  Encrypted data is informed through event 'CY_BLE_EVT_AES_CCM_ENCRYPT_COMPLETE'
*
* \param param: Parameter is of type 'cy_stc_ble_aes_ccm_encrypt_info_t ' 
* 
*  \return
*  cy_en_ble_api_result_t: Return value indicates whether the function succeeded or
*  failed. Following are the possible error codes.
*
*   Errors codes                          |   Description
*   ------------                          |   -----------
*   CY_BLE_SUCCESS                        |   On successful operation.
*   CY_BLE_ERROR_INVALID_PARAMETER        |   One of the inputs is a null pointer. 
*   CY_BLE_ERROR_MEMORY_ALLOCATION_FAILED |   If Memory allocation failed.
*   CY_BLE_ERROR_INSUFFICIENT_RESOURCES   |   If Stack resources are unavailable.
* 
******************************************************************************/
cy_en_ble_api_result_t Cy_BLE_AesCcmEncrypt
(
    cy_stc_ble_aes_ccm_encrypt_info_t  * param
);


/******************************************************************************
* Function Name: Cy_BLE_AesCcmDecrypt
***************************************************************************//**
* 
*  This function decrypts the given data.
* 
*  Decrypted data is informed through event 'CY_BLE_EVT_AES_CCM_DECRYPT_COMPLETE'
*
* \param param: Parameter is of type 'cy_stc_ble_aes_ccm_decrypt_info_t ' 
* 
* \return
*  cy_en_ble_api_result_t: Return value indicates whether the function succeeded or
*  failed. Following are the possible error codes.
*
*  Error codes                            | Description
*  ------------                           | -----------
*   CY_BLE_SUCCESS                        | On successful operation.
*   CY_BLE_ERROR_INVALID_PARAMETER        | One of the inputs is a NULL pointer.
*   CY_BLE_ERROR_MEMORY_ALLOCATION_FAILED | Memory allocation failed.
*   CY_BLE_ERROR_INSUFFICIENT_RESOURCES   | Stack resources are unavailable.
* 
******************************************************************************/
cy_en_ble_api_result_t Cy_BLE_AesCcmDecrypt
(
    cy_stc_ble_aes_ccm_decrypt_info_t  * param
);

/******************************************************************************
* Function Name: Cy_BLE_GenerateAesCmac
***************************************************************************//**
* 
*  This API enables the application to generate the AES CMAC of 16 bytes,
*  for a given variable length message and CMAC Key.
* 
*  After this API call, if the return value is CY_BLE_SUCCESS,
*  then CY_BLE_EVT_AES_CMAC_GEN_COMPLETE event indicates completion of cmac generation.
*  Check the output parameter cmac to get the generated cmac value after completion of CMAC generation.
*
* \param cmacGenParam: Parameter is of type 'cy_stc_ble_aes_cmac_generate_param_t ' 
* 
* \return
*  cy_en_ble_api_result_t: Return value indicates whether the function succeeded or
*  failed. Following are the possible error codes.
*
*  Error codes                     | Description
*  ------------                    | -----------
*   CY_BLE_SUCCESS                 | On successful operation.
*   CY_BLE_ERROR_INVALID_PARAMETER | One of the inputs is a NULL pointer, the 'buffer' can be NULL only if 'size' is zero.
*   CY_BLE_ERROR_INVALID_OPERATION | Application has already initiated AES CMAC operation that is pending completion.
* 
******************************************************************************/
cy_en_ble_api_result_t Cy_BLE_GenerateAesCmac
(
    cy_stc_ble_aes_cmac_generate_param_t *cmacGenParam
);


/******************************************************************************
* Function Name: Cy_BLE_WriteAuthPayloadTimeout
***************************************************************************//**
* 
*  This function sets the Authentication Payload timeout in the BLE Controller for
*  LE_PING feature. Refer to Bluetooth 4.1 core specification, Volume 6, Part B,
*  section 4.6.5 for LE Ping operation.
*
*  The 'CY_BLE_EVT_WRITE_AUTH_PAYLOAD_TO_COMPLETE' event is triggered on successful setting of the Authentication Payload timeout in BLE Controller for
*  LE_PING feature.
*  
*  The 'CY_BLE_EVT_LE_PING_AUTH_TIMEOUT' event is triggered if the peer device has not responded
*  with the valid MIC packet within the application configured ping authentication time.
*     
* \param param: Parameter is of type 'cy_stc_ble_auth_payload_info_t ' 
*  param->bdHandle: Peer device handle.
*  param->authPayloadTimeout: Variable containing authentication timeout value to be
*                      written to the BLE Controller. Details on this parameter are
*                      as given below:
*                      - Value Range = 0x0001 to 0xFFFF
*                      - Default Value (N) = 3000 (30 seconds)
*                      - Time Calculation = N x 10 ms
*                      - Time Range = 10 ms to 655,350 ms
* 
* \return
*  cy_en_ble_api_result_t: Return value indicates whether the function succeeded or
*  failed. Following are the possible error codes.
*
*   Errors codes                          | Description
*   ------------                          | -----------
*   CY_BLE_SUCCESS                        | On successful operation.
*   CY_BLE_ERROR_INVALID_PARAMETER        | On specifying NULL as input parameter.
*   CY_BLE_ERROR_NO_DEVICE_ENTITY         | If connection does not exists for corresponding 'bdHandle'.
*   CY_BLE_ERROR_MEMORY_ALLOCATION_FAILED | If Memory allocation failed.
*   CY_BLE_ERROR_INSUFFICIENT_RESOURCES   | If Stack resources are unavailable.
* 
******************************************************************************/
cy_en_ble_api_result_t Cy_BLE_WriteAuthPayloadTimeout
(
    cy_stc_ble_auth_payload_info_t  * param
);


/******************************************************************************
* Function Name: Cy_BLE_ReadAuthPayloadTimeout
***************************************************************************//**
* 
*  This function reads the Authentication Payload timeout set in the BLE Controller
*  for LE_PING feature. Refer to Bluetooth 4.1 core specification, Volume 6, Part B,
*  section 4.6.5 for LE Ping operation.
* 
*  Timeout is informed through event 'CY_BLE_EVT_READ_AUTH_PAYLOAD_TO_COMPLETE'
*
* \param param: Parameter is of type 'cy_stc_ble_auth_payload_info_t ' 
*  param->bdHandle: Peer device handle.
*  param->authPayloadTimeout: to be ignored.
* 
* \return
*  cy_en_ble_api_result_t: Return value indicates whether the function succeeded or
*  failed. Following are the possible error codes.
*
*   Errors codes                          | Description
*   ------------                          | -----------
*   CY_BLE_SUCCESS                        | On successful operation.
*   CY_BLE_ERROR_INVALID_PARAMETER        | On specifying NULL as input parameter.
*   CY_BLE_ERROR_NO_DEVICE_ENTITY         | If connection does not exists for corresponding 'bdHandle'.
*   CY_BLE_ERROR_MEMORY_ALLOCATION_FAILED | If Memory allocation failed.
*   CY_BLE_ERROR_INSUFFICIENT_RESOURCES   | If Stack resources are unavailable.
* 
******************************************************************************/
cy_en_ble_api_result_t Cy_BLE_ReadAuthPayloadTimeout
(
    cy_stc_ble_auth_payload_info_t  * param
);


/******************************************************************************
* Function Name: Cy_BLE_SetSlaveLatencyMode
***************************************************************************//**
*
* This function overrides the default BLE Stack behavior for the LE connection that
* is established with non zero slave latency. This API can be used by the application
* to force set quick transmission for a link related to a specified 'bdHandle' during the
* slave latency period. 
* 
* If the force quick transmit option is selected, the device will always respond to
* all the Connection Events (CE) ignoring the slave latency. To re-enable the BLE Stack
* control quick transmit behavior, the application should call this API with force 
* quick transmit option set to zero.
*
* BLE Stack Control Policy: The BLE Stack enables quick transmission whenever any
* data packet is queued in the link layer. Upon successful transmission of a data packet, the
* BLE Stack resets the quick transmit to enable latency for power save.
*
* The BLE Stack also enables quick transmit whenever any real time LL Control PDU
* is received. After the acknowledgement of the PDU is processed, the quick transmit
* option is reset.
*
*  Command completion is informed through event 'CY_BLE_EVT_SET_SLAVE_LATENCY_MODE_COMPLETE'
*
* \param param: parameter is of type 'cy_stc_ble_slave_latency_info_t'
*
* \return
* cy_en_ble_api_result_t: Return value indicates whether the function succeeded or
* failed. Following are the possible error codes.
*
*  Errors codes                          | Description
*  ------------                          | -----------
*  CY_BLE_SUCCESS                        | On successful operation.
*  CY_BLE_ERROR_INVALID_PARAMETER        | On specifying NULL as input parameter.
*  CY_BLE_ERROR_NO_DEVICE_ENTITY         | If connection does not exists for corresponding 'bdHandle'.
*  CY_BLE_ERROR_MEMORY_ALLOCATION_FAILED | If Memory allocation failed.
*  CY_BLE_ERROR_INSUFFICIENT_RESOURCES   | If Stack resources are unavailable.
*
******************************************************************************/
cy_en_ble_api_result_t Cy_BLE_SetSlaveLatencyMode
(
    cy_stc_ble_slave_latency_info_t  * param
);


/******************************************************************************
* Function Name: Cy_BLE_SetDataLength
***************************************************************************//**
*
* This API allows the application to suggest maximum transmission packet size and maximum packet transmission time 
* for the current connection. 
* This is a non-blocking function. Successful setting of controller operation is 
* informed through 'CY_BLE_EVT_SET_DATA_LENGTH_COMPLETE'.
* Actual data length used by controller will be informed through the 'CY_BLE_EVT_DATA_LENGTH_CHANGE' event.
*
*  \param param: parameter is of type 'cy_stc_ble_set_data_length_info_t'.
*
* \return
*  cy_en_ble_api_result_t : Return value indicates whether the function succeeded or
* failed. Following are the possible error codes.
*
*  Errors codes                          | Description
*  ------------                          | -----------
*  CY_BLE_SUCCESS                        | On successful operation.
*  CY_BLE_ERROR_INVALID_PARAMETER        | On specifying NULL as input parameter.
*  CY_BLE_ERROR_NO_DEVICE_ENTITY         | If connection does not exists for corresponding 'bdHandle'.
*  CY_BLE_ERROR_MEMORY_ALLOCATION_FAILED | If Memory allocation failed.
*  CY_BLE_ERROR_INSUFFICIENT_RESOURCES   | If Stack resources are unavailable.                                    
*
******************************************************************************/
cy_en_ble_api_result_t Cy_BLE_SetDataLength
(
    const cy_stc_ble_set_data_length_info_t  * param
);


/******************************************************************************
* Function Name: Cy_BLE_SetSuggestedDataLength
***************************************************************************//**
*
* This API allows the application to specify its preferred values for the Link 
* Layer maximum Tx packet (Data Channel PDU) size (connInitialMaxTxOctets) and 
* maximum Tx packet transmission time (connInitialMaxTxTime) to be used for new 
* connections. 
* This is a non-blocking function. Successful setting of controller operation is 
* informed through 'CY_BLE_EVT_SET_SUGGESTED_DATA_LENGTH_COMPLETE'.
* Actual data length used by controller will be informed through the 'CY_BLE_EVT_DATA_LENGTH_CHANGE' event.
*
*  \param param: parameter is of type 'cy_stc_ble_suggested_data_length_info_t'.
*
*  \return
*  cy_en_ble_api_result_t : Return value indicates whether the function succeeded or
*  failed. Following are the possible error codes.
*
*  Errors codes                          | Description
*  ------------                          | -----------
*  CY_BLE_SUCCESS                        | On successful operation.
*  CY_BLE_ERROR_INVALID_PARAMETER        | On specifying NULL for input parameter.
*  CY_BLE_ERROR_MEMORY_ALLOCATION_FAILED | If Memory allocation failed.
*  CY_BLE_ERROR_INSUFFICIENT_RESOURCES   | If Stack resources are unavailable. 
*
******************************************************************************/
cy_en_ble_api_result_t Cy_BLE_SetSuggestedDataLength
(
    const cy_stc_ble_suggested_data_length_info_t  * param
);


/******************************************************************************
* Function Name: Cy_BLE_GetDataLength
***************************************************************************//**
*
* This API allows the application to read the suggested and maximum Tx/Rx packet size and
* suggested and maximum Tx/Rx time that the BLE Stack uses. 
*
* This is a non-blocking function. 
* Current data length parameters are informed through 'CY_BLE_EVT_GET_DATA_LENGTH_COMPLETE'
*
*
* \return
*  cy_en_ble_api_result_t : Return value indicates whether the function succeeded or
*  failed. Following are the possible error codes.
*
*  Errors codes                          | Description
*  ------------                          | -----------
*  CY_BLE_SUCCESS                        | On successful operation.
*  CY_BLE_ERROR_MEMORY_ALLOCATION_FAILED | If Memory allocation failed.
*  CY_BLE_ERROR_INSUFFICIENT_RESOURCES   | If Stack resources are unavailable. 
*
******************************************************************************/
cy_en_ble_api_result_t Cy_BLE_GetDataLength
(
    void
);


/******************************************************************************
* Function Name: Cy_BLE_AddDeviceToResolvingList
***************************************************************************//**
*
* This API is used to add a device to the resolving list in the controller for
* resolving Resolvable Private Address (RPA). This API can be used to update
* local and/or peer IRKs for an existing Resolving List entry by passing the
* same peer address type and peer address in the argument.
* 
*  Completion of operation is informed through the 'CY_BLE_EVT_ADD_DEVICE_TO_RPA_LIST_COMPLETE' event.
*  Successful operation triggers the 'CY_BLE_EVT_PENDING_FLASH_WRITE' event.
*
*  \param param: parameter is of type 'cy_stc_ble_resolving_device_info_t'
*
* \return
*  cy_en_ble_api_result_t : Return value indicates whether the function succeeded or failed.
*                       Following are the possible error codes.
*
*  Error codes                           | Description
*  ------------                          | -----------
*  CY_BLE_SUCCESS                        | On successful operation.
*  CY_BLE_ERROR_INVALID_PARAMETER        | On specifying NULL for input parameter.
*  CY_BLE_ERROR_MEMORY_ALLOCATION_FAILED | Memory allocation failed.
*  CY_BLE_ERROR_INSUFFICIENT_RESOURCES   | Stack resources are unavailable.
*
******************************************************************************/
cy_en_ble_api_result_t Cy_BLE_AddDeviceToResolvingList
(
    cy_stc_ble_resolving_device_info_t  * param
);


/******************************************************************************
* Function Name: Cy_BLE_GetPeerResolvableAddress
***************************************************************************//**
*
* This API is used to get the current peer Resolvable Private Address being used for the corresponding 
* peer Public and Random (static) Identity Address. The peer's resolvable address being used may 
* change after the command is called.  
*
* This is a non-blocking function. 
* Peer resolvable address is informed through 'CY_BLE_EVT_GET_PEER_RPA_COMPLETE'
*
*  \param param: Buffer that contains the information of peer identity address and address type
*
*  \return
*  cy_en_ble_api_result_t : Return value indicates whether the function succeeded or
* failed. Following are the possible error codes.
*
*  Errors codes                          | Description
*  ------------                          | -----------
*  CY_BLE_SUCCESS                        | On successful operation.
*  CY_BLE_ERROR_INVALID_PARAMETER        | Input parameter is NULL.
*  CY_BLE_ERROR_MEMORY_ALLOCATION_FAILED | If Memory allocation failed.
*  CY_BLE_ERROR_INSUFFICIENT_RESOURCES   | If Stack resources are unavailable.
*
******************************************************************************/
cy_en_ble_api_result_t Cy_BLE_GetPeerResolvableAddress
(
    cy_stc_ble_peer_id_addr_t   * param
);


/******************************************************************************
* Function Name: Cy_BLE_GetLocalResolvableAddress
***************************************************************************//**
*
* This API is used to get the current local Resolvable Private Address being used for the corresponding peer
* Identity Address. The local resolvable address being used may change after the command is called.
*
* This is a non-blocking function. 
* Local resolvable address is informed through 'CY_BLE_EVT_GET_LOCAL_RPA_COMPLETE'
*
*  \param param: Buffer that contains the information of peer identity address and address type.
*
* \return
*  cy_en_ble_api_result_t : Return value indicates whether the function succeeded or
* failed. Following are the possible error codes.
* 
*  Errors codes                          | Description
*  ------------                          | -----------
*  CY_BLE_SUCCESS                        | On successful operation.
*  CY_BLE_ERROR_INVALID_PARAMETER        | Input parameter is NULL.
*  CY_BLE_ERROR_MEMORY_ALLOCATION_FAILED | If Memory allocation failed.
*  CY_BLE_ERROR_INSUFFICIENT_RESOURCES   | If Stack resources are unavailable.
*
******************************************************************************/
cy_en_ble_api_result_t Cy_BLE_GetLocalResolvableAddress
(
    cy_stc_ble_peer_id_addr_t     * param
);


/******************************************************************************
* Function Name: Cy_BLE_RemoveDeviceFromResolvingList
***************************************************************************//**
*
* This API is used to remove one device from the list of address translations used to resolve Resolvable 
* Private Addresses in the BLE Stack.
* Passing all zero address as parameter clears the resolving list.
* 
*  Completion of operation is informed through 'CY_BLE_EVT_REMOVE_DEVICE_FROM_RPA_LIST_COMPLETE' event.
*  Successful operation triggers the 'CY_BLE_EVT_PENDING_FLASH_WRITE' event.
*
*  \param param: Buffer that contains the information of peer ID address and address type. 
*               If all bytes of peer ID address as well as address type is zero, this API will clear resolving list.
*
* \return
*  cy_en_ble_api_result_t : Return value indicates whether the function succeeded or
* failed. Following are the possible error codes.
*
*  Error codes                           | Description
*  ------------                          | -----------
*  CY_BLE_SUCCESS                        | On successful operation.
*  CY_BLE_ERROR_INVALID_PARAMETER        | On specifying NULL for input parameter.
*  CY_BLE_ERROR_MEMORY_ALLOCATION_FAILED | Memory allocation failed.
*  CY_BLE_ERROR_INSUFFICIENT_RESOURCES   | Stack resources are unavailable.
*
******************************************************************************/
cy_en_ble_api_result_t Cy_BLE_RemoveDeviceFromResolvingList
(
    cy_stc_ble_peer_id_addr_t  * param
);


/******************************************************************************
* Function Name: Cy_BLE_SetResolvablePvtAddressTimeOut
***************************************************************************//**
*
* This API is used to set the length of time the controller uses a Resolvable Private Address before a new 
* resolvable private address is generated and starts being used.  This timeout applies to all addresses 
* generated by the BLE Stack.
*
* This is a non-blocking function. 
* Request complete is informed through 'CY_BLE_EVT_SET_RPA_TO_COMPLETE'
*
*  \param param: RPA_Timeout measured in seconds.
*           Range for N: 0x0001 - 0xA1B8 (1 sec - approximately 11.5 hours)
*           Default: N= 0x0384 (900 secs or 15 minutes)
*
* \return
*  cy_en_ble_api_result_t : Return value indicates whether the function succeeded or
* failed. Following are the possible error codes.
*
*  Errors codes                          | Description
*  ------------                          | -----------
*  CY_BLE_SUCCESS                        | On successful operation.
*  CY_BLE_ERROR_MEMORY_ALLOCATION_FAILED | If Memory allocation failed.
*  CY_BLE_ERROR_INSUFFICIENT_RESOURCES   | If Stack resources are unavailable.
******************************************************************************/
cy_en_ble_api_result_t Cy_BLE_SetResolvablePvtAddressTimeOut
(
    uint16_t      param
);


/******************************************************************************
* Function Name: Cy_BLE_SetAddressResolutionEnable
***************************************************************************//**
*
* This API is used to enable resolution of Resolvable Private Addresses in the BLE Stack. This causes the
* BLE Stack to use the resolving list whenever the Controller receives a local or peer Resolvable Private Address.
*
* This is a non-blocking function. 
* Request complete is informed through 'CY_BLE_EVT_SET_RPA_ENABLE_COMPLETE'
*
*  \param param: 0x00 - Address Resolution in controller disabled (default) \n
*                        0x01 - Address Resolution in controller enabled \n
*                        0x02 - 0xFF Reserved for Future Use
*
* \return
*  cy_en_ble_api_result_t : Return value indicates whether the function succeeded or
* failed. Following are the possible error codes.
*
*  Errors codes                          | Description
*  ------------                          | -----------
*  CY_BLE_SUCCESS                        | On successful operation.
*  CY_BLE_ERROR_MEMORY_ALLOCATION_FAILED | If Memory allocation failed.
*  CY_BLE_ERROR_INSUFFICIENT_RESOURCES   | If Stack resources are unavailable.
*
******************************************************************************/
cy_en_ble_api_result_t Cy_BLE_SetAddressResolutionEnable
(
    uint8_t       param
);


/******************************************************************************
* Function Name: Cy_BLE_GetResolvingList
***************************************************************************//**
*
* This API is used to get resolving list. 
* This API is a blocking call. Hence no event will be generated.
*
*  \param param: Pointer to store the base address of resolving list.
*
* \return
*  cy_en_ble_api_result_t : Return value indicates whether the function succeeded or
* failed. Following are the possible error codes.
*
*  Error codes                           | Description
*  ------------                          | -----------
*  CY_BLE_SUCCESS                        | On successful operation.
*  CY_BLE_ERROR_INVALID_PARAMETER        | On specifying NULL for input parameter.
*  CY_BLE_ERROR_INVALID_OPERATION        | If privacy feature is not enabled.
*
* Note : The resolving list memory will be used by the stack. Hence the application 
*        should not modify the contents using the pointer provided by the stack.
******************************************************************************/
cy_en_ble_api_result_t Cy_BLE_GetResolvingList(cy_stc_ble_resolving_list_retention_t const **param);


/******************************************************************************
* Function Name: Cy_BLE_SetPrivacyMode
***************************************************************************//**
*
* This API is used to allow the Host to specify the privacy mode to be used for 
* a given entry on the Resolving List. 

* The effect of this setting is specified in [Vol 6] Part B, Section 4.7.
*
* When an entry on the Resolving List is removed, the mode associated with that
* entry shall also be removed.

* This is not a blocking function. 
* Successful operation will be informed through 'CY_BLE_EVT_SET_PRIVACY_MODE_COMPLETE'.

*
*  \param privacyModeInfo: Buffer that contains the information of peer address, peer address type,
                    and privacy mode.
*
* \return
*  cy_en_ble_api_result_t : Return value indicates whether the function succeeded or
* failed. Following are the possible error codes.
*
*  Error codes                           | Description
*  ------------                          | -----------
*  CY_BLE_SUCCESS                        | On successful operation.
*  CY_BLE_ERROR_INVALID_PARAMETER        | On specifying NULL for input parameter.
*  CY_BLE_ERROR_MEMORY_ALLOCATION_FAILED | Memory allocation failed.
*  CY_BLE_ERROR_INSUFFICIENT_RESOURCES   | Stack resources are unavailable.
*
******************************************************************************/

cy_en_ble_api_result_t Cy_BLE_SetPrivacyMode
(
    cy_stc_ble_privacy_mode_info_t *privacyModeInfo
);


/******************************************************************************
* Function Name: Cy_BLE_SetLeEventMask
***************************************************************************//**
*
* This API is equivalent of the LE_Set_Event_Mask HCI command
* and is used to control which LE events are generated by the HCI for the Host.
* The host will process these events and will send appropriate events to the application.
* If the bit in the hciLeEventMask is set to a one, then the event 
* associated with that bit will be enabled. The Host must
* deal with each event that is generated by an LE Controller. The event mask
* allows the application to control which events will be generated for the host.
*
* This is not a blocking function. 
* Successful operation will be informed through 'CY_BLE_EVT_LE_SET_EVENT_MASK_COMPLETE'.
*
* \param param: Pointer to the LE Mask of size 8 bytes.
*         Refer Core Spec, Vol2, Part E, 7.8.1 for further information.
*
* \return
* cy_en_ble_api_result_t : Return value indicates whether the function succeeded or
* failed. Following are the possible error codes.
*
*  Errors codes                          | Description
*  ------------                          | -----------
*  CY_BLE_SUCCESS                        | On successful operation.
*  CY_BLE_ERROR_INVALID_PARAMETER        | On specifying NULL as input parameter.
*  CY_BLE_ERROR_MEMORY_ALLOCATION_FAILED | If Memory allocation failed.
*  CY_BLE_ERROR_INSUFFICIENT_RESOURCES   | If Stack resources are unavailable.
******************************************************************************/
cy_en_ble_api_result_t Cy_BLE_SetLeEventMask
(
    const uint8_t * param
);


/******************************************************************************
* Function Name: Cy_BLE_GetChannelMap
***************************************************************************//**
* 
* This function reads the channel map for data channels. This
* classification persists until it is overwritten by a subsequent call to this
* function or the controller is reset. If this command is used, updates should
* be sent within 10 seconds of the BLE Host knowing that the channel
* classification has changed. The interval between two successive commands sent
* will be at least one second. This command will be used only when the local
* device supports the Master role. 
* *
* This is not a blocking function. Successful operation is informed through 'CY_BLE_EVT_GET_CHANNEL_MAP_COMPLETE'
*
*  \param param: Pointer to security information of the device of type 
*             cy_stc_ble_channel_map_info_t. 
*   param->channelMap: To be ignored. 
* \return
* cy_en_ble_api_result_t : Return value indicates whether the function succeeded or
* failed. Following are the possible error codes.
*
*  Errors codes                          | Description
*  ------------                          | -----------
*  CY_BLE_SUCCESS                        | On successful operation.
*  CY_BLE_ERROR_INVALID_PARAMETER        | On specifying NULL as input parameter.
*  CY_BLE_ERROR_NO_DEVICE_ENTITY         | If connection does not exists for corresponding 'bdHandle'.
*  CY_BLE_ERROR_MEMORY_ALLOCATION_FAILED | If Memory allocation failed.
*  CY_BLE_ERROR_INSUFFICIENT_RESOURCES   | If Stack resources are unavailable.
******************************************************************************/
cy_en_ble_api_result_t Cy_BLE_GetChannelMap
(
    cy_stc_ble_channel_map_info_t  * param
);

/******************************************************************************
* Function Name: Cy_BLE_SetHostChannelClassification
***************************************************************************//**
* 
*  This function sets channel classification for data channels. This
*  classification persists until it is overwritten by a subsequent call to this
*  function or the controller is reset. If this command is used, updates should
*  be sent within 10 seconds of the BLE Host knowing that the channel
*  classification has changed. The interval between two successive commands sent
*  will be at least one second. This command will be used only when the local
*  device supports the Master role. 
*  
*  For details, refer to Bluetooth core specification 4.1, Volume 2, part E,
*  section 7.8.19.
* 
*  This is a non blocking function. Successful operation is informed through 
*  event 'CY_BLE_EVT_SET_HOST_CHANNEL_COMPLETE'
* 
*  \param param: Pointer of type cy_stc_ble_channel_map_info_t. 
*   param->bdHandle: To be ignored.
*
* \return
*  cy_en_ble_api_result_t : Return value indicates whether the function succeeded or
*  failed. Following are the possible error codes.
*
*   Errors codes                          | Description
*   ------------                          | -----------
*   CY_BLE_SUCCESS                        | On successful operation.
*   CY_BLE_ERROR_INVALID_PARAMETER        | On specifying NULL as input parameter.
*   CY_BLE_ERROR_MEMORY_ALLOCATION_FAILED | If Memory allocation failed.
*   CY_BLE_ERROR_INSUFFICIENT_RESOURCES   | If Stack resources are unavailable.
******************************************************************************/
cy_en_ble_api_result_t Cy_BLE_SetHostChannelClassification
(
    cy_stc_ble_channel_map_info_t  * param
);

/******************************************************************************
* Function Name: Cy_BLE_AddDeviceToWhiteList
***************************************************************************//**
* 
*  This function adds the device to the Whitelist. The maximum number of devices that
*  can be added to the Whitelist is eight including CY_BLE_GAP_MAX_BONDED_DEVICE.
*  Refer to Bluetooth 4.1 core specification, Volume 3, Part C, section 9.3.5 for more details on whitelist.
* 
*  Completion of operation is informed through 'CY_BLE_EVT_ADD_DEVICE_TO_WHITE_LIST_COMPLETE' event.
*  Successful operation triggers 'CY_BLE_EVT_PENDING_FLASH_WRITE' event
*
*  \param param: Peer device address of type cy_stc_ble_bd_addr_t.
* 
* \return
*  cy_en_ble_api_result_t : Return value indicates whether the function succeeded or
*  failed. Following are the possible error codes.
*
*  Error codes                           | Description
*  ------------                          | -----------
*  CY_BLE_SUCCESS                        | On successful operation.
*  CY_BLE_ERROR_INVALID_PARAMETER        | On specifying NULL for 'param' or bdAddr 'type' has an invalid value.
*  CY_BLE_ERROR_MEMORY_ALLOCATION_FAILED | Memory allocation failed.
*  CY_BLE_ERROR_INSUFFICIENT_RESOURCES   | Stack resources are unavailable.
* 
******************************************************************************/
cy_en_ble_api_result_t Cy_BLE_AddDeviceToWhiteList
(
    cy_stc_ble_bd_addr_t  * param
);

/******************************************************************************
* Function Name: Cy_BLE_RemoveDeviceFromWhiteList
***************************************************************************//**
* 
*  This function removes the device from the Whitelist.
* 
*  Completion of operation is informed through the 'CY_BLE_EVT_REMOVE_DEVICE_FROM_WHITE_LIST_COMPLETE' event.
*  Completion operation triggers the 'CY_BLE_EVT_PENDING_FLASH_WRITE' event.
* 
*  \param param: Pointer to peer device address, of type cy_stc_ble_bd_addr_t. If the device
*           address is set to 0, then all devices shall be removed from the trusted Whitelist.
* 
* \return
*  cy_en_ble_api_result_t : Return value indicates whether the function succeeded or
*  failed. Following are the possible error codes.
*
*   Errors codes                          | Description
*   ------------                          | -----------
*   CY_BLE_SUCCESS                        | On successful operation.
*   CY_BLE_ERROR_INVALID_PARAMETER        | On specifying NULL as input parameter.
*   CY_BLE_ERROR_MEMORY_ALLOCATION_FAILED | If Memory allocation failed.
*   CY_BLE_ERROR_INSUFFICIENT_RESOURCES   | If Stack resources are unavailable.
* 
******************************************************************************/
cy_en_ble_api_result_t Cy_BLE_RemoveDeviceFromWhiteList
(
    cy_stc_ble_bd_addr_t  * param
);


/******************************************************************************
* Function Name: Cy_BLE_GetWhiteList
***************************************************************************//**
* 
*  This function is used to get the Whitelist.
*  This is a blocking call. Hence no event will be generated.
* 
*  \param param: Pointer to store the baseaddress of Whitelist.
* 
* \return
*  cy_en_ble_api_result_t : Return value indicates whether the function succeeded or
*  failed. Following are the possible error codes.
*
*   Errors codes                          | Description
*   ------------                          | -----------
*   CY_BLE_SUCCESS                        | On successful operation.
*   CY_BLE_ERROR_INVALID_PARAMETER        | On specifying NULL as input parameter.
*
* Note : The Whitelist memory will be used by the stack. Hence the application 
*        should not modify the contents using the pointer provided by the stack.
******************************************************************************/
cy_en_ble_api_result_t Cy_BLE_GetWhiteList(cy_stc_ble_white_list_retention_t const**param);


/******************************************************************************
* Function Name: Cy_BLE_isLLControlProcPending
***************************************************************************//**
*
* This function checks the Link Layer state for any pending real time
* control (LL_CHANNEL_MAP, LL_CONNECTION_UPDATE) procedure. When any such 
* procedure is pending in Link layer busy state, it is indicated by the Link Layer.
*
*  Completion operation triggers 'CY_BLE_EVT_LL_CNTRL_PROC_PENDING_COMPLETE' event
*
* Applications using specific GAP APIs or L2CAP API that can result
* in initiation of real-time procedures such as LL_CHANNEL_MAP or LL_CONNECTION_UPDATE
* can check the state of the Link Layer to avoid any such rejection from the BLE Stack.
*
* The BLE Stack can reject the new request 
*
* \param param: Peer device handle.
*
* \return
* cy_en_ble_api_result_t: Return value indicates the Link Layer status for any pending
* real time procedure.
*
*  Errors codes                          | Description
*  -------------                         | -----------
*  CY_BLE_SUCCESS                        | On successful operation.
*  CY_BLE_ERROR_NO_DEVICE_ENTITY         | Device identified by bdHandle is not present
*  CY_BLE_ERROR_MEMORY_ALLOCATION_FAILED | If Memory allocation failed.
*  CY_BLE_ERROR_INSUFFICIENT_RESOURCES   | If Stack resources are unavailable.
*
******************************************************************************/
cy_en_ble_api_result_t Cy_BLE_isLLControlProcPending
(
    uint8_t   param
);


/******************************************************************************
* Function Name: Cy_BLE_RegisterHciCb
***************************************************************************//**
* 
*  This API will initialize the HCI layer of the BLE Host Stack and will register Rx callback for all hci events and data. 
*  This API must be called before calling Cy_BLE_HciTransmit. 
*  This API should not be called during normal stack operation. 
*  If an application randomly uses hci APIs along with other APIs, behavior will be unpredictable.  
*
*  \param param: parameter is of type 'cy_ble_hci_rx_cb_t', callback function for Rx packets
* 
* \return
*  cy_en_ble_api_result_t : Return value indicates whether the function succeeded or
*  failed. Following are the possible error codes.
*
*   Errors codes                     | Description
*   ------------                     | -----------
*   CY_BLE_SUCCESS                   | On successful operation.
*   CY_BLE_ERROR_INVALID_PARAMETER   | On specifying NULL as input parameter.
*
******************************************************************************/
cy_en_ble_api_result_t Cy_BLE_RegisterHciCb
(
    cy_ble_hci_rx_cb_t     param
);


/******************************************************************************
* Function Name: Cy_BLE_HciShutdown
***************************************************************************//**
*  This function stops any ongoing operation on the BLE Stack and forces the BLE Host HCI and 
*  the BLE controller to shut down. The only functions that can be called after calling this
*  function is Cy_BLE_StackInit or Cy_BLE_RegisterHciCb. On calling one of these functions, the BLE Radio is turned 
*  off.
* 
*  No event is generated on calling this function.
* 
* \return
*  None
* 
******************************************************************************/
void Cy_BLE_HciShutdown
(
    void
);

/******************************************************************************
* Function Name: Cy_BLE_HciTransmit
***************************************************************************//**
* 
*  The application must use this API for sending packets. This API
*   must be called after calling 'Cy_BLE_RegisterHciCb'.
* 
*  \param param: parameter is of type 'cy_stc_ble_hci_tx_packet_info_t'
*
* \return
*  cy_en_ble_api_result_t : Return value indicates whether the function succeeded or
*  failed. Following are the possible error codes.
*
*   Errors codes                     | Description
*   ------------                     | -----------
*   CY_BLE_SUCCESS                   | On successful operation.
*   CY_BLE_ERROR_INVALID_PARAMETER   | On specifying NULL as input parameter.
*
******************************************************************************/
cy_en_ble_api_result_t Cy_BLE_HciTransmit
(
    cy_stc_ble_hci_tx_packet_info_t  * param
);


/******************************************************************************
* Function Name: Cy_BLE_SetPhy
***************************************************************************//**
*
* This API allows application to Sets the PHY for the current connection. 
* This is a non-blocking function and successful set to controller operation is 
* informed through 'CY_BLE_EVT_SET_PHY_COMPLETE'.
* Actual Phy used by controller will be informed through the 'CY_BLE_EVT_PHY_UPDATE_COMPLETE' event.
*
*  \param param: parameter is of type 'cy_stc_ble_set_phy_info_t'.
*
* \return
*  cy_en_ble_api_result_t : Return value indicates whether the function succeeded or
* failed. Following are the possible error codes.
*
*  Errors codes                          | Description
*  ------------                          | -----------
*  CY_BLE_SUCCESS                        | On successful operation.
*  CY_BLE_ERROR_NO_DEVICE_ENTITY         | Device identified by bdHandle is not present
*  CY_BLE_ERROR_INVALID_PARAMETER        | Null pointer passed.
*  CY_BLE_ERROR_MEMORY_ALLOCATION_FAILED | If Memory allocation failed.
*  CY_BLE_ERROR_INSUFFICIENT_RESOURCES   | If Stack resources are unavailable.
*
******************************************************************************/
cy_en_ble_api_result_t Cy_BLE_SetPhy
(
    const cy_stc_ble_set_phy_info_t  * param
);


/******************************************************************************
* Function Name: Cy_BLE_SetDefaultPhy
***************************************************************************//**
*
* This API allows the application to set a default PHY for all connections.
* This is a non-blocking function. Successful setting of the controller operation is 
* informed through 'CY_BLE_EVT_SET_DEFAULT_PHY_COMPLETE'.
* The actual Phy used by the controller will be informed through the 'CY_BLE_EVT_PHY_UPDATE_COMPLETE' event.
*
*  \param param: parameter is of type 'cy_stc_ble_set_suggested_phy_info_t'.
*
*  \return
*  cy_en_ble_api_result_t : Return value indicates whether the function succeeded or
*  failed. Following are the possible error codes.
*
*  Errors codes                          | Description
*  ------------                          | -----------
*  CY_BLE_SUCCESS                        | On successful operation.
*  CY_BLE_ERROR_INVALID_PARAMETER        | Null pointer.
*  CY_BLE_ERROR_MEMORY_ALLOCATION_FAILED | If Memory allocation failed.
*  CY_BLE_ERROR_INSUFFICIENT_RESOURCES   | If Stack resources are unavailable.
*
******************************************************************************/
cy_en_ble_api_result_t Cy_BLE_SetDefaultPhy
(
    const cy_stc_ble_set_suggested_phy_info_t  * param
);


/******************************************************************************
* Function Name: Cy_BLE_GetPhy
***************************************************************************//**
*
* This API allows the application to read the current PHY for the current connection.
*
* This is a non-blocking function. 
* Current Phy for Tx and Rx is informed through 'CY_BLE_EVT_GET_PHY_COMPLETE'
*
*  \param param: Peer bdHandle
*
*  \return
*  cy_en_ble_api_result_t : Return value indicates whether the function succeeded or
*  failed. Following are the possible error codes.
*
*  Errors codes                          | Description
*  ------------                          | -----------
*  CY_BLE_SUCCESS                        | On successful operation.
*  CY_BLE_ERROR_NO_DEVICE_ENTITY         | Device identified by bdHandle is not present.
*  CY_BLE_ERROR_MEMORY_ALLOCATION_FAILED | If Memory allocation failed.
*  CY_BLE_ERROR_INSUFFICIENT_RESOURCES   | If Stack resources are unavailable.
*
******************************************************************************/
cy_en_ble_api_result_t Cy_BLE_GetPhy
(
    uint8_t       param
);

/******************************************************************************
* Function Name: Cy_BLE_GetTemperature
***************************************************************************//**
*
* This API allows the application to read the current temperature from the temperature
* sensor in the BLE radio.
*
* This is a non-blocking function. 
* Temperature value is informed through 'CY_BLE_EVT_RADIO_TEMPERATURE' in degreeC units
*
*  \return
*  cy_en_ble_api_result_t : Return value indicates whether the function succeeded or
*  failed. Following are the possible error codes.
*
*  Errors codes                          | Description
*  ------------                          | -----------
*  CY_BLE_SUCCESS                        | On successful operation.
*  CY_BLE_ERROR_MEMORY_ALLOCATION_FAILED | If Memory allocation failed.
*  CY_BLE_ERROR_INSUFFICIENT_RESOURCES   | If Stack resources are unavailable.
*
******************************************************************************/
cy_en_ble_api_result_t Cy_BLE_GetTemperature
(
    void
);

/******************************************************************************
* Function Name: Cy_BLE_GetBatteryLevel
***************************************************************************//**
*
* This API allows the application to read the current supply voltage of the BLE radio.
*
* This is a non-blocking function. 
* Voltage value is informed through 'CY_BLE_EVT_RADIO_VOLTAGE_LEVEL' in milliVolts.
*
*  \return
*  cy_en_ble_api_result_t : Return value indicates whether the function succeeded or
*  failed. Following are the possible error codes.
*
*  Errors codes                          | Description
*  ------------                          | -----------
*  CY_BLE_SUCCESS                        | On successful operation.
*  CY_BLE_ERROR_MEMORY_ALLOCATION_FAILED | If Memory allocation failed.
*  CY_BLE_ERROR_INSUFFICIENT_RESOURCES   | If Stack resources are unavailable.
*
******************************************************************************/
cy_en_ble_api_result_t Cy_BLE_GetBatteryLevel
(
    void
);

#ifdef __cplusplus
}
#endif /* __cplusplus */
/** @} */

#endif /* CY_BLE_STACK_HOST_MAIN_H_ */


/*EOF*/
