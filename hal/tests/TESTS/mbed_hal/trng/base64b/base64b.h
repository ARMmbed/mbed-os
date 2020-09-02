/*
* Copyright (c) 2018 ARM Limited. All rights reserved.
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

#include <stdint.h>
#include <stdlib.h>
#include <string>

typedef enum {
    BASE64_SUCCESS = 0,
    BASE64_INVALID_PARAMETER = 1,
    BASE64_BUFFER_TOO_SMALL = 2,
    BASE64_ERROR = 3,
} base64_result_e;

base64_result_e trng_EncodeBase64(const void *buffer, uint32_t bufferSize, char *string, uint32_t stringSize);
base64_result_e trng_DecodeNBase64(const char *string, uint32_t stringMaxSize, void *buffer, uint32_t bufferSize,
                                   uint32_t *lengthWritten, uint32_t *charsProcessed);


