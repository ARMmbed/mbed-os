/*
 * Copyright (c) 2013 Nordic Semiconductor ASA
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

#if DEVICE_SERIAL

#include "hal/serial_api.h"

#include "nrf_uarte.h"
#include "nrf_drv_uart.h"
#include "nrf_drv_common.h"
#include "nrf_atfifo.h"
#include "app_util_platform.h"
#include "nrf_rtc.h"
#include "pinmap_ex.h"

#include "platform/mbed_critical.h"

#if UART0_ENABLED == 0
#error UART0 is disabled. DEVICE_SERIAL must also be disabled to continue.
#endif


/***
 *       _____             __ _                       _   _
 *      / ____|           / _(_)                     | | (_)
 *     | |     ___  _ __ | |_ _  __ _ _   _ _ __ __ _| |_ _  ___  _ __
 *     | |    / _ \| '_ \|  _| |/ _` | | | | '__/ _` | __| |/ _ \| '_ \
 *     | |___| (_) | | | | | | | (_| | |_| | | | (_| | |_| | (_) | | | |
 *      \_____\___/|_| |_|_| |_|\__, |\__,_|_|  \__,_|\__|_|\___/|_| |_|
 *                               __/ |
 *                              |___/
 */

/**
 * Idle timeout between characters before DMA buffer is flushed.
 */
#ifndef MBED_CONF_NORDIC_UART_TIMEOUT_US
#define MBED_CONF_NORDIC_UART_TIMEOUT_US 2000
#endif

/**
 * Default FIFO buffer size for UARTE0.
 */
#ifndef MBED_CONF_NORDIC_UART_0_FIFO_SIZE
#define MBED_CONF_NORDIC_UART_0_FIFO_SIZE 32
#endif

/**
 * Default FIFO buffer size for UARTE1.
 */
#ifndef MBED_CONF_NORDIC_UART_1_FIFO_SIZE
#define MBED_CONF_NORDIC_UART_1_FIFO_SIZE 32
#endif

/**
 * Default DMA buffer size. Each instance has two DMA buffers.
 */
#ifndef MBED_CONF_NORDIC_UART_DMA_SIZE
#define MBED_CONF_NORDIC_UART_DMA_SIZE 8
#else
#if MBED_CONF_NORDIC_UART_DMA_SIZE < 5
#error MBED_CONF_NORDIC_UART_DMA_SIZE must be at least 5 bytes
#endif
#if MBED_CONF_NORDIC_UART_DMA_SIZE > 255
#error MBED_CONF_NORDIC_UART_DMA_SIZE must be less than 256 bytes
#endif
#endif

/**
 * Internal short names.
 */
#define IDLE_TIMEOUT_US         MBED_CONF_NORDIC_UART_TIMEOUT_US
#define UART0_FIFO_BUFFER_SIZE  MBED_CONF_NORDIC_UART_0_FIFO_SIZE
#define UART1_FIFO_BUFFER_SIZE  MBED_CONF_NORDIC_UART_1_FIFO_SIZE
#define DMA_BUFFER_SIZE         MBED_CONF_NORDIC_UART_DMA_SIZE
#define NUMBER_OF_BANKS         2

/**
 * Default timer delay for callbacks.
 */
#define CALLBACK_DELAY_US       100

/**
 * Use RTC2 for idle timeouts.
 * Each channel is dedicated to one particular task.
 */
#define UARTE0_RTC_TIMEOUT_CHANNEL  0
#define UARTE1_RTC_TIMEOUT_CHANNEL  1

/**
 * RTC frequency.
 */
#define RTC_FREQUENCY   32768

/**
 * SWI IRQ numbers
 */
#define UARTE0_SWI_TX_0_IRQ     SWI2_EGU2_IRQn
#define UARTE0_SWI_RX_0_IRQ     SWI3_EGU3_IRQn
#define UARTE1_SWI_TX_0_IRQ     SWI4_EGU4_IRQn
#define UARTE1_SWI_RX_0_IRQ     SWI5_EGU5_IRQn

/***
 *      _______                   _       __
 *     |__   __|                 | |     / _|
 *        | |_   _ _ __   ___  __| | ___| |_ ___
 *        | | | | | '_ \ / _ \/ _` |/ _ \  _/ __|
 *        | | |_| | |_) |  __/ (_| |  __/ | \__ \
 *        |_|\__, | .__/ \___|\__,_|\___|_| |___/
 *            __/ | |
 *           |___/|_|
 */

/**
 * Missing event typedefs.
 */
typedef enum
{
    NRF_UARTE_EVENT_RXDRDY    = offsetof(NRF_UARTE_Type, EVENTS_RXDRDY),
    NRF_UARTE_EVENT_TXDRDY    = offsetof(NRF_UARTE_Type, EVENTS_TXDRDY),
} nrf_uarte_event_extra_t;

/**
 * Missing interrupt masks.
 */
typedef enum
{
    NRF_UARTE_INT_RXDRDY_MASK = UARTE_INTENSET_RXDRDY_Msk,
    NRF_UARTE_INT_TXDRDY_MASK = UARTE_INTENSET_TXDRDY_Msk,
} nrf_uarte_int_mask_extra_t;

/**
 * Internal struct for storing each UARTE instance's state:
 *
 *  owner: pointer to serial object currently using instance.
 *  buffer: buffers assigned to EasyDMA.
 *  rxdrdy_counter: count received characters for idle detection.
 *  endrx_counter: count filled DMA buffers for idle detection.
 *  tx_data: 1 byte Tx buffer for blocking putc.
 *  tx_in_progress: mutex for atomic Tx.
 *  rx_in_progress: mutex for atomic Rx when using async API.
 *  tx_asynch: set synch or asynch mode for Tx.
 *  rx_asynch: set synch or asynch mode for Rx.
 *  ticker_is_running: flag for enabling/disabling idle timer.
 *  callback_posted: flag for posting only one callback.
 *  active_bank: flag for buffer swapping.
 *  fifo: pointer to the FIFO buffer.
 */
typedef struct {
    struct serial_s *owner;
    uint8_t buffer[NUMBER_OF_BANKS][DMA_BUFFER_SIZE];
    uint32_t rxdrdy_counter;
    uint32_t endrx_counter;
    uint32_t usage_counter;
    uint8_t tx_data;
    volatile uint8_t tx_in_progress;
    volatile uint8_t rx_in_progress;
    bool tx_asynch;
    bool rx_asynch;
    bool ticker_is_running;
    bool callback_posted;
    uint8_t active_bank;
    nrf_atfifo_t *fifo;
} nordic_uart_state_t;

/**
 * Turn Mbed HAL IRQ flags into maskable bit masks.
 */
typedef enum {
    NORDIC_TX_IRQ = (1 << 0),
    NORDIC_RX_IRQ = (1 << 1),
} nordic_irq_t;


/***
 *       _____ _       _           _  __      __        _       _     _
 *      / ____| |     | |         | | \ \    / /       (_)     | |   | |
 *     | |  __| | ___ | |__   __ _| |  \ \  / /_ _ _ __ _  __ _| |__ | | ___  ___
 *     | | |_ | |/ _ \| '_ \ / _` | |   \ \/ / _` | '__| |/ _` | '_ \| |/ _ \/ __|
 *     | |__| | | (_) | |_) | (_| | |    \  / (_| | |  | | (_| | |_) | |  __/\__ \
 *      \_____|_|\___/|_.__/ \__,_|_|     \/ \__,_|_|  |_|\__,_|_.__/|_|\___||___/
 *
 *
 */

/**
 * UARTE state. One for each instance.
 */
static nordic_uart_state_t nordic_nrf5_uart_state[UART_ENABLED_COUNT] = { 0 };

/**
 * Array with UARTE register pointers for easy access.
 */
static NRF_UARTE_Type *nordic_nrf5_uart_register[UART_ENABLED_COUNT] = {
    NRF_UARTE0,
#if UART1_ENABLED
    NRF_UARTE1,
#endif
};

/**
 * @brief      Create atomic fifo using macro. Macro defines static arrays
 *             for buffer and internal state.
 */
NRF_ATFIFO_DEF(nordic_nrf5_uart_fifo_0, uint8_t, UART0_FIFO_BUFFER_SIZE);

#if UART1_ENABLED
NRF_ATFIFO_DEF(nordic_nrf5_uart_fifo_1, uint8_t, UART1_FIFO_BUFFER_SIZE);
#endif

/**
 * Global variables expected by mbed_retarget.cpp for STDOUT.
 */
int stdio_uart_inited = 0;
serial_t stdio_uart = { 0 };


/***
 *       _____          _                    _______ _
 *      / ____|        | |                  |__   __(_)
 *     | |    _   _ ___| |_ ___  _ __ ___      | |   _ _ __ ___   ___ _ __
 *     | |   | | | / __| __/ _ \| '_ ` _ \     | |  | | '_ ` _ \ / _ \ '__|
 *     | |___| |_| \__ \ || (_) | | | | | |    | |  | | | | | | |  __/ |
 *      \_____\__,_|___/\__\___/|_| |_| |_|    |_|  |_|_| |_| |_|\___|_|
 *
 *
 */

/**
 * @brief      Set timout for a particular channel in RTC2.
 *
 * @param[in]  timeout  The timeout
 * @param[in]  channel  The channel
 */
static void nordic_custom_ticker_set(uint32_t timeout, int channel)
{
    /**
     * Add timeout to current time and set as compare value for channel.
     */
    uint32_t now = nrf_rtc_counter_get(NRF_RTC2);
    uint32_t ticksout = (timeout * RTC_FREQUENCY) / (1000 * 1000);
    nrf_rtc_cc_set(NRF_RTC2, channel, ticksout + now);

    /**
     * Enable interrupt for channel.
     */
    uint32_t mask = nrf_rtc_int_get(NRF_RTC2);
    nrf_rtc_int_enable(NRF_RTC2, mask | RTC_CHANNEL_INT_MASK(channel));
}

/**
 * @brief      Set idle timeout for particular instance.
 *             This function translates instance number to RTC channel.
 *
 * @param[in]  instance  The instance
 */
static void nordic_custom_ticker_set_timeout(int instance)
{
    if (instance == 0) {

        nordic_custom_ticker_set(IDLE_TIMEOUT_US, UARTE0_RTC_TIMEOUT_CHANNEL);
    }
    else if (instance == 1) {

        nordic_custom_ticker_set(IDLE_TIMEOUT_US, UARTE1_RTC_TIMEOUT_CHANNEL);
    }
}

/***
 *      _______ _                       _    _                 _ _
 *     |__   __(_)                     | |  | |               | | |
 *        | |   _ _ __ ___   ___ _ __  | |__| | __ _ _ __   __| | | ___ _ __
 *        | |  | | '_ ` _ \ / _ \ '__| |  __  |/ _` | '_ \ / _` | |/ _ \ '__|
 *        | |  | | | | | | |  __/ |    | |  | | (_| | | | | (_| | |  __/ |
 *        |_|  |_|_| |_| |_|\___|_|    |_|  |_|\__,_|_| |_|\__,_|_|\___|_|
 *
 *
 */

/**
 * @brief      Interrupt handler for idle timeouts.
 *             This function fans out interrupts from ISR and
 *             translates channel to instance.
 *
 * @param[in]  instance  The instance
 */
static void nordic_nrf5_uart_timeout_handler(uint32_t instance)
{
    /**
     * Check if any characters have been received or buffers been flushed
     * since the last idle timeout.
     */
    if ((nordic_nrf5_uart_state[instance].rxdrdy_counter > 0) ||
        (nordic_nrf5_uart_state[instance].endrx_counter > 0)) {

        /* Activity detected, reset timeout. */
        nordic_custom_ticker_set_timeout(instance);

    } else {

        /* No activity detected, no timeout set. */
        nordic_nrf5_uart_state[instance].ticker_is_running = false;

        /**
         * Stop Rx, this triggers a buffer swap and copies data from
         * DMA buffer to FIFO buffer.
         */
        nrf_uarte_task_trigger(nordic_nrf5_uart_register[instance],
                               NRF_UARTE_TASK_STOPRX);
    }

    /* reset activity counters. */
    nordic_nrf5_uart_state[instance].rxdrdy_counter = 0;
    nordic_nrf5_uart_state[instance].endrx_counter = 0;
}

/**
 * @brief      RTC2 ISR. Used for timeouts and scheduled callbacks.
 */
static void nordic_nrf5_rtc2_handler(void)
{
    /* Channel 0 */
    if (nrf_rtc_event_pending(NRF_RTC2, NRF_RTC_EVENT_COMPARE_0)) {

        /* Clear event and disable interrupt for channel. */
        nrf_rtc_event_clear(NRF_RTC2, NRF_RTC_EVENT_COMPARE_0);

        uint32_t mask = nrf_rtc_int_get(NRF_RTC2);
        nrf_rtc_int_enable(NRF_RTC2, mask & ~NRF_RTC_INT_COMPARE0_MASK);

        /* Call timeout handler with instance ID. */
        nordic_nrf5_uart_timeout_handler(0);
    }

#if UART1_ENABLED
    /* Channel 1 */
    if (nrf_rtc_event_pending(NRF_RTC2, NRF_RTC_EVENT_COMPARE_1)) {

        /* Clear event and disable interrupt for channel. */
        nrf_rtc_event_clear(NRF_RTC2, NRF_RTC_EVENT_COMPARE_1);

        uint32_t mask = nrf_rtc_int_get(NRF_RTC2);
        nrf_rtc_int_enable(NRF_RTC2, mask & ~NRF_RTC_INT_COMPARE1_MASK);

        /* Call timeout handler with instance ID. */
        nordic_nrf5_uart_timeout_handler(1);
    }
#endif
}

/***
 *       _____        __ _                            _____       _                             _
 *      / ____|      / _| |                          |_   _|     | |                           | |
 *     | (___   ___ | |_| |___      ____ _ _ __ ___    | |  _ __ | |_ ___ _ __ _ __ _   _ _ __ | |_
 *      \___ \ / _ \|  _| __\ \ /\ / / _` | '__/ _ \   | | | '_ \| __/ _ \ '__| '__| | | | '_ \| __|
 *      ____) | (_) | | | |_ \ V  V / (_| | | |  __/  _| |_| | | | ||  __/ |  | |  | |_| | |_) | |_
 *     |_____/ \___/|_|  \__| \_/\_/ \__,_|_|  \___| |_____|_| |_|\__\___|_|  |_|   \__,_| .__/ \__|
 *                                                                                       | |
 *                                                                                       |_|
 */

/**
 * @brief      SWI interrupt handler for signaling RxIrq handler.
 *
 * @param[in]  instance  The instance
 */
static void nordic_nrf5_uart_callback_handler(uint32_t instance)
{
    /* Flag that no callback is posted. */
    nordic_nrf5_uart_state[instance].callback_posted = false;

    /* Check if callback handler is set and if event mask match. */
    uart_irq_handler callback = (uart_irq_handler) nordic_nrf5_uart_state[instance].owner->handler;
    uint32_t mask = nordic_nrf5_uart_state[instance].owner->mask;

    if (callback && (mask & NORDIC_RX_IRQ)) {

        /* Invoke callback function. */
        uint32_t context = nordic_nrf5_uart_state[instance].owner->context;
        callback(context, RxIrq);
    }
}

static void nordic_nrf5_uart_swi_rx_0(void)
{
    nordic_nrf5_uart_callback_handler(0);
}

static void nordic_nrf5_uart_swi_rx_1(void)
{
    nordic_nrf5_uart_callback_handler(1);
}

/**
 * @brief      SWI interrupt handler for when the Tx buffer has been transmitted.
 *
 * @param[in]  instance  The instance
 */
static void nordic_nrf5_uart_event_handler_endtx(int instance)
{
    /* Disable TXDRDY event again. */
    nordic_nrf5_uart_register[instance]->INTEN &= ~NRF_UARTE_INT_TXDRDY_MASK;

    /* Release mutex. As the owner this call is safe. */
    nordic_nrf5_uart_state[instance].tx_in_progress = 0;

    /* Check if callback handler and Tx event mask is set. */
    uart_irq_handler callback = (uart_irq_handler) nordic_nrf5_uart_state[instance].owner->handler;
    uint32_t mask = nordic_nrf5_uart_state[instance].owner->mask;

    if (callback && (mask & NORDIC_TX_IRQ)) {

        /* Invoke callback function. */
        uint32_t context = nordic_nrf5_uart_state[instance].owner->context;
        callback(context, TxIrq);
    }
}

/**
 * @brief      Asynchronous event handler for when Tx DMA buffer has been sent.
 *
 * @param[in]  instance  The instance
 */
#if DEVICE_SERIAL_ASYNCH
static void nordic_nrf5_uart_event_handler_endtx_asynch(int instance)
{
    /* Disable ENDTX interrupt. */
    nordic_nrf5_uart_register[instance]->INTEN &= ~NRF_UARTE_INT_ENDTX_MASK;

    /* Set Tx done and reset Tx mode to be not asynchronous. */
    nordic_nrf5_uart_state[instance].tx_in_progress = 0;
    nordic_nrf5_uart_state[instance].tx_asynch = false;

    /* Cast handler to callback function pointer. */
    void (*callback)(void) = (void (*)(void)) nordic_nrf5_uart_state[instance].owner->tx_handler;
    uint32_t mask = nordic_nrf5_uart_state[instance].owner->tx_mask;

    /* Signal error if event mask matches and event handler is set. */
    if (callback && (mask & SERIAL_EVENT_TX_COMPLETE)) {

        /* Store event value so it can be read back. */
        nordic_nrf5_uart_state[instance].owner->tx_event = SERIAL_EVENT_TX_COMPLETE;

        /* Signal callback handler. */
        callback();
    }
}
#endif

static void nordic_nrf5_uart_swi_tx_0(void)
{
#if DEVICE_SERIAL_ASYNCH
    if (nordic_nrf5_uart_state[0].tx_asynch) {

        nordic_nrf5_uart_event_handler_endtx_asynch(0);
    } else
#endif
    {
        nordic_nrf5_uart_event_handler_endtx(0);
    }
}

#if UART1_ENABLED
static void nordic_nrf5_uart_swi_tx_1(void)
{
#if DEVICE_SERIAL_ASYNCH
    if (nordic_nrf5_uart_state[1].tx_asynch) {

        nordic_nrf5_uart_event_handler_endtx_asynch(1);
    } else
#endif
    {
        nordic_nrf5_uart_event_handler_endtx(1);
    }
}
#endif

/**
 * @brief      Trigger Tx SWI.
 *
 * @param[in]  instance  The instance.
 */
static void nordic_swi_tx_trigger(int instance)
{
    if (instance == 0) {

        NVIC_SetPendingIRQ(UARTE0_SWI_TX_0_IRQ);
    }
#if UART1_ENABLED
    else if (instance == 1) {

        NVIC_SetPendingIRQ(UARTE1_SWI_TX_0_IRQ);
    }
#endif
}

/**
 * @brief      Trigger Rx SWI.
 *
 * @param[in]  instance  The instance
 */
static void nordic_swi_rx_trigger(int instance)
{
    if (instance == 0) {

        NVIC_SetPendingIRQ(UARTE0_SWI_RX_0_IRQ);
    }
    else if (instance == 1) {

        NVIC_SetPendingIRQ(UARTE1_SWI_RX_0_IRQ);
    }
}

/***
 *      _    _         _____ _______   ______               _     _    _                 _ _
 *     | |  | |  /\   |  __ \__   __| |  ____|             | |   | |  | |               | | |
 *     | |  | | /  \  | |__) | | |    | |____   _____ _ __ | |_  | |__| | __ _ _ __   __| | | ___ _ __
 *     | |  | |/ /\ \ |  _  /  | |    |  __\ \ / / _ \ '_ \| __| |  __  |/ _` | '_ \ / _` | |/ _ \ '__|
 *     | |__| / ____ \| | \ \  | |    | |___\ V /  __/ | | | |_  | |  | | (_| | | | | (_| | |  __/ |
 *      \____/_/    \_\_|  \_\ |_|    |______\_/ \___|_| |_|\__| |_|  |_|\__,_|_| |_|\__,_|_|\___|_|
 *
 *
 */

/**
 * @brief      Event handler for when Rx buffer is full or buffer swap has been
 *             triggered by idle task.
 *
 *             Copy data from DMA buffer to FIFO buffer.
 *             Post callback if not already posted.
 *
 * @param[in]  instance  The instance
 */
static void nordic_nrf5_uart_event_handler_endrx(int instance)
{
    /* Increment idle counter. */
    core_util_atomic_incr_u32(&nordic_nrf5_uart_state[instance].endrx_counter, 1);

    /* Read out active bank flag and swap DMA buffers. */
    uint8_t active_bank = nordic_nrf5_uart_state[instance].active_bank;
    nordic_nrf5_uart_state[instance].active_bank = active_bank ^ 0x01;

    /* Get number of bytes in DMA buffer. */
    uint32_t available = nrf_uarte_rx_amount_get(nordic_nrf5_uart_register[instance]);

    if (available > 0) {

        /* Copy data from DMA buffer to FIFO buffer. */
        for (size_t index = 0; index < available; index++) {

            /* Atomic FIFO can be used safely without disabling interrutps. */
            nrf_atfifo_item_put_t fifo_context;

            /* Get pointer to available space. */
            uint8_t *byte = (uint8_t *) nrf_atfifo_item_alloc(nordic_nrf5_uart_state[instance].fifo, &fifo_context);

            if (byte != NULL) {

                /* Copy 1 byte from DMA buffer and commit to FIFO buffer. */
                *byte = nordic_nrf5_uart_state[instance].buffer[active_bank][index];
                nrf_atfifo_item_put(nordic_nrf5_uart_state[instance].fifo, &fifo_context);

            } else {

                /* Buffer overflow. */
                break;
            }
        }

        /* Signal callback through lower priority SWI if not already posted. */
        if (nordic_nrf5_uart_state[instance].callback_posted == false) {

            nordic_nrf5_uart_state[instance].callback_posted = true;
            nordic_swi_rx_trigger(instance);
        }
    }
}

/**
 * @brief      Event handler for when DMA has been armed with Rx buffer.
 *
 *             Arm Rx buffer with second buffer for optimal reception.
 *
 * @param[in]  instance  The instance
 */
static void nordic_nrf5_uart_event_handler_rxstarted(int instance)
{
    uint8_t next_bank = nordic_nrf5_uart_state[instance].active_bank ^ 0x01;

    nrf_uarte_rx_buffer_set(nordic_nrf5_uart_register[instance], nordic_nrf5_uart_state[instance].buffer[next_bank], DMA_BUFFER_SIZE);
}

/**
 * @brief      Event handler for when a character has been received in DMA buffer.
 *
 *             Increment idle counter and set idle timeout if not already set.
 *
 * @param[in]  instance  The instance
 */
static void nordic_nrf5_uart_event_handler_rxdrdy(int instance)
{
    /* Increment idle counter. */
    core_util_atomic_incr_u32(&nordic_nrf5_uart_state[instance].rxdrdy_counter, 1);

    /* Set idle timeout if not already set. */
    if (nordic_nrf5_uart_state[instance].ticker_is_running == false) {

        nordic_nrf5_uart_state[instance].ticker_is_running = true;

        nordic_custom_ticker_set_timeout(instance);
    }
}

#if DEVICE_SERIAL_ASYNCH
/**
 * @brief      Asynchronous event handler for when Rx DMA buffer is full.
 *
 * @param[in]  instance  The instance
 */
static void nordic_nrf5_uart_event_handler_endrx_asynch(int instance)
{
    /* Set Rx done and reset Rx mode to be not asynchronous. */
    nordic_nrf5_uart_state[instance].rx_in_progress = 0;
    nordic_nrf5_uart_state[instance].rx_asynch = false;

    /* Cast handler to callback function pointer. */
    void (*callback)(void) = (void (*)(void)) nordic_nrf5_uart_state[instance].owner->rx_handler;
    uint32_t mask = nordic_nrf5_uart_state[instance].owner->rx_mask;

    /* Signal error if event mask matches and event handler is set. */
    if (callback && (mask & SERIAL_EVENT_RX_COMPLETE)) {

        /* Store event value so it can be read back. */
        nordic_nrf5_uart_state[instance].owner->rx_event = SERIAL_EVENT_RX_COMPLETE;

        /* Signal callback handler. */
        callback();
    }
}
#endif

/**
 * @brief      UARTE event handler.
 *
 *             Collect signals from UARTE0 and UARTE1 ISR and translate to instance.
 *
 * @param[in]  instance  The instance
 */
static void nordic_nrf5_uart_event_handler(int instance)
{
    /* DMA buffer is full or has been swapped out by idle timeout. */
    if (nrf_uarte_event_check(nordic_nrf5_uart_register[instance], NRF_UARTE_EVENT_ENDRX))
    {
        nrf_uarte_event_clear(nordic_nrf5_uart_register[instance], NRF_UARTE_EVENT_ENDRX);

#if DEVICE_SERIAL_ASYNCH
        /* Call appropriate event handler based on current mode. */
        if (nordic_nrf5_uart_state[instance].rx_asynch) {

            nordic_nrf5_uart_event_handler_endrx_asynch(instance);
        } else
#endif
        {
            nordic_nrf5_uart_event_handler_endrx(instance);
        }
    }

    /* Rx DMA buffer has been armed. */
    if (nrf_uarte_event_check(nordic_nrf5_uart_register[instance], NRF_UARTE_EVENT_RXSTARTED))
    {
        nrf_uarte_event_clear(nordic_nrf5_uart_register[instance], NRF_UARTE_EVENT_RXSTARTED);

        nordic_nrf5_uart_event_handler_rxstarted(instance);
    }

    /* Single character has been put in DMA buffer. */
    if (nrf_uarte_event_check(nordic_nrf5_uart_register[instance], NRF_UARTE_EVENT_RXDRDY))
    {
        nrf_uarte_event_clear(nordic_nrf5_uart_register[instance], NRF_UARTE_EVENT_RXDRDY);

        nordic_nrf5_uart_event_handler_rxdrdy(instance);
    }

    /* Tx single character has been sent. */
    if (nrf_uarte_event_check(nordic_nrf5_uart_register[instance], NRF_UARTE_EVENT_TXDRDY)) {

        nrf_uarte_event_clear(nordic_nrf5_uart_register[instance], NRF_UARTE_EVENT_TXDRDY);

        /* In non-async transfers this will generate an interrupt if callback and mask is set. */
        if (!nordic_nrf5_uart_state[instance].tx_asynch) {

            /* Use SWI to de-prioritize callback. */
            nordic_swi_tx_trigger(instance);
        }
    }

#if DEVICE_SERIAL_ASYNCH
    /* Tx DMA buffer has been sent. */
    if (nrf_uarte_event_check(nordic_nrf5_uart_register[instance], NRF_UARTE_EVENT_ENDTX))
    {
        nrf_uarte_event_clear(nordic_nrf5_uart_register[instance], NRF_UARTE_EVENT_ENDTX);

        /* Call async event handler in async mode. */
        if (nordic_nrf5_uart_state[instance].tx_asynch) {

            /* Use SWI to de-prioritize callback. */
            nordic_swi_tx_trigger(instance);
        }
    }
#endif
}

/**
 * @brief      UARTE0 ISR.
 */
static void nordic_nrf5_uart0_handler(void)
{
    /* Call event handler with instance ID. */
    nordic_nrf5_uart_event_handler(0);
}

#if UART1_ENABLED
/**
 * @brief      UARTE1 ISR.
 */
static void nordic_nrf5_uart1_handler(void)
{
    /* Call event handler with instance ID. */
    nordic_nrf5_uart_event_handler(1);
}
#endif


/***
 *       _____             __ _                       _   _
 *      / ____|           / _(_)                     | | (_)
 *     | |     ___  _ __ | |_ _  __ _ _   _ _ __ __ _| |_ _  ___  _ __
 *     | |    / _ \| '_ \|  _| |/ _` | | | | '__/ _` | __| |/ _ \| '_ \
 *     | |___| (_) | | | | | | | (_| | |_| | | | (_| | |_| | (_) | | | |
 *      \_____\___/|_| |_|_| |_|\__, |\__,_|_|  \__,_|\__|_|\___/|_| |_|
 *                               __/ |
 *                              |___/
 */

/**
 * @brief      Enable UARTE interrupts.
 *
 *             Translates instance to UARTE register.
 *             Set IRQ priority to highest to avoid Rx overflow.
 *
 * @param[in]  instance  The instance
 */
static void nordic_nrf5_uart_irq_enable(int instance)
{
    if (instance == 0) {

        nrf_drv_common_irq_enable(UARTE0_UART0_IRQn, APP_IRQ_PRIORITY_HIGHEST);
    }

#if UART1_ENABLED
    else if (instance == 1) {

        nrf_drv_common_irq_enable(UARTE1_IRQn, APP_IRQ_PRIORITY_HIGHEST);
    }
#endif
}

/**
 * @brief      Configure UARTE based on serial object settings.
 *
 *             Common for both Rx and Tx.
 *
 * @param      obj   The object
 */
static void nordic_nrf5_uart_configure_object(serial_t *obj)
{
    MBED_ASSERT(obj);

#if DEVICE_SERIAL_ASYNCH
    struct serial_s *uart_object = &obj->serial;
#else
    struct serial_s *uart_object = obj;
#endif

    /* Configure Tx and Rx pins. */
    nrf_gpio_pin_set(uart_object->tx);
    nrf_gpio_cfg_output(uart_object->tx);
    nrf_gpio_cfg_input(uart_object->rx, NRF_GPIO_PIN_NOPULL);

    nrf_uarte_txrx_pins_set(nordic_nrf5_uart_register[uart_object->instance],
                            uart_object->tx,
                            uart_object->rx);

    /* Set hardware flow control pins. */
    if (uart_object->hwfc == NRF_UART_HWFC_ENABLED) {

        /* Check if pin is set before configuring it. */
        if (uart_object->rts != NRF_UART_PSEL_DISCONNECTED) {

            nrf_gpio_cfg_output(uart_object->rts);
        }

        /* Check if pin is set before configuring it. */
        if (uart_object->cts != NRF_UART_PSEL_DISCONNECTED) {

            nrf_gpio_cfg_input(uart_object->cts, NRF_GPIO_PIN_NOPULL);
        }

        nrf_uarte_hwfc_pins_set(nordic_nrf5_uart_register[uart_object->instance],
                                uart_object->rts,
                                uart_object->cts);
    }

    /* Enable flow control and parity. */
    nrf_uarte_configure(nordic_nrf5_uart_register[uart_object->instance],
                        uart_object->parity,
                        uart_object->hwfc);

    /* Set baudrate. */
    nrf_uarte_baudrate_set(nordic_nrf5_uart_register[uart_object->instance],
                           uart_object->baudrate);
}

/**
 * @brief      Setup non-asynchronous reception.
 *
 * @param[in]  instance  The instance
 */
static void nordic_nrf5_uart_configure_rx(int instance)
{
    /* Disable interrupts during confiration. */
    nordic_nrf5_uart_register[instance]->INTEN &= ~(NRF_UARTE_INT_RXSTARTED_MASK |
                                                    NRF_UARTE_INT_ENDRX_MASK     |
                                                    NRF_UARTE_INT_RXDRDY_MASK);

    /* Clear FIFO buffer. */
    nrf_atfifo_clear(nordic_nrf5_uart_state[instance].fifo);

    /* Clear Rx related events. */
    nrf_uarte_event_clear(nordic_nrf5_uart_register[instance], NRF_UARTE_EVENT_RXSTARTED);
    nrf_uarte_event_clear(nordic_nrf5_uart_register[instance], NRF_UARTE_EVENT_ENDRX);
    nrf_uarte_event_clear(nordic_nrf5_uart_register[instance], NRF_UARTE_EVENT_RXDRDY);

    /* Enable shortcut between buffer full and begin reception on next buffer armed. */
    nrf_uarte_shorts_enable(nordic_nrf5_uart_register[instance], NRF_UARTE_SHORT_ENDRX_STARTRX);

    /* Reset bank flag. */
    nordic_nrf5_uart_state[instance].active_bank = 0;

    /* Arm first DMA buffer. */
    nrf_uarte_rx_buffer_set(nordic_nrf5_uart_register[instance],
                            nordic_nrf5_uart_state[instance].buffer[0],
                            DMA_BUFFER_SIZE);

    /* Set non-asynchronous mode. */
    nordic_nrf5_uart_state[instance].rx_asynch = false;

    /* Enable interrupts again. */
    nordic_nrf5_uart_register[instance]->INTEN |= (NRF_UARTE_INT_RXSTARTED_MASK |
                                                   NRF_UARTE_INT_ENDRX_MASK     |
                                                   NRF_UARTE_INT_RXDRDY_MASK);
}

#if DEVICE_SERIAL_ASYNCH
/**
 * @brief      Setup asynchronous reception.
 *
 * @param[in]  instance  The instance
 */
static void nordic_nrf5_uart_configure_rx_asynch(int instance)
{
    /* Disable Rx related interrupts. */
    nordic_nrf5_uart_register[instance]->INTEN &= ~(NRF_UARTE_INT_RXSTARTED_MASK |
                                                    NRF_UARTE_INT_ENDRX_MASK     |
                                                    NRF_UARTE_INT_RXDRDY_MASK);

    /* Clear Rx related events. */
    nrf_uarte_event_clear(nordic_nrf5_uart_register[instance], NRF_UARTE_EVENT_RXSTARTED);
    nrf_uarte_event_clear(nordic_nrf5_uart_register[instance], NRF_UARTE_EVENT_ENDRX);
    nrf_uarte_event_clear(nordic_nrf5_uart_register[instance], NRF_UARTE_EVENT_RXDRDY);

    /* Disable shortcut. Next Rx buffer must be manually started. */
    nrf_uarte_shorts_disable(nordic_nrf5_uart_register[instance], NRF_UARTE_SHORT_ENDRX_STARTRX);

    /* Set asynchronous mode. */
    nordic_nrf5_uart_state[instance].rx_asynch = true;

    /* Enable Rx interrupt. */
    nordic_nrf5_uart_register[instance]->INTEN |= NRF_UARTE_INT_ENDRX_MASK;
}
#endif

/**
 * @brief      Main configuration function.
 *
 * @param      obj   The serial object
 */
static void nordic_nrf5_serial_configure(serial_t *obj)
{
    MBED_ASSERT(obj);

#if DEVICE_SERIAL_ASYNCH
    struct serial_s *uart_object = &obj->serial;
#else
    struct serial_s *uart_object = obj;
#endif

    /* Get object instance. */
    int instance = uart_object->instance;

    /* Only configure if instance owner has changed or an update is forced. */
    if ((uart_object != nordic_nrf5_uart_state[instance].owner) || (uart_object->update)) {

        /* Configure common setting. */
        nordic_nrf5_uart_configure_object(obj);

        /* Set new owner. */
        nordic_nrf5_uart_state[instance].owner = uart_object;
        uart_object->update = false;

#if DEVICE_SERIAL_ASYNCH
        /* Set asynchronous mode. */
        if (uart_object->rx_asynch == true) {

            nordic_nrf5_uart_configure_rx_asynch(instance);
        } else
#endif
        {
            /* Set non-asynchronous mode. */
            nordic_nrf5_uart_configure_rx(instance);
            nrf_uarte_task_trigger(nordic_nrf5_uart_register[instance],
                                   NRF_UARTE_TASK_STARTRX);
        }
    }
#if DEVICE_SERIAL_ASYNCH
    /* Owner hasn't changed but mode has. Reconfigure. */
    else if ((uart_object->rx_asynch == false) && (nordic_nrf5_uart_state[instance].rx_asynch == true)) {

        nordic_nrf5_uart_configure_rx(instance);
        nrf_uarte_task_trigger(nordic_nrf5_uart_register[instance],
                               NRF_UARTE_TASK_STARTRX);

    /* Owner hasn't changed but mode has. Reconfigure. */
    } else if ((uart_object->rx_asynch == true) && (nordic_nrf5_uart_state[instance].rx_asynch == false)) {

        nordic_nrf5_uart_configure_rx_asynch(instance);
    }
#endif
}

/***
 *      __  __ _              _   _    _          _                 _____ _____
 *     |  \/  | |            | | | |  | |   /\   | |          /\   |  __ \_   _|
 *     | \  / | |__   ___  __| | | |__| |  /  \  | |         /  \  | |__) || |
 *     | |\/| | '_ \ / _ \/ _` | |  __  | / /\ \ | |        / /\ \ |  ___/ | |
 *     | |  | | |_) |  __/ (_| | | |  | |/ ____ \| |____   / ____ \| |    _| |_
 *     |_|  |_|_.__/ \___|\__,_| |_|  |_/_/    \_\______| /_/    \_\_|   |_____|
 *
 *
 */

/** Initialize the serial peripheral. It sets the default parameters for serial
 *  peripheral, and configures its specifieds pins.
 *
 * Param obj The serial object
 * Param tx  The TX pin name
 * Param rx  The RX pin name
 */
void serial_init(serial_t *obj, PinName tx, PinName rx)
{
    MBED_ASSERT(obj);

#if DEVICE_SERIAL_ASYNCH
    struct serial_s *uart_object = &obj->serial;
#else
    struct serial_s *uart_object = obj;
#endif

    /* Only initialize on first call. */
    static bool first_init = true;
    if (first_init) {
        first_init = false;

        /* Register RTC2 ISR. */
        NVIC_SetVector(RTC2_IRQn, (uint32_t) nordic_nrf5_rtc2_handler);

        /* Clear RTC2 channel events. */
        nrf_rtc_event_clear(NRF_RTC2, NRF_RTC_EVENT_COMPARE_0);
        nrf_rtc_event_clear(NRF_RTC2, NRF_RTC_EVENT_COMPARE_1);

        /* Enable interrupts for all four RTC2 channels. */
        nrf_rtc_event_enable(NRF_RTC2,
                             NRF_RTC_INT_COMPARE0_MASK |
                             NRF_RTC_INT_COMPARE1_MASK);

        /* Enable RTC2 IRQ. Priority is set to lowest so that the UARTE ISR can interrupt it. */
        nrf_drv_common_irq_enable(RTC2_IRQn, APP_IRQ_PRIORITY_LOWEST);

        /* Start RTC2. According to the datasheet the added power consumption is neglible so
         * the RTC2 will run forever.
         */
        nrf_rtc_task_trigger(NRF_RTC2, NRF_RTC_TASK_START);

        /* Enable interrupts for SWI. */
        NVIC_SetVector(UARTE0_SWI_TX_0_IRQ, (uint32_t) nordic_nrf5_uart_swi_tx_0);
        NVIC_SetVector(UARTE0_SWI_RX_0_IRQ, (uint32_t) nordic_nrf5_uart_swi_rx_0);
        nrf_drv_common_irq_enable(UARTE0_SWI_TX_0_IRQ, APP_IRQ_PRIORITY_LOWEST);
        nrf_drv_common_irq_enable(UARTE0_SWI_RX_0_IRQ, APP_IRQ_PRIORITY_LOWEST);

#if UART1_ENABLED
        NVIC_SetVector(UARTE1_SWI_TX_0_IRQ, (uint32_t) nordic_nrf5_uart_swi_tx_1);
        NVIC_SetVector(UARTE1_SWI_RX_0_IRQ, (uint32_t) nordic_nrf5_uart_swi_rx_1);
        nrf_drv_common_irq_enable(UARTE1_SWI_TX_0_IRQ, APP_IRQ_PRIORITY_LOWEST);
        nrf_drv_common_irq_enable(UARTE1_SWI_RX_0_IRQ, APP_IRQ_PRIORITY_LOWEST);
#endif

        /* Initialize FIFO buffer for UARTE0. */
        NRF_ATFIFO_INIT(nordic_nrf5_uart_fifo_0);
        nordic_nrf5_uart_state[0].fifo = nordic_nrf5_uart_fifo_0;

        /* Initialize owner to NULL. */
        nordic_nrf5_uart_state[0].owner = NULL;

        /* Enable interrupts for UARTE0. */
        NVIC_SetVector(UARTE0_UART0_IRQn, (uint32_t) nordic_nrf5_uart0_handler);
        nordic_nrf5_uart_irq_enable(0);

#if UART1_ENABLED
        /* Initialize FIFO buffer for UARTE1. */
        NRF_ATFIFO_INIT(nordic_nrf5_uart_fifo_1);
        nordic_nrf5_uart_state[1].fifo = nordic_nrf5_uart_fifo_1;

        /* Initialize owner to NULL. */
        nordic_nrf5_uart_state[1].owner = NULL;

        /* Enable interrupts for UARTE1. */
        NVIC_SetVector(UARTE1_IRQn, (uint32_t) nordic_nrf5_uart1_handler);
        nordic_nrf5_uart_irq_enable(1);
#endif
    }

    /* Get instance ID based on provided pins. */
    int instance = pin_instance_uart(tx, rx);

    uart_object->instance = instance;

    /* Increment usage counter for this instance. */
    nordic_nrf5_uart_state[instance].usage_counter++;

    /* Enable instance on first usage. */
    if (nordic_nrf5_uart_state[instance].usage_counter == 1) {

        nrf_uarte_enable(nordic_nrf5_uart_register[instance]);
    }

    /* Store pins in serial object. */
    if (tx == NC) {

        uart_object->tx = NRF_UART_PSEL_DISCONNECTED;
    } else {

        uart_object->tx = tx;
    }

    if (rx == NC) {

        uart_object->rx = NRF_UART_PSEL_DISCONNECTED;
    } else {

        uart_object->rx = rx;
    }

    /* Set default parity, baud rate, and callback handler. */
    uart_object->parity = NRF_UART_PARITY_EXCLUDED;
    uart_object->baudrate = NRF_UART_BAUDRATE_9600;
    uart_object->cts = NRF_UART_PSEL_DISCONNECTED;
    uart_object->rts = NRF_UART_PSEL_DISCONNECTED;
    uart_object->hwfc = NRF_UART_HWFC_DISABLED;
    uart_object->handler = 0;

    /* The STDIO object is stored in this file. Set the flag once initialized. */
    if (obj == &stdio_uart) {
        stdio_uart_inited = 1;
    }

    /* Take ownership and configure UART. */
    uart_object->update = true;
    nordic_nrf5_serial_configure(obj);
}

/** Release the serial peripheral, not currently invoked. It requires further
 *  resource management.
 *
 * Param obj The serial object
 */
void serial_free(serial_t *obj)
{
    MBED_ASSERT(obj);

#if DEVICE_SERIAL_ASYNCH
    struct serial_s *uart_object = &obj->serial;
#else
    struct serial_s *uart_object = obj;
#endif

    int instance = uart_object->instance;

    /* Only consider disabling UARTE if number of users is not zero. */
    if (nordic_nrf5_uart_state[instance].usage_counter > 0) {

        /* Decrement usage counter for this instance. */
        nordic_nrf5_uart_state[instance].usage_counter--;

        /* Disable instance when not in use. */
        if (nordic_nrf5_uart_state[instance].usage_counter == 0) {

            nrf_uarte_disable(nordic_nrf5_uart_register[instance]);
        }
    }
}

/** Configure the baud rate
 *
 * Param obj      The serial object
 * Param baudrate The baud rate to be configured
 */
void serial_baud(serial_t *obj, int baudrate)
{
    MBED_ASSERT(obj);

#if DEVICE_SERIAL_ASYNCH
    struct serial_s *uart_object = &obj->serial;
#else
    struct serial_s *uart_object = obj;
#endif

    nrf_uart_baudrate_t new_rate = NRF_UART_BAUDRATE_9600;

    /* Round down to nearest supported baud rate. */
    if (baudrate < 2400) {
        new_rate = NRF_UARTE_BAUDRATE_1200;
    } else if (baudrate < 4800) {
        new_rate = NRF_UARTE_BAUDRATE_2400;
    } else if (baudrate < 9600) {
        new_rate = NRF_UARTE_BAUDRATE_4800;
    } else if (baudrate < 14400) {
        new_rate = NRF_UARTE_BAUDRATE_9600;
    } else if (baudrate < 19200) {
        new_rate = NRF_UARTE_BAUDRATE_14400;
    } else if (baudrate < 28800) {
        new_rate = NRF_UARTE_BAUDRATE_19200;
    } else if (baudrate < 38400) {
        new_rate = NRF_UARTE_BAUDRATE_28800;
    } else if (baudrate < 57600) {
        new_rate = NRF_UARTE_BAUDRATE_38400;
    } else if (baudrate < 76800) {
        new_rate = NRF_UARTE_BAUDRATE_57600;
    } else if (baudrate < 115200) {
        new_rate = NRF_UARTE_BAUDRATE_76800;
    } else if (baudrate < 230400) {
        new_rate = NRF_UARTE_BAUDRATE_115200;
    } else if (baudrate < 250000) {
        new_rate = NRF_UARTE_BAUDRATE_230400;
    } else if (baudrate < 460800) {
        new_rate = NRF_UARTE_BAUDRATE_250000;
    } else if (baudrate < 921600) {
        new_rate = NRF_UARTE_BAUDRATE_460800;
    } else if (baudrate < 1000000) {
        new_rate = NRF_UARTE_BAUDRATE_921600;
    } else {
        new_rate = NRF_UARTE_BAUDRATE_1000000;
    }

    /* Force reconfiguration next time serial object is owner if baud rate has changed. */
    if (uart_object->baudrate != new_rate) {

        uart_object->baudrate = new_rate;
        uart_object->update = true;
    }
}

/** Configure the format. Set the number of bits, parity and the number of stop bits
 *
 * Param obj       The serial object
 * Param data_bits The number of data bits
 * Param parity    The parity
 * Param stop_bits The number of stop bits
 */
void serial_format(serial_t *obj, int data_bits, SerialParity parity, int stop_bits)
{
    MBED_ASSERT(obj);

    /**
     * Only 8-bit mode, None/Even parity, and 1 stop bit supported by hardware.
     */
    MBED_ASSERT(data_bits == 8);
    MBED_ASSERT((parity == ParityNone) || (parity == ParityEven));
    MBED_ASSERT(stop_bits == 1);

#if DEVICE_SERIAL_ASYNCH
    struct serial_s *uart_object = &obj->serial;
#else
    struct serial_s *uart_object = obj;
#endif

    /**
     * Only force change if parity has changed.
     */
    if ((uart_object->parity != NRF_UART_PARITY_EXCLUDED) && (parity == ParityNone)) {

        uart_object->parity = NRF_UART_PARITY_EXCLUDED;
        uart_object->update = true;

    } else if ((uart_object->parity != NRF_UART_PARITY_INCLUDED) && (parity == ParityEven)) {

        uart_object->parity = NRF_UART_PARITY_INCLUDED;
        uart_object->update = true;
    }
}

/** Configure the serial for the flow control. It sets flow control in the hardware
 *  if a serial peripheral supports it, otherwise software emulation is used.
 *
 * Param obj    The serial object
 * Param type   The type of the flow control. Look at the available FlowControl types.
 * Param rxflow The TX pin name
 * Param txflow The RX pin name
 */
void serial_set_flow_control(serial_t *obj, FlowControl type, PinName rxflow, PinName txflow)
{
    MBED_ASSERT(obj);

#if DEVICE_SERIAL_ASYNCH
    struct serial_s *uart_object = &obj->serial;
#else
    struct serial_s *uart_object = obj;
#endif

    /**
     * Convert Mbed pin names to Nordic pin names.
     */
    uart_object->cts = ((txflow == NC) || (type == FlowControlRTS)) ? NRF_UART_PSEL_DISCONNECTED : (uint32_t) txflow;
    uart_object->rts = ((rxflow == NC) || (type == FlowControlCTS)) ? NRF_UART_PSEL_DISCONNECTED : (uint32_t) rxflow;
    uart_object->hwfc = (type == FlowControlNone) ? NRF_UART_HWFC_DISABLED : NRF_UART_HWFC_ENABLED;

    /* Force reconfiguration next time object is owner. */
    uart_object->update = true;
}

/** Clear the serial peripheral
 *
 * Param obj The serial object
 */
void serial_clear(serial_t *obj)
{
    MBED_ASSERT(obj);

#if DEVICE_SERIAL_ASYNCH
    struct serial_s *uart_object = &obj->serial;
#else
    struct serial_s *uart_object = obj;
#endif

    /**
     * Reconfigure UART.
     */
    uart_object->update = true;
    nordic_nrf5_uart_configure_object(obj);
}

/** Set the break
 *
 * Param obj The serial object
 */
void serial_break_set(serial_t *obj)
{
    MBED_ASSERT(obj);

#if DEVICE_SERIAL_ASYNCH
    struct serial_s *uart_object = &obj->serial;
#else
    struct serial_s *uart_object = obj;
#endif

    /* Set Tx pin low. */
    nrf_gpio_pin_clear(uart_object->tx);
}

/** Clear the break
 *
 * Param obj The serial object
 */
void serial_break_clear(serial_t *obj)
{
    MBED_ASSERT(obj);

#if DEVICE_SERIAL_ASYNCH
    struct serial_s *uart_object = &obj->serial;
#else
    struct serial_s *uart_object = obj;
#endif

    /* Set Tx pin high (default). */
    nrf_gpio_pin_set(uart_object->tx);
}

/** Configure the TX pin for UART function.
 *
 * Param tx The pin name used for TX
 */
void serial_pinout_tx(PinName tx)
{
    /**
     * Legacy API. Not used by Mbed.
     */
    MBED_ASSERT(0);
}

/***
 *       _____ _                 _                 _____ _____
 *      / ____(_)               | |          /\   |  __ \_   _|
 *     | (___  _ _ __ ___  _ __ | | ___     /  \  | |__) || |
 *      \___ \| | '_ ` _ \| '_ \| |/ _ \   / /\ \ |  ___/ | |
 *      ____) | | | | | | | |_) | |  __/  / ____ \| |    _| |_
 *     |_____/|_|_| |_| |_| .__/|_|\___| /_/    \_\_|   |_____|
 *                        | |
 *                        |_|
 */

/** The serial interrupt handler registration
 *
 * Param obj     The serial object
 * Param handler The interrupt handler which will be invoked when the interrupt fires
 * Param id      The SerialBase object
 */
void serial_irq_handler(serial_t *obj, uart_irq_handler handler, uint32_t id)
{
    MBED_ASSERT(obj);

#if DEVICE_SERIAL_ASYNCH
    struct serial_s *uart_object = &obj->serial;
#else
    struct serial_s *uart_object = obj;
#endif

    /* Store handler and ID in serial object. */
    uart_object->handler = (uint32_t) handler;
    uart_object->context = id;
}

/** Configure serial interrupt. This function is used for word-approach
 *
 * Param obj    The serial object
 * Param irq    The serial IRQ type (RX or TX)
 * Param enable Set to non-zero to enable events, or zero to disable them
 */
void serial_irq_set(serial_t *obj, SerialIrq irq, uint32_t enable)
{
    MBED_ASSERT(obj);

#if DEVICE_SERIAL_ASYNCH
    struct serial_s *uart_object = &obj->serial;
#else
    struct serial_s *uart_object = obj;
#endif

    /* Convert Mbed type to Nordic IRQ mask. */
    uint32_t type = (irq == TxIrq) ? NORDIC_TX_IRQ : NORDIC_RX_IRQ;

    /* Enable/disable interrupt bit mask. */
    if (enable) {

        uart_object->mask |= type;

    } else {

        uart_object->mask &= ~type;
    }

    /* Enable TXDRDY event. */
    if ((type == NORDIC_TX_IRQ) && enable) {

        /* Clear Tx ready event and enable Tx ready interrupts. */
        nrf_uarte_event_clear(nordic_nrf5_uart_register[uart_object->instance], NRF_UARTE_EVENT_TXDRDY);
        nordic_nrf5_uart_register[uart_object->instance]->INTEN |= NRF_UARTE_INT_TXDRDY_MASK;

    /* Disable TXDRDY event. */
    } else if ((type == NORDIC_TX_IRQ) && !enable) {

        /* Disable Tx ready interrupts and clear Tx ready event. */
        nordic_nrf5_uart_register[uart_object->instance]->INTEN &= ~NRF_UARTE_INT_TXDRDY_MASK;
        nrf_uarte_event_clear(nordic_nrf5_uart_register[uart_object->instance], NRF_UARTE_EVENT_TXDRDY);
    }
}

/** Get character. This is a blocking call, waiting for a character
 *
 * Param obj The serial object
 */
int serial_getc(serial_t *obj)
{
    MBED_ASSERT(obj);

#if DEVICE_SERIAL_ASYNCH
    struct serial_s *uart_object = &obj->serial;
    uart_object->rx_asynch = false;
#else
    struct serial_s *uart_object = obj;
#endif

    int instance = uart_object->instance;

    /* Take ownership and configure UART if necessary. */
    nordic_nrf5_serial_configure(obj);

    /**
     * Use head and tail pointer in FIFO to determine whether there is data available.
     */
    nrf_atfifo_t *fifo = nordic_nrf5_uart_state[instance].fifo;

    volatile uint16_t *head = &fifo->head.pos.rd;
    volatile uint16_t *tail = &fifo->tail.pos.rd;

    /* serial_getc is a blocking call. */
    while (*head == *tail);

    /* Get 1 byte from FIFO buffer. The buffer is atomic
     * and doesn't need to be protected in a critical section.
     */
    nrf_atfifo_item_get_t context;
    uint8_t *byte = (uint8_t *) nrf_atfifo_item_get(fifo, &context);
    nrf_atfifo_item_free(fifo, &context);

    return *byte;
}

/** Send a character. This is a blocking call, waiting for a peripheral to be available
 *  for writing
 *
 * Param obj The serial object
 * Param c   The character to be sent
 */
void serial_putc(serial_t *obj, int character)
{
    MBED_ASSERT(obj);

#if DEVICE_SERIAL_ASYNCH
    struct serial_s *uart_object = &obj->serial;
#else
    struct serial_s *uart_object = obj;
#endif

    int instance = uart_object->instance;

    /**
     * tx_in_progress acts like a mutex to ensure only one transmission can be active at a time.
     * The flag is modified using the atomic compare-and-set function.
     */
    bool mutex = false;

    do {
        uint8_t expected = 0;
        uint8_t desired = 1;

        mutex = core_util_atomic_cas_u8((uint8_t *) &nordic_nrf5_uart_state[instance].tx_in_progress, &expected, desired);
    } while (mutex == false);

    /* Take ownership and configure UART if necessary. */
    nordic_nrf5_serial_configure(obj);

    /**
     * The UARTE module can generate two different Tx events: TXDRDY when each character has
     * been transmitted and ENDTX when the entire buffer has been sent.
     *
     * For the blocking serial_putc, TXDRDY interrupts are enabled and only used for the
     * single character TX IRQ callback handler.
     */

    /* Arm Tx DMA buffer. */
    nordic_nrf5_uart_state[instance].tx_data = character;
    nrf_uarte_tx_buffer_set(nordic_nrf5_uart_register[instance],
                            &nordic_nrf5_uart_state[instance].tx_data,
                            1);

    /* Clear TXDRDY event and enable TXDRDY interrupts. */
    nrf_uarte_event_clear(nordic_nrf5_uart_register[instance], NRF_UARTE_EVENT_TXDRDY);
    nordic_nrf5_uart_register[instance]->INTEN |= NRF_UARTE_INT_TXDRDY_MASK;

    /* Clear ENDTX event. */
    nrf_uarte_event_clear(nordic_nrf5_uart_register[instance], NRF_UARTE_EVENT_ENDTX);

    /* Trigger DMA transfer. */
    nrf_uarte_task_trigger(nordic_nrf5_uart_register[instance],
                           NRF_UARTE_TASK_STARTTX);
}

/** Check if the serial peripheral is readable
 *
 * Param obj The serial object
 * Return Non-zero value if a character can be read, 0 if nothing to read
 */
int serial_readable(serial_t *obj)
{
    MBED_ASSERT(obj);

#if DEVICE_SERIAL_ASYNCH
    struct serial_s *uart_object = &obj->serial;
    uart_object->rx_asynch = false;
#else
    struct serial_s *uart_object = obj;
#endif

    int instance = uart_object->instance;

    /* Take ownership and configure UART if necessary. */
    nordic_nrf5_serial_configure(obj);

    /**
     * Use head and tail pointer in FIFO to determine whether there is data available.
     */
    nrf_atfifo_t *fifo = nordic_nrf5_uart_state[instance].fifo;

    return (fifo->head.pos.rd != fifo->tail.pos.rd);
}

/** Check if the serial peripheral is writable
 *
 * Param obj The serial object
 * Return Non-zero value if a character can be written, 0 otherwise.
 */
int serial_writable(serial_t *obj)
{
    MBED_ASSERT(obj);

#if DEVICE_SERIAL_ASYNCH
    struct serial_s *uart_object = &obj->serial;
#else
    struct serial_s *uart_object = obj;
#endif

    int instance = uart_object->instance;

    return (nordic_nrf5_uart_state[instance].tx_in_progress == 0);
}

/***
 *                                    _                                                _____ _____
 *         /\                        | |                                         /\   |  __ \_   _|
 *        /  \   ___ _   _ _ __   ___| |__  _ __ ___  _ __   ___  _   _ ___     /  \  | |__) || |
 *       / /\ \ / __| | | | '_ \ / __| '_ \| '__/ _ \| '_ \ / _ \| | | / __|   / /\ \ |  ___/ | |
 *      / ____ \\__ \ |_| | | | | (__| | | | | | (_) | | | | (_) | |_| \__ \  / ____ \| |    _| |_
 *     /_/    \_\___/\__, |_| |_|\___|_| |_|_|  \___/|_| |_|\___/ \__,_|___/ /_/    \_\_|   |_____|
 *                    __/ |
 *                   |___/
 */

#if DEVICE_SERIAL_ASYNCH

/** Begin asynchronous TX transfer. The used buffer is specified in the serial object,
 *  tx_buff
 *
 * Param obj       The serial object
 * Param tx        The transmit buffer
 * Param tx_length The number of bytes to transmit
 * Param tx_width  Deprecated argument
 * Param handler   The serial handler
 * Param event     The logical OR of events to be registered
 * Param hint      A suggestion for how to use DMA with this transfer
 * Return Returns number of data transfered, otherwise returns 0
 */
int serial_tx_asynch(serial_t *obj, const void *tx, size_t tx_length, uint8_t tx_width, uint32_t handler, uint32_t mask, DMAUsage hint)
{
    MBED_ASSERT(obj);
    MBED_ASSERT(tx_width == 8);
    MBED_ASSERT(tx_length < 256);

    int instance = obj->serial.instance;

    /**
     * tx_in_progress acts like a mutex to ensure only one transmission can be active at a time.
     * The flag is modified using the atomic compare-and-set function.
     */
    bool mutex = false;

    do {
        uint8_t expected = 0;
        uint8_t desired = 1;

        mutex = core_util_atomic_cas_u8((uint8_t *) &nordic_nrf5_uart_state[instance].tx_in_progress, &expected, desired);
    } while (mutex == false);

    /* State variables. */
    int result = 0;
    bool valid = false;

    /**
     * EasyDMA can only access RAM. Check if provided buffer is in RAM or flash.
     * If the buffer is in flash, check if the FIFO buffer is large enough to store
     * the Tx data.
     */
    if (instance == 0) {

        if (nrf_drv_is_in_RAM(tx) || (tx_length <= UART0_FIFO_BUFFER_SIZE)) {
            valid = true;
        }
    }
#if UART1_ENABLED
    else {
        if (nrf_drv_is_in_RAM(tx) || (tx_length <= UART1_FIFO_BUFFER_SIZE)) {
            valid = true;
        }
    }
#endif

    if (valid) {

        /* Setup buffers for transfer. */
        uint8_t *buffer = NULL;

        /* Tx buffer is in RAM. */
        if (nrf_drv_is_in_RAM(tx)) {

            buffer = (uint8_t *) tx;
        } else {

            /**
             * Tx buffer is in flash. Copy Tx buffer to FIFO buffer.
             * NOTE: this prevents simultaneous Rx using non-asynchronous API.
             */
            const uint8_t *pointer = (const uint8_t *) tx;

            for (size_t index = 0; index < tx_length; index++) {
                nordic_nrf5_uart_fifo_0_data[index] = pointer[index];
            }

            buffer = (uint8_t *) nordic_nrf5_uart_fifo_0_data;
        }

        /* Store callback handler, mask and reset event value. */
        obj->serial.tx_handler = handler;
        obj->serial.tx_mask = mask;
        obj->serial.tx_event = 0;

        /* Enable asynchronous mode and configure UART. */
        nordic_nrf5_uart_state[instance].tx_asynch = true;
        nordic_nrf5_serial_configure(obj);

        /**
         * The UARTE module can generate two different Tx events: TXDRDY when each
         * character has been transmitted and ENDTX when the entire buffer has been sent.
         *
         * For the async serial_tx_async, TXDRDY interrupts are disabled completely. ENDTX
         * interrupts are enabled and used to signal the completion of the async transfer.
         *
         * The ENDTX interrupt is diabled immediately after it is fired in the ISR.
         */

        /* Clear Tx event and enable Tx interrupts. */
        nrf_uarte_event_clear(nordic_nrf5_uart_register[instance], NRF_UARTE_EVENT_ENDTX);
        nordic_nrf5_uart_register[instance]->INTEN |= NRF_UARTE_INT_ENDTX_MASK;

        /* Set Tx DMA buffer. */
        nrf_uarte_tx_buffer_set(nordic_nrf5_uart_register[obj->serial.instance],
                                buffer,
                                tx_length);

        /* Trigger DMA transfer. */
        nrf_uarte_task_trigger(nordic_nrf5_uart_register[obj->serial.instance],
                               NRF_UARTE_TASK_STARTTX);

        /* Setup complete, return length as sign of success. */
        result = tx_length;

    } else {

        /* Signal error if event mask matches and event handler is set. */
        if ((mask & SERIAL_EVENT_ERROR) && handler) {

            /* Cast handler to callback function pointer. */
            void (*callback)(void) = (void (*)(void)) handler;

            /* Store event value so it can be read back. */
            obj->serial.event = SERIAL_EVENT_ERROR;

            /* Signal callback handler. */
            callback();
        }
    }

    return result;
}

/** Begin asynchronous RX transfer (enable interrupt for data collecting)
 *  The used buffer is specified in the serial object - rx_buff
 *
 * Param obj        The serial object
 * Param rx         The receive buffer
 * Param rx_length  The number of bytes to receive
 * Param rx_width   Deprecated argument
 * Param handler    The serial handler
 * Param event      The logical OR of events to be registered
 * Param handler    The serial handler
 * Param char_match A character in range 0-254 to be matched
 * Param hint       A suggestion for how to use DMA with this transfer
 */
void serial_rx_asynch(serial_t *obj, void *rx, size_t rx_length, uint8_t rx_width, uint32_t handler, uint32_t mask, uint8_t char_match, DMAUsage hint)
{
    MBED_ASSERT(obj);
    MBED_ASSERT(rx_width == 8);
    MBED_ASSERT(rx_length < 256);
    MBED_ASSERT(char_match == SERIAL_RESERVED_CHAR_MATCH);  // EasyDMA based UART handling does not support char_match

    int instance = obj->serial.instance;

    /**
     * rx_in_progress acts like a mutex to ensure only one asynchronous reception can be active at a time.
     * The flag is modified using the atomic compare-and-set function.
     */
    bool mutex = false;

    do {
        uint8_t expected = 0;
        uint8_t desired = 1;

        mutex = core_util_atomic_cas_u8((uint8_t *) &nordic_nrf5_uart_state[instance].rx_in_progress, &expected, desired);
    } while (mutex == false);

    /* Store callback handler, mask and reset event value. */
    obj->serial.rx_handler = handler;
    obj->serial.rx_mask = mask;
    obj->serial.rx_event = 0;

    /* Enable asynchronous mode and configure UART. */
    obj->serial.rx_asynch = true;
    nordic_nrf5_serial_configure(obj);

    /* Set Rx DMA buffer. */
    nrf_uarte_rx_buffer_set(nordic_nrf5_uart_register[instance],
                            (uint8_t *) rx,
                            rx_length);

    /* Enable reception. */
    nrf_uarte_task_trigger(nordic_nrf5_uart_register[instance],
                           NRF_UARTE_TASK_STARTRX);

}

/** Attempts to determine if the serial peripheral is already in use for TX
 *
 * Param obj The serial object
 * Return Non-zero if the RX transaction is ongoing, 0 otherwise
 */
uint8_t serial_tx_active(serial_t *obj)
{
    MBED_ASSERT(obj);

    return nordic_nrf5_uart_state[obj->serial.instance].tx_asynch;
}

/** Attempts to determine if the serial peripheral is already in use for RX
 *
 * Param obj The serial object
 * Return Non-zero if the RX transaction is ongoing, 0 otherwise
 */
uint8_t serial_rx_active(serial_t *obj)
{
    MBED_ASSERT(obj);

    return nordic_nrf5_uart_state[obj->serial.instance].rx_asynch;
}

/** The asynchronous TX and RX handler.
 *
 * Param obj The serial object
 * Return Returns event flags if an RX transfer termination condition was met; otherwise returns 0
 */
int serial_irq_handler_asynch(serial_t *obj)
{
    MBED_ASSERT(obj);

    return (obj->serial.tx_event | obj->serial.rx_event);
}

/** Abort the ongoing TX transaction. It disables the enabled interupt for TX and
 *  flushes the TX hardware buffer if TX FIFO is used
 *
 * Param obj The serial object
 */
void serial_tx_abort_asynch(serial_t *obj)
{
    MBED_ASSERT(obj);

    /* Transmission might be in progress. Disable interrupts to prevent ISR from firing. */
    core_util_critical_section_enter();

    int instance = obj->serial.instance;

    /* Disable ENDTX interrupts. */
    nordic_nrf5_uart_register[instance]->INTEN &= ~NRF_UARTE_INT_ENDTX_MASK;

    /* Clear ENDTX event. */
    nrf_uarte_event_clear(nordic_nrf5_uart_register[instance], NRF_UARTE_EVENT_ENDTX);

    /* Reset Tx flags. */
    nordic_nrf5_uart_state[instance].tx_in_progress = 0;
    nordic_nrf5_uart_state[instance].tx_asynch = false;

    /* Force reconfiguration. */
    obj->serial.update = true;
    nordic_nrf5_serial_configure(obj);

    /* Trigger STOP task. */
    nrf_uarte_task_trigger(nordic_nrf5_uart_register[instance],
                           NRF_UARTE_TASK_STOPTX);

    /* Enable interrupts again. */
    core_util_critical_section_exit();
}

/** Abort the ongoing RX transaction. It disables the enabled interrupt for RX and
 *  flushes the RX hardware buffer if RX FIFO is used
 *
 * Param obj The serial object
 */
void serial_rx_abort_asynch(serial_t *obj)
{
    MBED_ASSERT(obj);

    /* Transmission might be in progress. Disable interrupts to prevent ISR from firing. */
    core_util_critical_section_enter();

    /* Reset Rx flags. */
    nordic_nrf5_uart_state[obj->serial.instance].rx_in_progress = 0;
    nordic_nrf5_uart_state[obj->serial.instance].rx_asynch = false;
    obj->serial.rx_asynch = false;

    /* Force reconfiguration. */
    obj->serial.update = true;
    nordic_nrf5_serial_configure(obj);

    /* Enable interrupts again. */
    core_util_critical_section_exit();
}

#endif // DEVICE_SERIAL_ASYNCH

#endif // DEVICE_SERIAL
