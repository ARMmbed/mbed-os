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
#ifndef __SN_GRS_STUB_H__
#define __SN_GRS_STUB_H__

#include "sn_nsdl_lib.h"
#include "sn_grs.h"

typedef struct {
    bool retNull;
    int8_t infoRetCounter;
    int8_t info2ndRetCounter;
    int8_t expectedInt8;
    int8_t int8SuccessCounter;
    struct grs_s *expectedGrs;
    sn_nsdl_resource_info_s *expectedInfo;
    sn_grs_resource_list_s *expectedList;

    bool useMockedPath;
    uint8_t mockedPath[8];
    uint8_t mockedPathLen;
}sn_grs_stub_def;

extern sn_grs_stub_def sn_grs_stub;

#endif //__SN_GRS_STUB_H__
