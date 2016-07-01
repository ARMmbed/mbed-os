/* Copyright (c) 2013 Nordic Semiconductor. All Rights Reserved.
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
 
#include "nrf_drv_spis.h"
#include <stdbool.h>
#include <stdio.h>
#include "nrf.h"
#include "nrf_gpio.h"
#include "app_error.h"
#include "app_util_platform.h"
#include "nrf_drv_config.h"
#include "nrf_drv_common.h"
#include "nordic_common.h"
#include "sdk_common.h"
#include "nrf_assert.h"

#if !SPIS_COUNT
    #warning No SPIS instances enabled.
#else

/**@brief States of the SPI transaction state machine. */
typedef enum
{
    SPIS_STATE_INIT,                                 /**< Initialization state. In this state the module waits for a call to @ref spi_slave_buffers_set. */                                                                                             
    SPIS_BUFFER_RESOURCE_REQUESTED,                  /**< State where the configuration of the memory buffers, which are to be used in SPI transaction, has started. */
    SPIS_BUFFER_RESOURCE_CONFIGURED,                 /**< State where the configuration of the memory buffers, which are to be used in SPI transaction, has completed. */
    SPIS_XFER_COMPLETED                              /**< State where SPI transaction has been completed. */
} nrf_drv_spis_state_t;


#if PERIPHERAL_RESOURCE_SHARING_ENABLED
    #define IRQ_HANDLER_NAME(n) irq_handler_for_instance_##n
    #define IRQ_HANDLER(n)      static void IRQ_HANDLER_NAME(n)(void)

    #if SPIS0_ENABLED
        IRQ_HANDLER(0);
    #endif
    #if SPIS1_ENABLED
        IRQ_HANDLER(1);
    #endif
    #if SPIS2_ENABLED
        IRQ_HANDLER(2);
    #endif
    static nrf_drv_irq_handler_t const m_irq_handlers[SPIS_COUNT] = {
    #if SPIS0_ENABLED
        IRQ_HANDLER_NAME(0),
    #endif
    #if SPIS1_ENABLED
        IRQ_HANDLER_NAME(1),
    #endif
    #if SPIS2_ENABLED
        IRQ_HANDLER_NAME(2),
    #endif
    };
#else
    #define IRQ_HANDLER(n) void SPIS##n##_IRQ_HANDLER(void)
#endif // PERIPHERAL_RESOURCE_SHARING_ENABLED

#define SPIS_IRQHANDLER_TEMPLATE(NUM) \
    IRQ_HANDLER(NUM)                                                        \
    {                                                                       \
        spis_irq_handler(NRF_SPIS##NUM, &m_cb[SPIS##NUM##_INSTANCE_INDEX]); \
    }


/**@brief SPIS control block - driver instance local data. */
typedef struct
{
    volatile uint32_t             tx_buffer_size;  //!< SPI slave TX buffer size in bytes.
    volatile uint32_t             rx_buffer_size;  //!< SPI slave RX buffer size in bytes.
    nrf_drv_spis_event_handler_t  handler;         //!< SPI event handler.
    volatile const uint8_t *      tx_buffer;       //!< SPI slave TX buffer.
    volatile uint8_t *            rx_buffer;       //!< SPI slave RX buffer.
    nrf_drv_state_t               state;           //!< driver initialization state.
    volatile nrf_drv_spis_state_t spi_state;       //!< SPI slave state.
} spis_cb_t;

static spis_cb_t m_cb[SPIS_COUNT];

static nrf_drv_spis_config_t const m_default_config[SPIS_COUNT] = {
#if SPIS0_ENABLED
    NRF_DRV_SPIS_DEFAULT_CONFIG(0),
#endif
#if SPIS1_ENABLED
    NRF_DRV_SPIS_DEFAULT_CONFIG(1),
#endif
#if SPIS2_ENABLED
    NRF_DRV_SPIS_DEFAULT_CONFIG(2),
#endif
};


ret_code_t nrf_drv_spis_init(nrf_drv_spis_t const * const  p_instance,
                             nrf_drv_spis_config_t const * p_config,
                             nrf_drv_spis_event_handler_t  event_handler)
{
    spis_cb_t * p_cb = &m_cb[p_instance->instance_id];
    
    NRF_SPIS_Type * p_spis = p_instance->p_reg;

    if (p_cb->state != NRF_DRV_STATE_UNINITIALIZED)
    {
        return NRF_ERROR_INVALID_STATE;
    }
    if (p_config == NULL)
    {
        p_config = &m_default_config[p_instance->instance_id];
    }
    if ((uint32_t)p_config->mode > (uint32_t)NRF_DRV_SPIS_MODE_3)
    {
        return NRF_ERROR_INVALID_PARAM;
    }
    if (!event_handler)
    {
        return NRF_ERROR_NULL;
    }
#if PERIPHERAL_RESOURCE_SHARING_ENABLED
    if (nrf_drv_common_per_res_acquire(p_spis,
            m_irq_handlers[p_instance->instance_id]) != NRF_SUCCESS)
    {
        return NRF_ERROR_BUSY;
    }
#endif

    // Configure the SPI pins for input.
    uint32_t mosi_pin;
    uint32_t miso_pin;
    
    if (p_config->miso_pin != NRF_DRV_SPIS_PIN_NOT_USED)
    {
        nrf_gpio_cfg(p_config->miso_pin,
                    NRF_GPIO_PIN_DIR_INPUT,
                    NRF_GPIO_PIN_INPUT_CONNECT,
                    NRF_GPIO_PIN_NOPULL,
                    p_config->miso_drive,
                    NRF_GPIO_PIN_NOSENSE);
        miso_pin = p_config->miso_pin;
    }
    else
    {
        miso_pin = NRF_SPIS_PIN_NOT_CONNECTED;
    }
    
    if (p_config->mosi_pin != NRF_DRV_SPIS_PIN_NOT_USED)
    {
        nrf_gpio_cfg(p_config->mosi_pin,
                     NRF_GPIO_PIN_DIR_INPUT,
                     NRF_GPIO_PIN_INPUT_CONNECT,
                     NRF_GPIO_PIN_NOPULL,
                     NRF_GPIO_PIN_S0S1,
                     NRF_GPIO_PIN_NOSENSE);
        mosi_pin = p_config->mosi_pin;
    }
    else
    {
        mosi_pin = NRF_SPIS_PIN_NOT_CONNECTED;
    }
    
    nrf_gpio_cfg(p_config->csn_pin,
                 NRF_GPIO_PIN_DIR_INPUT,
                 NRF_GPIO_PIN_INPUT_CONNECT,
                 p_config->csn_pullup,
                 NRF_GPIO_PIN_S0S1,
                 NRF_GPIO_PIN_NOSENSE);
    
    nrf_gpio_cfg(p_config->sck_pin,
                 NRF_GPIO_PIN_DIR_INPUT,
                 NRF_GPIO_PIN_INPUT_CONNECT,
                 NRF_GPIO_PIN_NOPULL,
                 NRF_GPIO_PIN_S0S1,
                 NRF_GPIO_PIN_NOSENSE);

    nrf_spis_pins_set(p_spis, p_config->sck_pin, mosi_pin, miso_pin, p_config->csn_pin);
    
    nrf_spis_rx_buffer_set(p_spis, NULL, 0);
    nrf_spis_tx_buffer_set(p_spis, NULL, 0);
    
    // Configure SPI mode.
    nrf_spis_configure(p_spis, (nrf_spis_mode_t) p_config->mode,
                               (nrf_spis_bit_order_t) p_config->bit_order);
    
    // Configure DEF and ORC characters.
    nrf_spis_def_set(p_spis, p_config->def);
    nrf_spis_orc_set(p_spis, p_config->orc);
    
    // Clear possible pending events.
    nrf_spis_event_clear(p_spis, NRF_SPIS_EVENT_END);
    nrf_spis_event_clear(p_spis, NRF_SPIS_EVENT_ACQUIRED);
    
    // Enable END_ACQUIRE shortcut.        
    nrf_spis_shorts_enable(p_spis, NRF_SPIS_SHORT_END_ACQUIRE);
    
    m_cb[p_instance->instance_id].spi_state = SPIS_STATE_INIT;
    m_cb[p_instance->instance_id].handler = event_handler;

    
    // Enable IRQ.
    nrf_spis_int_enable(p_spis, NRF_SPIS_INT_ACQUIRED_MASK | NRF_SPIS_INT_END_MASK);
    nrf_drv_common_irq_enable(p_instance->irq, p_config->irq_priority);
    
    p_cb->state = NRF_DRV_STATE_INITIALIZED;
    
    // Enable SPI slave device.        
    nrf_spis_enable(p_spis);
    
    return NRF_SUCCESS;
}


void nrf_drv_spis_uninit(nrf_drv_spis_t const * const  p_instance)
{
    spis_cb_t * p_cb = &m_cb[p_instance->instance_id];
    ASSERT(p_cb->state != NRF_DRV_STATE_UNINITIALIZED);
    
    NRF_SPIS_Type * p_spis = p_instance->p_reg;

    #define DISABLE_ALL 0xFFFFFFFF
    nrf_spis_disable(p_spis);
    nrf_drv_common_irq_disable(p_instance->irq);
    nrf_spis_int_disable(p_spis, DISABLE_ALL);
    #undef  DISABLE_ALL

#if PERIPHERAL_RESOURCE_SHARING_ENABLED
    nrf_drv_common_per_res_release(p_spis);
#endif

    p_cb->state = NRF_DRV_STATE_UNINITIALIZED;
}


/**@brief Function for executing the state entry action. */
static void spis_state_entry_action_execute(NRF_SPIS_Type * p_spis,
                                                     spis_cb_t * p_cb)
{
    nrf_drv_spis_event_t event;
    
    switch (p_cb->spi_state)
    {                             
        case SPIS_BUFFER_RESOURCE_REQUESTED:
            nrf_spis_task_trigger(p_spis, NRF_SPIS_TASK_ACQUIRE);
            break;            
     
        case SPIS_BUFFER_RESOURCE_CONFIGURED:
            event.evt_type  = NRF_DRV_SPIS_BUFFERS_SET_DONE;
            event.rx_amount = 0;
            event.tx_amount = 0;     
            
            APP_ERROR_CHECK_BOOL(p_cb->handler != NULL);
            p_cb->handler(event);
            break;
            
        case SPIS_XFER_COMPLETED:        
            event.evt_type  = NRF_DRV_SPIS_XFER_DONE;
            event.rx_amount = nrf_spis_rx_amount_get(p_spis);
            event.tx_amount = nrf_spis_tx_amount_get(p_spis);
            APP_ERROR_CHECK_BOOL(p_cb->handler != NULL);
            p_cb->handler(event);
            break;
            
        default:
            // No implementation required.            
            break;
    }
}

/**@brief Function for changing the state of the SPI state machine.
 *
 * @param[in] p_spis    SPIS instance register.
 * @param[in] p_cb      SPIS instance control block.
 * @param[in] new_state State where the state machine transits to.
 */
static void spis_state_change(NRF_SPIS_Type * p_spis,
                              spis_cb_t * p_cb,
                              nrf_drv_spis_state_t new_state)
{
    p_cb->spi_state = new_state;
    spis_state_entry_action_execute(p_spis, p_cb);
}


ret_code_t nrf_drv_spis_buffers_set(nrf_drv_spis_t const * const  p_instance,
                                    const uint8_t * p_tx_buffer,
                                    uint8_t   tx_buffer_length,
                                    uint8_t * p_rx_buffer,
                                    uint8_t   rx_buffer_length)
{
    spis_cb_t * p_cb = &m_cb[p_instance->instance_id];
    uint32_t err_code;

    VERIFY_PARAM_NOT_NULL(p_rx_buffer);
    VERIFY_PARAM_NOT_NULL(p_tx_buffer);

    // EasyDMA requires that transfer buffers are placed in Data RAM region;
    // signal error if they are not.
    if ((p_tx_buffer != NULL && !nrf_drv_is_in_RAM(p_tx_buffer)) ||
        (p_rx_buffer != NULL && !nrf_drv_is_in_RAM(p_rx_buffer)))
    {
        return NRF_ERROR_INVALID_ADDR;
    }
    
    switch (p_cb->spi_state)
    {
        case SPIS_STATE_INIT:
        case SPIS_XFER_COMPLETED:
        case SPIS_BUFFER_RESOURCE_CONFIGURED:        
            p_cb->tx_buffer      = p_tx_buffer;
            p_cb->rx_buffer      = p_rx_buffer;
            p_cb->tx_buffer_size = tx_buffer_length;
            p_cb->rx_buffer_size = rx_buffer_length;        
            err_code             = NRF_SUCCESS;            
                        
            spis_state_change(p_instance->p_reg, p_cb, SPIS_BUFFER_RESOURCE_REQUESTED);             
            break;

        case SPIS_BUFFER_RESOURCE_REQUESTED:
            err_code = NRF_ERROR_INVALID_STATE; 
            break;
        
        default:
            // @note: execution of this code path would imply internal error in the design.
            err_code = NRF_ERROR_INTERNAL;             
            break;
    }
    
    return err_code;
}

static void spis_irq_handler(NRF_SPIS_Type * p_spis, spis_cb_t * p_cb)
{
    // @note: as multiple events can be pending for processing, the correct event processing order 
    // is as follows:
    // - SPI semaphore acquired event.
    // - SPI transaction complete event.
    
    // Check for SPI semaphore acquired event.
    if (nrf_spis_event_check(p_spis, NRF_SPIS_EVENT_ACQUIRED))
    {
        nrf_spis_event_clear(p_spis, NRF_SPIS_EVENT_ACQUIRED);
        
        switch (p_cb->spi_state)
        {                
            case SPIS_BUFFER_RESOURCE_REQUESTED:     
                nrf_spis_tx_buffer_set(p_spis, (uint8_t *)p_cb->tx_buffer, p_cb->tx_buffer_size);
                nrf_spis_rx_buffer_set(p_spis, (uint8_t *)p_cb->rx_buffer, p_cb->rx_buffer_size);
                
                nrf_spis_task_trigger(p_spis, NRF_SPIS_TASK_RELEASE);
                
                spis_state_change(p_spis, p_cb, SPIS_BUFFER_RESOURCE_CONFIGURED);                                                                       
                break;
                
            default:
                // No implementation required.
                break;
        }
    }

    // Check for SPI transaction complete event.
    if (nrf_spis_event_check(p_spis, NRF_SPIS_EVENT_END))
    {
        nrf_spis_event_clear(p_spis, NRF_SPIS_EVENT_END);
        
        switch (p_cb->spi_state)
        {
            case SPIS_BUFFER_RESOURCE_CONFIGURED:    
                spis_state_change(p_spis, p_cb, SPIS_XFER_COMPLETED);
                break;

            default:
                // No implementation required.                    
                break;                
        }    
    }
}

#if SPIS0_ENABLED
    SPIS_IRQHANDLER_TEMPLATE(0)
#endif

#if SPIS1_ENABLED
    SPIS_IRQHANDLER_TEMPLATE(1)
#endif

#if SPIS2_ENABLED
    SPIS_IRQHANDLER_TEMPLATE(2)
#endif

#endif // SPI_COUNT > 0
