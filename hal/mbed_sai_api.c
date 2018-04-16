/* mbed Microcontroller Library
 * Copyright (c) 2018 ARM Limited
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

#include "hal/sai_api.h"

#if DEVICE_SAI

#include "platform/mbed_toolchain.h"
#include <string.h>

const sai_format_t sai_mode_i2s16 = {
    .bclk_polarity = true,
    .wclk_polarity = true,
    .ws_delay = true,
    .ws_length = 16,
    .frame_length = 1,
    .word_mask = 0,
    .word_length = 16,
    .data_length = 16,
    .lsb_first = false,
    .aligned_left = true,
    .bit_shift = 0
};

const sai_format_t sai_mode_i2s16w32 = {
    .bclk_polarity = true,
    .wclk_polarity = true,
    .ws_delay = true,
    .ws_length = 32,
    .frame_length = 1,
    .word_mask = 0,
    .word_length = 32,
    .data_length = 16,
    .lsb_first = false,
    .aligned_left = true,
    .bit_shift = 0
};

const sai_format_t sai_mode_i2s32 = {
    .bclk_polarity = true,
    .wclk_polarity = true,
    .ws_delay = true,
    .ws_length = 32,
    .frame_length = 1,
    .word_mask = 0,
    .word_length = 32,
    .data_length = 32,
    .lsb_first = false,
    .aligned_left = true,
    .bit_shift = 0
};

const sai_format_t sai_mode_pcm16l = {
    .bclk_polarity = true,
    .wclk_polarity = true,
    .ws_delay = true,
    .ws_length = 13,
    .frame_length = 1,
    .word_mask = 0,
    .word_length = 16,
    .data_length = 16,
    .lsb_first = false,
    .aligned_left = true,
    .bit_shift = 0
};

const sai_format_t sai_mode_pcm16s = {
    .bclk_polarity = true,
    .wclk_polarity = true,
    .ws_delay = true,
    .ws_length = 1,
    .frame_length = 1,
    .word_mask = 0,
    .word_length = 16,
    .data_length = 16,
    .lsb_first = false,
    .aligned_left = true,
    .bit_shift = 0
};


bool sai_check_sanity(sai_init_t *init) {
    return  (init != NULL) && 
            (init->sample_rate != 0) &&
            (init->format.ws_length <= init->format.word_length) &&
            (init->format.bit_shift <= (init->format.word_length - init->format.data_length)) &&
            (init->format.data_length <= init->format.word_length);
}

#endif // DEVICE_SAI
