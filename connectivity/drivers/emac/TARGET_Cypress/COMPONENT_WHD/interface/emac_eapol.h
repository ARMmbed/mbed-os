/*
 * Copyright 2019 Cypress Semiconductor Corporation
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
#ifndef INCLUDED_EMAC_EAPOL_H_
#define INCLUDED_EMAC_EAPOL_H_

#include "whd.h"

#ifdef __cplusplus
extern "C"
{
#endif

/******************************************************
 *                      Macros
 ******************************************************/

/******************************************************
 *                    Constants
 ******************************************************/
#define EAPOL_PACKET_TYPE     (0x888E)

/******************************************************
 *                   Enumerations
 ******************************************************/

/******************************************************
 *                 Type Definitions
 ******************************************************/

typedef whd_buffer_t    whd_eapol_packet_t;

typedef void (*eapol_packet_handler_t)(whd_interface_t interface, whd_buffer_t buffer);

/******************************************************
 *                    Structures
 ******************************************************/

/******************************************************
 *                 Global Variables
 ******************************************************/

/******************************************************
 *               Function Declarations
 ******************************************************/
whd_result_t emac_register_eapol_packet_handler(eapol_packet_handler_t eapol_packet_handler);
void         emac_unregister_eapol_packet_handler(void);

#ifdef __cplusplus
}     /* extern "C" */
#endif
#endif /* ifndef INCLUDED_EMAC_EAPOL_H_ */
