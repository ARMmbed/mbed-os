/*
 * Copyright (c) Nordic Semiconductor ASA
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without modification,
 * are permitted provided that the following conditions are met:
 *
 *   1. Redistributions of source code must retain the above copyright notice, this
 *   list of conditions and the following disclaimer.
 *
 *   2. Redistributions in binary form must reproduce the above copyright notice, this
 *   list of conditions and the following disclaimer in the documentation and/or
 *   other materials provided with the distribution.
 *
 *   3. Neither the name of Nordic Semiconductor ASA nor the names of other
 *   contributors to this software may be used to endorse or promote products
 *   derived from this software without specific prior written permission.
 *
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
  
/** @file
 *
 * @defgroup nrf_dfu_ble_svc DFU BLE SVC 
 * @{
 *
 * @brief DFU BLE SVC in bootloader. The DFU BLE SuperVisor Calls allow an application to execute
 *        functions in the installed bootloader. 
 *
 * @details This module implements handling of SuperVisor Calls in the bootloader. 
 *          SuperVisor Calls allow for an application to execute calls into the bootloader.
 *          Currently, it is possible to exchange bonding information (like keys) from the 
 *          application to a bootloader supporting DFU OTA using BLE, so the update process can be 
 *          done through an already existing bond.
 *
 * @note The application must make sure that all SuperVisor Calls (SVC) are forwarded to the 
 *       bootloader to ensure correct behavior. Forwarding of SVCs to the bootloader is 
 *       done using the SoftDevice SVC @ref sd_softdevice_vector_table_base_set with the value 
 *       present in @c NRF_UICR->BOOTLOADERADDR.
 */
 
#ifndef DFU_BLE_SVC_H__
#define DFU_BLE_SVC_H__

#include "nrf_svc.h"
#include <stdint.h>
#include "ble_gap.h"
#include "nrf.h"
#include "nrf_soc.h"
#include "nrf_error_sdm.h"

#define BOOTLOADER_SVC_BASE     0x0     /**< The number of the lowest SVC number reserved for the bootloader. */
#define SYSTEM_SERVICE_ATT_SIZE 8       /**< Size of the system service attribute length including CRC-16 at the end. */  

/**@brief The SVC numbers used by the SVC functions in the SoC library. */
enum BOOTLOADER_SVCS
{
    DFU_BLE_SVC_PEER_DATA_SET = BOOTLOADER_SVC_BASE,    /**< SVC number for the setting of peer data call. */
    BOOTLOADER_SVC_LAST
};

/**@brief   DFU Peer data structure.
 *
 * @details This structure contains peer data needed for connection to a bonded device during DFU.
 *          The peer data must be provided by the application to the bootloader during buttonless
 *          update. See @ref dfu_ble_svc_peer_data_set. It contains bond information about the
 *          desired DFU peer.
 */
typedef struct
{
    ble_gap_addr_t      addr;                                   /**< BLE GAP address of the device that initiated the DFU process. */
    ble_gap_irk_t       irk;                                    /**< IRK of the device that initiated the DFU process if this device uses Private Resolvable Addresses. */
    ble_gap_enc_key_t   enc_key;                                /**< Encryption key structure containing encrypted diversifier and LTK for re-establishing the bond. */
    uint8_t             sys_serv_attr[SYSTEM_SERVICE_ATT_SIZE]; /**< System service attributes for restoring of Service Changed Indication setting in DFU mode. */
} dfu_ble_peer_data_t;

/**@brief   SVC Function for setting peer data containing address, IRK, and LTK to establish bonded
 *          connection in DFU mode.
 *
 * @param[in] p_peer_data  Pointer to the peer data containing keys for the connection.
 *
 * @retval NRF_ERROR_NULL If a NULL pointer was provided as argument.
 * @retval NRF_SUCCESS    If the function completed successfully.
 */
SVCALL(DFU_BLE_SVC_PEER_DATA_SET, uint32_t, dfu_ble_svc_peer_data_set(dfu_ble_peer_data_t * p_peer_data));

#endif // DFU_BLE_SVC_H__

/** @} */
