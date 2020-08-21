/* Copyright (c) 2019 Unisoc Communications Inc.
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

#ifndef _RDA5981_OTA_H_
#define _RDA5981_OTA_H_
#include <stdint.h>
#ifdef __cplusplus
extern "C" {
#endif

extern const unsigned int RDA_FW_INFO_ADDR;
extern const unsigned int RDA_UPGRADE_ADDR;

/*
 * function: start to wirte a partition. this func will erase given flash region
 * @addr:    partition start address, must be 4k alignment
 * @img_len: length of image getted from OTA server, must be 4k alignment
 * return: 0:success, else:fail
 */
int rda5981_write_partition_start(unsigned int addr, unsigned int img_len);

/*
 * function: write image to flash, without erase.
 * the write region must be inside of the area given by func rda5981_write_partition_start
 * the write region must be in order, otherwise the end function will return crc error.
 * the maximum length could be write once time is 0x1000
 * @offset: offset from image inital position, must be 1k alignment
 * @buf: data to be written
 * @len: buffer len, max #0x1000, must be 1k alignment
 * return: 0:success, else:fail
 */
int rda5981_write_partition(unsigned int offset, const unsigned char *buf, unsigned int len);

/*
 * function: end of writing partition
 * return: 0:crc32 check success, else:fail
 */
int rda5981_write_partition_end(void);

#ifdef __cplusplus
}
#endif

#endif /*_RDA5981_OTA_H_*/
