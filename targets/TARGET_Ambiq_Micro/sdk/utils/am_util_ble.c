//*****************************************************************************
//
//! @file am_util_apollo3_ble.c
//!
//! @brief Useful BLE functions not covered by the HAL.
//!
//! This file contains functions for interacting with the Apollo3 BLE hardware
//! that are not already covered by the HAL. Most of these commands either
//! adjust RF settings or facilitate RF testing operations.
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
#include <string.h>
#include "am_util_delay.h"
#include "am_mcu_apollo.h"

//*****************************************************************************
//
// Globals
//
//*****************************************************************************

//*****************************************************************************
//
// In DTM mode, set TX to constant trans mode for SRRC/FCC/CE
//set enable as 'true' to constant trans mode, 'false' back to normal
//*****************************************************************************
uint32_t
am_util_ble_set_constant_transmission(void *pHandle, bool enable)
{
    am_hal_ble_state_t *pBLE = pHandle;

    am_hal_ble_sleep_set(pBLE, false);
    am_hal_ble_plf_reg_write(pBLE, 0x43000004, 0xFFFFFFFF);
    if ( enable )
    {
        am_hal_ble_plf_reg_write(pBLE, 0x508000E0, 0x00008000);
    }
    else
    {
        am_hal_ble_plf_reg_write(pBLE, 0x508000E0, 0x00000000);
    }

    return AM_HAL_STATUS_SUCCESS;
}

//*****************************************************************************
//
// Manually enable/disable transmitter
// set ui8TxCtrl as 1 to manually enable transmitter, 0 back to default
//
//*****************************************************************************
uint32_t
am_util_ble_transmitter_control(void *pHandle, uint8_t ui8TxCtrl)
{
    am_hal_ble_state_t *pBLE = pHandle;
    uint32_t RegValueTRX;

    am_hal_ble_sleep_set(pBLE, false);
    if (ui8TxCtrl)
    {
        RegValueTRX = 0x2000A;
    }
    else
    {
        RegValueTRX = 0x8;
    }

    //
    // Unlock the BLE registers.
    //
    am_hal_ble_plf_reg_write(pBLE, 0x43000004, 0xFFFFFFFF);
    am_hal_ble_plf_reg_write(pBLE, 0x52400000, RegValueTRX);

    return AM_HAL_STATUS_SUCCESS;
}

//*****************************************************************************
//
//to fix the channel 1 bug in DTM mode
//
//*****************************************************************************

uint32_t
am_util_ble_init_rf_channel(void *pHandle)
{
    if (!APOLLO3_GE_B0)
    {
        am_hal_ble_buffer(16) sWriteCommand;
        am_hal_ble_buffer(16) sResponse;
        am_hal_ble_state_t *pBLE = pHandle;
        uint32_t ui32IntEnable;

        uint32_t ui32Module = pBLE->ui32Module;
        am_hal_ble_sleep_set(pBLE, false);

        //issue the HCI command with to init for the channel 1
        sWriteCommand.bytes[0] = 0x01;
        sWriteCommand.bytes[1] = 0x1d;
        sWriteCommand.bytes[2] = 0x20;
        sWriteCommand.bytes[3] = 0x01;
        sWriteCommand.bytes[4] = 0x00;

        //
        // Temporarily disable BLE interrupts.
        //
        ui32IntEnable = BLEIFn(ui32Module)->INTEN;
        BLEIFn(ui32Module)->INTEN = 0;

        // reserved packet_payload
        am_hal_ble_blocking_hci_write(pBLE,
                                      AM_HAL_BLE_RAW,
                                      sWriteCommand.words,
                                      5);
        BLEIFn(ui32Module)->BLEDBG_b.IOCLKON = 1;

        //
        // Wait for the response.
        //
        while ( BLEIFn(ui32Module)->BSTATUS_b.BLEIRQ == 0 );
        am_hal_ble_blocking_hci_read(pBLE, sResponse.words, 0);

        am_util_delay_ms(10);

        // issue the HCI command with to stop test for the channel 1
        sWriteCommand.bytes[0] = 0x01;
        sWriteCommand.bytes[1] = 0x1f;
        sWriteCommand.bytes[2] = 0x20;
        sWriteCommand.bytes[3] = 0x00;
        // reserved packet_payload
        am_hal_ble_blocking_hci_write(pBLE,
                                      AM_HAL_BLE_RAW,
                                      sWriteCommand.words,
                                      4);
        BLEIFn(ui32Module)->BLEDBG_b.IOCLKON = 1;

        //
        // Wait for the response.
        //
        while ( BLEIFn(ui32Module)->BSTATUS_b.BLEIRQ == 0 );

        am_hal_ble_blocking_hci_read(pBLE, sResponse.words, 0);

        //
        // Re-enable BLE interrupts.
        //
        BLEIFn(ui32Module)->INTCLR = ui32IntEnable;
        BLEIFn(ui32Module)->INTEN = ui32IntEnable;
    }
    
    return AM_HAL_STATUS_SUCCESS;
}

//*****************************************************************************
//
// BLE init for BQB test
//set enable as 'true' to init as BQB test mode, 'false' back to default
//*****************************************************************************
uint32_t
am_util_ble_BQB_test_init(void *pHandle, bool enable)
{
    am_hal_ble_state_t *pBLE = pHandle;

    am_hal_ble_sleep_set(pBLE, false);
    am_hal_ble_plf_reg_write(pBLE, 0x43000004, 0xFFFFFFFF);

    if ( enable )
    {
        am_hal_ble_plf_reg_write(pBLE, 0x51800028, 0x0000209c);
    }
    else
    {
        am_hal_ble_plf_reg_write(pBLE, 0x51800028, 0x00003ff6);
    }

    am_hal_ble_plf_reg_write(pBLE, 0x45800070, 0x100);
    am_hal_ble_plf_reg_write(pBLE, 0x45800070, 0);

    return AM_HAL_STATUS_SUCCESS;
}

//*****************************************************************************
//
// Set the 32M crystal frequency
// based on the tested values at customer side.
// set trim value smaller in case of negative frequency offset
// ui32TrimValue: default is 0x400
//*****************************************************************************
uint32_t
am_util_ble_crystal_trim_set(void *pHandle, uint32_t ui32TrimValue)
{
    am_hal_ble_state_t *pBLE = pHandle;
    uint32_t RegValueMCGR;

    ui32TrimValue &= 0x7FF;

    am_hal_ble_plf_reg_read(pBLE, 0x43000004, &RegValueMCGR);
    //
    // Unlock the BLE registers.
    //
    am_hal_ble_plf_reg_write(pBLE, 0x43000004, 0xFFFFFFFF);
    am_hal_ble_plf_reg_write(pBLE, 0x43800004, ui32TrimValue);
    am_hal_ble_plf_reg_write(pBLE, 0x43000004, RegValueMCGR);

    return AM_HAL_STATUS_SUCCESS;
}

//*****************************************************************************
//
// Manually enable/disable transmitter to output carrier signal
// set ui8TxChannel as 0 to 0x27 for each transmit channel, 0xFF back to normal modulate mode
//
//*****************************************************************************
uint32_t
am_util_ble_hci_reset(void *pHandle)
{
    am_hal_ble_buffer(16) sWriteCommand;
    am_hal_ble_buffer(16) sResponse;
    am_hal_ble_state_t *pBLE = pHandle;
    uint32_t ui32IntEnable;

    am_hal_ble_sleep_set(pBLE, false);
    uint32_t ui32Module = pBLE->ui32Module;

    // issue the HCI command with to reset hci
    sWriteCommand.bytes[0] = 0x01;
    sWriteCommand.bytes[1] = 0x03;
    sWriteCommand.bytes[2] = 0x0c;
    sWriteCommand.bytes[3] = 0x00;
    //
    // Temporarily disable BLE interrupts.
    //
    ui32IntEnable = BLEIFn(ui32Module)->INTEN;
    BLEIFn(ui32Module)->INTEN = 0;

    am_hal_ble_blocking_hci_write(pBLE,
                                  AM_HAL_BLE_RAW,
                                  sWriteCommand.words,
                                  4);
    BLEIFn(ui32Module)->BLEDBG_b.IOCLKON = 1;

    //
    // Wait for the response.
    //
    for (uint32_t i = 0; i < 1000; i++)
    {
        if ( BLEIFn(ui32Module)->BSTATUS_b.BLEIRQ != 0 )
        {
            break;
        }
        else if (i == (1000 - 1))
        {
            return AM_HAL_BLE_NO_HCI_RESPONSE;
        }
        else
        {
            am_util_delay_ms(1);
        }
    }

    am_hal_ble_blocking_hci_read(pBLE, sResponse.words, 0);

    //
    // Re-enable BLE interrupts.
    //
    BLEIFn(ui32Module)->INTCLR = ui32IntEnable;
    BLEIFn(ui32Module)->INTEN = ui32IntEnable;

    return AM_HAL_STATUS_SUCCESS;
}

//*****************************************************************************
//
//to do directly output modulation signal. change channel ranges from 0 to 0x27, pattern from 0 to 7.
//
//*****************************************************************************
uint32_t
am_util_ble_trasmitter_test_ex(void *pHandle, uint8_t channel, uint8_t pattern)
{
    am_hal_ble_buffer(16) sWriteCommand;
    am_hal_ble_buffer(16) sResponse;
    am_hal_ble_state_t *pBLE = pHandle;
    uint32_t ui32IntEnable;

    am_hal_ble_sleep_set(pBLE, false);
    uint32_t ui32Module = pBLE->ui32Module;

    // issue the HCI command with to TX carrier wave
    sWriteCommand.bytes[0] = 0x01;
    sWriteCommand.bytes[1] = 0x1E;
    sWriteCommand.bytes[2] = 0x20;
    sWriteCommand.bytes[3] = 0x03;
    sWriteCommand.bytes[4] = channel;
    sWriteCommand.bytes[5] = 0x25;
    sWriteCommand.bytes[6] = pattern;

    //
    // Temporarily disable BLE interrupts.
    //
    ui32IntEnable = BLEIFn(ui32Module)->INTEN;
    BLEIFn(ui32Module)->INTEN = 0;

    am_hal_ble_blocking_hci_write(pBLE,
                                  AM_HAL_BLE_RAW,
                                  sWriteCommand.words,
                                  7);
    BLEIFn(ui32Module)->BLEDBG_b.IOCLKON = 1;

    //
    // Wait for the response.
    //
    for (uint32_t i = 0; i < 100; i++)
    {
        if (BLEIFn(ui32Module)->BSTATUS_b.BLEIRQ != 0)
        {
            break;
        }
        else if (i == (100 - 1))
        {
            return AM_HAL_BLE_NO_HCI_RESPONSE;
        }
        else
        {
            am_util_delay_ms(1);
        }
    }

    am_hal_ble_blocking_hci_read(pBLE, sResponse.words, 0);

    //
    // Re-enable BLE interrupts.
    //
    BLEIFn(ui32Module)->INTCLR = ui32IntEnable;
    BLEIFn(ui32Module)->INTEN = ui32IntEnable;
    return AM_HAL_STATUS_SUCCESS;
}

//*****************************************************************************
//
//to do directly receiver test. change channel ranges from 0 to 0x27, return received packets in 100ms.
//
//*****************************************************************************

uint32_t
am_util_ble_receiver_test_ex(void *pHandle, uint8_t channel, uint32_t *recvpackets)
{
    am_hal_ble_buffer(16) sWriteCommand;
    am_hal_ble_buffer(16) sResponse;
    am_hal_ble_state_t *pBLE = pHandle;
    uint32_t ui32IntEnable;

    uint32_t ui32Module = pBLE->ui32Module;
    am_hal_ble_sleep_set(pBLE, false);

    sWriteCommand.bytes[0] = 0x01;
    sWriteCommand.bytes[1] = 0x1d;
    sWriteCommand.bytes[2] = 0x20;
    sWriteCommand.bytes[3] = 0x01;
    sWriteCommand.bytes[4] = channel;

    //
    // Temporarily disable BLE interrupts.
    //
    ui32IntEnable = BLEIFn(ui32Module)->INTEN;
    BLEIFn(ui32Module)->INTEN = 0;

    // reserved packet_payload
    am_hal_ble_blocking_hci_write(pBLE,
                                  AM_HAL_BLE_RAW,
                                  sWriteCommand.words,
                                  5);
    BLEIFn(ui32Module)->BLEDBG_b.IOCLKON = 1;

    //
    // Wait for the response.
    //
    while ( BLEIFn(ui32Module)->BSTATUS_b.BLEIRQ == 0 );
    am_hal_ble_blocking_hci_read(pBLE, sResponse.words, 0);

    am_util_delay_ms(100);

// issue the HCI command with to stop test for the channel 1
    sWriteCommand.bytes[0] = 0x01;
    sWriteCommand.bytes[1] = 0x1f;
    sWriteCommand.bytes[2] = 0x20;
    sWriteCommand.bytes[3] = 0x00;
    // reserved packet_payload
    am_hal_ble_blocking_hci_write(pBLE,
                                  AM_HAL_BLE_RAW,
                                  sWriteCommand.words,
                                  4);
    BLEIFn(ui32Module)->BLEDBG_b.IOCLKON = 1;

    //
    // Wait for the response.
    //
    while ( BLEIFn(ui32Module)->BSTATUS_b.BLEIRQ == 0 );

    am_hal_ble_blocking_hci_read(pBLE, sResponse.words, 0);
    *recvpackets = (sResponse.bytes[8] << 8) + sResponse.bytes[7];
    //
    // Re-enable BLE interrupts.
    //
    BLEIFn(ui32Module)->INTCLR = ui32IntEnable;
    BLEIFn(ui32Module)->INTEN = ui32IntEnable;

    return AM_HAL_STATUS_SUCCESS;
}

//*****************************************************************************
//
//to directly output carrier wave. change channel ranges from 0 to 0x27.
//
//*****************************************************************************
uint32_t
am_util_ble_set_carrier_wave_ex(void *pHandle, uint8_t channel)
{
    am_hal_ble_buffer(16) sWriteCommand;
    am_hal_ble_buffer(16) sResponse;
    am_hal_ble_state_t *pBLE = pHandle;
    uint32_t ui32IntEnable;

    // channel 0xFF to disable the constant transmission
    if ( channel == 0xFF )
    {
        am_util_ble_transmitter_control(pBLE, false);
        return AM_HAL_STATUS_SUCCESS;
    }

    am_hal_ble_sleep_set(pBLE, false);
    uint32_t ui32Module = pBLE->ui32Module;

    // issue the HCI command with to TX carrier wave
    sWriteCommand.bytes[0] = 0x01;
    sWriteCommand.bytes[1] = 0x1E;
    sWriteCommand.bytes[2] = 0x20;
    sWriteCommand.bytes[3] = 0x03;
    sWriteCommand.bytes[4] = channel;
    sWriteCommand.bytes[5] = 0x25;
    sWriteCommand.bytes[6] = 0x00;

    //
    // Temporarily disable BLE interrupts.
    //
    ui32IntEnable = BLEIFn(ui32Module)->INTEN;
    BLEIFn(ui32Module)->INTEN = 0;

    am_hal_ble_blocking_hci_write(pBLE,
                                  AM_HAL_BLE_RAW,
                                  sWriteCommand.words,
                                  7);
    BLEIFn(ui32Module)->BLEDBG_b.IOCLKON = 1;

    //
    // Wait for the response.
    //
    for (uint32_t i = 0; i < 100; i++)
    {
        if (BLEIFn(ui32Module)->BSTATUS_b.BLEIRQ != 0)
        {
            break;
        }
        else if (i == (100 - 1))
        {
            return AM_HAL_BLE_NO_HCI_RESPONSE;
        }
        else
        {
            am_util_delay_ms(1);
        }
    }

    am_hal_ble_blocking_hci_read(pBLE, sResponse.words, 0);

    //
    // Re-enable BLE interrupts.
    //
    BLEIFn(ui32Module)->INTCLR = ui32IntEnable;
    BLEIFn(ui32Module)->INTEN = ui32IntEnable;

    am_util_ble_transmitter_control(pBLE, true);

    return AM_HAL_STATUS_SUCCESS;
}

//*****************************************************************************
//
// Manually enable/disable transmitter to output carrier wave signal
// set ui8TxChannel as 0 to 0x27 for each transmit channel, 0xFF back to normal modulate mode
//
//*****************************************************************************
uint32_t
am_util_ble_transmitter_control_ex(void *pHandle, uint8_t ui8TxChannel)
{
    return am_util_ble_set_carrier_wave_ex(pHandle, ui8TxChannel);
}

//*****************************************************************************
//
//to directly output constant modulation signal. change channel from 0 to 0x27.
//
//*****************************************************************************
uint32_t
am_util_ble_set_constant_transmission_ex(void *pHandle, uint8_t channel)
{
    am_hal_ble_buffer(16) sWriteCommand;
    am_hal_ble_buffer(16) sResponse;
    am_hal_ble_state_t *pBLE = pHandle;
    uint32_t ui32IntEnable;

    // channel 0xFF to disable the constant transmission
    if ( channel == 0xFF )
    {
        am_util_ble_set_constant_transmission(pBLE, false);
        return AM_HAL_STATUS_SUCCESS;
    }

    uint32_t ui32Module = pBLE->ui32Module;
    am_util_ble_set_constant_transmission(pBLE, true);

    // issue the HCI command with to TX constant transmission
    sWriteCommand.bytes[0] = 0x01;
    sWriteCommand.bytes[1] = 0x1E;
    sWriteCommand.bytes[2] = 0x20;
    sWriteCommand.bytes[3] = 0x03;
    sWriteCommand.bytes[4] = channel;
    sWriteCommand.bytes[5] = 0x25;
    sWriteCommand.bytes[6] = 0x00;

    //
    // Temporarily disable BLE interrupts.
    //
    ui32IntEnable = BLEIFn(ui32Module)->INTEN;
    BLEIFn(ui32Module)->INTEN = 0;

    am_hal_ble_blocking_hci_write(pBLE,
                                  AM_HAL_BLE_RAW,
                                  sWriteCommand.words,
                                  7);
    BLEIFn(ui32Module)->BLEDBG_b.IOCLKON = 1;

    //
    // Wait for the response.
    //
    for (uint32_t i = 0; i < 100; i++)
    {
        if (BLEIFn(ui32Module)->BSTATUS_b.BLEIRQ != 0)
        {
            break;
        }
        else if (i == (100 - 1))
        {
            return AM_HAL_BLE_NO_HCI_RESPONSE;
        }
        else
        {
            am_util_delay_ms(1);
        }
    }

    am_hal_ble_blocking_hci_read(pBLE, sResponse.words, 0);

    //
    // Re-enable BLE interrupts.
    //
    BLEIFn(ui32Module)->INTCLR = ui32IntEnable;
    BLEIFn(ui32Module)->INTEN = ui32IntEnable;

    return AM_HAL_STATUS_SUCCESS;
}

//*****************************************************************************
//
// read current modex value from BLEIP
//*****************************************************************************
uint32_t
am_util_ble_read_modex_value(void *pHandle)
{
    am_hal_ble_state_t *pBLE = pHandle;
    uint32_t temp = 0;
    if (APOLLO3_GE_B0)
    {
        // for B0 Chip,the modex value address is changed to 0x20006874
        am_hal_ble_plf_reg_read(pBLE, 0x20006874, &temp);
    }
    else
    {
        am_hal_ble_plf_reg_read(pBLE, 0x20006070, &temp);
    }
    return temp;
}
