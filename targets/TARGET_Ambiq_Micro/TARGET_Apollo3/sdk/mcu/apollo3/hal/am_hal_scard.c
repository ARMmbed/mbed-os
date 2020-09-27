//*****************************************************************************
//
//  am_hal_scard.c
//! @file
//!
//! @brief Functions for interfacing with the SCARD.
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

#include <stdint.h>
#include <stdbool.h>
#include "am_mcu_apollo.h"

//*****************************************************************************
//
// SCARD magic number for handle verification.
//
//*****************************************************************************
#define AM_HAL_MAGIC_SCARD               0xEA9E06

#define AM_HAL_SCARD_CHK_HANDLE(h)                                             \
    ((h) &&                                                                   \
     ((am_hal_handle_prefix_t *)(h))->s.bInit &&                              \
     (((am_hal_handle_prefix_t *)(h))->s.magic == AM_HAL_MAGIC_SCARD))

//*****************************************************************************
//
// Convenience macro for passing errors.
//
//*****************************************************************************
#define RETURN_ON_ERROR(x)                                                    \
    if ((x) != AM_HAL_STATUS_SUCCESS)                                         \
    {                                                                         \
        return (x);                                                           \
    };

//*****************************************************************************
//
// Baudrate to byte-time in microseconds with a little extra margin.
//
//*****************************************************************************
#define ONE_BIT_US(baudrate)       (AM_HAL_SCARD_CLK_FREQ/(baudrate))
#define ONE_BIT_DELAY(handle)                                                \
    am_hal_flash_delay(FLASH_CYCLES_US(ONE_BIT_US((handle)->ui32BaudRate)))

#define SCARD_MAX_SYNC_TIME_MS      10

#define delay_ms(ms)        am_hal_flash_delay(FLASH_CYCLES_US(1000 * (ms)))
#define delay_us(us)        am_hal_flash_delay(FLASH_CYCLES_US(us))

#define SCARD_WHILE_TIMEOUT_MS(expr, timeout, error)                                \
{                                                                         \
    uint32_t ui32Timeout = 0;                                             \
    while ( expr )                                                        \
    {                                                                     \
        if ( ui32Timeout == (timeout * 1000) )                            \
        {                                                                 \
            return error;                                                 \
        }                                                                 \
                                                                          \
        delay_us(1);                                                      \
        ui32Timeout++;                                                    \
    }                                                                     \
}

#define SCARD_SYNC_OPER(module, operation)   do{\
                                                SCARDn(module)->SR1_b.SYNCEND = 1;\
                                                operation;\
                                                SCARD_WHILE_TIMEOUT_MS(!SCARDn(module)->SR1_b.SYNCEND, SCARD_MAX_SYNC_TIME_MS, AM_HAL_SCARD_STATUS_BUS_ERROR) ;\
                                             } while ( 0 )

//*****************************************************************************
//
// Transmission parameters F and D look-up tables
// Per the ETU 7816-3 protocol ETU is computed from 2 parameters, FI and DI.
// ETU: Elementary Time Unit
// FI: Clock rate conversion factor
// DI: Bit rate adjustment factor
//
//*****************************************************************************
static uint16_t g_F_Integer[16][2] =
{
    // FI   { F, f(max)}
    /*0000*/{ 372,  4},
    /*0001*/{ 372,  5},
    /*0010*/{ 558,  6},
    /*0011*/{ 744,  8},
    /*0100*/{1116, 12},
    /*0101*/{1488, 16},
    /*0110*/{1860, 20},
    /*0111*/{   0,  0},
    /*1000*/{   0,  0},
    /*1001*/{ 512,  5},
    /*1010*/{ 768,  7},  //7.5
    /*1011*/{1024, 10},
    /*1100*/{1536, 15},
    /*1101*/{2048, 20},
    /*1110*/{   0,  0},
    /*1111*/{   0,  0}
};
static uint8_t g_D_Integer[16] =
{
    //DI  0000 0001 0010 0011 0100 0101 0110 0111
    /*D*/    0,   1,   2,   4,   8,  16,  32,  64,
    //DI  1000 1001 1010 1011 1100 1101 1110 1111
    /*D*/   12,  20,   0,   0,   0,   0,   0,   0
};

static uint16_t g_WaitTime = AM_HAL_SCARD_WAIT_MAX_TIME; //Set to max

//*****************************************************************************
//
// Structure for handling SCARD register state information for power up/down
//
//*****************************************************************************
typedef struct
{
    bool bValid;
    uint32_t regIER;
    uint32_t regTCR;
    uint32_t regUCR;
    uint32_t regBPRL;
    uint32_t regBPRH;
    uint32_t regUCR1;
    uint32_t regIER1;
    uint32_t regGTR;
    uint32_t regRETXCNT;
    uint32_t regCLKCTRL;
}
am_hal_scard_register_state_t;

//*****************************************************************************
//
// Structure for handling SCARD instance state information.
//
//*****************************************************************************
typedef struct
{
    am_hal_handle_prefix_t prefix;
    am_hal_scard_register_state_t sRegState;

    uint32_t ui32Module;

    bool bEnableTxQueue;
    am_hal_queue_t sTxQueue;

    bool bEnableRxQueue;
    am_hal_queue_t sRxQueue;

    uint32_t ui32BaudRate;
}
am_hal_scard_state_t;

//*****************************************************************************
//
// State structure for each module.
//
//*****************************************************************************
am_hal_scard_state_t g_am_hal_scard_states[AM_REG_SCARD_NUM_MODULES];

//*****************************************************************************
//
// Allows the SCARD HAL to use extra space to store TX and RX data.
//
//*****************************************************************************
static uint32_t
buffer_configure(void *pHandle, uint8_t *pui8TxBuffer, uint32_t ui32TxBufferSize,
                 uint8_t *pui8RxBuffer, uint32_t ui32RxBufferSize)
{
    am_hal_scard_state_t *pState = (am_hal_scard_state_t *) pHandle;
    uint32_t ui32ErrorStatus;

    //
    // Check to make sure this is a valid handle.
    //
    if ( !AM_HAL_SCARD_CHK_HANDLE(pHandle) )
    {
        return AM_HAL_STATUS_INVALID_HANDLE;
    }

    //
    // Check to see if we have a TX buffer.
    //
    if ( pui8TxBuffer && ui32TxBufferSize )
    {
        //
        // If so, initialzie the transmit queue, and enable the TX FIFO
        // interrupt.
        //
        pState->bEnableTxQueue = true;
        am_hal_queue_init(&pState->sTxQueue, pui8TxBuffer, 1, ui32TxBufferSize);
        ui32ErrorStatus = am_hal_scard_interrupt_enable(pHandle, 0, AM_HAL_SCARD_INT_TBERBFEN);
        RETURN_ON_ERROR(ui32ErrorStatus);
    }
    else
    {
        //
        // If not, make sure the TX FIFO interrupt is disabled.
        //
        pState->bEnableTxQueue = false;
        ui32ErrorStatus = am_hal_scard_interrupt_disable(pHandle, 0, AM_HAL_SCARD_INT_TBERBFEN);
        RETURN_ON_ERROR(ui32ErrorStatus);
    }

    //
    // Check to see if we have an RX buffer.
    //
    if ( pui8RxBuffer && ui32RxBufferSize )
    {
        //
        // If so, initialize the receive queue and the associated interupts.
        //
        pState->bEnableRxQueue = true;
        am_hal_queue_init(&pState->sRxQueue, pui8RxBuffer, 1, ui32RxBufferSize);
        ui32ErrorStatus = am_hal_scard_interrupt_enable(pHandle, 0, (AM_HAL_SCARD_INT_FHFEN |
                                                                     AM_HAL_SCARD_INT_FNEEN));
        RETURN_ON_ERROR(ui32ErrorStatus);
    }
    else
    {
        pState->bEnableRxQueue = false;
        ui32ErrorStatus = am_hal_scard_interrupt_disable(pHandle, 0, (AM_HAL_SCARD_INT_FHFEN |
                                                                      AM_HAL_SCARD_INT_FNEEN));
        RETURN_ON_ERROR(ui32ErrorStatus);
    }

    return AM_HAL_STATUS_SUCCESS;
} // buffer_configure()

//*****************************************************************************
//
// Initialization function.
//
//*****************************************************************************
uint32_t
am_hal_scard_initialize(uint32_t ui32Module, void **ppHandle)
{
    //
    // Check that the request module is in range.
    //
#ifndef AM_HAL_DISABLE_API_VALIDATION
    if ( ui32Module >= AM_REG_SCARD_NUM_MODULES )
    {
        return AM_HAL_STATUS_OUT_OF_RANGE;
    }
#endif // AM_HAL_DISABLE_API_VALIDATION

    //
    // Check for valid arguements.
    //
    if ( !ppHandle )
    {
        return AM_HAL_STATUS_INVALID_ARG;
    }

    //
    // Check if the handle is unallocated.
    //
    if ( g_am_hal_scard_states[ui32Module].prefix.s.bInit )
    {
        return AM_HAL_STATUS_INVALID_OPERATION;
    }

    //
    // Initialize the handle.
    //
    g_am_hal_scard_states[ui32Module].prefix.s.bInit = true;
    g_am_hal_scard_states[ui32Module].prefix.s.magic = AM_HAL_MAGIC_SCARD;
    g_am_hal_scard_states[ui32Module].ui32Module = ui32Module;
    g_am_hal_scard_states[ui32Module].sRegState.bValid = false;
    g_am_hal_scard_states[ui32Module].ui32BaudRate = 0;

    //
    // Return the handle.
    //
    *ppHandle = (void *)&g_am_hal_scard_states[ui32Module];

    //
    // Return the status.
    //
    return AM_HAL_STATUS_SUCCESS;
} // am_hal_scard_initialize()

//*****************************************************************************
//
// De-Initialization function.
//
//*****************************************************************************
uint32_t
am_hal_scard_deinitialize(void *pHandle)
{
    am_hal_scard_state_t *pState = (am_hal_scard_state_t *)pHandle;

    //
    // Check the handle.
    //
#ifndef AM_HAL_DISABLE_API_VALIDATION
    if ( !AM_HAL_SCARD_CHK_HANDLE(pHandle) )
    {
        return AM_HAL_STATUS_INVALID_HANDLE;
    }
#endif // AM_HAL_DISABLE_API_VALIDATION

    //
    // Reset the handle.
    //
    pState->prefix.s.bInit = false;
    pState->ui32Module = 0;
    pState->sRegState.bValid = false;

    //
    // Return the status.
    //
    return AM_HAL_STATUS_SUCCESS;
} // am_hal_scard_deinitialize()

//*****************************************************************************
//
// Power control functions.
//
//*****************************************************************************
uint32_t
am_hal_scard_power_control(void *pHandle,
                           am_hal_sysctrl_power_state_e ePowerState,
                           bool bRetainState)
{
    am_hal_scard_state_t *pState = (am_hal_scard_state_t *) pHandle;
    uint32_t ui32Module = pState->ui32Module;

#ifndef AM_HAL_DISABLE_API_VALIDATION
    if ( ui32Module >= AM_REG_SCARD_NUM_MODULES )
    {
        return AM_HAL_STATUS_OUT_OF_RANGE;
    }
#endif // AM_HAL_DISABLE_API_VALIDATION

    am_hal_pwrctrl_periph_e eSCCPowerModule = ((am_hal_pwrctrl_periph_e)
                                                (AM_HAL_PWRCTRL_PERIPH_SCARD +
                                                 ui32Module));

    //
    // Check to make sure this is a valid handle.
    //
    if ( !AM_HAL_SCARD_CHK_HANDLE(pHandle) )
    {
        return AM_HAL_STATUS_INVALID_HANDLE;
    }

    //
    // Decode the requested power state and update SCARD operation accordingly.
    //
    switch (ePowerState)
    {
        //
        // Turn on the SCC.
        //
        case AM_HAL_SYSCTRL_WAKE:
            //
            // Make sure we don't try to restore an invalid state.
            //
            if ( bRetainState && !pState->sRegState.bValid )
            {
                return AM_HAL_STATUS_INVALID_OPERATION;
            }

            //
            // Enable power control.
            //
            am_hal_pwrctrl_periph_enable(eSCCPowerModule);

            if ( bRetainState )
            {
                //
                // Restore SCC registers
                //
                AM_CRITICAL_BEGIN

                SCARDn(ui32Module)->IER = pState->sRegState.regIER;
                SCARD_SYNC_OPER(ui32Module, SCARDn(ui32Module)->TCR = pState->sRegState.regTCR);
                SCARD_SYNC_OPER(ui32Module, SCARDn(ui32Module)->UCR = pState->sRegState.regUCR);
                SCARDn(ui32Module)->BPRL = pState->sRegState.regBPRL;
                SCARDn(ui32Module)->BPRH   = pState->sRegState.regBPRH;
                SCARDn(ui32Module)->UCR1 = pState->sRegState.regUCR1;
                SCARDn(ui32Module)->IER1  = pState->sRegState.regIER1;
                SCARDn(ui32Module)->GTR  = pState->sRegState.regGTR;
                SCARDn(ui32Module)->RETXCNT  = pState->sRegState.regRETXCNT;
                SCARDn(ui32Module)->CLKCTRL  = pState->sRegState.regCLKCTRL;
                pState->sRegState.bValid = false;

                AM_CRITICAL_END
            }
            break;

        //
        // Turn off the SCARD.
        //
        case AM_HAL_SYSCTRL_NORMALSLEEP:
        case AM_HAL_SYSCTRL_DEEPSLEEP:
            if ( bRetainState )
            {
                AM_CRITICAL_BEGIN

                pState->sRegState.regIER = SCARDn(ui32Module)->IER;
                pState->sRegState.regTCR = SCARDn(ui32Module)->TCR;
                pState->sRegState.regUCR = SCARDn(ui32Module)->UCR;
                pState->sRegState.regBPRL = SCARDn(ui32Module)->BPRL;
                pState->sRegState.regBPRH   = SCARDn(ui32Module)->BPRH;
                pState->sRegState.regUCR1 = SCARDn(ui32Module)->UCR1;
                pState->sRegState.regIER1  = SCARDn(ui32Module)->IER1;
                pState->sRegState.regGTR = SCARDn(ui32Module)->GTR;
                pState->sRegState.regRETXCNT = SCARDn(ui32Module)->RETXCNT;
                pState->sRegState.regCLKCTRL = SCARDn(ui32Module)->CLKCTRL;
                pState->sRegState.bValid = true;

                AM_CRITICAL_END
            }

            //
            // Clear all interrupts before sleeping as having a pending SCARD
            // interrupt burns power.
            //
            am_hal_scard_interrupt_clear(pState, 0, AM_HAL_SCARD_INT_ALL);
            am_hal_scard_interrupt_clear(pState, 1, AM_HAL_SCARD_INT_ALL);

            //
            // Disable power control.
            //
            am_hal_pwrctrl_periph_disable(eSCCPowerModule);
            break;

        default:
            return AM_HAL_STATUS_INVALID_ARG;
    }

    //
    // Return the status.
    //
    return AM_HAL_STATUS_SUCCESS;
} // am_hal_scard_power_control()

//*****************************************************************************
//
// SCARD configuration.
//
//*****************************************************************************
uint32_t
am_hal_scard_configure(void *pHandle, am_hal_scard_config_t *psConfig)
{
    uint32_t status = AM_HAL_STATUS_SUCCESS;
    am_hal_scard_state_t *pState = (am_hal_scard_state_t *) pHandle;
    uint32_t ui32Module = pState->ui32Module;

    //
    // Check to make sure this is a valid handle.
    //
#ifndef AM_HAL_DISABLE_API_VALIDATION
    if ( !AM_HAL_SCARD_CHK_HANDLE(pHandle) )
    {
        return AM_HAL_STATUS_INVALID_HANDLE;
    }
#endif // AM_HAL_DISABLE_API_VALIDATION
    //
    // Start by enabling the clocks, which needs to happen in a critical
    // section.
    //
    AM_CRITICAL_BEGIN

    SCARDn(ui32Module)->CLKCTRL_b.APBCLKEN = 1;
    SCARDn(ui32Module)->CLKCTRL_b.CLKEN = 1;

    AM_CRITICAL_END
    //
    // Set the baud rate.
    //
    status = am_hal_scard_control(pHandle, AM_HAL_SCARD_REQ_BAUDRATE, &psConfig->ui32Fidi);

    //RETURN_ON_ERROR(ui32ErrorStatus);
    //
    // Copy the configuration options into the appropriate registers.
    //
    status = am_hal_scard_control(pHandle, AM_HAL_SCARD_REQ_PROTOCOL, &psConfig->ui32Protocol);
    status = am_hal_scard_control(pHandle, AM_HAL_SCARD_REQ_CARD_FORMAT, &psConfig->ui32Direction);
    status = am_hal_scard_control(pHandle, AM_HAL_SCARD_REQ_PARITY, &psConfig->ui32Parity);
    status = am_hal_scard_control(pHandle, AM_HAL_SCARD_REQ_GUARDTIME, &psConfig->ui32GuardTime);
    SCARDn(ui32Module)->UCR1_b.CLKIOV = psConfig->ui32ClkLevel;
    status = am_hal_scard_control(pHandle, AM_HAL_SCARD_REQ_CLK_STOP, NULL);
    SCARD_SYNC_OPER(ui32Module, SCARDn(ui32Module)->UCR_b.RIU = 1);
    if ( AM_HAL_STATUS_SUCCESS != status )
    {
        return AM_HAL_STATUS_FAIL;
    }
    //
    // Set up any buffers that might exist.
    //
    buffer_configure(pHandle,
                     psConfig->pui8TxBuffer,
                     psConfig->ui32TxBufferSize,
                     psConfig->pui8RxBuffer,
                     psConfig->ui32RxBufferSize);

    return AM_HAL_STATUS_SUCCESS;
} // am_hal_scard_configure()

//*****************************************************************************
//
// Set Baud Rate Register based on the parameters F and D.
//
//*****************************************************************************
static void
config_baudrate(void *pHandle, uint32_t ui32Fidi)
{
    uint16_t bpr;
    uint32_t ui32ActualBaud;
    am_hal_scard_state_t *pState = (am_hal_scard_state_t *) pHandle;
    uint32_t ui32Module = pState->ui32Module;

    // F is the clock rate conversion integer
    // D is the baud rate adjustment integer
    //    1 ETU = (F/D)*(1/f) s
    // The default values of these parameters are:
    // F = 372 ; D = 1; f (max.) = 5 MHz

    //
    // BPRL and BPRH are used for counting ETU
    //
    bpr = ((g_F_Integer[AM_HAL_SCARD_FI(ui32Fidi)][0] != 0) && (g_D_Integer[AM_HAL_SCARD_DI(ui32Fidi)] != 0)) ? \
        g_F_Integer[AM_HAL_SCARD_FI(ui32Fidi)][0] / g_D_Integer[AM_HAL_SCARD_DI(ui32Fidi)] : \
        g_F_Integer[AM_HAL_SCARD_FI(AM_HAL_SCARD_FI_DI_DEFAULT)][0] / g_D_Integer[AM_HAL_SCARD_DI(AM_HAL_SCARD_FI_DI_DEFAULT)];

    SCARDn(ui32Module)->BPRL = bpr & 0xFF;
    SCARDn(ui32Module)->BPRH = (SCARDn(ui32Module)->BPRH & (~SCARD_BPRH_BPRH_Msk)) | ((bpr >> 8) & SCARD_BPRH_BPRH_Msk) ;
    ui32ActualBaud = (uint32_t)(AM_HAL_SCARD_CLK_FREQ / bpr);
    pState->ui32BaudRate = ui32ActualBaud;
} // config_baudrate()

//*****************************************************************************
//
// Set card format, direct convention or inverse convention
//
//*****************************************************************************
static uint32_t
config_cardformat(uint32_t ui32Module, uint32_t ui32Format)
{
    switch(ui32Format)
    {
        //
        // Inverse convention
        //
        case AM_HAL_SCARD_CONV_MSB_0X3F:
            SCARD_SYNC_OPER(ui32Module, SCARDn(ui32Module)->TCR_b.AUTOCONV = 1);
            SCARD_SYNC_OPER(ui32Module, SCARDn(ui32Module)->TCR_b.CONV = 1);
            break;
        //
        // Direct convention
        //
        case AM_HAL_SCARD_CONV_LSB_0X3B:
            SCARD_SYNC_OPER(ui32Module, SCARDn(ui32Module)->TCR_b.AUTOCONV = 1);
            SCARD_SYNC_OPER(ui32Module, SCARDn(ui32Module)->TCR_b.CONV = 0);
            break;
        //
        // Not set by software, configured by the first received byte
        //
        case AM_HAL_SCARD_CONV_AUTO:
        default:
            SCARD_SYNC_OPER(ui32Module, SCARDn(ui32Module)->TCR_b.AUTOCONV = 0);
            SCARD_SYNC_OPER(ui32Module, SCARDn(ui32Module)->TCR_b.SS = 1);
            break;
    }
    return AM_HAL_STATUS_SUCCESS;
}

//*****************************************************************************
//
// Enable/disbale parity and set it to odd/even
//
//*****************************************************************************
static uint32_t
config_parity(uint32_t ui32Module, uint32_t ui32Parity)
{
    //
    // T1 protocol
    //
    if ( SCARDn(ui32Module)->TCR_b.PROT )
    {
        //
        // Enable parity
        //
        if ( ui32Parity & 0xF0 )
        {
            SCARDn(ui32Module)->UCR1_b.T1PAREN = 1;
            SCARD_SYNC_OPER(ui32Module, SCARDn(ui32Module)->TCR_b.FIP = ui32Parity & 0xF);
        }
        //
        // Disbale parity
        //
        else
        {
            SCARDn(ui32Module)->UCR1_b.T1PAREN = 0;
        }
    }
    //
    // T0 protocol, always enable parity
    //
    else
    {
        SCARD_SYNC_OPER(ui32Module, SCARDn(ui32Module)->TCR_b.FIP = ui32Parity & 0xF);
    }
    return AM_HAL_STATUS_SUCCESS;
}

//*****************************************************************************
//
// Set protocol, T0 or T1
//
//*****************************************************************************
static uint32_t
config_protocol(uint32_t ui32Module, uint32_t ui32Protocol)
{
    if ( 1 == ui32Protocol )
    {
        SCARD_SYNC_OPER(ui32Module, SCARDn(ui32Module)->TCR_b.PROT = AM_HAL_SCARD_PROTOCOL_T1);
    }
    else if ( 0 == ui32Protocol )
    {
        SCARD_SYNC_OPER(ui32Module, SCARDn(ui32Module)->TCR_b.PROT = AM_HAL_SCARD_PROTOCOL_T0);
    }
    else
    {
        return AM_HAL_SCARD_STATUS_PROTOCAL_NOT_SUPPORT;
    }
    return AM_HAL_STATUS_SUCCESS;
}

//*****************************************************************************
//
// Set and start ETU counter
//
//*****************************************************************************
static uint32_t
config_etucounter(uint32_t ui32Module, uint16_t ui16Etu)
{
    //
    // Set low-8bit first, then set high-8bit, after software writes ECNTH, ETU counter starts counting
    //
    SCARD_WHILE_TIMEOUT_MS(!SCARDn(ui32Module)->SR1_b.IDLE, 100, AM_HAL_SCARD_STATUS_BUS_ERROR);
    SCARDn(ui32Module)->SR1_b.SYNCEND = 1;
    SCARDn(ui32Module)->ECNTL = (ui16Etu) & 0xFF;
    SCARD_WHILE_TIMEOUT_MS(!SCARDn(ui32Module)->SR1_b.SYNCEND, 100, AM_HAL_SCARD_STATUS_BUS_ERROR);
    SCARDn(ui32Module)->ECNTH = ((ui16Etu) >> 8);
    return AM_HAL_STATUS_SUCCESS;
}

//*****************************************************************************
//
// Read as much data from the SCARD FIFO as possible, up to ui32NumBytes
//
//*****************************************************************************
uint32_t scard_fifo_read(void *pHandle, uint8_t *pui8Data, uint32_t ui32NumBytes, uint32_t *pui32NumBytesRead)
{
    uint32_t ui32ErrorStatus = AM_HAL_STATUS_SUCCESS;
    uint32_t i = 0;
    am_hal_scard_state_t *pState = (am_hal_scard_state_t *) pHandle;
    uint32_t ui32Module = pState->ui32Module;
    uint8_t ui8Index = 0;

    SCARD_SYNC_OPER(ui32Module, SCARDn(ui32Module)->TCR_b.TR = 0);

    while ( ui32NumBytes )
    {
        config_etucounter(ui32Module, g_WaitTime);
        while ( (!SCARDn(ui32Module)->SR_b.FNE) && (!SCARDn(ui32Module)->SR1_b.ECNTOVER) && (!SCARDn(ui32Module)->SR_b.PE) && (!SCARDn(ui32Module)->SR_b.FER) );
        //
        // Read times out
        //
        if ( SCARDn(ui32Module)->SR1_b.ECNTOVER )
        {
            break;
        }
        //
        // Parity error or Frame error
        //
        else if ( (SCARDn(ui32Module)->SR_b.PE) || (SCARDn(ui32Module)->SR_b.FER) )
        {
            SCARDn(ui32Module)->SR_b.PE = 0;
            SCARDn(ui32Module)->SR_b.FER = 0;
            ui32ErrorStatus = AM_HAL_STATUS_FAIL;
            break;
        }
        //
        // RX FIFO is full, read 8 bytes out
        //
        else if ( SCARDn(ui32Module)->SR_b.TBERBF )
        {
            for ( ui8Index = 0; ui8Index < AM_HAL_SCARD_FIFO_MAX; ui8Index++ )
            {
                pui8Data[i++] = SCARDn(ui32Module)->DR_b.DR;
            }
            ui32NumBytes -= AM_HAL_SCARD_FIFO_MAX;
        }
        //
        // RX FIFO is half full, read 4 bytes out
        //
        else if ( SCARDn(ui32Module)->SR_b.FHF )
        {
            for ( ui8Index = 0; ui8Index < AM_HAL_SCARD_FIFO_MAX / 2; ui8Index++ )
            {
                pui8Data[i++] = SCARDn(ui32Module)->DR_b.DR;
            }
            ui32NumBytes -= AM_HAL_SCARD_FIFO_MAX / 2;
        }
        //
        // RX FIFO is not empty, read as much as we can
        //
        else if ( SCARDn(ui32Module)->SR_b.FNE )
        {
            pui8Data[i++] = SCARDn(ui32Module)->DR_b.DR;
            ui32NumBytes--;
        }
    }
    if ( pui32NumBytesRead )
    {
        *pui32NumBytesRead = i;
    }
    return ui32ErrorStatus;
}

//*****************************************************************************
//
// Read as much data from the SCARD FIFO as possible, up to ui32NumBytes
//
//*****************************************************************************
uint32_t scard_fifo_write(void *pHandle, uint8_t *pui8Data, uint32_t ui32NumBytes, uint32_t *pui32NumBytesWritten)
{
    am_hal_scard_state_t *pState = (am_hal_scard_state_t *) pHandle;
    uint32_t ui32Module = pState->ui32Module;
    uint32_t i = 0;

    if ( ui32NumBytes )
    {
        SCARD_SYNC_OPER(ui32Module, SCARDn(ui32Module)->TCR_b.TR = 1);

        while ( 1 != ui32NumBytes-- )
        {
            //
            // Write 1 byte into DR
            //
            SCARDn(ui32Module)->DR_b.DR = pui8Data[i++];
            SCARD_WHILE_TIMEOUT_MS((!SCARDn(ui32Module)->SR_b.TBERBF) && (!SCARDn(ui32Module)->SR_b.PE) && (!SCARDn(ui32Module)->SR_b.FER), 100, AM_HAL_SCARD_STATUS_BUS_ERROR);
            //
            // Parity error or Frame error
            //
            if ( (SCARDn(ui32Module)->SR_b.PE) || (SCARDn(ui32Module)->SR_b.FER) )
            {
                SCARDn(ui32Module)->SR_b.PE = 0;
                SCARDn(ui32Module)->SR_b.FER = 0;
                return AM_HAL_STATUS_FAIL;
            }
        }

        //
        // Enable fast TX to RX function before the last byte
        //
        SCARD_SYNC_OPER(ui32Module, SCARDn(ui32Module)->TCR_b.LCT = 1);
        SCARDn(ui32Module)->DR_b.DR = pui8Data[i++];
        //
        // SCC should switch back to RX after all data sent out
        //
        SCARD_WHILE_TIMEOUT_MS((!SCARDn(ui32Module)->SR_b.FT2REND) && (!SCARDn(ui32Module)->SR_b.PE) && (!SCARDn(ui32Module)->SR_b.FER), 100, AM_HAL_SCARD_STATUS_BUS_ERROR);
        //
        // Parity error or Frame error
        //
        if ( (SCARDn(ui32Module)->SR_b.PE) || (SCARDn(ui32Module)->SR_b.FER) )
        {
            SCARDn(ui32Module)->SR_b.PE = 0;
            SCARDn(ui32Module)->SR_b.FER = 0;
            return AM_HAL_STATUS_FAIL;
        }

        SCARD_SYNC_OPER(ui32Module, SCARDn(ui32Module)->TCR_b.TR = 0);
    }
    if ( pui32NumBytesWritten )
    {
        *pui32NumBytesWritten = i;
    }
    return AM_HAL_STATUS_SUCCESS;
}

//*****************************************************************************
//
// Empty the SCARD RX FIFO, and place the data into the RX queue.
//
//*****************************************************************************
static uint32_t
rx_queue_update(void *pHandle)
{
    am_hal_scard_state_t *pState = (am_hal_scard_state_t *) pHandle;

    uint8_t pui8Data[AM_HAL_SCARD_FIFO_MAX];
    uint32_t ui32BytesTransferred;
    uint32_t ui32ErrorStatus;

    AM_CRITICAL_BEGIN

    //
    // Read as much of the FIFO as we can.
    //
    ui32ErrorStatus = scard_fifo_read(pHandle, pui8Data, AM_HAL_SCARD_FIFO_MAX,
                                     &ui32BytesTransferred);
    //
    // If we were successful, go ahead and transfer the data along to the
    // buffer.
    //
    if ( ui32ErrorStatus == AM_HAL_STATUS_SUCCESS )
    {
        if ( !am_hal_queue_item_add(&pState->sRxQueue, pui8Data,
                                    ui32BytesTransferred) )
        {
            ui32ErrorStatus = AM_HAL_SCARD_STATUS_RX_QUEUE_FULL;
        }
    }

    AM_CRITICAL_END

    return ui32ErrorStatus;
} // rx_queue_update()

//*****************************************************************************
//
// Transfer as much data as possible from the TX queue to the TX FIFO.
//
//*****************************************************************************
static uint32_t
tx_queue_update(void *pHandle)
{
    am_hal_scard_state_t *pState = (am_hal_scard_state_t *) pHandle;

    uint8_t pui8Data;
    uint32_t ui32BytesTransferred;
    uint32_t ui32ErrorStatus = AM_HAL_STATUS_SUCCESS;

    AM_CRITICAL_BEGIN

    //
    // Attempt to grab an item from the queue, and add it to the fifo.
    //
    while ( 1 )
    {
        if ( am_hal_queue_item_get(&pState->sTxQueue, &pui8Data, 1) )
        {
            ui32ErrorStatus = scard_fifo_write(pHandle, &pui8Data, 1, &ui32BytesTransferred);
        }
        else
        {
            //
            // If we didn't get anything from the queue, we can just return.
            //
            break;
        }
    }

    AM_CRITICAL_END

    return ui32ErrorStatus;
} // tx_queue_update()

//*****************************************************************************
//
// Attempt to read N bytes from the FIFO, but give up if they aren't there.
//
//*****************************************************************************
static uint32_t
read_nonblocking(void *pHandle, uint8_t *pui8Data, uint32_t ui32NumBytes,
                 uint32_t *pui32NumBytesRead)
{
    uint32_t ui32BufferData;
    uint32_t ui32BytesTransferred;
    uint32_t ui32ErrorStatus = AM_HAL_STATUS_SUCCESS;

    am_hal_scard_state_t *pState = (am_hal_scard_state_t *) pHandle;

    //
    // Check to make sure this is a valid handle.
    //
#ifndef AM_HAL_DISABLE_API_VALIDATION
    if ( (!AM_HAL_SCARD_CHK_HANDLE(pHandle)) || (NULL == pui8Data) || (NULL == pui32NumBytesRead) )
    {
        return AM_HAL_STATUS_INVALID_HANDLE;
    }
#endif // AM_HAL_DISABLE_API_VALIDATION

    //
    // Start by setting the number of bytes read to 0.
    //
    *pui32NumBytesRead = 0;

    if ( ui32NumBytes == 0 )
    {
        return AM_HAL_STATUS_SUCCESS;
    }

    //
    // Check to see if the circular receive buffer has been enabled.
    //
    if ( pState->bEnableRxQueue )
    {
        //
        // If it is, update it, and then try to read the requested number of
        // bytes, giving up if fewer were actually found.
        //
        ui32ErrorStatus = rx_queue_update(pHandle);
        RETURN_ON_ERROR(ui32ErrorStatus);

        ui32BufferData = am_hal_queue_data_left(&pState->sRxQueue);

        ui32BytesTransferred = (ui32NumBytes < ui32BufferData ?
                                ui32NumBytes : ui32BufferData);

        am_hal_queue_item_get(&pState->sRxQueue, pui8Data, ui32BytesTransferred);
    }
    else
    {
        //
        // If the buffer isn't enabled, just read straight from the FIFO.
        //
        ui32ErrorStatus = scard_fifo_read(pHandle, pui8Data, ui32NumBytes,
                                         &ui32BytesTransferred);
    }

    //
    // Let the caller know how much we transferred if they provided us with a
    // pointer.
    //
    *pui32NumBytesRead = ui32BytesTransferred;

    return ui32ErrorStatus;
} // read_nonblocking()

//*****************************************************************************
//
// Attempt to write N bytes to the FIFO, but give up if there's no space.
//
//*****************************************************************************
static uint32_t
write_nonblocking(void *pHandle, uint8_t *pui8Data, uint32_t ui32NumBytes,
                  uint32_t *pui32NumBytesWritten)
{
    uint32_t ui32ErrorStatus;
    uint32_t ui32BufferSpace;
    uint32_t ui32BytesTransferred;

    am_hal_scard_state_t *pState = (am_hal_scard_state_t *) pHandle;

    //
    // Check to make sure this is a valid handle.
    //
#ifndef AM_HAL_DISABLE_API_VALIDATION
    if ( (!AM_HAL_SCARD_CHK_HANDLE(pHandle)) || (NULL == pui8Data) || (NULL == pui32NumBytesWritten) )
    {
        return AM_HAL_STATUS_INVALID_HANDLE;
    }
#endif // AM_HAL_DISABLE_API_VALIDATION

    //
    // Let the caller know how much we transferred if they provided us with a
    // pointer.
    //
    *pui32NumBytesWritten = 0;

    if ( ui32NumBytes == 0 )
    {
        return AM_HAL_STATUS_SUCCESS;
    }

    //
    // Check to see if the circular transmit buffer has been enabled.
    //
    if ( pState->bEnableTxQueue )
    {
        //
        // If it has, been enabled, write as much data to it as we can, and let
        // the caller know how much that was.
        //
        ui32BufferSpace = am_hal_queue_space_left(&pState->sTxQueue);

        ui32BytesTransferred = (ui32NumBytes < ui32BufferSpace ?
                             ui32NumBytes : ui32BufferSpace);

        am_hal_queue_item_add(&pState->sTxQueue, pui8Data, ui32BytesTransferred);

        //
        // Transfer as much data as possible from the queue to the fifo.
        //
        ui32ErrorStatus = tx_queue_update(pHandle);
        RETURN_ON_ERROR(ui32ErrorStatus);
    }
    else
    {
        //
        // If the buffer isn't enabled, just write straight to the FIFO.
        //
        scard_fifo_write(pHandle, pui8Data, ui32NumBytes,
                     &ui32BytesTransferred);
    }

    //
    // Let the caller know how much we transferred if they provided us with a
    // pointer.
    //
    *pui32NumBytesWritten = ui32BytesTransferred;

    return AM_HAL_STATUS_SUCCESS;
} // write_nonblocking()

//*****************************************************************************
//
// This function will keep reading bytes until it either gets N bytes or runs
// into an error.
//
//*****************************************************************************
static uint32_t
read_timeout(void *pHandle, uint8_t *pui8Data, uint32_t ui32NumBytes,
             uint32_t *pui32NumBytesRead, uint32_t ui32TimeoutMs)
{
    uint32_t ui32Status, ui32BytesRead, ui32RemainingBytes,
             ui32TimeSpent, i;

    //
    // If we don't have a timeout, just pass this directly to the nonblocking
    // call.
    //
    if ( ui32TimeoutMs == 0 )
    {
        return read_nonblocking(pHandle, pui8Data, ui32NumBytes,
                                pui32NumBytesRead);
    }

    i = 0;
    ui32RemainingBytes = ui32NumBytes;
    ui32TimeSpent = 0;

    //
    // Loop until we're done reading. This will either be because we hit a
    // timeout, or we got the right number of bytes. If the caller specified
    // "wait forever", then don't check the timeout.
    //
    while ( ui32RemainingBytes && (ui32TimeSpent < ui32TimeoutMs) )
    {
        //
        // Read as much as we can.
        //
        ui32BytesRead = 0;
        ui32Status = read_nonblocking(pHandle, &pui8Data[i],
                                      ui32RemainingBytes,
                                      &ui32BytesRead);
        //
        // Update the tracking variables.
        //
        i += ui32BytesRead;
        ui32RemainingBytes -= ui32BytesRead;

        if ( ui32Status != AM_HAL_STATUS_SUCCESS )
        {
            if ( pui32NumBytesRead )
            {
                *pui32NumBytesRead = i;
            }

            return ui32Status;
        }

        //
        // Update the timeout.
        //
        if ( ui32RemainingBytes )
        {
            delay_us(1);

            if ( ui32TimeoutMs != AM_HAL_SCARD_WAIT_FOREVER )
            {
                ui32TimeSpent++;
            }
        }
    }

    if ( pui32NumBytesRead )
    {
        *pui32NumBytesRead = i;
    }

    return AM_HAL_STATUS_SUCCESS;
} // read_timeout()

//*****************************************************************************
//
// This function will keep writing bytes until it either sends N bytes or runs
// into an error.
//
//*****************************************************************************
static uint32_t
write_timeout(void *pHandle, uint8_t *pui8Data, uint32_t ui32NumBytes,
              uint32_t *pui32NumBytesWritten, uint32_t ui32TimeoutMs)
{
    uint32_t ui32Status, ui32BytesWritten, ui32RemainingBytes,
             ui32TimeSpent, i;

    i = 0;
    ui32RemainingBytes = ui32NumBytes;
    ui32TimeSpent = 0;

    //
    // If we don't have a timeout, just pass this directly to the nonblocking
    // call.
    //
    if ( ui32TimeoutMs == 0 )
    {
        return write_nonblocking(pHandle, pui8Data, ui32NumBytes,
                                 pui32NumBytesWritten);
    }

    //
    // Loop until we're done write. This will either be because we hit a
    // timeout, or we sent the right number of bytes. If the caller specified
    // "wait forever", then don't check the timeout.
    //
    while ( ui32RemainingBytes && (ui32TimeSpent < ui32TimeoutMs) )
    {
        //
        // Write as much as we can.
        //
        ui32BytesWritten = 0;
        ui32Status = write_nonblocking(pHandle, &pui8Data[i],
                                       ui32RemainingBytes,
                                       &ui32BytesWritten);
        //
        // Update the tracking variables.
        //
        i += ui32BytesWritten;
        ui32RemainingBytes -= ui32BytesWritten;

        if ( ui32Status != AM_HAL_STATUS_SUCCESS )
        {
            if ( pui32NumBytesWritten )
            {
                *pui32NumBytesWritten = i;
            }

            return ui32Status;
        }

        //
        // Update the timeout.
        //
        if ( ui32RemainingBytes )
        {
            delay_us(1);

            if ( ui32TimeoutMs != AM_HAL_SCARD_WAIT_FOREVER )
            {
                ui32TimeSpent++;
            }
        }
    }

    if ( pui32NumBytesWritten )
    {
        *pui32NumBytesWritten = i;
    }

    return AM_HAL_STATUS_SUCCESS;
} // write_timeout()

//*****************************************************************************
//
// Send or receive bytes.
//
//*****************************************************************************
uint32_t
am_hal_scard_transfer(void *pHandle, const am_hal_scard_transfer_t *pTransfer)
{
    //
    // Pick the right function to use based on the transfer structure.
    //
    if ( pTransfer->ui32Direction == AM_HAL_SCARD_WRITE )
    {
        return write_timeout(pHandle,
                             pTransfer->pui8Data,
                             pTransfer->ui32NumBytes,
                             pTransfer->pui32BytesTransferred,
                             pTransfer->ui32TimeoutMs);
    }
    else if ( pTransfer->ui32Direction == AM_HAL_SCARD_READ )
    {
        return read_timeout(pHandle,
                            pTransfer->pui8Data,
                            pTransfer->ui32NumBytes,
                            pTransfer->pui32BytesTransferred,
                            pTransfer->ui32TimeoutMs);
    }

    return AM_HAL_STATUS_INVALID_OPERATION;
} // am_hal_scard_transfer()

//*****************************************************************************
//
// Wait for all of the traffic in the TX pipeline to be sent.
//
//*****************************************************************************
uint32_t
am_hal_scard_tx_flush(void *pHandle)
{
    am_hal_scard_state_t *pState = (am_hal_scard_state_t *) pHandle;
    uint32_t ui32Module = pState->ui32Module;

    //
    // If we have a TX queue, we should wait for it to empty.
    //
    if ( pState->bEnableTxQueue )
    {
        while ( am_hal_queue_data_left(&(pState->sTxQueue)) )
        {
            ONE_BIT_DELAY(pState);
        }
    }

    //
    // Wait for the IDLE bit to go high.
    //
    while ( SCARDn(ui32Module)->SR1_b.IDLE != 1 )
    {
        ONE_BIT_DELAY(pState);
    }

    return AM_HAL_STATUS_SUCCESS;
} // am_hal_scard_tx_flush()

//*****************************************************************************
//
// Interrupt service
//
//*****************************************************************************
uint32_t
am_hal_scard_interrupt_service(void *pHandle, uint32_t ui32Status, uint32_t *pui32ScardTxIdle)
{
    am_hal_scard_state_t *pState = (am_hal_scard_state_t *) pHandle;
    uint32_t ui32Module = pState->ui32Module;
    uint32_t ui32ErrorStatus;

#ifndef AM_HAL_DISABLE_API_VALIDATION
    if ( !AM_HAL_SCARD_CHK_HANDLE(pHandle) )
    {
        return AM_HAL_STATUS_INVALID_HANDLE;
    }
#endif // AM_HAL_DISABLE_API_VALIDATION

    //
    // Check to see if we have filled the Rx FIFO past the configured limit, or
    // if we have an 'old' character or two sitting in the FIFO.
    //
    if ( (ui32Status & (SCARD_SR_TBERBF_Msk | SCARD_SR_FHF_Msk | SCARD_SR_FNE_Msk) ) &&
         pState->bEnableRxQueue)
    {
        ui32ErrorStatus = rx_queue_update(pHandle);
        RETURN_ON_ERROR(ui32ErrorStatus);
    }

    //
    // Check to see if our TX buffer has been recently emptied. If so, we
    // should refill it from the TX ring buffer.
    //
    if ( (ui32Status & SCARD_SR_TBERBF_Msk) && pState->bEnableTxQueue )
    {
        ui32ErrorStatus = tx_queue_update(pHandle);
        RETURN_ON_ERROR(ui32ErrorStatus);
    }

    //
    // If this pointer is null, we can just return success now. There is no
    // need to figure out if the SCC is idle.
    //
    if ( pui32ScardTxIdle == 0 )
    {
        return AM_HAL_STATUS_SUCCESS;
    }

    if ( SCARDn(ui32Module)->SR1_b.IDLE == 1 )
    {
        *pui32ScardTxIdle = true;
    }
    else
    {
        *pui32ScardTxIdle = false;
    }
    return AM_HAL_STATUS_SUCCESS;
}

//*****************************************************************************
//
// Interrupt enable.
//
//*****************************************************************************
uint32_t
am_hal_scard_interrupt_enable(void *pHandle, uint32_t ui32Index, uint32_t ui32IntMask)
{
    am_hal_scard_state_t *pState = (am_hal_scard_state_t *) pHandle;
    uint32_t ui32Module = pState->ui32Module;

#ifndef AM_HAL_DISABLE_API_VALIDATION
    if ( (!AM_HAL_SCARD_CHK_HANDLE(pHandle)) || (ui32Index > 1) )
    {
        return AM_HAL_STATUS_INVALID_HANDLE;
    }
#endif // AM_HAL_DISABLE_API_VALIDATION

    if ( 0 == ui32Index )
    {
        SCARDn(ui32Module)->IER |= ui32IntMask;
    }
    else
    {
        SCARDn(ui32Module)->IER1 |= ui32IntMask;
    }

    return AM_HAL_STATUS_SUCCESS;
} // am_hal_scard_interrupt_enable()

//*****************************************************************************
//
// Interrupt disable.
//
//*****************************************************************************
uint32_t
am_hal_scard_interrupt_disable(void *pHandle, uint32_t ui32Index, uint32_t ui32IntMask)
{
    am_hal_scard_state_t *pState = (am_hal_scard_state_t *) pHandle;
    uint32_t ui32Module = pState->ui32Module;

#ifndef AM_HAL_DISABLE_API_VALIDATION
    if ( (!AM_HAL_SCARD_CHK_HANDLE(pHandle)) || (ui32Index > 1) )
    {
        return AM_HAL_STATUS_INVALID_HANDLE;
    }
#endif // AM_HAL_DISABLE_API_VALIDATION

    if ( 0 == ui32Index )
    {
        SCARDn(ui32Module)->IER &= ~ui32IntMask;
    }
    else
    {
        SCARDn(ui32Module)->IER1 &= ~ui32IntMask;
    }

    return AM_HAL_STATUS_SUCCESS;
} // am_hal_scard_interrupt_disable()

//*****************************************************************************
//
// Interrupt clear.
//
//*****************************************************************************
uint32_t
am_hal_scard_interrupt_clear(void *pHandle, uint32_t ui32Index, uint32_t ui32IntMask)
{
    am_hal_scard_state_t *pState = (am_hal_scard_state_t *) pHandle;
    uint32_t ui32Module = pState->ui32Module;

#ifndef AM_HAL_DISABLE_API_VALIDATION
    if ( (!AM_HAL_SCARD_CHK_HANDLE(pHandle)) || (ui32Index > 1) )
    {
        return AM_HAL_STATUS_INVALID_HANDLE;
    }
#endif // AM_HAL_DISABLE_API_VALIDATION

    if ( 0 == ui32Index )
    {
        SCARDn(ui32Module)->SR = ui32IntMask;
    }
    else
    {
        SCARDn(ui32Module)->SR1 = ui32IntMask;
    }

    return AM_HAL_STATUS_SUCCESS;
} // am_hal_scard_interrupt_clear()

//*****************************************************************************
//
// Returns the interrupt status.
//
//*****************************************************************************
uint32_t
am_hal_scard_interrupt_status_get(void *pHandle, uint32_t ui32Index, uint32_t *pui32Status)
{
    am_hal_scard_state_t *pState = (am_hal_scard_state_t *) pHandle;
    uint32_t ui32Module = pState->ui32Module;

#ifndef AM_HAL_DISABLE_API_VALIDATION
    if ( !AM_HAL_SCARD_CHK_HANDLE(pHandle) )
    {
        return AM_HAL_STATUS_INVALID_HANDLE;
    }
#endif // AM_HAL_DISABLE_API_VALIDATION

    //
    // If requested, only return the interrupts that are enabled.
    //
    *pui32Status = ui32Index ? SCARDn(ui32Module)->SR1 : SCARDn(ui32Module)->SR;

    return AM_HAL_STATUS_SUCCESS;
} // am_hal_scard_interrupt_status_get()


//*****************************************************************************
//
//! @brief SCARD control function
//!
//! @param handle       - handle for the SCARD.
//! @param eReq         - device specific special request code.
//! @param pArgs        - pointer to the request specific arguments.
//!
//! This function allows advanced settings
//!
//! @return status      - generic or interface specific status.
//
//*****************************************************************************
uint32_t
am_hal_scard_control(void *pHandle, am_hal_scard_request_e eReq, void *pArgs)
{
    am_hal_scard_state_t *pSCCState = (am_hal_scard_state_t*)pHandle;
    uint32_t status = AM_HAL_STATUS_SUCCESS;

#ifndef AM_HAL_DISABLE_API_VALIDATION
    if ( !AM_HAL_SCARD_CHK_HANDLE(pHandle) )
    {
        return AM_HAL_STATUS_INVALID_HANDLE;
    }

    //
    // Validate the parameters
    //
    if ( eReq >= AM_HAL_SCARD_REQ_MAX )
    {
        return AM_HAL_STATUS_INVALID_ARG;
    }
#endif // AM_HAL_DISABLE_API_VALIDATION

    uint32_t ui32Module = pSCCState->ui32Module;
    switch (eReq)
    {
        case AM_HAL_SCARD_REQ_ACTIVATE:
            {
                uint16_t etu;
                etu = ((SCARDn(ui32Module)->BPRH & SCARD_BPRH_BPRH_Msk) << 8);
                etu = etu | SCARDn(ui32Module)->BPRL;
                etu = (SCARD_RST_LOW_TIME / etu) + 1;

                config_etucounter(ui32Module, etu);
                SCARD_WHILE_TIMEOUT_MS(!SCARDn(ui32Module)->SR1_b.ECNTOVER, 1000, AM_HAL_SCARD_STATUS_BUS_ERROR);
                SCARDn(ui32Module)->SR1_b.ECNTOVER = 1;
                SCARD_SYNC_OPER(ui32Module, SCARDn(ui32Module)->UCR_b.RSTIN = 1);
            }
            break;
        case AM_HAL_SCARD_REQ_DEACTIVATE:
            SCARD_SYNC_OPER(ui32Module, SCARDn(ui32Module)->UCR_b.RSTIN = 0);
            break;
        case AM_HAL_SCARD_REQ_BAUDRATE:
            if ( pArgs )
            {
                config_baudrate(pHandle, *(uint32_t*)pArgs);
            }
            else
            {
                status = AM_HAL_STATUS_INVALID_ARG;
            }
            break;
        case AM_HAL_SCARD_REQ_CARD_FORMAT:
            if ( pArgs )
            {
                config_cardformat(ui32Module, *(uint32_t*)pArgs);
            }
            else
            {
                status = AM_HAL_STATUS_INVALID_ARG;
            }
            break;
        case AM_HAL_SCARD_REQ_PARITY:
            if ( pArgs )
            {
                config_parity(ui32Module, *(uint32_t*)pArgs);
            }
            else
            {
                status = AM_HAL_STATUS_INVALID_ARG;
            }
            break;
        case AM_HAL_SCARD_REQ_PROTOCOL:
            if ( pArgs )
            {
                if ( AM_HAL_STATUS_SUCCESS != config_protocol(ui32Module, *(uint32_t*)pArgs) )
                {
                    status = AM_HAL_STATUS_INVALID_ARG;
                }
            }
            else
            {
                status = AM_HAL_STATUS_INVALID_ARG;
            }
            break;
        case AM_HAL_SCARD_REQ_GUARDTIME:
            if ( pArgs )
            {
                SCARDn(ui32Module)->GTR = *(uint32_t*)pArgs;
            }
            else
            {
                status = AM_HAL_STATUS_INVALID_ARG;
            }
            break;
        case AM_HAL_SCARD_REQ_CLK_START:
            SCARD_SYNC_OPER(ui32Module, SCARDn(ui32Module)->UCR_b.CST = 0);
            break;
        case AM_HAL_SCARD_REQ_CLK_STOP:
            SCARD_SYNC_OPER(ui32Module, SCARDn(ui32Module)->UCR_b.CST = 1);
            break;
        default:
            status = AM_HAL_STATUS_INVALID_ARG;
    }

    return status;
}


