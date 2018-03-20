/*
 * Copyright (c) 2016-2017, Arm Limited and affiliates.
 * SPDX-License-Identifier: Apache-2.0
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#ifndef RF_DRIVER_STORAGE_H
#define RF_DRIVER_STORAGE_H

#include "ns_list.h"
#include "mac_common_defines.h"
#include "mlme.h"
#include "platform/arm_hal_phy.h"
#include "mac_api.h"

struct protocol_interface_rf_mac_setup;

/**
 *  PD-SAP data layer format
 */
typedef enum {
    MAC15_4_PD_SAP_DATA_IND = 0,
    MAC15_4_PD_SAP_DATA_TX_CONFIRM,
    MAC802_PD_SAP_DATA_IND,
    MAC802_PD_SAP_DATA_TX_CONFIRM,
    MACTUN_PD_SAP_DATA_IND,
    MACTUN_PD_SAP_NAP_IND,
    MACTUN_PD_SAP_CONFIRM,
    MACTUN_MLME_NAP_EXTENSION,
} arm_pd_sap_primitive_e;

/**
 * @brief struct arm_pd_sap_generic_confirm_t PD Data confirm structure
 *
 * See IEEE standard 802.15.4-2006 (table 7) for more details
 */
typedef struct arm_pd_sap_generic_confirm_s {
    phy_link_tx_status_e status;
} arm_pd_sap_generic_confirm_t;

/**
 * @brief struct arm_pd_sap_15_4_confirm_with_params_t PD Data confirm structure
 *
 * See IEEE standard 802.15.4-2006 (table 7) for more details
 */
typedef struct arm_pd_sap_15_4_confirm_with_params_s {
    phy_link_tx_status_e status;
    uint8_t cca_retry; //Non-standard addition, 0 if not available
    uint8_t tx_retry; //Non-standard addition, 0 if not available
} arm_pd_sap_15_4_confirm_with_params_t;

/**
 * @brief struct arm_pd_sap_generic_ind_t PD Data confirm structure
 *
 * See IEEE standard 802.15.4-2006 (table 8) for more details
 */
typedef struct arm_pd_sap_generic_ind_s {
    const uint8_t *data_ptr;
    uint16_t data_len;
    uint8_t link_quality;
    int8_t dbm; //Desibels; Non-standard addition
} arm_pd_sap_generic_ind_t;

/**
 * @brief struct arm_mlme_req_t Common MLME message data structure
 *
 */
typedef struct arm_mlme_req_s {
    mlme_primitive primitive;
    const void *mlme_ptr;
    uint16_t ptr_length;
} arm_mlme_req_t;

typedef struct arm_phy_sap_msg_s {
    arm_pd_sap_primitive_e id;
    union {
        arm_pd_sap_15_4_confirm_with_params_t mac15_4_pd_sap_confirm;
        arm_pd_sap_generic_confirm_t generic_confirm;
        arm_pd_sap_generic_ind_t generic_data_ind;
        arm_mlme_req_t mlme_request;
    } message;
} arm_phy_sap_msg_t;

/**
 * @brief struct mac_ps_data_cb Typedef for upper layer callback function
 * @param identifier Upper layer identifier (set by upper layer)
 * @param message PD-SAP confirm or indication message
 */
typedef int8_t mac_ps_data_cb(void *identifier, arm_phy_sap_msg_t *message);

typedef void internal_mib_observer(const mlme_set_t * set_req);

typedef struct arm_device_driver_list {
    int8_t id; /**< Event handler Tasklet ID */
    phy_device_driver_s *phy_driver;
    mac_ps_data_cb *phy_sap_upper_cb;
    void * phy_sap_identifier;
    internal_mib_observer *mlme_observer_cb;
    ns_list_link_t link;
} arm_device_driver_list_s;

arm_device_driver_list_s *arm_net_phy_driver_pointer(int8_t id);
uint32_t dev_get_phy_datarate(phy_device_driver_s *phy_driver, channel_page_e channel_page);
int8_t arm_net_phy_init(phy_device_driver_s *phy_driver, arm_net_phy_rx_fn *rx_cb, arm_net_phy_tx_done_fn *done_cb);
void arm_net_observer_cb_set(int8_t id, internal_mib_observer *observer_cb);
void arm_net_virtual_config_rx_cb_set(phy_device_driver_s *phy_driver, arm_net_virtual_config_rx_fn *virtual_config_rx);
void arm_net_virtual_confirmation_rx_cb_set(phy_device_driver_s *phy_driver, arm_net_virtual_confirmation_rx_fn *virtual_confirmation_rx);
#endif // RF_DRIVER_STORAGE_H
