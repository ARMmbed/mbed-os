/* 
 * Copyright (c) 2013 Nordic Semiconductor ASA
 * All rights reserved.
 * 
 * Redistribution and use in source and binary forms, with or without modification,
 * are permitted provided that the following conditions are met:
 * 
 *   1. Redistributions of source code must retain the above copyright notice, this list 
 *      of conditions and the following disclaimer.
 *
 *   2. Redistributions in binary form, except as embedded into a Nordic Semiconductor ASA 
 *      integrated circuit in a product or a software update for such product, must reproduce 
 *      the above copyright notice, this list of conditions and the following disclaimer in 
 *      the documentation and/or other materials provided with the distribution.
 *
 *   3. Neither the name of Nordic Semiconductor ASA nor the names of its contributors may be 
 *      used to endorse or promote products derived from this software without specific prior 
 *      written permission.
 *
 *   4. This software, with or without modification, must only be used with a 
 *      Nordic Semiconductor ASA integrated circuit.
 *
 *   5. Any software provided in binary or object form under this license must not be reverse 
 *      engineered, decompiled, modified and/or disassembled. 
 * 
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR
 * ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON
 * ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 * 
 */


#include "device_manager.h"
#include "app_trace.h"
#include "pstorage.h"
#include "nrf_ble_hci.h"
#include "app_error.h"

#if defined ( __CC_ARM )
    #ifndef __ALIGN
        #define __ALIGN(x)      __align(x)                  /**< Forced aligment keyword for ARM Compiler */
    #endif
#elif defined ( __ICCARM__ )
    #ifndef __ALIGN
        #define __ALIGN(x)                                  /**< Forced aligment keyword for IAR Compiler */
    #endif
#elif defined   ( __GNUC__ )
    #ifndef __ALIGN
        #define __ALIGN(x)      __attribute__((aligned(x))) /**< Forced aligment keyword for GNU Compiler */
    #endif
#endif

#define INVALID_ADDR_TYPE 0xFF   /**< Identifier for an invalid address type. */
#define EDIV_INIT_VAL     0xFFFF /**< Initial value for diversifier. */

/**
 * @defgroup device_manager_app_states Connection Manager Application States
 * @{
 */
#define STATE_CONTROL_PROCEDURE_IN_PROGRESS 0x01 /**< State where a security procedure is ongoing. */
#define STATE_QUEUED_CONTROL_REQUEST        0x02 /**< State where it is known if there is any queued security request or not. */
/** @} */

/**
 * @defgroup device_manager_conn_inst_states Connection Manager Connection Instances States.
 * @{
 */
#define STATE_IDLE             0x01 /**< State where connection instance is free. */
#define STATE_CONNECTED        0x02 /**< State where connection is successfully established. */
#define STATE_PAIRING          0x04 /**< State where pairing procedure is in progress. This state is used for pairing and bonding, as pairing is needed for both. */
#define STATE_BONDED           0x08 /**< State where device is bonded. */
#define STATE_DISCONNECTING    0x10 /**< State where disconnection is in progress, application will be notified first, but no further active procedures on the link. */
#define STATE_PAIRING_PENDING  0x20 /**< State where pairing request is pending on the link. */
#define STATE_BOND_INFO_UPDATE 0x40 /**< State where information has been updated, update the flash. */
#define STATE_LINK_ENCRYPTED   0x80 /**< State where link is encrypted. */
/** @} */

/**
 * @defgroup device_manager_peer_id_defines Peer Identification Information Defines.
 *
 * @brief These defines are used to know which of the peer identification is applicable for a peer.
 *
 * @details These defines are used for peer identification. Here, bit map is used because it is
 *          possible that the application has both IRK and address for identification.
 * @{
 */
#define UNASSIGNED            0xFF /**< Peer instance is unassigned/unused. */
#define IRK_ENTRY             0x01 /**< Peer instance has IRK as identification information. */
#define ADDR_ENTRY            0x02 /**< Peer instance has address as identification information. */
#define SERVICE_CONTEXT_ENTRY 0x04 /**< Peer instance has service context set. */
#define APP_CONTEXT_ENTRY     0x08 /**< Peer instance has an application context set. */
/** @} */

/**@brief Device store state identifiers. */
typedef enum
{
    STORE_ALL_CONTEXT, /**< Store all context. */
    FIRST_BOND_STORE,  /**< Store bond. */
    UPDATE_PEER_ADDR   /**< Update peer address. */
} device_store_state_t;

/**
 * @defgroup device_manager_context_offsets Context Offsets
 * @{
 *
 * @brief Context offsets each of the context information in persistent memory.
 *
 * @details Below is a layout showing how each how the context information is stored in persistent
 *          memory.
 *
 * All Device context is stored in the flash as follows:
 * +---------+---------+---------+------------------+----------------+--------------------+
 * | Block / Device ID + Layout of stored information in storage block                    |
 * +---------+---------+---------+------------------+----------------+--------------------+
 * | Block 0 | Device 0| Peer Id | Bond Information | Service Context| Application Context|
 * +---------+---------+---------+------------------+----------------+--------------------+
 * | Block 1 | Device 1| Peer Id | Bond Information | Service Context| Application Context|
 * +---------+---------+---------+------------------+----------------+--------------------+
 * |  ...              | ....                                                             |
 * +---------+---------+---------+------------------+----------------+--------------------+
 * | Block N | Device N| Peer Id | Bond Information | Service Context| Application Context|
 * +---------+---------+---------+------------------+----------------+--------------------+
 *
 * The following defines are used to get offset of each of the components within a block.
 */

#define PEER_ID_STORAGE_OFFSET     0                                               /**< Offset at which peer id is stored in the block. */
#define BOND_STORAGE_OFFSET        PEER_ID_SIZE                                    /**< Offset at which bond information is stored in the block. */
#define SERVICE_STORAGE_OFFSET     (BOND_STORAGE_OFFSET + BOND_SIZE)               /**< Offset at which service context is stored in the block. */
#define APP_CONTEXT_STORAGE_OFFSET (SERVICE_STORAGE_OFFSET + SERVICE_CONTEXT_SIZE) /**< Offset at which application context is stored in the block. */
/** @} */

/**
 * @defgroup device_manager_context_size Context size.
 * @{
 *
 * @brief This group defines the size of each of the context information.
 */
#define PEER_ID_SIZE               (sizeof(peer_id_t))                                             /**< Size of peer identification information. */
#define BOND_SIZE                  (sizeof(bond_context_t))                                        /**< Size of bond information. */
#define DEVICE_CONTEXT_SIZE        (PEER_ID_SIZE + BOND_SIZE)                                      /**< Size of Device context, include peer identification and bond information. */
#define GATTS_SERVICE_CONTEXT_SIZE (sizeof(dm_gatts_context_t))                                    /**< Size of GATTS service context. */
#define GATTC_SERVICE_CONTEXT_SIZE (sizeof(dm_gatt_client_context_t))                              /**< Size of GATTC service context. */
#define SERVICE_CONTEXT_SIZE       (GATTS_SERVICE_CONTEXT_SIZE + GATTC_SERVICE_CONTEXT_SIZE)       /**< Combined size of GATTS and GATTC service contexts. */
#define APP_CONTEXT_MIN_SIZE       4                                                               /**< Minimum size for application context data. */
#if (DEVICE_MANAGER_APP_CONTEXT_SIZE != 0)
#define APP_CONTEXT_SIZE           (sizeof(uint32_t) + DEVICE_MANAGER_APP_CONTEXT_SIZE)            /**< Size of application context including length field. */
#else //DEVICE_MANAGER_APP_CONTEXT_SIZE
#define APP_CONTEXT_SIZE           0                                                               /**< Size of application context. */
#endif // DEVICE_MANAGER_APP_CONTEXT_SIZE
#define ALL_CONTEXT_SIZE           (DEVICE_CONTEXT_SIZE + SERVICE_CONTEXT_SIZE + APP_CONTEXT_SIZE) /**< Size of all contexts. */
/** @} */


/**
 * @defgroup device_manager_log Module's Log Macros
 *
 * @details Macros used for creating module logs which can be useful in understanding handling
 *          of events or actions on API requests. These are intended for debugging purposes and
 *          can be disabled by defining the DM_DISABLE_LOGS.
 *
 * @note That if ENABLE_DEBUG_LOG_SUPPORT is disabled, having DM_DISABLE_LOGS has no effect.
 * @{
 */
#define nDM_DISABLE_LOGS        /**< Enable this macro to disable any logs from this module. */

#ifndef DM_DISABLE_LOGS
#define DM_LOG  app_trace_log  /**< Used for logging details. */
#define DM_ERR  app_trace_log  /**< Used for logging errors in the module. */
#define DM_TRC  app_trace_log  /**< Used for getting trace of execution in the module. */
#define DM_DUMP app_trace_dump /**< Used for dumping octet information to get details of bond information etc. */
#else //DM_DISABLE_LOGS
#define DM_DUMP(...)           /**< Disables dumping of octet streams. */
#define DM_LOG(...)            /**< Disables detailed logs. */
#define DM_ERR(...)            /**< Disables error logs. */
#define DM_TRC(...)            /**< Disables traces. */
#endif //DM_DISABLE_LOGS
/** @} */

/**
 * @defgroup device_manager_mutex_lock_unlock Module's Mutex Lock/Unlock Macros.
 *
 * @details Macros used to lock and unlock modules. Currently the SDK does not use mutexes but
 *          framework is provided in case need arises to use an alternative architecture.
 * @{
 */
#define DM_MUTEX_LOCK()   SDK_MUTEX_LOCK(m_dm_mutex)   /**< Lock module using mutex. */
#define DM_MUTEX_UNLOCK() SDK_MUTEX_UNLOCK(m_dm_mutex) /**< Unlock module using mutex. */
/** @} */


/**
 * @defgroup device_manager_misc_defines Miscellaneous defines used across the module.
 * @{
 */
#define DM_GATT_ATTR_SIZE            6                                                   /**< Size of each GATT attribute to be stored persistently. */
#define DM_GATT_SERVER_ATTR_MAX_SIZE ((DM_GATT_ATTR_SIZE * DM_GATT_CCCD_COUNT) + 2) /**< Maximum size of GATT attributes to be stored.*/
#define DM_SERVICE_CONTEXT_COUNT     (DM_PROTOCOL_CNTXT_ALL + 1)                         /**< Maximum number of service contexts. */
#define DM_EVT_DEVICE_CONTEXT_BASE   0x20                                                /**< Base for device context base. */
#define DM_EVT_SERVICE_CONTEXT_BASE  0x30                                                /**< Base for service context base. */
#define DM_EVT_APP_CONTEXT_BASE      0x40                                                /**< Base for application context base. */
#define DM_LOAD_OPERATION_ID         0x01                                                /**< Load operation identifier. */
#define DM_STORE_OPERATION_ID        0x02                                                /**< Store operation identifier. */
#define DM_CLEAR_OPERATION_ID        0x03                                                /**< Clear operation identifier. */
/** @} */

#define DM_GATTS_INVALID_SIZE        0xFFFFFFFF                                     /**< Identifer for GATTS invalid size. */

/**
 * @defgroup api_param_check API Parameters check macros.
 *
 * @details Macros for verifying parameters passed to the module in the APIs. These macros
 *          could be mapped to nothing in the final version of the code in order to save execution
 *          time and program size.
 * @{
 */

//#define DM_DISABLE_API_PARAM_CHECK /**< Macro to disable API parameters check. */

#undef NULL_PARAM_CHECK
#undef VERIFY_MODULE_INITIALIZED
#undef VERIFY_MODULE_INITIALIZED_VOID
#undef VERIFY_APP_REGISTERED
#undef VERIFY_APP_REGISTERED_VOID
#undef VERIFY_CONNECTION_INSTANCE
#undef VERIFY_DEVICE_INSTANCE

#ifndef DM_DISABLE_API_PARAM_CHECK

/**@brief Macro for verifying NULL parameters are not passed to API.
 *
 * @param[in] PARAM Parameter checked for NULL.
 *
 * @retval (NRF_ERROR_NULL | DEVICE_MANAGER_ERR_BASE) when @ref PARAM is NULL.
 */
#define NULL_PARAM_CHECK(PARAM)                            \
    if ((PARAM) == NULL)                                   \
    {                                                      \
        return (NRF_ERROR_NULL | DEVICE_MANAGER_ERR_BASE); \
    }
/**@} */


/**@brief Macro for verifying module's initialization status.
 *
 * @retval (NRF_ERROR_INVALID_STATE | DEVICE_MANAGER_ERR_BASE) when module is not initialized.
 */
#define VERIFY_MODULE_INITIALIZED()                                     \
    do                                                                  \
    {                                                                   \
        if (!m_module_initialized)                                      \
        {                                                               \
            return (NRF_ERROR_INVALID_STATE | DEVICE_MANAGER_ERR_BASE); \
        }                                                               \
    } while (0)


/**@brief Macro for verifying module's initialization status. Returns in case it is not initialized.
 */
#define VERIFY_MODULE_INITIALIZED_VOID() \
    do                                   \
    {                                    \
        if (!m_module_initialized)       \
        {                                \
            return;                      \
        }                                \
    } while (0)


/**@brief Macro for verifying that the application is registered.
 *
 * @param[in] X Application instance identifier.
 *
 * @retval (NRF_ERROR_INVALID_STATE | DEVICE_MANAGER_ERR_BASE) when module API is called without
 *         registering an application with the module.
 */
#define VERIFY_APP_REGISTERED(X)                                        \
    do                                                                  \
    {                                                                   \
        if (((X) >= DEVICE_MANAGER_MAX_APPLICATIONS) ||                 \
            (m_application_table[(X)].ntf_cb == NULL))                  \
        {                                                               \
            return (NRF_ERROR_INVALID_STATE | DEVICE_MANAGER_ERR_BASE); \
        }                                                               \
    } while (0)


/**@brief Macro for verifying that the application is registered. Returns in case it is not
 *        registered.
 *
 * @param[in] X Application instance identifier.
 */
#define VERIFY_APP_REGISTERED_VOID(X)                   \
    do                                                  \
    {                                                   \
        if (((X) >= DEVICE_MANAGER_MAX_APPLICATIONS) || \
            (m_application_table[(X)].ntf_cb == NULL))  \
        {                                               \
            return;                                     \
        }                                               \
    } while (0)


/**@brief Macro for verifying connection instance is allocated.
 *
 * @param[in] X Connection instance identifier.
 *
 * @retval (NRF_ERROR_INVALID_ADDR | DEVICE_MANAGER_ERR_BASE) when connection instance is not
 *         allocated.
 */
#define VERIFY_CONNECTION_INSTANCE(X)                                  \
    do                                                                 \
    {                                                                  \
        if (((X) >= DEVICE_MANAGER_MAX_CONNECTIONS) ||                 \
            (m_connection_table[(X)].state == STATE_IDLE))             \
        {                                                              \
            return (NRF_ERROR_INVALID_ADDR | DEVICE_MANAGER_ERR_BASE); \
        }                                                              \
    } while (0)


/**@brief Macro for verifying if device instance is allocated.
 *
 * @param[in] X Device instance identifier.
 *
 * @retval (NRF_ERROR_INVALID_ADDR | DEVICE_MANAGER_ERR_BASE) when device instance is not allocated.
 */
#define VERIFY_DEVICE_INSTANCE(X)                                      \
    do                                                                 \
    {                                                                  \
        if (((X) >= DEVICE_MANAGER_MAX_BONDS) ||                       \
            (m_peer_table[(X)].id_bitmap == UNASSIGNED))               \
        {                                                              \
            return (NRF_ERROR_INVALID_ADDR | DEVICE_MANAGER_ERR_BASE); \
        }                                                              \
    } while (0)

/**@brief Macro for verifying if device is bonded and thus can store data persistantly.
 *
 * @param[in] X Connection instance identifier.
 *
 * @retval (NRF_ERROR_INVALID_STATE | DEVICE_MANAGER_ERR_BASE) when device is not bonded.
 */
#define VERIFY_DEVICE_BOND(X)                                              \
    do                                                                     \
    {                                                                      \
        if ((m_connection_table[(X)].state & STATE_BONDED) != STATE_BONDED)\
        {                                                                  \
            return (NRF_ERROR_INVALID_STATE | DEVICE_MANAGER_ERR_BASE);    \
        }                                                                  \
    } while (0)
#else
#define NULL_PARAM_CHECK(X)
#define VERIFY_MODULE_INITIALIZED()
#define VERIFY_MODULE_INITIALIZED_VOID()
#define VERIFY_APP_REGISTERED(X)
#define VERIFY_APP_REGISTERED_VOID(X)
#define VERIFY_CONNECTION_INSTANCE(X)
#define VERIFY_DEVICE_INSTANCE(X)
#endif //DM_DISABLE_API_PARAM_CHECK
/** @} */

#define INVALID_CONTEXT_LEN 0xFFFFFFFF /**< Identifier for invalid context length. */
/**@brief Macro for checking that application context size is greater that minimal size.
 *
 * @param[in] X Size of application context.
 *
 * @retval (NRF_ERROR_INVALID_PARAM) when size is smaller than minimun required size.
 */
#define SIZE_CHECK_APP_CONTEXT(X)                                      \
    if ((X) < (APP_CONTEXT_MIN_SIZE))                                  \
    {                                                                  \
        return NRF_ERROR_INVALID_PARAM;                                \
    }


/**
 * @defgroup dm_data_types Module's internal data types.
 *
 * @brief This section describes a module's internal data structures.
 * @{
 */
/**@brief Peer identification information.
 */
typedef struct
{
    ble_gap_id_key_t peer_id;   /**< IRK and/or address of peer. */
    uint16_t         ediv;      /**< Peer's encrypted diversifier. */
    uint8_t          id_bitmap; /**< Contains information if above field is valid. */
} peer_id_t;

STATIC_ASSERT(sizeof(peer_id_t) % 4 == 0); /**< Check to ensure Peer identification information is a multiple of 4. */

/**@brief Portion of bonding information exchanged by a device during bond creation that needs to
 *        be stored persistently.
 *
 * @note  An entry is not made in this table unless device is bonded.
 */
typedef struct
{
    ble_gap_enc_key_t peer_enc_key; /**< Local LTK info, central IRK and address */
} bond_context_t;

STATIC_ASSERT(sizeof(bond_context_t) % 4 == 0); /**< Check to ensure bond information is a multiple of 4. */

/**@brief GATT Server Attributes size and data.
 */
typedef struct
{
    uint32_t flags;                                    /**< Flags identifying the stored attributes. */
    uint32_t size;                                     /**< Size of stored attributes. */
    uint8_t  attributes[DM_GATT_SERVER_ATTR_MAX_SIZE]; /**< Array to hold the server attributes. */
} dm_gatts_context_t;

STATIC_ASSERT(sizeof(dm_gatts_context_t) % 4 == 0); /**< Check to ensure GATT Server Attributes size and data information is a multiple of 4. */

/**@brief GATT Client context information. Placeholder for now.
 */
typedef struct
{
    void * p_dummy; /**< Placeholder, currently unused. */
} dm_gatt_client_context_t;

STATIC_ASSERT(sizeof(dm_gatt_client_context_t) % 4 == 0);  /**< Check to ensure GATT Client context information is a multiple of 4. */
STATIC_ASSERT((DEVICE_MANAGER_APP_CONTEXT_SIZE % 4) == 0); /**< Check to ensure device manager application context information is a multiple of 4. */

/**@brief Connection instance definition. Maintains information with respect to an active peer.
 */
typedef struct
{
    ble_gap_addr_t peer_addr;     /**< Peer identification information. This information is retained as long as the connection session exists, once disconnected, for non-bonded devices this information is not stored persistently. */
    uint16_t       conn_handle;   /**< Connection handle for the device. */
    uint8_t        state;         /**< Link state. */
    uint8_t        bonded_dev_id; /**< In case the device is bonded, this points to the corresponding bonded device. This index can be used to index service and bond context as well. */
} connection_instance_t;

/**@brief Application instance definition. Maintains information with respect to a registered
 *        application.
 */
typedef struct
{
    dm_event_cb_t        ntf_cb;    /**< Callback registered with the application. */
    ble_gap_sec_params_t sec_param; /**< Local security parameters registered by the application. */
    uint8_t              state;     /**< Application state. Currently this is used only for knowing if any security procedure is in progress and/or a security procedure is pending to be requested. */
    uint8_t              service;   /**< Service registered by the application. */
} application_instance_t;

/**@brief Function for performing necessary action of storing each of the service context as
 *        registered by the application.
 *
 * @param[in] p_block_handle Storage block identifier.
 * @param[in] p_handle       Device handle identifying device that is stored.
 *
 * @retval Operation result code.
 */
typedef ret_code_t (* service_context_access_t)(pstorage_handle_t const * p_block_handle,
                                                dm_handle_t const       * p_handle);

/**@brief Function for performing necessary action of applying the context information.
 *
 * @param[in] p_handle Device handle identifying device that is stored.
 *
 * @retval Operation result code.
 */
typedef ret_code_t (* service_context_apply_t)(dm_handle_t * p_handle);

/**@brief Function for performing necessary functions of storing or updating.
 *
 * @param[in] p_dest Destination address where data is stored persistently.
 * @param[in] p_src  Source address containing data to be stored.
 * @param[in] size   Size of data to be stored expressed in bytes. Must be word aligned.
 * @param[in] offset Offset in bytes to be applied when writing to the block.
 *
 * @retval Operation result code.
 */
typedef uint32_t (* storage_operation)(pstorage_handle_t * p_dest,
                                       uint8_t           * p_src,
                                       pstorage_size_t     size,
                                       pstorage_size_t     offset);
/** @} */

/**
 * @defgroup dm_tables Module's internal tables.
 *
 * @brief This section describes the module's internal tables and the static global variables
 *        needed for its functionality.
 * @{
 */
#if (DEVICE_MANAGER_APP_CONTEXT_SIZE != 0)
static uint8_t * m_app_context_table[DEVICE_MANAGER_MAX_BONDS];                     /**< Table to remember application contexts of bonded devices. */
#endif //DEVICE_MANAGER_APP_CONTEXT_SIZE
__ALIGN(sizeof(uint32_t))
static peer_id_t              m_peer_table[DEVICE_MANAGER_MAX_BONDS] ;              /**< Table to maintain bonded devices' identification information, an instance is allocated in the table when a device is bonded and freed when bond information is deleted. */
__ALIGN(sizeof(uint32_t))
static bond_context_t         m_bond_table[DEVICE_MANAGER_MAX_CONNECTIONS];         /**< Table to maintain bond information for active peers. */
static dm_gatts_context_t     m_gatts_table[DEVICE_MANAGER_MAX_CONNECTIONS];        /**< Table for service information for active connection instances. */
static connection_instance_t  m_connection_table[DEVICE_MANAGER_MAX_CONNECTIONS];   /**< Table to maintain active peer information. An instance is allocated in the table when a new connection is established and freed on disconnection. */
static application_instance_t m_application_table[DEVICE_MANAGER_MAX_APPLICATIONS]; /**< Table to maintain application instances. */
static pstorage_handle_t      m_storage_handle;                                     /**< Persistent storage handle for blocks requested by the module. */
static uint32_t               m_peer_addr_update;                                   /**< 32-bit bitmap to remember peer device address update. */
static ble_gap_id_key_t       m_local_id_info;                                      /**< ID information of central in case resolvable address is used. */
static bool                   m_module_initialized = false;                         /**< State indicating if module is initialized or not. */
static uint8_t                m_irk_index_table[DEVICE_MANAGER_MAX_BONDS];          /**< List maintaining IRK index list. */

SDK_MUTEX_DEFINE(m_dm_mutex) /**< Mutex variable. Currently unused, this declaration does not occupy any space in RAM. */
/** @} */

static __INLINE ret_code_t no_service_context_store(pstorage_handle_t const * p_block_handle,
                                                    dm_handle_t const       * p_handle);

static __INLINE ret_code_t gatts_context_store(pstorage_handle_t const * p_block_handle,
                                               dm_handle_t const       * p_handle);

static __INLINE ret_code_t gattc_context_store(pstorage_handle_t const * p_block_handle,
                                               dm_handle_t const       * p_handle);

static __INLINE ret_code_t gattsc_context_store(pstorage_handle_t const * p_block_handle,
                                                dm_handle_t const       * p_handle);

static __INLINE ret_code_t no_service_context_load(pstorage_handle_t const * p_block_handle,
                                                   dm_handle_t const       * p_handle);

static __INLINE ret_code_t gatts_context_load(pstorage_handle_t const * p_block_handle,
                                              dm_handle_t const       * p_handle);

static __INLINE ret_code_t gattc_context_load(pstorage_handle_t const * p_block_handle,
                                              dm_handle_t const       * p_handle);

static __INLINE ret_code_t gattsc_context_load(pstorage_handle_t const * p_block_handle,
                                               dm_handle_t const       * p_handle);

static __INLINE ret_code_t no_service_context_apply(dm_handle_t * p_handle);

static __INLINE ret_code_t gatts_context_apply(dm_handle_t * p_handle);

static __INLINE ret_code_t gattc_context_apply(dm_handle_t * p_handle);

static __INLINE ret_code_t gattsc_context_apply(dm_handle_t * p_handle);


/**< Array of function pointers based on the types of service registered. */
const service_context_access_t m_service_context_store[DM_SERVICE_CONTEXT_COUNT] =
{
    no_service_context_store, /**< Dummy function, when there is no service context registered. */
    gatts_context_store,      /**< GATT Server context store function. */
    gattc_context_store,      /**< GATT Client context store function. */
    gattsc_context_store      /**< GATT Server & Client context store function. */
};


/**< Array of function pointers based on the types of service registered. */
const service_context_access_t m_service_context_load[DM_SERVICE_CONTEXT_COUNT] =
{
    no_service_context_load,  /**< Dummy function, when there is no service context registered. */
    gatts_context_load,       /**< GATT Server context load function. */
    gattc_context_load,       /**< GATT Client context load function. */
    gattsc_context_load       /**< GATT Server & Client context load function. */
};


/**< Array of function pointers based on the types of service registered. */
const service_context_apply_t m_service_context_apply[DM_SERVICE_CONTEXT_COUNT] =
{
    no_service_context_apply, /**< Dummy function, when there is no service context registered. */
    gatts_context_apply,      /**< GATT Server context apply function. */
    gattc_context_apply,      /**< GATT Client context apply function. */
    gattsc_context_apply      /**< GATT Server & Client context apply function. */
};


const uint32_t m_context_init_len = 0xFFFFFFFF; /**< Constant used to update the initial value for context in the flash. */

/**@brief Function for setting update status for the device identified by 'index'.
 *
 * @param[in] index Device identifier.
 */
static __INLINE void update_status_bit_set(uint32_t index)
{
    m_peer_addr_update |= (BIT_0 << index);
}


/**@brief Function for resetting update status for device identified by 'index'.
 *
 * @param[in] index Device identifier.
 */
static __INLINE void update_status_bit_reset(uint32_t index)
{
    m_peer_addr_update &= (~((uint32_t)BIT_0 << index));
}


/**@brief Function for providing update status for the device identified by 'index'.
 *
 * @param[in] index Device identifier.
 *
 * @retval true if the bit is set, false otherwise.
 */
static __INLINE bool update_status_bit_is_set(uint32_t index)
{
    return ((m_peer_addr_update & (BIT_0 << index)) ? true : false);
}


/**@brief Function for initialiasing the application instance identified by 'index'.
 *
 * @param[in] index Device identifier.
 */
static __INLINE void application_instance_init(uint32_t index)
{
    DM_TRC("[DM]: Initializing Application Instance 0x%08X.\r\n", index);

    m_application_table[index].ntf_cb  = NULL;
    m_application_table[index].state   = 0x00;
    m_application_table[index].service = 0x00;
}


/**@brief Function for initialiasing the connection instance identified by 'index'.
 *
 * @param[in] index Device identifier.
 */
static __INLINE void connection_instance_init(uint32_t index)
{
    DM_TRC("[DM]: Initializing Connection Instance 0x%08X.\r\n", index);

    m_connection_table[index].state         = STATE_IDLE;
    m_connection_table[index].conn_handle   = BLE_CONN_HANDLE_INVALID;
    m_connection_table[index].bonded_dev_id = DM_INVALID_ID;

    memset(&m_connection_table[index].peer_addr, 0, sizeof (ble_gap_addr_t));
}


/**@brief Function for initialiasing the peer device instance identified by 'index'.
 *
 * @param[in] index Device identifier.
 */
static __INLINE void peer_instance_init(uint32_t index)
{
    DM_TRC("[DM]: Initializing Peer Instance 0x%08X.\r\n", index);

    memset(m_peer_table[index].peer_id.id_addr_info.addr, 0, BLE_GAP_ADDR_LEN);
    memset(m_peer_table[index].peer_id.id_info.irk, 0, BLE_GAP_SEC_KEY_LEN);

    //Initialize the address type to invalid.
    m_peer_table[index].peer_id.id_addr_info.addr_type = INVALID_ADDR_TYPE;

    //Initialize the identification bit map to unassigned.
    m_peer_table[index].id_bitmap = UNASSIGNED;

    // Initialize diversifier.
    m_peer_table[index].ediv      = EDIV_INIT_VAL;


    //Reset the status bit.
    update_status_bit_reset(index);

#if (DEVICE_MANAGER_APP_CONTEXT_SIZE != 0)
    //Initialize the application context for bond device.
    m_app_context_table[index] = NULL;
#endif //DEVICE_MANAGER_APP_CONTEXT_SIZE
}


/**@brief Function for searching connection instance matching the connection handle and the state
 *        requested.
 *
 * @details Connection handle and state information is used to get a connection instance, it
 *          is possible to ignore the connection handle by using BLE_CONN_HANDLE_INVALID.
 *
 * @param[in]  conn_handle Connection handle.
 * @param[in]  state       Connection instance state.
 * @param[out] p_instance  Connection instance.
 *
 * @retval NRF_SUCCESS             Operation success.
 * @retval NRF_ERROR_INVALID_STATE Operation failure. Invalid state
 * @retval NRF_ERROR_NOT_FOUND     Operation failure. Not found
 */
static ret_code_t connection_instance_find(uint16_t   conn_handle,
                                           uint8_t    state,
                                           uint32_t * p_instance)
{
    ret_code_t err_code;
    uint32_t   index;

    err_code = NRF_ERROR_INVALID_STATE;

    for (index = 0; index < DEVICE_MANAGER_MAX_CONNECTIONS; index++)
    {
        //Search only based on the state.
        if (state & m_connection_table[index].state)
        {
            //Ignore the connection handle.
            if ((conn_handle == BLE_CONN_HANDLE_INVALID) ||
                (conn_handle == m_connection_table[index].conn_handle))
            {
                //Search for matching connection handle.
                (*p_instance) = index;
                err_code      = NRF_SUCCESS;

                break;
            }
            else
            {
                err_code = NRF_ERROR_NOT_FOUND;
            }
        }
    }

    return err_code;
}


/**@brief Function for allocating device instance for a bonded device.
 *
 * @param[out] p_device_index Device index.
 * @param[in]  p_addr         Peer identification information.
 *
 * @retval NRF_SUCCESS            Operation success.
 * @retval DM_DEVICE_CONTEXT_FULL Operation failure.
 */
static __INLINE ret_code_t device_instance_allocate(uint8_t *              p_device_index,
                                                    ble_gap_addr_t const * p_addr)
{
    ret_code_t err_code;
    uint32_t   index;

    err_code = DM_DEVICE_CONTEXT_FULL;

    for (index = 0; index < DEVICE_MANAGER_MAX_BONDS; index++)
    {
        DM_TRC("[DM]:[DI 0x%02X]: Device type 0x%02X.\r\n",
               index, m_peer_table[index].peer_id.id_addr_info.addr_type);
        DM_TRC("[DM]: Device Addr 0x%02X 0x%02X 0x%02X 0x%02X 0x%02X 0x%02X.\r\n",
               m_peer_table[index].peer_id.id_addr_info.addr[0],
               m_peer_table[index].peer_id.id_addr_info.addr[1],
               m_peer_table[index].peer_id.id_addr_info.addr[2],
               m_peer_table[index].peer_id.id_addr_info.addr[3],
               m_peer_table[index].peer_id.id_addr_info.addr[4],
               m_peer_table[index].peer_id.id_addr_info.addr[5]);

        if (m_peer_table[index].id_bitmap == UNASSIGNED)
        {
            if (p_addr->addr_type != BLE_GAP_ADDR_TYPE_RANDOM_PRIVATE_RESOLVABLE)
            {
                m_peer_table[index].id_bitmap            &= (~ADDR_ENTRY);
                m_peer_table[index].peer_id.id_addr_info  = (*p_addr);
            }
            else
            {
                m_peer_table[index].id_bitmap &= (~IRK_ENTRY);
            }

            (*p_device_index) = index;
            err_code          = NRF_SUCCESS;

            DM_LOG("[DM]: Allocated device instance 0x%02X\r\n", index);

            break;
        }
    }

    return err_code;
}


/**@brief Function for freeing a device instance allocated for bonded device.
 *
 * @param[in] device_index Device index.
 *
 * @retval NRF_SUCCESS On success, else an error code indicating reason for failure.
 */
static __INLINE ret_code_t device_instance_free(uint32_t device_index)
{
    ret_code_t        err_code;
    pstorage_handle_t block_handle;

    //Get the block handle.
    err_code = pstorage_block_identifier_get(&m_storage_handle, device_index, &block_handle);

    if (err_code == NRF_SUCCESS)
    {
        DM_TRC("[DM]:[DI 0x%02X]: Freeing Instance.\r\n", device_index);

        //Request clearing of the block.
        err_code = pstorage_clear(&block_handle, ALL_CONTEXT_SIZE);

        if (err_code == NRF_SUCCESS)
        {
            peer_instance_init(device_index);
        }
    }

    return err_code;
}


/**@brief Function for searching for the device in the bonded device list.
 *
 * @param[in]  p_addr         Peer identification information.
 * @param[out] p_device_index Device index.
 *
 * @retval NRF_SUCCESS         Operation success.
 * @retval NRF_ERROR_NOT_FOUND Operation failure.
 */
static ret_code_t device_instance_find(ble_gap_addr_t const * p_addr, uint32_t * p_device_index, uint16_t ediv)
{
    ret_code_t err_code;
    uint32_t   index;

    err_code = NRF_ERROR_NOT_FOUND;

    if (NULL != p_addr)
    {
        DM_TRC("[DM]: Searching for device 0x%02X 0x%02X 0x%02X 0x%02X 0x%02X 0x%02X.\r\n",
               p_addr->addr[0],
               p_addr->addr[1],
               p_addr->addr[2],
               p_addr->addr[3],
               p_addr->addr[4],
               p_addr->addr[5]);
    }

    for (index = 0; index < DEVICE_MANAGER_MAX_BONDS; index++)
    {
        DM_TRC("[DM]:[DI 0x%02X]: Device type 0x%02X.\r\n",
               index, m_peer_table[index].peer_id.id_addr_info.addr_type);
        DM_TRC("[DM]: Device Addr 0x%02X 0x%02X 0x%02X 0x%02X 0x%02X 0x%02X.\r\n",
               m_peer_table[index].peer_id.id_addr_info.addr[0],
               m_peer_table[index].peer_id.id_addr_info.addr[1],
               m_peer_table[index].peer_id.id_addr_info.addr[2],
               m_peer_table[index].peer_id.id_addr_info.addr[3],
               m_peer_table[index].peer_id.id_addr_info.addr[4],
               m_peer_table[index].peer_id.id_addr_info.addr[5]);

        if (((NULL == p_addr) && (ediv == m_peer_table[index].ediv)) ||
            ((NULL != p_addr) && (memcmp(&m_peer_table[index].peer_id.id_addr_info, p_addr, sizeof(ble_gap_addr_t)) == 0)))
        {
            DM_LOG("[DM]: Found device at instance 0x%02X\r\n", index);

            (*p_device_index) = index;
            err_code          = NRF_SUCCESS;

            break;
        }
    }

    return err_code;
}


/**@brief Function for notifying connection manager event to the application.
 *
 * @param[in] p_handle     Device handle identifying device.
 * @param[in] p_event      Connection manager event details.
 * @param[in] event_result Event result code.
 */
static __INLINE void app_evt_notify(dm_handle_t const * const p_handle,
                                    dm_event_t const * const  p_event,
                                    uint32_t                  event_result)
{
    dm_event_cb_t app_cb = m_application_table[0].ntf_cb;

    DM_MUTEX_UNLOCK();

    DM_TRC("[DM]: Notifying application of event 0x%02X\r\n", p_event->event_id);

    //No need to do any kind of return value processing thus can be supressed.
    UNUSED_VARIABLE(app_cb(p_handle, p_event, event_result));

    DM_MUTEX_LOCK();
}


/**@brief Function for allocating instance.
 *
 * @details The instance identifier is provided in the 'p_instance' parameter if the routine
 *          succeeds.
 *
 * @param[out] p_instance Connection instance.
 *
 * @retval NRF_SUCCESS      Operation success.
 * @retval NRF_ERROR_NO_MEM Operation failure. No memory.
 */
static __INLINE uint32_t connection_instance_allocate(uint32_t * p_instance)
{
    uint32_t err_code;

    DM_TRC("[DM]: Request to allocation connection instance\r\n");

    err_code = connection_instance_find(BLE_CONN_HANDLE_INVALID, STATE_IDLE, p_instance);

    if (err_code == NRF_SUCCESS)
    {
        DM_LOG("[DM]:[%02X]: Connection Instance Allocated.\r\n", (*p_instance));
        m_connection_table[*p_instance].state = STATE_CONNECTED;
    }
    else
    {
        DM_LOG("[DM]: No free connection instances available\r\n");
        err_code = NRF_ERROR_NO_MEM;
    }

    return err_code;
}


/**@brief Function for freeing instance. Instance identifier is provided in the parameter
 *        'p_instance' in case the routine succeeds.
 *
 * @param[in] p_instance Connection instance.
 */
static __INLINE void connection_instance_free(uint32_t const * p_instance)
{
    DM_TRC("[DM]:[CI 0x%02X]: Freeing connection instance\r\n", (*p_instance));

    if (m_connection_table[*p_instance].state != STATE_IDLE)
    {
        DM_LOG("[DM]:[%02X]: Freed connection instance.\r\n", (*p_instance));
        connection_instance_init(*p_instance);
    }
}


/**@brief Function for storage operation dummy handler.
 *
 * @param[in] p_dest Destination address where data is to be stored persistently.
 * @param[in] p_src  Source address containing data to be stored. API assumes this to be resident
 *                   memory and no intermediate copy of data is made by the API.
 * @param[in] size   Size of data to be stored expressed in bytes. Should be word aligned.
 * @param[in] offset Offset in bytes to be applied when writing to the block.
 *                   For example, if within a block of 100 bytes, application wishes to
 *                   write 20 bytes at offset of 12, then this field should be set to 12.
 *                   Should be word aligned.
 *
 * @retval NRF_SUCCESS Operation success.
 */
static uint32_t storage_operation_dummy_handler(pstorage_handle_t * p_dest,
                                                uint8_t           * p_src,
                                                pstorage_size_t     size,
                                                pstorage_size_t     offset)
{
    return NRF_SUCCESS;
}


/**@brief Function for saving the device context persistently.
 *
 * @param[in] p_handle Device handle identifying device.
 * @param[in] state    Device store state.
 */
static __INLINE void device_context_store(dm_handle_t const * p_handle, device_store_state_t state)
{
    pstorage_handle_t block_handle;
    storage_operation store_fn;
    ret_code_t        err_code;

    DM_LOG("[DM]: --> device_context_store\r\n");

    err_code = pstorage_block_identifier_get(&m_storage_handle,
                                             p_handle->device_id,
                                             &block_handle);

    if (err_code == NRF_SUCCESS)
    {
        if ((STATE_BOND_INFO_UPDATE ==
             (m_connection_table[p_handle->connection_id].state & STATE_BOND_INFO_UPDATE)) ||
            (state == UPDATE_PEER_ADDR))
        {
            DM_LOG("[DM]:[DI %02X]:[CI %02X]: -> Updating bonding information.\r\n",
                   p_handle->device_id, p_handle->connection_id);

            store_fn = pstorage_update;
        }
        else if (state == FIRST_BOND_STORE)
        {
            DM_LOG("[DM]:[DI %02X]:[CI %02X]: -> Storing bonding information.\r\n",
                   p_handle->device_id, p_handle->connection_id);

            store_fn = pstorage_store;
        }
        else
        {
            DM_LOG("[DM]:[DI %02X]:[CI %02X]: -> No update in bonding information.\r\n",
                   p_handle->device_id, p_handle->connection_id);

            //No operation needed.
            store_fn = storage_operation_dummy_handler;
        }

        //Store the peer id.
        err_code = store_fn(&block_handle,
                            (uint8_t *)&m_peer_table[p_handle->device_id],
                            PEER_ID_SIZE,
                            PEER_ID_STORAGE_OFFSET);

        if ((err_code == NRF_SUCCESS) && (state != UPDATE_PEER_ADDR))
        {
            m_connection_table[p_handle->connection_id].state &= (~STATE_BOND_INFO_UPDATE);

            //Store the bond information.
            err_code = store_fn(&block_handle,
                                (uint8_t *)&m_bond_table[p_handle->connection_id],
                                BOND_SIZE,
                                BOND_STORAGE_OFFSET);

            if (err_code != NRF_SUCCESS)
            {
                DM_ERR("[DM]:[0x%02X]:Failed to store bond information, reason 0x%08X\r\n",
                       p_handle->device_id, err_code);
            }
        }

        if (state != UPDATE_PEER_ADDR)
        {
            //Store the service information
            err_code = m_service_context_store[m_application_table[p_handle->appl_id].service]
                    (
                        &block_handle,
                        p_handle
                    );

            if (err_code != NRF_SUCCESS)
            {
                //Notify application of an error event.
                DM_ERR("[DM]: Failed to store service context, reason %08X\r\n", err_code);
            }
        }
    }

    if (err_code != NRF_SUCCESS)
    {
        //Notify application of an error event.
        DM_ERR("[DM]: Failed to store device context, reason %08X\r\n", err_code);
    }
}


/**@brief Function for storing when there is no service registered.
 *
 * @param[in] p_block_handle Storage block identifier.
 * @param[in] p_handle       Device handle identifying device that is loaded.
 *
 * @retval NRF_SUCCESS
 */
static __INLINE ret_code_t no_service_context_store(pstorage_handle_t const * p_block_handle,
                                                    dm_handle_t const       * p_handle)
{
    DM_LOG("[DM]: --> no_service_context_store\r\n");

    return NRF_SUCCESS;
}


/**@brief Function for storing GATT Server context.
 *
 * @param[in] p_block_handle Storage block identifier.
 * @param[in] p_handle       Device handle identifying device that is stored.
 *
 * @retval NRF_SUCCESS Operation success.
 */
static __INLINE ret_code_t gatts_context_store(pstorage_handle_t const * p_block_handle,
                                               dm_handle_t const       * p_handle)
{
    storage_operation store_fn;
    uint32_t          attr_flags = BLE_GATTS_SYS_ATTR_FLAG_SYS_SRVCS | BLE_GATTS_SYS_ATTR_FLAG_USR_SRVCS;
    uint16_t          attr_len   = DM_GATT_SERVER_ATTR_MAX_SIZE;
    uint8_t           sys_data[DM_GATT_SERVER_ATTR_MAX_SIZE];

    DM_LOG("[DM]: --> gatts_context_store\r\n");

    uint32_t err_code = sd_ble_gatts_sys_attr_get(
        m_connection_table[p_handle->connection_id].conn_handle,
        sys_data,
        &attr_len,
        attr_flags);

    if (err_code == NRF_SUCCESS)
    {
        if (memcmp(m_gatts_table[p_handle->connection_id].attributes, sys_data, attr_len) == 0)
        {
            //No store operation is needed.
            DM_LOG("[DM]:[0x%02X]: No change in GATTS Context information.\r\n",
                   p_handle->device_id);

            if ((m_connection_table[p_handle->connection_id].state & STATE_CONNECTED) !=
                STATE_CONNECTED)
            {
                DM_LOG("[DM]:[0x%02X]: Resetting GATTS for active instance.\r\n",
                       p_handle->connection_id);

                //Reset GATTS information for the current context.
                memset(&m_gatts_table[p_handle->connection_id], 0, sizeof(dm_gatts_context_t));
            }
        }
        else
        {
            if (m_gatts_table[p_handle->connection_id].size != 0)
            {
                //There is data already stored in persistent memory, therefore an update is needed.
                DM_LOG("[DM]:[0x%02X]: Updating stored service context\r\n", p_handle->device_id);

                store_fn = pstorage_update;
            }
            else
            {
                //Fresh write, a store is needed.
                DM_LOG("[DM]:[0x%02X]: Storing service context\r\n", p_handle->device_id);

                store_fn = pstorage_store;
            }

            m_gatts_table[p_handle->connection_id].flags = attr_flags;
            m_gatts_table[p_handle->connection_id].size  = attr_len;
            memcpy(m_gatts_table[p_handle->connection_id].attributes, sys_data, attr_len);

            DM_DUMP((uint8_t *)&m_gatts_table[p_handle->connection_id], sizeof(dm_gatts_context_t));

            DM_LOG("[DM]:[0x%02X]: GATTS Data size 0x%08X\r\n",
                   p_handle->device_id,
                   m_gatts_table[p_handle->connection_id].size);

            //Store GATTS information.
            err_code = store_fn((pstorage_handle_t *)p_block_handle,
                                (uint8_t *)&m_gatts_table[p_handle->connection_id],
                                GATTS_SERVICE_CONTEXT_SIZE,
                                SERVICE_STORAGE_OFFSET);

            if (err_code != NRF_SUCCESS)
            {
                DM_ERR("[DM]:[0x%02X]:Failed to store service context, reason 0x%08X\r\n",
                       p_handle->device_id,
                       err_code);
            }
            else
            {
                DM_LOG("[DM]: Service context successfully stored.\r\n");
            }
        }
    }

    return NRF_SUCCESS;
}


/**@brief Function for storing GATT Client context.
 *
 * @param[in] p_block_handle Storage block identifier.
 * @param[in] p_handle       Device handle identifying device that is stored.
 *
 * @retval NRF_SUCCESS Operation success.
 */
static __INLINE ret_code_t gattc_context_store(pstorage_handle_t const * p_block_handle,
                                               dm_handle_t const       * p_handle)
{
    DM_LOG("[DM]: --> gattc_context_store\r\n");

    return NRF_SUCCESS;
}


/**@brief Function for storing GATT Server & Client context.
 *
 * @param[in] p_block_handle Storage block identifier.
 * @param[in] p_handle       Device handle identifying device that is stored.
 *
 * @retval NRF_SUCCESS On success, else an error code indicating reason for failure.
 */
static __INLINE ret_code_t gattsc_context_store(pstorage_handle_t const * p_block_handle,
                                                dm_handle_t const       * p_handle)
{
    DM_LOG("[DM]: --> gattsc_context_store\r\n");

    ret_code_t err_code = gatts_context_store(p_block_handle, p_handle);

    if (NRF_SUCCESS == err_code)
    {
        err_code = gattc_context_store(p_block_handle, p_handle);
    }

    return err_code;
}


/**@brief Function for loading when there is no service registered.
 *
 * @param[in] p_block_handle Storage block identifier.
 * @param[in] p_handle       Device handle identifying device that is loaded.
 *
 * @retval NRF_SUCCESS
 */
static __INLINE ret_code_t no_service_context_load(pstorage_handle_t const * p_block_handle,
                                                   dm_handle_t const       * p_handle)
{
    DM_LOG("[DM]: --> no_service_context_load\r\n");

    return NRF_SUCCESS;
}


/**@brief Function for loading GATT Server context.
 *
 * @param[in] p_block_handle Storage block identifier.
 * @param[in] p_handle       Device handle identifying device that is loaded.
 *
 * @retval NRF_SUCCESS On success, else an error code indicating reason for failure.
 */
static __INLINE ret_code_t gatts_context_load(pstorage_handle_t const * p_block_handle,
                                              dm_handle_t const       * p_handle)
{
    DM_LOG("[DM]:[CI 0x%02X]:[DI 0x%02X]: --> gatts_context_load\r\n",
           p_handle->connection_id,
           p_handle->device_id);

    ret_code_t err_code = pstorage_load((uint8_t *)&m_gatts_table[p_handle->connection_id],
                                        (pstorage_handle_t *)p_block_handle,
                                        GATTS_SERVICE_CONTEXT_SIZE,
                                        SERVICE_STORAGE_OFFSET);

    if (err_code == NRF_SUCCESS)
    {
        DM_LOG("[DM]:[%02X]:[Block ID 0x%08X]: Service context loaded, size 0x%08X\r\n",
               p_handle->connection_id,
               p_block_handle->block_id,
               m_gatts_table[p_handle->connection_id].size);
        DM_DUMP((uint8_t *)&m_gatts_table[p_handle->connection_id], sizeof(dm_gatts_context_t));

        if (m_gatts_table[p_handle->connection_id].size == DM_GATTS_INVALID_SIZE)
        {
            m_gatts_table[p_handle->connection_id].size = 0;
        }
    }
    else
    {
        DM_ERR("[DM]:[%02X]: Failed to load Service context, reason %08X\r\n",
               p_handle->connection_id,
               err_code);
    }

    return err_code;
}


/**@brief Function for loading GATT Client context.
 *
 * @param[in] p_block_handle Storage block identifier.
 * @param[in] p_handle       Device handle identifying device that is loaded.
 *
 * @retval NRF_SUCCESS
 */
static __INLINE ret_code_t gattc_context_load(pstorage_handle_t const * p_block_handle,
                                              dm_handle_t const       * p_handle)
{
    DM_LOG("[DM]: --> gattc_context_load\r\n");

    return NRF_SUCCESS;
}


/**@brief Function for loading GATT Server & Client context.
 *
 * @param[in] p_block_handle Storage block identifier.
 * @param[in] p_handle       Device handle identifying device that is loaded.
 *
 * @retval NRF_SUCCESS On success, else an error code indicating reason for failure.
 */
static __INLINE ret_code_t gattsc_context_load(pstorage_handle_t const * p_block_handle,
                                               dm_handle_t const       * p_handle)
{
    DM_LOG("[DM]: --> gattsc_context_load\r\n");

    ret_code_t err_code = gatts_context_load(p_block_handle, p_handle);

    if (NRF_SUCCESS == err_code)
    {
        err_code = gattc_context_load(p_block_handle, p_handle);
    }

    return err_code;
}


/**@brief Function for applying when there is no service registered.
 *
 * @param[in] p_handle Device handle identifying device that is applied.
 *
 * @retval NRF_SUCCESS
 */
static __INLINE ret_code_t no_service_context_apply(dm_handle_t * p_handle)
{
    DM_LOG("[DM]: --> no_service_context_apply\r\n");
    DM_LOG("[DM]:[CI 0x%02X]: No Service context\r\n", p_handle->connection_id);

    return NRF_SUCCESS;
}


/**@brief Function for applying GATT Server context.
 *
 * @param[in] p_handle Device handle identifying device that is applied.
 *
 * @retval NRF_SUCCESS                    On success.
 * @retval DM_SERVICE_CONTEXT_NOT_APPLIED On failure.
 */
static __INLINE ret_code_t gatts_context_apply(dm_handle_t * p_handle)
{
    uint32_t err_code;

    uint8_t * p_gatts_context = NULL;
    uint16_t  context_len     = 0;
    uint32_t  context_flags   = 0;

    DM_LOG("[DM]: --> gatts_context_apply\r\n");
    DM_LOG("[DM]:[CI 0x%02X]: State 0x%02X, Size 0x%08X\r\n",
           p_handle->connection_id,
           m_connection_table[p_handle->connection_id].state,
           m_gatts_table[p_handle->connection_id].size);

    if ((m_gatts_table[p_handle->connection_id].size != 0) &&
        (
            ((m_connection_table[p_handle->connection_id].state & STATE_LINK_ENCRYPTED) == STATE_LINK_ENCRYPTED) &&
            ((m_connection_table[p_handle->connection_id].state & STATE_BOND_INFO_UPDATE)
             != STATE_BOND_INFO_UPDATE)
        )
       )
    {
        DM_LOG("[DM]: Setting stored context.\r\n");

        p_gatts_context = &m_gatts_table[p_handle->connection_id].attributes[0];
        context_len     = m_gatts_table[p_handle->connection_id].size;
        context_flags   = m_gatts_table[p_handle->connection_id].flags;
    }

    err_code = sd_ble_gatts_sys_attr_set(m_connection_table[p_handle->connection_id].conn_handle,
                                         p_gatts_context,
                                         context_len,
                                         context_flags);

    if (err_code == NRF_ERROR_INVALID_DATA)
    {
        // Indication that the ATT table has changed. Restore the system attributes to system
        // services only and send a service changed indication if possible.
        context_flags = BLE_GATTS_SYS_ATTR_FLAG_SYS_SRVCS;
        err_code      = sd_ble_gatts_sys_attr_set(m_connection_table[p_handle->connection_id].conn_handle,
                                                  p_gatts_context,
                                                  context_len,
                                                  context_flags);
    }

    if (err_code != NRF_SUCCESS)
    {
        DM_LOG("[DM]: Failed to set system attributes, reason 0x%08X.\r\n", err_code);

        err_code = DM_SERVICE_CONTEXT_NOT_APPLIED;
    }

    if (context_flags == BLE_GATTS_SYS_ATTR_FLAG_SYS_SRVCS)
    {
        err_code = sd_ble_gatts_service_changed(m_connection_table[p_handle->connection_id].conn_handle,
                                                0x000C,
                                                0xFFFF);
        if (err_code != NRF_SUCCESS)
        {
            DM_LOG("[DM]: Failed to send Service Changed indication, reason 0x%08X.\r\n", err_code);
            if ((err_code != BLE_ERROR_INVALID_CONN_HANDLE) &&
                (err_code != NRF_ERROR_INVALID_STATE) &&
                (err_code != BLE_ERROR_NO_TX_PACKETS) &&
                (err_code != NRF_ERROR_BUSY))
            {
                // Those errors can be expected when sending trying to send Service Changed
                // Indication if the CCCD is not set to indicate. Thus set the returning error
                //  code to success.
                err_code = NRF_SUCCESS;
            }
            else
            {
                err_code = DM_SERVICE_CONTEXT_NOT_APPLIED;
            }
        }
    }

    return err_code;
}


/**@brief Function for applying GATT Client context.
 *
 * @param[in] p_handle Device handle identifying device that is applied.
 *
 * @retval NRF_SUCCESS On success.
 */
static __INLINE ret_code_t gattc_context_apply(dm_handle_t * p_handle)
{
    DM_LOG("[DM]: --> gattc_context_apply\r\n");

    return NRF_SUCCESS;
}


/**@brief Function for applying GATT Server & Client context.
 *
 * @param[in] p_handle Device handle identifying device that is applied.
 *
 * @retval NRF_SUCCESS On success, else an error code indicating reason for failure.
 */
static __INLINE ret_code_t gattsc_context_apply(dm_handle_t * p_handle)
{
    uint32_t err_code;

    DM_LOG("[DM]: --> gattsc_context_apply\r\n");

    err_code = gatts_context_apply(p_handle);

    if (err_code == NRF_SUCCESS)
    {
        err_code = gattc_context_apply(p_handle);
    }

    return err_code;
}


/**@brief Function for pstorage module callback.
 *
 * @param[in] p_handle Identifies module and block for which callback is received.
 * @param[in] op_code  Identifies the operation for which the event is notified.
 * @param[in] result   Identifies the result of flash access operation.
 *                     NRF_SUCCESS implies, operation succeeded.
 * @param[in] p_data   Identifies the application data pointer. In case of store operation, this
 *                     points to the resident source of application memory that application can now
 *                     free or reuse. In case of clear, this is NULL as no application pointer is
 *                     needed for this operation.
 * @param[in] data_len Length of data provided by the application for the operation.
 */
static void dm_pstorage_cb_handler(pstorage_handle_t * p_handle,
                                   uint8_t             op_code,
                                   uint32_t            result,
                                   uint8_t           * p_data,
                                   uint32_t            data_len)
{
    VERIFY_APP_REGISTERED_VOID(0);

    if (data_len > ALL_CONTEXT_SIZE)
    {
        //Clearing of all bonds at initialization, no event is generated.
        return;
    }

    DM_MUTEX_LOCK();

    dm_event_t        dm_event;
    dm_handle_t       dm_handle;
    dm_context_t      context_data;
    pstorage_handle_t block_handle;
    uint32_t          index_count;
    uint32_t          err_code;

    bool app_notify = true;

    err_code = dm_handle_initialize(&dm_handle);
    APP_ERROR_CHECK(err_code);

    dm_handle.appl_id = 0;
    dm_event.event_id = 0x00;

    //Construct the event which it is related to.

    //Initialize context data information and length.
    context_data.p_data = p_data;
    context_data.len    = data_len;

    for (uint32_t index = 0; index < DEVICE_MANAGER_MAX_BONDS; index++)
    {
        err_code = pstorage_block_identifier_get(&m_storage_handle, index, &block_handle);
        if ((err_code == NRF_SUCCESS) &&
            (
                (memcmp(p_handle, &block_handle, sizeof(pstorage_handle_t)) == 0)
            )
           )
        {
            dm_handle.device_id = index;
            break;
        }
    }

    if (dm_handle.device_id != DM_INVALID_ID)
    {
        if (op_code == PSTORAGE_CLEAR_OP_CODE)
        {
            if (data_len == ALL_CONTEXT_SIZE)
            {
                dm_event.event_id = DM_EVT_DEVICE_CONTEXT_BASE;
            }
            else
            {
                dm_event.event_id = DM_EVT_APP_CONTEXT_BASE;
            }
        }
        else
        {
            //Update or store operation.
            //Context is identified based on the pointer value. Device context, application context
            //and service context all have their own value range.
            index_count = ((uint32_t)(p_data - (uint8_t *)m_peer_table)) / PEER_ID_SIZE;

            if (index_count < DEVICE_MANAGER_MAX_BONDS)
            {
                dm_event.event_param.p_device_context = &context_data;

                //Only the peer identification is stored, not bond information. Hence do not notify
                //the application yet, unless the store operation resulted in a failure.
                if ((result == NRF_SUCCESS) &&
                    (
                        (update_status_bit_is_set(dm_handle.device_id) == false)
                    )
                   )
                {
                    app_notify = false;
                }
                else
                {
                    //Reset update status since update is complete.
                    update_status_bit_reset(dm_handle.device_id);

                    //Notify application of error in storing the context.
                    dm_event.event_id = DM_EVT_DEVICE_CONTEXT_BASE;
                }
            }
            else
            {
                index_count = ((uint32_t)(p_data - (uint8_t *)m_bond_table)) / BOND_SIZE;

                if (index_count < DEVICE_MANAGER_MAX_CONNECTIONS)
                {
                    DM_LOG("[DM]:[0x%02X]:[0x%02X]: Bond context Event\r\n",
                           dm_handle.device_id,
                           dm_handle.connection_id);

                    dm_event.event_param.p_device_context = &context_data;
                    dm_event.event_id                     = DM_EVT_DEVICE_CONTEXT_BASE;
                    dm_handle.connection_id               = index_count;

                    ble_gap_sec_keyset_t keys_exchanged;
                    keys_exchanged.keys_peer.p_enc_key = NULL;
                    keys_exchanged.keys_peer.p_id_key  = &m_local_id_info;
                    keys_exchanged.keys_own.p_enc_key  = &m_bond_table[index_count].peer_enc_key;
                    keys_exchanged.keys_own.p_id_key   = &m_peer_table[dm_handle.device_id].peer_id;

                    //Context information updated to provide the keys.
                    context_data.p_data = (uint8_t *)&keys_exchanged;
                    context_data.len    = sizeof(ble_gap_sec_keyset_t);
                }
                else
                {
                    index_count = ((uint32_t)(p_data - (uint8_t *)m_gatts_table)) /
                                  GATTS_SERVICE_CONTEXT_SIZE;

                    if (index_count < DEVICE_MANAGER_MAX_CONNECTIONS)
                    {
                        DM_LOG("[DM]:[0x%02X]:[0x%02X]: Service context Event\r\n",
                               dm_handle.device_id,
                               dm_handle.connection_id);

                        //Notify application.
                        dm_event.event_id       = DM_EVT_SERVICE_CONTEXT_BASE;
                        dm_handle.connection_id = index_count;
                        dm_handle.service_id    = DM_PROTOCOL_CNTXT_GATT_SRVR_ID;

                        //Reset the service context now that it was successfully written to the
                        //application and the link is disconnected.
                        if ((m_connection_table[index_count].state & STATE_CONNECTED) !=
                            STATE_CONNECTED)
                        {
                            DM_LOG("[DM]:[0x%02X]:[0x%02X]: Resetting bond information for "
                                   "active instance.\r\n",
                                   dm_handle.device_id,
                                   dm_handle.connection_id);

                            memset(&m_gatts_table[dm_handle.connection_id],
                                   0,
                                   sizeof(dm_gatts_context_t));
                        }
                    }
                    else
                    {
                        DM_LOG("[DM]:[0x%02X]:[0x%02X]: App context Event\r\n",
                               dm_handle.device_id,
                               dm_handle.connection_id);

                        app_notify        = false;
                        dm_event.event_id = DM_EVT_APP_CONTEXT_BASE;
#if (DEVICE_MANAGER_APP_CONTEXT_SIZE != 0)

                        if (p_data == (uint8_t *)(&m_context_init_len))
                        {
                            //Context data is deleted.
                            //This is a workaround to get the right event as on delete operation
                            //update operation is used instead of clear.
                            op_code    = PSTORAGE_CLEAR_OP_CODE;
                            app_notify = true;
                        }
                        else if (m_app_context_table[dm_handle.device_id] == p_data)
                        {
                            app_notify                         = true;
                            dm_event.event_param.p_app_context = &context_data;

                            //Verify if the device is connected, if yes set connection instance.
                            for (uint32_t index = 0;
                                 index < DEVICE_MANAGER_MAX_CONNECTIONS;
                                 index++)
                            {
                                if (dm_handle.device_id == m_connection_table[index].bonded_dev_id)
                                {
                                    dm_handle.connection_id = index;
                                    break;
                                }
                            }
                        }
                        else
                        {
                            //No implementation needed.
                        }
#endif //DEVICE_MANAGER_APP_CONTEXT_SIZE
                    }
                }
            }
        }

        if (app_notify == true)
        {
            if (op_code == PSTORAGE_CLEAR_OP_CODE)
            {
                dm_event.event_id |= DM_CLEAR_OPERATION_ID;
            }
            else if (op_code == PSTORAGE_LOAD_OP_CODE)
            {
                dm_event.event_id |= DM_LOAD_OPERATION_ID;
            }
            else
            {
                dm_event.event_id |= DM_STORE_OPERATION_ID;
            }

            dm_event.event_param.p_app_context = &context_data;
            app_evt_notify(&dm_handle, &dm_event, result);
        }
    }

    DM_MUTEX_UNLOCK();
}


ret_code_t dm_init(dm_init_param_t const * const p_init_param)
{
    pstorage_module_param_t param;
    pstorage_handle_t       block_handle;
    ret_code_t              err_code;
    uint32_t                index;

    DM_LOG("[DM]: >> dm_init.\r\n");

    NULL_PARAM_CHECK(p_init_param);

    SDK_MUTEX_INIT(m_dm_mutex);

    DM_MUTEX_LOCK();

    for (index = 0; index < DEVICE_MANAGER_MAX_APPLICATIONS; index++)
    {
        application_instance_init(index);
    }

    for (index = 0; index < DEVICE_MANAGER_MAX_CONNECTIONS; index++)
    {
        connection_instance_init(index);
    }

    memset(m_gatts_table, 0, sizeof(m_gatts_table));

    //Initialization of all device instances.
    for (index = 0; index < DEVICE_MANAGER_MAX_BONDS; index++)
    {
        peer_instance_init(index);
        m_irk_index_table[index] = DM_INVALID_ID;
    }

    //All context with respect to a particular device is stored contiguously.
    param.block_size  = ALL_CONTEXT_SIZE;
    param.block_count = DEVICE_MANAGER_MAX_BONDS;
    param.cb          = dm_pstorage_cb_handler;

    err_code = pstorage_register(&param, &m_storage_handle);

    if (err_code == NRF_SUCCESS)
    {
        m_module_initialized = true;

        if (p_init_param->clear_persistent_data == false)
        {
            DM_LOG("[DM]: Storage handle 0x%08X.\r\n", m_storage_handle.block_id);

            //Copy bonded peer device address and IRK to RAM table.

            //Bonded devices are stored in range (0,DEVICE_MANAGER_MAX_BONDS-1). The remaining
            //range is for active connections that may or may not be bonded.
            for (index = 0; index < DEVICE_MANAGER_MAX_BONDS; index++)
            {
                err_code = pstorage_block_identifier_get(&m_storage_handle, index, &block_handle);

                //Issue read request if you successfully get the block identifier.
                if (err_code == NRF_SUCCESS)
                {
                    DM_TRC("[DM]:[0x%02X]: Block handle 0x%08X.\r\n", index, block_handle.block_id);

                    err_code = pstorage_load((uint8_t *)&m_peer_table[index],
                                             &block_handle,
                                             sizeof(peer_id_t),
                                             0);

                    if (err_code != NRF_SUCCESS)
                    {
                        // In case a peer device could not be loaded successfully, rest of the
                        // initialization procedure are skipped and an error is sent to the
                        // application.
                        DM_ERR(
                            "[DM]: Failed to load peer device %08X from storage, reason %08X.\r\n",
                            index,
                            err_code);

                        m_module_initialized = false;
                        break;
                    }
                    else
                    {
                        DM_TRC("[DM]:[DI 0x%02X]: Device type 0x%02X.\r\n",
                               index,
                               m_peer_table[index].peer_id.id_addr_info.addr_type);
                        DM_TRC("[DM]: Device Addr 0x%02X 0x%02X 0x%02X 0x%02X 0x%02X 0x%02X.\r\n",
                               m_peer_table[index].peer_id.id_addr_info.addr[0],
                               m_peer_table[index].peer_id.id_addr_info.addr[1],
                               m_peer_table[index].peer_id.id_addr_info.addr[2],
                               m_peer_table[index].peer_id.id_addr_info.addr[3],
                               m_peer_table[index].peer_id.id_addr_info.addr[4],
                               m_peer_table[index].peer_id.id_addr_info.addr[5]);
                    }
                }
                else
                {
                    //In case a peer device could not be loaded successfully, rest of the
                    //initialization procedure are skipped and an error is sent to the application.
                    DM_LOG("[DM]: Failed to get block handle for instance %08X, reason %08X.\r\n",
                           index,
                           err_code);

                    m_module_initialized = false;
                    break;
                }
            }
        }
        else
        {
            err_code = pstorage_clear(&m_storage_handle, (param.block_size * param.block_count));
            DM_ERR("[DM]: Successfully requested clear of persistent data.\r\n");
        }
    }
    else
    {
        DM_ERR("[DM]: Failed to register with storage module, reason 0x%08X.\r\n", err_code);
    }

    DM_MUTEX_UNLOCK();

    DM_TRC("[DM]: << dm_init.\r\n");

    return err_code;
}


ret_code_t dm_register(dm_application_instance_t    * p_appl_instance,
                       dm_application_param_t const * p_appl_param)
{
    VERIFY_MODULE_INITIALIZED();
    NULL_PARAM_CHECK(p_appl_instance);
    NULL_PARAM_CHECK(p_appl_param);
    NULL_PARAM_CHECK(p_appl_param->evt_handler);

    DM_MUTEX_LOCK();

    DM_LOG("[DM]: >> dm_register.\r\n");

    uint32_t err_code;

    //Verify if an application instance is available. Currently only one instance is supported.
    if (m_application_table[0].ntf_cb == NULL)
    {
        DM_LOG("[DM]: Application Instance allocated.\r\n");

        //Mark instance as allocated.
        m_application_table[0].ntf_cb    = p_appl_param->evt_handler;
        m_application_table[0].sec_param = p_appl_param->sec_param;
        m_application_table[0].service   = p_appl_param->service_type;

        m_application_table[0].sec_param.kdist_peer.enc  = 0;
        m_application_table[0].sec_param.kdist_peer.id   = 1;
        m_application_table[0].sec_param.kdist_peer.sign = 0;
        m_application_table[0].sec_param.kdist_own.enc   = 1;
        m_application_table[0].sec_param.kdist_own.id    = 1;
        m_application_table[0].sec_param.kdist_own.sign  = 0;
        //Populate application's instance variable with the assigned allocation instance.
        *p_appl_instance = 0;
        err_code         = NRF_SUCCESS;
    }
    else
    {
        err_code = (NRF_ERROR_NO_MEM | DEVICE_MANAGER_ERR_BASE);
    }

    DM_MUTEX_UNLOCK();

    DM_TRC("[DM]: << dm_register.\r\n");

    return err_code;
}


ret_code_t dm_security_setup_req(dm_handle_t * p_handle)
{
    VERIFY_MODULE_INITIALIZED();
    NULL_PARAM_CHECK(p_handle);
    VERIFY_APP_REGISTERED(p_handle->appl_id);
    VERIFY_CONNECTION_INSTANCE(p_handle->connection_id);

    DM_MUTEX_LOCK();

    DM_LOG("[DM]: >> dm_security_setup_req\r\n");

    uint32_t err_code = (NRF_ERROR_INVALID_STATE | DEVICE_MANAGER_ERR_BASE);

    if ((m_connection_table[p_handle->connection_id].state & STATE_CONNECTED) == STATE_CONNECTED)
    {
        err_code = sd_ble_gap_authenticate(m_connection_table[p_handle->connection_id].conn_handle,
                                           &m_application_table[0].sec_param);
    }

    DM_TRC("[DM]: << dm_security_setup_req, 0x%08X\r\n", err_code);

    DM_MUTEX_UNLOCK();

    return err_code;
}


ret_code_t dm_security_status_req(dm_handle_t const    * p_handle,
                                  dm_security_status_t * p_status)
{
    VERIFY_MODULE_INITIALIZED();
    NULL_PARAM_CHECK(p_handle);
    NULL_PARAM_CHECK(p_status);
    VERIFY_APP_REGISTERED(p_handle->appl_id);
    VERIFY_CONNECTION_INSTANCE(p_handle->connection_id);

    DM_MUTEX_LOCK();

    DM_LOG("[DM]: >> dm_security_status_req\r\n");

    if ((m_connection_table[p_handle->connection_id].state & STATE_PAIRING) ||
        (m_connection_table[p_handle->connection_id].state & STATE_PAIRING_PENDING))
    {
        (*p_status) = ENCRYPTION_IN_PROGRESS;
    }
    else if (m_connection_table[p_handle->connection_id].state & STATE_LINK_ENCRYPTED)
    {
        (*p_status) = ENCRYPTED;
    }
    else
    {
        (*p_status) = NOT_ENCRYPTED;
    }

    DM_TRC("[DM]: << dm_security_status_req\r\n");

    DM_MUTEX_UNLOCK();

    return NRF_SUCCESS;
}


ret_code_t dm_whitelist_create(dm_application_instance_t const * p_handle,
                               ble_gap_whitelist_t             * p_whitelist)
{
    VERIFY_MODULE_INITIALIZED();
    NULL_PARAM_CHECK(p_handle);
    NULL_PARAM_CHECK(p_whitelist);
    NULL_PARAM_CHECK(p_whitelist->pp_addrs);
    NULL_PARAM_CHECK(p_whitelist->pp_irks);
    VERIFY_APP_REGISTERED(*p_handle);

    DM_MUTEX_LOCK();

    DM_LOG("[DM]: >> dm_whitelist_create\r\n");

    uint32_t addr_count = 0;
    uint32_t irk_count  = 0;
    bool     connected  = false;

    for (uint32_t index = 0; index < DEVICE_MANAGER_MAX_BONDS; index++)
    {
        connected = false;

        for (uint32_t c_index = 0; c_index < DEVICE_MANAGER_MAX_CONNECTIONS; c_index++)
        {
            if ((index == m_connection_table[c_index].bonded_dev_id) &&
                ((m_connection_table[c_index].state & STATE_CONNECTED) == STATE_CONNECTED))
            {
                connected = true;
                break;
            }
        }

        if (connected == false)
        {
            if ((irk_count < p_whitelist->irk_count) &&
                ((m_peer_table[index].id_bitmap & IRK_ENTRY) == 0))
            {
                p_whitelist->pp_irks[irk_count] = &m_peer_table[index].peer_id.id_info;
                m_irk_index_table[irk_count]    = index;
                irk_count++;
            }

            if ((addr_count < p_whitelist->addr_count) &&
                (m_peer_table[index].id_bitmap & ADDR_ENTRY) == 0)
            {
                p_whitelist->pp_addrs[addr_count] = &m_peer_table[index].peer_id.id_addr_info;
                addr_count++;
            }
        }
    }

    p_whitelist->addr_count = addr_count;
    p_whitelist->irk_count  = irk_count;

    DM_LOG("[DM]: Created whitelist, number of IRK = 0x%02X, number of addr = 0x%02X\r\n",
           irk_count,
           addr_count);

    DM_TRC("[DM]: << dm_whitelist_create\r\n");

    DM_MUTEX_UNLOCK();

    return NRF_SUCCESS;
}


ret_code_t dm_device_add(dm_handle_t               * p_handle,
                         dm_device_context_t const * p_context)
{
    return (API_NOT_IMPLEMENTED | DEVICE_MANAGER_ERR_BASE);
}


ret_code_t dm_device_delete(dm_handle_t const * p_handle)
{
    VERIFY_MODULE_INITIALIZED();
    NULL_PARAM_CHECK(p_handle);
    VERIFY_APP_REGISTERED(p_handle->appl_id);
    VERIFY_DEVICE_INSTANCE(p_handle->device_id);

    DM_MUTEX_LOCK();

    DM_TRC("[DM]: >> dm_device_delete\r\n");

    uint32_t err_code = device_instance_free(p_handle->device_id);

    DM_TRC("[DM]: << dm_device_delete\r\n");

    DM_MUTEX_UNLOCK();

    return err_code;
}


ret_code_t dm_device_delete_all(dm_application_instance_t const * p_handle)
{
    VERIFY_MODULE_INITIALIZED();
    NULL_PARAM_CHECK(p_handle);
    VERIFY_APP_REGISTERED((*p_handle));

    DM_MUTEX_LOCK();

    uint32_t err_code = NRF_SUCCESS;

    DM_TRC("[DM]: >> dm_device_delete_all\r\n");

    for (uint32_t index = 0; index < DEVICE_MANAGER_MAX_BONDS; index++)
    {
        if (m_peer_table[index].id_bitmap != UNASSIGNED)
        {
            err_code = device_instance_free(index);
        }
    }

    DM_TRC("[DM]: << dm_device_delete_all\r\n");

    DM_MUTEX_UNLOCK();

    return err_code;
}


ret_code_t dm_service_context_set(dm_handle_t const          * p_handle,
                                  dm_service_context_t const * p_context)
{
    VERIFY_MODULE_INITIALIZED();
    NULL_PARAM_CHECK(p_handle);
    NULL_PARAM_CHECK(p_context);
    VERIFY_APP_REGISTERED(p_handle->appl_id);
    VERIFY_CONNECTION_INSTANCE(p_handle->connection_id);
    VERIFY_DEVICE_INSTANCE(p_handle->device_id);

    DM_MUTEX_LOCK();

    DM_TRC("[DM]: >> dm_service_context_set\r\n");

    if ((p_context->context_data.p_data != NULL) &&
        (
            (p_context->context_data.len != 0) &&
            (p_context->context_data.len < DM_GATT_SERVER_ATTR_MAX_SIZE)
        )
       )
    {
        if (p_context->service_type == DM_PROTOCOL_CNTXT_GATT_SRVR_ID)
        {
            memcpy(m_gatts_table[p_handle->connection_id].attributes,
                   p_context->context_data.p_data,
                   p_context->context_data.len);
        }
    }

    pstorage_handle_t block_handle;
    uint32_t          err_code = pstorage_block_identifier_get(&m_storage_handle,
                                                               p_handle->device_id,
                                                               &block_handle);

    err_code = m_service_context_store[p_context->service_type](&block_handle, p_handle);

    DM_TRC("[DM]: << dm_service_context_set\r\n");

    DM_MUTEX_UNLOCK();

    return err_code;
}


ret_code_t dm_service_context_get(dm_handle_t const    * p_handle,
                                  dm_service_context_t * p_context)
{
    VERIFY_MODULE_INITIALIZED();
    NULL_PARAM_CHECK(p_handle);
    NULL_PARAM_CHECK(p_context);
    VERIFY_APP_REGISTERED(p_handle->appl_id);
    VERIFY_DEVICE_INSTANCE(p_handle->device_id);

    if ((m_connection_table[p_handle->connection_id].state & STATE_CONNECTED) != STATE_CONNECTED)
    {
        DM_TRC("[DM]: Device must be connected to get context. \r\n");

        return (FEATURE_NOT_ENABLED | DEVICE_MANAGER_ERR_BASE);
    }

    DM_MUTEX_LOCK();

    DM_TRC("[DM]: >> dm_service_context_get\r\n");

    if (p_context->service_type == DM_PROTOCOL_CNTXT_GATT_SRVR_ID)
    {
        p_context->context_data.p_data = m_gatts_table[p_handle->connection_id].attributes;
        p_context->context_data.len    = m_gatts_table[p_handle->connection_id].size;
    }

    pstorage_handle_t block_handle;
    uint32_t          err_code = pstorage_block_identifier_get(&m_storage_handle,
                                                               p_handle->device_id,
                                                               &block_handle);

    err_code = m_service_context_load[p_context->service_type](&block_handle, p_handle);

    if (p_context->service_type == DM_PROTOCOL_CNTXT_GATT_SRVR_ID)
    {
        p_context->context_data.p_data = m_gatts_table[p_handle->connection_id].attributes;
        p_context->context_data.len    = m_gatts_table[p_handle->connection_id].size;
    }

    DM_TRC("[DM]: << dm_service_context_get\r\n");

    DM_MUTEX_UNLOCK();

    return err_code;
}


ret_code_t dm_service_context_delete(dm_handle_t const * p_handle)
{
    VERIFY_MODULE_INITIALIZED();
    NULL_PARAM_CHECK(p_handle);
    VERIFY_APP_REGISTERED(p_handle->appl_id);
    VERIFY_DEVICE_INSTANCE(p_handle->device_id);

    DM_LOG("[DM]: Context delete is not supported yet.\r\n");

    return (API_NOT_IMPLEMENTED | DEVICE_MANAGER_ERR_BASE);
}


ret_code_t dm_application_context_set(dm_handle_t const              * p_handle,
                                      dm_application_context_t const * p_context)
{
#if (DEVICE_MANAGER_APP_CONTEXT_SIZE != 0)
    VERIFY_MODULE_INITIALIZED();
    NULL_PARAM_CHECK(p_handle);
    NULL_PARAM_CHECK(p_context);
    NULL_PARAM_CHECK(p_context->p_data);
    VERIFY_APP_REGISTERED(p_handle->appl_id);
    VERIFY_DEVICE_INSTANCE(p_handle->device_id);
    VERIFY_DEVICE_BOND(p_handle->connection_id);
    SIZE_CHECK_APP_CONTEXT(p_context->len);

    DM_MUTEX_LOCK();

    DM_TRC("[DM]: >> dm_application_context_set\r\n");

    uint32_t          err_code;
    uint32_t          context_len;
    pstorage_handle_t block_handle;

    storage_operation store_fn = pstorage_store;

    err_code = pstorage_block_identifier_get(&m_storage_handle,
                                             p_handle->device_id,
                                             &block_handle);

    if (err_code == NRF_SUCCESS)
    {
        err_code = pstorage_load((uint8_t *)&context_len,
                                 &block_handle,
                                 sizeof(uint32_t),
                                 APP_CONTEXT_STORAGE_OFFSET);

        if ((err_code == NRF_SUCCESS) && (context_len != INVALID_CONTEXT_LEN))
        {
            //Data already exists. Need an update.
            store_fn = pstorage_update;

            DM_LOG("[DM]:[DI 0x%02X]: Updating existing application context, existing len 0x%08X, "
                   "new length 0x%08X.\r\n",
                   p_handle->device_id,
                   context_len,
                   p_context->len);
        }
        else
        {
            DM_LOG("[DM]: Storing application context.\r\n");
        }

        //Store/update context length.
        err_code = store_fn(&block_handle,
                            (uint8_t *)(&p_context->len),
                            sizeof(uint32_t),
                            APP_CONTEXT_STORAGE_OFFSET);

        if (err_code == NRF_SUCCESS)
        {
            //Update context data is used for application context as flash is never
            //cleared if a delete of application context is called.
            err_code = pstorage_update(&block_handle,
                                       p_context->p_data,
                                       DEVICE_MANAGER_APP_CONTEXT_SIZE,
                                       (APP_CONTEXT_STORAGE_OFFSET + sizeof(uint32_t)));
            if (err_code == NRF_SUCCESS)
            {
                m_app_context_table[p_handle->device_id] = p_context->p_data;
            }
        }
    }

    DM_TRC("[DM]: << dm_application_context_set\r\n");

    DM_MUTEX_UNLOCK();

    return err_code;

#else //DEVICE_MANAGER_APP_CONTEXT_SIZE
    return (FEATURE_NOT_ENABLED | DEVICE_MANAGER_ERR_BASE);
#endif //DEVICE_MANAGER_APP_CONTEXT_SIZE
}


ret_code_t dm_application_context_get(dm_handle_t const        * p_handle,
                                      dm_application_context_t * p_context)
{
#if (DEVICE_MANAGER_APP_CONTEXT_SIZE != 0)
    VERIFY_MODULE_INITIALIZED();
    NULL_PARAM_CHECK(p_handle);
    NULL_PARAM_CHECK(p_context);
    VERIFY_APP_REGISTERED(p_handle->appl_id);
    VERIFY_DEVICE_INSTANCE(p_handle->device_id);

    DM_MUTEX_LOCK();

    DM_TRC("[DM]: >> dm_application_context_get\r\n");

    uint32_t          context_len;
    uint32_t          err_code;
    pstorage_handle_t block_handle;

    //Check if the context exists.
    if (NULL == p_context->p_data)
    {
        p_context->p_data = m_app_context_table[p_handle->device_id];
    }
    else
    {
        m_app_context_table[p_handle->device_id] = p_context->p_data;
    }

    err_code = pstorage_block_identifier_get(&m_storage_handle,
                                             p_handle->device_id,
                                             &block_handle);

    if (err_code == NRF_SUCCESS)
    {
        err_code = pstorage_load((uint8_t *)&context_len,
                                 &block_handle,
                                 sizeof(uint32_t),
                                 APP_CONTEXT_STORAGE_OFFSET);

        if ((err_code == NRF_SUCCESS) && (context_len != INVALID_CONTEXT_LEN))
        {
            err_code = pstorage_load(p_context->p_data,
                                     &block_handle,
                                     DEVICE_MANAGER_APP_CONTEXT_SIZE,
                                     (APP_CONTEXT_STORAGE_OFFSET + sizeof(uint32_t)));
            if (err_code == NRF_SUCCESS)
            {
                p_context->len = context_len;
            }
        }
        else
        {
            err_code = DM_NO_APP_CONTEXT;
        }
    }

    DM_TRC("[DM]: << dm_application_context_get\r\n");

    DM_MUTEX_UNLOCK();

    return err_code;

#else //DEVICE_MANAGER_APP_CONTEXT_SIZE
    return (FEATURE_NOT_ENABLED | DEVICE_MANAGER_ERR_BASE);
#endif //DEVICE_MANAGER_APP_CONTEXT_SIZE
}


ret_code_t dm_application_context_delete(const dm_handle_t * p_handle)
{
#if (DEVICE_MANAGER_APP_CONTEXT_SIZE != 0)
    VERIFY_MODULE_INITIALIZED();
    NULL_PARAM_CHECK(p_handle);
    VERIFY_APP_REGISTERED(p_handle->appl_id);
    VERIFY_DEVICE_INSTANCE(p_handle->device_id);

    DM_MUTEX_LOCK();

    DM_TRC("[DM]: >> dm_application_context_delete\r\n");

    uint32_t          err_code;
    uint32_t          context_len;
    pstorage_handle_t block_handle;

    err_code = pstorage_block_identifier_get(&m_storage_handle,
                                             p_handle->device_id,
                                             &block_handle);

    if (err_code == NRF_SUCCESS)
    {
        err_code = pstorage_load((uint8_t *)&context_len,
                                 &block_handle,
                                 sizeof(uint32_t),
                                 APP_CONTEXT_STORAGE_OFFSET);

        if (context_len != m_context_init_len)
        {
            err_code = pstorage_update(&block_handle,
                                       (uint8_t *)&m_context_init_len,
                                       sizeof(uint32_t),
                                       APP_CONTEXT_STORAGE_OFFSET);

            if (err_code != NRF_SUCCESS)
            {
                DM_ERR("[DM]: Failed to delete application context, reason 0x%08X\r\n", err_code);
            }
            else
            {
                m_app_context_table[p_handle->device_id] = NULL;
            }
        }
    }

    DM_TRC("[DM]: << dm_application_context_delete\r\n");

    DM_MUTEX_UNLOCK();

    return err_code;
#else //DEVICE_MANAGER_APP_CONTEXT_SIZE
    return (FEATURE_NOT_ENABLED | DEVICE_MANAGER_ERR_BASE);
#endif //DEVICE_MANAGER_APP_CONTEXT_SIZE
}


ret_code_t dm_application_instance_set(dm_application_instance_t const * p_appl_instance,
                                       dm_handle_t                     * p_handle)
{
    VERIFY_MODULE_INITIALIZED();
    NULL_PARAM_CHECK(p_handle);
    NULL_PARAM_CHECK(p_appl_instance);
    VERIFY_APP_REGISTERED((*p_appl_instance));

    p_handle->appl_id = (*p_appl_instance);

    return NRF_SUCCESS;
}


uint32_t dm_handle_initialize(dm_handle_t * p_handle)
{
    NULL_PARAM_CHECK(p_handle);

    p_handle->appl_id       = DM_INVALID_ID;
    p_handle->connection_id = DM_INVALID_ID;
    p_handle->device_id     = DM_INVALID_ID;
    p_handle->service_id    = DM_INVALID_ID;

    return NRF_SUCCESS;
}


ret_code_t dm_peer_addr_set(dm_handle_t const    * p_handle,
                            ble_gap_addr_t const * p_addr)
{
    VERIFY_MODULE_INITIALIZED();
    NULL_PARAM_CHECK(p_handle);
    NULL_PARAM_CHECK(p_addr);
    VERIFY_APP_REGISTERED(p_handle->appl_id);
    VERIFY_DEVICE_INSTANCE(p_handle->device_id);

    DM_MUTEX_LOCK();

    DM_TRC("[DM]: >> dm_peer_addr_set\r\n");

    ret_code_t err_code;

    if ((p_handle->connection_id == DM_INVALID_ID) &&
        (p_addr->addr_type != BLE_GAP_ADDR_TYPE_RANDOM_PRIVATE_RESOLVABLE))
    {
        m_peer_table[p_handle->device_id].peer_id.id_addr_info = (*p_addr);
        update_status_bit_set(p_handle->device_id);
        device_context_store(p_handle, UPDATE_PEER_ADDR);
        err_code = NRF_SUCCESS;
    }
    else
    {
        err_code = (NRF_ERROR_INVALID_PARAM | DEVICE_MANAGER_ERR_BASE);
    }

    DM_TRC("[DM]: << dm_peer_addr_set\r\n");

    DM_MUTEX_UNLOCK();

    return err_code;
}


ret_code_t dm_peer_addr_get(dm_handle_t const * p_handle,
                            ble_gap_addr_t    * p_addr)
{
    VERIFY_MODULE_INITIALIZED();
    NULL_PARAM_CHECK(p_handle);
    NULL_PARAM_CHECK(p_addr);
    VERIFY_APP_REGISTERED(p_handle->appl_id);

    DM_MUTEX_LOCK();

    DM_TRC("[DM]: >> dm_peer_addr_get\r\n");

    ret_code_t err_code;

    err_code = (NRF_ERROR_NOT_FOUND | DEVICE_MANAGER_ERR_BASE);

    if (p_handle->device_id == DM_INVALID_ID)
    {
        if ((p_handle->connection_id != DM_INVALID_ID) &&
            ((m_connection_table[p_handle->connection_id].state & STATE_CONNECTED) ==
             STATE_CONNECTED))
        {
            DM_TRC("[DM]:[CI 0x%02X]: Address get for non bonded active connection.\r\n",
                   p_handle->connection_id);

            (*p_addr) = m_connection_table[p_handle->connection_id].peer_addr;
            err_code  = NRF_SUCCESS;
        }
    }
    else
    {
        if ((m_peer_table[p_handle->device_id].id_bitmap & ADDR_ENTRY) == 0)
        {
            DM_TRC("[DM]:[DI 0x%02X]: Address get for bonded device.\r\n",
                   p_handle->device_id);

            (*p_addr) = m_peer_table[p_handle->device_id].peer_id.id_addr_info;
            err_code  = NRF_SUCCESS;
        }
    }

    DM_TRC("[DM]: << dm_peer_addr_get\r\n");

    DM_MUTEX_UNLOCK();

    return err_code;
}


ret_code_t dm_distributed_keys_get(dm_handle_t const * p_handle,
                                   dm_sec_keyset_t   * p_key_dist)
{
    VERIFY_MODULE_INITIALIZED();
    NULL_PARAM_CHECK(p_handle);
    NULL_PARAM_CHECK(p_key_dist);
    VERIFY_APP_REGISTERED(p_handle->appl_id);
    VERIFY_DEVICE_INSTANCE(p_handle->device_id);

    DM_MUTEX_LOCK();

    DM_TRC("[DM]: >> dm_distributed_keys_get\r\n");

    ret_code_t        err_code;
    ble_gap_enc_key_t peer_enc_key;
    pstorage_handle_t block_handle;

    err_code                                   = NRF_ERROR_NOT_FOUND;
    p_key_dist->keys_central.enc_key.p_enc_key = NULL;
    p_key_dist->keys_central.p_id_key          = (dm_id_key_t *)&m_peer_table[p_handle->device_id].peer_id;
    p_key_dist->keys_central.p_sign_key        = NULL;
    p_key_dist->keys_periph.p_id_key           = (dm_id_key_t *)&m_local_id_info;
    p_key_dist->keys_periph.p_sign_key         = NULL;
    p_key_dist->keys_periph.enc_key.p_enc_key  = (dm_enc_key_t *)&peer_enc_key;

    if ((m_peer_table[p_handle->device_id].id_bitmap & IRK_ENTRY) == 0)
    {
//        p_key_dist->keys_periph.p_id_key->id_addr_info.addr_type = INVALID_ADDR_TYPE;
    }

    err_code = pstorage_block_identifier_get(&m_storage_handle, p_handle->device_id, &block_handle);
    if (err_code == NRF_SUCCESS)
    {

        err_code = pstorage_load((uint8_t *)&peer_enc_key,
                                 &block_handle,
                                 BOND_SIZE,
                                 BOND_STORAGE_OFFSET);

        if (err_code == NRF_SUCCESS)
        {
            p_key_dist->keys_central.enc_key.p_enc_key = NULL;
            p_key_dist->keys_central.p_id_key          = (dm_id_key_t *)&m_peer_table[p_handle->device_id].peer_id;
            p_key_dist->keys_central.p_sign_key        = NULL;
            p_key_dist->keys_periph.p_id_key           = (dm_id_key_t *)&m_local_id_info;
            p_key_dist->keys_periph.p_sign_key         = NULL;
            p_key_dist->keys_periph.enc_key.p_enc_key  = (dm_enc_key_t *)&peer_enc_key;
        }
    }

    DM_TRC("[DM]: << dm_distributed_keys_get\r\n");

    DM_MUTEX_UNLOCK();

    return err_code;
}


/**@brief Function for loading bond information for a connection instance.
 */
void bond_data_load(dm_handle_t * p_handle)
{
    pstorage_handle_t block_handle;

    uint32_t err_code = pstorage_block_identifier_get(&m_storage_handle,
                                                       p_handle->device_id,
                                                       &block_handle);
    if (err_code == NRF_SUCCESS)
    {
        DM_LOG(
            "[DM]:[%02X]:[Block ID 0x%08X]:Loading bond information at %p, size 0x%08X, offset 0x%08X.\r\n",
            p_handle->connection_id,
            block_handle.block_id,
            &m_bond_table[p_handle->connection_id],
            BOND_SIZE,
            BOND_STORAGE_OFFSET);

        err_code = pstorage_load((uint8_t *)&m_bond_table[p_handle->connection_id],
                                 &block_handle,
                                 BOND_SIZE,
                                 BOND_STORAGE_OFFSET);

        if (err_code != NRF_SUCCESS)
        {
            DM_ERR("[DM]:[%02X]: Failed to load Bond information, reason %08X\r\n",
                   p_handle->connection_id,
                   err_code);
        }

        DM_LOG(
            "[DM]:[%02X]:Loading service context at %p, size 0x%08X, offset 0x%08X.\r\n",
            p_handle->connection_id,
            &m_gatts_table[p_handle->connection_id],
            sizeof(dm_gatts_context_t),
            SERVICE_STORAGE_OFFSET);

        err_code = m_service_context_load[m_application_table[0].service](
            &block_handle,
            p_handle);

        if (err_code != NRF_SUCCESS)
        {
            DM_ERR(
                "[DM]:[%02X]: Failed to load service information, reason %08X\r\n",
                p_handle->connection_id,
                err_code);
        }
    }
    else
    {
        DM_ERR("[DM]:[%02X]: Failed to get block identifier for "
               "device %08X, reason %08X.\r\n", p_handle->connection_id, p_handle->device_id, err_code);
    }
}


void dm_ble_evt_handler(ble_evt_t * p_ble_evt)
{
    uint32_t    err_code;
    uint32_t    index;
    uint32_t    device_index        = DM_INVALID_ID;
    bool        notify_app          = false;
    dm_handle_t handle;
    dm_event_t  event;
    uint32_t    event_result;
    ble_gap_enc_info_t * p_enc_info = NULL;

    VERIFY_MODULE_INITIALIZED_VOID();
    VERIFY_APP_REGISTERED_VOID(0);
    DM_MUTEX_LOCK();

    err_code = dm_handle_initialize(&handle);
    APP_ERROR_CHECK(err_code);

    event_result                    = NRF_SUCCESS;
    err_code                        = NRF_SUCCESS;
    event.event_param.p_gap_param   = &p_ble_evt->evt.gap_evt;
    event.event_paramlen            = sizeof(ble_gap_evt_t);
    handle.device_id                = DM_INVALID_ID;
    handle.appl_id                  = 0;
    index                           = 0x00;

    if (p_ble_evt->header.evt_id != BLE_GAP_EVT_CONNECTED)
    {
        err_code = connection_instance_find(p_ble_evt->evt.gap_evt.conn_handle,
                                            STATE_CONNECTED,
                                            &index);

        if (err_code == NRF_SUCCESS)
        {
            handle.device_id     = m_connection_table[index].bonded_dev_id;
            handle.connection_id = index;
        }
    }

    switch (p_ble_evt->header.evt_id)
    {
        case BLE_GAP_EVT_CONNECTED:
            //Allocate connection instance for a new connection.
            err_code = connection_instance_allocate(&index);

            //Connection instance is successfully allocated.
            if (err_code == NRF_SUCCESS)
            {
                //Application notification related information.
                notify_app           = true;
                event.event_id       = DM_EVT_CONNECTION;
                handle.connection_id = index;

                m_connection_table[index].conn_handle = p_ble_evt->evt.gap_evt.conn_handle;
                m_connection_table[index].state       = STATE_CONNECTED;
                m_connection_table[index].peer_addr   =
                    p_ble_evt->evt.gap_evt.params.connected.peer_addr;

                if (p_ble_evt->evt.gap_evt.params.connected.irk_match == 1)
                {
                    if (m_irk_index_table[p_ble_evt->evt.gap_evt.params.connected.irk_match_idx] != DM_INVALID_ID)
                    {
                        device_index = m_irk_index_table[p_ble_evt->evt.gap_evt.params.connected.irk_match_idx];
                        err_code = NRF_SUCCESS;
                    }
                }
                else
                {
                    //Use the device address to check if the device exists in the bonded device list.
                    err_code = device_instance_find(&p_ble_evt->evt.gap_evt.params.connected.peer_addr,
                                                    &device_index, EDIV_INIT_VAL);
                }

                if (err_code == NRF_SUCCESS)
                {
                    m_connection_table[index].bonded_dev_id = device_index;
                    m_connection_table[index].state        |= STATE_BONDED;
                    handle.device_id                        = device_index;

                    bond_data_load(&handle);
                }
            }
            break;

        case BLE_GAP_EVT_DISCONNECTED:
            //Disconnection could be peer or self initiated hence disconnecting and connecting
            //both states are permitted, however, connection handle must be known.
            DM_LOG("[DM]: Disconnect Reason 0x%04X\r\n",
                   p_ble_evt->evt.gap_evt.params.disconnected.reason);

            m_connection_table[index].state &= (~STATE_CONNECTED);

            if ((m_connection_table[index].state & STATE_BONDED) == STATE_BONDED)
            {
                if ((m_connection_table[index].state & STATE_LINK_ENCRYPTED) == STATE_LINK_ENCRYPTED)
                {
                    //Write bond information persistently.
                    device_context_store(&handle, STORE_ALL_CONTEXT);
                }
            }
            else
            {
                //Free any allocated instances for devices that is not bonded.
                if (handle.device_id != DM_INVALID_ID)
                {
                    peer_instance_init(handle.device_id);
                    handle.device_id = DM_INVALID_ID;
                }
            }

            m_connection_table[index].state = STATE_DISCONNECTING;
            notify_app                      = true;
            event.event_id                  = DM_EVT_DISCONNECTION;

            break;

        case BLE_GAP_EVT_SEC_INFO_REQUEST:
            DM_LOG("[DM]: >> BLE_GAP_EVT_SEC_INFO_REQUEST\r\n");

            //If the device is already bonded, respond with existing info, else NULL.
            if (m_connection_table[index].bonded_dev_id == DM_INVALID_ID)
            {
                //Find device based on div.
                err_code = device_instance_find(NULL,&device_index, p_ble_evt->evt.gap_evt.params.sec_info_request.master_id.ediv);
                if (err_code == NRF_SUCCESS)
                {
                    //Load needed bonding information.
                    m_connection_table[index].bonded_dev_id = device_index;
                    m_connection_table[index].state        |= STATE_BONDED;
                    handle.device_id                        = device_index;
                    bond_data_load(&handle);
                }
            }

            if (m_connection_table[index].bonded_dev_id != DM_INVALID_ID)
            {
                p_enc_info = &m_bond_table[index].peer_enc_key.enc_info;
                DM_DUMP((uint8_t *)p_enc_info, sizeof(ble_gap_enc_info_t));
            }

            err_code = sd_ble_gap_sec_info_reply(p_ble_evt->evt.gap_evt.conn_handle,
                                                 p_enc_info,
                                                 &m_peer_table[index].peer_id.id_info,
                                                 NULL);

            if (err_code != NRF_SUCCESS)
            {
                DM_ERR("[DM]:[CI %02X]:[DI %02X]: Security information response failed, reason "
                       "0x%08X\r\n", index, m_connection_table[index].bonded_dev_id, err_code);
            }
            break;

        case BLE_GAP_EVT_SEC_PARAMS_REQUEST:
            DM_LOG("[DM]: >> BLE_GAP_EVT_SEC_PARAMS_REQUEST\r\n");

            event.event_id = DM_EVT_SECURITY_SETUP;

            m_connection_table[index].state |= STATE_PAIRING;
            notify_app                       = true;

            if (m_connection_table[index].bonded_dev_id == DM_INVALID_ID)
            {
                //Assign a peer index as a new bond or update existing bonds.
                err_code = device_instance_allocate((uint8_t *)&device_index,
                                                    &m_connection_table[index].peer_addr);

                //Allocation successful.
                if (err_code == NRF_SUCCESS)
                {
                    DM_LOG("[DM]:[CI 0x%02X]:[DI 0x%02X]: Bonded!\r\n",index, device_index);

                    handle.device_id                        = device_index;
                    m_connection_table[index].bonded_dev_id = device_index;
                }
                else
                {
                    DM_LOG("[DM]: Security parameter request failed, reason 0x%08X.\r\n", err_code);
                    event_result = err_code;
                    notify_app   = true;
                }

                ble_gap_sec_keyset_t keys_exchanged;

                DM_LOG("[DM]: 0x%02X, 0x%02X, 0x%02X, 0x%02X\r\n",
                       p_ble_evt->evt.gap_evt.params.sec_params_request.peer_params.kdist_peer.enc,
                       p_ble_evt->evt.gap_evt.params.sec_params_request.peer_params.kdist_own.id,
                       p_ble_evt->evt.gap_evt.params.sec_params_request.peer_params.kdist_peer.sign,
                       p_ble_evt->evt.gap_evt.params.sec_params_request.peer_params.bond);

                keys_exchanged.keys_peer.p_enc_key  = NULL;
                keys_exchanged.keys_peer.p_id_key   = &m_peer_table[m_connection_table[index].bonded_dev_id].peer_id;
                keys_exchanged.keys_peer.p_sign_key = NULL;
                keys_exchanged.keys_peer.p_pk       = NULL;
                keys_exchanged.keys_own.p_enc_key   = &m_bond_table[index].peer_enc_key;
                keys_exchanged.keys_own.p_id_key    = NULL;
                keys_exchanged.keys_own.p_sign_key  = NULL;
                keys_exchanged.keys_own.p_pk        = NULL;

                err_code = sd_ble_gap_sec_params_reply(p_ble_evt->evt.gap_evt.conn_handle,
                                                       BLE_GAP_SEC_STATUS_SUCCESS,
                                                       &m_application_table[0].sec_param,
                                                       &keys_exchanged);

                if (err_code != NRF_SUCCESS)
                {
                    DM_LOG("[DM]: Security parameter reply request failed, reason 0x%08X.\r\n", err_code);
                    event_result = err_code;
                    notify_app   = false;
                }

            }
            else
            {
                //Bond/key refresh.
                DM_LOG("[DM]: !!! Bond/key refresh !!!\r\n");
                //Set the update flag for bond data.
                m_connection_table[index].state |= STATE_BOND_INFO_UPDATE;
                event.event_id                   = DM_EVT_SECURITY_SETUP_REFRESH;

                err_code = sd_ble_gap_sec_params_reply(p_ble_evt->evt.gap_evt.conn_handle,
                                                       BLE_GAP_SEC_STATUS_PAIRING_NOT_SUPP,
                                                       NULL,
                                                       NULL);

                if (err_code != NRF_SUCCESS)
                {
                    DM_LOG("[DM]: Security parameter reply request failed, reason 0x%08X.\r\n", err_code);
                    event_result = err_code;
                    notify_app   = false;
                }

            }
            break;

        case BLE_GAP_EVT_AUTH_STATUS:
        {
            DM_LOG("[DM]: >> BLE_GAP_EVT_AUTH_STATUS, status %08X\r\n",
                   p_ble_evt->evt.gap_evt.params.auth_status.auth_status);

            m_application_table[0].state    &= (~STATE_CONTROL_PROCEDURE_IN_PROGRESS);
            m_connection_table[index].state &= (~STATE_PAIRING);
            event.event_id                   = DM_EVT_SECURITY_SETUP_COMPLETE;
            notify_app                       = true;

            if (p_ble_evt->evt.gap_evt.params.auth_status.auth_status != BLE_GAP_SEC_STATUS_SUCCESS)
            {
                // In case of key refresh attempt, since this behavior is now rejected, we don't do anything here
                if ((m_connection_table[index].state & STATE_BOND_INFO_UPDATE)
                    != STATE_BOND_INFO_UPDATE)
                {
                    // Free the allocation as bonding failed.
                    ret_code_t result = device_instance_free(m_connection_table[index].bonded_dev_id);
                    (void) result;
                    event_result = p_ble_evt->evt.gap_evt.params.auth_status.auth_status;
                }
            }
            else
            {
                DM_DUMP((uint8_t *)&p_ble_evt->evt.gap_evt.params.auth_status,
                        sizeof(ble_gap_evt_auth_status_t));
                DM_DUMP((uint8_t *)&m_bond_table[index], sizeof(bond_context_t));

                if (p_ble_evt->evt.gap_evt.params.auth_status.bonded == 1)
                {
                    if (handle.device_id != DM_INVALID_ID)
                    {
                        m_connection_table[index].state |= STATE_BONDED;

                        //IRK and/or public address is shared, update it.
                        if (p_ble_evt->evt.gap_evt.params.auth_status.kdist_peer.id == 1)
                        {
                            m_peer_table[handle.device_id].id_bitmap &= (~IRK_ENTRY);
                        }

                        if (m_connection_table[index].bonded_dev_id != DM_INVALID_ID)
                        {
                            DM_LOG("[DM]:[CI 0x%02X]:[DI 0x%02X]: Bonded!\r\n",
                                   index,
                                   handle.device_id);

                            if (m_connection_table[index].peer_addr.addr_type !=
                                BLE_GAP_ADDR_TYPE_RANDOM_PRIVATE_RESOLVABLE)
                            {
                               m_peer_table[handle.device_id].peer_id.id_addr_info =
                                    m_connection_table[index].peer_addr;
                               m_peer_table[handle.device_id].id_bitmap &= (~ADDR_ENTRY);

                               DM_DUMP((uint8_t *)&m_peer_table[handle.device_id].peer_id.id_addr_info,
                                       sizeof(m_peer_table[handle.device_id].peer_id.id_addr_info));
                            }
                            else
                            {
                                // Here we must fetch the keys from the keyset distributed.
                                m_peer_table[handle.device_id].ediv       = m_bond_table[index].peer_enc_key.master_id.ediv;
                                m_peer_table[handle.device_id].id_bitmap &= (~IRK_ENTRY);
                            }

                            device_context_store(&handle, FIRST_BOND_STORE);
                        }
                    }
                }
                else
                {
                    //Pairing request, no need to touch the bonding info.
                }
            }
            break;
        }

        case BLE_GAP_EVT_CONN_SEC_UPDATE:
            DM_LOG("[DM]: >> BLE_GAP_EVT_CONN_SEC_UPDATE, Mode 0x%02X, Level 0x%02X\r\n",
                   p_ble_evt->evt.gap_evt.params.conn_sec_update.conn_sec.sec_mode.sm,
                   p_ble_evt->evt.gap_evt.params.conn_sec_update.conn_sec.sec_mode.lv);

            if ((p_ble_evt->evt.gap_evt.params.conn_sec_update.conn_sec.sec_mode.lv == 1) &&
                (p_ble_evt->evt.gap_evt.params.conn_sec_update.conn_sec.sec_mode.sm == 1) &&
                ((m_connection_table[index].state & STATE_BONDED) == STATE_BONDED))
            {
                //Lost bond case, generate a security refresh event!
                memset(m_gatts_table[index].attributes, 0, DM_GATT_SERVER_ATTR_MAX_SIZE);

                event.event_id                   = DM_EVT_SECURITY_SETUP_REFRESH;
                m_connection_table[index].state |= STATE_PAIRING_PENDING;
                m_connection_table[index].state |= STATE_BOND_INFO_UPDATE;
                m_application_table[0].state    |= STATE_QUEUED_CONTROL_REQUEST;
            }
            else
            {
                m_connection_table[index].state |= STATE_LINK_ENCRYPTED;
                event.event_id                   = DM_EVT_LINK_SECURED;

                //Apply service context.
                err_code = m_service_context_apply[m_application_table[0].service](&handle);

                if (err_code != NRF_SUCCESS)
                {
                    DM_ERR("[DM]:[CI 0x%02X]:[DI 0x%02X]: Failed to apply service context\r\n",
                            handle.connection_id,
                            handle.device_id);

                    event_result = DM_SERVICE_CONTEXT_NOT_APPLIED;
                }
            }
            event_result = NRF_SUCCESS;
            notify_app   = true;

            break;

        case BLE_GATTS_EVT_SYS_ATTR_MISSING:
            DM_LOG("[DM]: >> BLE_GATTS_EVT_SYS_ATTR_MISSING\r\n");

            //Apply service context.
            event_result = m_service_context_apply[m_application_table[0].service](&handle);
            break;

        case BLE_GAP_EVT_SEC_REQUEST:
            DM_LOG("[DM]: >> BLE_GAP_EVT_SEC_REQUEST\r\n");

            //Verify if the device is already bonded, and if it is bonded, initiate encryption.
            //If the device is not bonded, an instance needs to be allocated in order to initiate
            //bonding. The application have to initiate the procedure, the module will not do this
            //automatically.
            event.event_id = DM_EVT_SECURITY_SETUP;
            notify_app     = true;

            break;

        default:
            break;
    }

    if (notify_app)
    {
        app_evt_notify(&handle, &event, event_result);

        //Freeing the instance after the event is notified so the application can get the context.
        if (event.event_id == DM_EVT_DISCONNECTION)
        {
            //Free the instance.
            connection_instance_free(&index);
        }
    }

    UNUSED_VARIABLE(err_code);

    DM_MUTEX_UNLOCK();
}


ret_code_t dm_handle_get(uint16_t conn_handle, dm_handle_t * p_handle)
{
    ret_code_t err_code;
    uint32_t   index;

    NULL_PARAM_CHECK(p_handle);
    VERIFY_APP_REGISTERED(p_handle->appl_id);

    p_handle->device_id  = DM_INVALID_ID;

    err_code = NRF_ERROR_NOT_FOUND;

    for (index = 0; index < DEVICE_MANAGER_MAX_CONNECTIONS; index++)
    {
        //Search for matching connection handle.
        if (conn_handle == m_connection_table[index].conn_handle)
        {
            p_handle->connection_id = index;
            p_handle->device_id     = m_connection_table[index].bonded_dev_id;

            err_code = NRF_SUCCESS;
            break;
        }
    }
    return err_code;
}
