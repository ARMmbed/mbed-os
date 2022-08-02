/* mbed Microcontroller Library
 *******************************************************************************
 * Copyright (c) 2015, STMicroelectronics
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 * 1. Redistributions of source code must retain the above copyright notice,
 *    this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright notice,
 *    this list of conditions and the following disclaimer in the documentation
 *    and/or other materials provided with the distribution.
 * 3. Neither the name of STMicroelectronics nor the names of its contributors
 *    may be used to endorse or promote products derived from this software
 *    without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
 * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
 * SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
 * CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
 * OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
 * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *******************************************************************************
 */
#include "mbed_assert.h"
#include "mbed_error.h"
#include "mbed_debug.h"
#include "mbed_critical.h"
#include "mbed_wait_api.h"
#include "spi_api.h"

#if DEVICE_SPI
#include <stdbool.h>
#include <math.h>
#include <string.h>
#include "cmsis.h"
#include "pinmap.h"
#include "PeripheralPins.h"
#include "spi_device.h"

#if DEVICE_SPI_ASYNCH
#define SPI_INST(obj)    ((SPI_TypeDef *)(obj->spi.spi))
#else
#define SPI_INST(obj)    ((SPI_TypeDef *)(obj->spi))
#endif

#if DEVICE_SPI_ASYNCH
#define SPI_S(obj)    (( struct spi_s *)(&(obj->spi)))
#else
#define SPI_S(obj)    (( struct spi_s *)(obj))
#endif

#ifndef DEBUG_STDIO
#   define DEBUG_STDIO 0
#endif

#if DEBUG_STDIO
#   include <stdio.h>
#   define DEBUG_PRINTF(...) do { printf(__VA_ARGS__); } while(0)
#else
#   define DEBUG_PRINTF(...) {}
#endif

/* Consider 10ms as the default timeout for sending/receving 1 byte */
#define TIMEOUT_1_BYTE 10

#if defined(SPI_FLAG_FRLVL) // STM32F0 STM32F3 STM32F7 STM32L4
extern HAL_StatusTypeDef HAL_SPIEx_FlushRxFifo(SPI_HandleTypeDef *hspi);
#endif

#if defined(SPI_DATASIZE_17BIT) || defined(SPI_DATASIZE_18BIT) || defined(SPI_DATASIZE_19BIT) || defined(SPI_DATASIZE_20BIT) || \
    defined(SPI_DATASIZE_21BIT) || defined(SPI_DATASIZE_22BIT) || defined(SPI_DATASIZE_23BIT) || defined(SPI_DATASIZE_24BIT) || \
    defined(SPI_DATASIZE_25BIT) || defined(SPI_DATASIZE_26BIT) || defined(SPI_DATASIZE_27BIT) || defined(SPI_DATASIZE_28BIT) || \
    defined(SPI_DATASIZE_29BIT) || defined(SPI_DATASIZE_30BIT) || defined(SPI_DATASIZE_31BIT) || defined(SPI_DATASIZE_32BIT)

#define HAS_32BIT_SPI_TRANSFERS 1
#endif // SPI_DATASIZE_X

/**
 * Flush RX FIFO/input register of SPI interface and clear overrun flag.
 */
static inline void spi_flush_rx(spi_t *obj)
{
#if defined(SPI_FLAG_FRLVL)
    HAL_SPIEx_FlushRxFifo(&(SPI_S(obj)->handle));
#endif
    LL_SPI_ClearFlag_OVR(SPI_INST(obj));
}

void spi_get_capabilities(PinName ssel, bool slave, spi_capabilities_t *cap)
{
    if (slave) {
        cap->minimum_frequency = 200000;            // 200 kHz
        cap->maximum_frequency = 2000000;           // 2 MHz
        cap->word_length = 0x00008080;              // 8 and 16 bit symbols
        cap->support_slave_mode = false;            // to be determined later based on ssel
        cap->hw_cs_handle = false;                  // irrelevant in slave mode
        cap->slave_delay_between_symbols_ns = 2500; // 2.5 us
        cap->clk_modes = 0x0f;                      // all clock modes
        cap->tx_rx_buffers_equal_length = false;    // rx/tx buffers can have different sizes
#if DEVICE_SPI_ASYNCH
        cap->async_mode = true;
#else
        cap->async_mode = false;
#endif
    } else {
        cap->minimum_frequency = 200000;          // 200 kHz
        cap->maximum_frequency = 2000000;         // 2 MHz
        cap->word_length = STM32_SPI_CAPABILITY_WORD_LENGTH;            // Defined in spi_device.h
        cap->support_slave_mode = false;          // to be determined later based on ssel
        cap->hw_cs_handle = false;                // to be determined later based on ssel
        cap->slave_delay_between_symbols_ns = 0;  // irrelevant in master mode
        cap->clk_modes = 0x0f;                    // all clock modes
        cap->tx_rx_buffers_equal_length = false;  // rx/tx buffers can have different sizes
#if DEVICE_SPI_ASYNCH
        cap->async_mode = true;
#else
        cap->async_mode = false;
#endif
    }

    // check if given ssel pin is in the cs pinmap
    const PinMap *cs_pins = spi_master_cs_pinmap();
    while (cs_pins->pin != NC) {
        if (cs_pins->pin == ssel) {
#if DEVICE_SPISLAVE
            cap->support_slave_mode = true;
#endif
            cap->hw_cs_handle = true;
            break;
        }
        cs_pins++;
    }
}

void init_spi(spi_t *obj)
{
    struct spi_s *spiobj = SPI_S(obj);
    SPI_HandleTypeDef *handle = &(spiobj->handle);

    __HAL_SPI_DISABLE(handle);

    DEBUG_PRINTF("init_spi: instance=0x%8X\r\n", (int)handle->Instance);
    if (HAL_SPI_Init(handle) != HAL_OK) {
        error("Cannot initialize SPI");
    }
    /* In some cases after SPI object re-creation SPI overrun flag may not
     * be cleared, so clear RX data explicitly to prevent any transmissions errors */
    spi_flush_rx(obj);
    /* In case of standard 4 wires SPI,PI can be kept enabled all time
     * and SCK will only be generated during the write operations. But in case
     * of 3 wires, it should be only enabled during rd/wr unitary operations,
     * which is handled inside STM32 HAL layer.
     */
    if (handle->Init.Direction  == SPI_DIRECTION_2LINES) {
        __HAL_SPI_ENABLE(handle);
    }
}

SPIName spi_get_peripheral_name(PinName mosi, PinName miso, PinName sclk)
{
    SPIName spi_mosi = (SPIName)pinmap_peripheral(mosi, PinMap_SPI_MOSI);
    SPIName spi_miso = (SPIName)pinmap_peripheral(miso, PinMap_SPI_MISO);
    SPIName spi_sclk = (SPIName)pinmap_peripheral(sclk, PinMap_SPI_SCLK);

    SPIName spi_per;

    // MISO or MOSI may be not connected
    if (miso == NC) {
        spi_per = (SPIName)pinmap_merge(spi_mosi, spi_sclk);
    } else if (mosi == NC) {
        spi_per = (SPIName)pinmap_merge(spi_miso, spi_sclk);
    } else {
        SPIName spi_data = (SPIName)pinmap_merge(spi_mosi, spi_miso);
        spi_per = (SPIName)pinmap_merge(spi_data, spi_sclk);
    }

    return spi_per;
}

#if STATIC_PINMAP_READY
#define SPI_INIT_DIRECT spi_init_direct
void spi_init_direct(spi_t *obj, const spi_pinmap_t *pinmap)
#else
#define SPI_INIT_DIRECT _spi_init_direct
static void _spi_init_direct(spi_t *obj, const spi_pinmap_t *pinmap)
#endif
{
    struct spi_s *spiobj = SPI_S(obj);
    SPI_HandleTypeDef *handle = &(spiobj->handle);

    spiobj->spi = (SPIName)pinmap->peripheral;
    MBED_ASSERT(spiobj->spi != (SPIName)NC);

#if defined(SPI_IP_VERSION_V2)
    RCC_PeriphCLKInitTypeDef PeriphClkInit = {0};
#endif /* SPI_IP_VERSION_V2 */

#if defined SPI1_BASE
    // Enable SPI clock
    if (spiobj->spi == SPI_1) {
#if defined(SPI_IP_VERSION_V2)
        PeriphClkInit.PeriphClockSelection = RCC_PERIPHCLK_SPI1;
#if defined (RCC_SPI123CLKSOURCE_PLL)
        PeriphClkInit.Spi123ClockSelection = RCC_SPI123CLKSOURCE_PLL;
#else
        PeriphClkInit.Spi1ClockSelection = RCC_SPI1CLKSOURCE_SYSCLK;
#endif
        if (HAL_RCCEx_PeriphCLKConfig(&PeriphClkInit) != HAL_OK) {
            error("HAL_RCCEx_PeriphCLKConfig\n");
        }
#endif /* SPI_IP_VERSION_V2 */

        __HAL_RCC_SPI1_FORCE_RESET();
        __HAL_RCC_SPI1_RELEASE_RESET();
        __HAL_RCC_SPI1_CLK_ENABLE();
        spiobj->spiIRQ = SPI1_IRQn;
    }
#endif

#if defined SPI2_BASE
    if (spiobj->spi == SPI_2) {
#if defined(SPI_IP_VERSION_V2)
        PeriphClkInit.PeriphClockSelection = RCC_PERIPHCLK_SPI2;
#if defined (RCC_SPI123CLKSOURCE_PLL)
        PeriphClkInit.Spi123ClockSelection = RCC_SPI123CLKSOURCE_PLL;
#else
        PeriphClkInit.Spi2ClockSelection = RCC_SPI2CLKSOURCE_SYSCLK;
#endif
        if (HAL_RCCEx_PeriphCLKConfig(&PeriphClkInit) != HAL_OK) {
            error("HAL_RCCEx_PeriphCLKConfig\n");
        }
#endif /* SPI_IP_VERSION_V2 */

        __HAL_RCC_SPI2_FORCE_RESET();
        __HAL_RCC_SPI2_RELEASE_RESET();
        __HAL_RCC_SPI2_CLK_ENABLE();
        spiobj->spiIRQ = SPI2_IRQn;
    }
#endif

#if defined SPI3_BASE
    if (spiobj->spi == SPI_3) {
#if defined(SPI_IP_VERSION_V2)
        PeriphClkInit.PeriphClockSelection = RCC_PERIPHCLK_SPI3;
#if defined (RCC_SPI123CLKSOURCE_PLL)
        PeriphClkInit.Spi123ClockSelection = RCC_SPI123CLKSOURCE_PLL;
#else
        PeriphClkInit.Spi3ClockSelection = RCC_SPI3CLKSOURCE_SYSCLK;
#endif
        if (HAL_RCCEx_PeriphCLKConfig(&PeriphClkInit) != HAL_OK) {
            error("HAL_RCCEx_PeriphCLKConfig\n");
        }
#endif /* SPI_IP_VERSION_V2 */

        __HAL_RCC_SPI3_FORCE_RESET();
        __HAL_RCC_SPI3_RELEASE_RESET();
        __HAL_RCC_SPI3_CLK_ENABLE();
        spiobj->spiIRQ = SPI3_IRQn;
    }
#endif

#if defined SPI4_BASE
    if (spiobj->spi == SPI_4) {
#if defined(SPI_IP_VERSION_V2)
        PeriphClkInit.PeriphClockSelection = RCC_PERIPHCLK_SPI4;
        PeriphClkInit.Spi45ClockSelection = RCC_SPI45CLKSOURCE_PCLK1;
        if (HAL_RCCEx_PeriphCLKConfig(&PeriphClkInit) != HAL_OK) {
            error("HAL_RCCEx_PeriphCLKConfig\n");
        }
#endif /* SPI_IP_VERSION_V2 */

        __HAL_RCC_SPI4_FORCE_RESET();
        __HAL_RCC_SPI4_RELEASE_RESET();
        __HAL_RCC_SPI4_CLK_ENABLE();
        spiobj->spiIRQ = SPI4_IRQn;
    }
#endif

#if defined SPI5_BASE
    if (spiobj->spi == SPI_5) {
#if defined(SPI_IP_VERSION_V2)
        PeriphClkInit.PeriphClockSelection = RCC_PERIPHCLK_SPI5;
        PeriphClkInit.Spi45ClockSelection = RCC_SPI45CLKSOURCE_PCLK1;
        if (HAL_RCCEx_PeriphCLKConfig(&PeriphClkInit) != HAL_OK) {
            error("HAL_RCCEx_PeriphCLKConfig\n");
        }
#endif /* SPI_IP_VERSION_V2 */

        __HAL_RCC_SPI5_FORCE_RESET();
        __HAL_RCC_SPI5_RELEASE_RESET();
        __HAL_RCC_SPI5_CLK_ENABLE();
        spiobj->spiIRQ = SPI5_IRQn;
    }
#endif

#if defined SPI6_BASE
    if (spiobj->spi == SPI_6) {
#if defined(SPI_IP_VERSION_V2)
        PeriphClkInit.PeriphClockSelection = RCC_PERIPHCLK_SPI6;
        PeriphClkInit.Spi6ClockSelection = RCC_SPI6CLKSOURCE_PCLK4;
        if (HAL_RCCEx_PeriphCLKConfig(&PeriphClkInit) != HAL_OK) {
            error("HAL_RCCEx_PeriphCLKConfig\n");
        }
#endif /* SPI_IP_VERSION_V2 */

        __HAL_RCC_SPI6_FORCE_RESET();
        __HAL_RCC_SPI6_RELEASE_RESET();
        __HAL_RCC_SPI6_CLK_ENABLE();
        spiobj->spiIRQ = SPI6_IRQn;
    }
#endif

    // Configure the SPI pins
    pin_function(pinmap->mosi_pin, pinmap->mosi_function);
    pin_mode(pinmap->mosi_pin, PullDown);  // Pull Down is set for output line

    pin_function(pinmap->miso_pin, pinmap->miso_function);
    pin_mode(pinmap->miso_pin, PullNone);

    pin_function(pinmap->sclk_pin, pinmap->sclk_function);
    pin_mode(pinmap->sclk_pin, PullNone);

    spiobj->pin_miso = pinmap->miso_pin;
    spiobj->pin_mosi = pinmap->mosi_pin;
    spiobj->pin_sclk = pinmap->sclk_pin;
    spiobj->pin_ssel = pinmap->ssel_pin;
    if (pinmap->ssel_pin != NC) {
        pin_function(pinmap->ssel_pin, pinmap->ssel_function);
        pin_mode(pinmap->ssel_pin, PullNone);
        handle->Init.NSS = SPI_NSS_HARD_OUTPUT;
#if defined(SPI_NSS_PULSE_ENABLE)
        handle->Init.NSSPMode = SPI_NSS_PULSE_ENABLE;
#endif
    } else {
        handle->Init.NSS = SPI_NSS_SOFT;
#if defined(SPI_NSS_PULSE_DISABLE)
        handle->Init.NSSPMode = SPI_NSS_PULSE_DISABLE;
#endif
    }

    /* Fill default value */
    handle->Instance = SPI_INST(obj);
    handle->Init.Mode              = SPI_MODE_MASTER;
    handle->Init.BaudRatePrescaler = SPI_BAUDRATEPRESCALER_256;

    if (pinmap->miso_pin != NC) {
        handle->Init.Direction     = SPI_DIRECTION_2LINES;
    } else {
        handle->Init.Direction      = SPI_DIRECTION_1LINE;
    }

    handle->Init.CLKPhase          = SPI_PHASE_1EDGE;
    handle->Init.CLKPolarity       = SPI_POLARITY_LOW;
    handle->Init.CRCCalculation    = SPI_CRCCALCULATION_DISABLE;
    handle->Init.CRCPolynomial     = 7;
#if defined(SPI_CRC_LENGTH_DATASIZE)
    handle->Init.CRCLength         = SPI_CRC_LENGTH_DATASIZE;
#endif
    handle->Init.DataSize          = SPI_DATASIZE_8BIT;
    handle->Init.FirstBit          = SPI_FIRSTBIT_MSB;
    handle->Init.TIMode            = SPI_TIMODE_DISABLE;

#if defined (SPI_IP_VERSION_V2)
    handle->Init.NSSPolarity = SPI_NSS_POLARITY_LOW;
    handle->Init.MasterKeepIOState = SPI_MASTER_KEEP_IO_STATE_ENABLE;
    handle->Init.FifoThreshold     = SPI_FIFO_THRESHOLD_01DATA;
    handle->Init.TxCRCInitializationPattern = SPI_CRC_INITIALIZATION_ALL_ZERO_PATTERN;
    handle->Init.RxCRCInitializationPattern = SPI_CRC_INITIALIZATION_ALL_ZERO_PATTERN;
    handle->Init.MasterSSIdleness = SPI_MASTER_SS_IDLENESS_00CYCLE;
    handle->Init.MasterInterDataIdleness = SPI_MASTER_INTERDATA_IDLENESS_00CYCLE;
    handle->Init.MasterReceiverAutoSusp = SPI_MASTER_RX_AUTOSUSP_DISABLE;
    handle->Init.IOSwap = SPI_IO_SWAP_DISABLE;
#if defined(SPI_RDY_MASTER_MANAGEMENT_INTERNALLY)
    handle->Init.ReadyMasterManagement = SPI_RDY_MASTER_MANAGEMENT_INTERNALLY;
    handle->Init.ReadyPolarity = SPI_RDY_POLARITY_HIGH;
#endif
#endif /* SPI_IP_VERSION_V2 */

    /*
    * According the STM32 Datasheet for SPI peripheral we need to PULLDOWN
    * or PULLUP the SCK pin according the polarity used.
    */
    pin_mode(spiobj->pin_sclk, (handle->Init.CLKPolarity == SPI_POLARITY_LOW) ? PullDown : PullUp);

    init_spi(obj);
}

void spi_init(spi_t *obj, PinName mosi, PinName miso, PinName sclk, PinName ssel)
{
    // determine the SPI to use
    uint32_t spi_mosi = pinmap_peripheral(mosi, PinMap_SPI_MOSI);
    uint32_t spi_miso = pinmap_peripheral(miso, PinMap_SPI_MISO);
    uint32_t spi_sclk = pinmap_peripheral(sclk, PinMap_SPI_SCLK);
    uint32_t spi_ssel = pinmap_peripheral(ssel, PinMap_SPI_SSEL);
    uint32_t spi_data = pinmap_merge(spi_mosi, spi_miso);
    uint32_t spi_cntl = pinmap_merge(spi_sclk, spi_ssel);

    int peripheral = (int)pinmap_merge(spi_data, spi_cntl);

    // pin out the spi pins
    int mosi_function = (int)pinmap_find_function(mosi, PinMap_SPI_MOSI);
    int miso_function = (int)pinmap_find_function(miso, PinMap_SPI_MISO);
    int sclk_function = (int)pinmap_find_function(sclk, PinMap_SPI_SCLK);
    int ssel_function = (int)pinmap_find_function(ssel, PinMap_SPI_SSEL);

    const spi_pinmap_t explicit_spi_pinmap = {peripheral, mosi, mosi_function, miso, miso_function, sclk, sclk_function, ssel, ssel_function};

    SPI_INIT_DIRECT(obj, &explicit_spi_pinmap);
}

void spi_free(spi_t *obj)
{
    struct spi_s *spiobj = SPI_S(obj);
    SPI_HandleTypeDef *handle = &(spiobj->handle);

    DEBUG_PRINTF("spi_free\r\n");

    __HAL_SPI_DISABLE(handle);
    HAL_SPI_DeInit(handle);

#if defined(DUAL_CORE) && (TARGET_STM32H7)
    while (LL_HSEM_1StepLock(HSEM, CFG_HW_RCC_SEMID)) {
    }
#endif /* DUAL_CORE */
#if defined SPI1_BASE
    // Reset SPI and disable clock
    if (spiobj->spi == SPI_1) {
        __HAL_RCC_SPI1_FORCE_RESET();
        __HAL_RCC_SPI1_RELEASE_RESET();
        __HAL_RCC_SPI1_CLK_DISABLE();
    }
#endif
#if defined SPI2_BASE
    if (spiobj->spi == SPI_2) {
        __HAL_RCC_SPI2_FORCE_RESET();
        __HAL_RCC_SPI2_RELEASE_RESET();
        __HAL_RCC_SPI2_CLK_DISABLE();
    }
#endif

#if defined SPI3_BASE
    if (spiobj->spi == SPI_3) {
        __HAL_RCC_SPI3_FORCE_RESET();
        __HAL_RCC_SPI3_RELEASE_RESET();
        __HAL_RCC_SPI3_CLK_DISABLE();
    }
#endif

#if defined SPI4_BASE
    if (spiobj->spi == SPI_4) {
        __HAL_RCC_SPI4_FORCE_RESET();
        __HAL_RCC_SPI4_RELEASE_RESET();
        __HAL_RCC_SPI4_CLK_DISABLE();
    }
#endif

#if defined SPI5_BASE
    if (spiobj->spi == SPI_5) {
        __HAL_RCC_SPI5_FORCE_RESET();
        __HAL_RCC_SPI5_RELEASE_RESET();
        __HAL_RCC_SPI5_CLK_DISABLE();
    }
#endif

#if defined SPI6_BASE
    if (spiobj->spi == SPI_6) {
        __HAL_RCC_SPI6_FORCE_RESET();
        __HAL_RCC_SPI6_RELEASE_RESET();
        __HAL_RCC_SPI6_CLK_DISABLE();
    }
#endif
#if defined(DUAL_CORE) && (TARGET_STM32H7)
    LL_HSEM_ReleaseLock(HSEM, CFG_HW_RCC_SEMID, HSEM_CR_COREID_CURRENT);
#endif /* DUAL_CORE */

    // Configure GPIOs back to reset value
    pin_function(spiobj->pin_miso, STM_PIN_DATA(STM_MODE_ANALOG, GPIO_NOPULL, 0));
    pin_function(spiobj->pin_mosi, STM_PIN_DATA(STM_MODE_ANALOG, GPIO_NOPULL, 0));
    pin_function(spiobj->pin_sclk, STM_PIN_DATA(STM_MODE_ANALOG, GPIO_NOPULL, 0));
    if (handle->Init.NSS != SPI_NSS_SOFT) {
        pin_function(spiobj->pin_ssel, STM_PIN_DATA(STM_MODE_ANALOG, GPIO_NOPULL, 0));
    }
}

void spi_format(spi_t *obj, int bits, int mode, int slave)
{
    struct spi_s *spiobj = SPI_S(obj);
    SPI_HandleTypeDef *handle = &(spiobj->handle);
    PinMode pull = PullNone;

    DEBUG_PRINTF("spi_format, bits:%d, mode:%d, slave?:%d\r\n", bits, mode, slave);

    // Save new values
    uint32_t DataSize;
    switch (bits) {
#if defined(SPI_DATASIZE_4BIT)
        case 4:
            DataSize = SPI_DATASIZE_4BIT;
            break;
#endif
#if defined(SPI_DATASIZE_5BIT)
        case 5:
            DataSize = SPI_DATASIZE_5BIT;
            break;
#endif
#if defined(SPI_DATASIZE_6BIT)
        case 6:
            DataSize = SPI_DATASIZE_6BIT;
            break;
#endif
#if defined(SPI_DATASIZE_7BIT)
        case 7:
            DataSize = SPI_DATASIZE_7BIT;
            break;
#endif
#if defined(SPI_DATASIZE_9BIT)
        case 9:
            DataSize = SPI_DATASIZE_9BIT;
            break;
#endif
#if defined(SPI_DATASIZE_10BIT)
        case 10:
            DataSize = SPI_DATASIZE_10BIT;
            break;
#endif
#if defined(SPI_DATASIZE_11BIT)
        case 11:
            DataSize = SPI_DATASIZE_11BIT;
            break;
#endif
#if defined(SPI_DATASIZE_12BIT)
        case 12:
            DataSize = SPI_DATASIZE_12BIT;
            break;
#endif
#if defined(SPI_DATASIZE_13BIT)
        case 13:
            DataSize = SPI_DATASIZE_13BIT;
            break;
#endif
#if defined(SPI_DATASIZE_14BIT)
        case 14:
            DataSize = SPI_DATASIZE_14BIT;
            break;
#endif
#if defined(SPI_DATASIZE_15BIT)
        case 15:
            DataSize = SPI_DATASIZE_15BIT;
            break;
#endif
#if defined(SPI_DATASIZE_17BIT)
        case 17:
            DataSize = SPI_DATASIZE_17BIT;
            break;
#endif
#if defined(SPI_DATASIZE_18BIT)
        case 18:
            DataSize = SPI_DATASIZE_18BIT;
            break;
#endif
#if defined(SPI_DATASIZE_19BIT)
        case 19:
            DataSize = SPI_DATASIZE_19BIT;
            break;
#endif
#if defined(SPI_DATASIZE_20BIT)
        case 20:
            DataSize = SPI_DATASIZE_20BIT;
            break;
#endif
#if defined(SPI_DATASIZE_21BIT)
        case 21:
            DataSize = SPI_DATASIZE_21BIT;
            break;
#endif
#if defined(SPI_DATASIZE_22BIT)
        case 22:
            DataSize = SPI_DATASIZE_22BIT;
            break;
#endif
#if defined(SPI_DATASIZE_23BIT)
        case 23:
            DataSize = SPI_DATASIZE_23BIT;
            break;
#endif
#if defined(SPI_DATASIZE_24BIT)
        case 24:
            DataSize = SPI_DATASIZE_24BIT;
            break;
#endif
#if defined(SPI_DATASIZE_25BIT)
        case 25:
            DataSize = SPI_DATASIZE_25BIT;
            break;
#endif
#if defined(SPI_DATASIZE_26BIT)
        case 26:
            DataSize = SPI_DATASIZE_26BIT;
            break;
#endif
#if defined(SPI_DATASIZE_27BIT)
        case 27:
            DataSize = SPI_DATASIZE_27BIT;
            break;
#endif
#if defined(SPI_DATASIZE_28BIT)
        case 28:
            DataSize = SPI_DATASIZE_28BIT;
            break;
#endif
#if defined(SPI_DATASIZE_29BIT)
        case 29:
            DataSize = SPI_DATASIZE_29BIT;
            break;
#endif
#if defined(SPI_DATASIZE_30BIT)
        case 30:
            DataSize = SPI_DATASIZE_30BIT;
            break;
#endif
#if defined(SPI_DATASIZE_31BIT)
        case 31:
            DataSize = SPI_DATASIZE_31BIT;
            break;
#endif
#if defined(SPI_DATASIZE_32BIT)
        case 32:
            DataSize = SPI_DATASIZE_32BIT;
            break;
#endif
        case 16:
            DataSize = SPI_DATASIZE_16BIT;
            break;
        // 8 bits is the default for anything not found before
        default:
            DataSize = SPI_DATASIZE_8BIT;
            break;
    }

    handle->Init.DataSize = DataSize;

    switch (mode) {
        case 0:
            handle->Init.CLKPolarity = SPI_POLARITY_LOW;
            handle->Init.CLKPhase = SPI_PHASE_1EDGE;
            break;
        case 1:
            handle->Init.CLKPolarity = SPI_POLARITY_LOW;
            handle->Init.CLKPhase = SPI_PHASE_2EDGE;
            break;
        case 2:
            handle->Init.CLKPolarity = SPI_POLARITY_HIGH;
            handle->Init.CLKPhase = SPI_PHASE_1EDGE;
            break;
        default:
            handle->Init.CLKPolarity = SPI_POLARITY_HIGH;
            handle->Init.CLKPhase = SPI_PHASE_2EDGE;
            break;
    }

    if (handle->Init.NSS != SPI_NSS_SOFT) {
        handle->Init.NSS = (slave) ? SPI_NSS_HARD_INPUT : SPI_NSS_HARD_OUTPUT;
    }

    if (slave) {
        handle->Init.Mode = SPI_MODE_SLAVE;

        if (handle->Init.Direction == SPI_DIRECTION_1LINE) {
            /*  SPI slave implemtation in MBED does not support the 3 wires SPI.
             *  (e.g. when MISO is not connected). So we're forcing slave in
             *  2LINES mode. As MISO is not connected, slave will only read
             *  from master, and cannot write to it. Inform user.
             */
            debug("3 wires SPI slave not supported - slave will only read\r\n");
            handle->Init.Direction = SPI_DIRECTION_2LINES;
        }

        pin_mode(spiobj->pin_mosi, PullNone);
        pin_mode(spiobj->pin_miso, PullDown);  // Pull Down is set for output line
    }

    /*
    * According the STM32 Datasheet for SPI peripheral we need to PULLDOWN
    * or PULLUP the SCK pin according the polarity used.
    */
    pull = (handle->Init.CLKPolarity == SPI_POLARITY_LOW) ? PullDown : PullUp;
    pin_mode(spiobj->pin_sclk, pull);

    init_spi(obj);
}

/*
 * Only the IP clock input is family dependant so it computed
 * separately in spi_get_clock_freq
 */
extern int spi_get_clock_freq(spi_t *obj);

static const uint32_t baudrate_prescaler_table[] =  {SPI_BAUDRATEPRESCALER_2,
                                                     SPI_BAUDRATEPRESCALER_4,
                                                     SPI_BAUDRATEPRESCALER_8,
                                                     SPI_BAUDRATEPRESCALER_16,
                                                     SPI_BAUDRATEPRESCALER_32,
                                                     SPI_BAUDRATEPRESCALER_64,
                                                     SPI_BAUDRATEPRESCALER_128,
                                                     SPI_BAUDRATEPRESCALER_256
                                                    };

/**
 * Convert SPI_BAUDRATEPRESCALER_<X> constant into numeric prescaler rank.
 */
static uint8_t spi_get_baudrate_prescaler_rank(uint32_t value)
{
    switch (value) {
        case SPI_BAUDRATEPRESCALER_2:
            return 0;
        case SPI_BAUDRATEPRESCALER_4:
            return 1;
        case SPI_BAUDRATEPRESCALER_8:
            return 2;
        case SPI_BAUDRATEPRESCALER_16:
            return 3;
        case SPI_BAUDRATEPRESCALER_32:
            return 4;
        case SPI_BAUDRATEPRESCALER_64:
            return 5;
        case SPI_BAUDRATEPRESCALER_128:
            return 6;
        case SPI_BAUDRATEPRESCALER_256:
            return 7;
        default:
            return 0xFF;
    }
}

/**
 * Get actual SPI baudrate.
 *
 * It may differ from a value that is passed to the ::spi_frequency function.
 */
int spi_get_baudrate(spi_t *obj)
{
    struct spi_s *spiobj = SPI_S(obj);
    SPI_HandleTypeDef *handle = &(spiobj->handle);

    int freq = spi_get_clock_freq(obj);
    uint8_t baudrate_rank = spi_get_baudrate_prescaler_rank(handle->Init.BaudRatePrescaler);
    MBED_ASSERT(baudrate_rank != 0xFF);
    return freq >> (baudrate_rank + 1);
}


void spi_frequency(spi_t *obj, int hz)
{
    struct spi_s *spiobj = SPI_S(obj);
    int spi_hz = 0;
    uint8_t prescaler_rank = 0;
    uint8_t last_index = (sizeof(baudrate_prescaler_table) / sizeof(baudrate_prescaler_table[0])) - 1;
    SPI_HandleTypeDef *handle = &(spiobj->handle);

    /* Calculate the spi clock for prescaler_rank 0: SPI_BAUDRATEPRESCALER_2 */
    spi_hz = spi_get_clock_freq(obj) / 2;

    /* Define pre-scaler in order to get highest available frequency below requested frequency */
    while ((spi_hz > hz) && (prescaler_rank < last_index)) {
        spi_hz = spi_hz / 2;
        prescaler_rank++;
    }

    /*  Use the best fit pre-scaler */
    handle->Init.BaudRatePrescaler = baudrate_prescaler_table[prescaler_rank];

    /*  In case maximum pre-scaler still gives too high freq, raise an error */
    if (spi_hz > hz) {
        DEBUG_PRINTF("WARNING: lowest SPI freq (%d)  higher than requested (%d)\r\n", spi_hz, hz);
    }

    DEBUG_PRINTF("spi_frequency, request:%d, select:%d\r\n", hz, spi_hz);

    init_spi(obj);
}

static inline int ssp_readable(spi_t *obj)
{
    int status;
    struct spi_s *spiobj = SPI_S(obj);
    SPI_HandleTypeDef *handle = &(spiobj->handle);

    // Check if data is received
#if defined(SPI_IP_VERSION_V2)
    status = ((__HAL_SPI_GET_FLAG(handle, SPI_FLAG_RXP) != RESET) ? 1 : 0);
#else /* SPI_IP_VERSION_V2 */
    status = ((__HAL_SPI_GET_FLAG(handle, SPI_FLAG_RXNE) != RESET) ? 1 : 0);
#endif /* SPI_IP_VERSION_V2 */
    return status;
}

static inline int ssp_writeable(spi_t *obj)
{
    int status;
    struct spi_s *spiobj = SPI_S(obj);
    SPI_HandleTypeDef *handle = &(spiobj->handle);

    // Check if data is transmitted
#if defined(SPI_IP_VERSION_V2)
    status = ((__HAL_SPI_GET_FLAG(handle, SPI_FLAG_TXP) != RESET) ? 1 : 0);
#else /* SPI_IP_VERSION_V2 */
    status = ((__HAL_SPI_GET_FLAG(handle, SPI_FLAG_TXE) != RESET) ? 1 : 0);
#endif /* SPI_IP_VERSION_V2 */

    return status;
}

static inline int ssp_busy(spi_t *obj)
{
    int status;
    struct spi_s *spiobj = SPI_S(obj);
    SPI_HandleTypeDef *handle = &(spiobj->handle);
#if defined(SPI_IP_VERSION_V2)
    status = ((__HAL_SPI_GET_FLAG(handle, SPI_FLAG_RXWNE) != RESET) ? 1 : 0);
#else /* SPI_IP_VERSION_V2 */
    status = ((__HAL_SPI_GET_FLAG(handle, SPI_FLAG_BSY) != RESET) ? 1 : 0);
#endif /* SPI_IP_VERSION_V2 */
    return status;
}

static inline int datasize_to_transfer_bitshift(uint32_t DataSize)
{
    switch (DataSize) {
#if defined(SPI_DATASIZE_4BIT)
        case SPI_DATASIZE_4BIT:
#endif
#if defined(SPI_DATASIZE_5BIT)
        case SPI_DATASIZE_5BIT:
#endif
#if defined(SPI_DATASIZE_6BIT)
        case SPI_DATASIZE_6BIT:
#endif
#if defined(SPI_DATASIZE_7BIT)
        case SPI_DATASIZE_7BIT:
#endif
        case SPI_DATASIZE_8BIT:
            return 0;
#if defined(SPI_DATASIZE_9BIT)
        case SPI_DATASIZE_9BIT:
#endif
#if defined(SPI_DATASIZE_10BIT)
        case SPI_DATASIZE_10BIT:
#endif
#if defined(SPI_DATASIZE_11BIT)
        case SPI_DATASIZE_11BIT:
#endif
#if defined(SPI_DATASIZE_12BIT)
        case SPI_DATASIZE_12BIT:
#endif
#if defined(SPI_DATASIZE_13BIT)
        case SPI_DATASIZE_13BIT:
#endif
#if defined(SPI_DATASIZE_14BIT)
        case SPI_DATASIZE_14BIT:
#endif
#if defined(SPI_DATASIZE_15BIT)
        case SPI_DATASIZE_15BIT:
#endif
        case SPI_DATASIZE_16BIT:
            return 1;
#if defined(SPI_DATASIZE_17BIT)
        case SPI_DATASIZE_17BIT:
            return 2;
#endif
#if defined(SPI_DATASIZE_18BIT)
        case SPI_DATASIZE_18BIT:
            return 2;
#endif
#if defined(SPI_DATASIZE_19BIT)
        case SPI_DATASIZE_19BIT:
            return 2;
#endif
#if defined(SPI_DATASIZE_20BIT)
        case SPI_DATASIZE_20BIT:
            return 2;
#endif
#if defined(SPI_DATASIZE_21BIT)
        case SPI_DATASIZE_21BIT:
            return 2;
#endif
#if defined(SPI_DATASIZE_22BIT)
        case SPI_DATASIZE_22BIT:
            return 2;
#endif
#if defined(SPI_DATASIZE_23BIT)
        case SPI_DATASIZE_23BIT:
            return 2;
#endif
#if defined(SPI_DATASIZE_24BIT)
        case SPI_DATASIZE_24BIT:
            return 2;
#endif
#if defined(SPI_DATASIZE_25BIT)
        case SPI_DATASIZE_25BIT:
            return 2;
#endif
#if defined(SPI_DATASIZE_26BIT)
        case SPI_DATASIZE_26BIT:
            return 2;
#endif
#if defined(SPI_DATASIZE_27BIT)
        case SPI_DATASIZE_27BIT:
            return 2;
#endif
#if defined(SPI_DATASIZE_28BIT)
        case SPI_DATASIZE_28BIT:
            return 2;
#endif
#if defined(SPI_DATASIZE_29BIT)
        case SPI_DATASIZE_29BIT:
            return 2;
#endif
#if defined(SPI_DATASIZE_30BIT)
        case SPI_DATASIZE_30BIT:
            return 2;
#endif
#if defined(SPI_DATASIZE_31BIT)
        case SPI_DATASIZE_31BIT:
            return 2;
#endif
#if defined(SPI_DATASIZE_32BIT)
        case SPI_DATASIZE_32BIT:
            return 2;
#endif
        // This point should never be reached, so return a negative value for assertion checking
        default:
            return -1;
    }
}

static inline int spi_get_word_from_buffer(const void *buffer, int bitshift)
{
    if (bitshift == 1) {
        return *((uint16_t *)buffer);
#ifdef HAS_32BIT_SPI_TRANSFERS
    } else if (bitshift == 2) {
        return *((uint32_t *)buffer);
#endif /* HAS_32BIT_SPI_TRANSFERS */
    } else {
        return *((uint8_t *)buffer);
    }
}

static inline void spi_put_word_to_buffer(void *buffer, int bitshift, int data)
{
    if (bitshift == 1) {
        *((uint16_t *)buffer) = data;
#ifdef HAS_32BIT_SPI_TRANSFERS
    } else if (bitshift == 2) {
        *((uint32_t *)buffer) = data;
#endif /* HAS_32BIT_SPI_TRANSFERS */
    } else {
        *((uint8_t *)buffer) = data;
    }
}


/**
 * Check if SPI master interface is writable.
 *
 * @param obj
 * @return  0 - SPI isn't writable, non-zero - SPI is writable
 */
static inline int msp_writable(spi_t *obj)
{
#if defined(SPI_IP_VERSION_V2)
    return (int)LL_SPI_IsActiveFlag_TXP(SPI_INST(obj));
#else /* SPI_IP_VERSION_V2 */
    return (int)LL_SPI_IsActiveFlag_TXE(SPI_INST(obj));
#endif /* SPI_IP_VERSION_V2 */
}

/**
 * Check if SPI master interface is readable.
 *
 * @param obj
 * @return 0 - SPI isn't readable, non-zero - SPI is readable
 */
static inline int msp_readable(spi_t *obj)
{
#if defined(SPI_IP_VERSION_V2)
    return (int)LL_SPI_IsActiveFlag_RXP(SPI_INST(obj));
#else /* SPI_IP_VERSION_V2 */
    return (int)LL_SPI_IsActiveFlag_RXNE(SPI_INST(obj));
#endif /* SPI_IP_VERSION_V2 */
}

/**
 * Wait till SPI master interface is writable.
 */
static inline void msp_wait_writable(spi_t *obj)
{
    while (!msp_writable(obj));
}

/**
 * Wait till SPI master interface is readable.
 */
static inline void msp_wait_readable(spi_t *obj)
{
    while (!msp_readable(obj));
}

/**
 * Check if SPI master interface is busy.
 *
 * @param obj
 * @return 0 - SPI isn't busy, non-zero - SPI is busy
 */
static inline int msp_busy(spi_t *obj)
{
#if defined(SPI_IP_VERSION_V2)
    return !(int)LL_SPI_IsActiveFlag_TXC(SPI_INST(obj));
#else /* SPI_IP_VERSION_V2 */
    return (int)LL_SPI_IsActiveFlag_BSY(SPI_INST(obj));
#endif /* SPI_IP_VERSION_V2 */
}

/**
 * Wait till SPI master interface isn't busy.
 */
static inline void msp_wait_not_busy(spi_t *obj)
{
    while (msp_busy(obj));
}

/**
 * Write data to SPI master interface.
 */
static inline void msp_write_data(spi_t *obj, int value, int bitshift)
{
    if (bitshift == 1) {
        LL_SPI_TransmitData16(SPI_INST(obj), (uint16_t)value);
#ifdef HAS_32BIT_SPI_TRANSFERS
    } else if (bitshift == 2) {
        LL_SPI_TransmitData32(SPI_INST(obj), (uint32_t)value);
#endif /* HAS_32BIT_SPI_TRANSFERS */
    } else {
        LL_SPI_TransmitData8(SPI_INST(obj), (uint8_t)value);
    }
}

/**
 * Read data from SPI master interface.
 */
static inline int msp_read_data(spi_t *obj, int bitshift)
{
    if (bitshift == 1) {
        return LL_SPI_ReceiveData16(SPI_INST(obj));
#ifdef HAS_32BIT_SPI_TRANSFERS
    } else if (bitshift == 2) {
        return LL_SPI_ReceiveData32(SPI_INST(obj));
#endif /* HAS_32BIT_SPI_TRANSFERS */
    } else {
        return LL_SPI_ReceiveData8(SPI_INST(obj));
    }
}

/**
 * Transmit and receive SPI data in bidirectional mode.
 *
 * @param obj spi object
 * @param tx_buffer byte-array of data to write to the device
 * @param tx_length number of bytes to write, may be zero
 * @param rx_buffer byte-array of data to read from the device
 * @param rx_length number of bytes to read, may be zero
 * @return number of transmitted and received bytes or negative code in case of error.
 */
static int spi_master_one_wire_transfer(spi_t *obj, const char *tx_buffer, int tx_length,
                                        char *rx_buffer, int rx_length)
{
    struct spi_s *spiobj = SPI_S(obj);
    SPI_HandleTypeDef *handle = &(spiobj->handle);
    const int bitshift = datasize_to_transfer_bitshift(handle->Init.DataSize);
    MBED_ASSERT(bitshift >= 0);
    const int word_size = 0x01 << bitshift;

    /* Ensure that spi is disabled */
    LL_SPI_Disable(SPI_INST(obj));

    /* Transmit data */
    if (tx_length) {
        LL_SPI_SetTransferDirection(SPI_INST(obj), LL_SPI_HALF_DUPLEX_TX);
#if defined(SPI_IP_VERSION_V2)
        /* Set transaction size */
        LL_SPI_SetTransferSize(SPI_INST(obj), tx_length >> bitshift);
#endif /* SPI_IP_VERSION_V2 */
        LL_SPI_Enable(SPI_INST(obj));
#if defined(SPI_IP_VERSION_V2)
        /* Master transfer start */
        LL_SPI_StartMasterTransfer(SPI_INST(obj));
#endif /* SPI_IP_VERSION_V2 */

        for (int i = 0; i < tx_length; i += word_size) {
            msp_wait_writable(obj);
            msp_write_data(obj, spi_get_word_from_buffer(tx_buffer + i, bitshift), bitshift);
        }

        /* Wait end of transaction */
        msp_wait_not_busy(obj);

        LL_SPI_Disable(SPI_INST(obj));

#if defined(SPI_IP_VERSION_V2)
        /* Clear transaction flags */
        LL_SPI_ClearFlag_EOT(SPI_INST(obj));
        LL_SPI_ClearFlag_TXTF(SPI_INST(obj));
        /* Reset transaction size */
        LL_SPI_SetTransferSize(SPI_INST(obj), 0);
#endif /* SPI_IP_VERSION_V2 */
    }

    /* Receive data */
    if (rx_length) {
        LL_SPI_SetTransferDirection(SPI_INST(obj), LL_SPI_HALF_DUPLEX_RX);
#if defined(SPI_IP_VERSION_V2)
        /* Set transaction size and run SPI */
        LL_SPI_SetTransferSize(SPI_INST(obj), rx_length >> bitshift);
        LL_SPI_Enable(SPI_INST(obj));
        LL_SPI_StartMasterTransfer(SPI_INST(obj));

        /* Receive data */
        for (int i = 0; i < rx_length; i += word_size) {
            msp_wait_readable(obj);
            spi_put_word_to_buffer(rx_buffer + i, bitshift, msp_read_data(obj, bitshift));
        }

        /* Stop SPI */
        LL_SPI_Disable(SPI_INST(obj));
        /* Clear transaction flags */
        LL_SPI_ClearFlag_EOT(SPI_INST(obj));
        LL_SPI_ClearFlag_TXTF(SPI_INST(obj));
        /* Reset transaction size */
        LL_SPI_SetTransferSize(SPI_INST(obj), 0);

#else /* SPI_IP_VERSION_V2 */
        /* Unlike STM32H7 other STM32 families generates SPI Clock signal continuously in half-duplex receive mode
         * till SPI is enabled. To stop clock generation a SPI should be disabled during last frame receiving,
         * after generation at least one SPI clock cycle. It causes necessity of critical section usage.
         * So the following consequences of steps is used to receive each byte:
         * 1. Enter into critical section.
         * 2. Enable SPI.
         * 3. Wait one SPI clock cycle.
         * 4. Disable SPI.
         * 5. Wait full byte receiving.
         * 6. Read byte.
         * It gives some overhead, but gives stable byte reception without dummy reads and
         * short delay of critical section holding.
         */

        /* get estimation about one SPI clock cycle */
        uint32_t baudrate_period_ns = 1000000000 / spi_get_baudrate(obj);

        for (int i = 0; i < rx_length; i += word_size) {
            core_util_critical_section_enter();
            LL_SPI_Enable(SPI_INST(obj));
            /* Wait single SPI clock cycle. */
            wait_ns(baudrate_period_ns);
            LL_SPI_Disable(SPI_INST(obj));
            core_util_critical_section_exit();

            msp_wait_readable(obj);
            spi_put_word_to_buffer(rx_buffer + i, bitshift, msp_read_data(obj, bitshift));
        }

#endif /* SPI_IP_VERSION_V2 */
    }

    return rx_length + tx_length;
}

int spi_master_write(spi_t *obj, int value)
{
    struct spi_s *spiobj = SPI_S(obj);
    SPI_HandleTypeDef *handle = &(spiobj->handle);

    if (handle->Init.Direction == SPI_DIRECTION_1LINE) {
        int result = spi_master_one_wire_transfer(obj, (const char *)&value, 1, NULL, 0);
        return result == 1 ? HAL_OK : HAL_ERROR;
    }
    const int bitshift = datasize_to_transfer_bitshift(handle->Init.DataSize);
    MBED_ASSERT(bitshift >= 0);

#if defined(LL_SPI_RX_FIFO_TH_HALF)
    /*  Configure the default data size */
    if (bitshift == 0) {
        LL_SPI_SetRxFIFOThreshold(SPI_INST(obj), LL_SPI_RX_FIFO_TH_QUARTER);
    } else {
        LL_SPI_SetRxFIFOThreshold(SPI_INST(obj), LL_SPI_RX_FIFO_TH_HALF);
    }
#endif

    /*  Here we're using LL which means direct registers access
     *  There is no error management, so we may end up looping
     *  infinitely here in case of faulty device for instance,
     *  but this will increase performances significantly
     */

#if defined(SPI_IP_VERSION_V2)
    /* Master transfer start */
    LL_SPI_StartMasterTransfer(SPI_INST(obj));
#endif /* SPI_IP_VERSION_V2 */

    /* Transmit data */
    msp_wait_writable(obj);
    msp_write_data(obj, value, bitshift);

    /* Receive data */
    msp_wait_readable(obj);
    return msp_read_data(obj, bitshift);
}

int spi_master_block_write(spi_t *obj, const char *tx_buffer, int tx_length,
                           char *rx_buffer, int rx_length, char write_fill)
{
    struct spi_s *spiobj = SPI_S(obj);
    SPI_HandleTypeDef *handle = &(spiobj->handle);
    const int bitshift = datasize_to_transfer_bitshift(handle->Init.DataSize);
    /* check buffer sizes are multiple of spi word size */
    MBED_ASSERT(tx_length >> bitshift << bitshift == tx_length);
    MBED_ASSERT(rx_length >> bitshift << bitshift == rx_length);
    int total = (tx_length > rx_length) ? tx_length : rx_length;

    if (handle->Init.Direction == SPI_DIRECTION_2LINES) {
        int write_fill_frame = write_fill;
        /* extend fill symbols for 16/32 bit modes */
        for (int i = 0; i < bitshift; i++) {
            write_fill_frame = (write_fill_frame << 8) | write_fill;
        }

        const int word_size = 0x01 << bitshift;
        for (int i = 0; i < total; i += word_size) {
            int out = (i < tx_length) ? spi_get_word_from_buffer(tx_buffer + i, bitshift) : write_fill_frame;
            int in = spi_master_write(obj, out);
            if (i < rx_length) {
                spi_put_word_to_buffer(rx_buffer + i, bitshift, in);
            }
        }
    } else {
        /* 1 wire case */
        int result = spi_master_one_wire_transfer(obj, tx_buffer, tx_length, rx_buffer, rx_length);
        if (result != tx_length + rx_length) {
            /*  report an error */
            total = 0;
        }
    }

    return total;
}

int spi_slave_receive(spi_t *obj)
{
    return ((ssp_readable(obj) && !ssp_busy(obj)) ? 1 : 0);
};

int spi_slave_read(spi_t *obj)
{
    struct spi_s *spiobj = SPI_S(obj);
    SPI_HandleTypeDef *handle = &(spiobj->handle);
    while (!ssp_readable(obj));
    const int bitshift = datasize_to_transfer_bitshift(handle->Init.DataSize);
    MBED_ASSERT(bitshift >= 0);

    if (bitshift == 1) {
        return LL_SPI_ReceiveData16(SPI_INST(obj));
#ifdef HAS_32BIT_SPI_TRANSFERS
    } else if (bitshift == 2) {
        return LL_SPI_ReceiveData32(SPI_INST(obj));
#endif
    } else {
        return LL_SPI_ReceiveData8(SPI_INST(obj));
    }
}

void spi_slave_write(spi_t *obj, int value)
{
    SPI_TypeDef *spi = SPI_INST(obj);
    struct spi_s *spiobj = SPI_S(obj);
    SPI_HandleTypeDef *handle = &(spiobj->handle);
    while (!ssp_writeable(obj));
    const int bitshift = datasize_to_transfer_bitshift(handle->Init.DataSize);
    MBED_ASSERT(bitshift >= 0);

    if (bitshift == 1) {
        LL_SPI_TransmitData16(spi, (uint16_t)value);
#ifdef HAS_32BIT_SPI_TRANSFERS
    } else if (bitshift == 2) {
        LL_SPI_TransmitData32(spi, (uint32_t)value);
#endif
    } else {
        LL_SPI_TransmitData8(spi, (uint8_t)value);
    }
}

int spi_busy(spi_t *obj)
{
    return ssp_busy(obj);
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

#if DEVICE_SPI_ASYNCH
typedef enum {
    SPI_TRANSFER_TYPE_NONE = 0,
    SPI_TRANSFER_TYPE_TX = 1,
    SPI_TRANSFER_TYPE_RX = 2,
    SPI_TRANSFER_TYPE_TXRX = 3,
} transfer_type_t;


/// @returns the number of bytes transferred, or `0` if nothing transferred
static int spi_master_start_asynch_transfer(spi_t *obj, transfer_type_t transfer_type, const void *tx, void *rx, size_t length)
{
    struct spi_s *spiobj = SPI_S(obj);
    SPI_HandleTypeDef *handle = &(spiobj->handle);
    // bool is16bit = (handle->Init.DataSize == SPI_DATASIZE_16BIT);
    const int bitshift = datasize_to_transfer_bitshift(handle->Init.DataSize);
    MBED_ASSERT(bitshift >= 0);
    // the HAL expects number of transfers instead of number of bytes
    // so the number of transfers depends on the container size
    size_t words;

    DEBUG_PRINTF("SPI inst=0x%8X Start: %u, %u\r\n", (int)handle->Instance, transfer_type, length);

    obj->spi.transfer_type = transfer_type;

    words = length >> bitshift;

    // enable the interrupt
    IRQn_Type irq_n = spiobj->spiIRQ;
    NVIC_DisableIRQ(irq_n);
    NVIC_ClearPendingIRQ(irq_n);
    NVIC_SetPriority(irq_n, 1);
    NVIC_EnableIRQ(irq_n);

    // flush FIFO
#if defined(SPI_FLAG_FRLVL)
    HAL_SPIEx_FlushRxFifo(handle);
#endif

    // enable the right hal transfer
    int rc = 0;
#if defined(SPI_IP_VERSION_V2)
    // HAL SPI API assumes that SPI disabled between transfers and
    // doesn't work properly if SPI is enabled.
    LL_SPI_Disable(SPI_INST(obj));
#endif
    switch (transfer_type) {
        case SPI_TRANSFER_TYPE_TXRX:
            rc = HAL_SPI_TransmitReceive_IT(handle, (uint8_t *)tx, (uint8_t *)rx, words);
            break;
        case SPI_TRANSFER_TYPE_TX:
            rc = HAL_SPI_Transmit_IT(handle, (uint8_t *)tx, words);
            break;
        case SPI_TRANSFER_TYPE_RX:
            // the receive function also "transmits" the receive buffer so in order
            // to guarantee that 0xff is on the line, we explicitly memset it here
            memset(rx, SPI_FILL_CHAR, length);
            rc = HAL_SPI_Receive_IT(handle, (uint8_t *)rx, words);
            break;
        default:
            length = 0;
    }

    if (rc) {
#if defined(SPI_IP_VERSION_V2)
        // enable SPI back in case of error
        if (handle->Init.Direction != SPI_DIRECTION_1LINE) {
            LL_SPI_Enable(SPI_INST(obj));
        }
#endif
        DEBUG_PRINTF("SPI: RC=%u\n", rc);
        length = 0;
    }

    return length;
}

// asynchronous API
// DMA support for SPI is currently not supported, hence asynchronous SPI does not support high speeds(MHZ range)
void spi_master_transfer(spi_t *obj, const void *tx, size_t tx_length, void *rx, size_t rx_length, uint8_t bit_width, uint32_t handler, uint32_t event, DMAUsage hint)
{
    struct spi_s *spiobj = SPI_S(obj);
    SPI_HandleTypeDef *handle = &(spiobj->handle);

    // TODO: DMA usage is currently ignored
    (void) hint;

    // check which use-case we have
    bool use_tx = (tx != NULL && tx_length > 0);
    bool use_rx = (rx != NULL && rx_length > 0);
    const int bitshift = datasize_to_transfer_bitshift(handle->Init.DataSize);
    MBED_ASSERT(bitshift >= 0);

    // don't do anything, if the buffers aren't valid
    if (!use_tx && !use_rx) {
        return;
    }

    // copy the buffers to the SPI object
    obj->tx_buff.buffer = (void *) tx;
    obj->tx_buff.length = tx_length;
    obj->tx_buff.pos = 0;
    obj->tx_buff.width = 8 << bitshift;

    obj->rx_buff.buffer = rx;
    obj->rx_buff.length = rx_length;
    obj->rx_buff.pos = 0;
    obj->rx_buff.width = obj->tx_buff.width;

    obj->spi.event = event;

    // register the thunking handler
    IRQn_Type irq_n = spiobj->spiIRQ;
    NVIC_SetVector(irq_n, (uint32_t)handler);
    DEBUG_PRINTF("SPI: Transfer: tx %u (%u), rx %u (%u), IRQ %u\n", use_tx, tx_length, use_rx, rx_length, irq_n);

    // enable the right hal transfer
    if (use_tx && use_rx) {
        // we cannot manage different rx / tx sizes, let's use smaller one
        size_t size = (tx_length < rx_length) ? tx_length : rx_length;
        if (tx_length != rx_length) {
            DEBUG_PRINTF("SPI: Full duplex transfer only 1 size: %d\n", size);
            obj->tx_buff.length = size;
            obj->rx_buff.length = size;
        }
        spi_master_start_asynch_transfer(obj, SPI_TRANSFER_TYPE_TXRX, tx, rx, size);
    } else if (use_tx) {
        spi_master_start_asynch_transfer(obj, SPI_TRANSFER_TYPE_TX, tx, NULL, tx_length);
    } else if (use_rx) {
        spi_master_start_asynch_transfer(obj, SPI_TRANSFER_TYPE_RX, NULL, rx, rx_length);
    }
}

inline uint32_t spi_irq_handler_asynch(spi_t *obj)
{
    int event = 0;
    SPI_HandleTypeDef *handle = &(SPI_S(obj)->handle);

    // call the CubeF4 handler, this will update the handle
    HAL_SPI_IRQHandler(handle);

    if (handle->State == HAL_SPI_STATE_READY) {
        // When HAL SPI is back to READY state, check if there was an error
        int error = obj->spi.handle.ErrorCode;
        if (error != HAL_SPI_ERROR_NONE) {
            // something went wrong and the transfer has definitely completed
            event = SPI_EVENT_ERROR | SPI_EVENT_INTERNAL_TRANSFER_COMPLETE;

            if (error & HAL_SPI_ERROR_OVR) {
                // buffer overrun
                event |= SPI_EVENT_RX_OVERFLOW;
            }
        } else {
            // else we're done
            event = SPI_EVENT_COMPLETE | SPI_EVENT_INTERNAL_TRANSFER_COMPLETE;
        }
        // disable the interrupt
        NVIC_DisableIRQ(obj->spi.spiIRQ);
        NVIC_ClearPendingIRQ(obj->spi.spiIRQ);
#if !defined(SPI_IP_VERSION_V2)
        if (handle->Init.Direction == SPI_DIRECTION_1LINE && obj->rx_buff.buffer != NULL) {
            /**
             * In case of 3-wire SPI data receiving we usually get dummy reads.
             * So we need to cleanup FIFO/input register before next transmission.
             * Probably it's better to set SPI_EVENT_RX_OVERFLOW event flag,
             * but let's left it as is for backward compatibility.
             */
            spi_flush_rx(obj);
        }
#else
        // reset transfer size
        LL_SPI_SetTransferSize(SPI_INST(obj), 0);

        // HAL_SPI_TransmitReceive_IT/HAL_SPI_Transmit_IT/HAL_SPI_Receive_IT
        // function disable SPI after transfer. So we need enabled it back,
        // otherwise spi_master_block_write/spi_master_write won't work in 4-wire mode.
        if (handle->Init.Direction != SPI_DIRECTION_1LINE) {
            LL_SPI_Enable(SPI_INST(obj));
        }
#endif /* SPI_IP_VERSION_V2 */
    }

    return (event & (obj->spi.event | SPI_EVENT_INTERNAL_TRANSFER_COMPLETE));
}

uint8_t spi_active(spi_t *obj)
{
    struct spi_s *spiobj = SPI_S(obj);
    SPI_HandleTypeDef *handle = &(spiobj->handle);
    HAL_SPI_StateTypeDef state = HAL_SPI_GetState(handle);

    switch (state) {
        case HAL_SPI_STATE_RESET:
        case HAL_SPI_STATE_READY:
        case HAL_SPI_STATE_ERROR:
            return 0;
        default:
            return 1;
    }
}

void spi_abort_asynch(spi_t *obj)
{
    struct spi_s *spiobj = SPI_S(obj);
    SPI_HandleTypeDef *handle = &(spiobj->handle);

    // disable interrupt
    IRQn_Type irq_n = spiobj->spiIRQ;
    NVIC_ClearPendingIRQ(irq_n);
    NVIC_DisableIRQ(irq_n);

    // clean-up
    LL_SPI_Disable(SPI_INST(obj));
    HAL_SPI_DeInit(handle);
    HAL_SPI_Init(handle);
    // cleanup input buffer
    spi_flush_rx(obj);
    // enable SPI back if it isn't 3-wire mode
    if (handle->Init.Direction != SPI_DIRECTION_1LINE) {
        LL_SPI_Enable(SPI_INST(obj));
    }
}

#endif //DEVICE_SPI_ASYNCH

#endif
