/***************************************************************************//**
* \file cy_ble.h
* \version 2.0
*
* \brief
*  Contains the prototypes and constants used in the BLE Component.
*
********************************************************************************
* \copyright
* Copyright 2017, Cypress Semiconductor Corporation.  All rights reserved.
* You may use this file only in accordance with the license, terms, conditions,
* disclaimers, and limitations in the end user license agreement accompanying
* the software package with which this file was provided.
*******************************************************************************/


#ifndef CY_BLE_H
#define CY_BLE_H

#include <string.h>
#include "syslib/cy_syslib.h"
#include "sysint/cy_sysint.h"
#include "syspm/cy_syspm.h"
#if ((CY_BLE_CONFIG_STACK_MODE_HOST_UART) || (CY_BLE_CONFIG_STACK_MODE_CONTR_UART))
    #include "scb/Cy_SCB_uart.h"
#endif /* (CY_BLE_STACK_MODE_HOST_UART) */
#include "BLE_config.h"
#include <cy_device_headers.h>
#ifndef CY_IP_MXBLESS
    #error "The BLE middleware is not supported on this device"
#endif

/***************************************
* Stack includes
***************************************/

#include "cy_ble_stack_host_main.h"
#include "cy_ble_stack_gap.h"
#include "cy_ble_stack_gap_central.h"
#include "cy_ble_stack_gap_peripheral.h"
#include "cy_ble_stack_gatt_server.h"
#include "cy_ble_stack_gatt_client.h"
#include "cy_ble_stack_gatt_db.h"
#include "cy_ble_stack_l2cap.h"

/* The C binding of definitions if building with the C++ compiler */
#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */


/***************************************
* Type Definitions
***************************************/

/** The event callback function prototype to receive events from the BLE Component */
typedef void (* cy_ble_callback_t) (uint32_t eventCode, void *eventParam);

#define CY_BLE_M0S8BLESS_VERSION             (4u)

#define CY_BLE_STACK_MODE                    (CY_BLE_CONFIG_STACK_MODE)
#define CY_BLE_STACK_HOST_ONLY               (1u)
#define CY_BLE_STACK_DEBUG                   (2u)
#define CY_BLE_STACK_RELEASE                 (3u)
#define CY_BLE_STACK_HOST_CONTR_UART         (4u)
#define CY_BLE_STACK_HOST_IPC                (5u)

#define CY_BLE_STACK_MODE_HOST_UART          (CY_BLE_CONFIG_STACK_MODE_HOST_UART)
#define CY_BLE_STACK_MODE_IPC                (CY_BLE_STACK_MODE == CY_BLE_STACK_DEBUG)
#define CY_BLE_STACK_MODE_HOST_IPC           ((CY_BLE_MODE_PROFILE) &&                                        \
                                              (((CY_BLE_CONFIG_STACK_MODE == CY_BLE_CONFIG_STACK_HOST_IPC) || \
                                                (CY_BLE_CONFIG_STACK_MODE == CY_BLE_CONFIG_STACK_DEBUG)) &&   \
                                               CY_BLE_HOST_CORE))


#define CY_BLE_CONN_COUNT                    (CY_BLE_CONFIG_CONN_COUNT)
#define CY_BLE_GAPP_CONF_COUNT               (CY_BLE_CONFIG_GAP_PERIPHERAL_COUNT + CY_BLE_CONFIG_GAP_BROADCASTER_COUNT)
#define CY_BLE_GAPC_CONF_COUNT               (CY_BLE_CONFIG_GAP_CENTRAL_COUNT + CY_BLE_CONFIG_GAP_OBSERVER_COUNT)

#define CY_BLE_BONDING_YES                   (0x00u)
#define CY_BLE_BONDING_NO                    (0x01u)

#if (CY_BLE_MODE_PROFILE)
    #define CY_BLE_BONDING_REQUIREMENT       (CY_BLE_CONFIG_BONDING_REQUIREMENT)
#if (CY_BLE_BONDING_REQUIREMENT == CY_BLE_BONDING_YES)
    #define CY_BLE_MAX_BONDED_DEVICES        (CY_BLE_CONFIG_MAX_BONDED_DEVICES)
#else
    #define CY_BLE_MAX_BONDED_DEVICES        (0u)
#endif  /* (CY_BLE_BONDING_REQUIREMENT == CY_BLE_BONDING_YES) */
    #define CY_BLE_ENABLE_LL_PRIVACY         (CY_BLE_CONFIG_ENABLE_LL_PRIVACY)
#if (CY_BLE_ENABLE_LL_PRIVACY > 0u)
    #define CY_BLE_MAX_RESOLVABLE_DEVICES    (CY_BLE_CONFIG_MAX_RESOLVABLE_DEVICES)
#else
    #define CY_BLE_MAX_RESOLVABLE_DEVICES    (0u)
#endif  /* (CY_BLE_ENABLE_LL_PRIVACY > 0u) */
    #define CY_BLE_MAX_WHITE_LIST_SIZE       (CY_BLE_CONFIG_MAX_WHITE_LIST_SIZE)
    #define CY_BLE_GAP_SECURITY_LEVEL        (CY_BLE_CONFIG_GAP_SECURITY_LEVEL)
#else
    #define CY_BLE_MAX_BONDED_DEVICES        (CY_BLE_DEFAULT_BONDED_DEVICE_LIST_SZ)
    #define CY_BLE_MAX_RESOLVABLE_DEVICES    (CY_BLE_DEFAULT_RPA_LIST_SZ)
    #define CY_BLE_MAX_WHITE_LIST_SIZE       (CY_BLE_MAX_WHITELIST_LIST_SZ)
    #define CY_BLE_ENABLE_LL_PRIVACY         (1u)
    #define CY_BLE_GAP_SECURITY_LEVEL        (0x0u)
#endif /* CY_BLE_MODE_PROFILE */

#define CY_BLE_SHARING_NONE                  (0u)
#define CY_BLE_SHARING_EXPORT                (1u)
#define CY_BLE_SHARING_IMPORT                (2u)
#define CY_BLE_SHARING_MODE                  (CY_BLE_CONFIG_SHARING_MODE)
#define CY_BLE_SHARING_MODE_EXPORT           (CY_BLE_SHARING_MODE == CY_BLE_SHARING_EXPORT)
#define CY_BLE_SHARING_MODE_IMPORT           (CY_BLE_SHARING_MODE == CY_BLE_SHARING_IMPORT)

/* Align the buffer size value to 4 */
#define CY_BLE_ALIGN_TO_4(x)    ((((x) & 3u) == 0u) ? (x) : (((x) - ((x) & 3u)) + 4u))

#if (CY_BLE_MODE_PROFILE)

#define CY_BLE_GAP_ROLE                       (CY_BLE_CONFIG_GAP_ROLE)
#define CY_BLE_GAP_HCI                        (0x00u)
#define CY_BLE_GAP_PERIPHERAL                 (0x01u)
#define CY_BLE_GAP_CENTRAL                    (0x02u)
#define CY_BLE_GAP_BOTH                       (CY_BLE_GAP_CENTRAL | CY_BLE_GAP_PERIPHERAL)
#define CY_BLE_GAP_BROADCASTER                (0x04u)
#define CY_BLE_GAP_OBSERVER                   (0x08u)

#define CY_BLE_GAP_ROLE_PERIPHERAL            (0u != (CY_BLE_GAP_ROLE & CY_BLE_GAP_PERIPHERAL))
#define CY_BLE_GAP_ROLE_CENTRAL               (0u != (CY_BLE_GAP_ROLE & CY_BLE_GAP_CENTRAL))
#define CY_BLE_GAP_ROLE_OBSERVER              (0u != (CY_BLE_GAP_ROLE & CY_BLE_GAP_OBSERVER))
#define CY_BLE_GAP_ROLE_BROADCASTER           (0u != (CY_BLE_GAP_ROLE & CY_BLE_GAP_BROADCASTER))

#define CY_BLE_L2CAP_Q_DEPTH_PER_CONN         (CY_BLE_L2CAP_STACK_Q_DEPTH_PER_CONN + CY_BLE_CONFIG_ADD_Q_DEPTH_PER_CONN)
#define CY_BLE_L2CAP_Q_HEAP                   CY_BLE_ALIGN_TO_4(CY_BLE_L2CAP_Q_DEPTH_PER_CONN * \
                                                                CY_BLE_L2CAP_QUEUE_ELEMENT_SIZE * CY_BLE_CONN_COUNT)

/* If mtu > 32 stack queue depth = (stack queue depth per connection + 2) * maxBleConnection */
#define CY_BLE_GATT_MTU_BUFF_COUNT            ((CY_BLE_L2CAP_Q_DEPTH_PER_CONN + ((CY_BLE_CONFIG_GATT_MTU >                  \
                                                                                  CY_BLE_L2CAP_MTU_MIN_VALUE) ? 2u : 0u)) * \
                                               CY_BLE_CONN_COUNT)

/* GATT MTU Size */
#define CY_BLE_GATT_MTU                       (CY_BLE_CONFIG_GATT_MTU)
#define CY_BLE_GATT_MTU_PLUS_L2CAP_MEM_EXT    (CY_BLE_ALIGN_TO_4(CY_BLE_GATT_MTU + CY_BLE_MEM_EXT_SZ + \
                                                                 CY_BLE_L2CAP_HDR_SZ))

/* GATT Maximum attribute length */
#define CY_BLE_GATT_MAX_ATTR_LEN              ((CY_BLE_CONFIG_GATT_DB_MAX_VALUE_LEN == 0u) ? (1u) : \
                                               (CY_BLE_CONFIG_GATT_DB_MAX_VALUE_LEN))
#define CY_BLE_GATT_MAX_ATTR_LEN_PLUS_L2CAP_MEM_EXT                   \
    (CY_BLE_ALIGN_TO_4(CY_BLE_GATT_MAX_ATTR_LEN + CY_BLE_MEM_EXT_SZ + \
                       CY_BLE_L2CAP_HDR_SZ))

/* The header length for Prepare Write request */
#define CY_BLE_GATT_PREPARE_WRITE_HEADER_LEN    (5u)
/* The header length for Write request */
#define CY_BLE_GATT_WRITE_HEADER_LEN            (3u)

/* The number of characteristics supporting a Reliable Write property */
#define CY_BLE_GATT_RELIABLE_CHAR_COUNT         (CY_BLE_CONFIG_GATT_RELIABLE_CHAR_COUNT)
/* The total length of characteristics with a Reliable Write property */
#define CY_BLE_GATT_RELIABLE_CHAR_LENGTH        (CY_BLE_CONFIG_GATT_RELIABLE_CHAR_LENGTH)

#define CY_BLE_GATT_PREPARE_LENGTH              ((CY_BLE_GATT_RELIABLE_CHAR_LENGTH > CY_BLE_GATT_MAX_ATTR_LEN) ? \
                                                 CY_BLE_GATT_RELIABLE_CHAR_LENGTH : CY_BLE_GATT_MAX_ATTR_LEN)

/* The number of buffers required for Prepare Write request based on assumption that the negotiated MTU
 *  size is equal to the CY_BLE_GATT_DEFAULT_MTU and all the characteristics supporting a Reliable Write
 *  property must be written, in order, in a single operation.
 *  The buffer count is 0 when the maximum attribute size is less than the minimum MTU - 3.
 */
#define CY_BLE_GATT_MAX_PREPARE_BUFF_COUNT                                                                      \
    (((CY_BLE_GATT_MAX_ATTR_LEN <= (CY_BLE_GATT_DEFAULT_MTU - CY_BLE_GATT_WRITE_HEADER_LEN)) &&                 \
      (CY_BLE_GATT_RELIABLE_CHAR_COUNT == 0u)) ? 0u :                                                           \
     ((CY_BLE_GATT_PREPARE_LENGTH / (CY_BLE_GATT_DEFAULT_MTU - CY_BLE_GATT_PREPARE_WRITE_HEADER_LEN)) +         \
      (((CY_BLE_GATT_PREPARE_LENGTH % (CY_BLE_GATT_DEFAULT_MTU - CY_BLE_GATT_PREPARE_WRITE_HEADER_LEN)) > 0u) ? \
       1u : 0u)))

#define CY_BLE_GATT_PREPARE_LENGTH_ALIGN               ((CY_BLE_GATT_MAX_PREPARE_BUFF_COUNT == 0u) ? 0u : \
                                                        CY_BLE_ALIGN_TO_4(CY_BLE_GATT_PREPARE_LENGTH))

/* The parameter to enable the application to provide a dynamically allocated buffer for Prepare Write request.  */
#define CY_BLE_GATT_ENABLE_EXTERNAL_PREP_WRITE_BUFF    (CY_BLE_CONFIG_GATT_ENABLE_EXTERNAL_PREP_WRITE_BUFF)

/* Buffer length for the data received during Prepare Write requests
 * For dynamic memory allocation by the application level, set the EnableExternalPrepWriteBuff parameter
 * in the Expression view of the Advanced tab to true.
 */
#define CY_BLE_GATT_PREPARE_WRITE_BUFF_LEN             ((CY_BLE_GATT_ENABLE_EXTERNAL_PREP_WRITE_BUFF != 0u) ? 0u : \
                                                        (CY_BLE_GATT_PREPARE_LENGTH_ALIGN +                        \
                                                         (CY_BLE_GATT_MAX_PREPARE_BUFF_COUNT *                     \
                                                          sizeof(cy_stc_ble_gatt_handle_value_offset_param_t))))

#define CY_BLE_L2CAP_ENABLE                            (CY_BLE_CONFIG_L2CAP_ENABLE)

#if (CY_BLE_L2CAP_ENABLE != 0u)
/* L2CAP MTU Size */
    #define CY_BLE_L2CAP_MTU                      (CY_BLE_CONFIG_L2CAP_MTU)
/* L2CAP PMS Size */
    #define CY_BLE_L2CAP_MPS                      (CY_BLE_CONFIG_L2CAP_MPS)
    #define CY_BLE_L2CAP_MTU_MPS                  (CY_BLE_L2CAP_MTU / CY_BLE_L2CAP_MPS)
/* Number of L2CAP Logical channels */
    #define CY_BLE_L2CAP_LOGICAL_CHANNEL_COUNT    (CY_BLE_CONFIG_L2CAP_LOGICAL_CHANNEL_COUNT)
/* Number of L2CAP PSMs */
    #define CY_BLE_L2CAP_PSM_COUNT                (CY_BLE_CONFIG_L2CAP_PSM_COUNT)
#else
/* L2CAP MTU Size */
    #define CY_BLE_L2CAP_MTU                      (0u)
/* L2CAP PMS Size */
    #define CY_BLE_L2CAP_MPS                      (0u)
    #define CY_BLE_L2CAP_MTU_MPS                  (0u)
/* Number of L2CAP Logical channels */
    #define CY_BLE_L2CAP_LOGICAL_CHANNEL_COUNT    (0u)
/* Number of L2CAP PSMs */
    #define CY_BLE_L2CAP_PSM_COUNT                (0u)
#endif  /* CY_BLE_L2CAP_ENABLE != 0u */

/* L2CAP RAM buffer sizes */
#define CY_BLE_L2CAP_MTU_PLUS_L2CAP_MEM_EXT     (CY_BLE_ALIGN_TO_4(CY_BLE_L2CAP_MTU + \
                                                                   CY_BLE_MEM_EXT_SZ + CY_BLE_L2CAP_HDR_SZ))
#define CY_BLE_L2CAP_MPS_PLUS_L2CAP_MEM_EXT     (CY_BLE_ALIGN_TO_4(CY_BLE_L2CAP_MPS + CY_BLE_MEM_EXT_SZ + \
                                                                   CY_BLE_L2CAP_HDR_SZ))
#define CY_BLE_L2CAP_PSM_PLUS_L2CAP_MEM_EXT     (CY_BLE_ALIGN_TO_4((CY_BLE_L2CAP_PSM_SIZE + CY_BLE_MEM_EXT_SZ) * \
                                                                   CY_BLE_L2CAP_PSM_COUNT))
#define CY_BLE_L2CAP_CBFC_PLUS_L2CAP_MEM_EXT    (CY_BLE_ALIGN_TO_4((CY_BLE_L2CAP_CBFC_CHANNEL_SIZE + \
                                                                    CY_BLE_MEM_EXT_SZ) * CY_BLE_L2CAP_LOGICAL_CHANNEL_COUNT))

#endif /* CY_BLE_MODE_PROFILE */

/* LL max data length */
#define CY_BLE_LL_MAX_SUPPORTED_TX_PAYLOAD_SIZE    (251u)
#define CY_BLE_LL_MAX_SUPPORTED_RX_PAYLOAD_SIZE    (251u)
#define CY_BLE_LL_MIN_SUPPORTED_TX_PAYLOAD_SIZE    (27u)
#define CY_BLE_LL_MIN_SUPPORTED_RX_PAYLOAD_SIZE    (27u)
#define CY_BLE_LL_MAX_TX_TIME                      (0x0848u)

#if (CY_BLE_MODE_PROFILE)
    #define CY_BLE_LL_MAX_TX_PAYLOAD_SIZE          (CY_BLE_CONFIG_LL_MAX_TX_PAYLOAD_SIZE)
    #define CY_BLE_LL_MAX_RX_PAYLOAD_SIZE          (CY_BLE_CONFIG_LL_MAX_RX_PAYLOAD_SIZE)
#else   /* Use maximum payload size for HCI mode */
    #define CY_BLE_LL_MAX_TX_PAYLOAD_SIZE          (CY_BLE_LL_MAX_SUPPORTED_TX_PAYLOAD_SIZE)
    #define CY_BLE_LL_MAX_RX_PAYLOAD_SIZE          (CY_BLE_LL_MAX_SUPPORTED_RX_PAYLOAD_SIZE)
#endif /* CY_BLE_MODE_PROFILE */

#define CY_BLE_LL_MAX_TX_PAYLOAD_BUFFER_SZ         CY_BLE_ALIGN_TO_4(CY_BLE_LL_MAX_TX_PAYLOAD_SIZE)
#define CY_BLE_LL_MAX_RX_PAYLOAD_BUFFER_SZ         CY_BLE_ALIGN_TO_4(CY_BLE_LL_MAX_RX_PAYLOAD_SIZE)

#if ((CY_BLE_LL_MAX_TX_PAYLOAD_SIZE > CY_BLE_LL_MIN_SUPPORTED_TX_PAYLOAD_SIZE) || \
    (CY_BLE_LL_MAX_RX_PAYLOAD_SIZE > CY_BLE_LL_MIN_SUPPORTED_RX_PAYLOAD_SIZE))
    #define CY_BLE_DLE_FEATURE                     (CY_BLE_DLE_FEATURE_MASK)
    #define CY_BLE_LL_DLE_HEAP_SZ                  (CY_BLE_LL_DLE_HEAP_REQ)
#else
    #define CY_BLE_DLE_FEATURE                     (0u)
    #define CY_BLE_LL_DLE_HEAP_SZ                  (0u)
#endif /* DLE enabled */

#if (CY_BLE_ENABLE_LL_PRIVACY > 0u)
    #define CY_BLE_LL_PRIVACY_FEATURE              (CY_BLE_PRIVACY_1_2_FEATURE_MASK)
    #define CY_BLE_LL_PRIVACY_HEAP_SZ              (CY_BLE_LL_PRIVACY_HEAP_REQ)
    #define CY_BLE_STORE_RESOLVING_LIST_FEATURE    (CY_BLE_PERSISTENT_STORE_RESOLVING_LIST)
/* Resolving list memory computation */
    #define CY_BLE_STACK_RESOLVING_LIST_SIZE       ((CY_BLE_LL_PRIVACY_HEAP_SZ * CY_BLE_MAX_RESOLVABLE_DEVICES) + \
                                                    CY_BLE_LL_PRIVACY_RETENTION_HEAP_REQ)
#else
    #define CY_BLE_LL_PRIVACY_FEATURE              (0u)
    #define CY_BLE_LL_PRIVACY_HEAP_SZ              (0u)
    #define CY_BLE_STORE_RESOLVING_LIST_FEATURE    (0u)
    #define CY_BLE_STACK_RESOLVING_LIST_SIZE       (0u)
#endif /* CY_BLE_MAX_RESOLVABLE_DEVICES > 0u */

#if (CY_BLE_GAP_SECURITY_LEVEL == 0x03)
    #define CY_BLE_SECURE_CONN_FEATURE            (CY_BLE_SECURE_CONN_FEATURE_MASK)
    #define CY_BLE_RAM_SECURE_CONNECTIONS_SIZE    (CY_BLE_RAM_SIZE_SECURE_CONNECTIONS)
#else
    #define CY_BLE_SECURE_CONN_FEATURE            (0u)
    #define CY_BLE_RAM_SECURE_CONNECTIONS_SIZE    (0u)
#endif /* CY_BLE_GAP_SECURITY_LEVEL == 0x03 */

#if ((CY_BLE_CONFIG_ENABLE_PHY_UPDATE != 0u) || (CY_BLE_MODE_HCI))
    #define CY_BLE_PHY_UPDATE_FEATURE    (CY_BLE_PHY_UPDATE_FEATURE_MASK)
#else
    #define CY_BLE_PHY_UPDATE_FEATURE    (0u)
#endif  /* (CY_BLE_CONFIG_ENABLE_PHY_UPDATE != 0u) || (CY_BLE_MODE_HCI) */

/** Radio power calibration */
#if (CY_BLE_CONFIG_TX_POWER_CALIBRATION_ENABLE != 0u)
    #define CY_BLE_CONFIG_TX_POWER_CALIBRATION_FEATURE    (CY_BLE_PERSISTENT_RADIO_CALIBRATION_MASK)
#else
    #define CY_BLE_CONFIG_TX_POWER_CALIBRATION_FEATURE    (0u)           
#endif  /* CY_BLE_CONFIG_TX_POWER CALIBRATION != 0u */

#if ((CY_BLE_MODE_PROFILE) && (CY_BLE_BONDING_REQUIREMENT == CY_BLE_BONDING_YES))
    #define CY_BLE_STORE_BONDLIST_FEATURE     (CY_BLE_PERSISTENT_STORE_BONDLIST)
/* Bonded device list flash memory computation */
    #define CY_BLE_STACK_BOND_FLASH_SIZE      ((CY_BLE_STACK_ONE_BONDED_DEVICE_SIZE * CY_BLE_MAX_BONDED_DEVICES) + \
                                               (CY_BLE_STACK_ONE_DEVICE_Q_ENTITY_SIZE * (CY_BLE_CONN_COUNT +       \
                                                                                         CY_BLE_MAX_BONDED_DEVICES)))
/* Bonded device list RAM memory computation */
    #define CY_BLE_STACK_BOND_RAM_SIZE        ((CY_BLE_STACK_ONE_BONDED_DEVICE_SIZE +    \
                                                CY_BLE_STACK_ONE_DEVICE_Q_ENTITY_SIZE) * \
                                               (CY_BLE_CONN_COUNT + CY_BLE_MAX_BONDED_DEVICES))
#else
    #define CY_BLE_STORE_BONDLIST_FEATURE     (0u)
    #define CY_BLE_STACK_BOND_FLASH_SIZE      (0u)
    #define CY_BLE_STACK_BOND_RAM_SIZE        (0u)
#endif  /* (CY_BLE_MODE_PROFILE) && (CY_BLE_BONDING_REQUIREMENT == CY_BLE_BONDING_YES) */

#define CY_BLE_STORE_WHITELIST_FEATURE        (CY_BLE_PERSISTENT_STORE_WHITELIST)
/* Whitelist list memory computation */
#define CY_BLE_STACK_WHITELIST_SIZE           ((CY_BLE_LL_ONE_WHITELIST_HEAP_REQ * CY_BLE_MAX_WHITE_LIST_SIZE) + \
                                               CY_BLE_LL_WHITELIST_RETENTION_HEAP_REQ)

#define CY_BLE_DLE_FEATURE_ENABLED            (CY_BLE_DLE_FEATURE != 0u)
#define CY_BLE_LL_PRIVACY_FEATURE_ENABLED     (CY_BLE_LL_PRIVACY_FEATURE != 0u)
#define CY_BLE_SECURE_CONN_FEATURE_ENABLED    (CY_BLE_SECURE_CONN_FEATURE != 0u)
#define CY_BLE_PHY_UPDATE_FEATURE_ENABLED     (CY_BLE_PHY_UPDATE_FEATURE != 0u)

#define CY_BLE_LL_ACL_TX_HEAP_SZ              (CY_BLE_LL_DEFAULT_NUM_ACL_TX_PACKETS *   \
                                               (CY_BLE_LL_MAX_TX_PAYLOAD_BUFFER_SZ +    \
                                                CY_BLE_LL_ACL_DATA_PACKET_OVERHEAD_SZ + \
                                                CY_BLE_MEM_EXT_SZ))

#define CY_BLE_LL_ACL_RX_HEAP_SZ              (CY_BLE_LL_DEFAULT_NUM_ACL_RX_PACKETS *   \
                                               (CY_BLE_LL_MAX_RX_PAYLOAD_BUFFER_SZ +    \
                                                CY_BLE_LL_ACL_DATA_PACKET_OVERHEAD_SZ + \
                                                CY_BLE_MEM_EXT_SZ))


#define CY_BLE_LL_CONTROLLER_HEAP_REQ         ((CY_BLE_DLE_DEVICE_PARAM_SZ) +                     \
                                               (CY_BLE_LL_DLE_HEAP_REQ * CY_BLE_CONN_COUNT) +     \
                                               ((CY_BLE_CONTROLLER_CONN_SPECIFIC_HEAP_REQ +       \
                                                 CY_BLE_LL_ACL_TX_HEAP_SZ) * CY_BLE_CONN_COUNT) + \
                                               CY_BLE_CONTROLLER_DEVICE_SPECIFIC_HEAP_REQ +       \
                                               CY_BLE_LL_ACL_RX_HEAP_SZ +                         \
                                               CY_BLE_STACK_RESOLVING_LIST_SIZE +                 \
                                               CY_BLE_STACK_WHITELIST_SIZE)

/** CRC size for stack flash*/
#define CY_BLE_STACK_FLASH_CRC_SIZE (2u)

#if (CY_BLE_MODE_PROFILE)
/**
 * \addtogroup group_ble_common_api_macros
 * @{
 */

/** The size of RAM memory required for the stack */
    #define CY_BLE_STACK_RAM_SIZE    (CY_BLE_ALIGN_TO_4(CY_BLE_DEFAULT_HOST_RAM_SIZE + CY_BLE_LL_CONTROLLER_HEAP_REQ +      \
                                                        CY_BLE_RAM_SECURE_CONNECTIONS_SIZE + CY_BLE_L2CAP_Q_HEAP +          \
                                                        CY_BLE_STACK_BOND_RAM_SIZE +                                        \
                                                        (CY_BLE_GATT_MTU_PLUS_L2CAP_MEM_EXT * CY_BLE_GATT_MTU_BUFF_COUNT) + \
                                                        (CY_BLE_L2CAP_PSM_PLUS_L2CAP_MEM_EXT * CY_BLE_L2CAP_PSM_COUNT) +    \
                                                        (CY_BLE_L2CAP_CBFC_PLUS_L2CAP_MEM_EXT * 2u *                        \
                                                         CY_BLE_L2CAP_LOGICAL_CHANNEL_COUNT) +                              \
                                                        (CY_BLE_L2CAP_MTU_PLUS_L2CAP_MEM_EXT * 2u *                         \
                                                         CY_BLE_L2CAP_LOGICAL_CHANNEL_COUNT) +                              \
                                                        (CY_BLE_STACK_BUFFER_MGR_UTIL_RAM_SZ * CY_BLE_STACK_APP_MIN_POOL) + \
                                                        (CY_BLE_RAM_SIZE_HOST_SINGLE_CONN * CY_BLE_CONN_COUNT) +  \
                                                        (CY_BLE_GATT_PREPARE_WRITE_BUFF_LEN))) /* This buffer must always be the latest */
#else
/** The size of RAM memory required for the stack */
    #define CY_BLE_STACK_RAM_SIZE    (CY_BLE_LL_CONTROLLER_HEAP_REQ)
#endif /* CY_BLE_MODE_PROFILE */


/** The size of flash memory required for the bonding data */
#define CY_BLE_STACK_FLASH_SIZE     (CY_BLE_STACK_BOND_FLASH_SIZE +     \
                                     CY_BLE_STACK_RESOLVING_LIST_SIZE + \
                                     CY_BLE_STACK_WHITELIST_SIZE +      \
                                     CY_BLE_STACK_FLASH_CRC_SIZE )

/** @} group_ble_common_api_macros */

/***************************************
* Private Function Prototypes
***************************************/
cy_en_syspm_status_t Cy_BLE_DeepSleepCallback(cy_stc_syspm_callback_params_t *callbackParams);
cy_en_syspm_status_t Cy_BLE_SleepCallback(cy_stc_syspm_callback_params_t *callbackParams);

/***************************************
* Function Prototypes
***************************************/
/**
 * \addtogroup group_ble_common_api_functions
 * @{
 */
cy_en_ble_api_result_t Cy_BLE_Enable(void);
cy_en_ble_api_result_t Cy_BLE_Disable(void);
void Cy_BLE_ConfigureExtPA(uint32_t controlValue);
/** @} */


/***************************************
* External data references
***************************************/
void Cy_BLE_UnregisterHostPMCallbacks(void);
extern cy_ble_callback_t Cy_BLE_ApplCallback;
extern uint8_t cy_ble_initVar;
#if (CY_BLE_SHARING_MODE_EXPORT)
extern uint8_t *cy_ble_stackMemoryRam;
#else
extern uint8_t cy_ble_stackMemoryRam[CY_BLE_STACK_RAM_SIZE];
#endif  /* CY_BLE_SHARING_MODE_EXPORT */

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* CY_BLE_H */


/* [] END OF FILE */
