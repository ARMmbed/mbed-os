/*
 * Copyright (c) 2014-2017, Arm Limited and affiliates.
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
#ifndef IPHC_DECOMPRESS_H_
#define IPHC_DECOMPRESS_H_

uint16_t iphc_header_scan(buffer_t *buf, uint16_t *uncompressed_size);

buffer_t *iphc_decompress(const lowpan_context_list_t *context_list, buffer_t *buf);

#endif /* IPHC_DECOMPRESS_H_ */
