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

#include "nrf_drv_twi.h"
#include "nrf_drv_common.h"
#include "nrf_gpio.h"
#include "nrf_assert.h"
#include "app_util_platform.h"
#include "nrf_delay.h"

#include <stdio.h>

#define TWI0_IRQ_HANDLER    SPI0_TWI0_IRQHandler
#define TWI1_IRQ_HANDLER    SPI1_TWI1_IRQHandler

#if (defined(TWIM_IN_USE) && defined(TWI_IN_USE))
    // TWIM and TWI combined
    #define CODE_FOR_TWIM(code) if (p_instance->use_easy_dma) { code }
    #define CODE_FOR_TWI(code)  else { code }
#elif (defined(TWIM_IN_USE) && !defined(TWI_IN_USE))
    // TWIM only
    #define CODE_FOR_TWIM(code) { code }
    #define CODE_FOR_TWI(code)
#elif (!defined(TWIM_IN_USE) && defined(TWI_IN_USE))
    // TWI only
    #define CODE_FOR_TWIM(code)
    #define CODE_FOR_TWI(code)  { code }
#else
    #error "Wrong configuration."
#endif

// All interrupt flags
#define DISABLE_ALL  0xFFFFFFFF

#define SCL_PIN_CONF        ((GPIO_PIN_CNF_SENSE_Disabled << GPIO_PIN_CNF_SENSE_Pos)    \
                            | (GPIO_PIN_CNF_DRIVE_S0D1     << GPIO_PIN_CNF_DRIVE_Pos)   \
                            | (GPIO_PIN_CNF_PULL_Pullup    << GPIO_PIN_CNF_PULL_Pos)    \
                            | (GPIO_PIN_CNF_INPUT_Connect  << GPIO_PIN_CNF_INPUT_Pos)   \
                            | (GPIO_PIN_CNF_DIR_Input      << GPIO_PIN_CNF_DIR_Pos))

#define SDA_PIN_CONF        SCL_PIN_CONF

#define SCL_PIN_CONF_CLR    ((GPIO_PIN_CNF_SENSE_Disabled << GPIO_PIN_CNF_SENSE_Pos)    \
                            | (GPIO_PIN_CNF_DRIVE_S0D1     << GPIO_PIN_CNF_DRIVE_Pos)   \
                            | (GPIO_PIN_CNF_PULL_Pullup    << GPIO_PIN_CNF_PULL_Pos)    \
                            | (GPIO_PIN_CNF_INPUT_Connect  << GPIO_PIN_CNF_INPUT_Pos)   \
                            | (GPIO_PIN_CNF_DIR_Output     << GPIO_PIN_CNF_DIR_Pos))

#define SDA_PIN_CONF_CLR    SCL_PIN_CONF_CLR

// Control block - driver instance local data.
typedef struct
{
    nrf_drv_twi_evt_handler_t handler;
    void *                    p_context;
    volatile uint32_t         int_mask;
    nrf_drv_twi_xfer_desc_t   xfer_desc;
    uint32_t                  flags;
    uint8_t *                 p_curr_buf;
    uint8_t                   curr_length;
    bool                      curr_no_stop;
    nrf_drv_state_t           state;
    bool                      error;
    volatile bool             busy;
    bool                      repeated;
    uint8_t                   bytes_transferred;
} twi_control_block_t;

static twi_control_block_t m_cb[TWI_COUNT];

static nrf_drv_twi_config_t const m_default_config[TWI_COUNT] = {
#if TWI0_ENABLED
    NRF_DRV_TWI_DEFAULT_CONFIG(0),
#endif
#if TWI1_ENABLED
    NRF_DRV_TWI_DEFAULT_CONFIG(1),
#endif
};

#if PERIPHERAL_RESOURCE_SHARING_ENABLED
    #define IRQ_HANDLER_NAME(n) irq_handler_for_instance_##n
    #define IRQ_HANDLER(n)      static void IRQ_HANDLER_NAME(n)(void)

    #if TWI0_ENABLED
        IRQ_HANDLER(0);
    #endif
    #if TWI1_ENABLED
        IRQ_HANDLER(1);
    #endif
    static nrf_drv_irq_handler_t const m_irq_handlers[TWI_COUNT] = {
    #if TWI0_ENABLED
        IRQ_HANDLER_NAME(0),
    #endif
    #if TWI1_ENABLED
        IRQ_HANDLER_NAME(1),
    #endif
    };
#else
    #define IRQ_HANDLER(n) void SPI##n##_TWI##n##_IRQHandler(void)
#endif // PERIPHERAL_RESOURCE_SHARING_ENABLED

static void twi_clear_bus(nrf_drv_twi_t const * const p_instance,
                          nrf_drv_twi_config_t const * p_config)
{
    NRF_GPIO->PIN_CNF[p_config->scl] = SCL_PIN_CONF;
    NRF_GPIO->PIN_CNF[p_config->sda] = SDA_PIN_CONF;

    nrf_gpio_pin_set(p_config->scl);
    nrf_gpio_pin_set(p_config->sda);

    NRF_GPIO->PIN_CNF[p_config->scl] = SCL_PIN_CONF_CLR;
    NRF_GPIO->PIN_CNF[p_config->sda] = SDA_PIN_CONF_CLR;

    nrf_delay_us(4);

    for(int i = 0; i < 9; i++)
    {
        if (nrf_gpio_pin_read(p_config->sda))
        {
            if(i == 0)
            {
                return;
            }
            else
            {
                break;
            }
        }
        nrf_gpio_pin_clear(p_config->scl);
        nrf_delay_us(4);
        nrf_gpio_pin_set(p_config->scl);
        nrf_delay_us(4);
    }
    nrf_gpio_pin_clear(p_config->sda);
    nrf_delay_us(4);
    nrf_gpio_pin_set(p_config->sda);
}

ret_code_t nrf_drv_twi_init(nrf_drv_twi_t const *        p_instance,
                            nrf_drv_twi_config_t const * p_config,
                            nrf_drv_twi_evt_handler_t    event_handler,
                            void *                       p_context)
{
    twi_control_block_t * p_cb  = &m_cb[p_instance->drv_inst_idx];

    if (p_cb->state != NRF_DRV_STATE_UNINITIALIZED)
    {
        return NRF_ERROR_INVALID_STATE;
    }

    if (p_config == NULL)
    {
        p_config = &m_default_config[p_instance->drv_inst_idx];
    }

#if PERIPHERAL_RESOURCE_SHARING_ENABLED
    if (nrf_drv_common_per_res_acquire(p_instance->reg.p_twi,
            m_irq_handlers[p_instance->drv_inst_idx]) != NRF_SUCCESS)
    {
        return NRF_ERROR_BUSY;
    }
#endif // PERIPHERAL_RESOURCE_SHARING_ENABLED

    p_cb->handler   = event_handler;
    p_cb->p_context = p_context;
    p_cb->int_mask  = 0;
    p_cb->repeated  = false;
    p_cb->busy      = false;

    twi_clear_bus(p_instance, p_config);

    /* To secure correct signal levels on the pins used by the TWI
       master when the system is in OFF mode, and when the TWI master is
       disabled, these pins must be configured in the GPIO peripheral.
    */
    NRF_GPIO->PIN_CNF[p_config->scl] = SCL_PIN_CONF;
    NRF_GPIO->PIN_CNF[p_config->sda] = SDA_PIN_CONF;

    CODE_FOR_TWIM
    (
        NRF_TWIM_Type * p_twim = p_instance->reg.p_twim;
        nrf_twim_pins_set(p_twim, p_config->scl, p_config->sda);
        nrf_twim_frequency_set(p_twim,
            (nrf_twim_frequency_t)p_config->frequency);
    )
    CODE_FOR_TWI
    (
        NRF_TWI_Type * p_twi = p_instance->reg.p_twi;
        nrf_twi_pins_set(p_twi, p_config->scl, p_config->sda);
        nrf_twi_frequency_set(p_twi,
            (nrf_twi_frequency_t)p_config->frequency);
    )

    if (p_cb->handler)
    {
        CODE_FOR_TWIM
        (
            nrf_drv_common_irq_enable(nrf_drv_get_IRQn((void *)p_instance->reg.p_twim),
                p_config->interrupt_priority);
        )
        CODE_FOR_TWI
        (
            nrf_drv_common_irq_enable(nrf_drv_get_IRQn((void *)p_instance->reg.p_twi),
                p_config->interrupt_priority);
        )
    }

    p_cb->state = NRF_DRV_STATE_INITIALIZED;

    return NRF_SUCCESS;
}

void nrf_drv_twi_uninit(nrf_drv_twi_t const * p_instance)
{
    twi_control_block_t * p_cb = &m_cb[p_instance->drv_inst_idx];
    ASSERT(p_cb->state != NRF_DRV_STATE_UNINITIALIZED);

    if (p_cb->handler)
    {
        CODE_FOR_TWIM
        (
            nrf_drv_common_irq_disable(nrf_drv_get_IRQn((void *)p_instance->reg.p_twim));
        )
        CODE_FOR_TWI
        (
            nrf_drv_common_irq_disable(nrf_drv_get_IRQn((void *)p_instance->reg.p_twi));
        )
    }
    nrf_drv_twi_disable(p_instance);

#if PERIPHERAL_RESOURCE_SHARING_ENABLED
    nrf_drv_common_per_res_release(p_instance->reg.p_twi);
#endif

    p_cb->state = NRF_DRV_STATE_UNINITIALIZED;
}

void nrf_drv_twi_enable(nrf_drv_twi_t const * p_instance)
{
    twi_control_block_t * p_cb = &m_cb[p_instance->drv_inst_idx];
    ASSERT(p_cb->state == NRF_DRV_STATE_INITIALIZED);

    CODE_FOR_TWIM
    (
        NRF_TWIM_Type * p_twim = p_instance->reg.p_twim;

        nrf_twim_enable(p_twim);
    )
    CODE_FOR_TWI
    (
        NRF_TWI_Type * p_twi = p_instance->reg.p_twi;

        nrf_twi_enable(p_twi);
    )

    p_cb->state = NRF_DRV_STATE_POWERED_ON;
}

void nrf_drv_twi_disable(nrf_drv_twi_t const * p_instance)
{
    twi_control_block_t * p_cb = &m_cb[p_instance->drv_inst_idx];
    ASSERT(p_cb->state != NRF_DRV_STATE_UNINITIALIZED);

    CODE_FOR_TWIM
    (
        NRF_TWIM_Type * p_twim = p_instance->reg.p_twim;
        p_cb->int_mask = 0;
        nrf_twim_int_disable(p_twim, DISABLE_ALL);
        nrf_twim_shorts_disable(p_twim, DISABLE_ALL);
        nrf_twim_disable(p_twim);
    )
    CODE_FOR_TWI
    (
        NRF_TWI_Type * p_twi = p_instance->reg.p_twi;
        nrf_twi_int_disable(p_twi, DISABLE_ALL);
        nrf_twi_shorts_disable(p_twi, DISABLE_ALL);
        nrf_twi_disable(p_twi);
    )

    p_cb->state = NRF_DRV_STATE_INITIALIZED;
}

#ifdef TWI_IN_USE
static bool twi_send_byte(NRF_TWI_Type  * p_twi,
                          uint8_t const * p_data,
                          uint8_t         length,
                          uint8_t       * p_bytes_transferred,
                          bool            no_stop)
{
    if (*p_bytes_transferred < length)
    {
        nrf_twi_txd_set(p_twi, p_data[*p_bytes_transferred]);
        ++(*p_bytes_transferred);
    }
    else
    {
        if (no_stop)
        {
            nrf_twi_task_trigger(p_twi, NRF_TWI_TASK_SUSPEND);
            return false;
        }
        else
        {
            nrf_twi_task_trigger(p_twi, NRF_TWI_TASK_STOP);
        }
    }
    return true;
}

static void twi_receive_byte(NRF_TWI_Type * p_twi,
                             uint8_t      * p_data,
                             uint8_t        length,
                             uint8_t      * p_bytes_transferred)
{
    if (*p_bytes_transferred < length)
    {
        p_data[*p_bytes_transferred] = nrf_twi_rxd_get(p_twi);

        ++(*p_bytes_transferred);

        if (*p_bytes_transferred == length-1)
        {
            nrf_twi_shorts_set(p_twi, NRF_TWI_SHORT_BB_STOP_MASK);
        }
        else if (*p_bytes_transferred == length)
        {
            return;
        }

        nrf_twi_task_trigger(p_twi, NRF_TWI_TASK_RESUME);
    }
}

static bool twi_transfer(NRF_TWI_Type  * p_twi,
                         bool          * p_error,
                         uint8_t       * p_bytes_transferred,
                         uint8_t       * p_data,
                         uint8_t         length,
                         bool            no_stop)
{
    bool do_stop_check;

    if ((*p_error == true) || (*p_bytes_transferred == length))
    {
        do_stop_check = true;
    }
    else
    {
        do_stop_check = false;
    }

    if (*p_error)
    {
        nrf_twi_event_clear(p_twi, NRF_TWI_EVENT_ERROR);
        nrf_twi_event_clear(p_twi, NRF_TWI_EVENT_TXDSENT);
        nrf_twi_event_clear(p_twi, NRF_TWI_EVENT_RXDREADY);
    }
    else if (nrf_twi_event_check(p_twi, NRF_TWI_EVENT_ERROR))
    {
        nrf_twi_event_clear(p_twi, NRF_TWI_EVENT_ERROR);
        nrf_twi_task_trigger(p_twi, NRF_TWI_TASK_STOP);
        *p_error = true;
    }
    else
    {
        if (nrf_twi_event_check(p_twi, NRF_TWI_EVENT_TXDSENT))
        {
            nrf_twi_event_clear(p_twi, NRF_TWI_EVENT_TXDSENT);
            if (nrf_twi_event_check(p_twi, NRF_TWI_EVENT_ERROR))
            {
                nrf_twi_event_clear(p_twi, NRF_TWI_EVENT_ERROR);
                nrf_twi_task_trigger(p_twi, NRF_TWI_TASK_STOP);
                *p_error = true;
            }
            else
            {
                if (!twi_send_byte(p_twi, p_data, length, p_bytes_transferred, no_stop))
                {
                    return false;
                }
            }
        }
        else if (nrf_twi_event_check(p_twi, NRF_TWI_EVENT_RXDREADY))
        {
            nrf_twi_event_clear(p_twi, NRF_TWI_EVENT_RXDREADY);
            if (nrf_twi_event_check(p_twi, NRF_TWI_EVENT_ERROR))
            {
                nrf_twi_event_clear(p_twi, NRF_TWI_EVENT_ERROR);
                nrf_twi_task_trigger(p_twi, NRF_TWI_TASK_STOP);
                *p_error = true;
            }
            else
            {
                twi_receive_byte(p_twi, p_data, length, p_bytes_transferred);
            }
        }
    }

    if (do_stop_check && nrf_twi_event_check(p_twi, NRF_TWI_EVENT_STOPPED))
    {
        nrf_twi_event_clear(p_twi, NRF_TWI_EVENT_STOPPED);
        return false;
    }

    return true;
}

static ret_code_t twi_tx_start_transfer(twi_control_block_t * p_cb,
                                        NRF_TWI_Type *        p_twi,
                                        uint8_t const *       p_data,
                                        uint8_t               length,
                                        bool                  no_stop)
{
    ret_code_t ret_code = NRF_SUCCESS;

    nrf_twi_event_clear(p_twi, NRF_TWI_EVENT_STOPPED);
    nrf_twi_event_clear(p_twi, NRF_TWI_EVENT_ERROR);
    nrf_twi_event_clear(p_twi, NRF_TWI_EVENT_TXDSENT);
    nrf_twi_event_clear(p_twi, NRF_TWI_EVENT_RXDREADY);
    nrf_twi_shorts_set(p_twi, 0);

    p_cb->bytes_transferred = 0;
    p_cb->error             = false;

    // In case TWI is suspended resume its operation.
    nrf_twi_task_trigger(p_twi, NRF_TWI_TASK_RESUME);
    nrf_twi_task_trigger(p_twi, NRF_TWI_TASK_STARTTX);

    (void)twi_send_byte(p_twi, p_data, length, &p_cb->bytes_transferred, no_stop);

    if (p_cb->handler)
    {
        p_cb->int_mask = NRF_TWI_INT_STOPPED_MASK   |
                        NRF_TWI_INT_ERROR_MASK     |
                        NRF_TWI_INT_TXDSENT_MASK   |
                        NRF_TWI_INT_RXDREADY_MASK;
        nrf_twi_int_enable(p_twi, p_cb->int_mask);
    }
    else
    {
        while (twi_transfer(p_twi, &p_cb->error, &p_cb->bytes_transferred, (uint8_t *)p_data, length, no_stop))
        {}

        if (p_cb->error)
        {
            ret_code = NRF_ERROR_INTERNAL;
        }
    }
    return ret_code;
}

static ret_code_t twi_rx_start_transfer(twi_control_block_t * p_cb,
                                        NRF_TWI_Type *        p_twi,
                                        uint8_t const *       p_data,
                                        uint8_t               length)
{
    ret_code_t ret_code = NRF_SUCCESS;

    nrf_twi_event_clear(p_twi, NRF_TWI_EVENT_STOPPED);
    nrf_twi_event_clear(p_twi, NRF_TWI_EVENT_ERROR);
    nrf_twi_event_clear(p_twi, NRF_TWI_EVENT_TXDSENT);
    nrf_twi_event_clear(p_twi, NRF_TWI_EVENT_RXDREADY);

    p_cb->bytes_transferred = 0;
    p_cb->error             = false;

    if (length == 1)
    {
        nrf_twi_shorts_set(p_twi, NRF_TWI_SHORT_BB_STOP_MASK);
    }
    else
    {
        nrf_twi_shorts_set(p_twi, NRF_TWI_SHORT_BB_SUSPEND_MASK);
    }
    // In case TWI is suspended resume its operation.
    nrf_twi_task_trigger(p_twi, NRF_TWI_TASK_RESUME);
    nrf_twi_task_trigger(p_twi, NRF_TWI_TASK_STARTRX);

    if (p_cb->handler)
    {
        p_cb->int_mask = NRF_TWI_INT_STOPPED_MASK   |
                        NRF_TWI_INT_ERROR_MASK     |
                        NRF_TWI_INT_TXDSENT_MASK   |
                        NRF_TWI_INT_RXDREADY_MASK;
        nrf_twi_int_enable(p_twi, p_cb->int_mask);
    }
    else
    {
        while (twi_transfer(p_twi, &p_cb->error, &p_cb->bytes_transferred, (uint8_t*)p_data, length, false))
        {}

        if (p_cb->error)
        {
            ret_code = NRF_ERROR_INTERNAL;
        }
    }
    return ret_code;
}

__STATIC_INLINE ret_code_t twi_xfer(twi_control_block_t           * p_cb,
                                    NRF_TWI_Type                  * p_twi,
                                    nrf_drv_twi_xfer_desc_t const * p_xfer_desc,
                                    uint32_t                        flags)
{
    ret_code_t ret = NRF_SUCCESS;

    /* Block TWI interrupts to ensure that function is not interrupted by TWI interrupt. */
    nrf_twi_int_disable(p_twi, DISABLE_ALL);

    if (p_cb->busy)
    {
        nrf_twi_int_enable(p_twi, p_cb->int_mask);
        return NRF_ERROR_BUSY;
    }
    else
    {
        p_cb->busy = (NRF_DRV_TWI_FLAG_NO_XFER_EVT_HANDLER & flags) ? false : true;
    }

    if (flags & NRF_DRV_TWI_FLAG_HOLD_XFER)
    {
        return NRF_ERROR_NOT_SUPPORTED;
    }

    p_cb->flags       = flags;
    p_cb->xfer_desc   = *p_xfer_desc;
    p_cb->curr_length = p_xfer_desc->primary_length;
    p_cb->p_curr_buf  = p_xfer_desc->p_primary_buf;
    nrf_twi_address_set(p_twi, p_xfer_desc->address);

    if (p_xfer_desc->type != NRF_DRV_TWI_XFER_RX)
    {
        p_cb->curr_no_stop = ((p_xfer_desc->type == NRF_DRV_TWI_XFER_TX) &&
                             !(flags & NRF_DRV_TWI_FLAG_TX_NO_STOP)) ? false : true;
        ret = twi_tx_start_transfer(p_cb, p_twi, p_xfer_desc->p_primary_buf, p_xfer_desc->primary_length, p_cb->curr_no_stop);
    }
    else
    {
        p_cb->curr_no_stop = false;
        ret = twi_rx_start_transfer(p_cb, p_twi, p_xfer_desc->p_primary_buf, p_xfer_desc->primary_length);
    }
    if (p_cb->handler == NULL)
    {
        p_cb->busy = false;
    }
    return ret;
}
#endif

#ifdef TWIM_IN_USE
__STATIC_INLINE void twim_list_enable_handle(NRF_TWIM_Type * p_twim, uint32_t flags)
{
    if (NRF_DRV_TWI_FLAG_TX_POSTINC & flags)
    {
        nrf_twim_tx_list_enable(p_twim);
    }
    else
    {
        nrf_twim_tx_list_disable(p_twim);
    }

    if (NRF_DRV_TWI_FLAG_RX_POSTINC & flags)
    {
        nrf_twim_rx_list_enable(p_twim);
    }
    else
    {
        nrf_twim_rx_list_disable(p_twim);
    }
#ifndef NRF52_PAN_46
#endif
}
__STATIC_INLINE ret_code_t twim_xfer(twi_control_block_t           * p_cb,
                                     NRF_TWIM_Type                 * p_twim,
                                     nrf_drv_twi_xfer_desc_t const * p_xfer_desc,
                                     uint32_t                        flags)
{
    ret_code_t ret = NRF_SUCCESS;
    nrf_twim_task_t  start_task = NRF_TWIM_TASK_STARTTX;
    nrf_twim_event_t evt_to_wait = NRF_TWIM_EVENT_STOPPED;

    if (!nrf_drv_is_in_RAM(p_xfer_desc->p_primary_buf))
    {
        return NRF_ERROR_INVALID_ADDR;
    }
    /* Block TWI interrupts to ensure that function is not interrupted by TWI interrupt. */
    nrf_twim_int_disable(p_twim, DISABLE_ALL);
    if (p_cb->busy)
    {
        nrf_twim_int_enable(p_twim, p_cb->int_mask);
        return NRF_ERROR_BUSY;
    }
    else
    {

        p_cb->busy = ((NRF_DRV_TWI_FLAG_NO_XFER_EVT_HANDLER & flags) ||
                      (NRF_DRV_TWI_FLAG_REPEATED_XFER & flags)) ? false: true;
    }

    p_cb->xfer_desc = *p_xfer_desc;
    p_cb->repeated = (flags & NRF_DRV_TWI_FLAG_REPEATED_XFER) ? true : false;
    nrf_twim_address_set(p_twim, p_xfer_desc->address);

    nrf_twim_event_clear(p_twim, NRF_TWIM_EVENT_STOPPED);
    nrf_twim_event_clear(p_twim, NRF_TWIM_EVENT_ERROR);

    twim_list_enable_handle(p_twim, flags);
    switch (p_xfer_desc->type)
    {
    case NRF_DRV_TWI_XFER_TXTX:
        ASSERT(!(flags & NRF_DRV_TWI_FLAG_REPEATED_XFER));
        ASSERT(!(flags & NRF_DRV_TWI_FLAG_HOLD_XFER));
        ASSERT(!(flags & NRF_DRV_TWI_FLAG_NO_XFER_EVT_HANDLER));
        if (!nrf_drv_is_in_RAM(p_xfer_desc->p_secondary_buf))
        {
            return NRF_ERROR_INVALID_ADDR;
        }
        nrf_twim_shorts_set(p_twim, NRF_TWIM_SHORT_LASTTX_SUSPEND_MASK);
        nrf_twim_tx_buffer_set(p_twim, p_xfer_desc->p_primary_buf, p_xfer_desc->primary_length);
        nrf_twim_event_clear(p_twim, NRF_TWIM_EVENT_TXSTARTED);
        nrf_twim_event_clear(p_twim, NRF_TWIM_EVENT_LASTTX);
        nrf_twim_event_clear(p_twim, NRF_TWIM_EVENT_SUSPENDED);
        nrf_twim_task_trigger(p_twim, NRF_TWIM_TASK_RESUME);
        nrf_twim_task_trigger(p_twim, NRF_TWIM_TASK_STARTTX);
        while(!nrf_twim_event_check(p_twim, NRF_TWIM_EVENT_TXSTARTED))
        {}
        nrf_twim_event_clear(p_twim, NRF_TWIM_EVENT_TXSTARTED);
        nrf_twim_tx_buffer_set(p_twim, p_xfer_desc->p_secondary_buf, p_xfer_desc->secondary_length);
        p_cb->int_mask = NRF_TWIM_INT_SUSPENDED_MASK | NRF_TWIM_INT_ERROR_MASK;
        break;
    case NRF_DRV_TWI_XFER_TXRX:
        nrf_twim_tx_buffer_set(p_twim, p_xfer_desc->p_primary_buf, p_xfer_desc->primary_length);
        nrf_twim_rx_buffer_set(p_twim, p_xfer_desc->p_secondary_buf, p_xfer_desc->secondary_length);
        nrf_twim_shorts_set(p_twim, NRF_TWIM_SHORT_LASTTX_STARTRX_MASK |
                                    NRF_TWIM_SHORT_LASTRX_STOP_MASK);
        p_cb->int_mask = NRF_TWIM_INT_STOPPED_MASK | NRF_TWIM_INT_ERROR_MASK;
        break;
    case NRF_DRV_TWI_XFER_TX:
        nrf_twim_tx_buffer_set(p_twim, p_xfer_desc->p_primary_buf, p_xfer_desc->primary_length);
        if (NRF_DRV_TWI_FLAG_TX_NO_STOP & flags)
        {
            nrf_twim_shorts_set(p_twim, NRF_TWIM_SHORT_LASTTX_SUSPEND_MASK);
            p_cb->int_mask = NRF_TWIM_INT_SUSPENDED_MASK | NRF_TWIM_INT_ERROR_MASK;
            nrf_twim_event_clear(p_twim, NRF_TWIM_EVENT_SUSPENDED);
            evt_to_wait = NRF_TWIM_EVENT_SUSPENDED;
        }
        else
        {
            nrf_twim_shorts_set(p_twim, NRF_TWIM_SHORT_LASTTX_STOP_MASK);
            p_cb->int_mask = NRF_TWIM_INT_STOPPED_MASK | NRF_TWIM_INT_ERROR_MASK;
        }
        nrf_twim_task_trigger(p_twim, NRF_TWIM_TASK_RESUME);
        break;
    case NRF_DRV_TWI_XFER_RX:
        nrf_twim_rx_buffer_set(p_twim, p_xfer_desc->p_primary_buf, p_xfer_desc->primary_length);
        nrf_twim_shorts_set(p_twim, NRF_TWIM_SHORT_LASTRX_STOP_MASK);
        p_cb->int_mask = NRF_TWIM_INT_STOPPED_MASK | NRF_TWIM_INT_ERROR_MASK;
        start_task = NRF_TWIM_TASK_STARTRX;
        nrf_twim_task_trigger(p_twim, NRF_TWIM_TASK_RESUME);
        break;
    default:
        ret = NRF_ERROR_INVALID_PARAM;
        break;
    }

    if (!(flags & NRF_DRV_TWI_FLAG_HOLD_XFER) && (p_xfer_desc->type != NRF_DRV_TWI_XFER_TXTX))
    {
        nrf_twim_task_trigger(p_twim, start_task);
    }

    if (p_cb->handler)
    {
        if (flags & NRF_DRV_TWI_FLAG_NO_XFER_EVT_HANDLER)
        {
            p_cb->int_mask = NRF_TWIM_INT_ERROR_MASK;
        }
        nrf_twim_int_enable(p_twim, p_cb->int_mask);
    }
    else
    {
        while (!nrf_twim_event_check(p_twim, evt_to_wait))
        {
            if (nrf_twim_event_check(p_twim, NRF_TWIM_EVENT_ERROR))
            {
                nrf_twim_event_clear(p_twim, NRF_TWIM_EVENT_ERROR);
                nrf_twim_task_trigger(p_twim, NRF_TWIM_TASK_RESUME);
                nrf_twim_task_trigger(p_twim, NRF_TWIM_TASK_STOP);
                evt_to_wait = NRF_TWIM_EVENT_STOPPED;
            }
        }

        uint32_t errorsrc =  nrf_twim_errorsrc_get_and_clear(p_twim);

        p_cb->busy = false;

        if (errorsrc)
        {
            ret = NRF_ERROR_INTERNAL;
        }
    }
    return ret;
}
#endif

ret_code_t nrf_drv_twi_xfer(nrf_drv_twi_t           const * p_instance,
                            nrf_drv_twi_xfer_desc_t const * p_xfer_desc,
                            uint32_t                        flags)
{
    ret_code_t ret = NRF_SUCCESS;
    twi_control_block_t * p_cb = &m_cb[p_instance->drv_inst_idx];

    // TXRX and TXTX transfers are support only in non-blocking mode.
    ASSERT( !((p_cb->handler == NULL) && (p_xfer_desc->type == NRF_DRV_TWI_XFER_TXRX)));
    ASSERT( !((p_cb->handler == NULL) && (p_xfer_desc->type == NRF_DRV_TWI_XFER_TXTX)));

    CODE_FOR_TWIM
    (
        ret = twim_xfer(p_cb, (NRF_TWIM_Type *)p_instance->reg.p_twim, p_xfer_desc, flags);
    )
    CODE_FOR_TWI
    (
        if ( (NRF_DRV_TWI_FLAG_TX_POSTINC | NRF_DRV_TWI_FLAG_RX_POSTINC) & flags)
        {
            return NRF_ERROR_NOT_SUPPORTED;
        }
        ret = twi_xfer(p_cb, (NRF_TWI_Type  *)p_instance->reg.p_twi, p_xfer_desc, flags);
    )
    return ret;
}

ret_code_t nrf_drv_twi_tx(nrf_drv_twi_t const * p_instance,
                          uint8_t               address,
                          uint8_t const *       p_data,
                          uint8_t               length,
                          bool                  no_stop)
{
    nrf_drv_twi_xfer_desc_t xfer = NRF_DRV_TWI_XFER_DESC_TX(address, (uint8_t*)p_data, length);

    return nrf_drv_twi_xfer(p_instance, &xfer, no_stop ? NRF_DRV_TWI_FLAG_TX_NO_STOP : 0);
}

ret_code_t nrf_drv_twi_rx(nrf_drv_twi_t const * p_instance,
                          uint8_t               address,
                          uint8_t *             p_data,
                          uint8_t               length)
{
    nrf_drv_twi_xfer_desc_t xfer = NRF_DRV_TWI_XFER_DESC_RX(address, p_data, length);
    return nrf_drv_twi_xfer(p_instance, &xfer, 0);
}

uint32_t nrf_drv_twi_data_count_get(nrf_drv_twi_t const * const p_instance)
{
    CODE_FOR_TWIM
    (
        ASSERT(false);
        return 0;
    )
    CODE_FOR_TWI
    (
        return m_cb[p_instance->drv_inst_idx].bytes_transferred;
    )
}
uint32_t nrf_drv_twi_start_task_get(nrf_drv_twi_t const * p_instance, nrf_drv_twi_xfer_type_t xfer_type)
{
    CODE_FOR_TWIM
    (
        return (uint32_t)nrf_twim_task_address_get(p_instance->reg.p_twim,
            (xfer_type != NRF_DRV_TWI_XFER_RX) ? NRF_TWIM_TASK_STARTTX : NRF_TWIM_TASK_STARTRX);
    )
    CODE_FOR_TWI
    (
        return (uint32_t)nrf_twi_task_address_get(p_instance->reg.p_twi,
                (xfer_type != NRF_DRV_TWI_XFER_RX) ? NRF_TWI_TASK_STARTTX : NRF_TWI_TASK_STARTRX);
    )
}

uint32_t nrf_drv_twi_stopped_event_get(nrf_drv_twi_t const * p_instance)
{
    CODE_FOR_TWIM
    (
        return (uint32_t)nrf_twim_event_address_get(p_instance->reg.p_twim, NRF_TWIM_EVENT_STOPPED);
    )
    CODE_FOR_TWI
    (
        return (uint32_t)nrf_twi_event_address_get(p_instance->reg.p_twi, NRF_TWI_EVENT_STOPPED);
    )
}

#ifdef TWIM_IN_USE
static void irq_handler_twim(NRF_TWIM_Type * p_twim, twi_control_block_t * p_cb)
{
    ASSERT(p_cb->handler);

    if (nrf_twim_event_check(p_twim, NRF_TWIM_EVENT_ERROR))
    {
        nrf_twim_event_clear(p_twim, NRF_TWIM_EVENT_ERROR);
        if (!nrf_twim_event_check(p_twim, NRF_TWIM_EVENT_STOPPED))
        {
            nrf_twim_int_disable(p_twim, p_cb->int_mask);
            p_cb->int_mask = NRF_TWIM_INT_STOPPED_MASK;
            nrf_twim_int_enable(p_twim, p_cb->int_mask);

            nrf_twim_task_trigger(p_twim, NRF_TWIM_TASK_RESUME);
            nrf_twim_task_trigger(p_twim, NRF_TWIM_TASK_STOP);
            return;
        }
    }

    nrf_drv_twi_evt_t event;

    if (nrf_twim_event_check(p_twim, NRF_TWIM_EVENT_STOPPED))
    {
        nrf_twim_event_clear(p_twim, NRF_TWIM_EVENT_STOPPED);
        event.xfer_desc = p_cb->xfer_desc;
        if (p_cb->error)
        {

            event.xfer_desc.primary_length = (p_cb->xfer_desc.type == NRF_DRV_TWI_XFER_RX) ?
                (uint8_t)nrf_twim_rxd_amount_get(p_twim) : (uint8_t)nrf_twim_txd_amount_get(p_twim);
            event.xfer_desc.secondary_length = (p_cb->xfer_desc.type == NRF_DRV_TWI_XFER_TXRX) ?
                (uint8_t)nrf_twim_rxd_amount_get(p_twim) : (uint8_t)nrf_twim_txd_amount_get(p_twim);

        }
        nrf_twim_event_clear(p_twim, NRF_TWIM_EVENT_LASTTX);
        nrf_twim_event_clear(p_twim, NRF_TWIM_EVENT_LASTRX);
        if (!p_cb->repeated || p_cb->error)
        {
            nrf_twim_shorts_set(p_twim, 0);
            p_cb->int_mask = 0;
            nrf_twim_int_disable(p_twim, DISABLE_ALL);
        }
    }
    else
    {
        nrf_twim_event_clear(p_twim, NRF_TWIM_EVENT_SUSPENDED);
        if (p_cb->xfer_desc.type == NRF_DRV_TWI_XFER_TX)
        {
            event.xfer_desc = p_cb->xfer_desc;
            if (!p_cb->repeated)
            {
                nrf_twim_shorts_set(p_twim, 0);
                p_cb->int_mask = 0;
                nrf_twim_int_disable(p_twim, DISABLE_ALL);
            }
        }
        else
        {
            nrf_twim_shorts_set(p_twim, NRF_TWIM_SHORT_LASTTX_STOP_MASK);
            p_cb->int_mask = NRF_TWIM_INT_STOPPED_MASK | NRF_TWIM_INT_ERROR_MASK;
            nrf_twim_int_disable(p_twim, DISABLE_ALL);
            nrf_twim_int_enable(p_twim, p_cb->int_mask);
            nrf_twim_task_trigger(p_twim, NRF_TWIM_TASK_STARTTX);
            nrf_twim_task_trigger(p_twim, NRF_TWIM_TASK_RESUME);
            return;
        }
    }

    uint32_t errorsrc = nrf_twim_errorsrc_get_and_clear(p_twim);
    if (errorsrc & NRF_TWIM_ERROR_ADDRESS_NACK)
    {
        event.type = NRF_DRV_TWI_EVT_ADDRESS_NACK;
    }
    else if (errorsrc & NRF_TWIM_ERROR_DATA_NACK)
    {
        event.type = NRF_DRV_TWI_EVT_DATA_NACK;
    }
    else
    {
        event.type = NRF_DRV_TWI_EVT_DONE;
    }

    if (!p_cb->repeated)
    {
        p_cb->busy = false;
    }
    p_cb->handler(&event, p_cb->p_context);
}
#endif // TWIM_IN_USE

#ifdef TWI_IN_USE
static void irq_handler_twi(NRF_TWI_Type * p_twi, twi_control_block_t * p_cb)
{
    ASSERT(p_cb->handler);

    if (twi_transfer(p_twi, &p_cb->error, &p_cb->bytes_transferred, p_cb->p_curr_buf, p_cb->curr_length, p_cb->curr_no_stop ))
    {
        return;
    }

    if (!p_cb->error &&
        ((p_cb->xfer_desc.type == NRF_DRV_TWI_XFER_TXRX) ||
         (p_cb->xfer_desc.type == NRF_DRV_TWI_XFER_TXTX)) &&
        p_cb->p_curr_buf == p_cb->xfer_desc.p_primary_buf)
    {
        p_cb->p_curr_buf   = p_cb->xfer_desc.p_secondary_buf;
        p_cb->curr_length  = p_cb->xfer_desc.secondary_length;
        p_cb->curr_no_stop = (p_cb->flags & NRF_DRV_TWI_FLAG_TX_NO_STOP);

        if (p_cb->xfer_desc.type == NRF_DRV_TWI_XFER_TXTX)
        {
            (void)twi_tx_start_transfer(p_cb, p_twi, p_cb->p_curr_buf, p_cb->curr_length, p_cb->curr_no_stop);
        }
        else
        {
            (void)twi_rx_start_transfer(p_cb, p_twi, p_cb->p_curr_buf, p_cb->curr_length);
        }
    }
    else
    {
        nrf_drv_twi_evt_t event;
        event.xfer_desc = p_cb->xfer_desc;

        if (p_cb->error)
        {
            uint32_t errorsrc = nrf_twi_errorsrc_get_and_clear(p_twi);
            if (errorsrc & NRF_TWI_ERROR_ADDRESS_NACK)
            {
                event.type = NRF_DRV_TWI_EVT_ADDRESS_NACK;
            }
            else if (errorsrc & NRF_TWI_ERROR_DATA_NACK)
            {
                event.type = NRF_DRV_TWI_EVT_DATA_NACK;
            }
        }
        else
        {
            event.type = NRF_DRV_TWI_EVT_DONE;
        }

        p_cb->busy = false;

        if (!(NRF_DRV_TWI_FLAG_NO_XFER_EVT_HANDLER & p_cb->flags))
        {
            p_cb->handler(&event, p_cb->p_context);
        }
    }

}
#endif // TWI_IN_USE

#if TWI0_ENABLED
IRQ_HANDLER(0)
{
    #if (TWI0_USE_EASY_DMA == 1) && defined(NRF52)
        irq_handler_twim(NRF_TWIM0,
    #else
        irq_handler_twi(NRF_TWI0,
    #endif
            &m_cb[TWI0_INSTANCE_INDEX]);
}
#endif // TWI0_ENABLED

#if TWI1_ENABLED
IRQ_HANDLER(1)
{
    #if (TWI1_USE_EASY_DMA == 1)
        irq_handler_twim(NRF_TWIM1,
    #else
        irq_handler_twi(NRF_TWI1,
    #endif
            &m_cb[TWI1_INSTANCE_INDEX]);
}
#endif // TWI1_ENABLED
