/**
 * @file    spimss.h
 * @brief   Serial Peripheral Interface (SPIMSS) function prototypes and data types.
 */ 

/* ****************************************************************************
 * Copyright (C) Maxim Integrated Products, Inc., All Rights Reserved.
 *
 * Permission is hereby granted, free of charge, to any person obtaining a
 * copy of this software and associated documentation files (the "Software"),
 * to deal in the Software without restriction, including without limitation
 * the rights to use, copy, modify, merge, publish, distribute, sublicense,
 * and/or sell copies of the Software, and to permit persons to whom the
 * Software is furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included
 * in all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS
 * OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
 * MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
 * IN NO EVENT SHALL MAXIM INTEGRATED BE LIABLE FOR ANY CLAIM, DAMAGES
 * OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE,
 * ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR
 * OTHER DEALINGS IN THE SOFTWARE.
 *
 * Except as contained in this notice, the name of Maxim Integrated
 * Products, Inc. shall not be used except as stated in the Maxim Integrated
 * Products, Inc. Branding Policy.
 *
 * The mere transfer of this software does not imply any licenses
 * of trade secrets, proprietary technology, copyrights, patents,
 * trademarks, maskwork rights, or any other form of intellectual
 * property whatsoever. Maxim Integrated Products, Inc. retains all
 * ownership rights.
 *
 *
 *************************************************************************** */
 
/* Define to prevent redundant inclusion */
#ifndef _SPIMSS_H_
#define _SPIMSS_H_

/* **** Includes **** */
#include "mxc_device.h"
#include "mxc_sys.h"
#include "mxc_pins.h"
#include "gpio.h"
#include "spimss_regs.h"

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @defgroup spimss SPIMSS
 * @ingroup spimss
 * @{
 */

/* **** Definitions **** */


/** 
 * @brief Enumeration type for setting the number data lines to use for communication.
 */
typedef enum {  // ONLY FOR COMPATIBILITY FOR CONSOLIDATION WITH SPY17, NOT USED OR NEEDED
    DUMMY_1,    /**< NOT USED                */
    DUMMY_2,    /**< NOT USED                */
    DUMMY_3,    /**< NOT USED                */
} mxc_spimss_width_t;

/**
 * @brief Structure type representing a SPI Master Transaction request.
 */
typedef struct mxc_spimss_req mxc_spimss_req_t;

/**
 * @brief Callback function type used in asynchronous SPI Master communication requests.
 * @details The function declaration for the SPI Master callback is:
 * @code 
 * void callback(spi_req_t * req, int error_code);
 * @endcode
 * |        |                                            |
 * | -----: | :----------------------------------------- |
 * | \p req |  Pointer to a #spi_req object representing the active SPI Master active transaction. |
 * | \p error_code | An error code if the active transaction had a failure or #E_NO_ERROR if successful. |
 * @note Callback will execute in interrupt context
 */
typedef void (*mxc_spimss_callback_fn)(mxc_spimss_req_t * req, int error_code);

/**
 * @brief      Structure definition for an SPI Master Transaction request.
 * @note       When using this structure for an asynchronous operation, the
 *             structure must remain allocated until the callback is completed.
 */
struct mxc_spimss_req {
    uint8_t                 ssel;       /**< Not Used*/
    uint8_t                 deass;      /**< Not Used*/
    const void              *tx_data;   /**< Pointer to a buffer to transmit data from. NULL if undesired. */
    void                    *rx_data;   /**< Pointer to a buffer to store data received. NULL if undesired.*/
    mxc_spimss_width_t      width;      /**< Not Used */
    unsigned                len;        /**< Number of transfer units to send from the \p tx_data buffer. */
    unsigned                bits;       /**< Number of bits in transfer unit (e.g. 8 for byte, 16 for short) */
    unsigned                rx_num;     /**< Number of bytes actually read into the \p rx_data buffer. */
    unsigned                tx_num;     /**< Number of bytes actually sent from the \p tx_data buffer */
    mxc_spimss_callback_fn  callback;   /**< Callback function if desired, NULL otherwise */
};

/* **** Function Prototypes **** */

/**
 * @brief     Initialize the spi.
 * @param     spi     Pointer to spi module to initialize.
 * @param     mode    SPI mode for clock phase and polarity.
 * @param     freq    Desired clock frequency.
 * @param     sys_cfg System configuration object
 * @param     drv_ssel 1 SSEL will be drive by driver
 *                     0 SSEL will NOT be drive by driver
 *
 * @return \c #E_NO_ERROR if successful, appropriate error otherwise
 */
int MXC_SPIMSS_Init(mxc_spimss_regs_t *spi, unsigned mode, unsigned freq, const sys_map_t sys_cfg, unsigned drv_ssel);

/**
 * @brief      Shutdown SPI module.
 * @param      spi  Pointer to SPI regs.
 * 
 * @return  \c #E_NO_ERROR if successful, appropriate error otherwise
 */
int MXC_SPIMSS_Shutdown(mxc_spimss_regs_t *spi);

/**
 * @brief     Execute a master transaction.
 * @param     spi   Pointer to spi module.
 * @param     req   Pointer to spi request
 * 
 * @return  \c #E_NO_ERROR if successful, @ref
 *             MXC_Error_Codes "error" if unsuccessful.
 */
int MXC_SPIMSS_MasterTrans(mxc_spimss_regs_t *spi, mxc_spimss_req_t *req);

/**
 * @brief      Execute SPI transaction based on interrupt handler
 * @param      spi   The spi
 *
 */
void MXC_SPIMSS_Handler(mxc_spimss_regs_t *spi);

/**
 * @brief     Execute a slave transaction.
 * @param     spi   Pointer to spi module.
 * @param     req   Pointer to spi request
 * 
 * @return  \c #E_NO_ERROR if successful, @ref
 *             MXC_Error_Codes "error" if unsuccessful.
 */
int MXC_SPIMSS_SlaveTrans(mxc_spimss_regs_t *spi, mxc_spimss_req_t *req);

/**
 * @brief      Asynchronously read/write SPI Master data
 *
 * @param      spi   Pointer to spi module
 * @param      req   Pointer to spi request
 *
 * @return  \c #E_NO_ERROR if successful, @ref
 *             MXC_Error_Codes "error" if unsuccessful.
 */
int MXC_SPIMSS_MasterTransAsync(mxc_spimss_regs_t *spi, mxc_spimss_req_t *req);

/**
 * @brief      Asynchronously read/write SPI Slave data
 *
 * @param      spi   Pointer to spi module
 * @param      req   Pointer to spi request
 *
 * @return  \c #E_NO_ERROR if successful, @ref
 *             MXC_Error_Codes "error" if unsuccessful.
 */
int MXC_SPIMSS_SlaveTransAsync(mxc_spimss_regs_t *spi, mxc_spimss_req_t *req);

/**
 * @brief   Sets the TX data to transmit as a 'dummy' byte
 *
 * In single wire master mode, this data is transmitted on MOSI when performing
 * an RX (MISO) only transaction. This defaults to 0.
 *
 * @param   spi             Pointer to SPI registers (selects the SPI block used.)
 * @param   defaultTXData   Data to shift out in RX-only transactions
 *
 * @return  Success/Fail, see \ref MXC_Error_Codes for a list of return codes.
 */
int MXC_SPIMSS_SetDefaultTXData (mxc_spimss_req_t* spi, unsigned int defaultTXData);

/**
 * @brief      Aborts an Asynchronous request
 *
 * @param      req   Pointer to spi request
 * @return  \c #E_NO_ERROR if successful, @ref
 *             MXC_Error_Codes "error" if unsuccessful.
 */
int MXC_SPIMSS_AbortAsync(mxc_spimss_req_t *req);

/**@} end of group spimss */

#ifdef __cplusplus
}
#endif

#endif /* _SPIMSS_H_ */
