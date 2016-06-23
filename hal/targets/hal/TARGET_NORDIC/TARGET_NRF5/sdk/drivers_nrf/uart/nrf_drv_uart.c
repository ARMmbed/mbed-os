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

#include "nrf_drv_uart.h"
#include "nrf_assert.h"
#include "nordic_common.h"
#include "nrf_drv_common.h"
#include "nrf_gpio.h"
#include "app_util_platform.h"

// This set of macros makes it possible to exclude parts of code, when one type
// of supported peripherals is not used.
#ifdef NRF51
#define UART_IN_USE
#elif defined(NRF52)
#if (UART_EASY_DMA_SUPPORT == 1)
#define UARTE_IN_USE
#endif
#if (UART_LEGACY_SUPPORT == 1)
#define UART_IN_USE
#endif
#endif


#if (defined(UARTE_IN_USE) && defined(UART_IN_USE))
    // UARTE and UART combined
    #define CODE_FOR_UARTE(code) if (m_cb.use_easy_dma) { code }
    #define CODE_FOR_UART(code)   else { code }
#elif (defined(UARTE_IN_USE) && !defined(UART_IN_USE))
    // UARTE only
    #define CODE_FOR_UARTE(code) { code }
    #define CODE_FOR_UART(code)
#elif (!defined(UARTE_IN_USE) && defined(UART_IN_USE))
    // UART only
    #define CODE_FOR_UARTE(code)
    #define CODE_FOR_UART(code) { code }
#else
    #error "Wrong configuration."
#endif

#ifndef IS_EASY_DMA_RAM_ADDRESS
    #define IS_EASY_DMA_RAM_ADDRESS(addr) (((uint32_t)addr & 0xFFFF0000) == 0x20000000)
#endif

#define TX_COUNTER_ABORT_REQ_VALUE 256

typedef struct
{
    void                   * p_context;
    nrf_uart_event_handler_t handler;
    uint8_t          const * p_tx_buffer;
    uint8_t                * p_rx_buffer;
    uint8_t                * p_rx_secondary_buffer;
    volatile uint16_t        tx_counter;
    uint8_t                  tx_buffer_length;
    uint8_t                  rx_buffer_length;
    uint8_t                  rx_secondary_buffer_length;
    volatile uint8_t         rx_counter;
    bool                     rx_enabled;
    nrf_drv_state_t          state;
#if (defined(UARTE_IN_USE) && defined(UART_IN_USE))
    bool                     use_easy_dma;
#endif
} uart_control_block_t;

static uart_control_block_t m_cb;
static const nrf_drv_uart_config_t m_default_config = NRF_DRV_UART_DEFAULT_CONFIG;

__STATIC_INLINE void apply_config(nrf_drv_uart_config_t const * p_config)
{
    nrf_gpio_pin_set(p_config->pseltxd);
    nrf_gpio_cfg_output(p_config->pseltxd);
    nrf_gpio_cfg_input(p_config->pselrxd, NRF_GPIO_PIN_NOPULL);

    CODE_FOR_UARTE
    (
        nrf_uarte_baudrate_set(NRF_UARTE0, (nrf_uarte_baudrate_t)p_config->baudrate);
        nrf_uarte_configure(NRF_UARTE0, (nrf_uarte_parity_t)p_config->parity,
                            (nrf_uarte_hwfc_t)p_config->hwfc);
        nrf_uarte_txrx_pins_set(NRF_UARTE0, p_config->pseltxd, p_config->pselrxd);
        if (p_config->hwfc == NRF_UART_HWFC_ENABLED)
        {
            nrf_gpio_cfg_input(p_config->pselcts, NRF_GPIO_PIN_NOPULL);
            nrf_gpio_pin_set(p_config->pselrts);
            nrf_gpio_cfg_output(p_config->pselrts);
            nrf_uarte_hwfc_pins_set(NRF_UARTE0, p_config->pselrts, p_config->pselcts);
        }
    )
    CODE_FOR_UART
    (
        nrf_uart_baudrate_set(NRF_UART0, p_config->baudrate);
        nrf_uart_configure(NRF_UART0, p_config->parity, p_config->hwfc);
        nrf_uart_txrx_pins_set(NRF_UART0, p_config->pseltxd, p_config->pselrxd);
        if (p_config->hwfc == NRF_UART_HWFC_ENABLED)
        {
            nrf_gpio_cfg_input(p_config->pselcts, NRF_GPIO_PIN_NOPULL);
            nrf_gpio_pin_set(p_config->pselrts);
            nrf_gpio_cfg_output(p_config->pselrts);
            nrf_uart_hwfc_pins_set(NRF_UART0, p_config->pselrts, p_config->pselcts);
        }
    )
}

__STATIC_INLINE void interrupts_enable(uint8_t interrupt_priority)
{
    CODE_FOR_UARTE
    (
        nrf_uarte_event_clear(NRF_UARTE0, NRF_UARTE_EVENT_ENDRX);
        nrf_uarte_event_clear(NRF_UARTE0, NRF_UARTE_EVENT_ENDTX);
        nrf_uarte_event_clear(NRF_UARTE0, NRF_UARTE_EVENT_ERROR);
        nrf_uarte_event_clear(NRF_UARTE0, NRF_UARTE_EVENT_RXTO);
        nrf_uarte_int_enable(NRF_UARTE0, NRF_UARTE_INT_ENDRX_MASK |
                                         NRF_UARTE_INT_ENDTX_MASK |
                                         NRF_UARTE_INT_ERROR_MASK |
                                         NRF_UARTE_INT_RXTO_MASK);
    )
    CODE_FOR_UART
    (
        nrf_uart_event_clear(NRF_UART0, NRF_UART_EVENT_TXDRDY);
        nrf_uart_event_clear(NRF_UART0, NRF_UART_EVENT_RXTO);
        nrf_uart_int_enable(NRF_UART0, NRF_UART_INT_MASK_TXDRDY |
                                       NRF_UART_INT_MASK_RXTO);
    )
    nrf_drv_common_irq_enable(UART0_IRQn, interrupt_priority);
}

__STATIC_INLINE void interrupts_disable(void)
{
    CODE_FOR_UARTE
    (
        nrf_uarte_int_disable(NRF_UARTE0, NRF_UARTE_INT_ENDRX_MASK |
                                          NRF_UARTE_INT_ENDTX_MASK |
                                          NRF_UARTE_INT_ERROR_MASK |
                                          NRF_UARTE_INT_RXTO_MASK);
    )
    CODE_FOR_UART
    (
        nrf_uart_int_disable(NRF_UART0, NRF_UART_INT_MASK_RXDRDY |
                                        NRF_UART_INT_MASK_TXDRDY |
                                        NRF_UART_INT_MASK_ERROR  |
                                        NRF_UART_INT_MASK_RXTO);
    )
    nrf_drv_common_irq_disable(UART0_IRQn);
}

__STATIC_INLINE void pins_to_default(void)
{
    /* Reset pins to default states */
    uint32_t txd;
    uint32_t rxd;
    uint32_t rts;
    uint32_t cts;

    CODE_FOR_UARTE
    (
        txd = nrf_uarte_tx_pin_get(NRF_UARTE0);
        rxd = nrf_uarte_rx_pin_get(NRF_UARTE0);
        rts = nrf_uarte_rts_pin_get(NRF_UARTE0);
        cts = nrf_uarte_cts_pin_get(NRF_UARTE0);
        nrf_uarte_txrx_pins_disconnect(NRF_UARTE0);
        nrf_uarte_hwfc_pins_disconnect(NRF_UARTE0);
    )
    CODE_FOR_UART
    (
        txd = nrf_uart_tx_pin_get(NRF_UART0);
        rxd = nrf_uart_rx_pin_get(NRF_UART0);
        rts = nrf_uart_rts_pin_get(NRF_UART0);
        cts = nrf_uart_cts_pin_get(NRF_UART0);
        nrf_uart_txrx_pins_disconnect(NRF_UART0);
        nrf_uart_hwfc_pins_disconnect(NRF_UART0);
    )

    nrf_gpio_cfg_default(txd);
    nrf_gpio_cfg_default(rxd);

    if (cts != NRF_UART_PSEL_DISCONNECTED)
    {
        nrf_gpio_cfg_default(cts);
    }

    if (rts != NRF_UART_PSEL_DISCONNECTED)
    {
        nrf_gpio_cfg_default(rts);
    }

}

__STATIC_INLINE void uart_enable(void)
{
    CODE_FOR_UARTE(nrf_uarte_enable(NRF_UARTE0);)
    CODE_FOR_UART(nrf_uart_enable(NRF_UART0););
}

__STATIC_INLINE void uart_disable(void)
{
    CODE_FOR_UARTE(nrf_uarte_disable(NRF_UARTE0);)
    CODE_FOR_UART(nrf_uart_disable(NRF_UART0););
}

ret_code_t nrf_drv_uart_init(nrf_drv_uart_config_t const * p_config,
                             nrf_uart_event_handler_t      event_handler)
{
    if (m_cb.state != NRF_DRV_STATE_UNINITIALIZED)
    {
        return NRF_ERROR_INVALID_STATE;
    }

    if (p_config == NULL)
    {
        p_config = &m_default_config;
    }
#if (defined(UARTE_IN_USE) && defined(UART_IN_USE))
    m_cb.use_easy_dma = p_config->use_easy_dma;
#endif
    apply_config(p_config);

    m_cb.handler = event_handler;
    m_cb.p_context = p_config->p_context;

    if (m_cb.handler)
    {
        interrupts_enable(p_config->interrupt_priority);
    }

    uart_enable();
    m_cb.rx_buffer_length = 0;
    m_cb.rx_secondary_buffer_length = 0;
    m_cb.tx_buffer_length = 0;
    m_cb.state = NRF_DRV_STATE_INITIALIZED;
    m_cb.rx_enabled = false;
    return NRF_SUCCESS;
}

void nrf_drv_uart_uninit(void)
{
    uart_disable();

    if (m_cb.handler)
    {
        interrupts_disable();
    }

    pins_to_default();

    m_cb.state = NRF_DRV_STATE_UNINITIALIZED;
    m_cb.handler = NULL;
}

#if defined(UART_IN_USE)
__STATIC_INLINE void tx_byte(void)
{
    nrf_uart_event_clear(NRF_UART0, NRF_UART_EVENT_TXDRDY);
    uint8_t txd = m_cb.p_tx_buffer[m_cb.tx_counter];
    m_cb.tx_counter++;
    nrf_uart_txd_set(NRF_UART0, txd);
}

__STATIC_INLINE ret_code_t nrf_drv_uart_tx_for_uart()
{
    ret_code_t err_code = NRF_SUCCESS;
    
    nrf_uart_event_clear(NRF_UART0, NRF_UART_EVENT_TXDRDY);
    nrf_uart_task_trigger(NRF_UART0, NRF_UART_TASK_STARTTX);

    tx_byte();

    if (m_cb.handler == NULL)
    {
        while (m_cb.tx_counter < (uint16_t) m_cb.tx_buffer_length)
        {
            while (!nrf_uart_event_check(NRF_UART0, NRF_UART_EVENT_TXDRDY) &&
                    m_cb.tx_counter != TX_COUNTER_ABORT_REQ_VALUE)
            {
            }
            if (m_cb.tx_counter != TX_COUNTER_ABORT_REQ_VALUE)
            {
                tx_byte();
            }
        }

        if (m_cb.tx_counter == TX_COUNTER_ABORT_REQ_VALUE)
        {
            err_code = NRF_ERROR_FORBIDDEN;
        }
        else
        {
            while (!nrf_uart_event_check(NRF_UART0, NRF_UART_EVENT_TXDRDY))
            {
            }
            nrf_uart_task_trigger(NRF_UART0, NRF_UART_TASK_STOPTX);
        }
        m_cb.tx_buffer_length = 0;
    }
    
    return err_code;
}
#endif

#if defined(UARTE_IN_USE)
__STATIC_INLINE ret_code_t nrf_drv_uart_tx_for_uarte()
{    
    ret_code_t err_code = NRF_SUCCESS;
    
    nrf_uarte_event_clear(NRF_UARTE0, NRF_UARTE_EVENT_ENDTX);
    nrf_uarte_event_clear(NRF_UARTE0, NRF_UARTE_EVENT_TXSTOPPED);
    nrf_uarte_tx_buffer_set(NRF_UARTE0, m_cb.p_tx_buffer, m_cb.tx_buffer_length);
    nrf_uarte_task_trigger(NRF_UARTE0, NRF_UARTE_TASK_STARTTX);

    if (m_cb.handler == NULL)
    {
        bool endtx;
        bool txstopped;
        do
        {
            endtx     = nrf_uarte_event_check(NRF_UARTE0, NRF_UARTE_EVENT_ENDTX);
            txstopped = nrf_uarte_event_check(NRF_UARTE0, NRF_UARTE_EVENT_TXSTOPPED);
        }
        while ((!endtx) && (!txstopped));

        if (txstopped)
        {
            err_code = NRF_ERROR_FORBIDDEN;
        }
        m_cb.tx_buffer_length = 0;
    }
    
    return err_code;
}
#endif

ret_code_t nrf_drv_uart_tx(uint8_t const * const p_data, uint8_t length)
{
    ASSERT(m_cb.state == NRF_DRV_STATE_INITIALIZED);
    ASSERT(length>0);
    ASSERT(p_data);

    CODE_FOR_UARTE
    (
        // EasyDMA requires that transfer buffers are placed in DataRAM,
        // signal error if the are not.
        if (!IS_EASY_DMA_RAM_ADDRESS(p_data))
        {
            return NRF_ERROR_INVALID_ADDR;
        }
    )

    if (nrf_drv_uart_tx_in_progress())
    {
        return NRF_ERROR_BUSY;
    }
    m_cb.tx_buffer_length = length;
    m_cb.p_tx_buffer      = p_data;
    m_cb.tx_counter       = 0;

    CODE_FOR_UARTE
    (
        return nrf_drv_uart_tx_for_uarte();
    )
    CODE_FOR_UART
    (
        return nrf_drv_uart_tx_for_uart();
    )
}

bool nrf_drv_uart_tx_in_progress(void)
{
    return (m_cb.tx_buffer_length != 0);
}

#if defined(UART_IN_USE)
__STATIC_INLINE void rx_enable(void)
{
    nrf_uart_event_clear(NRF_UART0, NRF_UART_EVENT_ERROR);
    nrf_uart_event_clear(NRF_UART0, NRF_UART_EVENT_RXDRDY);
    nrf_uart_task_trigger(NRF_UART0, NRF_UART_TASK_STARTRX);
}

__STATIC_INLINE void rx_byte(void)
{
    if (!m_cb.rx_buffer_length)
    {
        nrf_uart_event_clear(NRF_UART0, NRF_UART_EVENT_RXDRDY);
        // Byte received when buffer is not set - data lost.
        (void) nrf_uart_rxd_get(NRF_UART0);
        return;
    }
    nrf_uart_event_clear(NRF_UART0, NRF_UART_EVENT_RXDRDY);
    m_cb.p_rx_buffer[m_cb.rx_counter] = nrf_uart_rxd_get(NRF_UART0);
    m_cb.rx_counter++;
}

__STATIC_INLINE ret_code_t nrf_drv_uart_rx_for_uart(uint8_t * p_data, uint8_t length, bool second_buffer)
{
    if ((!m_cb.rx_enabled) && (!second_buffer))
    {
        rx_enable();
    }
    if (m_cb.handler == NULL)
    {
        nrf_uart_event_clear(NRF_UART0, NRF_UART_EVENT_RXTO);
        
        bool rxrdy;
        bool rxto;
        bool error;
        do
        {
            do
            {
                error = nrf_uart_event_check(NRF_UART0, NRF_UART_EVENT_ERROR);
                rxrdy = nrf_uart_event_check(NRF_UART0, NRF_UART_EVENT_RXDRDY);
                rxto  = nrf_uart_event_check(NRF_UART0, NRF_UART_EVENT_RXTO);
            } while ((!rxrdy) && (!rxto) && (!error));

            if (error || rxto)
            {
                break;
            }
            rx_byte();
        } while (m_cb.rx_buffer_length > m_cb.rx_counter);

        m_cb.rx_buffer_length = 0;
        if (error)
        {
            return NRF_ERROR_INTERNAL;
        }

        if (rxto)
        {
            return NRF_ERROR_FORBIDDEN;
        }

        if (m_cb.rx_enabled)
        {
            nrf_uart_task_trigger(NRF_UART0, NRF_UART_TASK_STARTRX);
        }
        else
        {
            // Skip stopping RX if driver is forced to be enabled.
            nrf_uart_task_trigger(NRF_UART0, NRF_UART_TASK_STOPRX);
        }
    }
    else
    {
        nrf_uart_int_enable(NRF_UART0, NRF_UART_INT_MASK_RXDRDY | NRF_UART_INT_MASK_ERROR);
    }
    return NRF_SUCCESS;
}
#endif

#if defined(UARTE_IN_USE)
__STATIC_INLINE ret_code_t nrf_drv_uart_rx_for_uarte(uint8_t * p_data, uint8_t length, bool second_buffer)
{
    nrf_uarte_event_clear(NRF_UARTE0, NRF_UARTE_EVENT_ENDRX);
    nrf_uarte_event_clear(NRF_UARTE0, NRF_UARTE_EVENT_RXTO);
    nrf_uarte_rx_buffer_set(NRF_UARTE0, p_data, length);
    if (!second_buffer)
    {
        nrf_uarte_task_trigger(NRF_UARTE0, NRF_UARTE_TASK_STARTRX);
    }
    else
    {
        nrf_uarte_shorts_enable(NRF_UARTE0, NRF_UARTE_SHORT_ENDRX_STARTRX);
    }

    if (m_cb.handler == NULL)
    {
        bool endrx;
        bool rxto;
        bool error;
        do {
            endrx  = nrf_uarte_event_check(NRF_UARTE0, NRF_UARTE_EVENT_ENDRX);
            rxto   = nrf_uarte_event_check(NRF_UARTE0, NRF_UARTE_EVENT_RXTO);
            error  = nrf_uarte_event_check(NRF_UARTE0, NRF_UARTE_EVENT_ERROR);
        }while ((!endrx) && (!rxto) && (!error));

        m_cb.rx_buffer_length = 0;

        if (error)
        {
            return NRF_ERROR_INTERNAL;
        }

        if (rxto)
        {
            return NRF_ERROR_FORBIDDEN;
        }
    }
    else
    {
        nrf_uarte_int_enable(NRF_UARTE0, NRF_UARTE_INT_ERROR_MASK | NRF_UARTE_INT_ENDRX_MASK);
    }
    return NRF_SUCCESS;
}
#endif

ret_code_t nrf_drv_uart_rx(uint8_t * p_data, uint8_t length)
{
    ASSERT(m_cb.state == NRF_DRV_STATE_INITIALIZED);
    ASSERT(length>0);

    CODE_FOR_UARTE
    (
        // EasyDMA requires that transfer buffers are placed in DataRAM,
        // signal error if the are not.
        if (!IS_EASY_DMA_RAM_ADDRESS(p_data))
        {
            return NRF_ERROR_INVALID_ADDR;
        }
    )

    bool second_buffer = false;

    if (m_cb.handler)
    {
        CODE_FOR_UARTE
        (
            nrf_uarte_int_disable(NRF_UARTE0, NRF_UARTE_INT_ERROR_MASK | NRF_UARTE_INT_ENDRX_MASK);
        )
        CODE_FOR_UART
        (
            nrf_uart_int_disable(NRF_UART0, NRF_UART_INT_MASK_RXDRDY | NRF_UART_INT_MASK_ERROR);
        )
    }
    if (m_cb.rx_buffer_length != 0)
    {
        if (m_cb.rx_secondary_buffer_length != 0)
        {
            if (m_cb.handler)
            {
                CODE_FOR_UARTE
                (
                    nrf_uarte_int_enable(NRF_UARTE0, NRF_UARTE_INT_ERROR_MASK | NRF_UARTE_INT_ENDRX_MASK);
                )
                CODE_FOR_UART
                (
                    nrf_uart_int_enable(NRF_UART0, NRF_UART_INT_MASK_RXDRDY | NRF_UART_INT_MASK_ERROR);
                )
            }
            return NRF_ERROR_BUSY;
        }
        second_buffer = true;
    }

    if (!second_buffer)
    {
        m_cb.rx_buffer_length = length;
        m_cb.p_rx_buffer      = p_data;
        m_cb.rx_counter       = 0;
        m_cb.rx_secondary_buffer_length = 0;
    }
    else
    {
        m_cb.p_rx_secondary_buffer = p_data;
        m_cb.rx_secondary_buffer_length = length;
    }

    CODE_FOR_UARTE
    (
        return nrf_drv_uart_rx_for_uarte(p_data, length, second_buffer);
    )
    CODE_FOR_UART
    (
        return nrf_drv_uart_rx_for_uart(p_data, length, second_buffer);
    )
}

void nrf_drv_uart_rx_enable(void)
{
    //Easy dma mode does not support enabling receiver without setting up buffer.
    CODE_FOR_UARTE
    (
        ASSERT(false);
    )
    CODE_FOR_UART
    (
        if (!m_cb.rx_enabled)
        {
            rx_enable();
            m_cb.rx_enabled = true;
        }
    )
}

void nrf_drv_uart_rx_disable(void)
{
    //Easy dma mode does not support enabling receiver without setting up buffer.
    CODE_FOR_UARTE
    (
        ASSERT(false);
    )
    CODE_FOR_UART
    (
        nrf_uart_task_trigger(NRF_UART0, NRF_UART_TASK_STOPRX);
        m_cb.rx_enabled = false;
    )
}

uint32_t nrf_drv_uart_errorsrc_get(void)
{
    uint32_t errsrc;
    CODE_FOR_UARTE
    (
        nrf_uarte_event_clear(NRF_UARTE0, NRF_UARTE_EVENT_ERROR);
        errsrc = nrf_uarte_errorsrc_get_and_clear(NRF_UARTE0);
    )
    CODE_FOR_UART
    (
        nrf_uart_event_clear(NRF_UART0, NRF_UART_EVENT_ERROR);
        errsrc = nrf_uart_errorsrc_get_and_clear(NRF_UART0);
    )
    return errsrc;
}

__STATIC_INLINE void rx_done_event(uint8_t bytes, uint8_t * p_data)
{
    nrf_drv_uart_event_t event;

    event.type             = NRF_DRV_UART_EVT_RX_DONE;
    event.data.rxtx.bytes  = bytes;
    event.data.rxtx.p_data = p_data;

    m_cb.handler(&event,m_cb.p_context);
}

__STATIC_INLINE void tx_done_event(uint8_t bytes)
{
    nrf_drv_uart_event_t event;

    event.type             = NRF_DRV_UART_EVT_TX_DONE;
    event.data.rxtx.bytes  = bytes;
    event.data.rxtx.p_data = (uint8_t *)m_cb.p_tx_buffer;

    m_cb.tx_buffer_length = 0;

    m_cb.handler(&event,m_cb.p_context);
}

void nrf_drv_uart_tx_abort(void)
{
    CODE_FOR_UARTE
    (
        nrf_uarte_event_clear(NRF_UARTE0, NRF_UARTE_EVENT_TXSTOPPED);
        nrf_uarte_task_trigger(NRF_UARTE0, NRF_UARTE_TASK_STOPTX);
        if (m_cb.handler == NULL)
        {
            while(!nrf_uarte_event_check(NRF_UARTE0, NRF_UARTE_EVENT_TXSTOPPED));
        }
    )
    CODE_FOR_UART
    (
        nrf_uart_task_trigger(NRF_UART0, NRF_UART_TASK_STOPTX);
        if (m_cb.handler)
        {
            tx_done_event(m_cb.tx_counter);
        }
        else
        {
            m_cb.tx_counter       = TX_COUNTER_ABORT_REQ_VALUE;
        }
    )
}

void nrf_drv_uart_rx_abort(void)
{
    CODE_FOR_UARTE
    (
        nrf_uarte_task_trigger(NRF_UARTE0, NRF_UARTE_TASK_STOPRX);
    )
    CODE_FOR_UART
    (
        nrf_uart_int_disable(NRF_UART0, NRF_UART_INT_MASK_RXDRDY | NRF_UART_INT_MASK_ERROR);
        nrf_uart_task_trigger(NRF_UART0, NRF_UART_TASK_STOPRX);
    )
}


#if defined(UART_IN_USE)
__STATIC_INLINE void uart_irq_handler()
{
    if (nrf_uart_int_enable_check(NRF_UART0, NRF_UART_INT_MASK_ERROR) &&
        nrf_uart_event_check(NRF_UART0, NRF_UART_EVENT_ERROR))
    {
        nrf_drv_uart_event_t event;
        nrf_uart_event_clear(NRF_UART0, NRF_UART_EVENT_ERROR);
        nrf_uart_int_disable(NRF_UART0, NRF_UART_INT_MASK_RXDRDY | NRF_UART_INT_MASK_ERROR);
        if (!m_cb.rx_enabled)
        {
            nrf_uart_task_trigger(NRF_UART0, NRF_UART_TASK_STOPRX);
        }
        event.type                   = NRF_DRV_UART_EVT_ERROR;
        event.data.error.error_mask  = nrf_uart_errorsrc_get_and_clear(NRF_UART0);
        event.data.error.rxtx.bytes  = m_cb.rx_buffer_length;
        event.data.error.rxtx.p_data = m_cb.p_rx_buffer;

        //abort transfer
        m_cb.rx_buffer_length = 0;
        m_cb.rx_secondary_buffer_length = 0;

        m_cb.handler(&event,m_cb.p_context);
    }
    else if (nrf_uart_int_enable_check(NRF_UART0, NRF_UART_INT_MASK_RXDRDY) &&
             nrf_uart_event_check(NRF_UART0, NRF_UART_EVENT_RXDRDY))
    {
        rx_byte();
        if (m_cb.rx_buffer_length == m_cb.rx_counter)
        {
            if (m_cb.rx_secondary_buffer_length)
            {
                uint8_t * p_data     = m_cb.p_rx_buffer;
                uint8_t   rx_counter = m_cb.rx_counter;
                
                //Switch to secondary buffer.
                m_cb.rx_buffer_length = m_cb.rx_secondary_buffer_length;
                m_cb.p_rx_buffer = m_cb.p_rx_secondary_buffer;
                m_cb.rx_secondary_buffer_length = 0;
                m_cb.rx_counter = 0;
                rx_done_event(rx_counter, p_data);
            }
            else
            {
                if (!m_cb.rx_enabled)
                {
                    nrf_uart_task_trigger(NRF_UART0, NRF_UART_TASK_STOPRX);
                }
                nrf_uart_int_disable(NRF_UART0, NRF_UART_INT_MASK_RXDRDY | NRF_UART_INT_MASK_ERROR);
                m_cb.rx_buffer_length = 0;
                rx_done_event(m_cb.rx_counter, m_cb.p_rx_buffer);
            }
        }
    }

    if (nrf_uart_event_check(NRF_UART0, NRF_UART_EVENT_TXDRDY))
    {
        if (m_cb.tx_counter < (uint16_t) m_cb.tx_buffer_length)
        {
            tx_byte();
        }
        else
        {
            nrf_uart_event_clear(NRF_UART0, NRF_UART_EVENT_TXDRDY);
            if (m_cb.tx_buffer_length)
            {
                tx_done_event(m_cb.tx_buffer_length);
            }
        }
    }

    if (nrf_uart_event_check(NRF_UART0, NRF_UART_EVENT_RXTO))
    {
        nrf_uart_event_clear(NRF_UART0, NRF_UART_EVENT_RXTO);

        // RXTO event may be triggered as a result of abort call. In th
        if (m_cb.rx_enabled)
        {
            nrf_uart_task_trigger(NRF_UART0, NRF_UART_TASK_STARTRX);
        }
        if (m_cb.rx_buffer_length)
        {
            m_cb.rx_buffer_length = 0;
            rx_done_event(m_cb.rx_counter, m_cb.p_rx_buffer);
        }
    }
}
#endif

#if defined(UARTE_IN_USE)
__STATIC_INLINE void uarte_irq_handler()
{
    if (nrf_uarte_event_check(NRF_UARTE0, NRF_UARTE_EVENT_ERROR))
    {
        nrf_drv_uart_event_t event;

        nrf_uarte_event_clear(NRF_UARTE0, NRF_UARTE_EVENT_ERROR);

        event.type                   = NRF_DRV_UART_EVT_ERROR;
        event.data.error.error_mask  = nrf_uarte_errorsrc_get_and_clear(NRF_UARTE0);
        event.data.error.rxtx.bytes  = nrf_uarte_rx_amount_get(NRF_UARTE0);
        event.data.error.rxtx.p_data = m_cb.p_rx_buffer;

        //abort transfer
        m_cb.rx_buffer_length = 0;
        m_cb.rx_secondary_buffer_length = 0;

        m_cb.handler(&event,m_cb.p_context);
    }
    else if (nrf_uarte_event_check(NRF_UARTE0, NRF_UARTE_EVENT_ENDRX))
    {
        nrf_uarte_event_clear(NRF_UARTE0, NRF_UARTE_EVENT_ENDRX);
        uint8_t amount = nrf_uarte_rx_amount_get(NRF_UARTE0);
        // If the transfer was stopped before completion, amount of transfered bytes
        // will not be equal to the buffer length. Interrupted trunsfer is ignored.
        if (amount == m_cb.rx_buffer_length)
        {
            if (m_cb.rx_secondary_buffer_length)
            {
                uint8_t * p_data = m_cb.p_rx_buffer;
                nrf_uarte_shorts_disable(NRF_UARTE0, NRF_UARTE_SHORT_ENDRX_STARTRX);
                m_cb.rx_buffer_length = m_cb.rx_secondary_buffer_length;
                m_cb.p_rx_buffer = m_cb.p_rx_secondary_buffer;
                m_cb.rx_secondary_buffer_length = 0;
                rx_done_event(amount, p_data);
            }
            else
            {
                m_cb.rx_buffer_length = 0;
                rx_done_event(amount, m_cb.p_rx_buffer);
            }
        }
    }

    if (nrf_uarte_event_check(NRF_UARTE0, NRF_UARTE_EVENT_RXTO))
    {
        nrf_uarte_event_clear(NRF_UARTE0, NRF_UARTE_EVENT_RXTO);
        if (m_cb.rx_buffer_length)
        {
            m_cb.rx_buffer_length = 0;
            rx_done_event(nrf_uarte_rx_amount_get(NRF_UARTE0), m_cb.p_rx_buffer);
        }
    }

    if (nrf_uarte_event_check(NRF_UARTE0, NRF_UARTE_EVENT_ENDTX))
    {
        nrf_uarte_event_clear(NRF_UARTE0, NRF_UARTE_EVENT_ENDTX);
        if (m_cb.tx_buffer_length)
        {
            tx_done_event(nrf_uarte_tx_amount_get(NRF_UARTE0));
        }
    }
}
#endif

void UART0_IRQHandler(void)
{
    CODE_FOR_UARTE
    (
        uarte_irq_handler();
    )
    CODE_FOR_UART
    (
        uart_irq_handler();
    )
}
