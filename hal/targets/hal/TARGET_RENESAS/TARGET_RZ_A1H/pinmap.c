/* mbed Microcontroller Library
 * Copyright (c) 2006-2013 ARM Limited
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
#include "pinmap.h"
#include "mbed_error.h"
#include "gpio_addrdefine.h"

PinName gpio_multi_guard = (PinName)NC; /* If set pin name here, setting of the "pin" is just one time */

typedef struct {
    PinName pin;
    int function;
    int pm;
} PinFunc;

static const PinFunc PIPC_0_tbl[] = {
//   pin      func     pm
    {P4_0   , 2      , -1}, /* TIOC0A */
    {P5_0   , 6      , -1}, /* TIOC0A */
    {P7_0   , 7      , -1}, /* TIOC0A */
    {P10_4  , 2      , -1}, /* TIOC0A */
    {P4_1   , 2      , -1}, /* TIOC0B */
    {P5_1   , 6      , -1}, /* TIOC0B */
    {P7_1   , 7      , -1}, /* TIOC0B */
    {P10_5  , 2      , -1}, /* TIOC0B */
    {P4_2   , 2      , -1}, /* TIOC0C */
    {P5_5   , 6      , -1}, /* TIOC0C */
    {P7_2   , 7      , -1}, /* TIOC0C */
    {P10_6  , 2      , -1}, /* TIOC0C */
    {P4_3   , 2      , -1}, /* TIOC0D */
    {P5_7   , 6      , -1}, /* TIOC0D */
    {P7_3   , 7      , -1}, /* TIOC0D */
    {P10_7  , 2      , -1}, /* TIOC0D */
    {P2_11  , 5      , -1}, /* TIOC1A */
    {P6_0   , 5      , -1}, /* TIOC1A */
    {P7_4   , 7      , -1}, /* TIOC1A */
    {P8_8   , 5      , -1}, /* TIOC1A */
    {P9_7   , 4      , -1}, /* TIOC1A */
    {P10_8  , 2      , -1}, /* TIOC1A */
    {P2_12  , 8      , -1}, /* TIOC1B */
    {P5_2   , 6      , -1}, /* TIOC1B */
    {P6_1   , 5      , -1}, /* TIOC1B */
    {P7_5   , 7      , -1}, /* TIOC1B */
    {P8_9   , 5      , -1}, /* TIOC1B */
    {P10_9  , 2      , -1}, /* TIOC1B */
    {P2_1   , 6      , -1}, /* TIOC2A */
    {P6_2   , 6      , -1}, /* TIOC2A */
    {P7_6   , 7      , -1}, /* TIOC2A */
    {P8_14  , 4      , -1}, /* TIOC2A */
    {P10_10 , 2      , -1}, /* TIOC2A */
    {P2_2   , 6      , -1}, /* TIOC2B */
    {P6_3   , 6      , -1}, /* TIOC2B */
    {P7_7   , 7      , -1}, /* TIOC2B */
    {P8_15  , 4      , -1}, /* TIOC2B */
    {P10_11 , 2      , -1}, /* TIOC2B */
    {P10_11 , 2      , -1}, /* TIOC2B */
    {P3_4   , 6      , -1}, /* TIOC3A */
    {P7_8   , 7      , -1}, /* TIOC3A */
    {P8_10  , 4      , -1}, /* TIOC3A */
    {P3_5   , 6      , -1}, /* TIOC3B */
    {P7_9   , 7      , -1}, /* TIOC3B */
    {P8_11  , 4      , -1}, /* TIOC3B */
    {P3_6   , 6      , -1}, /* TIOC3C */
    {P5_3   , 6      , -1}, /* TIOC3C */
    {P7_10  , 7      , -1}, /* TIOC3C */
    {P8_12  , 4      , -1}, /* TIOC3C */
    {P3_7   , 6      , -1}, /* TIOC3D */
    {P5_4   , 6      , -1}, /* TIOC3D */
    {P7_11  , 7      , -1}, /* TIOC3D */
    {P8_13  , 4      , -1}, /* TIOC3D */
    {P3_8   , 6      , -1}, /* TIOC4A */
    {P4_4   , 3      , -1}, /* TIOC4A */
    {P7_12  , 7      , -1}, /* TIOC4A */
    {P11_0  , 2      , -1}, /* TIOC4A */
    {P3_9   , 6      , -1}, /* TIOC4B */
    {P4_5   , 3      , -1}, /* TIOC4B */
    {P7_13  , 7      , -1}, /* TIOC4B */
    {P11_1  , 2      , -1}, /* TIOC4B */
    {P3_10  , 6      , -1}, /* TIOC4C */
    {P4_6   , 3      , -1}, /* TIOC4C */
    {P7_14  , 7      , -1}, /* TIOC4C */
    {P11_2  , 2      , -1}, /* TIOC4C */
    {P3_11  , 6      , -1}, /* TIOC4D */
    {P4_7   , 3      , -1}, /* TIOC4D */
    {P7_15  , 7      , -1}, /* TIOC4D */
    {P11_3  , 2      , -1}, /* TIOC4D */
    {P5_7   , 1      , 1 }, /* TXOUT0M   */
    {P5_6   , 1      , 1 }, /* TXOUT0P   */
    {P5_5   , 1      , 1 }, /* TXOUT1M   */
    {P5_4   , 1      , 1 }, /* TXOUT1P   */
    {P5_3   , 1      , 1 }, /* TXOUT2M   */
    {P5_2   , 1      , 1 }, /* TXOUT2P   */
    {P5_1   , 1      , 1 }, /* TXCLKOUTM */
    {P5_0   , 1      , 1 }, /* TXCLKOUTP */
    {P2_11  , 4      , 0 }, /* SSITxD0 */
    {P4_7   , 5      , 0 }, /* SSITxD0 */
    {P7_4   , 6      , 0 }, /* SSITxD1 */
    {P10_15 , 2      , 0 }, /* SSITxD1 */
    {P4_15  , 6      , 0 }, /* SSITxD3 */
    {P7_11  , 2      , 0 }, /* SSITxD3 */
    {P2_7   , 4      , 0 }, /* SSITxD5 */
    {P4_11  , 5      , 0 }, /* SSITxD5 */
    {P8_10  , 8      , 0 }, /* SSITxD5 */
    {P3_7   , 8      , 0 }, /* WDTOVF */
    {NC     , 0      , -1}
};

void pin_function(PinName pin, int function) {
    if (pin == (PinName)NC) return;
    
    int n = pin >> 4;
    int bitmask = 1<<(pin  & 0xf);
    const PinFunc * Pipc_0_func = PIPC_0_tbl;
    int pipc_data = 1;
    
    if (gpio_multi_guard != pin) {
        if (function == 0) {
            // means GPIO mode
            *PMC(n) &= ~bitmask;
        } else {
            // alt-function mode
            --function;

            if (function & (1 << 2)) { *PFCAE(n) |= bitmask;}else  { *PFCAE(n) &= ~bitmask;}
            if (function & (1 << 1)) { *PFCE(n) |= bitmask;}else  { *PFCE(n) &= ~bitmask;}
            if (function & (1 << 0)) { *PFC(n) |= bitmask;}else  { *PFC(n) &= ~bitmask;}

            while (Pipc_0_func->pin != NC) {
                if ((Pipc_0_func->pin == pin) && ((Pipc_0_func->function - 1) == function)) {
                    pipc_data = 0;
                    if (Pipc_0_func->pm == 0) {
                        *PMSR(n) = (bitmask << 16) | 0;
                    } else if (Pipc_0_func->pm == 1) {
                        *PMSR(n) = (bitmask << 16) | bitmask;
                    } else {
                        // Do Nothing
                    }
                    break;
                }
                Pipc_0_func++;
            }
            if (pipc_data == 1) {
            *PIPC(n) |= bitmask;
            } else {
                *PIPC(n) &= ~bitmask;
            }

            if (P1_0 <= pin && pin <= P1_7 && function == 0) {
                *PBDC(n) |= bitmask;
            }
            *PMC(n) |= bitmask;
        }
    } else {
        gpio_multi_guard = (PinName)NC;
    }
}

void pin_mode(PinName pin, PinMode mode) {
//    if (pin == (PinName)NC) { return; }
}
