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

#ifndef MBED_EXT_FLASH_API_EXT_H
#define MBED_EXT_FLASH_API_EXT_H

#include "device.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct flash_s flash_t;

/**
  * global data structure
  */   
extern flash_t            flash;

enum {
  FLASH_COMPLETE = 0,
  FLASH_ERROR_2 = 1,
};

//void flash_init                 (flash_t *obj);
void flash_erase_sector            (flash_t *obj, uint32_t address);
void flash_erase_block(flash_t * obj, uint32_t address);
int  flash_read_word            (flash_t *obj, uint32_t address, uint32_t * data);
int  flash_write_word            (flash_t *obj, uint32_t address, uint32_t data);
int  flash_stream_read          (flash_t *obj, uint32_t address, uint32_t len, uint8_t * data);
int  flash_stream_write         (flash_t *obj, uint32_t address, uint32_t len, uint8_t * data);
void flash_write_protect        (flash_t *obj, uint32_t protect);
int flash_get_status(flash_t * obj);
int flash_set_status(flash_t * obj, uint32_t data);
void flash_reset_status(flash_t * obj);
int flash_burst_write(flash_t * obj, uint32_t address, uint32_t Length, uint8_t * data);
int flash_burst_read(flash_t * obj, uint32_t address, uint32_t Length, uint8_t * data);
int flash_set_extend_addr(flash_t * obj, uint32_t data);
int flash_get_extend_addr(flash_t * obj);

#ifdef __cplusplus
}
#endif


#endif
