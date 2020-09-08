//*****************************************************************************
//
//  am_hal_scard.h
//! @file
//!
//! @brief Functions for accessing and configuring the SCARD.
//!
//! @addtogroup scard3
//! @ingroup apollo3hal
//! @{
//
//*****************************************************************************

//*****************************************************************************
//
// Copyright (c) 2020, Ambiq Micro
// All rights reserved.
//
// Redistribution and use in source and binary forms, with or without
// modification, are permitted provided that the following conditions are met:
//
// 1. Redistributions of source code must retain the above copyright notice,
// this list of conditions and the following disclaimer.
//
// 2. Redistributions in binary form must reproduce the above copyright
// notice, this list of conditions and the following disclaimer in the
// documentation and/or other materials provided with the distribution.
//
// 3. Neither the name of the copyright holder nor the names of its
// contributors may be used to endorse or promote products derived from this
// software without specific prior written permission.
//
// Third party software included in this distribution is subject to the
// additional license terms as defined in the /docs/licenses directory.
//
// THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
// AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
// IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
// ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE
// LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
// CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
// SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
// INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
// CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
// ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
// POSSIBILITY OF SUCH DAMAGE.
//
// This is part of revision 2.4.2 of the AmbiqSuite Development Package.
//
//*****************************************************************************
// SPDX-License-Identifier: BSD-3-Clause
#ifndef AM_HAL_SCARD_H
#define AM_HAL_SCARD_H

#ifdef __cplusplus
extern "C"
{
#endif

//*****************************************************************************
//
// CMSIS-style macro for handling a variable SCARD module number.
//
//*****************************************************************************
#define SCARDn(n)    ((SCARD_Type*)(SCARD_BASE + (n * (SCARD_BASE - SCARD_BASE))))

//*****************************************************************************
//
// Initial character TS
//
//*****************************************************************************
#define AM_HAL_SCARD_DIR_MSB             0x3F //(H)LHHL LLL LLH, state L encodes value 1, msb
#define AM_HAL_SCARD_DIR_LSB             0x3B //(H)LHHL HHH LLH, state H encodes value 1, lsb

//*****************************************************************************
//
// Informations provided by T0
//
//*****************************************************************************
#define AM_HAL_SCARD_T0_BIT_TA1_MASK     (1 << 4)
#define AM_HAL_SCARD_T0_BIT_TB1_MASK     (1 << 5)
#define AM_HAL_SCARD_T0_BIT_TC1_MASK     (1 << 6)
#define AM_HAL_SCARD_T0_BIT_TD1_MASK     (1 << 7)

#define AM_HAL_SCARD_TA1_PRESENCE(T0)    (((T0) & AM_HAL_SCARD_T0_BIT_TA1_MASK) == AM_HAL_SCARD_T0_BIT_TA1_MASK)
#define AM_HAL_SCARD_TB1_PRESENCE(T0)    (((T0) & AM_HAL_SCARD_T0_BIT_TB1_MASK) == AM_HAL_SCARD_T0_BIT_TB1_MASK)
#define AM_HAL_SCARD_TC1_PRESENCE(T0)    (((T0) & AM_HAL_SCARD_T0_BIT_TC1_MASK) == AM_HAL_SCARD_T0_BIT_TC1_MASK)
#define AM_HAL_SCARD_TD1_PRESENCE(T0)    (((T0) & AM_HAL_SCARD_T0_BIT_TD1_MASK) == AM_HAL_SCARD_T0_BIT_TD1_MASK)

#define AM_HAL_SCARD_HISTORY_LEN(T0)     ((T0)&0x0F)

//*****************************************************************************
//
// Protocol type T
//
//*****************************************************************************
#define AM_HAL_SCARD_PROTOCOL_T0         0
#define AM_HAL_SCARD_PROTOCOL_T1         1
#define AM_HAL_SCARD_PROTOCOL_T15        15

#define AM_HAL_SCARD_PROTOCOL_DEFAULT    AM_HAL_SCARD_PROTOCOL_T0

//*****************************************************************************
//
// Structure and content of PPS request and PPS confirm
//
//*****************************************************************************
#define AM_HAL_SCARD_CLA_PPS             0xFF

#define AM_HAL_SCARD_PPS1_PRESENCE(PPS0) (((PPS0) & (1 << 4)) == (1 << 4))
#define AM_HAL_SCARD_PPS2_PRESENCE(PPS0) (((PPS0) & (1 << 5)) == (1 << 5))
#define AM_HAL_SCARD_PPS3_PRESENCE(PPS0) (((PPS0) & (1 << 6)) == (1 << 6))

//*****************************************************************************
//
// Informations provided by TA1
//
//*****************************************************************************
#define AM_HAL_SCARD_FI(TA1)         (((TA1) >> 4) & 0x0F)
#define AM_HAL_SCARD_DI(TA1)         (((TA1) >> 0) & 0x0F)

#define AM_HAL_SCARD_FI_DI_DEFAULT   0x11

//*****************************************************************************
//
// Informations provided by TDi
//
//*****************************************************************************
#define AM_HAL_SCARD_TDi_BIT_TAiP1_MASK      (1 << 4)
#define AM_HAL_SCARD_TDi_BIT_TBiP1_MASK      (1 << 5)
#define AM_HAL_SCARD_TDi_BIT_TCiP1_MASK      (1 << 6)
#define AM_HAL_SCARD_TDi_BIT_TDiP1_MASK      (1 << 7)

#define AM_HAL_SCARD_TAiP1_PRESENCE(TDi)     (((TDi) & AM_HAL_SCARD_TDi_BIT_TAiP1_MASK) == AM_HAL_SCARD_TDi_BIT_TAiP1_MASK)
#define AM_HAL_SCARD_TBiP1_PRESENCE(TDi)     (((TDi) & AM_HAL_SCARD_TDi_BIT_TBiP1_MASK) == AM_HAL_SCARD_TDi_BIT_TBiP1_MASK)
#define AM_HAL_SCARD_TCiP1_PRESENCE(TDi)     (((TDi) & AM_HAL_SCARD_TDi_BIT_TCiP1_MASK) == AM_HAL_SCARD_TDi_BIT_TCiP1_MASK)
#define AM_HAL_SCARD_TDiP1_PRESENCE(TDi)     (((TDi) & AM_HAL_SCARD_TDi_BIT_TDiP1_MASK) == AM_HAL_SCARD_TDi_BIT_TDiP1_MASK)

#define AM_HAL_SCARD_PROTOCOL_TYPE(TDi)      ((TDi) & 0x0F)

#define AM_HAL_SCARD_MAX_ATR_LENGTH          33 //1+32
#define AM_HAL_SCARD_MAX_PPS_LENGTH          6
#define AM_HAL_SCARD_APDU_HEADER_LENGTH      5
#define AM_HAL_SCARD_SW_LENGTH               2

//*****************************************************************************
//
// TypeDefs
//
//*****************************************************************************
typedef struct
{
    uint8_t pps0;
    uint8_t pps1;
    uint8_t pps2;
    uint8_t pps3;
}am_hal_scard_pps_t;

typedef struct
{
    uint8_t cla;
    uint8_t ins;
    uint8_t p1;
    uint8_t p2;
    uint8_t p3;
}am_hal_scard_header_t;

typedef struct
{
    am_hal_scard_header_t header;
    uint8_t data[256];
}am_hal_scard_tpdu_t;

typedef union
{
    struct
    {
        uint8_t s0;
        uint8_t s1;
    }element;

    uint16_t entirety;
}am_hal_scard_sw_t;

typedef enum
{
    AM_HAL_SCARD_CONV_AUTO,
    AM_HAL_SCARD_CONV_LSB_0X3B,
    AM_HAL_SCARD_CONV_MSB_0X3F
}am_hal_scard_cardformat_e;

typedef enum
{
    AM_HAL_SCARD_EVEN,
    AM_HAL_SCARD_ODD
}am_hal_scard_parity_e;

typedef enum
{
    AM_HAL_SCARD_APDU_CLA,
    AM_HAL_SCARD_APDU_INS,
    AM_HAL_SCARD_APDU_P1,
    AM_HAL_SCARD_APDU_P2,
    AM_HAL_SCARD_APDU_LC
}am_hal_scard_apdu_header_e;

//*****************************************************************************
//
// Definitions
//
//*****************************************************************************
#define SCARD_RST_LOW_TIME                   42000

#define AM_HAL_SCARD_PARITY_ENABLE           0x10

//*****************************************************************************
//
// SCARD configuration options.
//
//*****************************************************************************
typedef struct
{
    //
    // Standard SCARD options.
    //
    uint32_t ui32Fidi;
    uint32_t ui32Protocol;
    uint32_t ui32Direction;
    uint32_t ui32Parity;
    uint32_t ui32GuardTime;
    uint32_t ui32ClkLevel;

    //
    // Timeouts
    //
    uint32_t ui32TxTimeout;
    uint32_t ui32RxTimeout;

    //
    // Buffers
    //
    uint8_t *pui8TxBuffer;
    uint32_t ui32TxBufferSize;
    uint8_t *pui8RxBuffer;
    uint32_t ui32RxBufferSize;
}
am_hal_scard_config_t;

typedef enum
{
    AM_HAL_SCARD_REQ_ACTIVATE = 0,
    AM_HAL_SCARD_REQ_DEACTIVATE,
    AM_HAL_SCARD_REQ_BAUDRATE,
    AM_HAL_SCARD_REQ_CARD_FORMAT,
    AM_HAL_SCARD_REQ_PARITY,
    AM_HAL_SCARD_REQ_PROTOCOL,
    AM_HAL_SCARD_REQ_GUARDTIME,
    AM_HAL_SCARD_REQ_CLK_START,
    AM_HAL_SCARD_REQ_CLK_STOP,
    AM_HAL_SCARD_REQ_MAX
}am_hal_scard_request_e;

//*****************************************************************************
//
// @brief SCARD transfer structure.
//
// This structure describes a SCARD transaction that can be performed by \e
// am_hal_scard_transfer()
//
//*****************************************************************************
typedef struct
{
    //! Determines whether data should be read or written.
    //!
    //! Should be either AM_HAL_SCARD_WRITE or AM_HAL_SCARD_READ
    uint32_t ui32Direction;

    //! Pointer to data to be sent, or space to fill with received data.
    uint8_t *pui8Data;

    //! Number of bytes to send or receive.
    uint32_t ui32NumBytes;

    //! Timeout in milliseconds.
    //!
    //! Given a timeout value, the \e am_hal_scard_transfer() function will keep
    //! trying to transfer data until either the number of bytes is satisfied,
    //! or the time runs out. If provided with a value of zero, the transfer
    //! function will only send as much data as it can immediately deal with.
    //! If provided with a timeout value of \e AM_HAL_SCARD_WAIT_FOREVER, the
    //! function will block until either the final "read" byte is received or
    //! the final "write" byte is placed in the output buffer.
    uint32_t ui32TimeoutMs;

    //! Number of bytes successfully transferred.
    uint32_t *pui32BytesTransferred;
}
am_hal_scard_transfer_t;

//*****************************************************************************
//
// Scard transfer options.
//
//*****************************************************************************
#define AM_HAL_SCARD_WRITE                   1
#define AM_HAL_SCARD_READ                    0
#define AM_HAL_SCARD_WAIT_MAX_TIME           0xFFFF
#define AM_HAL_SCARD_WAIT_FOREVER            0xFFFFFFFF
#define AM_HAL_SCARD_CLK_FREQ                3000000

//*****************************************************************************
//
// SCARD interrupts.
//
//*****************************************************************************
#define AM_HAL_SCARD_INT_FHFEN               SCARD_IER_FHFEN_Msk
#define AM_HAL_SCARD_INT_FT2RENDEN           SCARD_IER_FT2RENDEN_Msk
#define AM_HAL_SCARD_INT_PEEN                SCARD_IER_PEEN_Msk
#define AM_HAL_SCARD_INT_OVREN               SCARD_IER_OVREN_Msk
#define AM_HAL_SCARD_INT_FEREN               SCARD_IER_FEREN_Msk
#define AM_HAL_SCARD_INT_TBERBFEN            SCARD_IER_TBERBFEN_Msk
#define AM_HAL_SCARD_INT_FNEEN               SCARD_IER_FNEEN_Msk
#define AM_HAL_SCARD_INT_SYNCENDEN           SCARD_IER1_SYNCENDEN_Msk
#define AM_HAL_SCARD_INT_PRLEN               SCARD_IER1_PRLEN_Msk
#define AM_HAL_SCARD_INT_ECNTOVEREN          SCARD_IER1_ECNTOVEREN_Msk
#define AM_HAL_SCARD_INT_ALL                 0xFFFFFFFF

//*****************************************************************************
//
//! @name SCARD Status Register
//! @brief Macro definitions for SCARD Status Register Bits.
//
//*****************************************************************************
#define AM_HAL_SCARD_SR_TX_EMPTY             (_VAL2FLD(SCARD_SR_TBERBF, 1))
#define AM_HAL_SCARD_SR_RX_FULL              (_VAL2FLD(SCARD_SR_TBERBF, 1))
#define AM_HAL_SCARD_SR_RX_HALF_FULL         (_VAL2FLD(SCARD_SR_FHF, 1))
#define AM_HAL_SCARD_SR_RX_NOT_EMPTY         (_VAL2FLD(SCARD_SR_FNE, 1))
#define AM_HAL_SCARD_SR_FT2REND              (_VAL2FLD(SCARD_SR_FT2REND, 1))
#define AM_HAL_SCARD_SR_IDLE                 (_VAL2FLD(SCARD_SR1_IDLE, 1))

//*****************************************************************************
//
// SCC FIFO size for Apollo3.
//
//*****************************************************************************
#define AM_HAL_SCARD_FIFO_MAX                8

//*****************************************************************************
//
//! @brief Initialize the SCARD interface.
//!
//! @param ui32Module is the module number for the SCARD to initialize.
//! @param ppHandle is the location to write the SCARD handle.
//!
//! This function sets internal tracking variables associated with a specific
//! SCARD module. It should be the first SCARD API called for each SCARD module in
//! use. The handle can be used to interact with the SCARD
//!
//! @return AM_HAL_STATUS_SUCCESS or applicable SCARD errors.
//
//*****************************************************************************
extern uint32_t am_hal_scard_initialize(uint32_t ui32Module, void **ppHandle);

//*****************************************************************************
//
//! @brief Deinitialize the SCARD interface.
//!
//! @param pHandle is a previously initialized SCARD handle.
//!
//! This function effectively disables future calls to interact with the SCARD
//! refered to by \e pHandle. The user may call this function if SCARD operation
//! is no longer desired.
//!
//! @return AM_HAL_STATUS_SUCCESS or applicable SCARD errors.
//
//*****************************************************************************
extern uint32_t am_hal_scard_deinitialize(void *pHandle);

//*****************************************************************************
//
//! @brief Change the power state of the SCARD module.
//!
//! @param pHandle is the handle for the SCARD to operate on.
//! @param ePowerstate is the desired power state of the SCARD.
//! @parame bRetainState is a flag to ask the HAL to save SCARD registers.
//!
//! This function can be used to switch the power to the SCARD on or off. If \e
//! bRetainState is true during a powerdown operation, it will store the SCARD
//! configuration registers to SRAM, so it can restore them on power-up.
//!
//! @return AM_HAL_STATUS_SUCCESS or applicable SCARD errors.
//
//*****************************************************************************
extern uint32_t am_hal_scard_power_control(void *pHandle,
                                          am_hal_sysctrl_power_state_e ePowerState,
                                          bool bRetainState);

//*****************************************************************************
//
//! @brief Used to configure basic SCARD settings.
//!
//! @param pHandle is the handle for the SCARD to operate on.
//! @param psConfig is a structure of SCARD configuration options.
//!
//! This function takes the options from an \e am_hal_scard_config_t structure,
//! and applies them to the SCARD referred to by \e pHandle.
//!
//! @return AM_HAL_STATUS_SUCCESS or applicable SCARD errors.
//
//*****************************************************************************
extern uint32_t am_hal_scard_configure(void *pHandle,
                                             am_hal_scard_config_t *psConfig);

//*****************************************************************************
//
//! @brief Transfer data through the SCARD interface.
//!
//! @param pHandle is the handle for the SCARD to operate on.
//! @param am_hal_scard_transfer_t is a structure describing the operation.
//!
//! This function executes a transaction as described by the \e
//! am_hal_scard_transfer_t structure. It can either read or write, and it will
//! take advantage of any buffer space provided by the \e
//! am_hal_scard_configure() function.
//!
//! @return AM_HAL_STATUS_SUCCESS or applicable SCARD errors.
//
//*****************************************************************************
extern uint32_t am_hal_scard_transfer(void *pHandle,
                                     const am_hal_scard_transfer_t *pTransfer);


//*****************************************************************************
//
//! @brief Wait for the SCARD TX to become idle
//!
//! @param pHandle is the handle for the SCARD to operate on.
//!
//! This function waits (polling) for all data in the SCARD TX FIFO and SCARD TX
//! buffer (if configured) to be fully sent on the physical SCARD interface.
//! This is not the most power-efficient way to wait for SCARD idle, but it can be
//! useful in simpler applications, or where power-efficiency is less important.
//!
//! Once this function returns, the SCARD can be safely disabled without
//! interfering with any previous transmissions.
//!
//! For a more power-efficient way to shut down the SCARD, check the
//! \e am_hal_scard_interrupt_service() function.
//!
//! @return AM_HAL_STATUS_SUCCESS or applicable SCARD errors.
//
//*****************************************************************************
extern uint32_t am_hal_scard_tx_flush(void *pHandle);

//*****************************************************************************
//
//! @brief This function handles the SCARD buffers during SCARD interrupts.
//!
//! @param pHandle is the handle for the SCARD to operate on.
//! @param ui32Status is the interrupt status at the time of ISR entry.
//! @param pui32ScardTxIdle can be used to store the SCARD idle status.
//!
//! The main purpose of this function is to manage the SCARD buffer system. Any
//! buffers configured by \e am_hal_scard_buffer_configure will be managed by
//! this service routine. Data queued for transmit will be added to the SCARD TX
//! FIFO as space allows, and data stored in the SCARD RX FIFO will be copied
//! out and stored in the RX buffer. This function will skip this transfer for
//! any buffer that has not been configured.
//!
//! In addition, this function can be used to alert the caller when the SCARD
//! becomes idle via the optional \e pui32ScardTxIdle argument. This function
//! will set this variable any time it completes its operation and the SCARD TX
//! channel is no longer in use (including both the FIFO and any configured
//! buffer).
//!
//! For RTOS-enabled cases, this function does not necessarily need to be
//! called inside the actual ISR for the SCARD, but it should be called promptly
//! in response to the receipt of a SCARD TX, RX, or RX timeout interrupt. If
//! the service routine is not called quickly enough, the caller risks an RX
//! FIFO overflow (data can be lost here), or a TX FIFO underflow (usually not
//! harmful).
//!
//! @return AM_HAL_STATUS_SUCCESS or applicable SCARD errors.
//
//*****************************************************************************
extern uint32_t am_hal_scard_interrupt_service(void *pHandle,
                                              uint32_t ui32Status,
                                              uint32_t *pui32ScardTxIdle);

//*****************************************************************************
//
//! @brief Enable interrupts.
//!
//! @param pHandle is the handle for the SCARD to operate on.
//! @param ui32IntMask is the bitmask of interrupts to enable.
//!
//! This function enables the SCARD interrupt(s) given by ui32IntMask. If
//! multiple interrupts are desired, they can be OR'ed together.
//!
//! @note This function need not be called for SCARD FIFO interrupts if the SCARD
//! buffer service provided by \e am_hal_scard_buffer_configure() and \e
//! am_hal_scard_interrupt_service() is already in use. Non-FIFO-related
//! interrupts do require the use of this function.
//!
//! The full list of interrupts is given by the following:
//!
//! @code
//!
//! AM_HAL_SCARD_INT_FHFEN
//! AM_HAL_SCARD_INT_FT2RENDEN
//! AM_HAL_SCARD_INT_PEEN
//! AM_HAL_SCARD_INT_OVREN
//! AM_HAL_SCARD_INT_FEREN
//! AM_HAL_SCARD_INT_TBERBFEN
//! AM_HAL_SCARD_INT_FNEEN
//! AM_HAL_SCARD_INT_SYNCENDEN
//! AM_HAL_SCARD_INT_PRLEN
//! AM_HAL_SCARD_INT_ECNTOVEREN
//!
//! @endcode
//!
//! @return AM_HAL_STATUS_SUCCESS or applicable SCARD errors.
//
//*****************************************************************************
extern uint32_t am_hal_scard_interrupt_enable(void *pHandle, uint32_t ui32Index,
                                             uint32_t ui32IntMask);

//*****************************************************************************
//
//! @brief Disable interrupts.
//!
//! @param pHandle is the handle for the SCARD to operate on.
//! @param ui32IntMask is the bitmask of interrupts to disable.
//!
//! This function disables the SCARD interrupt(s) given by ui32IntMask. If
//! multiple interrupts need to be disabled, they can be OR'ed together.
//!
//! @note This function need not be called for SCARD FIFO interrupts if the SCARD
//! buffer service provided by \e am_hal_scard_buffer_configure() and \e
//! am_hal_scard_interrupt_service() is already in use. Non-FIFO-related
//! interrupts do require the use of this function.
//!
//! The full list of interrupts is given by the following:
//!
//! @code
//!
//! AM_HAL_SCARD_INT_FHFEN
//! AM_HAL_SCARD_INT_FT2RENDEN
//! AM_HAL_SCARD_INT_PEEN
//! AM_HAL_SCARD_INT_OVREN
//! AM_HAL_SCARD_INT_FEREN
//! AM_HAL_SCARD_INT_TBERBFEN
//! AM_HAL_SCARD_INT_FNEEN
//! AM_HAL_SCARD_INT_SYNCENDEN
//! AM_HAL_SCARD_INT_PRLEN
//! AM_HAL_SCARD_INT_ECNTOVEREN
//!
//! @endcode
//!
//! @return AM_HAL_STATUS_SUCCESS or applicable SCARD errors.
//
//*****************************************************************************
extern uint32_t am_hal_scard_interrupt_disable(void *pHandle, uint32_t ui32Index,
                                              uint32_t ui32IntMask);

//*****************************************************************************
//
//! @brief Clear interrupt status.
//!
//! @param pHandle is the handle for the SCARD to operate on.
//! @param ui32IntMask is the bitmask of interrupts to clear.
//!
//! This function clears the SCARD interrupt(s) given by ui32IntMask. If
//! multiple interrupts need to be cleared, they can be OR'ed together.
//!
//! The full list of interrupts is given by the following:
//!
//! @code
//!
//! AM_HAL_SCARD_INT_FHFEN
//! AM_HAL_SCARD_INT_FT2RENDEN
//! AM_HAL_SCARD_INT_PEEN
//! AM_HAL_SCARD_INT_OVREN
//! AM_HAL_SCARD_INT_FEREN
//! AM_HAL_SCARD_INT_TBERBFEN
//! AM_HAL_SCARD_INT_FNEEN
//! AM_HAL_SCARD_INT_SYNCENDEN
//! AM_HAL_SCARD_INT_PRLEN
//! AM_HAL_SCARD_INT_ECNTOVEREN
//!
//! @endcode
//!
//! @return AM_HAL_STATUS_SUCCESS or applicable SCARD errors.
//
//*****************************************************************************
extern uint32_t am_hal_scard_interrupt_clear(void *pHandle, uint32_t ui32Index,
                                            uint32_t ui32IntMask);

//*****************************************************************************
//
//! @brief Read interrupt status.
//!
//! @param pHandle is the handle for the SCARD to operate on.
//!
//! @param pui32Status is the returned interrupt status (all bits OR'ed
//! together)
//!
//! @param bEnabled determines whether to read interrupts that were not
//! enabled.
//!
//! This function reads the status the SCARD interrupt(s) if \e bEnabled is
//! true, it will only return the status of the enabled interrupts. Otherwise,
//! it will return the status of all interrupts, enabled or disabled.
//!
//! The full list of interrupts is given by the following:
//!
//! @code
//!
//! AM_HAL_SCARD_INT_FHFEN
//! AM_HAL_SCARD_INT_FT2RENDEN
//! AM_HAL_SCARD_INT_PEEN
//! AM_HAL_SCARD_INT_OVREN
//! AM_HAL_SCARD_INT_FEREN
//! AM_HAL_SCARD_INT_TBERBFEN
//! AM_HAL_SCARD_INT_FNEEN
//! AM_HAL_SCARD_INT_SYNCENDEN
//! AM_HAL_SCARD_INT_PRLEN
//! AM_HAL_SCARD_INT_ECNTOVEREN
//!
//! @endcode
//!
//! @return AM_HAL_STATUS_SUCCESS or applicable SCARD errors.

//
//*****************************************************************************
extern uint32_t am_hal_scard_interrupt_status_get(void *pHandle, uint32_t ui32Index,
                                                 uint32_t *pui32Status);

extern uint32_t am_hal_scard_control(void *pHandle, am_hal_scard_request_e eReq, void *pArgs);

typedef enum
{
    AM_HAL_SCARD_STATUS_BUS_ERROR = AM_HAL_STATUS_MODULE_SPECIFIC_START,
    AM_HAL_SCARD_STATUS_RX_QUEUE_FULL,
    AM_HAL_SCARD_STATUS_PROTOCAL_NOT_SUPPORT,
}
am_hal_scard_errors_t;

#ifdef __cplusplus
}
#endif

#endif // AM_HAL_SCARD_H

//*****************************************************************************
//
// End Doxygen group.
//! @}
//
//*****************************************************************************
