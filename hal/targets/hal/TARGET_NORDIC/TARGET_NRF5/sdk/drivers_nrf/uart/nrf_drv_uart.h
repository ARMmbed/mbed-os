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

/**@file
 * @addtogroup nrf_uart UART driver and HAL
 * @ingroup nrf_drivers
 * @brief UART API.
 * @details The UART driver provides APIs for utilizing the UART peripheral.
 *
 * @defgroup nrf_drv_uart UART driver
 * @{
 * @ingroup  nrf_uart
 *
 * @brief    UART driver.
 */

#ifndef NRF_DRV_UART_H
#define NRF_DRV_UART_H

#include "nrf_uart.h"
#ifdef NRF52
#include "nrf_uarte.h"
#endif

#include "sdk_errors.h"
#include "nrf_drv_config.h"

/**
 * @brief Types of UART driver events.
 */
typedef enum
{
    NRF_DRV_UART_EVT_TX_DONE, ///< Requested TX transfer completed.
    NRF_DRV_UART_EVT_RX_DONE, ///< Requested RX transfer completed.
    NRF_DRV_UART_EVT_ERROR,   ///< Error reported by UART peripheral.
} nrf_drv_uart_evt_type_t;

/**@brief Structure for UART configuration. */
typedef struct
{
    uint32_t            pseltxd;            ///< TXD pin number.
    uint32_t            pselrxd;            ///< RXD pin number.
    uint32_t            pselcts;            ///< CTS pin number.
    uint32_t            pselrts;            ///< RTS pin number.
    void *              p_context;          ///< Context passed to interrupt handler.
    nrf_uart_hwfc_t     hwfc;               ///< Flow control configuration.
    nrf_uart_parity_t   parity;             ///< Parity configuration.
    nrf_uart_baudrate_t baudrate;           ///< Baudrate.
    uint8_t             interrupt_priority; ///< Interrupt priority.
#ifdef NRF52
    bool                use_easy_dma;
#endif
} nrf_drv_uart_config_t;

/**@brief UART default configuration. */
#ifdef NRF52
#if !UART_LEGACY_SUPPORT
#define DEFAULT_CONFIG_USE_EASY_DMA true
#elif !UART_EASY_DMA_SUPPORT
#define DEFAULT_CONFIG_USE_EASY_DMA false
#else
#define DEFAULT_CONFIG_USE_EASY_DMA UART0_CONFIG_USE_EASY_DMA
#endif
#define NRF_DRV_UART_DEFAULT_CONFIG                                                   \
    {                                                                                 \
        .pseltxd            = UART0_CONFIG_PSEL_TXD,                                  \
        .pselrxd            = UART0_CONFIG_PSEL_RXD,                                  \
        .pselcts            = UART0_CONFIG_PSEL_CTS,                                  \
        .pselrts            = UART0_CONFIG_PSEL_RTS,                                  \
        .p_context          = NULL,                                                   \
        .hwfc               = UART0_CONFIG_HWFC,                                      \
        .parity             = UART0_CONFIG_PARITY,                                    \
        .baudrate           = UART0_CONFIG_BAUDRATE,                                  \
        .interrupt_priority = UART0_CONFIG_IRQ_PRIORITY,                              \
        .use_easy_dma       = DEFAULT_CONFIG_USE_EASY_DMA                             \
    }
#else
#define NRF_DRV_UART_DEFAULT_CONFIG                                                   \
    {                                                                                 \
        .pseltxd            = UART0_CONFIG_PSEL_TXD,                                  \
        .pselrxd            = UART0_CONFIG_PSEL_RXD,                                  \
        .pselcts            = UART0_CONFIG_PSEL_CTS,                                  \
        .pselrts            = UART0_CONFIG_PSEL_RTS,                                  \
        .p_context          = NULL,                                                   \
        .hwfc               = UART0_CONFIG_HWFC,                                      \
        .parity             = UART0_CONFIG_PARITY,                                    \
        .baudrate           = UART0_CONFIG_BAUDRATE,                                  \
        .interrupt_priority = UART0_CONFIG_IRQ_PRIORITY                               \
    }
#endif

/**@brief Structure for UART transfer completion event. */
typedef struct
{
    uint8_t * p_data; ///< Pointer to memory used for transfer.
    uint8_t   bytes;  ///< Number of bytes transfered.
} nrf_drv_uart_xfer_evt_t;

/**@brief Structure for UART error event. */
typedef struct
{
    nrf_drv_uart_xfer_evt_t rxtx;      ///< Transfer details includes number of bytes transfered.
    uint32_t                error_mask;///< Mask of error flags that generated the event.
} nrf_drv_uart_error_evt_t;

/**@brief Structure for UART event. */
typedef struct
{
    nrf_drv_uart_evt_type_t type;      ///< Event type.
    union
    {
        nrf_drv_uart_xfer_evt_t  rxtx; ///< Data provided for transfer completion events.
        nrf_drv_uart_error_evt_t error;///< Data provided for error event.
    } data;
} nrf_drv_uart_event_t;

/**
 * @brief UART interrupt event handler.
 *
 * @param[in] p_event     Pointer to event structure. Event is allocated on the stack so it is available
 *                        only within the context of the event handler.
 * @param[in] p_context   Context passed to interrupt handler, set on initialization.
 */
typedef void (*nrf_uart_event_handler_t)(nrf_drv_uart_event_t * p_event, void * p_context);

/**
 * @brief Function for initializing the UART driver.
 *
 * This function configures and enables UART. After this function GPIO pins are controlled by UART.
 *
 * @param[in] p_config       Initial configuration. Default configuration used if NULL.
 * @param[in] event_handler  Event handler provided by the user. If not provided driver works in
 *                           blocking mode.
 *
 * @retval    NRF_SUCCESS             If initialization was successful.
 * @retval    NRF_ERROR_INVALID_STATE If driver is already initialized.
 */
ret_code_t nrf_drv_uart_init(nrf_drv_uart_config_t const * p_config,
                             nrf_uart_event_handler_t      event_handler);

/**
 * @brief Function for uninitializing  the UART driver.
 */
void nrf_drv_uart_uninit(void);

/**
 * @brief Function for getting the address of a specific UART task.
 *
 * @param[in] task Task.
 *
 * @return    Task address.
 */
__STATIC_INLINE uint32_t nrf_drv_uart_task_address_get(nrf_uart_task_t task);

/**
 * @brief Function for getting the address of a specific UART event.
 *
 * @param[in] event Event.
 *
 * @return    Event address.
 */
__STATIC_INLINE uint32_t nrf_drv_uart_event_address_get(nrf_uart_event_t event);

/**
 * @brief Function for sending data over UART.
 *
 * If an event handler was provided in nrf_drv_uart_init() call, this function
 * returns immediately and the handler is called when the transfer is done.
 * Otherwise, the transfer is performed in blocking mode, i.e. this function
 * returns when the transfer is finished. Blocking mode is not using interrupt so
 * there is no context switching inside the function.
 *
 * @note Peripherals using EasyDMA (i.e. UARTE) require that the transfer buffers
 *       are placed in the Data RAM region. If they are not and UARTE instance is
 *       used, this function will fail with error code NRF_ERROR_INVALID_ADDR.
 *
 * @param[in] p_data Pointer to data.
 * @param[in] length Number of bytes to send.
 *
 * @retval    NRF_SUCCESS            If initialization was successful.
 * @retval    NRF_ERROR_BUSY         If driver is already transferring.
 * @retval    NRF_ERROR_FORBIDDEN    If the transfer was aborted from a different context
 *                                   (blocking mode only, also see @ref nrf_drv_uart_rx_disable).
 * @retval    NRF_ERROR_INVALID_ADDR If p_data does not point to RAM buffer (UARTE only).
 */
ret_code_t nrf_drv_uart_tx(uint8_t const * const p_data, uint8_t length);

/**
 * @brief Function for checking if UART is currently transmitting.
 *
 * @retval true  If UART is transmitting.
 * @retval false If UART is not transmitting.
 */
bool nrf_drv_uart_tx_in_progress(void);

/**
 * @brief Function for aborting any ongoing transmission.
 * @note @ref NRF_DRV_UART_EVT_TX_DONE event will be generated in non-blocking mode. Event will
 *       contain number of bytes sent until abort was called. If Easy DMA is not used event will be
 *       called from the function context. If Easy DMA is used it will be called from UART interrupt
 *       context.
 */
void nrf_drv_uart_tx_abort(void);

/**
 * @brief Function for receiving data over UART.
 *
 * If an event handler was provided in the nrf_drv_uart_init() call, this function
 * returns immediately and the handler is called when the transfer is done.
 * Otherwise, the transfer is performed in blocking mode, i.e. this function
 * returns when the transfer is finished. Blocking mode is not using interrupt so
 * there is no context switching inside the function.
 * The receive buffer pointer is double buffered in non-blocking mode. The secondary
 * buffer can be set immediately after starting the transfer and will be filled
 * when the primary buffer is full. The double buffering feature allows 
 * receiving data continuously.
 *
 * @note Peripherals using EasyDMA (i.e. UARTE) require that the transfer buffers
 *       are placed in the Data RAM region. If they are not and UARTE instance is
 *       used, this function will fail with error code NRF_ERROR_INVALID_ADDR.
 * @param[in] p_data Pointer to data.
 * @param[in] length Number of bytes to receive.
 *
 * @retval    NRF_SUCCESS If initialization was successful.
 * @retval    NRF_ERROR_BUSY If the driver is already receiving
 *                           (and the secondary buffer has already been set
 *                           in non-blocking mode).
 * @retval    NRF_ERROR_FORBIDDEN If the transfer was aborted from a different context
 *                               (blocking mode only, also see @ref nrf_drv_uart_rx_disable).
 * @retval    NRF_ERROR_INTERNAL If UART peripheral reported an error.
 * @retval    NRF_ERROR_INVALID_ADDR If p_data does not point to RAM buffer (UARTE only).
 */
ret_code_t nrf_drv_uart_rx(uint8_t * p_data, uint8_t length);

/**
 * @brief Function for enabling receiver.
 *
 * UART has 6 byte long RX FIFO and it will be used to store incoming data. If user will not call
 * UART receive function before FIFO is filled, overrun error will encounter. Enabling receiver
 * without specifying RX buffer is supported only in UART mode (without Easy DMA). Receiver must be
 * explicitly closed by the user @sa nrf_drv_uart_rx_disable. Function asserts if mode is wrong.
 */
void nrf_drv_uart_rx_enable(void);

/**
 * @brief Function for disabling receiver.
 *
 * Function must be called to close the receiver after it has been explicitly enabled by
 * @sa nrf_drv_uart_rx_enable. Feature is supported only in UART mode (without Easy DMA). Function
 * asserts if mode is wrong.
 */
void nrf_drv_uart_rx_disable(void);

/**
 * @brief Function for aborting any ongoing reception.
 * @note @ref NRF_DRV_UART_EVT_RX_DONE event will be generated in non-blocking mode. Event will
 *       contain number of bytes received until abort was called. If Easy DMA is not used event will be
 *       called from the function context. If Easy DMA is used it will be called from UART interrupt
 *       context.
 */
void nrf_drv_uart_rx_abort(void);

/**
 * @brief Function for reading error source mask. Mask contains values from @ref nrf_uart_error_mask_t.
 * @note Function should be used in blocking mode only. In case of non-blocking mode error event is
 *       generated. Function clears error sources after reading.
 *
 * @retval    Mask of reported errors.
 */
uint32_t nrf_drv_uart_errorsrc_get(void);

#ifndef SUPPRESS_INLINE_IMPLEMENTATION
__STATIC_INLINE uint32_t nrf_drv_uart_task_address_get(nrf_uart_task_t task)
{
    return nrf_uart_task_address_get(NRF_UART0, task);
}

__STATIC_INLINE uint32_t nrf_drv_uart_event_address_get(nrf_uart_event_t event)
{
    return nrf_uart_event_address_get(NRF_UART0, event);
}
#endif //SUPPRESS_INLINE_IMPLEMENTATION
#endif //NRF_DRV_UART_H
/** @} */
