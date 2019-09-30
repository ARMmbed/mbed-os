/*
 * mbed Microcontroller Library
 * Copyright (c) 2017-2018 Future Electronics
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

#include "cmsis.h"
#include "mbed_assert.h"
#include "mbed_error.h"
#include "mbed_debug.h"
#include "PeripheralPins.h"
#include "pinmap.h"
#include "spi_api.h"
#include "psoc6_utils.h"

#include "cy_sysclk.h"
#include "cy_gpio.h"
#include "cy_scb_spi.h"
#include "cy_sysint.h"

#define SPI_DEFAULT_SPEED               100000
#define NUM_SPI_PORTS                   8
#define SPI_DEFAULT_IRQ_PRIORITY        3
#define SPI_OVERSAMPLE                  4       /* 4..16 */
// Default timeout in milliseconds.
#define SPI_DEFAULT_TIMEOUT             1000

#define PENDING_NONE                    0
#define PENDING_RX                      1
#define PENDING_TX                      2
#define PENDING_TX_RX                   3



static const cy_stc_scb_spi_config_t default_spi_config = {
    .spiMode                = CY_SCB_SPI_MASTER,
    .subMode                = CY_SCB_SPI_MOTOROLA,
    .sclkMode               = CY_SCB_SPI_CPHA0_CPOL0,
    .oversample             = SPI_OVERSAMPLE,
    .rxDataWidth            = 8,
    .txDataWidth            = 8,
    .enableMsbFirst         = true,
    .enableFreeRunSclk      = false,
    .enableInputFilter      = false,
    .enableMisoLateSample   = false,
    .enableTransferSeperation = false,
    .enableWakeFromSleep    = false,
    .ssPolarity             = CY_SCB_SPI_ACTIVE_LOW,
    .rxFifoTriggerLevel     = 0,
    .rxFifoIntEnableMask    = 0,
    .txFifoTriggerLevel     = 0,
    .txFifoIntEnableMask    = 0,
    .masterSlaveIntEnableMask = 0
};



typedef struct spi_s spi_obj_t;

#if DEVICE_SPI_ASYNCH
#define OBJ_P(in)     (&(in->spi))
#else
#define OBJ_P(in)     (in)
#endif


#if DEVICE_SPI_ASYNCH

static IRQn_Type spi_irq_allocate_channel(spi_obj_t *obj)
{
#if defined (TARGET_MCU_PSOC6_M0)
    obj->cm0p_irq_src = scb_0_interrupt_IRQn + obj->spi_id;
    return cy_m0_nvic_allocate_channel(CY_SERIAL_IRQN_ID + obj->spi_id);
#else
    return (IRQn_Type)(ioss_interrupts_gpio_0_IRQn + obj->spi_id);
#endif // M0
}

static void spi_irq_release_channel(spi_obj_t *obj)
{
#if defined (TARGET_MCU_PSOC6_M0)
    cy_m0_nvic_release_channel(obj->irqn, CY_SERIAL_IRQN_ID + obj->spi_id);
#endif //M0
}

static int spi_irq_setup_channel(spi_obj_t *obj)
{
    cy_stc_sysint_t irq_config;

    if (obj->irqn == unconnected_IRQn) {
        IRQn_Type irqn = spi_irq_allocate_channel(obj);
        if (irqn < 0) {
            return (-1);
        }
        // Configure NVIC
        irq_config.intrPriority = SPI_DEFAULT_IRQ_PRIORITY;
        irq_config.intrSrc = irqn;
#if defined (TARGET_MCU_PSOC6_M0)
        irq_config.cm0pSrc = obj->cm0p_irq_src;
#endif
        if (Cy_SysInt_Init(&irq_config, (cy_israddress)(obj->handler)) != CY_SYSINT_SUCCESS) {
            return(-1);
        }

        obj->irqn = irqn;
        NVIC_EnableIRQ(irqn);
    }
    return 0;
}

#endif // DEVICE_SPI_ASYNCH

static int allocate_divider(spi_obj_t *obj)
{
    if (obj->div_num == CY_INVALID_DIVIDER) {
        obj->div_type = CY_SYSCLK_DIV_16_BIT;
        obj->div_num = cy_clk_allocate_divider(CY_SYSCLK_DIV_16_BIT);
    }
    return (obj->div_num == CY_INVALID_DIVIDER)? -1 : 0;
}

static void free_divider(spi_obj_t *obj)
{
    if (obj->div_num != CY_INVALID_DIVIDER) {
        cy_clk_free_divider(obj->div_type, obj->div_num);
        obj->div_num = CY_INVALID_DIVIDER;
    }
}


/*
 * Initializes spi clock for the required speed
 */
static cy_en_sysclk_status_t spi_init_clock(spi_obj_t *obj, uint32_t frequency)
{
    cy_en_sysclk_status_t status = CY_SYSCLK_INVALID_STATE;
    uint32_t div_value;

    if (obj->div_num == CY_INVALID_DIVIDER) {
        if (allocate_divider(obj) < 0) {
            error("spi: cannot allocate clock divider.");
            return CY_SYSCLK_INVALID_STATE;
        }
    }

    // Set up proper frequency; round up the divider so the frequency is not higher than specified.
    div_value = (CY_CLK_PERICLK_FREQ_HZ + frequency *(SPI_OVERSAMPLE - 1)) / frequency / SPI_OVERSAMPLE;
    obj->clk_frequency = CY_CLK_PERICLK_FREQ_HZ / div_value / SPI_OVERSAMPLE;
    // Delay (in us) required for serialized read operation == 1.5 clocks, min 1us.
    obj->clk_delay = (1500000UL - 1 + obj->clk_frequency) / obj->clk_frequency;
    Cy_SysClk_PeriphDisableDivider(obj->div_type, obj->div_num);
    if (Cy_SysClk_PeriphSetDivider(obj->div_type, obj->div_num, div_value - 1) != CY_SYSCLK_SUCCESS) {
        obj->div_num = CY_INVALID_DIVIDER;
    }
    Cy_SysClk_PeriphEnableDivider(obj->div_type, obj->div_num);

    if (obj->div_num != CY_INVALID_DIVIDER) {
        status = Cy_SysClk_PeriphAssignDivider(obj->clock, obj->div_type, obj->div_num);
        if (status != CY_SYSCLK_SUCCESS) {
            error("spi: cannot assign clock divider.");
            return status;
        }
    }
    return CY_SYSCLK_SUCCESS;
}

/*
 * Sets up i/o connections for spi.
 */
static void spi_set_pins(spi_obj_t *obj)
{
    if (obj->pin_mosi != NC) {
        pin_function(obj->pin_mosi, pinmap_function(obj->pin_mosi, PinMap_SPI_MOSI));
    }
    if (obj->pin_miso != NC) {
        pin_function(obj->pin_miso, pinmap_function(obj->pin_miso, PinMap_SPI_MISO));
    }
    if (obj->pin_ssel != NC) {
        pin_function(obj->pin_ssel, pinmap_function(obj->pin_ssel, PinMap_SPI_SSEL));
    }
    if (obj->pin_sclk != NC) {
        pin_function(obj->pin_sclk, pinmap_function(obj->pin_sclk, PinMap_SPI_SCLK));
    }
    // Pin configuration in PinMap defaults to Master mode; revert for Slave.
    if (obj->ms_mode == CY_SCB_SPI_SLAVE) {
        if (obj->pin_sclk != NC) {
            pin_mode(obj->pin_sclk, PullNone);
        }
        if (obj->pin_mosi != NC) {
            pin_mode(obj->pin_mosi, PullNone);
        }
        if (obj->pin_miso != NC) {
            pin_mode(obj->pin_miso, PushPull);
        }
        if (obj->pin_ssel != NC) {
            pin_mode(obj->pin_ssel, PullNone);
        }
    }
}

/*
 * Sets i/o output pins into safe mode while re-configuring peripheral.
 */
static void spi_default_pins(spi_obj_t *obj)
{

    if (obj->ms_mode == CY_SCB_SPI_MASTER) {
        if (obj->pin_sclk != NC) {
            pin_function(obj->pin_sclk, CY_PIN_FUNCTION(HSIOM_SEL_GPIO, 0, PullDown, PIN_OUTPUT));
        }
        if (obj->pin_mosi != NC) {
            pin_function(obj->pin_mosi, CY_PIN_FUNCTION(HSIOM_SEL_GPIO, 0, PullUp, PIN_OUTPUT));
        }
        if (obj->pin_ssel != NC) {
            pin_function(obj->pin_ssel, CY_PIN_FUNCTION(HSIOM_SEL_GPIO, 0, PullUp, PIN_OUTPUT));
        }
    } else {
        if (obj->pin_miso != NC) {
            pin_function(obj->pin_miso, CY_PIN_FUNCTION(HSIOM_SEL_GPIO, 0, PullUp, PIN_OUTPUT));
        }
    }
}

/*
 * Initializes i/o pins for spi.
 */
static void spi_init_pins(spi_obj_t *obj)
{
    bool conflict = false;
    if (obj->pin_sclk != NC) {
        if (cy_reserve_io_pin(obj->pin_sclk)) {
            conflict = true;
        }
    }
    if (obj->pin_mosi != NC) {
        if (cy_reserve_io_pin(obj->pin_mosi)) {
            conflict = true;
        }
    }
    if (obj->pin_miso != NC) {
        if (cy_reserve_io_pin(obj->pin_miso)) {
            conflict = true;
        }
    }
    if (obj->pin_ssel != NC) {
        if (cy_reserve_io_pin(obj->pin_ssel)) {
            conflict = true;
        }
    }
    if (conflict) {
        error("SPI pin reservation conflict.");
    }
    spi_set_pins(obj);
}

/*
 * Initializes and enables SPI/SCB.
 */
static void spi_init_peripheral(spi_obj_t *obj)
{
    cy_stc_scb_spi_config_t spi_config = default_spi_config;
    spi_config.spiMode = obj->ms_mode;
    spi_config.sclkMode = obj->clk_mode;
    spi_config.rxDataWidth = obj->data_bits;
    spi_config.txDataWidth = obj->data_bits;
    Cy_SCB_SPI_Init(obj->base, &spi_config, &obj->context);
    Cy_SCB_SPI_Enable(obj->base);
}


/* Callback function to handle into and out of deep sleep state transitions.
 *
 */
#if DEVICE_SLEEP && DEVICE_LOWPOWERTIMER
static cy_en_syspm_status_t spi_pm_callback(cy_stc_syspm_callback_params_t *callback_params)
{
    cy_stc_syspm_callback_params_t params = *callback_params;
    spi_obj_t *obj = (spi_obj_t *)params.context;
    params.context = &obj->context;

    return Cy_SCB_SPI_DeepSleepCallback(&params);
}
#endif // DEVICE_SLEEP && DEVICE_LOWPOWERTIMER


void spi_init(spi_t *obj_in, PinName mosi, PinName miso, PinName sclk, PinName ssel)
{
    spi_obj_t *obj = OBJ_P(obj_in);
    uint32_t spi = (uint32_t)NC;
    en_clk_dst_t clock;

    if (mosi != NC) {
        spi = pinmap_merge(spi, pinmap_peripheral(mosi, PinMap_SPI_MOSI));
        clock = CY_PIN_CLOCK(pinmap_function(mosi, PinMap_SPI_MOSI));
    }
    if (miso != NC) {
        spi = pinmap_merge(spi, pinmap_peripheral(miso, PinMap_SPI_MISO));
        clock = CY_PIN_CLOCK(pinmap_function(miso, PinMap_SPI_MISO));
    }
    if (sclk != NC) {
        spi = pinmap_merge(spi, pinmap_peripheral(sclk, PinMap_SPI_SCLK));
        clock = CY_PIN_CLOCK(pinmap_function(sclk, PinMap_SPI_SCLK));
    }
    if (ssel != NC) {
        spi = pinmap_merge(spi, pinmap_peripheral(ssel, PinMap_SPI_SSEL));
        clock = CY_PIN_CLOCK(pinmap_function(ssel, PinMap_SPI_SSEL));
    }

    if (spi != (uint32_t)NC) {
        obj->base = (CySCB_Type*)spi;
        obj->spi_id = ((SPIName)spi - SPI_0) / (SPI_1 - SPI_0);
        obj->pin_mosi = mosi;
        obj->pin_miso = miso;
        obj->pin_sclk = sclk;
        obj->pin_ssel = ssel;
        obj->data_bits = 8;
        obj->clock = clock;
        obj->div_num = CY_INVALID_DIVIDER;
        obj->ms_mode = CY_SCB_SPI_MASTER;
#if DEVICE_SPI_ASYNCH
        obj->pending = PENDING_NONE;
        obj->events = 0;
        obj->tx_buffer = NULL;
        obj->tx_buffer_size = 0;
        obj->rx_buffer = NULL;
        obj->rx_buffer_size = 0;
#endif // DEVICE_SPI_ASYNCH
        spi_init_clock(obj, SPI_DEFAULT_SPEED);
        spi_init_peripheral(obj);
        spi_init_pins(obj);
#if DEVICE_SLEEP && DEVICE_LOWPOWERTIMER
        obj->pm_callback_handler.callback = spi_pm_callback;
        obj->pm_callback_handler.type = CY_SYSPM_DEEPSLEEP;
        obj->pm_callback_handler.skipMode = 0;
        obj->pm_callback_handler.callbackParams = &obj->pm_callback_params;
        obj->pm_callback_params.base = obj->base;
        obj->pm_callback_params.context = obj;
        if (!Cy_SysPm_RegisterCallback(&obj->pm_callback_handler)) {
            error("PM callback registration failed!");
        }
#endif // DEVICE_SLEEP && DEVICE_LOWPOWERTIMER
    } else {
        error("SPI pinout mismatch. Requested Rx and Tx pins can't be used for the same SPI communication.");
    }
}


void spi_free(spi_t *obj_in)
{
    spi_obj_t *obj = OBJ_P(obj_in);

    spi_default_pins(obj);
    Cy_SCB_SPI_Disable(obj->base, &obj->context);
    Cy_SCB_SPI_DeInit (obj->base);

#if DEVICE_SLEEP && DEVICE_LOWPOWERTIMER
    Cy_SysPm_UnregisterCallback(&obj->pm_callback_handler);
#endif // DEVICE_SLEEP && DEVICE_LOWPOWERTIMER

#if DEVICE_SPI_ASYNCH
    if (obj->irqn != unconnected_IRQn) {
        NVIC_DisableIRQ(obj->irqn);
        spi_irq_release_channel(obj);

        obj->irqn = unconnected_IRQn;
    }
#endif // DEVICE_SPI_ASYNCH

    if (obj->pin_sclk != NC) {
        cy_free_io_pin(obj->pin_sclk);
        obj->pin_sclk = NC;
    }
    if (obj->pin_mosi != NC) {
        cy_free_io_pin(obj->pin_mosi);
        obj->pin_mosi = NC;
    }
    if (obj->pin_miso != NC) {
        cy_free_io_pin(obj->pin_miso);
        obj->pin_miso = NC;
    }
    if (obj->pin_ssel != NC) {
        cy_free_io_pin(obj->pin_ssel);
        obj->pin_ssel = NC;
    }

    free_divider(obj);
}


void spi_format(spi_t *obj_in, int bits, int mode, int slave)
{
    spi_obj_t *obj = OBJ_P(obj_in);
    cy_en_scb_spi_mode_t new_mode = slave? CY_SCB_SPI_SLAVE : CY_SCB_SPI_MASTER;
    if ((bits < 4) || (bits > 16)) return;
    spi_default_pins(obj);
    Cy_SCB_SPI_Disable(obj->base, &obj->context);
    obj->data_bits = bits;
    obj->clk_mode = (cy_en_scb_spi_sclk_mode_t)(mode & 0x3);
    if (obj->ms_mode != new_mode) {
        obj->ms_mode = new_mode;
    }
    spi_init_peripheral(obj);
    spi_set_pins(obj);
}

void spi_frequency(spi_t *obj_in, int hz)
{
    spi_obj_t *obj = OBJ_P(obj_in);
    spi_default_pins(obj);
    Cy_SCB_SPI_Disable(obj->base, &obj->context);
    spi_init_clock(obj, hz);
    Cy_SCB_SPI_Enable(obj->base);
    spi_set_pins(obj);
}

int  spi_master_write(spi_t *obj_in, int value)
{
    spi_obj_t *obj = OBJ_P(obj_in);

    if (obj->ms_mode == CY_SCB_SPI_MASTER) {
        while (spi_busy(obj_in)) {
            // wait for the device to become ready
        }
        Cy_SCB_SPI_Write(obj->base, value);
        while (!Cy_SCB_SPI_IsTxComplete(obj->base)) {
            // wait for the transmission to complete
        }
        // Wait until RX FIFO is filled from the serial register.
        while (Cy_SCB_SPI_GetNumInRxFifo(obj->base) == 0) {
            /* busy loop */
        }
        return Cy_SCB_SPI_Read(obj->base);
    } else {
        return (int)CY_SCB_SPI_RX_NO_DATA;
    }
}

int spi_master_block_write(spi_t *obj_in, const char *tx_buffer, int tx_length, char *rx_buffer, int rx_length,  char write_fill)
{
    spi_obj_t *obj = OBJ_P(obj_in);
    int trans_length = 0;
    int rx_count = 0;
    int tx_count = 0;
    uint16_t tx_data = (uint8_t)write_fill;
    uint16_t write_fill16 = (tx_data << 8) | tx_data;
    const uint16_t *tx_buffer16 = (const uint16_t*)tx_buffer;
    uint16_t *rx_buffer16 = (uint16_t*)rx_buffer;

    if (obj->ms_mode != CY_SCB_SPI_MASTER) {
        return 0;
    }

    // If single transfer (data bits) is larger than byte, then writing must be performed
    // in words.
    bool word_transfer = !Cy_SCB_IsTxDataWidthByte(obj->base);
    if (word_transfer) {
        tx_length /= 2;
        rx_length /= 2;
        tx_data = write_fill16;
    }
    // Make sure no leftovers from previous transactions.
    Cy_SCB_SPI_ClearRxFifo(obj->base);
    // Calculate transaction length.
    trans_length = (tx_length > rx_length)? tx_length : rx_length;
    // get first byte to transmit.
    if (tx_count < tx_length) {
        if (word_transfer) {
            tx_data = *tx_buffer16++;
        } else {
            tx_data = *tx_buffer++;
        }
    }
    // Send required number of bytes.
    while (tx_count < trans_length) {
        if (Cy_SCB_SPI_Write(obj->base, tx_data)) {
            ++tx_count;
            // Get next byte to transfer.
            if (tx_count < tx_length) {
                if (word_transfer) {
                    tx_data = *tx_buffer16++;
                } else {
                    tx_data = *tx_buffer++;
                }
            } else {
                tx_data = write_fill16;
            }
        }
        // If we have bytes to receive check the rx fifo.
        if (rx_count < rx_length) {
            if (Cy_SCB_SPI_GetNumInRxFifo(obj->base) > 0) {
                if (word_transfer) {
                    *rx_buffer16++ = (uint16_t)Cy_SCB_SPI_Read(obj->base);
                } else {
                    *rx_buffer++ = (char)Cy_SCB_SPI_Read(obj->base);
                }
                ++rx_count;
            }
        }
    }
    // Wait for tx fifo to empty while reading received bytes.
    while (!Cy_SCB_SPI_IsTxComplete(obj->base)) {
        if ((rx_count < rx_length) && (Cy_SCB_SPI_GetNumInRxFifo(obj->base) > 0)) {
            if (word_transfer) {
                *rx_buffer16++ = (uint16_t)Cy_SCB_SPI_Read(obj->base);
            } else {
                *rx_buffer++ = (char)Cy_SCB_SPI_Read(obj->base);
            }
            ++rx_count;
        }
    }
    // Delay at least 1.5 clock cycle, so that FIFO is filled with the last char
    // and SCLK returns to idle state.
    Cy_SysLib_DelayUs(obj->clk_delay);
    // Read any remaining bytes from the fifo.
    while (rx_count < rx_length) {
        if (word_transfer) {
            *rx_buffer16++ = (uint16_t)Cy_SCB_SPI_Read(obj->base);
        } else {
            *rx_buffer++ = (char)Cy_SCB_SPI_Read(obj->base);
        }
        ++rx_count;
    }
    // Clean up if we have read less bytes than available.
    Cy_SCB_SPI_ClearRxFifo(obj->base);
    return word_transfer ? trans_length * 2 : trans_length;
}

int  spi_slave_receive(spi_t *obj_in)
{
    spi_obj_t *obj = OBJ_P(obj_in);
    if (obj->ms_mode == CY_SCB_SPI_SLAVE) {
        return Cy_SCB_SPI_GetNumInRxFifo(obj->base);
    } else {
        return 0;
    }
}

int  spi_slave_read(spi_t *obj_in)
{
    spi_obj_t *obj = OBJ_P(obj_in);

    if (obj->ms_mode == CY_SCB_SPI_SLAVE) {
        while (Cy_SCB_SPI_GetNumInRxFifo(obj->base) == 0) {
            // Wait for data.
        }
        return Cy_SCB_SPI_GetNumInRxFifo(obj->base);
    } else {
        return (int)CY_SCB_SPI_RX_NO_DATA;
    }
}

void spi_slave_write(spi_t *obj_in, int value)
{
    spi_obj_t *obj = OBJ_P(obj_in);

    if (obj->ms_mode == CY_SCB_SPI_SLAVE) {
        while ((Cy_SCB_SPI_GetTxFifoStatus(obj->base) & CY_SCB_SPI_TX_NOT_FULL) == 0) {
            // Wait for a place available in a fifo.
        }
        Cy_SCB_SPI_Write(obj->base, value);
    }
}

int  spi_busy(spi_t *obj)
{
    return !Cy_SCB_SPI_IsTxComplete(OBJ_P(obj)->base);
}

uint8_t spi_get_module(spi_t *obj_in)
{
    return (uint8_t) OBJ_P(obj_in)->spi_id;
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

void spi_master_transfer(spi_t *obj_in,
                         const void *tx,
                         size_t tx_length,
                         void *rx,
                         size_t rx_length,
                         uint8_t bit_width,
                         uint32_t handler,
                         uint32_t event,
                         DMAUsage hint)
{
    spi_obj_t *obj = OBJ_P(obj_in);

    (void)hint; // At the moment we do not support DAM transfers, so this parameter gets ignored.

    if (obj->pending != PENDING_NONE) {
        return;
    }

    // Validate buffer parameters.
    if (((obj->data_bits <= 8) && (bit_width != 8)) || ((obj->data_bits > 8) && (bit_width != 16))) {
        error("spi: buffer configurations does not match device configuration");
        return;
    }

    obj->events = event;
    obj->handler = handler;
    if (spi_irq_setup_channel(obj) < 0) {
        return;
    }

    if (tx_length > rx_length) {
        if (rx_length > 0) {
            // I) write + read, II) write only
            obj->pending = PENDING_TX_RX;
            obj->rx_buffer = NULL;
            obj->tx_buffer = (bit_width == 8)?
                             (void*)(((uint8_t*)tx) + rx_length) :
                             (void*)(((uint16_t*)tx) + rx_length);
            obj->tx_buffer_size = tx_length - rx_length;
            Cy_SCB_SPI_Transfer(obj->base, (void*)tx, rx, rx_length, &obj->context);
        } else {
            // I) write only.
            obj->pending = PENDING_TX;
            obj->rx_buffer = NULL;
            obj->tx_buffer = NULL;
            Cy_SCB_SPI_Transfer(obj->base, (void*)tx, NULL, tx_length, &obj->context);
        }
    } else if (rx_length > tx_length) {
        if (tx_length > 0) {
            // I) write + read, II) read only
            obj->pending = PENDING_TX_RX;
            obj->rx_buffer = (bit_width == 8)?
                             (void*)(((uint8_t*)rx) + tx_length) :
                             (void*)(((uint16_t*)rx) + tx_length);
            obj->rx_buffer_size = rx_length - tx_length;
            obj->tx_buffer = NULL;
            Cy_SCB_SPI_Transfer(obj->base, (void*)tx, rx, tx_length, &obj->context);
        } else {
            // I) read only.
            obj->pending = PENDING_RX;
            obj->rx_buffer = NULL;
            obj->tx_buffer = NULL;
            Cy_SCB_SPI_Transfer(obj->base, NULL, rx, rx_length, &obj->context);
        }
    } else {
        // Rx and Tx of the same size
        // I) write + read.
        obj->pending = PENDING_TX_RX;
        obj->rx_buffer = NULL;
        obj->tx_buffer = NULL;
        Cy_SCB_SPI_Transfer(obj->base, (void*)tx, rx, tx_length, &obj->context);
    }
}

uint32_t spi_irq_handler_asynch(spi_t *obj_in)
{
    spi_obj_t *obj = OBJ_P(obj_in);
    uint32_t event = 0;
    void *buf;
    // Process actual interrupt.
    Cy_SCB_SPI_Interrupt(obj->base, &obj->context);
    if (obj->context.status & CY_SCB_SPI_TRANSFER_OVERFLOW) {
        event = SPI_EVENT_RX_OVERFLOW;
    } else if (obj->context.status & (CY_SCB_SPI_SLAVE_TRANSFER_ERR | CY_SCB_SPI_TRANSFER_OVERFLOW)) {
        event = SPI_EVENT_ERROR;
    } else if (0 == (obj->context.status & CY_SCB_SPI_TRANSFER_ACTIVE)) {
        // Check to see if the second transfer phase needs to be started.
        MBED_ASSERT(!(obj->tx_buffer && obj->rx_buffer));
        if (obj->tx_buffer) {
            obj->pending = PENDING_TX;
            buf = obj->tx_buffer;
            obj->tx_buffer = NULL;
            Cy_SCB_SPI_Transfer(obj->base, buf, NULL, obj->tx_buffer_size, &obj->context);
        } else if (obj->rx_buffer) {
            obj->pending = PENDING_RX;
            buf = obj->rx_buffer;
            obj->rx_buffer = NULL;
            Cy_SCB_SPI_Transfer(obj->base, NULL, buf, obj->rx_buffer_size, &obj->context);
        } else {
            event = SPI_EVENT_COMPLETE;
            obj->pending = PENDING_NONE;
        }
    }
    return event & obj->events;
}

uint8_t spi_active(spi_t *obj_in)
{
    spi_obj_t *obj = OBJ_P(obj_in);
    return (obj->pending != PENDING_NONE);
}

void spi_abort_asynch(spi_t *obj_in)
{
    spi_obj_t *obj = OBJ_P(obj_in);
    Cy_SCB_SPI_AbortTransfer(obj->base, &obj->context);
}

#endif // DEVICE_ASYNCH
