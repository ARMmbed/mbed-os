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
#include "sai_api.h"

#if DEVICE_SAI

// math.h required for floating point operations for baud rate calculation
#include <math.h>
#include "mbed_assert.h"

#include <string.h>

#include "cmsis.h"
#include "pinmap.h"
#include "objects.h"
#include "PeripheralPins.h"

#include "stm_sai_api.h"
#include "stm_sai_driver.h"
#include "stm_i2s_driver.h"

static const stm_sai_api_t *sai_vtable[] = { &stm_sai_vtable, &stm_i2s_vtable };
static const sai_base_t sai_bases[] = {
    {
        .type = SAI_PERIPH_TYPE_SAI,
        .u.sai_block = SAI1_Block_A
    }, {
        .type = SAI_PERIPH_TYPE_SAI,
        .u.sai_block = SAI1_Block_B
    }, {
        .type = SAI_PERIPH_TYPE_I2S,
        .u.spi = SPI2
    }, {
        .type = SAI_PERIPH_TYPE_I2S,
        .u.spi = I2S2ext
    }, {
        .type = SAI_PERIPH_TYPE_I2S,
        .u.spi = SPI3
    }, {
        .type = SAI_PERIPH_TYPE_I2S,
        .u.spi = I2S3ext
    }
};

sai_result_t sai_init(sai_t *obj, sai_init_t *init) {
    MBED_ASSERT((init != NULL) && (obj != NULL));
        
    SAIName base = (SAIName)pinmap_peripheral(init->sd, PinMap_SAI_SD);
    MBED_ASSERT(base != (SAIName)NC);
    
    obj->base = &sai_bases[base];
    obj->is_receiver = init->is_receiver;
    obj->sd = init->sd;
    return sai_vtable[obj->base->type]->init(obj, init);
}

/** Transfer a sample and return the sample received meanwhile. */
bool sai_xfer(sai_t *obj, uint32_t *sample) {
    return sai_vtable[obj->base->type]->xfer(obj, sample);
}

/** Releases & de-initialize the referenced peripherals. */
void sai_free(sai_t *obj) {
    sai_vtable[obj->base->type]->free(obj);
}

#endif // DEVICE_SAI
