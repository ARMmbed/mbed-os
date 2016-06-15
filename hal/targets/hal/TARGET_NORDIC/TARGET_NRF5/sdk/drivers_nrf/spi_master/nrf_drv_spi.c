/* Copyright (c) 2015 Nordic Semiconductor. All Rights Reserved.
 *
 * The information contained herein is property of Nordic Semiconductor ASA.
 * Terms and conditions of usage are described in detail in NORDIC
 * SEMICONDUCTOR STANDARD SOFTWARE LICENSE AGREEMENT.
 *
 * Licensees are granted free, non-transferable use of the information. NO
 * WARRANTY of ANY KIND is provided. This heading must NOT be removed from
 * the file.
 *
 */

#include "nrf_drv_spi.h"
#include "nrf_drv_common.h"
#include "nrf_gpio.h"
#include "nrf_assert.h"
#include "app_util_platform.h"


#ifndef NRF52
    // Make sure SPIx_USE_EASY_DMA is 0 for nRF51 (if a common
    // "nrf_drv_config.h" file is provided for nRF51 and nRF52).
    #undef  SPI0_USE_EASY_DMA
    #define SPI0_USE_EASY_DMA 0
    #undef  SPI1_USE_EASY_DMA
    #define SPI1_USE_EASY_DMA 0
    #undef  SPI2_USE_EASY_DMA
    #define SPI2_USE_EASY_DMA 0
#endif

// This set of macros makes it possible to exclude parts of code when one type
// of supported peripherals is not used.
#if ((SPI0_ENABLED && SPI0_USE_EASY_DMA) || \
     (SPI1_ENABLED && SPI1_USE_EASY_DMA) || \
     (SPI2_ENABLED && SPI2_USE_EASY_DMA))
    #define SPIM_IN_USE
#endif
#if ((SPI0_ENABLED && !SPI0_USE_EASY_DMA) || \
     (SPI1_ENABLED && !SPI1_USE_EASY_DMA) || \
     (SPI2_ENABLED && !SPI2_USE_EASY_DMA))
    #define SPI_IN_USE
#endif
#if defined(SPIM_IN_USE) && defined(SPI_IN_USE)
    // SPIM and SPI combined
    #define CODE_FOR_SPIM(code) if (p_instance->use_easy_dma) { code }
    #define CODE_FOR_SPI(code)  else { code }
#elif defined(SPIM_IN_USE) && !defined(SPI_IN_USE)
    // SPIM only
    #define CODE_FOR_SPIM(code) { code }
    #define CODE_FOR_SPI(code)
#elif !defined(SPIM_IN_USE) && defined(SPI_IN_USE)
    // SPI only
    #define CODE_FOR_SPIM(code)
    #define CODE_FOR_SPI(code)  { code }
#else
    #error "Wrong configuration."
#endif

#ifdef SPIM_IN_USE
#ifdef NRF52_PAN_23
#define END_INT_MASK     (NRF_SPIM_INT_ENDTX_MASK | NRF_SPIM_INT_ENDRX_MASK)
#else
#define END_INT_MASK     NRF_SPIM_INT_END_MASK
#endif
#endif

// Control block - driver instance local data.
typedef struct
{
    nrf_drv_spi_handler_t handler;
    nrf_drv_spi_evt_t     evt;  // Keep the struct that is ready for event handler. Less memcpy.
    nrf_drv_state_t       state;
    volatile bool         transfer_in_progress;

    // [no need for 'volatile' attribute for the following members, as they
    //  are not concurrently used in IRQ handlers and main line code]
    uint8_t         ss_pin;
    uint8_t         orc;
    uint8_t         bytes_transferred;

    bool tx_done : 1;
    bool rx_done : 1;
} spi_control_block_t;
static spi_control_block_t m_cb[SPI_COUNT];

static nrf_drv_spi_config_t const m_default_config[SPI_COUNT] = {
#if SPI0_ENABLED
    NRF_DRV_SPI_DEFAULT_CONFIG(0),
#endif
#if SPI1_ENABLED
    NRF_DRV_SPI_DEFAULT_CONFIG(1),
#endif
#if SPI2_ENABLED
    NRF_DRV_SPI_DEFAULT_CONFIG(2),
#endif
};

#if PERIPHERAL_RESOURCE_SHARING_ENABLED
    #define IRQ_HANDLER_NAME(n) irq_handler_for_instance_##n
    #define IRQ_HANDLER(n)      static void IRQ_HANDLER_NAME(n)(void)

    #if SPI0_ENABLED
        IRQ_HANDLER(0);
    #endif
    #if SPI1_ENABLED
        IRQ_HANDLER(1);
    #endif
    #if SPI2_ENABLED
        IRQ_HANDLER(2);
    #endif
    static nrf_drv_irq_handler_t const m_irq_handlers[SPI_COUNT] = {
    #if SPI0_ENABLED
        IRQ_HANDLER_NAME(0),
    #endif
    #if SPI1_ENABLED
        IRQ_HANDLER_NAME(1),
    #endif
    #if SPI2_ENABLED
        IRQ_HANDLER_NAME(2),
    #endif
    };
#else
    #define IRQ_HANDLER(n) void SPI##n##_IRQ_HANDLER(void)
#endif // PERIPHERAL_RESOURCE_SHARING_ENABLED


ret_code_t nrf_drv_spi_init(nrf_drv_spi_t const * const p_instance,
                            nrf_drv_spi_config_t const * p_config,
                            nrf_drv_spi_handler_t handler)
{
    spi_control_block_t * p_cb  = &m_cb[p_instance->drv_inst_idx];

    if (p_cb->state != NRF_DRV_STATE_UNINITIALIZED)
    {
        return NRF_ERROR_INVALID_STATE;
    }

#if PERIPHERAL_RESOURCE_SHARING_ENABLED
    if (nrf_drv_common_per_res_acquire(p_instance->p_registers,
            m_irq_handlers[p_instance->drv_inst_idx]) != NRF_SUCCESS)
    {
        return NRF_ERROR_BUSY;
    }
#endif

    if (p_config == NULL)
    {
        p_config = &m_default_config[p_instance->drv_inst_idx];
    }

    p_cb->handler = handler;

    uint32_t mosi_pin;
    uint32_t miso_pin;
    // Configure pins used by the peripheral:
    // - SCK - output with initial value corresponding with the SPI mode used:
    //   0 - for modes 0 and 1 (CPOL = 0), 1 - for modes 2 and 3 (CPOL = 1);
    //   according to the reference manual guidelines this pin and its input
    //   buffer must always be connected for the SPI to work.
    if (p_config->mode <= NRF_DRV_SPI_MODE_1)
    {
        nrf_gpio_pin_clear(p_config->sck_pin);
    }
    else
    {
        nrf_gpio_pin_set(p_config->sck_pin);
    }
    NRF_GPIO->PIN_CNF[p_config->sck_pin] =
        (GPIO_PIN_CNF_DIR_Output        << GPIO_PIN_CNF_DIR_Pos)
      | (GPIO_PIN_CNF_INPUT_Connect     << GPIO_PIN_CNF_INPUT_Pos)
      | (GPIO_PIN_CNF_PULL_Disabled     << GPIO_PIN_CNF_PULL_Pos)
      | (GPIO_PIN_CNF_DRIVE_S0S1        << GPIO_PIN_CNF_DRIVE_Pos)
      | (GPIO_PIN_CNF_SENSE_Disabled    << GPIO_PIN_CNF_SENSE_Pos);
    // - MOSI (optional) - output with initial value 0,
    if (p_config->mosi_pin != NRF_DRV_SPI_PIN_NOT_USED)
    {
        mosi_pin = p_config->mosi_pin;
        nrf_gpio_pin_clear(mosi_pin);
        nrf_gpio_cfg_output(mosi_pin);
    }
    else
    {
        mosi_pin = NRF_SPI_PIN_NOT_CONNECTED;
    }
    // - MISO (optional) - input,
    if (p_config->miso_pin != NRF_DRV_SPI_PIN_NOT_USED)
    {
        miso_pin = p_config->miso_pin;
        nrf_gpio_cfg_input(miso_pin, NRF_GPIO_PIN_NOPULL);
    }
    else
    {
        miso_pin = NRF_SPI_PIN_NOT_CONNECTED;
    }
    // - Slave Select (optional) - output with initial value 1 (inactive).
    if (p_config->ss_pin != NRF_DRV_SPI_PIN_NOT_USED)
    {
        nrf_gpio_pin_set(p_config->ss_pin);
        nrf_gpio_cfg_output(p_config->ss_pin);
    }
    m_cb[p_instance->drv_inst_idx].ss_pin = p_config->ss_pin;

    CODE_FOR_SPIM
    (
        NRF_SPIM_Type * p_spim = p_instance->p_registers;
        nrf_spim_pins_set(p_spim, p_config->sck_pin, mosi_pin, miso_pin);
        nrf_spim_frequency_set(p_spim,
            (nrf_spim_frequency_t)p_config->frequency);
        nrf_spim_configure(p_spim,
            (nrf_spim_mode_t)p_config->mode,
            (nrf_spim_bit_order_t)p_config->bit_order);

        nrf_spim_orc_set(p_spim, p_config->orc);

        if (p_cb->handler)
        {
            nrf_spim_int_enable(p_spim, END_INT_MASK | NRF_SPIM_INT_STOPPED_MASK);
        }

        nrf_spim_enable(p_spim);
    )
    CODE_FOR_SPI
    (
        NRF_SPI_Type * p_spi = p_instance->p_registers;
        nrf_spi_pins_set(p_spi, p_config->sck_pin, mosi_pin, miso_pin);
        nrf_spi_frequency_set(p_spi,
            (nrf_spi_frequency_t)p_config->frequency);
        nrf_spi_configure(p_spi,
            (nrf_spi_mode_t)p_config->mode,
            (nrf_spi_bit_order_t)p_config->bit_order);

        m_cb[p_instance->drv_inst_idx].orc = p_config->orc;

        if (p_cb->handler)
        {
            nrf_spi_int_enable(p_spi, NRF_SPI_INT_READY_MASK);
        }

        nrf_spi_enable(p_spi);
    )

    if (p_cb->handler)
    {
        nrf_drv_common_irq_enable(p_instance->irq, p_config->irq_priority);
    }

    p_cb->transfer_in_progress = false;
    p_cb->state = NRF_DRV_STATE_INITIALIZED;

    return NRF_SUCCESS;
}

void nrf_drv_spi_uninit(nrf_drv_spi_t const * const p_instance)
{
    spi_control_block_t * p_cb = &m_cb[p_instance->drv_inst_idx];
    ASSERT(p_cb->state != NRF_DRV_STATE_UNINITIALIZED);

    if (p_cb->handler)
    {
        nrf_drv_common_irq_disable(p_instance->irq);
    }

    #define DISABLE_ALL  0xFFFFFFFF

    CODE_FOR_SPIM
    (
        NRF_SPIM_Type * p_spim = p_instance->p_registers;
        if (p_cb->handler)
        {
            nrf_spim_int_disable(p_spim, DISABLE_ALL);
            if (p_cb->transfer_in_progress)
            {
                // Ensure that SPI is not performing any transfer.
                nrf_spim_task_trigger(p_spim, NRF_SPIM_TASK_STOP);
                while (!nrf_spim_event_check(p_spim, NRF_SPIM_EVENT_STOPPED)) {}
                p_cb->transfer_in_progress = false;
            }
        }
        nrf_spim_disable(p_spim);
    )
    CODE_FOR_SPI
    (
        NRF_SPI_Type * p_spi = p_instance->p_registers;
        if (p_cb->handler)
        {
            nrf_spi_int_disable(p_spi, DISABLE_ALL);
        }
        nrf_spi_disable(p_spi);
    )
    #undef DISABLE_ALL

#if PERIPHERAL_RESOURCE_SHARING_ENABLED
    nrf_drv_common_per_res_release(p_instance->p_registers);
#endif

    p_cb->state = NRF_DRV_STATE_UNINITIALIZED;
}

ret_code_t nrf_drv_spi_transfer(nrf_drv_spi_t const * const p_instance,
                                uint8_t const * p_tx_buffer,
                                uint8_t         tx_buffer_length,
                                uint8_t       * p_rx_buffer,
                                uint8_t         rx_buffer_length)
{
    nrf_drv_spi_xfer_desc_t xfer_desc;
    xfer_desc.p_tx_buffer = p_tx_buffer;
    xfer_desc.p_rx_buffer = p_rx_buffer;
    xfer_desc.tx_length   = tx_buffer_length;
    xfer_desc.rx_length   = rx_buffer_length;

    return nrf_drv_spi_xfer(p_instance, &xfer_desc, 0);
}

static void finish_transfer(spi_control_block_t * p_cb)
{
    // If Slave Select signal is used, this is the time to deactivate it.
    if (p_cb->ss_pin != NRF_DRV_SPI_PIN_NOT_USED)
    {
        nrf_gpio_pin_set(p_cb->ss_pin);
    }

    // By clearing this flag before calling the handler we allow subsequent
    // transfers to be started directly from the handler function.
    p_cb->transfer_in_progress = false;
    p_cb->evt.type = NRF_DRV_SPI_EVENT_DONE;
    p_cb->handler(&p_cb->evt);
}

#ifdef SPI_IN_USE
// This function is called from IRQ handler or, in blocking mode, directly
// from the 'nrf_drv_spi_transfer' function.
// It returns true as long as the transfer should be continued, otherwise (when
// there is nothing more to send/receive) it returns false.
static bool transfer_byte(NRF_SPI_Type * p_spi, spi_control_block_t * p_cb)
{
    // Read the data byte received in this transfer and store it in RX buffer,
    // if needed.
    volatile uint8_t rx_data = nrf_spi_rxd_get(p_spi);
    if (p_cb->bytes_transferred < p_cb->evt.data.done.rx_length)
    {
        p_cb->evt.data.done.p_rx_buffer[p_cb->bytes_transferred] = rx_data;
    }

    ++p_cb->bytes_transferred;

    // Check if there are more bytes to send or receive and write proper data
    // byte (next one from TX buffer or over-run character) to the TXD register
    // when needed.
    // NOTE - we've already used 'p_cb->bytes_transferred + 1' bytes from our
    //        buffers, because we take advantage of double buffering of TXD
    //        register (so in effect one byte is still being transmitted now);
    //        see how the transfer is started in the 'nrf_drv_spi_transfer'
    //        function.
    uint16_t bytes_used = p_cb->bytes_transferred + 1;
    if (bytes_used < p_cb->evt.data.done.tx_length)
    {
        nrf_spi_txd_set(p_spi, p_cb->evt.data.done.p_tx_buffer[bytes_used]);
        return true;
    }
    else if (bytes_used < p_cb->evt.data.done.rx_length)
    {
        nrf_spi_txd_set(p_spi, p_cb->orc);
        return true;
    }

    return (p_cb->bytes_transferred < p_cb->evt.data.done.tx_length ||
            p_cb->bytes_transferred < p_cb->evt.data.done.rx_length);
}

static void spi_xfer(NRF_SPI_Type                  * p_spi,
                     spi_control_block_t           * p_cb,
                     nrf_drv_spi_xfer_desc_t const * p_xfer_desc)
{
    p_cb->bytes_transferred = 0;
    nrf_spi_int_disable(p_spi, NRF_SPI_INT_READY_MASK);

    nrf_spi_event_clear(p_spi, NRF_SPI_EVENT_READY);

    // Start the transfer by writing some byte to the TXD register;
    // if TX buffer is not empty, take the first byte from this buffer,
    // otherwise - use over-run character.
    nrf_spi_txd_set(p_spi,
        (p_xfer_desc->tx_length > 0 ?  p_xfer_desc->p_tx_buffer[0] : p_cb->orc));

    // TXD register is double buffered, so next byte to be transmitted can
    // be written immediately, if needed, i.e. if TX or RX transfer is to
    // be more that 1 byte long. Again - if there is something more in TX
    // buffer send it, otherwise use over-run character.
    if (p_xfer_desc->tx_length > 1)
    {
        nrf_spi_txd_set(p_spi, p_xfer_desc->p_tx_buffer[1]);
    }
    else if (p_xfer_desc->rx_length > 1)
    {
        nrf_spi_txd_set(p_spi, p_cb->orc);
    }

    // For blocking mode (user handler not provided) wait here for READY
    // events (indicating that the byte from TXD register was transmitted
    // and a new incoming byte was moved to the RXD register) and continue
    // transaction until all requested bytes are transferred.
    // In non-blocking mode - IRQ service routine will do this stuff.
    if (p_cb->handler)
    {
        nrf_spi_int_enable(p_spi, NRF_SPI_INT_READY_MASK);
    }
    else
    {
        do {
            while (!nrf_spi_event_check(p_spi, NRF_SPI_EVENT_READY)) {}
            nrf_spi_event_clear(p_spi, NRF_SPI_EVENT_READY);
        } while (transfer_byte(p_spi, p_cb));
        if (p_cb->ss_pin != NRF_DRV_SPI_PIN_NOT_USED)
        {
            nrf_gpio_pin_set(p_cb->ss_pin);
        }
    }
}
#endif // SPI_IN_USE

#ifdef SPIM_IN_USE
__STATIC_INLINE void spim_int_enable(NRF_SPIM_Type * p_spim, bool enable)
{
    if (!enable)
    {
        nrf_spim_int_disable(p_spim, END_INT_MASK | NRF_SPIM_INT_STOPPED_MASK);
    }
    else
    {
        nrf_spim_int_enable(p_spim, END_INT_MASK |  NRF_SPIM_INT_STOPPED_MASK);
    }
}

__STATIC_INLINE void spim_list_enable_handle(NRF_SPIM_Type * p_spim, uint32_t flags)
{
#ifndef NRF52_PAN_46
    if (NRF_DRV_SPI_FLAG_TX_POSTINC & flags)
    {
        nrf_spim_tx_list_enable(p_spim);
    }
    else
    {
        nrf_spim_tx_list_disable(p_spim);
    }

    if (NRF_DRV_SPI_FLAG_RX_POSTINC & flags)
    {
        nrf_spim_rx_list_enable(p_spim);
    }
    else
    {
        nrf_spim_rx_list_disable(p_spim);
    }
#endif
}

static ret_code_t spim_xfer(NRF_SPIM_Type                * p_spim,
                           spi_control_block_t           * p_cb,
                           nrf_drv_spi_xfer_desc_t const * p_xfer_desc,
                           uint32_t                        flags)
{
    // EasyDMA requires that transfer buffers are placed in Data RAM region;
    // signal error if they are not.
    if ((p_xfer_desc->p_tx_buffer != NULL && !nrf_drv_is_in_RAM(p_xfer_desc->p_tx_buffer)) ||
        (p_xfer_desc->p_rx_buffer != NULL && !nrf_drv_is_in_RAM(p_xfer_desc->p_rx_buffer)))
    {
        p_cb->transfer_in_progress = false;
        return NRF_ERROR_INVALID_ADDR;
    }

    nrf_spim_tx_buffer_set(p_spim, p_xfer_desc->p_tx_buffer, p_xfer_desc->tx_length);
    nrf_spim_rx_buffer_set(p_spim, p_xfer_desc->p_rx_buffer, p_xfer_desc->rx_length);

#ifdef NRF52_PAN_23
    nrf_spim_event_clear(p_spim, NRF_SPIM_EVENT_ENDTX);
    nrf_spim_event_clear(p_spim, NRF_SPIM_EVENT_ENDRX);
#else
    nrf_spim_event_clear(p_spim, NRF_SPIM_EVENT_END);
#endif
    nrf_spim_event_clear(p_spim, NRF_SPIM_EVENT_STOPPED);

    spim_list_enable_handle(p_spim, flags);

    if (!(flags & NRF_DRV_SPI_FLAG_HOLD_XFER))
    {
        nrf_spim_task_trigger(p_spim, NRF_SPIM_TASK_START);
    }

    if (!p_cb->handler)
    {
#ifdef NRF52_PAN_23
        while (!nrf_spim_event_check(p_spim, NRF_SPIM_EVENT_ENDTX) ||
               !nrf_spim_event_check(p_spim, NRF_SPIM_EVENT_ENDRX)) {}
#else
        while (!nrf_spim_event_check(p_spim, NRF_SPIM_EVENT_END)){}
#endif
        // Stop the peripheral after transaction is finished.
        nrf_spim_task_trigger(p_spim, NRF_SPIM_TASK_STOP);
        while (!nrf_spim_event_check(p_spim, NRF_SPIM_EVENT_STOPPED)) {}
        if (p_cb->ss_pin != NRF_DRV_SPI_PIN_NOT_USED)
        {
            nrf_gpio_pin_set(p_cb->ss_pin);
        }
    }
		else
		{
				spim_int_enable(p_spim, !(flags & NRF_DRV_SPI_FLAG_NO_XFER_EVT_HANDLER));
		}
    return NRF_SUCCESS;
}
#endif

ret_code_t nrf_drv_spi_xfer(nrf_drv_spi_t     const * const p_instance,
                            nrf_drv_spi_xfer_desc_t const * p_xfer_desc,
                            uint32_t                        flags)
{
    spi_control_block_t * p_cb  = &m_cb[p_instance->drv_inst_idx];
    ASSERT(p_cb->state != NRF_DRV_STATE_UNINITIALIZED);
    ASSERT(p_tx_buffer != NULL || tx_buffer_length == 0);
    ASSERT(p_rx_buffer != NULL || rx_buffer_length == 0);

    if (p_cb->transfer_in_progress)
    {
        return NRF_ERROR_BUSY;
    }
    else
    {
        if (p_cb->handler && !(flags & (NRF_DRV_SPI_FLAG_REPEATED_XFER | NRF_DRV_SPI_FLAG_NO_XFER_EVT_HANDLER)))
        {
            p_cb->transfer_in_progress = true;
        }
    }

    p_cb->evt.data.done = *p_xfer_desc;
    p_cb->tx_done = false;
    p_cb->rx_done = false;

    if (p_cb->ss_pin != NRF_DRV_SPI_PIN_NOT_USED)
    {
        nrf_gpio_pin_clear(p_cb->ss_pin);
    }
    CODE_FOR_SPIM
    (
        return spim_xfer(p_instance->p_registers, p_cb,  p_xfer_desc, flags);
    )
    CODE_FOR_SPI
    (
        if (flags)
        {
            p_cb->transfer_in_progress = false;
            return NRF_ERROR_NOT_SUPPORTED;
        }
        spi_xfer(p_instance->p_registers, p_cb, p_xfer_desc);
        return NRF_SUCCESS;
    )
}
#ifdef SPIM_IN_USE
static void irq_handler_spim(NRF_SPIM_Type * p_spim, spi_control_block_t * p_cb)
{
    ASSERT(p_cb->handler);

#ifdef NRF52_PAN_23
    if (nrf_spim_event_check(p_spim, NRF_SPIM_EVENT_STOPPED))
    {
        nrf_spim_event_clear(p_spim, NRF_SPIM_EVENT_STOPPED);
        finish_transfer(p_cb);
    }
    else
    {
        if (nrf_spim_event_check(p_spim, NRF_SPIM_EVENT_ENDTX))
        {
            nrf_spim_event_clear(p_spim, NRF_SPIM_EVENT_ENDTX);
            p_cb->tx_done = true;
        }
        if (nrf_spim_event_check(p_spim, NRF_SPIM_EVENT_ENDRX))
        {
            nrf_spim_event_clear(p_spim, NRF_SPIM_EVENT_ENDRX);
            p_cb->rx_done = true;
        }
        if (p_cb->tx_done && p_cb->rx_done)
        {
            nrf_spim_task_trigger(p_spim, NRF_SPIM_TASK_STOP);
        }
    }
#else
    if (nrf_spim_event_check(p_spim, NRF_SPIM_EVENT_END))
    {
        nrf_spim_event_clear(p_spim, NRF_SPIM_EVENT_END);
        finish_transfer(p_cb);
    }
#endif
}

uint32_t nrf_drv_spi_start_task_get(nrf_drv_spi_t const * p_instance)
{
    NRF_SPIM_Type * p_spim = (NRF_SPIM_Type *)p_instance->p_registers;
    return nrf_spim_task_address_get(p_spim, NRF_SPIM_TASK_START);
}

uint32_t nrf_drv_spi_end_event_get(nrf_drv_spi_t const * p_instance)
{
    NRF_SPIM_Type * p_spim = (NRF_SPIM_Type *)p_instance->p_registers;
    return nrf_spim_event_address_get(p_spim, NRF_SPIM_EVENT_END);
}
#endif // SPIM_IN_USE

#ifdef SPI_IN_USE
static void irq_handler_spi(NRF_SPI_Type * p_spi, spi_control_block_t * p_cb)
{
    ASSERT(p_cb->handler);

    nrf_spi_event_clear(p_spi, NRF_SPI_EVENT_READY);

    if (!transfer_byte(p_spi, p_cb))
    {
        finish_transfer(p_cb);
    }
}
#endif // SPI_IN_USE

#if SPI0_ENABLED
IRQ_HANDLER(0)
{
    spi_control_block_t * p_cb  = &m_cb[SPI0_INSTANCE_INDEX];
    #if SPI0_USE_EASY_DMA
        irq_handler_spim(NRF_SPIM0, p_cb);
    #else
        irq_handler_spi(NRF_SPI0, p_cb);
    #endif
}
#endif // SPI0_ENABLED

#if SPI1_ENABLED
IRQ_HANDLER(1)
{
    spi_control_block_t * p_cb  = &m_cb[SPI1_INSTANCE_INDEX];
    #if SPI1_USE_EASY_DMA
        irq_handler_spim(NRF_SPIM1, p_cb);
    #else
        irq_handler_spi(NRF_SPI1, p_cb);
    #endif
}
#endif // SPI1_ENABLED

#if SPI2_ENABLED
IRQ_HANDLER(2)
{
    spi_control_block_t * p_cb  = &m_cb[SPI2_INSTANCE_INDEX];
    #if SPI2_USE_EASY_DMA
        irq_handler_spim(NRF_SPIM2, p_cb);
    #else
        irq_handler_spi(NRF_SPI2, p_cb);
    #endif
}
#endif // SPI2_ENABLED
