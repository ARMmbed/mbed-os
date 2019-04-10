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
 *
 */
#ifndef CY_VSC_CMD_H_
#define CY_VSC_CMD_H_
/**
 * Vendor specific command complete
 */
typedef void (cy_vsc_callback_t)(uint16_t opcode, uint8_t status, uint16_t data_len, uint8_t* data);

/**
 * Send a vendor specific HCI command to the controller.
 *
 * @param opcode     : Opcode of vendor specific command
 * @param param_len  : Length of parameter buffer
 * @param param      : Parameters
 * @param vsc_cb     : Callback for command complete
 *
 * @return true if the request has been successfully sent otherwise false.
 *
 */
bool cy_send_vendor_specific_cmd(uint16_t opcode, uint8_t param_len, uint8_t *param, cy_vsc_callback_t *vsc_cb);
#endif // CY_VSC_CMD_H_
