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
#include "CppUTest/TestHarness.h"
#include <string.h>
#include <stdint.h>
#include "sn_nsdl.h"
#include "sn_coap_protocol.h"
#include "sn_nsdl_lib.h"
#include "sn_coap_header_internal.h"


TEST_GROUP(libCoap_header_check)
{
    void setup() {

    }

    void teardown() {

    }
};

TEST(libCoap_header_check, header_check)
{
    sn_coap_hdr_s coap_header;

    memset(&coap_header, 0, sizeof(sn_coap_hdr_s));
    coap_header.msg_type = COAP_MSG_TYPE_CONFIRMABLE;
    coap_header.msg_code = COAP_MSG_CODE_REQUEST_GET;

    /* Happy-happy case */
    CHECK(sn_coap_header_validity_check(&coap_header, (coap_version_e)COAP_VERSION_1) == 0);


    CHECK(sn_coap_header_validity_check(&coap_header, (coap_version_e) 0) == -1);

    coap_header.msg_type = (sn_coap_msg_type_e)0x40;

    CHECK(sn_coap_header_validity_check(&coap_header, (coap_version_e)COAP_VERSION_1) == -1);

    coap_header.msg_type = (sn_coap_msg_type_e)COAP_MSG_TYPE_CONFIRMABLE;
    coap_header.msg_code = (sn_coap_msg_code_e)5;

    CHECK(sn_coap_header_validity_check(&coap_header, (coap_version_e)COAP_VERSION_1) == -1);
}
