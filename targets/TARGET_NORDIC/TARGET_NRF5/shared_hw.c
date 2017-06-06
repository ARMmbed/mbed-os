 /* 
 * Copyright (c) 2017 Nordic Semiconductor ASA
 * All rights reserved.
 * 
 * Redistribution and use in source and binary forms, with or without modification,
 * are permitted provided that the following conditions are met:
 *
 *   1. Redistributions of source code must retain the above copyright notice, this list 
 *      of conditions and the following disclaimer.
 *
 *   2. Redistributions in binary form, except as embedded into a Nordic Semiconductor ASA 
 *      integrated circuit in a product or a software update for such product, must reproduce 
 *      the above copyright notice, this list of conditions and the following disclaimer in 
 *      the documentation and/or other materials provided with the distribution.
 *
 *   3. Neither the name of Nordic Semiconductor ASA nor the names of its contributors may be 
 *      used to endorse or promote products derived from this software without specific prior 
 *      written permission.
 *
 *   4. This software, with or without modification, must only be used with a 
 *      Nordic Semiconductor ASA integrated circuit.
 *
 *   5. Any software provided in binary or object form under this license must not be reverse 
 *      engineered, decompiled, modified and/or disassembled. 
 * 
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR
 * ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON
 * ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 * 
 */


#if (DEVICE_SPI || DEVICE_I2C)

#include <stdint.h>
#include "sdk_config.h"
#include "mbed_assert.h"
#include "mbed_error.h"

#include "shared_hw.h"


static const uint8_t m_shared_resource_map[] = {
    ((!!SPI0_ENABLED) << HW_RESOURCE_SPI) || ((!!TWI0_ENABLED) << HW_RESOURCE_TWI),
    ((!!SPI1_ENABLED) << HW_RESOURCE_SPI) || ((!!TWI1_ENABLED) << HW_RESOURCE_TWI),
    ((!!SPI2_ENABLED) << HW_RESOURCE_SPI)
};

#define SHARED_INSTANCES_CNT (sizeof(m_shared_resource_map)/sizeof(m_shared_resource_map[0]))

static uint8_t m_reservation = 0;


int8_t instance_hw_idx_get(hw_resource_type_t periph_type) {
    uint8_t mask  = 1;
    
    MBED_ASSERT(periph_type <= HW_RESOURCE_MAX);
    
    for (uint32_t i = 0; i < SHARED_INSTANCES_CNT; i++) {
        if (m_shared_resource_map[i] & (1 << periph_type)) {
            if  (!(m_reservation & mask)) {
                m_reservation |= mask;
                return i;
            }
        }
        
        mask <<= 1;
    }
    
    return -1;
}

void instance_hw_idx_free(uint8_t idx)
{
    MBED_ASSERT(idx < SHARED_INSTANCES_CNT);
    
    m_reservation &= (1 << idx);
}


#endif // (DEVICE_SPI || DEVICE_I2C)



