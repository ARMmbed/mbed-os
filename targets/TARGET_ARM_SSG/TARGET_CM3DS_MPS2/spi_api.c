/* mbed Microcontroller Library
 * Copyright (c) 2017-2018 ARM Limited
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

#include "spi_api.h"
#include "pinmap.h"
#include "mbed_error.h"
#include "mbed_wait_api.h"
#include "platform_devices.h"


static const PinMap PinMap_SPI_SCLK[] = {
    {SPI_SCLK,         SPI_0, 0},
    {CLCD_SCLK,        SPI_1, 0},
    {ADC_SCLK,         SPI_2, ALTERNATE_FUNC},
    {SHIELD_0_SPI_SCK, SPI_3, ALTERNATE_FUNC},
    {SHIELD_1_SPI_SCK, SPI_4, ALTERNATE_FUNC},
    {NC,               NC,    0}
};

static const PinMap PinMap_SPI_MOSI[] = {
    {SPI_MOSI,          SPI_0, 0},
    {CLCD_MOSI,         SPI_1, 0},
    {ADC_MOSI,          SPI_2, ALTERNATE_FUNC},
    {SHIELD_0_SPI_MOSI, SPI_3, ALTERNATE_FUNC},
    {SHIELD_1_SPI_MOSI, SPI_4, ALTERNATE_FUNC},
    {NC,                NC,    0}
};

static const PinMap PinMap_SPI_MISO[] = {
    {SPI_MISO,          SPI_0, 0},
    {CLCD_MISO,         SPI_1, 0},
    {ADC_MISO,          SPI_2, ALTERNATE_FUNC},
    {SHIELD_0_SPI_MISO, SPI_3, ALTERNATE_FUNC},
    {SHIELD_1_SPI_MISO, SPI_4, ALTERNATE_FUNC},
    {NC,                NC,    0}
};

static const PinMap PinMap_SPI_SSEL[] = {
    {SPI_SSEL,         SPI_0, 0},
    {CLCD_SSEL,        SPI_1, 0},
    {ADC_SSEL,         SPI_2, ALTERNATE_FUNC},
    {SHIELD_0_SPI_nCS, SPI_3, ALTERNATE_FUNC},
    {SHIELD_1_SPI_nCS, SPI_4, ALTERNATE_FUNC},
    {NC,               NC,    0}
};

/* SPI configuration values */
#define SPI_BITS_MIN_VALUE          4
#define SPI_BITS_MAX_VALUE          16
#define SPI_MODE_PHASE_BIT          0
#define SPI_MODE_PHASE_BIT_MSK      (0x1ul << SPI_MODE_PHASE_BIT)
#define SPI_MODE_POLARITY_BIT       1
#define SPI_MODE_POLARITY_BIT_MSK   (0x1ul << SPI_MODE_POLARITY_BIT)
#define SPI_MODE_MAX_VALUE_MSK      ((0x1ul << (SPI_MODE_POLARITY_BIT+1))-1)

static uint32_t spi_fill_object(spi_t *obj, PinName mosi, PinName miso,
                                PinName sclk, PinName ssel)
{
    /* Determine the SPI to use */
    uint32_t spi_mosi = pinmap_peripheral(mosi, PinMap_SPI_MOSI);
    uint32_t spi_miso = pinmap_peripheral(miso, PinMap_SPI_MISO);
    uint32_t spi_sclk = pinmap_peripheral(sclk, PinMap_SPI_SCLK);
    uint32_t spi_ssel = pinmap_peripheral(ssel, PinMap_SPI_SSEL);
    uint32_t spi_data = pinmap_merge(spi_mosi, spi_miso);
    uint32_t spi_cntl = pinmap_merge(spi_sclk, spi_ssel);
    uint32_t spi_index = pinmap_merge(spi_data, spi_cntl);
    if ((spi_data == (uint32_t)NC) || (spi_index == (uint32_t)NC)) {
        /* Both miso and mosi or all 4 pins are NC */
        error("SPI pinout mapping failed");
        return 1;
    }

    switch (spi_index) {
#ifdef ARM_SPI0
        case SPI_0:
            obj->spi = &SPI0_PL022_DEV;
            return 0;
#endif /* ARM_SPI0 */
#ifdef ARM_SPI1
        case SPI_1:
            obj->spi = &SPI1_PL022_DEV;
            return 0;
#endif /* ARM_SPI1 */
#ifdef ARM_SPI2
        case SPI_2:
            obj->spi = &SPI2_PL022_DEV;
            return 0;
#endif /* ARM_SPI2 */
#ifdef ARM_SPI3
        case SPI_3:
            obj->spi = &SPI3_PL022_DEV;
            return 0;
#endif /* ARM_SPI3 */
#ifdef ARM_SPI4
        case SPI_4:
            obj->spi = &SPI4_PL022_DEV;
            return 0;
#endif /* ARM_SPI4 */
        default:
            error("Can not assign valid SPI peripheral to the pins given");
            return 1;
    }
}

void spi_init(spi_t *obj, PinName mosi, PinName miso, PinName sclk, PinName ssel)
{
    if (spi_fill_object(obj, mosi, miso, sclk, ssel) != 0) {
        return;
    }

    (void)spi_pl022_init(obj->spi, SystemCoreClock);

    /*
     * If the pins are not linked to a GPIO,
     * pin_function will have no effect.
     * Mosi, miso and ssel pins are allowed to be NC,
     * call pin_function only if they are connected
     */
    if (mosi != NC) {
        pin_function(mosi, pinmap_function(mosi, PinMap_SPI_MOSI));
    }
    if (miso != NC) {
        pin_function(miso, pinmap_function(miso, PinMap_SPI_MISO));
    }
    if (ssel != NC) {
        pin_function(ssel, pinmap_function(ssel, PinMap_SPI_SSEL));
    }
    pin_function(sclk, pinmap_function(sclk, PinMap_SPI_SCLK));
}

void spi_free(spi_t *obj)
{
    /* No need to implement free function, this API is intentionally blank */
}

void spi_format(spi_t *obj, int bits, int mode, int slave)
{
    uint32_t polarity, phase, frame_format;
    struct spi_pl022_ctrl_cfg_t ctrl_cfg;

    if (!(bits >= SPI_BITS_MIN_VALUE && bits <= SPI_BITS_MAX_VALUE) ||
        (mode & ~SPI_MODE_MAX_VALUE_MSK)) {
        error("SPI format error");
        return;
    }

    spi_pl022_dev_disable(obj->spi);
    if (spi_pl022_get_ctrl_cfg(obj->spi, &ctrl_cfg) != 0) {
        error("SPI not initialized");
        return;
    };

    polarity = (mode & SPI_MODE_POLARITY_BIT_MSK) ? 1u : 0;
    phase = (mode & SPI_MODE_PHASE_BIT_MSK) ? 1u : 0;
    frame_format = (SPI_PL022_CFG_FRF_MOT << 0 |
                    polarity << 6 |
                    phase << 7);

    ctrl_cfg.frame_format = (uint8_t) frame_format;
    ctrl_cfg.word_size = (uint8_t) bits;
    ctrl_cfg.spi_mode =
                  slave ? SPI_PL022_SLAVE_SELECT : SPI_PL022_MASTER_SELECT;

    if (spi_pl022_set_ctrl_cfg(obj->spi, &ctrl_cfg) != 0) {
        error("SPI configuration failed");
    }

    spi_pl022_dev_enable(obj->spi);
}

void spi_frequency(spi_t *obj, int hz)
{
    spi_pl022_dev_disable(obj->spi);

    obj->spi->data->ctrl_cfg.bit_rate = hz;
    if (spi_pl022_set_sys_clk(obj->spi, SystemCoreClock) != 0) {
        error("SPI frequency config failed");
    }

    spi_pl022_dev_enable(obj->spi);
}

int spi_master_write(spi_t *obj, int value)
{
    int32_t rx_data = 0;
    uint32_t size = 1;

    if(obj->spi->data->ctrl_cfg.word_size > 8) {
        size = 2;
    }

    if (spi_pl022_txrx_blocking(obj->spi, &value, &size, &rx_data, &size) ) {
        return 0;
    }

    return rx_data;
}

int spi_master_block_write(spi_t *obj, const char *tx_buffer, int tx_length,
                           char *rx_buffer, int rx_length, char write_fill)
{
    if (spi_pl022_txrx_blocking(obj->spi, tx_buffer, (uint32_t*)&tx_length,
                                          rx_buffer, (uint32_t*)&rx_length)) {
        return 0;
    }

    return ((tx_length > rx_length) ? tx_length : rx_length);
}

int spi_slave_receive(spi_t *obj)
{
    int32_t status = spi_pl022_get_status(obj->spi);
    /* Rx FIFO not empty and device not busy */
    int32_t ret = ((status & SPI_PL022_SSPSR_RNE_MSK) &&
                        !(status & SPI_PL022_SSPSR_BSY_MSK));
    return ret;
}

uint8_t spi_get_module(spi_t *obj)
{
    if (obj->spi == &SPI0_PL022_DEV) {
        return SPI_0;
    } else if (obj->spi == &SPI1_PL022_DEV) {
        return SPI_1;
    } else if (obj->spi == &SPI2_PL022_DEV) {
        return SPI_2;
    } else if (obj->spi == &SPI3_PL022_DEV) {
        return SPI_3;
    } else if (obj->spi == &SPI4_PL022_DEV) {
        return SPI_4;
    } else {
        error("SPI object is not initialized");
        return (SPI_NC);
    }
}

int spi_slave_read(spi_t *obj)
{
    while(spi_slave_receive(obj) == 0) {};
    return spi_pl022_slave_read(obj->spi);
}

void spi_slave_write(spi_t *obj, int value)
{
    (void)spi_pl022_write(obj->spi, SPI_PL022_SLAVE_SELECT, &value);
}

int spi_busy(spi_t *obj)
{
    int32_t status = spi_pl022_get_status(obj->spi);
    return (status & SPI_PL022_SSPSR_BSY_MSK);
}

const PinMap *spi_master_mosi_pinmap()
{
    return PinMap_SPI_MOSI;
}

const PinMap *spi_master_miso_pinmap()
{
    return PinMap_SPI_MISO;
}

const PinMap *spi_master_clk_pinmap()
{
    return PinMap_SPI_SCLK;
}

const PinMap *spi_master_cs_pinmap()
{
    return PinMap_SPI_SSEL;
}

const PinMap *spi_slave_mosi_pinmap()
{
    return PinMap_SPI_MOSI;
}

const PinMap *spi_slave_miso_pinmap()
{
    return PinMap_SPI_MISO;
}

const PinMap *spi_slave_clk_pinmap()
{
    return PinMap_SPI_SCLK;
}

const PinMap *spi_slave_cs_pinmap()
{
    return PinMap_SPI_SSEL;
}
