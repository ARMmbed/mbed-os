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
 *************************************************************************** */

#include <stdio.h>
#include <stddef.h>
#include <stdint.h>
#include "mxc_errors.h"
#include "mxc_device.h"
#include "mxc_assert.h"
#include "mxc_lock.h"
#include "mxc_sys.h"
#include "mxc_delay.h"
#include "spimss_regs.h"
#include "spimss_reva_regs.h"
#include "spimss.h"

 /** 
 * @brief Enumeration type for setting the number data lines to use for communication.
 */
typedef enum {  // ONLY FOR COMPATIBILITY FOR CONSOLIDATION WITH SPY17, NOT USED OR NEEDED
    DUMMY_1_RevA,    /**< NOT USED                */
    DUMMY_2_RevA,    /**< NOT USED                */
    DUMMY_3_RevA,    /**< NOT USED                */
} spimss_reva_width_t;

/**
 * @brief Structure type representing a SPI Master Transaction request.
 */
typedef struct spimss_reva_req spimss_reva_req_t;

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
 * @addtogroup spi_async
 */
typedef void (*spimss_reva_callback_fn)(spimss_reva_req_t * req, int error_code);

/**
 * @brief      Structure definition for an SPI Master Transaction request.
 * @note       When using this structure for an asynchronous operation, the
 *             structure must remain allocated until the callback is completed.
 * @addtogroup spi_async
 */
struct spimss_reva_req {
    uint8_t            ssel;       /**< Not Used*/
    uint8_t            deass;      /**< Not Used*/
    const void         *tx_data;   /**< Pointer to a buffer to transmit data from. NULL if undesired. */
    void               *rx_data;   /**< Pointer to a buffer to store data received. NULL if undesired.*/
    spimss_reva_width_t     width;      /**< Not Used */
    unsigned           len;        /**< Number of transfer units to send from the \p tx_data buffer. */
    unsigned           bits;       /**< Number of bits in transfer unit (e.g. 8 for byte, 16 for short) */
    unsigned           rx_num;     /**< Number of bytes actually read into the \p rx_data buffer. */
    unsigned           tx_num;     /**< Number of bytes actually sent from the \p tx_data buffer */
    spimss_reva_callback_fn callback;   /**< Callback function if desired, NULL otherwise */
};


int MXC_SPIMSS_RevA_Init(mxc_spimss_reva_regs_t *spi, unsigned mode, unsigned freq, unsigned drv_ssel);
int MXC_SPIMSS_RevA_Shutdown(mxc_spimss_reva_regs_t *spi);
void MXC_SPIMSS_RevA_Handler(mxc_spimss_reva_regs_t *spi);
int MXC_SPIMSS_RevA_MasterTrans(mxc_spimss_reva_regs_t *spi, spimss_reva_req_t *req);
int MXC_SPIMSS_RevA_SlaveTrans(mxc_spimss_reva_regs_t *spi, spimss_reva_req_t *req);
int MXC_SPIMSS_RevA_MasterTransAsync(mxc_spimss_reva_regs_t *spi, spimss_reva_req_t *req);
int MXC_SPIMSS_RevA_SlaveTransAsync(mxc_spimss_reva_regs_t *spi, spimss_reva_req_t *req);
int MXC_SPIMSS_RevA_SetDefaultTXData (spimss_reva_req_t* spi, unsigned int defaultTXData);
int MXC_SPIMSS_RevA_AbortAsync(spimss_reva_req_t *req);
