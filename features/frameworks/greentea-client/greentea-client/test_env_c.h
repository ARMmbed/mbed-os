/** \addtogroup frameworks */
/** @{*/
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

#ifndef TEST_ENV_C_H
#define TEST_ENV_C_H


/**
 *  Greentea-client C API for communication with host side
 */
void GREENTEA_SETUP(const int timeout, const char * host_test);
void greentea_send_kv(const char * key, const char * val);
int greentea_parse_kv(char * key, char * val,
                        const int key_len, const int val_len);
char greentea_getc();

#endif /* TEST_ENV_C_H */

