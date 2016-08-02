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

#include "dfu_app_handler.h"
#include <string.h>
#include "bootloader_util.h"
#include "nrf.h"
#include "nrf_sdm.h"
#include "ble_gatt.h"
#include "ble_gatts.h"
#include "app_error.h"
#include "dfu_ble_svc.h"
#include "device_manager.h"
#include "nrf_delay.h"

#define IRQ_ENABLED            0x01                                     /**< Field that identifies if an interrupt is enabled. */
#define MAX_NUMBER_INTERRUPTS  32                                       /**< Maximum number of interrupts available. */

static void                    dfu_app_reset_prepare(void);             /**< Forward declaration of default reset handler. */
static dfu_app_reset_prepare_t m_reset_prepare = dfu_app_reset_prepare; /**< Callback function to application to prepare for system reset. Allows application to clean up service and memory before reset. */
static dfu_ble_peer_data_t     m_peer_data;                             /**< Peer data to be used for data exchange when resetting into DFU mode. */
static dm_handle_t             m_dm_handle;                             /**< Device Manager handle with instance IDs of current BLE connection. */


/**@brief Function for reset_prepare handler if the application has not registered a handler.
 */
static void dfu_app_reset_prepare(void)
{
    // Reset prepare should be handled by application.
    // This function can be extended to include default handling if application does not implement
    // own handler.
}


/**@brief Function for disabling all interrupts before jumping from bootloader to application.
 */
static void interrupts_disable(void)
{
    uint32_t interrupt_setting_mask;
    uint32_t irq;

    // Fetch the current interrupt settings.
    interrupt_setting_mask = NVIC->ISER[0];

    // Loop from interrupt 0 for disabling of all interrupts.
    for (irq = 0; irq < MAX_NUMBER_INTERRUPTS; irq++)
    {
        if (interrupt_setting_mask & (IRQ_ENABLED << irq))
        {
            // The interrupt was enabled, hence disable it.
            NVIC_DisableIRQ((IRQn_Type)irq);
        }
    }
}


/**@brief Function for providing peer information to DFU for re-establishing a bonded connection in
 *        DFU mode.
 *
 * @param[in] conn_handle   Connection handle for the connection requesting DFU mode.
 */
static void dfu_app_peer_data_set(uint16_t conn_handle)
{
    uint32_t                 err_code;
    dm_sec_keyset_t          key_set;
    uint32_t                 app_context_data = 0;
    dm_application_context_t app_context;


/** [DFU bond sharing] */
    err_code = dm_handle_get(conn_handle, &m_dm_handle);
    if (err_code == NRF_SUCCESS)
    {
        err_code = dm_distributed_keys_get(&m_dm_handle, &key_set);
        if (err_code == NRF_SUCCESS)
        {
            APP_ERROR_CHECK(err_code);

            m_peer_data.addr              = key_set.keys_central.p_id_key->id_addr_info;
            m_peer_data.irk               = key_set.keys_central.p_id_key->id_info;
            m_peer_data.enc_key.enc_info  = key_set.keys_periph.enc_key.p_enc_key->enc_info;
            m_peer_data.enc_key.master_id = key_set.keys_periph.enc_key.p_enc_key->master_id;

            err_code = dfu_ble_svc_peer_data_set(&m_peer_data);
            APP_ERROR_CHECK(err_code);

            app_context_data   = (DFU_APP_ATT_TABLE_CHANGED << DFU_APP_ATT_TABLE_POS);
            app_context.len    = sizeof(app_context_data);
            app_context.p_data = (uint8_t *)&app_context_data;
            app_context.flags  = 0;

            err_code = dm_application_context_set(&m_dm_handle, &app_context);
            APP_ERROR_CHECK(err_code);
        }
        else
        {
            // Keys were not available, thus we have a non-encrypted connection.
            err_code = dm_peer_addr_get(&m_dm_handle, &m_peer_data.addr);
            APP_ERROR_CHECK(err_code);

            err_code = dfu_ble_svc_peer_data_set(&m_peer_data);
            APP_ERROR_CHECK(err_code);
        }
    }
/** [DFU bond sharing] */
}


/**@brief Function for preparing the reset, disabling SoftDevice, and jumping to the bootloader.
 *
 * @param[in] conn_handle Connection handle for peer requesting to enter DFU mode.
 */
static void bootloader_start(uint16_t conn_handle)
{
    uint32_t err_code;
    uint16_t sys_serv_attr_len = sizeof(m_peer_data.sys_serv_attr);

    err_code = sd_ble_gatts_sys_attr_get(conn_handle,
                                         m_peer_data.sys_serv_attr,
                                         &sys_serv_attr_len,
                                         BLE_GATTS_SYS_ATTR_FLAG_SYS_SRVCS);
    if (err_code != NRF_SUCCESS)
    {
        // Any error at this stage means the system service attributes could not be fetched.
        // This means the service changed indication cannot be sent in DFU mode, but connection
        // is still possible to establish.
    }

    m_reset_prepare();

    err_code = sd_power_gpregret_set(BOOTLOADER_DFU_START);
    APP_ERROR_CHECK(err_code);

    err_code = sd_softdevice_disable();
    APP_ERROR_CHECK(err_code);

    err_code = sd_softdevice_vector_table_base_set(NRF_UICR->BOOTLOADERADDR);
    APP_ERROR_CHECK(err_code);

    dfu_app_peer_data_set(conn_handle);

    NVIC_ClearPendingIRQ(SWI2_IRQn);
    interrupts_disable();
    bootloader_util_app_start(NRF_UICR->BOOTLOADERADDR);
}


void dfu_app_on_dfu_evt(ble_dfu_t * p_dfu, ble_dfu_evt_t * p_evt)
{
    switch (p_evt->ble_dfu_evt_type)
    {
        case BLE_DFU_START:
            // Starting the bootloader - will cause reset.
            bootloader_start(p_dfu->conn_handle);
            break;

        default:
            {
                // Unsupported event received from DFU Service. 
                // Send back BLE_DFU_RESP_VAL_NOT_SUPPORTED message to peer.
                uint32_t err_code = ble_dfu_response_send(p_dfu,
                                                          BLE_DFU_START_PROCEDURE,
                                                          BLE_DFU_RESP_VAL_NOT_SUPPORTED);
                APP_ERROR_CHECK(err_code);
            }
            break;
    }
}


void dfu_app_reset_prepare_set(dfu_app_reset_prepare_t reset_prepare_func)
{
    m_reset_prepare = reset_prepare_func;
}


void dfu_app_dm_appl_instance_set(dm_application_instance_t app_instance)
{
    uint32_t err_code;
    
    err_code = dm_application_instance_set(&app_instance, &m_dm_handle);
    APP_ERROR_CHECK(err_code);
}
