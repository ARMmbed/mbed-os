/* mbed Microcontroller Library
 * Copyright (c) 2006-2018 ARM Limited
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
 
#ifndef __STM_SAI_API_H__
#define __STM_SAI_API_H__

#if DEVICE_SAI

#include <stdint.h>
#include <stdbool.h>
#include "stm32f4xx.h"
#include "sai_api.h"

typedef struct stm_sai_api_s {
    sai_result_t    (*init)(sai_t *obj, sai_init_t *init);
    bool            (*transfer)(sai_t *obj, uint32_t *sample);
    void            (*free)(sai_t *obj);
} stm_sai_api_t;

typedef enum sai_periph_type_e {
    SAI_PERIPH_TYPE_SAI = 0,
    SAI_PERIPH_TYPE_I2S = 1
} sai_periph_type_t;

// this is a tagged union
struct sai_base_s {
    sai_periph_type_t type;
    union {
        SPI_TypeDef         *spi;
        SAI_Block_TypeDef   *sai_block;
    } u;
};

#endif // DEVICE_SAI

#endif // __STM_SAI_API_H__
