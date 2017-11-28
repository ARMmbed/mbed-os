/*
 * Copyright (c) 2015-2016, Arm Limited and affiliates.
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
#ifndef __COAP_SERVICE_API_STUB_H__
#define __COAP_SERVICE_API_STUB_H__

#ifdef __cplusplus
extern "C" {
#endif

#include <inttypes.h>
#include <string.h>
#include "coap_service_api.h"

typedef struct {
    char* expectedCharPtr;
    void* cb;
    int8_t expectedInt8;
    int16_t expectedInt16;
    uint16_t expectedUint16;
    uint8_t callBackAccepted;
    coap_service_request_recv_cb* register_uri_send_cb;
    coap_service_virtual_socket_send_cb* socket_send_cb;
    coap_service_response_recv* response_resp_cb;
} coap_service_api_stub_def;

extern coap_service_api_stub_def coap_service_api_stub;
void reset_callback();
#ifdef __cplusplus
}
#endif

#endif // __COAP_SERVICE_API_STUB_H__
