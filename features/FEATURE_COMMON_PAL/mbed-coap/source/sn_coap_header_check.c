/*
 * Copyright (c) 2011-2015 ARM Limited. All rights reserved.
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

/**
 * \file sn_coap_header_check.c
 *
 * \brief CoAP Header validity checker
 *
 * Functionality: Checks validity of CoAP Header
 *
 */

/* * * * INCLUDE FILES * * * */
#include "ns_types.h"
#include "mbed-coap/sn_coap_header.h"
#include "mbed-coap/sn_coap_protocol.h"
#include "sn_coap_header_internal.h"
#include "sn_coap_protocol_internal.h"
#include "mbed-trace/mbed_trace.h"

#define TRACE_GROUP "coap"

/**
 * \fn int8_t sn_coap_header_validity_check(sn_coap_hdr_s *src_coap_msg_ptr, coap_version_e coap_version)
 *
 * \brief Checks validity of given Header
 *
 * \param *src_coap_msg_ptr is source for building Packet data
 * \param coap_version is version of used CoAP specification
 *
 * \return Return value is status of validity check. In ok cases 0 and in
 *         failure cases -1
 */
int8_t sn_coap_header_validity_check(sn_coap_hdr_s *src_coap_msg_ptr, coap_version_e coap_version)
{
    /* * Check validity of CoAP Version * */
    if (coap_version != COAP_VERSION_1) {
        return -1;
    }

    /* * Check validity of Message type * */
    switch (src_coap_msg_ptr->msg_type) {
        case COAP_MSG_TYPE_CONFIRMABLE:
        case COAP_MSG_TYPE_NON_CONFIRMABLE:
        case COAP_MSG_TYPE_ACKNOWLEDGEMENT:
        case COAP_MSG_TYPE_RESET:
            break;      /* Ok cases */
        default:
            tr_error("sn_coap_header_validity_check - unknown message type!");
            return -1;      /* Failed case */
    }

    /* * Check validity of Message code * */
    switch (src_coap_msg_ptr->msg_code) {
        case COAP_MSG_CODE_EMPTY:
        case COAP_MSG_CODE_REQUEST_GET:
        case COAP_MSG_CODE_REQUEST_POST:
        case COAP_MSG_CODE_REQUEST_PUT:
        case COAP_MSG_CODE_REQUEST_DELETE:
        case COAP_MSG_CODE_RESPONSE_CREATED:
        case COAP_MSG_CODE_RESPONSE_DELETED:
        case COAP_MSG_CODE_RESPONSE_VALID:
        case COAP_MSG_CODE_RESPONSE_CHANGED:
        case COAP_MSG_CODE_RESPONSE_CONTENT:
        case COAP_MSG_CODE_RESPONSE_BAD_REQUEST:
        case COAP_MSG_CODE_RESPONSE_UNAUTHORIZED:
        case COAP_MSG_CODE_RESPONSE_BAD_OPTION:
        case COAP_MSG_CODE_RESPONSE_FORBIDDEN:
        case COAP_MSG_CODE_RESPONSE_NOT_FOUND:
        case COAP_MSG_CODE_RESPONSE_METHOD_NOT_ALLOWED:
        case COAP_MSG_CODE_RESPONSE_NOT_ACCEPTABLE:
        case COAP_MSG_CODE_RESPONSE_REQUEST_ENTITY_INCOMPLETE:
        case COAP_MSG_CODE_RESPONSE_PRECONDITION_FAILED:
        case COAP_MSG_CODE_RESPONSE_REQUEST_ENTITY_TOO_LARGE:
        case COAP_MSG_CODE_RESPONSE_UNSUPPORTED_CONTENT_FORMAT:
        case COAP_MSG_CODE_RESPONSE_INTERNAL_SERVER_ERROR:
        case COAP_MSG_CODE_RESPONSE_NOT_IMPLEMENTED:
        case COAP_MSG_CODE_RESPONSE_BAD_GATEWAY:
        case COAP_MSG_CODE_RESPONSE_SERVICE_UNAVAILABLE:
        case COAP_MSG_CODE_RESPONSE_GATEWAY_TIMEOUT:
        case COAP_MSG_CODE_RESPONSE_PROXYING_NOT_SUPPORTED:
        case COAP_MSG_CODE_RESPONSE_CONTINUE:
            break;      /* Ok cases */
        default:
            tr_error("sn_coap_header_validity_check - unknown message code!");
            return -1;      /* Failed case */
    }

    /* Success */
    return 0;
}
