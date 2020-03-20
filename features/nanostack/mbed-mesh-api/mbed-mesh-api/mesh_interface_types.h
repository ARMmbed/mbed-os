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

#ifdef __cplusplus
}
#endif

#endif /* __MESH_INTERFACE_TYPES_H__ */
