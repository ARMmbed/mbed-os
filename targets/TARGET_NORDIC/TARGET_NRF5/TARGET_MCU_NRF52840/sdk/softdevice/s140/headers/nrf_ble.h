/* 
 * Copyright (c) 2000 Nordic Semiconductor ASA
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

/**
  @addtogroup BLE_COMMON BLE SoftDevice Common
  @{
  @defgroup ble_api Events, type definitions and API calls
  @{

  @brief Module independent events, type definitions and API calls for the BLE SoftDevice.

 */

#ifndef BLE_H__
#define BLE_H__

#include "nrf_ble_ranges.h"
#include "nrf_ble_types.h"
#include "nrf_ble_gap.h"
#include "nrf_ble_l2cap.h"
#include "nrf_ble_gatt.h"
#include "nrf_ble_gattc.h"
#include "nrf_ble_gatts.h"

#ifdef __cplusplus
extern "C" {
#endif

/** @addtogroup BLE_COMMON_ENUMERATIONS Enumerations
 * @{ */

/**
 * @brief Common API SVC numbers.
 */
enum BLE_COMMON_SVCS
{
  SD_BLE_ENABLE = BLE_SVC_BASE,         /**< Enable and initialize the BLE stack */
  SD_BLE_EVT_GET,                       /**< Get an event from the pending events queue. */
  SD_BLE_TX_PACKET_COUNT_GET,           /**< Get the total number of available application transmission packets for a particular connection. */
  SD_BLE_UUID_VS_ADD,                   /**< Add a Vendor Specific UUID. */
  SD_BLE_UUID_DECODE,                   /**< Decode UUID bytes. */
  SD_BLE_UUID_ENCODE,                   /**< Encode UUID bytes. */
  SD_BLE_VERSION_GET,                   /**< Get the local version information (company id, Link Layer Version, Link Layer Subversion). */
  SD_BLE_USER_MEM_REPLY,                /**< User Memory Reply. */
  SD_BLE_OPT_SET,                       /**< Set a BLE option. */
  SD_BLE_OPT_GET,                       /**< Get a BLE option. */
};

  /**
   * @brief BLE Module Independent Event IDs.
   */
enum BLE_COMMON_EVTS
{
  BLE_EVT_TX_COMPLETE  = BLE_EVT_BASE,  /**< Transmission Complete. @ref ble_evt_tx_complete_t */
  BLE_EVT_USER_MEM_REQUEST,             /**< User Memory request. @ref ble_evt_user_mem_request_t */
  BLE_EVT_USER_MEM_RELEASE,             /**< User Memory release. @ref ble_evt_user_mem_release_t */
  BLE_EVT_DATA_LENGTH_CHANGED           /**< Link layer PDU length changed. @ref ble_evt_data_length_changed_t. */
};

/**@brief BLE connection bandwidth types.
 * Bandwidth types supported by the SoftDevice. The bandwidth type dictates the maximum number of full length packets per connection interval.
 */
enum BLE_CONN_BWS
{
  BLE_CONN_BW_INVALID = 0,              /**< Invalid connection bandwidth. */
  BLE_CONN_BW_LOW,                      /**< Low connection bandwidth. */
  BLE_CONN_BW_MID,                      /**< Medium connection bandwidth. */
  BLE_CONN_BW_HIGH                      /**< High connection bandwidth. */
};

/**@brief Common Option IDs.
 * IDs that uniquely identify a common option.
 */
enum BLE_COMMON_OPTS
{
  BLE_COMMON_OPT_CONN_BW = BLE_OPT_BASE,     /**< Bandwidth configuration @ref ble_common_opt_conn_bw_t */
  BLE_COMMON_OPT_PA_LNA,                     /**< PA and LNA options */
  BLE_COMMON_OPT_CONN_EVT_EXT,               /**< Extended connection events option */
};

/** @} */

/** @addtogroup BLE_COMMON_DEFINES Defines
 * @{ */

/** @brief  Required pointer alignment for BLE Events.
*/
#define BLE_EVT_PTR_ALIGNMENT    4

/** @brief  Leaves the maximum of the two arguments.
*/
#define BLE_MAX(a, b) ((a) < (b) ? (b) : (a))

/** @brief  Maximum possible length for BLE Events.
 * @note Value of @ref ble_gatt_enable_params_t::att_mtu shall be used as a parameter.
 * If that value is set to 0 then @ref BLE_GATT_MTU_SIZE_DEFAULT must be used instead.
*/
#define BLE_EVT_LEN_MAX(ATT_MTU) (BLE_MAX( \
  sizeof(ble_evt_t), \
  BLE_MAX( \
    offsetof(ble_evt_t, evt.gattc_evt.params.rel_disc_rsp.includes) + ((ATT_MTU) - 2) / 6 * sizeof(ble_gattc_include_t), \
    offsetof(ble_evt_t, evt.gattc_evt.params.attr_info_disc_rsp.info.attr_info16) + ((ATT_MTU) - 2) / 4 * sizeof(ble_gattc_attr_info16_t) \
  ) \
))

/** @defgroup BLE_USER_MEM_TYPES User Memory Types
 * @{ */
#define BLE_USER_MEM_TYPE_INVALID               0x00  /**< Invalid User Memory Types. */
#define BLE_USER_MEM_TYPE_GATTS_QUEUED_WRITES   0x01  /**< User Memory for GATTS queued writes. */
/** @} */

/** @defgroup BLE_UUID_VS_COUNTS Vendor Specific UUID counts
 * @{
 */
#define BLE_UUID_VS_COUNT_MIN         1    /**< Minimum VS UUID count. */
#define BLE_UUID_VS_COUNT_DEFAULT     0    /**< Use the default VS UUID count (10 for this version of the SoftDevice). */
/** @} */

/** @} */

/** @addtogroup BLE_COMMON_STRUCTURES Structures
 * @{ */

/**@brief User Memory Block. */
typedef struct
{
  uint8_t          *p_mem;      /**< Pointer to the start of the user memory block. */
  uint16_t          len;        /**< Length in bytes of the user memory block. */
} ble_user_mem_block_t;

/**
 * @brief Event structure for @ref BLE_EVT_TX_COMPLETE.
 */
typedef struct
{
  uint8_t count;                        /**< Number of packets transmitted. */
} ble_evt_tx_complete_t;

/**@brief Event structure for @ref BLE_EVT_USER_MEM_REQUEST. */
typedef struct
{
  uint8_t                     type;     /**< User memory type, see @ref BLE_USER_MEM_TYPES. */
} ble_evt_user_mem_request_t;

/**@brief Event structure for @ref BLE_EVT_USER_MEM_RELEASE. */
typedef struct
{
  uint8_t                     type;       /**< User memory type, see @ref BLE_USER_MEM_TYPES. */
  ble_user_mem_block_t        mem_block;  /**< User memory block */
} ble_evt_user_mem_release_t;

/**@brief Event structure for @ref BLE_EVT_DATA_LENGTH_CHANGED. */
typedef struct
{
  uint16_t max_tx_octets;                 /**< The maximum number of payload octets in a Link Layer Data Channel PDU that the local Controller will send. Range: 27-251 */
  uint16_t max_tx_time;                   /**< The maximum time (in microseconds) that the local Controller will take to send a Link Layer Data Channel PDU. Range: 328-2120  */
  uint16_t max_rx_octets;                 /**< The maximum number of payload octets in a Link Layer Data Channel PDU that the local controller expects to receive. Range: 27-251 */
  uint16_t max_rx_time;                   /**< The maximum time (in microseconds) that the local Controller expects to take to receive a Link Layer Data Channel PDU. Range: 328-2120 */
} ble_evt_data_length_changed_t;

/**@brief Event structure for events not associated with a specific function module. */
typedef struct
{
  uint16_t conn_handle;                                 /**< Connection Handle on which this event occurred. */
  union
  {
    ble_evt_tx_complete_t           tx_complete;         /**< Transmission Complete. */
    ble_evt_user_mem_request_t      user_mem_request;    /**< User Memory Request Event Parameters. */
    ble_evt_user_mem_release_t      user_mem_release;    /**< User Memory Release Event Parameters. */
    ble_evt_data_length_changed_t   data_length_changed; /**< Data Length Changed Event Parameters. */
  } params;                                              /**< Event parameter union. */
} ble_common_evt_t;

/**@brief BLE Event header. */
typedef struct
{
  uint16_t evt_id;                /**< Value from a BLE_<module>_EVT series. */
  uint16_t evt_len;               /**< Length in octets including this header. */
} ble_evt_hdr_t;

/**@brief Common BLE Event type, wrapping the module specific event reports. */
typedef struct
{
  ble_evt_hdr_t header;           /**< Event header. */
  union
  {
    ble_common_evt_t  common_evt; /**< Common Event, evt_id in BLE_EVT_* series. */
    ble_gap_evt_t     gap_evt;    /**< GAP originated event, evt_id in BLE_GAP_EVT_* series. */
    ble_l2cap_evt_t   l2cap_evt;  /**< L2CAP originated event, evt_id in BLE_L2CAP_EVT* series. */
    ble_gattc_evt_t   gattc_evt;  /**< GATT client originated event, evt_id in BLE_GATTC_EVT* series. */
    ble_gatts_evt_t   gatts_evt;  /**< GATT server originated event, evt_id in BLE_GATTS_EVT* series. */
  } evt;                          /**< Event union. */
} ble_evt_t;


/**
 * @brief Version Information.
 */
typedef struct
{
  uint8_t   version_number;    /**< Link Layer Version number for BT 4.1 spec is 7 (https://www.bluetooth.org/en-us/specification/assigned-numbers/link-layer). */
  uint16_t  company_id;        /**< Company ID, Nordic Semiconductor's company ID is 89 (0x0059) (https://www.bluetooth.org/apps/content/Default.aspx?doc_id=49708). */
  uint16_t  subversion_number; /**< Link Layer Sub Version number, corresponds to the SoftDevice Config ID or Firmware ID (FWID). */
} ble_version_t;

/**
 * @brief Configuration parameters for the PA and LNA.
 */
typedef struct
{
     uint8_t enable :1;      /**< Enable toggling for this amplifier */
     uint8_t active_high :1; /**< Set the pin to be active high */
     uint8_t gpio_pin :6;    /**< The GPIO pin to toggle for this amplifier */
} ble_pa_lna_cfg_t;

/**
 * @brief PA & LNA GPIO toggle configuration
 *
 * This option configures the SoftDevice to toggle pins when the radio is active for use with a power amplifier and/or
 * a low noise amplifier.
 *
 * Toggling the pins is achieved by using two PPI channels and a GPIOTE channel. The hardware channel IDs are provided
 * by the application and should be regarded as reserved as long as any PA/LNA toggling is enabled.
 *
 * @note  @ref sd_ble_opt_get is not supported for this option.
 * @note  This feature is only supported for nRF52, on nRF51 @ref NRF_ERROR_NOT_SUPPORTED will always be returned.
 * @note  Setting this option while the radio is in use (i.e. any of the roles are active) may have undefined consequences
 * and must be avoided by the application.
 */
typedef struct
{
   ble_pa_lna_cfg_t pa_cfg;   /**< Power Amplifier configuration */
   ble_pa_lna_cfg_t lna_cfg;  /**< Low Noise Amplifier configuration */

   uint8_t ppi_ch_id_set;     /**< PPI channel used for radio pin setting */
   uint8_t ppi_ch_id_clr;     /**< PPI channel used for radio pin clearing */
   uint8_t gpiote_ch_id;      /**< GPIOTE channel used for radio pin toggling */
} ble_common_opt_pa_lna_t;

/**
 * @brief BLE connection bandwidth configuration parameters
 */
typedef struct
{
  uint8_t conn_bw_tx;   /**< Connection bandwidth configuration for transmission, see @ref BLE_CONN_BWS.*/
  uint8_t conn_bw_rx;   /**< Connection bandwidth configuration for reception, see @ref BLE_CONN_BWS.*/
} ble_conn_bw_t;

/**@brief BLE connection specific bandwidth configuration parameters.
 *
 * This can be used with @ref sd_ble_opt_set to set the bandwidth configuration to be used when creating connections.
 *
 * Call @ref sd_ble_opt_set with this option prior to calling @ref sd_ble_gap_adv_start or @ref sd_ble_gap_connect.
 *
 * The bandwidth configurations set via @ref sd_ble_opt_set are maintained separately for central and peripheral
 * connections. The given configurations are used for all future connections of the role indicated in this structure
 * unless they are changed by subsequent @ref sd_ble_opt_set calls.
 *
 * @note When this option is not used, the SoftDevice will use the default options:
 * - @ref BLE_CONN_BW_HIGH for @ref BLE_GAP_ROLE_PERIPH connections (both transmission and reception).
 * - @ref BLE_CONN_BW_MID for @ref BLE_GAP_ROLE_CENTRAL connections (both transmisison and reception).
 * This option allows the application to selectively override these defaults for each role.
 *
 * @note The global memory pool configuration can be set with the @ref ble_conn_bw_counts_t configuration parameter, which
 * is provided to @ref sd_ble_enable.
 *
 * @note @ref sd_ble_opt_get is not supported for this option.
 * @note Please refer to SoftDevice Specification for more information on bandwidth configuration.
 *
 * @mscs
 * @mmsc{@ref BLE_COMMON_CONF_BW}
 * @endmscs
 *
 * @retval ::NRF_SUCCESS Set successfully.
 * @retval ::BLE_ERROR_INVALID_ROLE The role is invalid.
 * @retval ::NRF_ERROR_INVALID_PARAM Invalid bandwidth configuration parameters.
 * @retval ::NRF_ERROR_NOT_SUPPORTED If the combination of role and bandwidth configuration is not supported.
 */
typedef struct
{
  uint8_t            role;     /**< BLE role of the connection, see @ref BLE_GAP_ROLES. */
  ble_conn_bw_t      conn_bw;  /**< Bandwidth configuration parameters. */
} ble_common_opt_conn_bw_t;

/**
 * @brief Configuration of extended BLE connection events.
 *
 * When enabled the SoftDevice will dynamically extend the connection event when possible.
 *
 * The connection event length is controlled by the bandwidth configuration as set by @ref ble_common_opt_conn_bw_t.
 * The connection event can be extended if there is time to send another packet pair before the start of the next connection interval,
 * and if there are no conflicts with other BLE roles requesting radio time.
 *
 * @note @ref sd_ble_opt_get is not supported for this option.
 */
typedef struct
{
   uint8_t enable : 1; /**< Enable extended BLE connection events, disabled by default. */
} ble_common_opt_conn_evt_ext_t;

/**@brief Option structure for common options. */
typedef union
{
  ble_common_opt_conn_bw_t      conn_bw;       /**< Parameters for the connection bandwidth option. */
  ble_common_opt_pa_lna_t       pa_lna;        /**< Parameters for controlling PA and LNA pin toggling. */
  ble_common_opt_conn_evt_ext_t conn_evt_ext;  /**< Parameters for enabling extended connection events. */
} ble_common_opt_t;

/**@brief Common BLE Option type, wrapping the module specific options. */
typedef union
{
  ble_common_opt_t  common_opt;         /**< COMMON options, opt_id in @ref BLE_COMMON_OPTS series. */
  ble_gap_opt_t     gap_opt;            /**< GAP option, opt_id in @ref BLE_GAP_OPTS series. */
} ble_opt_t;

/**
 * @brief BLE bandwidth count parameters
 *
 * These parameters are used to configure the memory pools allocated within the SoftDevice for application packets
 * (both transmission and reception) for all connections.
 *
 * @note The sum of all three counts must add up to the sum of @ref ble_gap_enable_params_t::central_conn_count and
 * @ref ble_gap_enable_params_t::periph_conn_count in @ref ble_gap_enable_params_t.
 */
typedef struct {
  uint8_t high_count;   /**< Total number of high bandwidth TX or RX memory pools available to the application at runtime for all active connections. */
  uint8_t mid_count;    /**< Total number of medium bandwidth TX or RX memory pools available to the application at runtime for all active connections. */
  uint8_t low_count;    /**< Total number of low bandwidth TX or RX memory pools available to the application at runtime for all active connections. */
} ble_conn_bw_count_t;

/**
 * @brief BLE bandwidth global memory pool configuration parameters
 *
 * These configuration parameters are used to set the amount of memory dedicated to application packets for
 * all connections. The application should specify the most demanding configuration for the intended use.
 *
 * Please refer to the SoftDevice Specification for more information on bandwidth configuration.
 *
 * @note Each connection created at runtime requires both a TX and an RX memory pool. By the use of these configuration
 * parameters, the application can decide the size and total number of the global memory pools that will be later
 * available for connection creation.
 *
 * @mscs
 * @mmsc{@ref BLE_COMMON_CONF_BW}
 * @endmscs
 *
 */
typedef struct {
  ble_conn_bw_count_t tx_counts;   /**< Global memory pool configuration for transmission.*/
  ble_conn_bw_count_t rx_counts;   /**< Global memory pool configuration for reception.*/
} ble_conn_bw_counts_t;

/**
 * @brief BLE Common Initialization parameters.
 *
 * @note If @ref p_conn_bw_counts is NULL the SoftDevice will assume default bandwidth configuration for all connections.
 * To fit a custom bandwidth configuration requirement, the application developer may have to specify a custom memory
 * pool configuration here. See @ref ble_common_opt_conn_bw_t for bandwidth configuration of individual connections.
 * Please refer to the SoftDevice Specification for more information on bandwidth configuration.
 */
typedef struct
{
  uint16_t                  vs_uuid_count;     /**< Maximum number of 128-bit, Vendor Specific UUID bases to allocate. */
  ble_conn_bw_counts_t      *p_conn_bw_counts; /**< Bandwidth configuration parameters or NULL for defaults. */
} ble_common_enable_params_t;

/**
 * @brief BLE Initialization parameters.
 */
typedef struct
{
  ble_common_enable_params_t        common_enable_params;  /**< Common init parameters @ref ble_common_enable_params_t. */
  ble_gap_enable_params_t           gap_enable_params;     /**< GAP init parameters @ref ble_gap_enable_params_t. */
  ble_gatt_enable_params_t          gatt_enable_params;    /**< GATT init parameters @ref ble_gatt_enable_params_t. */
  ble_gatts_enable_params_t         gatts_enable_params;   /**< GATTS init parameters @ref ble_gatts_enable_params_t. */
} ble_enable_params_t;

/** @} */

/** @addtogroup BLE_COMMON_FUNCTIONS Functions
 * @{ */

/**@brief Enable the BLE stack
 *
 * @param[in, out] p_ble_enable_params Pointer to ble_enable_params_t
 * @param[in, out] p_app_ram_base      Pointer to a variable containing the start address of the application RAM region
 * (APP_RAM_BASE). On return, this will contain the minimum start address of the application RAM region required by the
 * SoftDevice for this configuration. Calling @ref sd_ble_enable() with *p_app_ram_base set to 0 can be used during
 * development to find out how much memory a specific configuration will need.
 *
 * @note The memory requirement for a specific configuration will not increase between SoftDevices with the same major
 * version number.
 *
 * @note At runtime the IC's RAM is split into 2 regions: The SoftDevice RAM region is located between 0x20000000 and
 *       APP_RAM_BASE-1 and the application's RAM region is located between APP_RAM_BASE and the start of the call stack.
 *
 * @details This call initializes the BLE stack, no other BLE related function can be called before this one.
 *
 * @mscs
 * @mmsc{@ref BLE_COMMON_ENABLE}
 * @endmscs
 *
 * @retval ::NRF_SUCCESS              The BLE stack has been initialized successfully.
 * @retval ::NRF_ERROR_INVALID_STATE  The BLE stack had already been initialized and cannot be reinitialized.
 * @retval ::NRF_ERROR_INVALID_ADDR   Invalid or not sufficiently aligned pointer supplied.
 * @retval ::NRF_ERROR_INVALID_LENGTH One or more of the following is true:
 *                                    - The specified Attribute Table size is too small.
 *                                      The minimum acceptable size is defined by @ref BLE_GATTS_ATTR_TAB_SIZE_MIN.
 *                                    - The specified Attribute Table size is not a multiple of 4.
 *                                    - The device name length is invalid (must be between 0 and @ref BLE_GAP_DEVNAME_MAX_LEN).
 *                                    - The device name length is too long for the given Attribute Table.
 * @retval ::NRF_ERROR_INVALID_PARAM  One or more of the following is true:
 *                                    - Incorrectly configured VS UUID count.
 *                                    - Invalid connection count parameters.
 *                                    - Invalid device name location (vloc).
 *                                    - Invalid device name security mode.
 *                                    - Invalid maximum ATT_MTU size, see @ref ble_gatt_enable_params_t::att_mtu.
 * @retval ::NRF_ERROR_NOT_SUPPORTED  Device name security mode is not supported.
 * @retval ::NRF_ERROR_NO_MEM         The amount of memory assigned to the SoftDevice by *p_app_ram_base is not
 *                                    large enough to fit this configuration's memory requirement. Check *p_app_ram_base
 *                                    and set the start address of the application RAM region accordingly.
 * @retval ::NRF_ERROR_CONN_COUNT     The requested number of connections exceeds the maximum supported by the SoftDevice.
 *                                    Please refer to the SoftDevice Specification for more information on role configuration.
 */
SVCALL(SD_BLE_ENABLE, uint32_t, sd_ble_enable(ble_enable_params_t * p_ble_enable_params, uint32_t * p_app_ram_base));

/**@brief Get an event from the pending events queue.
 *
 * @param[out] p_dest Pointer to buffer to be filled in with an event, or NULL to retrieve the event length.
 *                    This buffer <b>must be aligned to the extend defined by @ref BLE_EVT_PTR_ALIGNMENT</b>.
 *                    The buffer should be interpreted as a @ref ble_evt_t struct.
 * @param[in, out] p_len Pointer the length of the buffer, on return it is filled with the event length.
 *
 * @details This call allows the application to pull a BLE event from the BLE stack. The application is signaled that
 * an event is available from the BLE stack by the triggering of the SD_EVT_IRQn interrupt.
 * The application is free to choose whether to call this function from thread mode (main context) or directly from the
 * Interrupt Service Routine that maps to SD_EVT_IRQn. In any case however, and because the BLE stack runs at a higher
 * priority than the application, this function should be called in a loop (until @ref NRF_ERROR_NOT_FOUND is returned)
 * every time SD_EVT_IRQn is raised to ensure that all available events are pulled from the BLE stack. Failure to do so
 * could potentially leave events in the internal queue without the application being aware of this fact.
 *
 * Sizing the p_dest buffer is equally important, since the application needs to provide all the memory necessary for the event to
 * be copied into application memory. If the buffer provided is not large enough to fit the entire contents of the event,
 * @ref NRF_ERROR_DATA_SIZE will be returned and the application can then call again with a larger buffer size.
 * The maximum possible event length is defined by @ref BLE_EVT_LEN_MAX. The application may also "peek" the event length
 * by providing p_dest as a NULL pointer and inspecting the value of *p_len upon return:
 *
 *     \code
 *     uint16_t len;
 *     errcode = sd_ble_evt_get(NULL, &len);
 *     \endcode
 *
 * @mscs
 * @mmsc{@ref BLE_COMMON_IRQ_EVT_MSC}
 * @mmsc{@ref BLE_COMMON_THREAD_EVT_MSC}
 * @endmscs
 *
 * @retval ::NRF_SUCCESS Event pulled and stored into the supplied buffer.
 * @retval ::NRF_ERROR_INVALID_ADDR Invalid or not sufficiently aligned pointer supplied.
 * @retval ::NRF_ERROR_NOT_FOUND No events ready to be pulled.
 * @retval ::NRF_ERROR_DATA_SIZE Event ready but could not fit into the supplied buffer.
 */
SVCALL(SD_BLE_EVT_GET, uint32_t, sd_ble_evt_get(uint8_t *p_dest, uint16_t *p_len));


/**@brief Get the total number of available guaranteed application transmission packets for a particular connection.
 *
 * @details This call allows the application to obtain the total number of guaranteed application transmission packets
 * available for a connection. Please note that this does not return the number of free packets, but rather the total
 * amount of them for that particular connection. The application has two options to handle transmitting application packets:
 * - Use a simple arithmetic calculation: after connection creation time the application should use this function to
 * find out the total amount of guaranteed packets available to it and store it in a variable.
 * Every time a packet is successfully queued for a transmission on this connection using any of the exposed functions in
 * this  BLE API, the application should decrement that variable. Conversely, whenever a @ref BLE_EVT_TX_COMPLETE event
 * with the conn_handle matching the particular connection is received by the application, it should retrieve the count
 * field in such event and add that number to the same variable storing the number of available guaranteed packets. This
 * mechanism allows the application to be aware at any time of the number of guaranteed application packets available for
 * each of the active connections, and therefore it can know with certainty whether it is possible to send more data or
 * it has to wait for a @ref BLE_EVT_TX_COMPLETE event before it proceeds.
 * The application can still pursue transmissions when the number of guaranteed application packets available is smaller
 * than or equal to zero, but successful queuing of the tranmsission is not guaranteed.
 * - Choose to simply not keep track of available packets at all, and instead handle the @ref BLE_ERROR_NO_TX_PACKETS error
 * by queueing the packet to be transmitted and try again as soon as a @ref BLE_EVT_TX_COMPLETE event arrives.
 *
 * The API functions that <b>may</b> consume an application packet depending on the parameters supplied to them can be found below:
 * - @ref sd_ble_gattc_write (write without response only)
 * - @ref sd_ble_gatts_hvx (notifications only)
 * - @ref sd_ble_l2cap_tx (all packets)
 *
 * @param[in]  conn_handle Connection handle.
 * @param[out] p_count Pointer to a uint8_t which will contain the number of application transmission packets upon
 *                     successful return.
 * @mscs
 * @mmsc{@ref BLE_COMMON_APP_BUFF_MSC}
 * @endmscs
 *
 * @retval ::NRF_SUCCESS Number of application transmission packets retrieved successfully.
 * @retval ::BLE_ERROR_INVALID_CONN_HANDLE Invalid Connection Handle.
 * @retval ::NRF_ERROR_INVALID_ADDR Invalid pointer supplied.
 */
SVCALL(SD_BLE_TX_PACKET_COUNT_GET, uint32_t, sd_ble_tx_packet_count_get(uint16_t conn_handle, uint8_t *p_count));


/**@brief Add a Vendor Specific base UUID.
 *
 * @details This call enables the application to add a vendor specific base UUID to the BLE stack's table, for later
 * use with all other modules and APIs. This then allows the application to use the shorter, 24-bit @ref ble_uuid_t
 * format when dealing with both 16-bit and 128-bit UUIDs without having to check for lengths and having split code
 * paths. This is accomplished by extending the grouping mechanism that the Bluetooth SIG standard base UUID uses
 * for all other 128-bit UUIDs. The type field in the @ref ble_uuid_t structure is an index (relative to
 * @ref BLE_UUID_TYPE_VENDOR_BEGIN) to the table populated by multiple calls to this function, and the uuid field
 * in the same structure contains the 2 bytes at indices 12 and 13. The number of possible 128-bit UUIDs available to
 * the application is therefore the number of Vendor Specific UUIDs added with the help of this function times 65536,
 * although restricted to modifying bytes 12 and 13 for each of the entries in the supplied array.
 *
 * @note Bytes 12 and 13 of the provided UUID will not be used internally, since those are always replaced by
 * the 16-bit uuid field in @ref ble_uuid_t.
 *
 * @note If a UUID is already present in the BLE stack's internal table, the corresponding index will be returned in
 * p_uuid_type along with an NRF_SUCCESS error code.
 *
 * @param[in]  p_vs_uuid    Pointer to a 16-octet (128-bit) little endian Vendor Specific UUID disregarding
 *                          bytes 12 and 13.
 * @param[out] p_uuid_type  Pointer to a uint8_t where the type field in @ref ble_uuid_t corresponding to this UUID will be stored.
 *
 * @retval ::NRF_SUCCESS Successfully added the Vendor Specific UUID.
 * @retval ::NRF_ERROR_INVALID_ADDR If p_vs_uuid or p_uuid_type is NULL or invalid.
 * @retval ::NRF_ERROR_NO_MEM If there are no more free slots for VS UUIDs.
 */
SVCALL(SD_BLE_UUID_VS_ADD, uint32_t, sd_ble_uuid_vs_add(ble_uuid128_t const *p_vs_uuid, uint8_t *p_uuid_type));


/** @brief Decode little endian raw UUID bytes (16-bit or 128-bit) into a 24 bit @ref ble_uuid_t structure.
 *
 * @details The raw UUID bytes excluding bytes 12 and 13 (i.e. bytes 0-11 and 14-15) of p_uuid_le are compared
 * to the corresponding ones in each entry of the table of vendor specific UUIDs populated with @ref sd_ble_uuid_vs_add
 * to look for a match. If there is such a match, bytes 12 and 13 are returned as p_uuid->uuid and the index
 * relative to @ref BLE_UUID_TYPE_VENDOR_BEGIN as p_uuid->type.
 *
 * @note If the UUID length supplied is 2, then the type set by this call will always be @ref BLE_UUID_TYPE_BLE.
 *
 * @param[in]   uuid_le_len Length in bytes of the buffer pointed to by p_uuid_le (must be 2 or 16 bytes).
 * @param[in]   p_uuid_le   Pointer pointing to little endian raw UUID bytes.
 * @param[out]  p_uuid      Pointer to a @ref ble_uuid_t structure to be filled in.
 *
 * @retval ::NRF_SUCCESS Successfully decoded into the @ref ble_uuid_t structure.
 * @retval ::NRF_ERROR_INVALID_ADDR Invalid pointer supplied.
 * @retval ::NRF_ERROR_INVALID_LENGTH Invalid UUID length.
 * @retval ::NRF_ERROR_NOT_FOUND For a 128-bit UUID, no match in the populated table of UUIDs.
 */
SVCALL(SD_BLE_UUID_DECODE, uint32_t, sd_ble_uuid_decode(uint8_t uuid_le_len, uint8_t const *p_uuid_le, ble_uuid_t *p_uuid));


/** @brief Encode a @ref ble_uuid_t structure into little endian raw UUID bytes (16-bit or 128-bit).
 *
 * @note The pointer to the destination buffer p_uuid_le may be NULL, in which case only the validity and size of p_uuid is computed.
 *
 * @param[in]   p_uuid        Pointer to a @ref ble_uuid_t structure that will be encoded into bytes.
 * @param[out]  p_uuid_le_len Pointer to a uint8_t that will be filled with the encoded length (2 or 16 bytes).
 * @param[out]  p_uuid_le     Pointer to a buffer where the little endian raw UUID bytes (2 or 16) will be stored.
 *
 * @retval ::NRF_SUCCESS Successfully encoded into the buffer.
 * @retval ::NRF_ERROR_INVALID_ADDR Invalid pointer supplied.
 * @retval ::NRF_ERROR_INVALID_PARAM Invalid UUID type.
 */
SVCALL(SD_BLE_UUID_ENCODE, uint32_t, sd_ble_uuid_encode(ble_uuid_t const *p_uuid, uint8_t *p_uuid_le_len, uint8_t *p_uuid_le));


/**@brief Get Version Information.
 *
 * @details This call allows the application to get the BLE stack version information.
 *
 * @param[out] p_version Pointer to a ble_version_t structure to be filled in.
 *
 * @retval ::NRF_SUCCESS  Version information stored successfully.
 * @retval ::NRF_ERROR_INVALID_ADDR Invalid pointer supplied.
 * @retval ::NRF_ERROR_BUSY The BLE stack is busy (typically doing a locally-initiated disconnection procedure).
 */
SVCALL(SD_BLE_VERSION_GET, uint32_t, sd_ble_version_get(ble_version_t *p_version));


/**@brief Provide a user memory block.
 *
 * @note This call can only be used as a response to a @ref BLE_EVT_USER_MEM_REQUEST event issued to the application.
 *
 * @param[in] conn_handle Connection handle.
 * @param[in,out] p_block Pointer to a user memory block structure.
 *
 * @mscs
 * @mmsc{@ref BLE_GATTS_QUEUED_WRITE_NOBUF_PEER_CANCEL_MSC}
 * @mmsc{@ref BLE_GATTS_QUEUED_WRITE_NOBUF_AUTH_MSC}
 * @mmsc{@ref BLE_GATTS_QUEUED_WRITE_NOBUF_NOAUTH_MSC}
 * @mmsc{@ref BLE_GATTS_QUEUED_WRITE_BUF_AUTH_MSC}
 * @mmsc{@ref BLE_GATTS_QUEUED_WRITE_BUF_NOAUTH_MSC}
 * @mmsc{@ref BLE_GATTS_QUEUED_WRITE_QUEUE_FULL_MSC}
 * @endmscs
 *
 * @retval ::NRF_SUCCESS Successfully queued a response to the peer.
 * @retval ::BLE_ERROR_INVALID_CONN_HANDLE Invalid Connection Handle.
 * @retval ::NRF_ERROR_INVALID_STATE Invalid Connection state or no execute write request pending.
 * @retval ::NRF_ERROR_BUSY The BLE stack is busy. Retry at later time.
 */
SVCALL(SD_BLE_USER_MEM_REPLY, uint32_t, sd_ble_user_mem_reply(uint16_t conn_handle, ble_user_mem_block_t const *p_block));

/**@brief Set a BLE option.
 *
 * @details This call allows the application to set the value of an option.
 *
 * @mscs
 * @mmsc{@ref BLE_GAP_PERIPH_BONDING_STATIC_PK_MSC}
 * @mmsc{@ref BLE_COMMON_CONF_BW}
 * @endmscs
 *
 * @param[in] opt_id Option ID, see @ref BLE_COMMON_OPTS and @ref BLE_GAP_OPTS.
 * @param[in] p_opt Pointer to a ble_opt_t structure containing the option value.
 *
 * @retval ::NRF_SUCCESS  Option set successfully.
 * @retval ::NRF_ERROR_INVALID_ADDR Invalid pointer supplied.
 * @retval ::BLE_ERROR_INVALID_CONN_HANDLE Invalid Connection Handle.
 * @retval ::NRF_ERROR_INVALID_PARAM Invalid parameter(s) supplied, check parameter limits and constraints.
 * @retval ::NRF_ERROR_INVALID_STATE Unable to set the parameter at this time.
 * @retval ::NRF_ERROR_BUSY The BLE stack is busy or the previous procedure has not completed.
 */
SVCALL(SD_BLE_OPT_SET, uint32_t, sd_ble_opt_set(uint32_t opt_id, ble_opt_t const *p_opt));


/**@brief Get a BLE option.
 *
 * @details This call allows the application to retrieve the value of an option.
 *
 * @param[in] opt_id Option ID, see @ref BLE_COMMON_OPTS and @ref BLE_GAP_OPTS.
 * @param[out] p_opt Pointer to a ble_opt_t structure to be filled in.
 *
 * @retval ::NRF_SUCCESS  Option retrieved successfully.
 * @retval ::NRF_ERROR_INVALID_ADDR Invalid pointer supplied.
 * @retval ::BLE_ERROR_INVALID_CONN_HANDLE Invalid Connection Handle.
 * @retval ::NRF_ERROR_INVALID_PARAM Invalid parameter(s) supplied, check parameter limits and constraints.
 * @retval ::NRF_ERROR_INVALID_STATE Unable to retrieve the parameter at this time.
 * @retval ::NRF_ERROR_BUSY The BLE stack is busy or the previous procedure has not completed.
 * @retval ::NRF_ERROR_NOT_SUPPORTED This option is not supported.
 *
 */
SVCALL(SD_BLE_OPT_GET, uint32_t, sd_ble_opt_get(uint32_t opt_id, ble_opt_t *p_opt));

/** @} */
#ifdef __cplusplus
}
#endif
#endif /* BLE_H__ */

/**
  @}
  @}
*/
