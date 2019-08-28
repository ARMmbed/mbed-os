/*
 * Copyright (c) 2018 ARM Limited. All rights reserved.
 * SPDX-License-Identifier: Apache-2.0
 * Licensed under the Apache License, Version 2.0 (the License); you may
 * not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an AS IS BASIS, WITHOUT
 * WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#if defined(MBED_CONF_NANOSTACK_CONFIGURATION) && DEVICE_SPI && DEVICE_INTERRUPTIN && defined(MBED_CONF_RTOS_PRESENT)

#include "nanostack/platform/arm_hal_phy.h"
#include "rf_configuration.h"


// Note that F_XO and F_DIG depends on the used clock frequency
#define F_XO    50000000
#define F_DIG   25000000
// Note that reference divider depends on REFDIV field in XO_RCO_CONF0 register
#define REF_DIVIDER     1
// Note that band selector depends on BS field in SYNT3 register
#define BAND_SELECTOR   4
#define DEF_2EXP33  8589934592
#define DEF_2EXP20  1048576
#define DEF_2EXP19  524288
#define DEF_2EXP16  65536
#define DEF_2EXP15  32768
// Use multiplier for better resolution
#define RESOLUTION_MULTIPLIER   1000000

// RSSI_TH is a 8-bit register which can be converted to dBm using formula RSSI_TH-146
#define MIN_RSSI_THRESHOLD  -146
#define MAX_RSSI_THRESHOLD  109

void rf_conf_calculate_datarate_registers(uint32_t datarate, uint16_t *datarate_mantissa, uint8_t *datarate_exponent)
{
    uint64_t datarate_m = (uint64_t)datarate * DEF_2EXP33;
    uint8_t datarate_e = 1;
    while (datarate_m >= DEF_2EXP16) {
        datarate_e++;
        uint16_t var_2exp_datarate_e = (uint32_t)2 << (datarate_e - 1);
        datarate_m = (uint64_t)datarate * DEF_2EXP33;
        datarate_m = datarate_m / ((uint64_t)var_2exp_datarate_e * F_DIG);
        datarate_m -= DEF_2EXP16;
    }
    *datarate_mantissa = datarate_m;
    *datarate_exponent = datarate_e;
}

void rf_conf_calculate_base_frequency_registers(uint32_t frequency, uint8_t *synt3, uint8_t *synt2, uint8_t *synt1, uint8_t *synt0)
{
    uint64_t freq_tmp = (uint64_t)frequency * RESOLUTION_MULTIPLIER;
    freq_tmp = (freq_tmp / (F_XO / ((BAND_SELECTOR / 2) * REF_DIVIDER)));
    freq_tmp *= DEF_2EXP20;
    freq_tmp /= RESOLUTION_MULTIPLIER;
    *synt3 = (uint8_t)(freq_tmp >> 24);
    *synt2 = (uint8_t)(freq_tmp >> 16);
    *synt1 = (uint8_t)(freq_tmp >> 8);
    *synt0 = (uint8_t)freq_tmp;
}

void rf_conf_calculate_deviation_registers(uint32_t deviation, uint8_t *fdev_m, uint8_t *fdev_e)
{
    uint64_t fdev_m_tmp = 0xffff;
    uint8_t fdev_e_tmp = 1;

    while (fdev_m_tmp > 255) {
        fdev_e_tmp++;
        uint16_t var_2exp_datarate_e_minus_1 = (uint16_t)2 << ((fdev_e_tmp - 1) - 1);
        fdev_m_tmp = (uint64_t)deviation * RESOLUTION_MULTIPLIER;
        fdev_m_tmp = (((fdev_m_tmp / F_XO) * DEF_2EXP19 * BAND_SELECTOR * REF_DIVIDER * (8 / BAND_SELECTOR)) / var_2exp_datarate_e_minus_1);
        fdev_m_tmp += RESOLUTION_MULTIPLIER / 2;
        fdev_m_tmp /= RESOLUTION_MULTIPLIER;
        fdev_m_tmp -= 256;
    }
    *fdev_m = (uint8_t)fdev_m_tmp;
    *fdev_e = fdev_e_tmp;
}

int rf_conf_calculate_channel_spacing_registers(uint32_t channel_spacing, uint8_t *ch_space)
{
    uint64_t ch_space_tmp = (uint64_t)channel_spacing * RESOLUTION_MULTIPLIER;
    ch_space_tmp /= F_XO;
    ch_space_tmp *= DEF_2EXP15;
    ch_space_tmp += RESOLUTION_MULTIPLIER / 2;
    ch_space_tmp /= RESOLUTION_MULTIPLIER;
    // Check if channel spacing is too high
    if (ch_space_tmp > 255) {
        return -1;
    }
    *ch_space = (uint8_t)ch_space_tmp;
    return 0;
}

/* Note: This function doesn't necessarily give the optimal RX filter settings.
 * When accurate chflt_m and chflt_e settings are needed they must be computed manually.
 * Function uses undefined values (900000, 852000, ...)
 * to find the chflt_m and chflt_e settings from the RX filter table (see. S2-LP datasheet).
 *
 *         E=0    E=1    E=2    E=3    E=4    E=5    E=6    E=7    E=8    E=9
 *  M=0  800.1  450.9  224.7  112.3   56.1   28.0   14.0    7.0    3.5    1.8
 *  M=1  795.1  425.9  212.4  106.2   53.0   26.5   13.3    6.6    3.3    1.7
 *  M=2  768.4  403.2  201.1  100.5   50.2   25.1   12.6    6.3    3.1    1.6
 *  M=3  736.8  380.8  190.0   95.0   47.4   23.7   11.9    5.9    3.0    1.5
 *  M=4  705.1  362.1  180.7   90.3   45.1   22.6   11.3    5.6    2.8    1.4
 *  M=5  670.9  341.7  170.6   85.3   42.6   21.3   10.6    5.3    2.7    1.3
 *  M=6  642.3  325.4  162.4   81.2   40.6   20.3   10.1    5.1    2.5    1.3
 *  M=7  586.7  294.5  147.1   73.5   36.7   18.4    9.2    4.6    2.3    1.2
 *  M=8  541.4  270.3  135.0   67.5   33.7   16.9    8.4    4.2    2.1    1.1
 */
void rf_conf_calculate_rx_filter_bandwidth_registers(uint32_t rx_bandwidth, uint8_t *chflt_m, uint8_t *chflt_e)
{
    uint8_t chflt_e_tmp = 0;
    uint8_t chflt_m_tmp = 0;

    while (rx_bandwidth < 900000u / (2 << chflt_e_tmp)) {
        chflt_e_tmp++;
    }
    uint32_t rx_bandwidth_tmp = rx_bandwidth;
    if (chflt_e_tmp > 0) {
        rx_bandwidth_tmp = rx_bandwidth * (2 << (chflt_e_tmp - 1));
    }
    if (852000 > rx_bandwidth_tmp) {
        chflt_m_tmp++;
    }
    if (806000 > rx_bandwidth_tmp) {
        chflt_m_tmp++;
    }
    if (760000 > rx_bandwidth_tmp) {
        chflt_m_tmp++;
    }
    if (724000 > rx_bandwidth_tmp) {
        chflt_m_tmp++;
    }
    if (682000 > rx_bandwidth_tmp) {
        chflt_m_tmp++;
    }
    if (650000 > rx_bandwidth_tmp) {
        chflt_m_tmp++;
    }
    if (588000 > rx_bandwidth_tmp) {
        chflt_m_tmp++;
    }
    if (542000 > rx_bandwidth_tmp) {
        chflt_m_tmp++;
    }
    *chflt_m = chflt_m_tmp;
    *chflt_e = chflt_e_tmp;
}

int16_t rf_conf_cca_threshold_percent_to_rssi(uint8_t percent)
{
    uint8_t step = (MAX_RSSI_THRESHOLD-MIN_RSSI_THRESHOLD);
    return MIN_RSSI_THRESHOLD + (step * percent) / 100;
}

void rf_conf_calculate_rssi_threshold_registers(int16_t rssi_threshold, uint8_t *rssi_th)
{
    *rssi_th = rssi_threshold + RSSI_OFFSET;
}

/*
 * Function calculates deviation from given parameters for 2FSK and 2GFSK modulations.
 * Calculated using formula Deviation=(modulation_index*datarate)/2
 */
uint32_t rf_conf_calculate_deviation(phy_modulation_index_e modulation_index, uint32_t datarate)
{
    uint32_t deviation = 0;
    if (modulation_index == MODULATION_INDEX_0_5) {
        deviation = datarate / 4;
    } else if (modulation_index == MODULATION_INDEX_1_0) {
        deviation = datarate / 2;
    }
    return deviation;
}

#endif // MBED_CONF_NANOSTACK_CONFIGURATION && DEVICE_SPI && DEVICE_INTERRUPTIN && defined(MBED_CONF_RTOS_PRESENT)

