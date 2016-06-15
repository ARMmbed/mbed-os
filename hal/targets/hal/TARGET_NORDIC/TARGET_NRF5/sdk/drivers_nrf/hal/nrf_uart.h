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
#ifndef NRF_UART_H__
#define NRF_UART_H__

#include "nrf.h"
#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>

/**
 * @defgroup nrf_uart_hal UART HAL
 * @{
 * @ingroup nrf_uart
 *
 * @brief Hardware access layer for accessing the UART peripheral.
 */

#define NRF_UART_PSEL_DISCONNECTED 0xFFFFFFFF

/**
 * @enum nrf_uart_task_t
 * @brief UART tasks.
 */
typedef enum
{
    /*lint -save -e30 -esym(628,__INTADDR__)*/
    NRF_UART_TASK_STARTRX = offsetof(NRF_UART_Type, TASKS_STARTRX), /**< Task for starting reception. */
    NRF_UART_TASK_STOPRX  = offsetof(NRF_UART_Type, TASKS_STOPRX),  /**< Task for stopping reception. */
    NRF_UART_TASK_STARTTX = offsetof(NRF_UART_Type, TASKS_STARTTX), /**< Task for starting transmission. */
    NRF_UART_TASK_STOPTX  = offsetof(NRF_UART_Type, TASKS_STOPTX),  /**< Task for stopping transmission. */
    NRF_UART_TASK_SUSPEND = offsetof(NRF_UART_Type, TASKS_SUSPEND), /**< Task for suspending UART. */
    /*lint -restore*/
} nrf_uart_task_t;

/**
 * @enum nrf_uart_event_t
 * @brief UART events.
 */
typedef enum
{
    /*lint -save -e30*/
    NRF_UART_EVENT_CTS    = offsetof(NRF_UART_Type, EVENTS_CTS),   /**< Event from CTS line activation. */
    NRF_UART_EVENT_NCTS   = offsetof(NRF_UART_Type, EVENTS_NCTS),  /**< Event from CTS line deactivation. */
    NRF_UART_EVENT_RXDRDY = offsetof(NRF_UART_Type, EVENTS_RXDRDY),/**< Event from data ready in RXD. */
    NRF_UART_EVENT_TXDRDY = offsetof(NRF_UART_Type, EVENTS_TXDRDY),/**< Event from data sent from TXD. */
    NRF_UART_EVENT_ERROR  = offsetof(NRF_UART_Type, EVENTS_ERROR), /**< Event from error detection. */
    NRF_UART_EVENT_RXTO   = offsetof(NRF_UART_Type, EVENTS_RXTO)   /**< Event from receiver timeout. */
    /*lint -restore*/
} nrf_uart_event_t;

/**
 * @enum nrf_uart_int_mask_t
 * @brief UART interrupts.
 */
typedef enum
{
    /*lint -save -e30*/
    NRF_UART_INT_MASK_CTS    = UART_INTENCLR_CTS_Msk,    /**< CTS line activation interrupt. */
    NRF_UART_INT_MASK_NCTS   = UART_INTENCLR_NCTS_Msk,   /**< CTS line deactivation interrupt. */
    NRF_UART_INT_MASK_RXDRDY = UART_INTENCLR_RXDRDY_Msk, /**< Data ready in RXD interrupt. */
    NRF_UART_INT_MASK_TXDRDY = UART_INTENCLR_TXDRDY_Msk,  /**< Data sent from TXD interrupt. */
    NRF_UART_INT_MASK_ERROR  = UART_INTENCLR_ERROR_Msk,  /**< Error detection interrupt. */
    NRF_UART_INT_MASK_RXTO   = UART_INTENCLR_RXTO_Msk    /**< Receiver timeout interrupt. */
    /*lint -restore*/
} nrf_uart_int_mask_t;

/**
 * @enum nrf_uart_baudrate_t
 * @brief Baudrates supported by UART.
 */
typedef enum
{
#ifdef NRF52
    NRF_UART_BAUDRATE_1200   =  UARTE_BAUDRATE_BAUDRATE_Baud1200, /**< 1200 baud. */
    NRF_UART_BAUDRATE_2400   =  UARTE_BAUDRATE_BAUDRATE_Baud2400, /**< 2400 baud. */
    NRF_UART_BAUDRATE_4800   =  UARTE_BAUDRATE_BAUDRATE_Baud4800, /**< 4800 baud. */
    NRF_UART_BAUDRATE_9600   =  UARTE_BAUDRATE_BAUDRATE_Baud9600, /**< 9600 baud. */
    NRF_UART_BAUDRATE_14400  =  UARTE_BAUDRATE_BAUDRATE_Baud14400, /**< 14400 baud. */
    NRF_UART_BAUDRATE_19200  =  UARTE_BAUDRATE_BAUDRATE_Baud19200, /**< 19200 baud. */
    NRF_UART_BAUDRATE_28800  =  UARTE_BAUDRATE_BAUDRATE_Baud28800, /**< 28800 baud. */
    NRF_UART_BAUDRATE_38400  =  UARTE_BAUDRATE_BAUDRATE_Baud38400, /**< 38400 baud. */
    NRF_UART_BAUDRATE_57600  =  UARTE_BAUDRATE_BAUDRATE_Baud57600, /**< 57600 baud. */
    NRF_UART_BAUDRATE_76800  =  UARTE_BAUDRATE_BAUDRATE_Baud76800, /**< 76800 baud. */
    NRF_UART_BAUDRATE_115200 =  UARTE_BAUDRATE_BAUDRATE_Baud115200, /**< 115200 baud. */
    NRF_UART_BAUDRATE_230400 =  UARTE_BAUDRATE_BAUDRATE_Baud230400, /**< 230400 baud. */
    NRF_UART_BAUDRATE_250000 =  UARTE_BAUDRATE_BAUDRATE_Baud250000, /**< 250000 baud. */
    NRF_UART_BAUDRATE_460800 =  UARTE_BAUDRATE_BAUDRATE_Baud460800, /**< 460800 baud. */
    NRF_UART_BAUDRATE_921600 =  UARTE_BAUDRATE_BAUDRATE_Baud921600, /**< 921600 baud. */
    NRF_UART_BAUDRATE_1000000 =  UARTE_BAUDRATE_BAUDRATE_Baud1M, /**< 1000000 baud. */
#else
    NRF_UART_BAUDRATE_1200   =  UART_BAUDRATE_BAUDRATE_Baud1200, /**< 1200 baud. */
    NRF_UART_BAUDRATE_2400   =  UART_BAUDRATE_BAUDRATE_Baud2400, /**< 2400 baud. */
    NRF_UART_BAUDRATE_4800   =  UART_BAUDRATE_BAUDRATE_Baud4800, /**< 4800 baud. */
    NRF_UART_BAUDRATE_9600   =  UART_BAUDRATE_BAUDRATE_Baud9600, /**< 9600 baud. */
    NRF_UART_BAUDRATE_14400  =  UART_BAUDRATE_BAUDRATE_Baud14400, /**< 14400 baud. */
    NRF_UART_BAUDRATE_19200  =  UART_BAUDRATE_BAUDRATE_Baud19200, /**< 19200 baud. */
    NRF_UART_BAUDRATE_28800  =  UART_BAUDRATE_BAUDRATE_Baud28800, /**< 28800 baud. */
    NRF_UART_BAUDRATE_38400  =  UART_BAUDRATE_BAUDRATE_Baud38400, /**< 38400 baud. */
    NRF_UART_BAUDRATE_57600  =  UART_BAUDRATE_BAUDRATE_Baud57600, /**< 57600 baud. */
    NRF_UART_BAUDRATE_76800  =  UART_BAUDRATE_BAUDRATE_Baud76800, /**< 76800 baud. */
    NRF_UART_BAUDRATE_115200 =  UART_BAUDRATE_BAUDRATE_Baud115200, /**< 115200 baud. */
    NRF_UART_BAUDRATE_230400 =  UART_BAUDRATE_BAUDRATE_Baud230400, /**< 230400 baud. */
    NRF_UART_BAUDRATE_250000 =  UART_BAUDRATE_BAUDRATE_Baud250000, /**< 250000 baud. */
    NRF_UART_BAUDRATE_460800 =  UART_BAUDRATE_BAUDRATE_Baud460800, /**< 460800 baud. */
    NRF_UART_BAUDRATE_921600 =  UART_BAUDRATE_BAUDRATE_Baud921600, /**< 921600 baud. */
    NRF_UART_BAUDRATE_1000000 =  UART_BAUDRATE_BAUDRATE_Baud1M, /**< 1000000 baud. */
#endif
} nrf_uart_baudrate_t;

/**
 * @enum nrf_uart_error_mask_t
 * @brief Types of UART error masks.
 */
typedef enum
{
    NRF_UART_ERROR_OVERRUN_MASK = UART_ERRORSRC_OVERRUN_Msk,   /**< Overrun error. */
    NRF_UART_ERROR_PARITY_MASK  = UART_ERRORSRC_PARITY_Msk,    /**< Parity error. */
    NRF_UART_ERROR_FRAMING_MASK = UART_ERRORSRC_FRAMING_Msk,   /**< Framing error. */
    NRF_UART_ERROR_BREAK_MASK   = UART_ERRORSRC_BREAK_Msk,     /**< Break error. */
} nrf_uart_error_mask_t;

/**
 * @enum nrf_uart_parity_t
 * @brief Types of UART parity modes.
 */
typedef enum
{
    NRF_UART_PARITY_EXCLUDED = UART_CONFIG_PARITY_Excluded << UART_CONFIG_PARITY_Pos, /**< Parity excluded. */
    NRF_UART_PARITY_INCLUDED = UART_CONFIG_PARITY_Included << UART_CONFIG_PARITY_Pos, /**< Parity included. */
} nrf_uart_parity_t;

/**
 * @enum nrf_uart_hwfc_t
 * @brief Types of UART flow control modes.
 */
typedef enum
{
    NRF_UART_HWFC_DISABLED = UART_CONFIG_HWFC_Disabled, /**< HW flow control disabled. */
    NRF_UART_HWFC_ENABLED  = UART_CONFIG_HWFC_Enabled,  /**< HW flow control enabled. */
} nrf_uart_hwfc_t;

/**
 * @brief Function for clearing a specific UART event.
 *
 * @param[in] p_reg  UART instance.
 * @param[in] event  Event to clear.
 */
__STATIC_INLINE void nrf_uart_event_clear(NRF_UART_Type * p_reg, nrf_uart_event_t event);

/**
 * @brief Function for checking the state of a specific UART event.
 *
 * @param[in] p_reg  UART instance.
 * @param[in] event  Event to check.
 *
 * @retval True if event is set, False otherwise.
 */
__STATIC_INLINE bool nrf_uart_event_check(NRF_UART_Type * p_reg, nrf_uart_event_t event);

/**
 * @brief Function for returning the address of a specific UART event register.
 *
 * @param[in] p_reg  UART instance.
 * @param[in] event  Desired event.
 *
 * @retval Address of specified event register.
 */
__STATIC_INLINE uint32_t nrf_uart_event_address_get(NRF_UART_Type  * p_reg,
                                                    nrf_uart_event_t  event);

/**
 * @brief Function for enabling a specific interrupt.
 *
 * @param p_reg     Instance.
 * @param int_mask Interrupts to enable.
 */
__STATIC_INLINE void nrf_uart_int_enable(NRF_UART_Type * p_reg, uint32_t int_mask);

/**
 * @brief Function for retrieving the state of a given interrupt.
 *
 * @param p_reg     Instance.
 * @param int_mask  Mask of interrupt to check.
 *
 * @retval true  If the interrupt is enabled.
 * @retval false If the interrupt is not enabled.
 */
__STATIC_INLINE bool nrf_uart_int_enable_check(NRF_UART_Type * p_reg, uint32_t int_mask);

/**
 * @brief Function for disabling specific interrupts.
 *
 * @param p_reg    Instance.
 * @param int_mask Interrupts to disable.
 */
__STATIC_INLINE void nrf_uart_int_disable(NRF_UART_Type * p_reg, uint32_t int_mask);

/**
 * @brief Function for getting error source mask. Function is clearing error source flags after reading.
 *
 * @param p_reg    Instance.
 * @return         Mask with error source flags.
 */
__STATIC_INLINE uint32_t nrf_uart_errorsrc_get_and_clear(NRF_UART_Type * p_reg);

/**
 * @brief Function for enabling UART.
 *
 * @param p_reg    Instance.
 */
__STATIC_INLINE void nrf_uart_enable(NRF_UART_Type * p_reg);

/**
 * @brief Function for disabling UART.
 *
 * @param p_reg    Instance.
 */
__STATIC_INLINE void nrf_uart_disable(NRF_UART_Type * p_reg);

/**
 * @brief Function for configuring TX/RX pins.
 *
 * @param p_reg    Instance.
 * @param pseltxd  TXD pin number.
 * @param pselrxd  RXD pin number.
 */
__STATIC_INLINE void nrf_uart_txrx_pins_set(NRF_UART_Type * p_reg, uint32_t pseltxd, uint32_t pselrxd);

/**
 * @brief Function for disconnecting TX/RX pins.
 *
 * @param p_reg    Instance.
 */
__STATIC_INLINE void nrf_uart_txrx_pins_disconnect(NRF_UART_Type * p_reg);

/**
 * @brief Function for getting TX pin.
 *
 * @param p_reg    Instance.
 */
__STATIC_INLINE uint32_t nrf_uart_tx_pin_get(NRF_UART_Type * p_reg);

/**
 * @brief Function for getting RX pin.
 *
 * @param p_reg    Instance.
 */
__STATIC_INLINE uint32_t nrf_uart_rx_pin_get(NRF_UART_Type * p_reg);

/**
 * @brief Function for getting RTS pin.
 *
 * @param p_reg    Instance.
 */
__STATIC_INLINE uint32_t nrf_uart_rts_pin_get(NRF_UART_Type * p_reg);

/**
 * @brief Function for getting CTS pin.
 *
 * @param p_reg    Instance.
 */
__STATIC_INLINE uint32_t nrf_uart_cts_pin_get(NRF_UART_Type * p_reg);


/**
 * @brief Function for configuring flow control pins.
 *
 * @param p_reg    Instance.
 * @param pselrts  RTS pin number.
 * @param pselcts  CTS pin number.
 */
__STATIC_INLINE void nrf_uart_hwfc_pins_set(NRF_UART_Type * p_reg,
                                            uint32_t        pselrts,
                                            uint32_t        pselcts);

/**
 * @brief Function for disconnecting flow control pins.
 *
 * @param p_reg    Instance.
 */
__STATIC_INLINE void nrf_uart_hwfc_pins_disconnect(NRF_UART_Type * p_reg);

/**
 * @brief Function for reading RX data.
 *
 * @param p_reg    Instance.
 * @return         Received byte.
 */
__STATIC_INLINE uint8_t nrf_uart_rxd_get(NRF_UART_Type * p_reg);

/**
 * @brief Function for setting Tx data.
 *
 * @param p_reg    Instance.
 * @param txd      Byte.
 */
__STATIC_INLINE void nrf_uart_txd_set(NRF_UART_Type * p_reg, uint8_t txd);

/**
 * @brief Function for starting an UART task.
 *
 * @param p_reg    Instance.
 * @param task     Task.
 */
__STATIC_INLINE void nrf_uart_task_trigger(NRF_UART_Type * p_reg, nrf_uart_task_t task);

/**
 * @brief Function for returning the address of a specific task register.
 *
 * @param p_reg Instance.
 * @param task  Task.
 *
 * @return      Task address.
 */
__STATIC_INLINE uint32_t nrf_uart_task_address_get(NRF_UART_Type * p_reg, nrf_uart_task_t task);

/**
 * @brief Function for configuring UART.
 *
 * @param p_reg  Instance.
 * @param hwfc   Hardware flow control. Enabled if true.
 * @param parity Parity. Included if true.
 */
__STATIC_INLINE void nrf_uart_configure(NRF_UART_Type   * p_reg,
                                            nrf_uart_parity_t parity,
                                            nrf_uart_hwfc_t   hwfc);

/**
 * @brief Function for setting UART baudrate.
 *
 * @param p_reg    Instance.
 * @param baudrate Baudrate.
 */
__STATIC_INLINE void nrf_uart_baudrate_set(NRF_UART_Type   * p_reg, nrf_uart_baudrate_t baudrate);

#ifndef SUPPRESS_INLINE_IMPLEMENTATION
__STATIC_INLINE void nrf_uart_event_clear(NRF_UART_Type * p_reg, nrf_uart_event_t event)
{
    *((volatile uint32_t *)((uint8_t *)p_reg + (uint32_t)event)) = 0x0UL;

}

__STATIC_INLINE bool nrf_uart_event_check(NRF_UART_Type * p_reg, nrf_uart_event_t event)
{
    return (bool)*(volatile uint32_t *)((uint8_t *)p_reg + (uint32_t)event);
}

__STATIC_INLINE uint32_t nrf_uart_event_address_get(NRF_UART_Type  * p_reg,
                                                    nrf_uart_event_t  event)
{
    return (uint32_t)((uint8_t *)p_reg + (uint32_t)event);
}

__STATIC_INLINE void nrf_uart_int_enable(NRF_UART_Type * p_reg, uint32_t int_mask)
{
    p_reg->INTENSET = int_mask;
}

__STATIC_INLINE bool nrf_uart_int_enable_check(NRF_UART_Type * p_reg, uint32_t int_mask)
{
    return (bool)(p_reg->INTENSET & int_mask);
}

__STATIC_INLINE void nrf_uart_int_disable(NRF_UART_Type * p_reg, uint32_t int_mask)
{
    p_reg->INTENCLR = int_mask;
}

__STATIC_INLINE uint32_t nrf_uart_errorsrc_get_and_clear(NRF_UART_Type * p_reg)
{
    uint32_t errsrc_mask = p_reg->ERRORSRC;
    p_reg->ERRORSRC = errsrc_mask;
    return errsrc_mask;
}

__STATIC_INLINE void nrf_uart_enable(NRF_UART_Type * p_reg)
{
    p_reg->ENABLE = UART_ENABLE_ENABLE_Enabled;
}

__STATIC_INLINE void nrf_uart_disable(NRF_UART_Type * p_reg)
{
    p_reg->ENABLE = UART_ENABLE_ENABLE_Disabled;
}

__STATIC_INLINE void nrf_uart_txrx_pins_set(NRF_UART_Type * p_reg, uint32_t pseltxd, uint32_t pselrxd)
{
    p_reg->PSELTXD = pseltxd;
    p_reg->PSELRXD = pselrxd;
}

__STATIC_INLINE void nrf_uart_txrx_pins_disconnect(NRF_UART_Type * p_reg)
{
    nrf_uart_txrx_pins_set(p_reg, NRF_UART_PSEL_DISCONNECTED, NRF_UART_PSEL_DISCONNECTED);
}

__STATIC_INLINE uint32_t nrf_uart_tx_pin_get(NRF_UART_Type * p_reg)
{
    return p_reg->PSELTXD;
}

__STATIC_INLINE uint32_t nrf_uart_rx_pin_get(NRF_UART_Type * p_reg)
{
    return p_reg->PSELRXD;
}

__STATIC_INLINE uint32_t nrf_uart_rts_pin_get(NRF_UART_Type * p_reg)
{
    return p_reg->PSELRTS;
}

__STATIC_INLINE uint32_t nrf_uart_cts_pin_get(NRF_UART_Type * p_reg)
{
    return p_reg->PSELCTS;
}

__STATIC_INLINE void nrf_uart_hwfc_pins_set(NRF_UART_Type * p_reg, uint32_t pselrts, uint32_t pselcts)
{
    p_reg->PSELRTS = pselrts;
    p_reg->PSELCTS = pselcts;
}

__STATIC_INLINE void nrf_uart_hwfc_pins_disconnect(NRF_UART_Type * p_reg)
{
    nrf_uart_hwfc_pins_set(p_reg, NRF_UART_PSEL_DISCONNECTED, NRF_UART_PSEL_DISCONNECTED);
}

__STATIC_INLINE uint8_t nrf_uart_rxd_get(NRF_UART_Type * p_reg)
{
    return p_reg->RXD;
}

__STATIC_INLINE void nrf_uart_txd_set(NRF_UART_Type * p_reg, uint8_t txd)
{
    p_reg->TXD = txd;
}

__STATIC_INLINE void nrf_uart_task_trigger(NRF_UART_Type * p_reg, nrf_uart_task_t task)
{
    *((volatile uint32_t *)((uint8_t *)p_reg + (uint32_t)task)) = 0x1UL;
}

__STATIC_INLINE uint32_t nrf_uart_task_address_get(NRF_UART_Type * p_reg, nrf_uart_task_t task)
{
    return (uint32_t)p_reg + (uint32_t)task;
}

__STATIC_INLINE void nrf_uart_configure(NRF_UART_Type   * p_reg,
                                            nrf_uart_parity_t parity,
                                            nrf_uart_hwfc_t   hwfc)
{
    p_reg->CONFIG = (uint32_t)parity | (uint32_t)hwfc;
}

__STATIC_INLINE void nrf_uart_baudrate_set(NRF_UART_Type   * p_reg, nrf_uart_baudrate_t baudrate)
{
    p_reg->BAUDRATE = baudrate;
}
#endif //SUPPRESS_INLINE_IMPLEMENTATION
/** @} */
#endif //NRF_UART_H__
