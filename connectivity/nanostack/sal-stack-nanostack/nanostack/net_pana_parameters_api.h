/*
 * Copyright (c) 2014-2017, Pelion and affiliates.
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

#ifndef NET_PANA_PARAMETERS_API_H_
#define NET_PANA_PARAMETERS_API_H_

#include "ns_types.h"

/**
 * \file net_pana_parameters_api.h
 * \brief An API for setting up or changing PANA library parameters.
 *
 * \section set-pana-param Setting up PANA parameters.
 *  -  net_pana_parameter_set(), A function to set up PANA library parameters.
 * \section check-pana-param Checking existing PANA setup.
 *  -  net_pana_parameter_read(), A function to read current PANA library setup.
 */

/*!
 * \struct pana_lib_parameters_s
 * \brief PANA library dynamic parameters.
 */
/** The structure defines PANA library parameters.*/
typedef struct pana_lib_parameters_s {
    uint16_t PCI_IRT;                   /**< Initial PCI timeout in seconds, default 10. */
    uint16_t PCI_MRT;                   /**< Max PCI timeout value in seconds, default 60.  */
    uint8_t PCI_MRC;                    /**< PCI_MRC Max PCI retransmission attempts, default 5. */
    uint16_t REQ_IRT;                   /**< PCI_MRC Initial request timeout in seconds, default 20. */
    uint16_t REQ_MRT;                   /**< Max request timeout value, default 60. */
    uint16_t REQ_MRC;                   /**< Max request retransmission attempts, default 4. */
    uint16_t AUTHENTICATION_TIMEOUT;        /**< Max timeout for authencication, default 100 seconds. */
    uint16_t KEY_UPDATE_THRESHOLD;      /**< Gap in seconds before the server starts to send a new network key, default 10. */
    uint8_t KEY_ID_MAX_VALUE;           /**< Define resolution for key ID [1-KEY_ID_MAX_VALUE], default 255. MIN accepted value is 3.*/
    uint16_t EAP_FRAGMENT_SIZE;         /**< Define EAP fragment slot size. Fragmentation is activated when EAP payload is more than 920. Default 296. */
    uint8_t  AUTH_COUNTER_MAX;          /**< Define PANA session re-authentication limit. When the MAX value is reached the server does not respond to the PANA notify request. Default 0xff. */
} pana_lib_parameters_s;

/**
 * \brief A function to set PANA library parameters.
 *
 * Note: This function should be called after net_init_core() and definitely
 * before creating any 6LoWPAN interface.
 *
 * For future compatibility, to support extensions to this structure, read
 * the current parameters using net_pana_parameter_read(),
 * modify known fields, then set.
 *
 * \param parameter_ptr A pointer for PANA parameters.
 *
 * \return 0, Change OK.
 * \return -1, Invalid values.
 * \return -2, PANA not supported.
 *
 */
extern int8_t net_pana_parameter_set(const pana_lib_parameters_s *parameter_ptr);


/**
 * \brief A function to read existing PANA library parameters.
 *
 * \param parameter_ptr An output pointer for PANA parameters.
 *
 * \return 0, Read OK.
 * \return -1, PANA not supported.
 */
extern int8_t net_pana_parameter_read(pana_lib_parameters_s *parameter_ptr);

#endif /* NET_PANA_PARAMETERS_API_H_ */
