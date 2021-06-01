/*
 * Copyright (c) 2015 ARM Limited. All rights reserved.
 * SPDX-License-Identifier: Apache-2.0
 * Licensed under the Apache License, Version 2.0 (the License); you may
 * not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an AS IS BASIS, WITHOUT
 * WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#ifndef __MESH_INTERFACE_TYPES_H__
#define __MESH_INTERFACE_TYPES_H__

#ifdef __cplusplus
extern "C" {
#endif

/**
 * Mesh network types
 */
typedef enum {
    MESH_TYPE_6LOWPAN_ND = 0,
    MESH_TYPE_THREAD
} mesh_network_type_t;

/**
 * Error status values returned by Mesh network API.
 */
typedef enum {
    MESH_ERROR_NONE = 0,    /*<! No error */
    MESH_ERROR_UNKNOWN,     /*<! Unspecified error */
    MESH_ERROR_MEMORY,      /*<! Memory error */
    MESH_ERROR_STATE,       /*<! Illegal state */
    MESH_ERROR_PARAM,       /*<! Illegal parameter */
} mesh_error_t;

/**
 * Mesh network connection status codes returned in callback.
 */
typedef enum {
    MESH_CONNECTED = 0,             /*<! connected to network */
    MESH_CONNECTED_LOCAL,           /*<! connected to network, got local IP */
    MESH_CONNECTED_GLOBAL,          /*<! connected to network, got global IP */
    MESH_DISCONNECTED,              /*<! disconnected from network */
    MESH_BOOTSTRAP_START_FAILED,    /*<! error during bootstrap start */
    MESH_BOOTSTRAP_FAILED,          /*<! error in bootstrap */
    MESH_BOOTSTRAP_STARTED          /*<! bootstrap started */
} mesh_connection_status_t;

/**
 * Mesh device types
 */
typedef enum {
    MESH_DEVICE_TYPE_THREAD_ROUTER = 0,         /*<! Thread router */
    MESH_DEVICE_TYPE_THREAD_SLEEPY_END_DEVICE,  /*<! Thread Sleepy end device */
    MESH_DEVICE_TYPE_THREAD_MINIMAL_END_DEVICE, /*<! Thread minimal end device */
    MESH_DEVICE_TYPE_WISUN_ROUTER,              /*<! Wi-SUN router */
    MESH_DEVICE_TYPE_WISUN_BORDER_ROUTER        /*<! Wi-SUN border router */
} mesh_device_type_t;

/**
 * Mesh channel function
 */
typedef enum {
    MESH_CHANNEL_FUNCTION_FIXED = 0x00,
    MESH_CHANNEL_FUNCTION_TR51CF,
    MESH_CHANNEL_FUNCTION_DH1CF,
    MESH_CHANNEL_FUNCTION_VENDOR_DEFINED
} mesh_channel_function_t;

/**
 * Mesh network statistics
 */
typedef struct {
    uint32_t rpl_total_memory;  /*<! RPL current memory usage total. */
    uint16_t etx_1st_parent;    /*<! Primary parent ETX. */
    uint16_t etx_2nd_parent;    /*<! Secondary parent ETX. */
    uint32_t asynch_tx_count;   /*<! Asynch TX counter */
    uint32_t asynch_rx_count;   /*<! Asynch RX counter */
    uint32_t join_state_1;      /*<! Time spent in individual Wi-SUN join state 1 Discovery */
    uint32_t join_state_2;      /*<! Time spent in individual Wi-SUN join state 2 Authentication */
    uint32_t join_state_3;      /*<! Time spent in individual Wi-SUN join state 3 Configuration learn */
    uint32_t join_state_4;      /*<! Time spent in individual Wi-SUN join state 4 RPL parent discovery */
    uint32_t join_state_5;      /*<! Time spent in individual Wi-SUN join state 5 Active state */
    uint32_t sent_PAS;          /*<! Amount of Wi-SUN Pan Advertisement Solicit Message sent */
    uint32_t sent_PA;           /*<! Amount of Wi-SUN Pan Advertisement Message sent */
    uint32_t sent_PCS;          /*<! Amount of Wi-SUN Pan Configuration Solicit Message sent */
    uint32_t sent_PC;           /*<! Amount of Wi-SUN Pan Configuration Message sent */
    uint32_t recv_PAS;          /*<! Amount of Wi-SUN Pan Advertisement Solicit Message received */
    uint32_t recv_PA;           /*<! Amount of Wi-SUN Pan Advertisement Message received */
    uint32_t recv_PCS;          /*<! Amount of Wi-SUN Pan Configuration Solicit Message received */
    uint32_t recv_PC;           /*<! Amount of Wi-SUN Pan Configuration Message received */
    uint32_t Neighbour_add;     /*<! New Neighbours found */
    uint32_t Neighbour_remove;  /*<! New Neighbours Removed */
    uint32_t Child_add;         /*<! New Child added */
    uint32_t child_remove;      /*<! Child lost */
} mesh_nw_statistics_t;

/**
 * Mesh physical layer statistics
 */
typedef struct {
    uint32_t mac_rx_count;            /*<! MAC RX packet count. */
    uint32_t mac_tx_count;            /*<! MAC TX packet count. */
    uint32_t mac_bc_rx_count;         /*<! MAC broadcast RX packet count. */
    uint32_t mac_bc_tx_count;         /*<! MAC broadcast TX packet count. */
    uint32_t mac_tx_bytes;            /*<! MAC TX bytes count. */
    uint32_t mac_rx_bytes;            /*<! MAC RX bytes count. */
    uint32_t mac_tx_failed_count;     /*<! MAC TX failed count. */
    uint32_t mac_retry_count;         /*<! MAC TX retry count. */
    uint32_t mac_cca_attempts_count;  /*<! MAC CCA attempts count. */
    uint32_t mac_failed_cca_count;    /*<! MAC failed CCA count. */
} mesh_mac_statistics_t;

#ifdef __cplusplus
}
#endif

#endif /* __MESH_INTERFACE_TYPES_H__ */
