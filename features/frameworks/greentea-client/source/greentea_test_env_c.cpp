/*
 * Copyright (c) 2013-2017, ARM Limited, All Rights Reserved
 * SPDX-License-Identifier: Apache-2.0
 *
 * Licensed under the Apache License, Version 2.0 (the "License"); you may
 * not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS, WITHOUT
 * WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include "greentea-client/test_env.h"


/**
 *   C extensions of greentea client API.
 *
 *   NOTE: Added to support mbed-tls tests generated in C. Please enable more API if needed.
 */

extern "C" void GREENTEA_SETUP_C(const int timeout, const char * host_test){
    GREENTEA_SETUP(timeout, host_test);
};

extern "C" void greentea_send_kv_c(const char * key, const char * val){
    greentea_send_kv(key, val);
};

extern "C" int greentea_parse_kv_c(char * key, char * val,
                                   const int key_len, const int val_len){
    return greentea_parse_kv(key, val, key_len, val_len);
};

