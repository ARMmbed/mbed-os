/* mbed Microcontroller Library
 * Copyright (c) 2013-2016 Realtek Semiconductor Corp.
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
#ifndef MBED_FLASH_EXT_H
#define MBED_FLASH_EXT_H

#include "device.h"
#include "flash_api.h"

#ifdef __cplusplus
extern "C" {
#endif

#define FLASH_PAGE_SIZE 256
#define FLASH_SIZE        0x200000
#define FLASH_OFS_START   0x0
#define FLASH_OFS_END     (FLASH_OFS_START + FLASH_SIZE)

extern void flash_ext_erase_sector(flash_t *obj, uint32_t address);
extern void flash_ext_erase_block(flash_t * obj, uint32_t address);
extern int flash_ext_read_word(flash_t *obj, uint32_t address, uint32_t *data);
extern int flash_ext_write_word(flash_t *obj, uint32_t address, uint32_t data);
extern int flash_ext_stream_read(flash_t *obj, uint32_t address, uint32_t len, uint8_t *data);
extern int flash_ext_stream_write(flash_t *obj, uint32_t address, uint32_t len, const uint8_t *data);
extern int flash_stream_read(flash_t *obj, uint32_t addr, uint32_t len, uint8_t *data);
extern int flash_stream_write(flash_t *obj, uint32_t addr, uint32_t len, const uint8_t *data);
extern void flash_ext_write_protect(flash_t *obj, uint32_t protect);
extern int flash_ext_get_status(flash_t *obj);
extern int flash_ext_set_status(flash_t *obj, uint32_t data);
extern void flash_ext_reset_status(flash_t *obj);

/* Internal functions for flash API */
extern void __flash_ext_turnon(void);
extern void __flash_ext_turnoff(void);
extern void __flash_ext_erase_sector(flash_t *obj, uint32_t address);
extern int __flash_ext_stream_write(flash_t *obj, uint32_t addr, uint32_t len, const uint8_t *data);
#ifdef __cplusplus
}
#endif

#endif
