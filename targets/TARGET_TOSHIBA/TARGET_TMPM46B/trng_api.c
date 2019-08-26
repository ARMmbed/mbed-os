/* mbed Microcontroller Library
 * (C)Copyright TOSHIBA ELECTRONIC DEVICES & STORAGE CORPORATION 2017 All rights reserved
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
#include "mbed_error.h"
#include "tmpm46b_esg.h"
#include "trng_api.h"

static Result ESG_Config(void)
{
    Result ret = ERROR;
    uint16_t Fintming = 560U;

    // Confirm the ESG core stops
    if (ESG_GetCalculationStatus() == ESG_CALCULATION_COMPLETE) {
        // Confirm no interrupt generation
        if (ESG_GetIntStatus() == DISABLE) {
            // Set the latch timing & output timing
            if ((ESG_SetLatchTiming(ESG_LATCH_TIMING_1) == SUCCESS) &&
                    (ESG_SetFintiming(Fintming) == SUCCESS)) {
                ret = SUCCESS;
            }
        }
    }
    return ret;
}

void trng_init(trng_t *obj)
{
    // Enable clock supply
    CG_SetFcPeriphB(CG_FC_PERIPH_ESG, ENABLE);
    // Initialise ESG core & Start up the ESG core
    if ((ESG_Config() == SUCCESS) && (ESG_Startup() == SUCCESS)) {
        obj->trng_init = true;
    } else {
        error("TRNG is not Initialised");
        obj->trng_init = false;
    }
}

void trng_free(trng_t *obj)
{
    ESG_IPReset();
    // Disable clock supply
    CG_SetFcPeriphB(CG_FC_PERIPH_ESG, DISABLE);
    obj->trng_init = false;
}

int trng_get_bytes(trng_t *obj, uint8_t *output, size_t length, size_t *output_length)
{
    volatile uint8_t random[64] = {0};
    uint8_t i;
    Result ret = ERROR;

    *output_length = 0;

    if (!obj->trng_init) {
        error("TRNG is not Initialised");
        return FAIL; // fail i.e. -1
    }

    while (ESG_GetIntStatus() == DISABLE) {
        // Wait for ESG core for generating an interrupt
    }
    // Interrupt clearing
    ret = ESG_ClrInt();
    if (ret == ERROR) {
        return FAIL; // fail i.e. -1
    }
    // Get the calculation result
    ESG_GetResult((uint32_t*)random); //512-bit entropy

    for (i = 0; (i < 64) && (i < length); i++) {
        *output++ = random[i];
        // clear the buffer
        random[i] = 0;
    }
    *output_length = i;
    //clear and restart the ESG core
    ESG_IPReset();
    ret |= ESG_Startup();
    if (ret == ERROR) {
        return FAIL; // fail i.e. -1
    }

    return ret;
}
