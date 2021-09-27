/**
 * @file    dma.h
 * @brief   Direct Memory Access (DMA) driver function prototypes and data types.
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
 *************************************************************************** */

#ifndef _DMA_H_
#define _DMA_H_

/* **** Includes **** */
#include <stdbool.h>
#include "mxc_device.h"
#include "mxc_errors.h"
#include "dma_regs.h"

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @defgroup dma Direct Memory Access (DMA)
 * @ingroup periphlibs
 * @{
 */

/* **** Definitions **** */


/**
 * @brief   Enumeration for the DMA Channel's priority level.
 *
 */
typedef enum {
    MXC_DMA_PRIO_HIGH    = MXC_V_DMA_CFG_PRI_HIGH,      ///< High Priority */
    MXC_DMA_PRIO_MEDHIGH = MXC_V_DMA_CFG_PRI_MEDHIGH,   ///< Medium High Priority */
    MXC_DMA_PRIO_MEDLOW  = MXC_V_DMA_CFG_PRI_MEDLOW,    ///< Medium Low Priority */
    MXC_DMA_PRIO_LOW     = MXC_V_DMA_CFG_PRI_LOW,       ///< Low Priority */
} mxc_dma_priority_t;

/** @brief DMA request select */
typedef enum {
    MXC_DMA_REQUEST_MEMTOMEM = MXC_S_DMA_CFG_REQSEL_MEMTOMEM,  ///< Memory to Memory DMA Request Selection
    MXC_DMA_REQUEST_SPI0RX   = MXC_S_DMA_CFG_REQSEL_SPI0RX,    ///< SPI0 Receive DMA Request Selection
    MXC_DMA_REQUEST_SPI1RX   = MXC_S_DMA_CFG_REQSEL_SPI1RX,    ///< SPI1 Receive DMA Request Selection
    MXC_DMA_REQUEST_UART0RX  = MXC_S_DMA_CFG_REQSEL_UART0RX,   ///< UART0 Receive DMA Request Selection
    MXC_DMA_REQUEST_UART1RX  = MXC_S_DMA_CFG_REQSEL_UART1RX,   ///< UART1 Receive DMA Request Selection
    MXC_DMA_REQUEST_I2C0RX   = MXC_S_DMA_CFG_REQSEL_I2C0RX,    ///< I2C0 Receive DMA Request Selection
    MXC_DMA_REQUEST_I2C1RX   = MXC_S_DMA_CFG_REQSEL_I2C1RX,    ///< I2C1 Receive DMA Request Selection
    MXC_DMA_REQUEST_SPI0TX   = MXC_S_DMA_CFG_REQSEL_SPI0TX,    ///< SPI0 Transmit DMA Request Selection
    MXC_DMA_REQUEST_SPI1TX   = MXC_S_DMA_CFG_REQSEL_SPI1TX,    ///< SPI1 Transmit DMA Request Selection
    MXC_DMA_REQUEST_UART0TX  = MXC_S_DMA_CFG_REQSEL_UART0TX,   ///< UART0 Transmit DMA Request Selection
    MXC_DMA_REQUEST_UART1TX  = MXC_S_DMA_CFG_REQSEL_UART1TX,   ///< UART1 Transmit DMA Request Selection
    MXC_DMA_REQUEST_I2C0TX   = MXC_S_DMA_CFG_REQSEL_I2C0TX,    ///< I2C0 Transmit DMA Request Selection
    MXC_DMA_REQUEST_I2C1TX   = MXC_S_DMA_CFG_REQSEL_I2C1TX,    ///< I2C1 Transmit DMA Request Selection
} mxc_dma_reqsel_t;

/** @brief Enumeration for the DMA prescaler */
typedef enum {
    MXC_DMA_PRESCALE_DISABLE = MXC_S_DMA_CFG_PSSEL_DIS,     ///< Prescaler disabled
    MXC_DMA_PRESCALE_DIV256  = MXC_S_DMA_CFG_PSSEL_DIV256,  ///< Divide by 256
    MXC_DMA_PRESCALE_DIV64K  = MXC_S_DMA_CFG_PSSEL_DIV64K,  ///< Divide by 65,536
    MXC_DMA_PRESCALE_DIV16M  = MXC_S_DMA_CFG_PSSEL_DIV16M,  ///< Divide by 16,777,216
} mxc_dma_prescale_t;

/** @brief Enumeration for the DMA timeout value */
typedef enum {
    MXC_DMA_TIMEOUT_4_CLK   = MXC_S_DMA_CFG_TOSEL_TO4,      ///< DMA timeout of 4 clocks
    MXC_DMA_TIMEOUT_8_CLK   = MXC_S_DMA_CFG_TOSEL_TO8,      ///< DMA timeout of 8 clocks
    MXC_DMA_TIMEOUT_16_CLK  = MXC_S_DMA_CFG_TOSEL_TO16,     ///< DMA timeout of 16 clocks
    MXC_DMA_TIMEOUT_32_CLK  = MXC_S_DMA_CFG_TOSEL_TO32,     ///< DMA timeout of 32 clocks
    MXC_DMA_TIMEOUT_64_CLK  = MXC_S_DMA_CFG_TOSEL_TO64,     ///< DMA timeout of 64 clocks
    MXC_DMA_TIMEOUT_128_CLK = MXC_S_DMA_CFG_TOSEL_TO128,    ///< DMA timeout of 128 clocks
    MXC_DMA_TIMEOUT_256_CLK = MXC_S_DMA_CFG_TOSEL_TO256,    ///< DMA timeout of 256 clocks
    MXC_DMA_TIMEOUT_512_CLK = MXC_S_DMA_CFG_TOSEL_TO512,    ///< DMA timeout of 512 clocks
} mxc_dma_timeout_t;

/** @brief DMA transfer data width */
typedef enum {
    /* Using the '_V_' define instead of the '_S_' since these same values will be used to
       specify the DSTWD also.  The API functions will shift the value the correct amount
       prior to writing the cfg register. */
    MXC_DMA_WIDTH_BYTE     = MXC_V_DMA_CFG_SRCWD_BYTE,      ///< DMA transfer in bytes
    MXC_DMA_WIDTH_HALFWORD = MXC_V_DMA_CFG_SRCWD_HALFWORD,  ///< DMA transfer in 16-bit half-words
    MXC_DMA_WIDTH_WORD     = MXC_V_DMA_CFG_SRCWD_WORD,      ///< DMA transfer in 32-bit words
} mxc_dma_width_t;

/**
 * @brief   The basic configuration information to set up a DMA channel
 *          and prepare it for transfers.
 *
 */
typedef struct {
    int ch;                 ///< The channel to load the configuration data into
    mxc_dma_reqsel_t reqsel;///< The request select line to be used (mem2mem, peripheral)
    mxc_dma_width_t srcwd;  ///< The source width (could be dependent on FIFO width)
    mxc_dma_width_t dstwd;  ///< The destination width (could be dependent on FIFO width)
    int srcinc_en;          ///< Whether to increment the source address during the transfer
    int dstinc_en;          ///< Whether to increment the source address during the transfer
} mxc_dma_config_t;

/**
 * @brief   The information needed to complete a DMA transfer
 *
 */
typedef struct {
    int ch;                 ///< The channel to use for the transfer
    void* source;           ///< Pointer to the source address, if applicable
    void* dest;             ///< Pointer to the destination address, if applicable
    int len;                ///< Number of bytes to transfer
} mxc_dma_srcdst_t;

/**
 * @brief   The advanced configuration options, these are optional but could
 *          be needed in cases where multiple DMA channels are running concurrently
 *          or DMA is being used with low bandwidth peripherals.
 *
 */
typedef struct {
    int ch;                     ///< The channel to use for the transfer
    mxc_dma_priority_t prio;    ///< The DMA priority for the channel
    unsigned int reqwait_en;    ///< Delay the timeout timer start until after first transfer
    mxc_dma_timeout_t tosel;    ///< Number of prescaled clocks seen by the channel before a timeout
    mxc_dma_prescale_t pssel;   ///< Prescaler for the timeout timer
    unsigned int burst_size;    ///< Number of bytes moved in a single burst
} mxc_dma_adv_config_t;

/**
 * @brief   The callback called on completion of a DMA_MemCpy() transfer
 *
 * @param   dest    Pointer to the destination of the copy
 */
typedef void (*mxc_dma_complete_cb_t) (void* dest);

/**
 * @brief   The callback called on completion of a transfer,
 * @note    This callback is used with MXC_DMA_DoTransfer()
 *          to allow the user to chain an unlimited number of
 *          DMA Transfers.
 *
 * @param   trans    Struct of the completed transfer
 *
 * @return  Returns the next transfer to be completed, or NULL
 *          if no more transfers will be done
 */
typedef mxc_dma_srcdst_t (*mxc_dma_trans_chain_t) (mxc_dma_srcdst_t dest);

/* **** Function Prototypes **** */
/*************************/
/* Low Level Functions   */
/*************************/
/**
 * @brief      Initialize DMA resources
 * @details    This initialization is required before using the DMA driver functions.
 * @return     #E_NO_ERROR if successful
 */
int MXC_DMA_Init (void);

/**
 * @brief      Request DMA channel
 * @details    Returns a handle to the first free DMA channel, which can be used via API calls
 *             or direct access to channel registers using the MXC_DMA_GetCHRegs(int ch) function.
 * @return     Non-negative channel handle (inclusive of zero).
 * @return     #E_NONE_AVAIL    All channels in use.
 * @return     #E_BAD_STATE     DMA is not initialized, call MXC_DMA_Init() first.
 * @return     #E_BUSY          DMA is currently busy (locked), try again later.
 */
int MXC_DMA_AcquireChannel (void);

/**
 * @brief      Release DMA channel
 * @details    Stops any DMA operation on the channel and returns it to the pool of free channels.
 *
 * @param          ch   channel handle to release
 *
 * @return     #E_BAD_PARAM if an unused or invalid channel handle, #E_NO_ERROR otherwise
 */
int MXC_DMA_ReleaseChannel (int ch);

/**
 * @brief      Configure the DMA channel
 * @details    Configures the channel, which was previously requested by MXC_DMA_Getchannel()
 *
 * @param      config   Struct containing DMA configuration parameters
 * @param      srcdst   Struct containing pointers and length of DMA operation
 *
 * @return     #E_BAD_PARAM if an unused or invalid channel handle, #E_NO_ERROR otherwise
 */
int MXC_DMA_ConfigChannel (mxc_dma_config_t config, mxc_dma_srcdst_t srcdst);

/**
 * @brief      Configure the DMA channel with more advanced parameters
 *
 * @param      advConfig    Struct containing advanced DMA parameters
 *
 * @return     #E_BAD_PARAM if an unused or invalid channel handle, #E_NO_ERROR otherwise
 */
int MXC_DMA_AdvConfigChannel (mxc_dma_adv_config_t advConfig);

/**
 * @brief      Set channel source, destination, and count for the transfer
 * @param      srcdst Struct containing the channel, source, destination, and count for the channel
 * @note       Unless the channel request select is #mxc_dma_srcdst_t = MXC_DMA_REQUEST_MEMTOMEM,
 *             either src_addr or dst_addr will be ignored by the DMA engine.
 *             In these cases, the address is a don't-care. See the User's
 *             Guide for more information.
 * @return     #E_BAD_PARAM if an unused or invalid channel handle, #E_NO_ERROR otherwise
 */
int MXC_DMA_SetSrcDst (mxc_dma_srcdst_t srcdst);

/**
 * @brief      Get channel source, destination, and count for transfer
 *
 * @param      srcdst Pointer to struct with the correct channel number
 *
 * @return     See \ref MXC_Error_Codes for a list of return values
 */
int MXC_DMA_GetSrcDst (mxc_dma_srcdst_t *srcdst);

/**
 * @brief      Set channel reload source, destination, and count for the transfer
 * @param      srcdstReload Struct containing the channel, source, destination, and count for the channel
 * @note       Unless the channel request select is #mxc_dma_srcdst_t = MXC_DMA_REQUEST_MEMTOMEM,
 *             either src_addr or dst_addr will be ignored by the DMA engine.
 *             In these cases, the address is a don't-care. See the User's
 *             Guide for more information.
 * @return     #E_BAD_PARAM if an unused or invalid channel handle, #E_NO_ERROR otherwise
 */
int MXC_DMA_SetSrcReload (mxc_dma_srcdst_t srcdstReload);

/**
 * @brief      Get channel reload source, destination, and count for transfer
 *
 * @param      srcdstReload Pointer to struct with the correct channel number
 *
 * @return     See \ref MXC_Error_Codes for a list of return values
 */
int MXC_DMA_GetSrcReload (mxc_dma_srcdst_t *srcdstReload);

/**
 * @brief      Set channel interrupt callback
 * @param      ch        channel handle
 * @param      callback  Pointer to a function to call when the channel
 *                       interrupt flag is set and interrupts are enabled or
 *                       when DMA is shutdown by the driver.
 * @details    Configures the channel interrupt callback. The @p callback
 *             function is called for two conditions:
 *               -# When the channel's interrupt flag is set and DMA interrupts
 *                  are enabled.
 *               -# If the driver calls the MXC_DMA_Shutdown() function. The
 *                  callback function prototype is:
 * @code
 *             void callback_fn(int ch, int reason);
 * @endcode
 *             @p ch indicates the channel that generated the callback, @p
 *             reason is either #E_NO_ERROR for a DMA interrupt or #E_SHUTDOWN
 *             if the DMA is being shutdown.
 *
 * @return     #E_BAD_PARAM if an unused or invalid channel handle, #E_NO_ERROR
 *             otherwise
 */
int MXC_DMA_SetCallback (int ch, void (*callback) (int, int));

/**
 * @brief      Set channel interrupt
 * @note       Each channel has two interrupts (complete, and count to zero).
 *             To enable complete, pass true for chdis. To enable count to zero,
 *             pass true for ctz.
 * @param      ch Channel Handle
 * @param      chdis Enable channel complete interrupt
 * @param      ctz Enable channel count to zero interrupt.
 * @return     #E_BAD_PARAM if an unused or invalid channel handle, #E_NO_ERROR otherwise
 */
int MXC_DMA_SetChannelInterruptEn (int ch, bool chdis, bool ctz);

/**
 * @brief      Enable channel interrupt
 * @note       Each channel has two interrupts (complete, and count to zero)
               which must also be enabled with MXC_DMA_SetChannelInterruptEn()
 * @param      ch   channel handle
 * @param      flags The flags to enable
 * @return     #E_BAD_PARAM if an unused or invalid channel handle, #E_NO_ERROR otherwise
 */
int MXC_DMA_ChannelEnableInt (int ch, int flags);

/**
 * @brief      Disable channel interrupt
 * @param      ch   channel handle
 * @param      flags The flags to disable
 * @return     #E_BAD_PARAM if an unused or invalid channel handle, #E_NO_ERROR otherwise
 */
int MXC_DMA_ChannelDisableInt (int ch, int flags);

/**
 * @brief      Read channel interrupt flags
 * @param      ch   channel handle
 * @return     #E_BAD_PARAM if an unused or invalid channel handle, flags otherwise
 */
int MXC_DMA_ChannelGetFlags (int ch);

/**
 * @brief      Clear channel interrupt flags
 * @param      ch   channel handle
 * @param      flags The flags to clear
 * @return     #E_BAD_PARAM if an unused or invalid channel handle, #E_NO_ERROR otherwise
 */
int MXC_DMA_ChannelClearFlags (int ch, int flags);

/**
 * @brief      Enable channel interrupt
 * @note       Each channel has two interrupts (complete, and count to zero)
               which must also be enabled with MXC_DMA_SetChannelInterruptEn()
 * @param      ch   channel handle
 * @return     #E_BAD_PARAM if an unused or invalid channel handle, #E_NO_ERROR otherwise
 */
int MXC_DMA_EnableInt (int ch);

/**
 * @brief      Disable channel interrupt
 * @param      ch   channel handle
 * @return     #E_BAD_PARAM if an unused or invalid channel handle, #E_NO_ERROR otherwise
 */
int MXC_DMA_DisableInt (int ch);

/**
 * @brief      Start transfer
 * @param      ch   channel handle
 * @details    Start the DMA channel transfer, assumes that MXC_DMA_SetSrcDstCnt() has been called beforehand.
 * @return     #E_BAD_PARAM if an unused or invalid channel handle, #E_NO_ERROR otherwise
 */
int MXC_DMA_Start (int ch);

/**
 * @brief      Stop DMA transfer, irrespective of status (complete or in-progress)
 * @param      ch   channel handle
 * @return     #E_BAD_PARAM if an unused or invalid channel handle, #E_NO_ERROR otherwise
 */
int MXC_DMA_Stop (int ch);

/**
 * @brief      Get a pointer to the DMA channel registers
 * @param      ch   channel handle
 * @details    If direct access to DMA channel registers is required, this
 *             function can be used on a channel handle returned by MXC_DMA_AcquireChannel().
 * @return     NULL if an unused or invalid channel handle, or a valid pointer otherwise
 */
mxc_dma_ch_regs_t *MXC_DMA_GetCHRegs (int ch);

/**
 * @brief      Interrupt handler function
 * @details    Call this function as the ISR for each DMA channel under driver control.
 *             Interrupt flags for channel ch will be automatically cleared before return.
 */
void MXC_DMA_Handler();

/*************************/
/* High Level Functions  */
/*************************/

/**
 * @brief      Performs a memcpy, using DMA, optionally asynchronous
 * @note       The user must have the DMA interrupt enabled and call
 *             MXC_DMA_Handler() from the ISR.
 *
 * @param      dest     pointer to destination memory
 * @param      src      pointer to source memory
 * @param      len      number of bytes to copy
 * @param      callback function to call when transfer is complete
 *
 * @return     see \ref MXC_Error_Codes
 */
int MXC_DMA_MemCpy (void* dest, void* src, int len, mxc_dma_complete_cb_t callback);

/**
 * @brief      Performs a memcpy, using DMA, optionally asynchronous
 * @note       The user must have the DMA interrupt enabled and call
 *             MXC_DMA_Handler() from the ISR.
 *
 * @param      config   The channel config struct
 * @param      firstSrcDst  The source, destination, and count for the first transfer
 * @param      callback function is called when transfer is complete
 *
 * @return     see \ref MXC_Error_Codes
 */
int MXC_DMA_DoTransfer (mxc_dma_config_t config, mxc_dma_srcdst_t firstSrcDst, mxc_dma_trans_chain_t callback);
/**
 * For other functional uses of DMA (UART, SPI, etc) see the appropriate peripheral driver
 */

/**@} end of group dma */
#ifdef __cplusplus
}
#endif

#endif /* _DMA_H_ */
