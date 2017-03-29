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
  @addtogroup BLE_L2CAP Logical Link Control and Adaptation Protocol (L2CAP)
  @{
  @brief Definitions and prototypes for the L2CAP interface.
 */

#ifndef BLE_L2CAP_H__
#define BLE_L2CAP_H__

#include "ble_types.h"
#include "ble_ranges.h"
#include "ble_err.h"
#include "nrf_svc.h"

#ifdef __cplusplus
extern "C" {
#endif

/**@addtogroup BLE_L2CAP_ENUMERATIONS Enumerations
 * @{ */

/**@brief L2CAP API SVC numbers. */
enum BLE_L2CAP_SVCS
{
  SD_BLE_L2CAP_CID_REGISTER = BLE_L2CAP_SVC_BASE,  /**< Register a CID. */
  SD_BLE_L2CAP_CID_UNREGISTER,                     /**< Unregister a CID. */
  SD_BLE_L2CAP_TX                                  /**< Transmit a packet. */
};

/**@brief L2CAP Event IDs. */
enum BLE_L2CAP_EVTS
{
  BLE_L2CAP_EVT_RX  = BLE_L2CAP_EVT_BASE          /**< L2CAP packet received. */
};

/** @} */

/**@addtogroup BLE_L2CAP_DEFINES Defines
 * @{ */

/**@defgroup BLE_ERRORS_L2CAP SVC return values specific to L2CAP
 * @{ */
#define BLE_ERROR_L2CAP_CID_IN_USE            (NRF_L2CAP_ERR_BASE + 0x000)  /**< CID already in use. */
/** @} */

/**@brief Default L2CAP MTU. */
#define BLE_L2CAP_MTU_DEF           (23)

/**@brief Invalid Channel Identifier. */
#define BLE_L2CAP_CID_INVALID       (0x0000)

/**@brief Dynamic Channel Identifier base. */
#define BLE_L2CAP_CID_DYN_BASE      (0x0040)

/**@brief Maximum amount of dynamic CIDs. */
#define BLE_L2CAP_CID_DYN_MAX       (8)

/** @} */

/**@addtogroup BLE_L2CAP_STRUCTURES Structures
 * @{ */

/**@brief Packet header format for L2CAP transmission. */
typedef struct
{
  uint16_t   len;                                 /**< Length of valid info in data member. */
  uint16_t   cid;                                 /**< Channel ID on which packet is transmitted. */
} ble_l2cap_header_t;


/**@brief L2CAP Received packet event report. */
typedef struct
{
  ble_l2cap_header_t header;                      /**< L2CAP packet header. */
  uint8_t    data[1];                             /**< Packet data. @note This is a variable length array. The size of 1 indicated is only a placeholder for compilation.
                                                       See @ref sd_ble_evt_get for more information on how to use event structures with variable length array members. */
} ble_l2cap_evt_rx_t;


/**@brief L2CAP event callback event structure. */
typedef struct
{
  uint16_t conn_handle;                           /**< Connection Handle on which event occured. */
  union
  {
    ble_l2cap_evt_rx_t rx;                        /**< RX Event parameters. */
  } params;                                       /**< Event Parameters. */
} ble_l2cap_evt_t;

/** @} */

/**@addtogroup BLE_L2CAP_FUNCTIONS Functions
 * @{ */

/**@brief Register a CID with L2CAP.
 *
 * @details This registers a higher protocol layer with the L2CAP multiplexer, and is requried prior to all operations on the CID.
 *
 * @mscs
 * @mmsc{@ref BLE_L2CAP_API_MSC}
 * @endmscs
 *
 * @param[in] cid L2CAP CID.
 *
 * @retval ::NRF_SUCCESS Successfully registered a CID with the L2CAP layer.
 * @retval ::NRF_ERROR_INVALID_PARAM Invalid parameter(s) supplied, CID must be above @ref BLE_L2CAP_CID_DYN_BASE.
 * @retval ::BLE_ERROR_L2CAP_CID_IN_USE L2CAP CID already in use.
 * @retval ::NRF_ERROR_NO_MEM Not enough memory to complete operation.
 */
SVCALL(SD_BLE_L2CAP_CID_REGISTER, uint32_t, sd_ble_l2cap_cid_register(uint16_t cid));

/**@brief Unregister a CID with L2CAP.
 *
 * @details This unregisters a previously registerd higher protocol layer with the L2CAP multiplexer.
 *
 * @mscs
 * @mmsc{@ref BLE_L2CAP_API_MSC}
 * @endmscs
 *
 * @param[in] cid L2CAP CID.
 *
 * @retval ::NRF_SUCCESS Successfully unregistered the CID.
 * @retval ::NRF_ERROR_INVALID_PARAM Invalid parameter(s) supplied.
 * @retval ::NRF_ERROR_NOT_FOUND CID not previously registered.
 */
SVCALL(SD_BLE_L2CAP_CID_UNREGISTER, uint32_t, sd_ble_l2cap_cid_unregister(uint16_t cid));

/**@brief Transmit an L2CAP packet.
 *
 * @note    It is important to note that a call to this function will <b>consume an application packet</b>, and will therefore
 *          generate a @ref BLE_EVT_TX_COMPLETE event when the packet has been transmitted.
 *          Please see the documentation of @ref sd_ble_tx_packet_count_get for more details.
 *
 * @events
 * @event{@ref BLE_EVT_TX_COMPLETE}
 * @event{@ref BLE_L2CAP_EVT_RX}
 * @endevents
 *
 * @mscs
 * @mmsc{@ref BLE_L2CAP_API_MSC}
 * @endmscs
 *
 * @param[in] conn_handle Connection Handle.
 * @param[in] p_header    Pointer to a packet header containing length and CID.
 * @param[in] p_data      Pointer to the data to be transmitted.
 *
 * @retval ::NRF_SUCCESS Successfully queued an L2CAP packet for transmission.
 * @retval ::NRF_ERROR_INVALID_ADDR Invalid pointer supplied.
 * @retval ::NRF_ERROR_INVALID_PARAM Invalid parameter(s) supplied, CIDs must be registered beforehand with @ref sd_ble_l2cap_cid_register.
 * @retval ::NRF_ERROR_NOT_FOUND CID not found.
 * @retval ::NRF_ERROR_NO_MEM Not enough memory to complete operation.
 * @retval ::BLE_ERROR_NO_TX_PACKETS Not enough application packets available.
 * @retval ::NRF_ERROR_DATA_SIZE Invalid data size(s) supplied, see @ref BLE_L2CAP_MTU_DEF.
 */
SVCALL(SD_BLE_L2CAP_TX, uint32_t, sd_ble_l2cap_tx(uint16_t conn_handle, ble_l2cap_header_t const *p_header, uint8_t const *p_data));

/** @} */

#ifdef __cplusplus
}
#endif
#endif // BLE_L2CAP_H__

/**
  @}
*/
