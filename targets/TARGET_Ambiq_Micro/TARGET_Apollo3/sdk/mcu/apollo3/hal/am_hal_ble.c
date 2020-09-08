//*****************************************************************************
//
//! @file am_hal_ble.c
//!
//! @brief HAL functions for the BLE interface.
//!
//! @addtogroup
//! @ingroup
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
#include <string.h>
#include "am_mcu_apollo.h"
#include "am_hal_ble_patch.h"
#include "am_hal_ble_patch_b0.h"

//*****************************************************************************
//
// Globals
//
//*****************************************************************************
am_hal_ble_state_t g_sBLEState[AM_REG_BLEIF_NUM_MODULES];

//*****************************************************************************
//
// Helper macros for rev B0 parts.
//
//*****************************************************************************
#define BLEIF_INTSTAT_BLECSSTATN_Msk    BLEIF_INTSTAT_B2MSHUTDN_Msk
#define BLEIF_INTSTAT_BLECIRQN_Msk      BLEIF_INTSTAT_B2MACTIVE_Msk

#define SKIP_FALLING_EDGES 0

//*****************************************************************************
//
// SPI "options"
//
// These values affect the behavior of the BLE HAL in regards to the SPI bus,
// but end users aren't likely to need to modify them. They are collected here
// for testing and debugging purposes.
//
//*****************************************************************************
// The amount of extra delay to add between successive SPI TX packets (in
// microseconds).
#define AM_BLE_TX_PACKET_SPACING_US     1

// The BLE core takes a little while to wake up from a fresh boot, which means
// that the patch_apply function might time-out on the first few tries. Set
// this variable to let it try again for a finite number of trials.
#define AM_BLE_NUM_PATCH_TRIALS         5000

// Patch complete can also take some time.
#define AM_BLE_NUM_PATCH_CMP_TRIALS     5000

// How long the MCU should wait for SPI_STATUS before assuming the BLE core is
// busy (measured in 10 us increments).
#define AM_BLE_STATUS_TIMEOUT           300

//*****************************************************************************
//
// Private types.
//
//*****************************************************************************
#define AM_HAL_MAGIC_BLE        0x775230

#define AM_HAL_BLE_CHK_HANDLE(h)                                              \
    ((h) && ((am_hal_handle_prefix_t *)(h))->s.bInit                          \
     && (((am_hal_handle_prefix_t *)(h))->s.magic == AM_HAL_MAGIC_BLE))

//*****************************************************************************
//
// BLE Core maximum patch packet size.
//
// Specified as part of the protocol.
//
//*****************************************************************************
#define MAX_PATCH_PACKET_LEN    0x80

//*****************************************************************************
//
// Some of the NationZ register addresses are different between A1/A2 and B0.
//
//*****************************************************************************

#define AM_HAL_BLE_IP_RAM_32K_CLOCK_ADDR_A1         0x20006054
#define AM_HAL_BLE_IP_RAM_MODEX_TRIM_ADDR_A1        0x20006070
#define AM_HAL_BLE_IP_RAM_POWER_LEVEL_ADDR_A1       0x20006038
#define AM_HAL_BLE_IP_RAM_SLEEP_ENABLE_ADDR_A1      (0x200067b8 + 0x0c)

#define AM_HAL_BLE_IP_RAM_32K_CLOCK_ADDR_B0         0x20006858
#define AM_HAL_BLE_IP_RAM_MODEX_TRIM_ADDR_B0        0x20006874
#define AM_HAL_BLE_IP_RAM_POWER_LEVEL_ADDR_B0       0x20006838
#define AM_HAL_BLE_IP_RAM_SLEEP_ENABLE_ADDR_B0      (0x20006e0c + 0x0c)

//*****************************************************************************
//
// Static function prototypes.
//
//*****************************************************************************
static bool am_hal_ble_bus_lock(am_hal_ble_state_t *pBle);
static void am_hal_ble_bus_release(am_hal_ble_state_t *pBle);
static uint32_t am_hal_ble_fifo_drain(void *pHandle);
static void am_hal_ble_fifo_read(void *pHandle, uint32_t *pui32Data, uint32_t ui32NumBytes);
static bool am_hal_ble_check_status(am_hal_ble_state_t *pBle);
static bool am_hal_ble_check_irq(am_hal_ble_state_t *pBle);
static uint32_t am_hal_ble_cmd_write(void *pHandle, am_hal_ble_transfer_t *psTransfer);
static uint32_t am_hal_ble_load_modex_trim_set(void *pHandle);
static uint32_t nonblocking_write(am_hal_ble_state_t *pBle, am_hal_ble_transfer_t *psTransfer);
static uint32_t nonblocking_read(am_hal_ble_state_t *pBle, am_hal_ble_transfer_t *psTransfer);
static uint8_t am_hal_ble_read_trimdata_from_info1(void);

//*****************************************************************************
//
// Look up Table for NZ CRC16 generation
//
//*****************************************************************************
static const uint16_t ccitt_table[] =
{
    0x0000, 0x8005, 0x800F, 0x000A, 0x801B, 0x001E, 0x0014, 0x8011,
    0x8033, 0x0036, 0x003C, 0x8039, 0x0028, 0x802D, 0x8027, 0x0022,
    0x8063, 0x0066, 0x006C, 0x8069, 0x0078, 0x807D, 0x8077, 0x0072,
    0x0050, 0x8055, 0x805F, 0x005A, 0x804B, 0x004E, 0x0044, 0x8041,
    0x80C3, 0x00C6, 0x00CC, 0x80C9, 0x00D8, 0x80DD, 0x80D7, 0x00D2,
    0x00F0, 0x80F5, 0x80FF, 0x00FA, 0x80EB, 0x00EE, 0x00E4, 0x80E1,
    0x00A0, 0x80A5, 0x80AF, 0x00AA, 0x80BB, 0x00BE, 0x00B4, 0x80B1,
    0x8093, 0x0096, 0x009C, 0x8099, 0x0088, 0x808D, 0x8087, 0x0082,
    0x8183, 0x0186, 0x018C, 0x8189, 0x0198, 0x819D, 0x8197, 0x0192,
    0x01B0, 0x81B5, 0x81BF, 0x01BA, 0x81AB, 0x01AE, 0x01A4, 0x81A1,
    0x01E0, 0x81E5, 0x81EF, 0x01EA, 0x81FB, 0x01FE, 0x01F4, 0x81F1,
    0x81D3, 0x01D6, 0x01DC, 0x81D9, 0x01C8, 0x81CD, 0x81C7, 0x01C2,
    0x0140, 0x8145, 0x814F, 0x014A, 0x815B, 0x015E, 0x0154, 0x8151,
    0x8173, 0x0176, 0x017C, 0x8179, 0x0168, 0x816D, 0x8167, 0x0162,
    0x8123, 0x0126, 0x012C, 0x8129, 0x0138, 0x813D, 0x8137, 0x0132,
    0x0110, 0x8115, 0x811F, 0x011A, 0x810B, 0x010E, 0x0104, 0x8101,
    0x8303, 0x0306, 0x030C, 0x8309, 0x0318, 0x831D, 0x8317, 0x0312,
    0x0330, 0x8335, 0x833F, 0x033A, 0x832B, 0x032E, 0x0324, 0x8321,
    0x0360, 0x8365, 0x836F, 0x036A, 0x837B, 0x037E, 0x0374, 0x8371,
    0x8353, 0x0356, 0x035C, 0x8359, 0x0348, 0x834D, 0x8347, 0x0342,
    0x03C0, 0x83C5, 0x83CF, 0x03CA, 0x83DB, 0x03DE, 0x03D4, 0x83D1,
    0x83F3, 0x03F6, 0x03FC, 0x83F9, 0x03E8, 0x83ED, 0x83E7, 0x03E2,
    0x83A3, 0x03A6, 0x03AC, 0x83A9, 0x03B8, 0x83BD, 0x83B7, 0x03B2,
    0x0390, 0x8395, 0x839F, 0x039A, 0x838B, 0x038E, 0x0384, 0x8381,
    0x0280, 0x8285, 0x828F, 0x028A, 0x829B, 0x029E, 0x0294, 0x8291,
    0x82B3, 0x02B6, 0x02BC, 0x82B9, 0x02A8, 0x82AD, 0x82A7, 0x02A2,
    0x82E3, 0x02E6, 0x02EC, 0x82E9, 0x02F8, 0x82FD, 0x82F7, 0x02F2,
    0x02D0, 0x82D5, 0x82DF, 0x02DA, 0x82CB, 0x02CE, 0x02C4, 0x82C1,
    0x8243, 0x0246, 0x024C, 0x8249, 0x0258, 0x825D, 0x8257, 0x0252,
    0x0270, 0x8275, 0x827F, 0x027A, 0x826B, 0x026E, 0x0264, 0x8261,
    0x0220, 0x8225, 0x822F, 0x022A, 0x823B, 0x023E, 0x0234, 0x8231,
    0x8213, 0x0216, 0x021C, 0x8219, 0x0208, 0x820D, 0x8207, 0x0202
};

//*****************************************************************************
//
// Helper macros for delays.
//
//*****************************************************************************
#define delay_ms(ms)        am_hal_flash_delay(FLASH_CYCLES_US(1000 * (ms)))
#define delay_us(us)        am_hal_flash_delay(FLASH_CYCLES_US(us))

#define WHILE_TIMEOUT_MS(expr, timeout, error)                                \
    {                                                                         \
        uint32_t ui32Timeout = 0;                                             \
        while (expr)                                                          \
        {                                                                     \
            if (ui32Timeout == (timeout * 1000))                              \
            {                                                                 \
                return error;                                                 \
            }                                                                 \
                                                                              \
            delay_us(1);                                                      \
            ui32Timeout++;                                                    \
        }                                                                     \
    }

#define WHILE_TIMEOUT_MS_BREAK(expr, timeout, error)                                \
    {                                                                         \
        uint32_t ui32Timeout = 0;                                             \
        while (expr)                                                          \
        {                                                                     \
            if (ui32Timeout == (timeout * 1000))                              \
            {                                                                 \
                break;                                                 \
            }                                                                 \
                                                                              \
            delay_us(1);                                                      \
            ui32Timeout++;                                                    \
        }                                                                     \
    }
//*****************************************************************************
//
// Helper function for checking BLE data.
//
//*****************************************************************************
static bool
buffer_compare(void *b1, void *b2, uint32_t len)
{
    uint8_t *p1 = b1;
    uint8_t *p2 = b2;

    for (uint32_t i = 0; i < len; i++)
    {
        if (p1[i] != p2[i])
        {
            return false;
        }
    }

    return true;
}

//*****************************************************************************
//
// Helper function for CRC caculation of BLE patch.
//
//*****************************************************************************
static uint16_t
am_hal_ble_crc_nz(uint8_t *pui8Data, uint32_t len)
{
    uint16_t ui16CurValue = 0;
    uint32_t i;

    for (i = 0; i < len; i++)
    {
        ui16CurValue = ccitt_table[(((uint8_t)(ui16CurValue >> 8)) ^ pui8Data[i]) & 0xFF] ^ (ui16CurValue << 8);
    }

    return ((ui16CurValue ^ 0) & ((1 << 16) - 1));
}

//*****************************************************************************
//
// Default options for the BLE module.
//
//*****************************************************************************
const am_hal_ble_config_t am_hal_ble_default_config =
{
    // Configure the HCI interface clock for 6 MHz
    .ui32SpiClkCfg = AM_HAL_BLE_HCI_CLK_DIV8,

    // Set HCI read and write thresholds to 32 bytes each.
    .ui32ReadThreshold = 32,
    .ui32WriteThreshold = 32,

    // The MCU will supply the clock to the BLE core.
    .ui32BleClockConfig = AM_HAL_BLE_CORE_MCU_CLK,

    // Default settings for expected BLE clock drift.
    .ui32ClockDrift = 0,
    .ui32SleepClockDrift = 50,

    // Default setting - AGC Enabled
    .bAgcEnabled = true,

    // Default setting - Sleep Algo enabled
    .bSleepEnabled = true,

    // Apply the default patches when am_hal_ble_boot() is called.
    .bUseDefaultPatches = true,
};

//*****************************************************************************
//
// Function for controlling the WAKE signal.
//
//*****************************************************************************
uint32_t
am_hal_ble_wakeup_set(void *pHandle, uint32_t ui32Mode)
{
    am_hal_ble_state_t *pBle = pHandle;

    //
    // Check the handle.
    //
    if ( !AM_HAL_BLE_CHK_HANDLE(pHandle) )
    {
        return AM_HAL_STATUS_INVALID_HANDLE;
    }

//    am_hal_debug_gpio_set(BLE_DEBUG_TRACE_08);

    if ( ui32Mode )
    {
        BLEIFn(pBle->ui32Module)->BLECFG_b.WAKEUPCTL = BLEIF_BLECFG_WAKEUPCTL_ON;
        am_hal_debug_gpio_set(BLE_DEBUG_TRACE_08);
    }
    else
    {
#ifndef AM_DISABLE_BLE_SLEEP
        BLEIFn(pBle->ui32Module)->BLECFG_b.WAKEUPCTL = BLEIF_BLECFG_WAKEUPCTL_OFF;
        am_hal_debug_gpio_clear(BLE_DEBUG_TRACE_08);
#endif
    }

    return AM_HAL_STATUS_SUCCESS;

//    am_hal_debug_gpio_clear(BLE_DEBUG_TRACE_08);
}

//*****************************************************************************
//
// Buffer for patch data.
//
//*****************************************************************************
am_hal_ble_buffer(128 + 4) g_psPatchBuffer;

//*****************************************************************************
//
// Initialize the global variables associated with a BLE module, and return its
// handle.
//
//*****************************************************************************
uint32_t
am_hal_ble_initialize(uint32_t ui32Module, void **ppHandle)
{
    //
    // Check the arguments.
    //
    if (ui32Module >= AM_REG_BLEIF_NUM_MODULES)
    {
        return AM_HAL_STATUS_OUT_OF_RANGE;
    }

    if (!ppHandle)
    {
        return AM_HAL_STATUS_INVALID_ARG;
    }

    //
    // Check if the handle is unallocated.
    //
    if (g_sBLEState[ui32Module].prefix.s.bInit)
    {
        return AM_HAL_STATUS_INVALID_OPERATION;
    }

    //
    // Initialize the handle.
    //
    memset(&g_sBLEState[ui32Module].sCurrentTransfer, 0, sizeof(am_hal_ble_transfer_t));
    memset(&g_sBLEState[ui32Module].sSavedTransfer, 0, sizeof(am_hal_ble_transfer_t));

    g_sBLEState[ui32Module].prefix.s.bInit = true;
    g_sBLEState[ui32Module].prefix.s.magic = AM_HAL_MAGIC_BLE;
    g_sBLEState[ui32Module].ui32Module = ui32Module;
    g_sBLEState[ui32Module].ui32TransferIndex = 0;
    g_sBLEState[ui32Module].bPatchComplete = 0;
    g_sBLEState[ui32Module].bContinuePacket = 0;
    g_sBLEState[ui32Module].bSavedPacket = 0;
    g_sBLEState[ui32Module].bBusy = 0;
    g_sBLEState[ui32Module].bCmdComplete = 0;
    g_sBLEState[ui32Module].bDmaComplete = 0;
    g_sBLEState[ui32Module].bFlowControlComplete = 0;
    g_sBLEState[ui32Module].bUseDefaultPatches = false;

    //
    // Pass the handle back to the caller.
    //
    *ppHandle = &g_sBLEState[ui32Module];

    //
    // Return the status.
    //
    return AM_HAL_STATUS_SUCCESS;
} // am_hal_ble_initialize()

//*****************************************************************************
//
// Initialize the global variables associated with a BLE module, and return its
// handle.
//
//*****************************************************************************
uint32_t
am_hal_ble_deinitialize(void *pHandle)
{
    am_hal_ble_state_t *pBLE = (am_hal_ble_state_t *)pHandle;

    //
    // Check the handle.
    //
    if (!AM_HAL_BLE_CHK_HANDLE(pHandle))
    {
        return AM_HAL_STATUS_INVALID_HANDLE;
    }

    //
    // Initialize the handle.
    //
    memset(&(pBLE->sCurrentTransfer), 0, sizeof(am_hal_ble_transfer_t));

    pBLE->prefix.s.bInit = false;
    pBLE->prefix.s.magic = 0;
    pBLE->ui32Module = 0;
    pBLE->ui32TransferIndex = 0;
    pBLE->bPatchComplete = 0;
    pBLE->bContinuePacket = 0;
    pBLE->bSavedPacket = 0;
    pBLE->bBusy = 0;
    pBLE->bCmdComplete = 0;
    pBLE->bDmaComplete = 0;
    pBLE->bFlowControlComplete = 0;

    //
    // Return the status.
    //
    return AM_HAL_STATUS_SUCCESS;
} // am_hal_ble_deinitialize()

//*****************************************************************************
//
// Configuration function.
//
//*****************************************************************************
uint32_t
am_hal_ble_config(void *pHandle, const am_hal_ble_config_t *psConfig)
{
    uint32_t ui32Module;
    uint32_t ui32BleClkConfig;

    //
    // Check the handle.
    //
    if (!AM_HAL_BLE_CHK_HANDLE(pHandle))
    {
        return AM_HAL_STATUS_INVALID_HANDLE;
    }

    //
    // Handle is good, so get the module number.
    //
    ui32Module = ((am_hal_ble_state_t *) pHandle)->ui32Module;

    //
    // Configure the SPI.
    //
    BLEIFn(ui32Module)->MSPICFG = 0x3;
    BLEIFn(ui32Module)->MSPICFG_b.RDFC = 0;
    BLEIFn(ui32Module)->MSPICFG_b.WTFC = 0;
    BLEIFn(ui32Module)->MSPICFG_b.WTFCPOL = 1;
    BLEIFn(ui32Module)->FIFOTHR_b.FIFOWTHR = psConfig->ui32WriteThreshold;
    BLEIFn(ui32Module)->FIFOTHR_b.FIFORTHR = psConfig->ui32ReadThreshold;
    BLEIFn(ui32Module)->FIFOCTRL |= BLEIF_FIFOCTRL_POPWR_Msk;

    //
    // Clock configuration register writes need to be combined to a single
    // operation.
    //
    ui32BleClkConfig =  _VAL2FLD(BLEIF_CLKCFG_FSEL, psConfig->ui32SpiClkCfg);
    ui32BleClkConfig |= _VAL2FLD(BLEIF_CLKCFG_IOCLKEN, 1);

    if (psConfig->ui32BleClockConfig == AM_HAL_BLE_CORE_MCU_CLK)
    {
        ui32BleClkConfig |= _VAL2FLD(BLEIF_CLKCFG_CLK32KEN, 1);
    }

    BLEIFn(ui32Module)->CLKCFG = ui32BleClkConfig;

    if (APOLLO3_A1)
    {
        //
        // Modify the BLE core's NVDS settings to match our configuration.
        //
        uint8_t *pui8NVDSData = (uint8_t *) am_ble_nvds_patch.pui32Data;

        //
        // Set the clock source.
        //
        pui8NVDSData[AM_HAL_BLE_NVDS_CLOCKSOURCE_OFFSET + 3] =
            (psConfig->ui32BleClockConfig & 0xFF);

        //
        // Set the expected BLE clock drift PPM
        //
        pui8NVDSData[AM_HAL_BLE_NVDS_CLOCKDRIFT_OFFSET + 3] =
            (psConfig->ui32ClockDrift & 0x00FF);

        pui8NVDSData[AM_HAL_BLE_NVDS_CLOCKDRIFT_OFFSET + 4] =
            (psConfig->ui32ClockDrift & 0xFF00) >> 8;

        //
        // Set the sleep clock drift PPM.
        //
        pui8NVDSData[AM_HAL_BLE_NVDS_SLEEPCLOCKDRIFT_OFFSET + 3] =
            (psConfig->ui32SleepClockDrift & 0x00FF);

        pui8NVDSData[AM_HAL_BLE_NVDS_SLEEPCLOCKDRIFT_OFFSET + 4] =
            (psConfig->ui32SleepClockDrift & 0xFF00) >> 8;

        //
        // Configure Sleep mode.
        //
        pui8NVDSData[AM_HAL_BLE_NVDS_SLEEPENABLE_OFFSET + 3] = (psConfig->bSleepEnabled == true) ? 1 : 0;
        //
        // Configure AGC.
        //
        pui8NVDSData[AM_HAL_BLE_NVDS_AGC_OFFSET + 3] = (psConfig->bAgcEnabled == true) ? 1 : 0;

        //
        // Update the CRC.
        //
        am_ble_nvds_patch.ui32CRC = am_hal_ble_crc_nz(pui8NVDSData,
                                                      am_ble_nvds_patch.ui32Length);
    }

    //
    // Save the addresses to the patches we intend to use.
    //
    g_sBLEState[ui32Module].bUseDefaultPatches = psConfig->bUseDefaultPatches;

    //
    // Return the status.
    //
    return AM_HAL_STATUS_SUCCESS;
} // am_hal_ble_config()

//*****************************************************************************
//
// Enable BLE
//
//*****************************************************************************
uint32_t
am_hal_ble_power_control(void *pHandle, uint32_t ui32PowerState)
{
    uint32_t ui32Module;

    //
    // BLE buck is shared by Burst as well
    // Enable the BLE buck trim values if in use
    //
    am_hal_pwrctrl_blebuck_trim();

    //
    // Check the handle.
    //
    if ( !AM_HAL_BLE_CHK_HANDLE(pHandle) )
    {
        return AM_HAL_STATUS_INVALID_HANDLE;
    }

    //
    // Handle is good, so get the module number.
    //
    ui32Module = ((am_hal_ble_state_t *) pHandle)->ui32Module;

    if (ui32PowerState == AM_HAL_BLE_POWER_ACTIVE)
    {
        //
        // Don't run this initialization if the BLE is already enabled.
        //
        if ( PWRCTRL->DEVPWRSTATUS_b.BLEL == 0)
        {
            MCUCTRL->FEATUREENABLE |= 1;
            WHILE_TIMEOUT_MS ( ((MCUCTRL->FEATUREENABLE & 0x7) != 0x7), 100,
                               AM_HAL_BLE_FEATURE_DISABLED );

            //
            // Enable the BLE module.
            //
            if (am_hal_pwrctrl_periph_enable(AM_HAL_PWRCTRL_PERIPH_BLEL) !=
                AM_HAL_STATUS_SUCCESS)
            {
                return AM_HAL_BLE_REGULATOR_FAILED;
            }

            //
            // Release the BLE module RESET, start the "power state machine", and
            // enable the clocks.
            //
            BLEIFn(ui32Module)->CLKCFG = _VAL2FLD(BLEIF_CLKCFG_CLK32KEN, 1);
            BLEIFn(ui32Module)->BLEDBG_b.DBGDATA = 1 << 14;

            //
            // The reset bit is different between A0 and subsequent revisions.
            //
            if ( APOLLO3_GE_A1 )
            {
                MCUCTRL->MISCCTRL_b.BLE_RESETN = 1;
            }
            else
            {
                AM_REGVAL(0x40020198) = 0x1 << 2;
            }

            delay_ms(5);
            BLEIFn(ui32Module)->BLECFG_b.PWRSMEN = 1;

            //
            // Wait for indication that the power is on.
            //
            WHILE_TIMEOUT_MS ( BLEIFn(ui32Module)->BSTATUS_b.PWRST != 3, 1000,
                               AM_HAL_BLE_POWERUP_INCOMPLETE );
        }
    }
    else if (ui32PowerState == AM_HAL_BLE_POWER_OFF)
    {
        //
        // Reverse of power-up. Disable clocks, set reset, then disable power.
        //
        BLEIFn(ui32Module)->CLKCFG = 0;
        BLEIF->BLEDBG_b.DBGDATA = 0;

        if ( APOLLO3_GE_A1 )
        {
            MCUCTRL->MISCCTRL_b.BLE_RESETN = 0;
        }
        else
        {
            AM_REGVAL(0x40020198) &= ~(0x1 << 2);
        }

        BLEIF->BLECFG_b.PWRSMEN = 0;

        if (am_hal_pwrctrl_periph_disable(AM_HAL_PWRCTRL_PERIPH_BLEL) !=
            AM_HAL_STATUS_SUCCESS)
        {
            return AM_HAL_BLE_SHUTDOWN_FAILED;
        }

        delay_us(100);
    }
    else
    {
        return AM_HAL_STATUS_INVALID_OPERATION;
    }

    //
    // Return the status.
    //
    return AM_HAL_STATUS_SUCCESS;
} // am_hal_ble_power_control()

//*****************************************************************************
//
// Perform all of the operations necessary to prepare  the BLE controller for
// HCI operation.
//
//*****************************************************************************
uint32_t
am_hal_ble_boot(void *pHandle)
{
    uint32_t ui32Status;

    //
    // Check the handle.
    //
    if (!AM_HAL_BLE_CHK_HANDLE(pHandle))
    {
        return AM_HAL_STATUS_INVALID_HANDLE;
    }

    //
    // The handle is good, so we can access it as a structure.
    //
    am_hal_ble_state_t *pBLE = pHandle;

    if (pBLE->bUseDefaultPatches)
    {
        //
        // The B0 silicon patching method is slightly different from A1. B0 silicon
        // does not require the Copy Patch method introduced for A1 silicon.
        //
        if (APOLLO3_A0 || APOLLO3_A1)
        {
            ui32Status = am_hal_ble_default_copy_patch_apply(pHandle);
            if (ui32Status != AM_HAL_STATUS_SUCCESS)
            {
                return ui32Status;
            }
        }

        //
        // Apply the BLE trim value
        //
        ui32Status = am_hal_ble_default_trim_set_ramcode(pHandle);
        if (ui32Status != AM_HAL_STATUS_SUCCESS)
        {
            return ui32Status;
        }

        //
        // Apply the NVDS patch.
        //
        ui32Status = am_hal_ble_default_patch_apply(pHandle);
        if (ui32Status != AM_HAL_STATUS_SUCCESS)
        {
            return ui32Status;
        }

        //
        // Complete the patching step
        //
        ui32Status = am_hal_ble_patch_complete(pHandle);
        if (ui32Status != AM_HAL_STATUS_SUCCESS)
        {
            return ui32Status;
        }
    }

    if (am_hal_ble_check_32k_clock(pBLE) == AM_HAL_STATUS_FAIL)
    {
        return AM_HAL_BLE_32K_CLOCK_UNSTABLE;
    }
    else
    {
        return AM_HAL_STATUS_SUCCESS;
    }
} // am_hal_ble_boot()

//*****************************************************************************
//
// Apply a patch.
//
// Returns 0 for success or a numerical error code for failures.
//
//*****************************************************************************
uint32_t
am_hal_ble_patch_apply(void *pHandle, am_hal_ble_patch_t *psPatch)
{
    uint8_t pui8ExpectedResponse[32];
    uint32_t ui32ErrorStatus;
    uint32_t ui32Trial;

    //
    // Check the handle.
    //
    if (!AM_HAL_BLE_CHK_HANDLE(pHandle))
    {
        return AM_HAL_STATUS_INVALID_HANDLE;
    }

    uint32_t ui32Module = ((am_hal_ble_state_t *) pHandle)->ui32Module;
    am_hal_ble_transfer_t sTransfer;
    am_hal_ble_buffer(16) psPatchBuffer;

    //
    // Send a header packet.
    //
    psPatchBuffer.bytes[0] = 0x01;
    psPatchBuffer.bytes[1] = psPatch->ui32Type;
    psPatchBuffer.bytes[2] = 0xF1;
    psPatchBuffer.bytes[3] = 0x02;
    psPatchBuffer.bytes[4] = (psPatch->ui32Length & 0xFF);
    psPatchBuffer.bytes[5] = ((psPatch->ui32Length >> 8) & 0xFF);

    //
    // This first packet might take a few tries.
    //
    for ( ui32Trial = 0; ui32Trial < AM_BLE_NUM_PATCH_TRIALS; ui32Trial++)
    {
        ui32ErrorStatus = am_hal_ble_blocking_hci_write(pHandle,
                                                        AM_HAL_BLE_RAW,
                                                        psPatchBuffer.words,
                                                        6);

        if ( ui32ErrorStatus == AM_HAL_STATUS_SUCCESS )
        {
            break;
        }
    }

    if (ui32ErrorStatus != AM_HAL_STATUS_SUCCESS)
    {
        return ui32ErrorStatus;
    }

    //
    // Wait for the header response. It should be 5 bytes long.
    //
    WHILE_TIMEOUT_MS ( BLEIFn(ui32Module)->BSTATUS_b.BLEIRQ == 0, 1000,
                       AM_HAL_BLE_NO_HCI_RESPONSE );

    memset(&sTransfer, 0, sizeof(am_hal_ble_transfer_t));
    sTransfer.ui8Command = AM_HAL_BLE_READ;
    sTransfer.pui32Data = psPatchBuffer.words;
    sTransfer.ui16Length = 5;

    ui32ErrorStatus = am_hal_ble_blocking_transfer(pHandle, &sTransfer);
    if ( ui32ErrorStatus != AM_HAL_STATUS_SUCCESS )
    {
        return ui32ErrorStatus;
    }

    pui8ExpectedResponse[0] = 0x04;
    pui8ExpectedResponse[1] = psPatch->ui32Type;
    pui8ExpectedResponse[2] = 0xF1;
    pui8ExpectedResponse[3] = 0x01;
    pui8ExpectedResponse[4] = 0x00;

    if (!buffer_compare(psPatchBuffer.words, pui8ExpectedResponse, 5))
    {
        return AM_HAL_STATUS_FAIL;
    }

    //
    // Send all of the data, including the acknowledgements.
    //
    uint32_t ui32RemainingBytes = psPatch->ui32Length;
    uint32_t ui32Index = 0;

    while (ui32RemainingBytes)
    {
        //
        // Figure out how many bytes to send in the next packet.
        //
        uint32_t ui32TransferSize = (ui32RemainingBytes > MAX_PATCH_PACKET_LEN ?
                                     MAX_PATCH_PACKET_LEN : ui32RemainingBytes);

        //
        // Send a data header.
        //
        memset(&sTransfer, 0, sizeof(am_hal_ble_transfer_t));
        sTransfer.ui8Command = AM_HAL_BLE_WRITE;
        sTransfer.pui32Data = g_psPatchBuffer.words;
        sTransfer.ui16Length = ui32TransferSize + 4;
        sTransfer.bContinue = false;

        g_psPatchBuffer.bytes[0] = 0x01;
        g_psPatchBuffer.bytes[1] = psPatch->ui32Type;
        g_psPatchBuffer.bytes[2] = 0xF2;
        g_psPatchBuffer.bytes[3] = ui32TransferSize;

        // copy data into buffer
        memcpy(&g_psPatchBuffer.bytes[4], (uint8_t *)&(psPatch->pui32Data[ui32Index / 4]), ui32TransferSize);

        ui32ErrorStatus = am_hal_ble_blocking_transfer(pHandle, &sTransfer);
        if ( ui32ErrorStatus != AM_HAL_STATUS_SUCCESS )
        {
            return ui32ErrorStatus;
        }

        //
        // Read the acknowledgement.
        //
        WHILE_TIMEOUT_MS( BLEIFn(ui32Module)->BSTATUS_b.BLEIRQ == 0, 1000,
                          AM_HAL_BLE_NO_HCI_RESPONSE);

        memset(&sTransfer, 0, sizeof(am_hal_ble_transfer_t));
        sTransfer.ui8Command = AM_HAL_BLE_READ;
        sTransfer.pui32Data = psPatchBuffer.words;
        sTransfer.ui16Length = 5;

        ui32ErrorStatus = am_hal_ble_blocking_transfer(pHandle, &sTransfer);
        if ( ui32ErrorStatus != AM_HAL_STATUS_SUCCESS )
        {
            return ui32ErrorStatus;
        }

        pui8ExpectedResponse[0] = 0x04;
        pui8ExpectedResponse[1] = psPatch->ui32Type;
        pui8ExpectedResponse[2] = 0xF2;
        pui8ExpectedResponse[3] = 0x01;
        pui8ExpectedResponse[4] = 0x00;

        if (!buffer_compare(psPatchBuffer.words, pui8ExpectedResponse, 5))
        {
            return AM_HAL_STATUS_FAIL;
        }

        //
        // Update the tracking variables
        //
        ui32RemainingBytes -= ui32TransferSize;
        ui32Index += ui32TransferSize;
    }

    //
    // Send the CRC, and make sure we got it right.
    //
    psPatchBuffer.bytes[0] = 0x01;
    psPatchBuffer.bytes[1] = psPatch->ui32Type;
    psPatchBuffer.bytes[2] = 0xF3;
    psPatchBuffer.bytes[3] = 0x02;
    psPatchBuffer.bytes[4] = (psPatch->ui32CRC & 0xFF);
    psPatchBuffer.bytes[5] = ((psPatch->ui32CRC >> 8) & 0xFF);

    if (am_hal_ble_blocking_hci_write(pHandle, AM_HAL_BLE_RAW, psPatchBuffer.words, 6) !=
        AM_HAL_STATUS_SUCCESS)
    {
        return AM_HAL_STATUS_FAIL;
    }

    //
    // Wait for the header response. It should be 5 bytes long.
    //
    WHILE_TIMEOUT_MS( BLEIFn(ui32Module)->BSTATUS_b.BLEIRQ == 0, 1000,
                      AM_HAL_BLE_NO_HCI_RESPONSE );

    memset(&sTransfer, 0, sizeof(am_hal_ble_transfer_t));
    sTransfer.ui8Command = AM_HAL_BLE_READ;
    sTransfer.pui32Data = psPatchBuffer.words;
    sTransfer.ui16Length = 5;

    ui32ErrorStatus = am_hal_ble_blocking_transfer(pHandle, &sTransfer);
    if ( ui32ErrorStatus != AM_HAL_STATUS_SUCCESS )
    {
        return ui32ErrorStatus;
    }

    pui8ExpectedResponse[0] = 0x04;
    pui8ExpectedResponse[1] = psPatch->ui32Type;
    pui8ExpectedResponse[2] = 0xF3;
    pui8ExpectedResponse[3] = 0x01;
    pui8ExpectedResponse[4] = 0x00;

    if (!buffer_compare(psPatchBuffer.words, pui8ExpectedResponse, 5))
    {
        return AM_HAL_STATUS_FAIL;
    }
    else
    {
        return AM_HAL_STATUS_SUCCESS;
    }
} // am_hal_ble_patch_apply()

uint32_t
am_hal_ble_patch_copy_end_apply(void *pHandle)
{
    uint8_t pui8ExpectedResponse[32];
    uint32_t ui32ErrorStatus;
    uint32_t ui32Trial;

    //
    // Check the handle.
    //
    if (!AM_HAL_BLE_CHK_HANDLE(pHandle))
    {
        return AM_HAL_STATUS_INVALID_HANDLE;
    }

    uint32_t ui32Module = ((am_hal_ble_state_t *) pHandle)->ui32Module;
    am_hal_ble_transfer_t sTransfer;
    am_hal_ble_buffer(16) psPatchBuffer;

    //
    // Send a header packet.
    //
    psPatchBuffer.bytes[0] = 0x01;
    psPatchBuffer.bytes[1] = 0xEE;
    psPatchBuffer.bytes[2] = 0xF1;
    psPatchBuffer.bytes[3] = 0x02;
    psPatchBuffer.bytes[4] = 0x00;
    psPatchBuffer.bytes[5] = 0x00;

    //
    // This first packet might take a few tries.
    //
    for ( ui32Trial = 0; ui32Trial < AM_BLE_NUM_PATCH_TRIALS; ui32Trial++)
    {
        ui32ErrorStatus = am_hal_ble_blocking_hci_write(pHandle,
                                                        AM_HAL_BLE_RAW,
                                                        psPatchBuffer.words,
                                                        6);

        if ( ui32ErrorStatus == AM_HAL_STATUS_SUCCESS )
        {

            break;
        }
    }

    if (ui32ErrorStatus != AM_HAL_STATUS_SUCCESS)
    {

        return ui32ErrorStatus;
    }

    //
    // Wait for the header response. It should be 5 bytes long.
    //
    WHILE_TIMEOUT_MS( BLEIFn(ui32Module)->BSTATUS_b.BLEIRQ == 0, 1000, AM_HAL_BLE_NO_HCI_RESPONSE);

    memset(&sTransfer, 0, sizeof(am_hal_ble_transfer_t));
    sTransfer.ui8Command = AM_HAL_BLE_READ;
    sTransfer.pui32Data = psPatchBuffer.words;
    sTransfer.ui16Length = 5;

    ui32ErrorStatus = am_hal_ble_blocking_transfer(pHandle, &sTransfer);
    if ( ui32ErrorStatus != AM_HAL_STATUS_SUCCESS )
    {

        return ui32ErrorStatus;
    }

    pui8ExpectedResponse[0] = 0x04;
    pui8ExpectedResponse[1] = 0xEE;
    pui8ExpectedResponse[2] = 0xF1;
    pui8ExpectedResponse[3] = 0x01;
    pui8ExpectedResponse[4] = 0x00;

    if (!buffer_compare(psPatchBuffer.words, pui8ExpectedResponse, 5))
    {

        return AM_HAL_STATUS_FAIL;
    }
    return 0;
} // am_hal_ble_patch_copy_end_apply()

//*****************************************************************************
//
// Apply the default copy patch.
//
// Returns 0 for success or a numerical error code for failures.
//
//*****************************************************************************
uint32_t
am_hal_ble_default_copy_patch_apply(void *pHandle)
{
    uint32_t ui32Status;
    uint16_t ui16Crc;

    am_hal_ble_patch_t **psCopyPatch;

    psCopyPatch = am_hal_ble_default_copy_patches;

    ui16Crc = am_hal_ble_crc_nz((uint8_t*)(psCopyPatch[0]->pui32Data), psCopyPatch[0]->ui32Length);
    psCopyPatch[0]->ui32CRC = ui16Crc;
    ui32Status = am_hal_ble_patch_apply(pHandle, psCopyPatch[0]);
    if (ui32Status != AM_HAL_STATUS_SUCCESS)
    {
        return ui32Status;
    }

    ui32Status = am_hal_ble_patch_copy_end_apply(pHandle);
    if ( ui32Status != AM_HAL_STATUS_SUCCESS )
    {
        return ui32Status;
    }

    return AM_HAL_STATUS_SUCCESS;
}

//*****************************************************************************
//
// Apply the default patch.
//
// Returns 0 for success or a numerical error code for failures.
//
//*****************************************************************************
uint32_t
am_hal_ble_default_patch_apply(void *pHandle)
{
    uint32_t ui32Status, i = 0;
    uint16_t ui16Crc;
    uint32_t ui32NumPatches;
    am_hal_ble_patch_t **psDefaultPatches;

    if (APOLLO3_A0 || APOLLO3_A1)
    {
        ui32NumPatches = am_hal_ble_num_default_patches;
        psDefaultPatches = am_hal_ble_default_patches;
    }
    else
    {
        ui32NumPatches = am_hal_ble_num_default_patches_b0;
        psDefaultPatches = am_hal_ble_default_patches_b0;
    }

    for ( i = 0; i < ui32NumPatches; i++ )
    {
        ui16Crc = am_hal_ble_crc_nz((uint8_t*)(psDefaultPatches[i]->pui32Data), psDefaultPatches[i]->ui32Length);
        psDefaultPatches[i]->ui32CRC = ui16Crc;
        ui32Status = am_hal_ble_patch_apply(pHandle, psDefaultPatches[i]);
        if (ui32Status != AM_HAL_STATUS_SUCCESS)
        {
            return ui32Status;
        }
    }

    return AM_HAL_STATUS_SUCCESS;
} // am_hal_ble_default_patch_apply()

//*****************************************************************************
//
// Complete the patching process
//
//*****************************************************************************
uint32_t
am_hal_ble_patch_complete(void *pHandle)
{
    uint32_t ui32ErrorStatus;
    am_hal_ble_transfer_t sTransfer;
    am_hal_ble_buffer(12) sTxBuffer;
    am_hal_ble_buffer(12) sRxBuffer;
    uint32_t ui32Trial;

    am_hal_ble_state_t *pBLE = pHandle;

    //
    // Check the handle.
    //
    if (!AM_HAL_BLE_CHK_HANDLE(pHandle))
    {
        return AM_HAL_STATUS_INVALID_HANDLE;
    }

    uint32_t ui32Module = pBLE->ui32Module;

    //
    // Write the "patch complete" command.
    //
    memset(&sTransfer, 0, sizeof(am_hal_ble_transfer_t));
    sTransfer.ui8Command = AM_HAL_BLE_WRITE;
    sTransfer.pui32Data = sTxBuffer.words;
    sTransfer.ui16Length = 6;

    sTxBuffer.bytes[0] = 0x01;
    sTxBuffer.bytes[1] = 0xEE;
    sTxBuffer.bytes[2] = 0xF1;
    sTxBuffer.bytes[3] = 0x02;
    sTxBuffer.bytes[4] = 0x00;
    sTxBuffer.bytes[5] = 0x00;

    for ( ui32Trial = 0; ui32Trial < AM_BLE_NUM_PATCH_CMP_TRIALS; ui32Trial++)
    {
        ui32ErrorStatus = am_hal_ble_blocking_transfer(pHandle, &sTransfer);
        if ( ui32ErrorStatus == AM_HAL_STATUS_SUCCESS )
        {
            break;
        }
    }

    WHILE_TIMEOUT_MS ( BLEIFn(ui32Module)->BSTATUS_b.BLEIRQ == 0, 100,
                       AM_HAL_BLE_NO_HCI_RESPONSE );

    //
    // Read back the response.
    //
    sTransfer.ui8Command = AM_HAL_BLE_READ;
    sTransfer.pui32Data = sRxBuffer.words;
    sTransfer.ui16Length = 2;
    ui32ErrorStatus = am_hal_ble_blocking_transfer(pHandle, &sTransfer);
    if ( ui32ErrorStatus != AM_HAL_STATUS_SUCCESS )
    {
        return ui32ErrorStatus;
    }

    //
    // Check to see which format the response came back in. If it doesn't have
    // a 2-byte length header, we need to manually override the length, and
    // continue on to adjust the HCI format in the next packet. Otherwise, we
    // can just return from here.
    //
    if ( sRxBuffer.bytes[1] == 0xEE )
    {
        sTransfer.ui16Length = 3;
        ui32ErrorStatus = am_hal_ble_blocking_transfer(pHandle, &sTransfer);
        if ( ui32ErrorStatus != AM_HAL_STATUS_SUCCESS )
        {
            return ui32ErrorStatus;
        }
    }
    else
    {
        sTransfer.ui16Length = (sRxBuffer.bytes[0] + (sRxBuffer.bytes[1] << 8));
        ui32ErrorStatus = am_hal_ble_blocking_transfer(pHandle, &sTransfer);
        if ( ui32ErrorStatus != AM_HAL_STATUS_SUCCESS )
        {
            return ui32ErrorStatus;
        }

        //
        // Make sure to remember that we've sent the "patch complete" packet.
        //
        pBLE->bPatchComplete = true;

        return AM_HAL_STATUS_SUCCESS;
    }

    //
    // If we made it here, we need to tell the radio that we need two-byte
    // headers prepended to each HCI packet it sends us.
    //
    memset(&sTransfer, 0, sizeof(am_hal_ble_transfer_t));
    sTransfer.ui8Command = AM_HAL_BLE_WRITE;
    sTransfer.pui32Data = sTxBuffer.words;
    sTransfer.ui16Length = 5;

    sTxBuffer.bytes[0] = 0x01;
    sTxBuffer.bytes[1] = 0x04;
    sTxBuffer.bytes[2] = 0xFD;
    sTxBuffer.bytes[3] = 0x01;
    sTxBuffer.bytes[4] = 0x01;

    for ( ui32Trial = 0; ui32Trial < AM_BLE_NUM_PATCH_CMP_TRIALS; ui32Trial++)
    {
        ui32ErrorStatus = am_hal_ble_blocking_transfer(pHandle, &sTransfer);
        if ( ui32ErrorStatus == AM_HAL_STATUS_SUCCESS )
        {
            break;
        }
    }

    if (ui32ErrorStatus != AM_HAL_STATUS_SUCCESS)
    {
        return ui32ErrorStatus;
    }

    WHILE_TIMEOUT_MS ( BLEIFn(ui32Module)->BSTATUS_b.BLEIRQ == 0, 100,
                       AM_HAL_BLE_NO_HCI_RESPONSE );

    sTransfer.ui8Command = AM_HAL_BLE_READ;
    sTransfer.pui32Data = sRxBuffer.words;
    sTransfer.ui16Length = 9;
    ui32ErrorStatus = am_hal_ble_blocking_transfer(pHandle, &sTransfer);
    if ( ui32ErrorStatus != AM_HAL_STATUS_SUCCESS )
    {
        return ui32ErrorStatus;
    }

    //
    // Now that we're done patching, we can let the radio sleep.
    //
    am_hal_ble_wakeup_set(pBLE, 0);

    //
    // Make sure to remember that we've sent the "patch complete" packet.
    //
    pBLE->bPatchComplete = true;

    //
    // Delay to give the BLE core time to take the patch (assuming a patch was sent).
    //
    delay_ms(500);

    //
    // Load the modex trim data to the BLE controller.
    //
    am_hal_ble_load_modex_trim_set(pBLE);

    //
    // Return the status.
    //
    return AM_HAL_STATUS_SUCCESS;
} // am_hal_ble_patch_complete()

//*****************************************************************************
//
// Set one of the trim values for the BLE core.
//
//*****************************************************************************
uint32_t
am_hal_ble_trim_set(void *pHandle, uint32_t ui32BleCoreAddress, uint32_t ui32TrimValue, uint32_t ui32TrimMask)
{
    am_hal_ble_state_t *pBLE = pHandle;
    uint32_t ui32TrimValueSwapped, ui32LockValue, ui32ReadVal, ui32WriteVal;

    ui32TrimValueSwapped = (((ui32TrimValue & 0x000000FF) << 24) |
                            ((ui32TrimValue & 0x0000FF00) << 8) |
                            ((ui32TrimValue & 0x00FF0000) >> 8) |
                            ((ui32TrimValue & 0xFF000000) >> 24));

    if (ui32TrimValue != 0xFFFFFFFF)
    {
        //
        // Unlock the BLE registers and save the "lock register" value.
        //
        am_hal_ble_plf_reg_read(pBLE, 0x43000004, &ui32LockValue);
        am_hal_ble_plf_reg_write(pBLE, 0x43000004, 0xFFFFFFFF);

        //
        // Check to see if we need a bitfield mask. If not, we can just write
        // directly.
        //
        if (ui32TrimMask == 0xFFFFFFFF)
        {
            am_hal_ble_plf_reg_write(pBLE, ui32BleCoreAddress, ui32TrimValueSwapped);
        }
        else
        {
            //
            // If we do need a mask, read the register, mask out the old bits,
            // OR in the new, and write the new value back.
            //
            am_hal_ble_plf_reg_read(pBLE, ui32BleCoreAddress, &ui32ReadVal);
            ui32WriteVal = ((ui32ReadVal & (~ui32TrimMask)) | ui32TrimValueSwapped);

            am_hal_ble_plf_reg_write(pBLE, ui32BleCoreAddress, ui32WriteVal);
        }

        //
        // Unlock the BLE register.
        //
        am_hal_ble_plf_reg_write(pBLE, 0x43000004, ui32LockValue);
    }

    return AM_HAL_STATUS_SUCCESS;
} // am_hal_ble_trim_set()

//*****************************************************************************
//
// Set the bandgap voltage, bandgap current, and retention LDO output values
// based on the tested values stored in non-volatile memory.
//
//*****************************************************************************
uint32_t
am_hal_ble_default_trim_set_ramcode(void *pHandle)
{
    uint32_t ui32TrimValue;
    uint32_t ui32TrimValueSwapped;
    uint32_t *pRamCode;

    if (APOLLO3_B0)
    {
        pRamCode = (uint32_t *) (am_ble_performance_patch_b0.pui32Data);
    }
    else
    {
        pRamCode = (uint32_t *) (am_ble_performance_patch.pui32Data);
    }

    //
    // Set the bandgap voltage and current.
    //
    //ui32TrimValue = (AM_REGVAL(0x50023800) | (0x0F000000)) & (0xEFFFFFFF);
    ui32TrimValue = AM_REGVAL(0x50023800);
    ui32TrimValueSwapped = (((ui32TrimValue & 0x000000FF) << 24) |
                            ((ui32TrimValue & 0x0000FF00) << 8) |
                            ((ui32TrimValue & 0x00FF0000) >> 8) |
                            ((ui32TrimValue & 0xFF000000) >> 24));

    if (ui32TrimValueSwapped != 0xFFFFFFFF)
    {
        pRamCode[2] = ui32TrimValueSwapped;
    }

    //
    // Set the retention LDO voltage.
    //
    ui32TrimValue = AM_REGVAL(0x50023804);
    if (ui32TrimValue != 0xFFFFFFFF)
    {
        // 0xFFFFFFFF means the part has not been trimed.
        ui32TrimValue += 0x40000000;  // Increase the retention voltage to > 0.75v
    }
    ui32TrimValueSwapped = (((ui32TrimValue & 0x000000FF) << 24) |
                            ((ui32TrimValue & 0x0000FF00) << 8) |
                            ((ui32TrimValue & 0x00FF0000) >> 8) |
                            ((ui32TrimValue & 0xFF000000) >> 24));

    if ( ui32TrimValueSwapped != 0xFFFFFFFF )
    {
        pRamCode[3] = ((pRamCode[3] & (~0x1F0)) | ui32TrimValueSwapped);
    }

    return AM_HAL_STATUS_SUCCESS;
} // am_hal_ble_default_trim_set_ramcode()

//*****************************************************************************
//
// Builds a vendor-specific BLE command.
//
//*****************************************************************************
uint32_t
am_hal_ble_vs_command_build(uint32_t *pui32Command, uint32_t ui32OpCode,
                            uint32_t ui32TotalLength, uint8_t *pui8Parameters)
{
    uint8_t *pui8Dest = (uint8_t *) pui32Command;

    //
    // Build the header portion of the command from the given argments.
    //
    pui8Dest[0] = 0x01;
    pui8Dest[1] = ui32OpCode & 0xFF;
    pui8Dest[2] = (ui32OpCode >> 8) & 0xFF;
    pui8Dest[3] = (ui32TotalLength - 4) & 0xFF;

    //
    // Finish filling the array with any parameters that may be required.
    //
    for (uint32_t i = 4; i < ui32TotalLength; i++)
    {
        pui8Dest[i] = pui8Parameters[i - 4];
    }

    //
    // Return the status.
    //
    return AM_HAL_STATUS_SUCCESS;
} // am_hal_ble_vs_command_build()

//*****************************************************************************
//
// Returns the number of bytes written.
//
//*****************************************************************************
uint32_t
am_hal_ble_blocking_hci_write(void *pHandle, uint8_t ui8Type,
                              uint32_t *pui32Data, uint32_t ui32NumBytes)
{
    uint32_t ui32ErrorStatus;

    am_hal_ble_transfer_t HciWrite =
    {
        .pui32Data = pui32Data,
        .pui8Offset = {ui8Type, 0x0, 0x0},
        .ui8OffsetLen = 0,
        .ui16Length = ui32NumBytes,
        .ui8Command = AM_HAL_BLE_WRITE,
        .ui8RepeatCount = 0,
        .bContinue = false,
        .pfnTransferCompleteCB = 0x0,
        .pvContext = 0x0,
    };

    //
    // Check the handle.
    //
    if (!AM_HAL_BLE_CHK_HANDLE(pHandle))
    {
        return 0;
    }

    //
    // Fix up the offset length based on the packet type, and send the bytes.
    //
    if (ui8Type != AM_HAL_BLE_RAW)
    {
        HciWrite.ui8OffsetLen = 1;
    }

    ui32ErrorStatus = am_hal_ble_blocking_transfer(pHandle, &HciWrite);
    if (ui32ErrorStatus != AM_HAL_STATUS_SUCCESS)
    {
        return ui32ErrorStatus;
    }

    return AM_HAL_STATUS_SUCCESS;
} // am_hal_ble_blocking_hci_write()

//*****************************************************************************
//
// Returns the number of bytes received.
//
//*****************************************************************************
uint32_t
am_hal_ble_blocking_hci_read(void *pHandle, uint32_t *pui32Data, uint32_t *pui32BytesReceived)
{
    uint32_t ui32Module, ui32NumBytes, ui32ErrorStatus;

    am_hal_ble_buffer(2) sLengthBytes;

    am_hal_ble_transfer_t HciRead =
    {
        .pui32Data = sLengthBytes.words,
        .pui8Offset = {0x0, 0x0, 0x0},
        .ui8OffsetLen = 0,
        .ui16Length = 2,
        .ui8Command = AM_HAL_BLE_READ,
        .ui8RepeatCount = 0,
        .bContinue = false,
        .pfnTransferCompleteCB = 0x0,
        .pvContext = 0x0,
    };

    //
    // Check the handle.
    //
    if (!AM_HAL_BLE_CHK_HANDLE(pHandle))
    {
        return 0;
    }

    //
    // Handle is good, so get the module number.
    //
    ui32Module = ((am_hal_ble_state_t *) pHandle)->ui32Module;

    //
    // Make sure the IRQ signal is set.
    //
    if ( BLEIFn(ui32Module)->BSTATUS_b.BLEIRQ )
    {
        //
        // Read the length bytes.
        //
        ui32ErrorStatus = am_hal_ble_blocking_transfer(pHandle, &HciRead);
        if ( ui32ErrorStatus != AM_HAL_STATUS_SUCCESS)
        {
            return ui32ErrorStatus;
        }

        //
        // Read the rest of the packet.
        //
        HciRead.pui32Data = pui32Data;
        HciRead.ui16Length = (sLengthBytes.bytes[0] +
                              (sLengthBytes.bytes[1] << 8));

        //
        // Check if the length is not out of the boundary
        //
        // Fixme: it is assumed here all the sizes of the buffer are 256
        if (HciRead.ui16Length > 256)
        {
            return AM_HAL_STATUS_OUT_OF_RANGE;
        }

        ui32ErrorStatus = am_hal_ble_blocking_transfer(pHandle, &HciRead);
        if ( ui32ErrorStatus != AM_HAL_STATUS_SUCCESS)
        {
            return ui32ErrorStatus;
        }

        ui32NumBytes =  HciRead.ui16Length;
    }
    else
    {
        ui32NumBytes = 0;
    }

    if (pui32BytesReceived)
    {
        *pui32BytesReceived = ui32NumBytes;
    }

    return AM_HAL_STATUS_SUCCESS;
} // am_hal_ble_blocking_hci_read()

//*****************************************************************************
//
// Returns the number of bytes written.
//
//*****************************************************************************
uint32_t
am_hal_ble_nonblocking_hci_write(void *pHandle, uint8_t ui8Type,
                                 uint32_t *pui32Data, uint32_t ui32NumBytes,
                                 am_hal_ble_transfer_complete_cb_t pfnCallback,
                                 void *pvContext)
{
    //
    // Check the handle.
    //
    if (!AM_HAL_BLE_CHK_HANDLE(pHandle))
    {
        return 0;
    }

    am_hal_ble_transfer_t HciWrite =
    {
        .pui32Data = pui32Data,
        .pui8Offset = {ui8Type, 0x0, 0x0},
        .ui8OffsetLen = 0,
        .ui16Length = ui32NumBytes,
        .ui8Command = AM_HAL_BLE_WRITE,
        .ui8RepeatCount = 0,
        .bContinue = false,
        .pfnTransferCompleteCB = pfnCallback,
        .pvContext = pvContext,
    };

    //
    // Fix up the offset length based on the packet type, and send the bytes.
    //
    if (ui8Type != AM_HAL_BLE_RAW)
    {
        HciWrite.ui8OffsetLen = 1;
    }

    uint32_t ui32Status = am_hal_ble_nonblocking_transfer(pHandle, &HciWrite);

    return ui32Status;
} // am_hal_ble_nonblocking_hci_write()

//*****************************************************************************
//
// Returns the number of bytes received.
//
//*****************************************************************************
uint32_t
am_hal_ble_nonblocking_hci_read(void *pHandle, uint32_t *pui32Data,
                                am_hal_ble_transfer_complete_cb_t pfnCallback,
                                void *pvContext)
{
    uint32_t ui32Status;
    am_hal_ble_state_t *pBle = pHandle;

    am_hal_ble_buffer(2) sLengthBytes;

    am_hal_ble_transfer_t HciRead =
    {
        .pui32Data = sLengthBytes.words,
        .pui8Offset = {0x0, 0x0, 0x0},
        .ui8OffsetLen = 0,
        .ui16Length = 2,
        .ui8Command = AM_HAL_BLE_READ,
        .ui8RepeatCount = 0,
        .bContinue = false,
        .pfnTransferCompleteCB = pfnCallback,
        .pvContext = pvContext,
    };

    //
    // Check the handle.
    //
    if ( !AM_HAL_BLE_CHK_HANDLE(pHandle) )
    {
        return AM_HAL_STATUS_INVALID_HANDLE;
    }

    //
    // Make sure the IRQ signal is set.
    //
    if ( am_hal_ble_check_irq(pBle) )
    {
        //
        // Read the length bytes.
        //
        ui32Status = am_hal_ble_blocking_transfer(pHandle, &HciRead);

        if ( ui32Status != AM_HAL_STATUS_SUCCESS )
        {
            return ui32Status;
        }

        //
        // Read the rest of the packet.
        //
        HciRead.pfnTransferCompleteCB = pfnCallback;
        HciRead.pui32Data = pui32Data;
        HciRead.ui16Length = (sLengthBytes.bytes[0] +
                              (sLengthBytes.bytes[1] << 8));

        return am_hal_ble_nonblocking_transfer(pHandle, &HciRead);
    }

    //
    // If we get here, return fail.
    //
    return AM_HAL_STATUS_FAIL;
} // am_hal_ble_nonblocking_hci_read()

//*****************************************************************************
//
// Return true if BSTATUS is high.
//
//*****************************************************************************
static bool
am_hal_ble_check_status(am_hal_ble_state_t *pBle)
{
    //
    // We need to make a special exception for "continue" packets, since the
    // BLE radio may deassert the STATUS signal mid-packet.
    //
    if (pBle->bContinuePacket)
    {
        pBle->bContinuePacket = false;
        return true;
    }

    if ( BLEIFn(0)->BSTATUS_b.SPISTATUS == 0)
    {
        return false;
    }

    return true;
} // am_hal_ble_check_status()

//*****************************************************************************
//
// Return true if IRQ is high.
//
//*****************************************************************************
static bool
am_hal_ble_check_irq(am_hal_ble_state_t *pBle)
{
    if ( BLEIFn(pBle->ui32Module)->BSTATUS_b.BLEIRQ )
    {
        return true;
    }

    return false;
} // am_hal_ble_check_irq()

//*****************************************************************************
//
// Return true if we recently received a BSTATUS edge.
//
//*****************************************************************************
static bool
am_hal_ble_check_status_edge(am_hal_ble_state_t *pBle)
{
    //
    // We need to make a special exception for "continue" packets, since the
    // BLE radio may deassert the STATUS signal mid-packet.
    //
    if (pBle->bContinuePacket)
    {
        pBle->bContinuePacket = false;
        return true;
    }

    if (pBle->bPatchComplete == false)
    {
        return am_hal_ble_check_status(pBle);
    }

    if ( BLEIFn(0)->INTSTAT_b.BLECSSTAT == 0)
    {
        return false;
    }

    return true;
} // am_hal_ble_check_status_edge()

//*****************************************************************************
//
// Blocking write to the BLE module.
//
//*****************************************************************************
uint32_t
am_hal_ble_blocking_transfer(void *pHandle, am_hal_ble_transfer_t *psTransfer)
{
    am_hal_ble_state_t *pBle = pHandle;
    uint32_t ui32IntEnable;
    uint32_t ui32Module;

    //
    // Check the handle.
    //
    if (!AM_HAL_BLE_CHK_HANDLE(pHandle))
    {
        return AM_HAL_STATUS_INVALID_HANDLE;
    }

    //
    // Handle is good, so get the module number.
    //
    ui32Module = ((am_hal_ble_state_t *) pHandle)->ui32Module;

    //
    // If the transfer doesn't have any bytes in it, just return success.
    //
    if (psTransfer->ui16Length == 0)
    {
        return AM_HAL_STATUS_SUCCESS;
    }

    //
    // Make sure we don't get any interrupts that might interfere with this
    // operation. We will save the interrupt enable register state so we can
    // restore it later. Also, make sure "command complete" is clear, so we can
    // detect the end of the transaction.
    //
    ui32IntEnable = BLEIFn(ui32Module)->INTEN;
    BLEIFn(ui32Module)->INTEN_b.BLECIRQ = 0;
    BLEIFn(ui32Module)->INTEN_b.BLECSSTAT = 0;
    BLEIFn(ui32Module)->INTEN_b.CMDCMP = 0;
    BLEIFn(ui32Module)->INTEN_b.THR = 0;
    BLEIFn(ui32Module)->INTCLR_b.CMDCMP = 1;
    BLEIFn(ui32Module)->INTCLR_b.BLECSSTAT = 1;

    //
    // If we're writing, we need to lock down the bus now. Set the wakeup
    // signal, and start monitoring STATUS. If STATUS isn't high within our
    // configured timeout, we have to assume that the BLE core is unresponsive
    // and report an error back to the caller.
    //
    if (psTransfer->ui8Command == AM_HAL_BLE_WRITE)
    {
        uint32_t ui32SpiStatus = false;

        if ( pBle->bLastPacketWasTX == true)
        {
            //
            // wait some time to give the controller more time to consume
            // the last TX packet
            //
            if (!pBle->bPatchComplete)
            {
                delay_ms(3);
            }
            pBle->bLastPacketWasTX = false;
        }

        if (pBle->bPatchComplete)
        {
            uint32_t statusTimeout = 0;
            while (am_hal_ble_check_status(pBle) == true)
            {
                statusTimeout++;
                delay_us(10);
                if (statusTimeout > 300)
                {
                    break;
                }
            }
        }

        //
        // Make sure the IO clock for the STATUS signal is on.
        //
        BLEIFn(0)->BLEDBG_b.IOCLKON = 1;
        delay_us(5);

        //
        // Set WAKE, and wait for a positive edge on the STATUS signal.
        //
        am_hal_ble_wakeup_set(pBle, 1);

        //
        // If we don't see an edge on STATUS in X ms, assume it's not coming
        // and return with an AM_HAL_BLE_STATUS_SPI_NOT_READY error.
        //
        uint32_t ui32Timeout = 0;
        uint32_t ui32TimeoutLimit = AM_BLE_STATUS_TIMEOUT;

        while (1)
        {
            if (am_hal_ble_check_status_edge(pBle) == true)
            {
                if (am_hal_ble_bus_lock(pBle))
                {
                    ui32SpiStatus = AM_HAL_STATUS_SUCCESS;
                    break;
                }
            }
            else if ((ui32Timeout == ui32TimeoutLimit) ||
                     (BLEIFn(ui32Module)->BSTATUS_b.BLEIRQ))
            {
                ui32SpiStatus = AM_HAL_BLE_STATUS_SPI_NOT_READY;
                am_hal_ble_wakeup_set(pBle, 0);
                break;
            }

            ui32Timeout++;
            delay_us(10);
        }

        //
        // Disable IOCLK
        //
        BLEIFn(0)->BLEDBG_b.IOCLKON = 0;

        if (ui32SpiStatus != AM_HAL_STATUS_SUCCESS)
        {
            //
            // Restore the interrupt state.
            //
            BLEIFn(ui32Module)->INTEN = ui32IntEnable;
            am_hal_ble_wakeup_set(pBle, 0);
            return ui32SpiStatus;
        }
    }
    else
    {
        if (BLEIFn(ui32Module)->BSTATUS_b.BLEIRQ == 0)
        {
            //
            // Restore the interrupt state.
            //
            BLEIFn(ui32Module)->INTEN = ui32IntEnable;
            return AM_HAL_BLE_STATUS_IRQ_LOW;
        }

        if (!am_hal_ble_bus_lock(pBle))
        {
            //
            // Restore the interrupt state.
            //
            BLEIFn(ui32Module)->INTEN = ui32IntEnable;
            return AM_HAL_BLE_STATUS_BUS_BUSY;
        }
    }

    if (psTransfer->bContinue)
    {
        pBle->bContinuePacket = true;
    }

    //
    // Set the current transfer, and clear the command complete interrupt so we
    // can tell when the next command completes.
    //
    memcpy(&pBle->sCurrentTransfer, psTransfer, sizeof(am_hal_ble_transfer_t));

    //
    // Critical section to protect the gap between command and data.
    //
    AM_CRITICAL_BEGIN;

    //
    // Write the command word.
    //
    am_hal_ble_cmd_write(pHandle, psTransfer);

    //
    // Now we need to manage the fifos based on the type of transfer. In either
    // case, we will keep draining or refilling the FIFO until the full
    // transaction is complete.
    //
    if (psTransfer->ui8Command == AM_HAL_BLE_WRITE)
    {
        bool bCmdCmp = false;
        uint32_t numWait = 0;
        // Adjust the byte count to be sent/received for repeat count
        uint32_t ui32Bytes = pBle->sCurrentTransfer.ui16Length;

        uint32_t ui32FifoRem;
        uint32_t *pui32Buffer = pBle->sCurrentTransfer.pui32Data;

        //
        // Write the command word.
        //
        am_hal_ble_cmd_write(pHandle, psTransfer);

        //
        // Keep looping until we're out of bytes to send or command complete (error).
        //
        while (ui32Bytes)
        {
            //
            // Limit the wait to reasonable limit - instead of blocking forever
            //
            numWait = 0;
            while ((ui32FifoRem = BLEIFn(ui32Module)->FIFOPTR_b.FIFO0REM) < 4)
            {
                bCmdCmp = BLEIFn(ui32Module)->INTSTAT_b.CMDCMP;
                if (bCmdCmp || (numWait++ >= AM_HAL_IOM_MAX_BLOCKING_WAIT))
                {
                    //
                    // FIFO not expected to change any more - get out
                    //
                    break;
                }
                else
                {
                    am_hal_flash_delay( FLASH_CYCLES_US(1) );
                }
            }
            if (bCmdCmp || (ui32FifoRem < 4))
            {
                //
                // Something went wrong - bail out
                //
                break;
            }

            while ((ui32FifoRem >= 4) && ui32Bytes)
            {
                BLEIFn(ui32Module)->FIFOPUSH = *pui32Buffer++;
                ui32FifoRem -= 4;
                if (ui32Bytes >= 4)
                {
                    ui32Bytes -= 4;
                }
                else
                {
                    ui32Bytes = 0;
                }
            }
        }
       WHILE_TIMEOUT_MS_BREAK ( BLEIFn(ui32Module)->INTSTAT_b.CMDCMP == 0, 2,
                       AM_HAL_BLE_HCI_PACKET_INCOMPLETE );
       am_hal_ble_wakeup_set(pBle, 0);
    }
    else
    {
        while (pBle->ui32TransferIndex < pBle->sCurrentTransfer.ui16Length)
        {
            am_hal_ble_fifo_drain(pHandle);
        }
    }

    //
    // End the critical section.
    //
    AM_CRITICAL_END; //fixme moved further down to cover am_hal_ble_bus_release();

    //
    // Wait for the transaction to complete, and clear out any interrupts that
    // may have come up.
    //
    WHILE_TIMEOUT_MS ( BLEIFn(ui32Module)->INTSTAT_b.CMDCMP == 0, 10,
                       AM_HAL_BLE_HCI_PACKET_INCOMPLETE );
    BLEIFn(ui32Module)->INTCLR_b.CMDCMP = 1;
    BLEIFn(ui32Module)->INTCLR_b.THR = 1;

    //
    // Clear out the current transfer. We're done.
    //
    memset(&pBle->sCurrentTransfer, 0, sizeof(am_hal_ble_transfer_t));
    pBle->ui32TransferIndex = 0;

    //
    // Let the radio go back to sleep.
    //
    if (psTransfer->ui8Command == AM_HAL_BLE_WRITE)
    {
        am_hal_ble_wakeup_set(pBle, 0);
        pBle->bLastPacketWasTX = true;
    }

    if ((psTransfer->ui8Command == AM_HAL_BLE_READ) &&
        (pBle->bPatchComplete == true))
    {
        pBle->bLastPacketWasTX = false;
    }

    //
    // Restore the interrupt state.
    //
    BLEIFn(ui32Module)->INTEN = ui32IntEnable;

    //
    // Release the bus.
    //
    am_hal_ble_bus_release(pBle);

    //
    // End the critical section.
    //
    // AM_CRITICAL_END;  //fixme moved further down to cover am_hal_ble_bus_release();

    //
    // Return the status.
    //
    return AM_HAL_STATUS_SUCCESS;
} // am_hal_ble_blocking_transfer()

//*****************************************************************************
//
// Nonblocking write to the BLE module.
//
//*****************************************************************************
uint32_t
am_hal_ble_nonblocking_transfer(void *pHandle, am_hal_ble_transfer_t *psTransfer)
{
    am_hal_ble_state_t *pBle = pHandle;
    uint32_t ui32Status;

    //
    // Check the handle.
    //
    if (!AM_HAL_BLE_CHK_HANDLE(pHandle))
    {
        return AM_HAL_STATUS_INVALID_HANDLE;
    }

    //
    // Check to see if this is a write or a read.
    //
    if (psTransfer->ui8Command == AM_HAL_BLE_WRITE)
    {
        ui32Status = nonblocking_write(pBle, psTransfer);
    }
    else  // AM_HAL_BLE_READ case.
    {
        ui32Status = nonblocking_read(pBle, psTransfer);
    }

    return ui32Status;
} // am_hal_ble_nonblocking_transfer()

//*****************************************************************************
//
// Function for performing non-blocking writes to the HCI interface.
//
// This function will start a BLE write on the physical bus. The caller should
// have already set WAKEUP and received a STATUS interrupt before they call
// this function. When the write operation is complete, the MCU will receive a
// command complete interrupt.
//
// Before calling this function, the caller is responsible for ensuring that
// STATUS is high, that BLEIRQ is low, and the the bus isn't already in use. If
// any of these problems exists when this function is called, it will simply
// return with an error status.
//
//*****************************************************************************
static uint32_t
nonblocking_write(am_hal_ble_state_t *pBle, am_hal_ble_transfer_t *psTransfer)
{
    uint32_t ui32Status = AM_HAL_STATUS_SUCCESS;
    uint32_t ui32Module = pBle->ui32Module;

    //
    // This function goes in a critical section to make sure that the operation
    // isn't interrupted or started again.
    //
    AM_CRITICAL_BEGIN;

    do
    {
        //
        // Check for any of the various reasons that we might not be able to
        // perform a write right now. If the bus is busy, if the BLE core requires
        // a READ operation, or if the BLE core simply isn't ready yet, stop here
        // and throw an error.
        //
        if ( pBle->bBusy )
        {
            ui32Status = AM_HAL_BLE_STATUS_BUS_BUSY;
            break;
        }

        if ( am_hal_ble_check_irq(pBle) )
        {
            ui32Status = AM_HAL_BLE_REQUESTING_READ;
            break;
        }

        if ( !am_hal_ble_check_status(pBle) )
        {
            ui32Status = AM_HAL_BLE_STATUS_SPI_NOT_READY;
            break;
        }

        if (psTransfer->ui16Length == 0)
        {
            ui32Status = AM_HAL_STATUS_SUCCESS;
            break;
        }

        //
        // With the obvious error cases out of the way, we can claim the bus and
        // start the transaction.
        //
        if ( pBle->bLastPacketWasTX == true )
        {
            delay_us(AM_BLE_TX_PACKET_SPACING_US);
        }

        pBle->bBusy = true;
        pBle->bLastPacketWasTX = true;

        //
        // Save the current transfer.
        //
        memcpy(&pBle->sCurrentTransfer, psTransfer, sizeof(am_hal_ble_transfer_t));

        //
        // Prepare the DMA.
        //
        BLEIFn(ui32Module)->DMATARGADDR = (uint32_t)pBle->sCurrentTransfer.pui32Data;
        BLEIFn(ui32Module)->DMATOTCOUNT = pBle->sCurrentTransfer.ui16Length;
        BLEIFn(ui32Module)->DMATRIGEN = BLEIF_DMATRIGEN_DTHREN_Msk;
        BLEIFn(ui32Module)->DMACFG =
            (_VAL2FLD(BLEIF_DMACFG_DMADIR, BLEIF_DMACFG_DMADIR_M2P) |
             _VAL2FLD(BLEIF_DMACFG_DMAPRI, BLEIF_DMACFG_DMAPRI_HIGH));

        //
        // Write the command word, and enable the DMA.
        //
        ui32Status = am_hal_ble_cmd_write(pBle, &pBle->sCurrentTransfer);

        BLEIFn(ui32Module)->DMACFG |= _VAL2FLD(BLEIF_DMACFG_DMAEN, BLEIF_DMACFG_DMAEN_EN);

        //
        // Make sure WAKE goes low as quickly as possible after starting the write.
        //
        if (ui32Status == AM_HAL_STATUS_SUCCESS)
        {
            am_hal_ble_wakeup_set(pBle, 0);
        }
    }
    while (0);

    //
    // No matter what happened above, the function should end here. We'll end
    // the critical section and alert the caller of our status.
    //
    AM_CRITICAL_END;
    return ui32Status;
} // nonblocking_write()

//*****************************************************************************
//
// This function performs a nonblocking read from the BLE core.
//
//*****************************************************************************
static uint32_t
nonblocking_read(am_hal_ble_state_t *pBle, am_hal_ble_transfer_t *psTransfer)
{
    uint32_t ui32Status = AM_HAL_STATUS_SUCCESS;
    uint32_t ui32Module = pBle->ui32Module;

    //
    // This function goes in a critical section to make sure that the operation
    // isn't interrupted or started again.
    //
    AM_CRITICAL_BEGIN;

    do
    {
        if ( pBle->bBusy )
        {
            ui32Status = AM_HAL_BLE_STATUS_BUS_BUSY;
            break;
        }

        if ( !am_hal_ble_check_irq(pBle) )
        {
            ui32Status = AM_HAL_BLE_STATUS_IRQ_LOW;
            break;
        }

        if (psTransfer->ui16Length == 0)
        {
            ui32Status = AM_HAL_STATUS_SUCCESS;
            break;
        }

        //
        // With the obvious error cases out of the way, we can claim the bus and
        // start the transaction.
        //
        if ( pBle->bLastPacketWasTX == true )
        {
            delay_us(AM_BLE_TX_PACKET_SPACING_US);
        }

        pBle->bBusy = true;
        pBle->bLastPacketWasTX = false;

        //
        // Set the current transfer.
        //
        memcpy(&pBle->sCurrentTransfer, psTransfer, sizeof(am_hal_ble_transfer_t));

        BLEIFn(ui32Module)->DMATARGADDR = (uint32_t) pBle->sCurrentTransfer.pui32Data;
        BLEIFn(ui32Module)->DMATOTCOUNT = pBle->sCurrentTransfer.ui16Length;
        BLEIFn(ui32Module)->DMATRIGEN = (BLEIF_DMATRIGEN_DTHREN_Msk | BLEIF_INTCLR_CMDCMP_Msk);
        BLEIFn(ui32Module)->DMACFG =
            (_VAL2FLD(BLEIF_DMACFG_DMADIR, BLEIF_DMACFG_DMADIR_P2M) |
             _VAL2FLD(BLEIF_DMACFG_DMAPRI, BLEIF_DMACFG_DMAPRI_HIGH));

        //
        // Write the command word, and enable the DMA.
        //
        ui32Status = am_hal_ble_cmd_write(pBle, &pBle->sCurrentTransfer);
        BLEIFn(ui32Module)->DMACFG |= _VAL2FLD(BLEIF_DMACFG_DMAEN, BLEIF_DMACFG_DMAEN_EN);
    }
    while (0);

    //
    // No matter what happened above, the function should end here. We'll end
    // the critical section and alert the caller of our status.
    //
    AM_CRITICAL_END;
    return ui32Status;
} // nonblocking_read()

//*****************************************************************************
//
// Mark the BLE interface busy so it doesn't get used by more than one
// interface.
//
//*****************************************************************************
static bool
am_hal_ble_bus_lock(am_hal_ble_state_t *pBle)
{
    bool bLockObtained;

    //
    // In one atomic sweep, check to see if the bus is busy, and reserve it if
    // it isn't.
    //
    AM_CRITICAL_BEGIN;

    if (pBle->bBusy == false)
    {
        am_hal_debug_gpio_set(BLE_DEBUG_TRACE_11);
        pBle->bBusy = true;
        bLockObtained = true;
        pBle->bCmdComplete = 0;
        pBle->bDmaComplete = 0;
        pBle->bFlowControlComplete = 0;
    }
    else
    {
        bLockObtained = false;
    }

    AM_CRITICAL_END;

    //
    // Tell the caller if we successfully locked the bus.
    //
    return bLockObtained;
} // am_hal_ble_bus_lock()

//*****************************************************************************
//
// Release the bus so someone else can use it.
//
//*****************************************************************************
static void
am_hal_ble_bus_release(am_hal_ble_state_t *pBle)
{
    pBle->bBusy = false;
    am_hal_debug_gpio_clear(BLE_DEBUG_TRACE_11);
}

//*****************************************************************************
//
// Pull data out of the fifo for reads.
//
//*****************************************************************************
static uint32_t
am_hal_ble_fifo_drain(void *pHandle)
{
    uint32_t ui32Module;
    uint32_t ui32ReadSize, ui32RxDataLen, ui32BytesLeft;
    uint32_t *pDest;

    //
    // Check the handle.
    //
    if (!AM_HAL_BLE_CHK_HANDLE(pHandle))
    {
        return 0;
    }

    //
    // Handle is good, so get the module number.
    //
    ui32Module = ((am_hal_ble_state_t *) pHandle)->ui32Module;

    //
    // Rename some pointers for convenience.
    //
    am_hal_ble_state_t *pBle = pHandle;
    am_hal_ble_transfer_t *pTransfer = &pBle->sCurrentTransfer;

    //
    // Check to see how much data there is in the FIFO, and also how many
    // bytes are remaining in the transfer.
    //
    ui32RxDataLen = BLEIFn(ui32Module)->FIFOPTR_b.FIFO1SIZ;
    ui32BytesLeft = (pTransfer->ui16Length - pBle->ui32TransferIndex);

    //
    // Calculate how much we can drain the fifo.
    //
    if (ui32RxDataLen < 4)
    {
        return 0;
    }
    else if (ui32RxDataLen >= pTransfer->ui16Length)
    {
        ui32ReadSize = ui32BytesLeft;
    }
    else
    {
        ui32ReadSize = ui32RxDataLen & (~0x3);
    }

    //
    // Calculate the place where we last left off, feed the FIFO starting from
    // that location, and update the index to match.
    //
    pDest = &pTransfer->pui32Data[pBle->ui32TransferIndex / 4];

    am_hal_ble_fifo_read(pHandle, pDest, ui32ReadSize);

    pBle->ui32TransferIndex += ui32ReadSize;

    //
    // Return the number of bytes we wrote.
    //
    return ui32ReadSize;
} // am_hal_ble_fifo_drain()

//*****************************************************************************
//
// Write the command word for a BLE transfer.
//
//*****************************************************************************
uint32_t
am_hal_ble_cmd_write(void *pHandle, am_hal_ble_transfer_t *psTransfer)
{
    uint32_t ui32CmdWord, ui32OffsetHigh;
    uint32_t ui32Module;

    //
    // Check the handle.
    //
    if (!AM_HAL_BLE_CHK_HANDLE(pHandle))
    {
        return AM_HAL_STATUS_INVALID_HANDLE;
    }

    //
    // Handle is good, so get the module number.
    //
    ui32Module = ((am_hal_ble_state_t *) pHandle)->ui32Module;

    //
    // Figure out the command word and the offset register. Then write them.
    //
    switch (psTransfer->ui8OffsetLen)
    {
        case 0:
            ui32CmdWord = 0;
            ui32OffsetHigh = 0;
            break;

        case 1:
            ui32CmdWord = _VAL2FLD(BLEIF_CMD_OFFSETLO, psTransfer->pui8Offset[0]);
            ui32OffsetHigh = 0;
            break;

        case 2:
            ui32CmdWord = _VAL2FLD(BLEIF_CMD_OFFSETLO, psTransfer->pui8Offset[1]);
            ui32OffsetHigh = psTransfer->pui8Offset[0];
            break;

        case 3:
            ui32CmdWord = _VAL2FLD(BLEIF_CMD_OFFSETLO, psTransfer->pui8Offset[2]);
            ui32OffsetHigh = ((psTransfer->pui8Offset[1]) |
                              (psTransfer->pui8Offset[0] << 8));
            break;

        default:
            // Offset length was incorrect.
            return AM_HAL_STATUS_INVALID_ARG;
    }

    ui32CmdWord |= (_VAL2FLD(BLEIF_CMD_OFFSETCNT, psTransfer->ui8OffsetLen) |
                    _VAL2FLD(BLEIF_CMD_TSIZE, psTransfer->ui16Length)       |
                    _VAL2FLD(BLEIF_CMD_CONT, psTransfer->bContinue)         |
                    psTransfer->ui8Command);

    BLEIFn(ui32Module)->OFFSETHI = ui32OffsetHigh;
    BLEIFn(ui32Module)->CMD = ui32CmdWord;

    //
    // Return the status.
    //
    return AM_HAL_STATUS_SUCCESS;
} // am_hal_ble_cmd_write()

//*****************************************************************************
//
// Read ui32NumBytes from the RX FIFO.
//
//*****************************************************************************
static void
am_hal_ble_fifo_read(void *pHandle, uint32_t *pui32Data, uint32_t ui32NumBytes)
{
    uint32_t ui32Index;
    uint32_t ui32Module = ((am_hal_ble_state_t *) pHandle)->ui32Module;

    for (ui32Index = 0; (ui32Index * 4) < ui32NumBytes; ui32Index++)
    {
        pui32Data[ui32Index] = BLEIFn(ui32Module)->FIFOPOP;

#ifndef AM_HAL_BLE_NO_FIFO_PROTECTION
        BLEIFn(ui32Module)->FIFOPOP = 0;
#endif

    }
} // am_hal_ble_fifo_read()

//*****************************************************************************
//
// Call the appropriate callbacks when DMA transfers complete.
//
//*****************************************************************************
uint32_t
am_hal_ble_int_service(void *pHandle, uint32_t ui32Status)
{
    am_hal_ble_state_t *pBle = pHandle;
    uint32_t ui32Module;

    //
    // Check the handle.
    //
    if (!AM_HAL_BLE_CHK_HANDLE(pHandle))
    {
        return AM_HAL_STATUS_INVALID_HANDLE;
    }

    //
    // The handle is good, so get the module number.
    //
    ui32Module = ((am_hal_ble_state_t *) pHandle)->ui32Module;

    //
    // Track each of the interrupts signaling the end of an HCI transfer.
    //
    if ( ui32Status & BLEIF_INTSTAT_CMDCMP_Msk )
    {
        pBle->bCmdComplete = true;
    }

    if ( ui32Status & BLEIF_INTSTAT_DCMP_Msk )
    {
        pBle->bDmaComplete = true;
    }

    //
    // For B0 parts, we can detect when key flow control signals from the BLE
    // core are de-asserted.
    //
    if (APOLLO3_GE_B0)
    {
        //
        // Check for falling IRQ
        //
        if ( (ui32Status & BLEIF_INTSTAT_BLECIRQN_Msk)  &&
             (pBle->sCurrentTransfer.ui8Command == AM_HAL_BLE_READ) )
        {
            pBle->bFlowControlComplete = true;
        }

        //
        // Check for falling status.
        //
        if ( (ui32Status & BLEIF_INTSTAT_BLECSSTATN_Msk ) &&
             (pBle->sCurrentTransfer.ui8Command == AM_HAL_BLE_WRITE) )
        {
            pBle->bFlowControlComplete = true;
        }
    }

    //
    // If we get a command complete, we need to release the wake signal,
    // disable the DMA, release the bus, and call any callback that might
    // exist.
    //
    // For revision A parts, "command complete" means that the DMA operation
    // and the BLE SPI interface have both finished their operations. For rev B
    // parts, we will also wait for the flow control signal (either STATUS or
    // IRQ) to be removed.
    //
    if ( pBle->bCmdComplete && pBle->bDmaComplete &&
         ((pBle->bFlowControlComplete) || (!APOLLO3_GE_B0) || SKIP_FALLING_EDGES) )
    {
        //
        // Clean up our state flags.
        //
        pBle->bCmdComplete = false;
        pBle->bDmaComplete = false;
        pBle->bFlowControlComplete = false;

        //
        // If our FIFOs aren't empty right now, either the DMA didn't finish,
        // or this interrupt handler is somehow being called incorrectly.
        //
        if ( BLEIFn(ui32Module)->FIFOPTR != 0x20002000 )
        {
            return AM_HAL_BLE_FIFO_ERROR;
        }

        //
        // Drop the wake request if we had one, and make sure we remember if
        // the last packet was a transmit packet.
        //
        if ((pBle->sCurrentTransfer.ui8Command == AM_HAL_BLE_WRITE) &&
            (pBle->bPatchComplete == true))
        {
            pBle->bLastPacketWasTX = true;
            am_hal_ble_wakeup_set(pBle, 0);
        }

        //
        // If this was a read packet, remember that it wasn't a TX packet.
        //
        if (pBle->sCurrentTransfer.ui8Command == AM_HAL_BLE_READ)
        {
            pBle->bLastPacketWasTX = false;
        }

        //
        // Disable the DMA
        //
        BLEIFn(ui32Module)->DMACFG = 0;

        am_hal_ble_bus_release(pBle);

        if ( pBle->sCurrentTransfer.pfnTransferCompleteCB )
        {
            am_hal_ble_transfer_complete_cb_t pfnCallback;
            uint32_t ui32Length;
            uint8_t *pui8Data;
            void *pvContext;

            pfnCallback = pBle->sCurrentTransfer.pfnTransferCompleteCB;
            pui8Data = (uint8_t * ) pBle->sCurrentTransfer.pui32Data;
            ui32Length = pBle->sCurrentTransfer.ui16Length;
            pvContext = pBle->sCurrentTransfer.pvContext;

            pfnCallback(pui8Data, ui32Length, pvContext);
        }
    }

    //
    // Return the status.
    //
    return AM_HAL_STATUS_SUCCESS;
} // am_hal_ble_int_service()

//*****************************************************************************
//
// Interrupt Enable
//
//*****************************************************************************
uint32_t
am_hal_ble_int_enable(void *pHandle, uint32_t ui32InterruptMask)
{
    uint32_t ui32Module;

    //
    // Check the handle.
    //
    if (!AM_HAL_BLE_CHK_HANDLE(pHandle))
    {
        return AM_HAL_STATUS_INVALID_HANDLE;
    }

    //
    // Handle is good, so get the module number.
    //
    ui32Module = ((am_hal_ble_state_t *) pHandle)->ui32Module;

    AM_CRITICAL_BEGIN
    BLEIFn(ui32Module)->INTEN |= ui32InterruptMask;
    AM_CRITICAL_END

    //
    // Return the status.
    //
    return AM_HAL_STATUS_SUCCESS;
} // am_hal_ble_int_enable()

//*****************************************************************************
//
// Interrupt Enable
//
//*****************************************************************************
uint32_t
am_hal_ble_int_disable(void *pHandle, uint32_t ui32InterruptMask)
{
    uint32_t ui32Module;

    //
    // Check the handle.
    //
    if (!AM_HAL_BLE_CHK_HANDLE(pHandle))
    {
        return AM_HAL_STATUS_INVALID_HANDLE;
    }

    //
    // Handle is good, so get the module number.
    //
    ui32Module = ((am_hal_ble_state_t *) pHandle)->ui32Module;

    AM_CRITICAL_BEGIN
    BLEIFn(ui32Module)->INTEN &= ~ui32InterruptMask;
    AM_CRITICAL_END

    //
    // Return the status.
    //
    return AM_HAL_STATUS_SUCCESS;
} // am_hal_ble_int_disable()

//*****************************************************************************
//
// Check the status of the interrupts.
//
//*****************************************************************************
uint32_t
am_hal_ble_int_status(void *pHandle, bool bEnabledOnly)
{
    uint32_t ui32Module = ((am_hal_ble_state_t *) pHandle)->ui32Module;

    if (bEnabledOnly)
    {
        uint32_t ui32IntEn = BLEIFn(ui32Module)->INTEN;
        return ( BLEIFn(ui32Module)->INTSTAT & ui32IntEn );
    }
    else
    {
        return BLEIFn(ui32Module)->INTSTAT;
    }
} // am_hal_ble_int_status()

//*****************************************************************************
//
// Clear the interrupt status.
//
//*****************************************************************************
uint32_t
am_hal_ble_int_clear(void *pHandle, uint32_t ui32InterruptMask)
{
    uint32_t ui32Module;

    //
    // Check the handle.
    //
    if ( !AM_HAL_BLE_CHK_HANDLE(pHandle) )
    {
        return AM_HAL_STATUS_INVALID_HANDLE;
    }

    //
    // Handle is good, so get the module number.
    //
    ui32Module = ((am_hal_ble_state_t *)pHandle)->ui32Module;

    BLEIFn(ui32Module)->INTCLR = ui32InterruptMask;

    //
    // Return the status.
    //
    return AM_HAL_STATUS_SUCCESS;
} // am_hal_ble_int_clear()

//*****************************************************************************
//
// check 32768Hz clock is ready.
//
//*****************************************************************************
uint32_t
am_hal_ble_check_32k_clock(void *pHandle)
{
    am_hal_ble_state_t *pBLE = pHandle;
    uint32_t rc32k_clock = 0xFFFFFFFF;

    if (APOLLO3_B0)
    {
        am_hal_ble_plf_reg_read(pBLE, AM_HAL_BLE_IP_RAM_32K_CLOCK_ADDR_B0, &rc32k_clock);
    }
    else
    {
        am_hal_ble_plf_reg_read(pBLE, AM_HAL_BLE_IP_RAM_32K_CLOCK_ADDR_A1, &rc32k_clock);
    }

    // Normal 32KHz clock is about 0x8000
    if ( (rc32k_clock > 0x8200) || (rc32k_clock < 0x7B00) )
    {
        return AM_HAL_STATUS_FAIL;
    }
    else
    {
        return AM_HAL_STATUS_SUCCESS;
    }
} // am_hal_ble_check_32k_clock()

//*****************************************************************************
//
// Read a register value from the BLE core.
//
//*****************************************************************************
uint32_t
am_hal_ble_plf_reg_read(void *pHandle, uint32_t ui32Address, uint32_t *pui32Value)
{
    am_hal_ble_state_t *pBLE = pHandle;
    uint8_t pui8Parameter[4];
    uint32_t ui32IntEnable;

    uint32_t ui32Module = pBLE->ui32Module;

    //
    // Make a buffer big enough to hold the register write command, and a
    // second one big enough to hold the response.
    //
    am_hal_ble_buffer(AM_HAL_BLE_PLF_REGISTER_READ_LENGTH) sWriteCommand;
    am_hal_ble_buffer(32) sResponse;

    //
    // Prepare our register write value.
    //
    pui8Parameter[0] = ui32Address;
    pui8Parameter[1] = (ui32Address >> 8);
    pui8Parameter[2] = (ui32Address >> 16);
    pui8Parameter[3] = (ui32Address >> 24);

    sResponse.words[0] = 0;
    sResponse.words[1] = 0;
    sResponse.words[2] = 0;

    //
    // Fill the buffer with the specific command we want to write, and send it.
    //
    am_hal_ble_vs_command_build(sWriteCommand.words,
                                AM_HAL_BLE_PLF_REGISTER_READ_OPCODE,
                                AM_HAL_BLE_PLF_REGISTER_READ_LENGTH,
                                pui8Parameter);

    //
    // Temporarily disable BLE interrupts.
    //
    ui32IntEnable = BLEIFn(ui32Module)->INTEN;
    BLEIFn(ui32Module)->INTEN = 0;

    am_hal_ble_blocking_hci_write(pBLE,
                                  AM_HAL_BLE_RAW,
                                  sWriteCommand.words,
                                  AM_HAL_BLE_PLF_REGISTER_READ_LENGTH);

    //
    // Make sure the IO clock for the STATUS signal is on.
    //
    BLEIFn(ui32Module)->BLEDBG_b.IOCLKON = 1;

    //
    // Wait for the response, and return it to the caller via our variable.
    //
    WHILE_TIMEOUT_MS ( BLEIFn(ui32Module)->BSTATUS_b.BLEIRQ == 0, 500,
                       AM_HAL_BLE_NO_HCI_RESPONSE );

    am_hal_ble_blocking_hci_read(pBLE, sResponse.words, 0);

    *pui32Value = (((sResponse.words[1] & 0xFF000000) >> 24) |
                   ((sResponse.words[2] & 0x00FFFFFF) << 8));

    //
    // Re-enable BLE interrupts.
    //
    BLEIFn(ui32Module)->INTCLR = ui32IntEnable;
    BLEIFn(ui32Module)->INTEN = ui32IntEnable;

    return AM_HAL_STATUS_SUCCESS;

} // am_hal_ble_plf_reg_read()

//*****************************************************************************
//
// Write a register value to the BLE core.
//
//*****************************************************************************
uint32_t
am_hal_ble_plf_reg_write(void *pHandle, uint32_t ui32Address, uint32_t ui32Value)
{
    am_hal_ble_state_t *pBLE = pHandle;
    uint8_t pui8Parameter[8];
    uint32_t ui32IntEnable;

    uint32_t ui32Module = pBLE->ui32Module;

    //
    // Make a buffer big enough to hold the register write command, and a
    // second one big enough to hold the response.
    //
    am_hal_ble_buffer(AM_HAL_BLE_PLF_REGISTER_WRITE_LENGTH) sWriteCommand;
    am_hal_ble_buffer(16) sResponse;

    //
    // Prepare our register write value.
    //
    pui8Parameter[0] = ui32Address;
    pui8Parameter[1] = (ui32Address >> 8);
    pui8Parameter[2] = (ui32Address >> 16);
    pui8Parameter[3] = (ui32Address >> 24);
    pui8Parameter[4] = ui32Value;
    pui8Parameter[5] = (ui32Value >> 8);
    pui8Parameter[6] = (ui32Value >> 16);
    pui8Parameter[7] = (ui32Value >> 24);

    //
    // Fill the buffer with the specific command we want to write, and send it.
    //
    am_hal_ble_vs_command_build(sWriteCommand.words,
                                AM_HAL_BLE_PLF_REGISTER_WRITE_OPCODE,
                                AM_HAL_BLE_PLF_REGISTER_WRITE_LENGTH,
                                pui8Parameter);

    //
    // Temporarily disable BLE interrupts.
    //
    ui32IntEnable = BLEIFn(ui32Module)->INTEN;
    BLEIFn(ui32Module)->INTEN = 0;

    am_hal_ble_blocking_hci_write(pBLE,
                                  AM_HAL_BLE_RAW,
                                  sWriteCommand.words,
                                  AM_HAL_BLE_PLF_REGISTER_WRITE_LENGTH);

    //
    // Make sure the IO clock for the STATUS signal is on.
    //
    BLEIFn(ui32Module)->BLEDBG_b.IOCLKON = 1;

    //
    // Wait for the response.
    //
    WHILE_TIMEOUT_MS ( BLEIFn(ui32Module)->BSTATUS_b.BLEIRQ == 0, 50,
                       AM_HAL_BLE_NO_HCI_RESPONSE );

    am_hal_ble_blocking_hci_read(pBLE, sResponse.words, 0);

    //
    // Re-enable BLE interrupts.
    //
    BLEIFn(ui32Module)->INTCLR = ui32IntEnable;
    BLEIFn(ui32Module)->INTEN = ui32IntEnable;

    return AM_HAL_STATUS_SUCCESS;

} // am_hal_ble_plf_reg_write()

//*****************************************************************************
//
// Set the modulation frequency offset from INFO1,
// based on the tested values stored in non-volatile memory.
//
//*****************************************************************************
uint32_t
am_hal_ble_load_modex_trim_set(void *pHandle)
{
    uint8_t ui8TrimValue;
    //
    // load the modex trim data from info1.
    //
    ui8TrimValue = am_hal_ble_read_trimdata_from_info1();
    if ( ui8TrimValue )
    {
        am_hal_ble_transmitter_modex_set(pHandle, ui8TrimValue);
        return AM_HAL_STATUS_SUCCESS;
    }
    else
    {
        return AM_HAL_STATUS_FAIL;
    }
} // am_hal_ble_load_modex_trim_set()

//*****************************************************************************
//
// Load the modulation frequency offset from INFO1,
// based on the tested values stored in non-volatile memory.
//
//*****************************************************************************
uint8_t
am_hal_ble_read_trimdata_from_info1(void)
{
    uint32_t ui32TrimValue = 0, temp = 0;
    uint8_t TrimData = 0;

    temp = ui32TrimValue = AM_REGVAL(0x50023808);
    temp &= 0xffffff00;

    if ( temp == 0x18240600 )
    {
        TrimData = ui32TrimValue & 0xFF;
    }
    else
    {
        TrimData = 0;
    }

    if ( (TrimData > 0x40) || (TrimData < 0x20) )
    {
        TrimData = 0;
    }

    return TrimData;
} // am_hal_ble_read_trimdata_from_info1()

//*****************************************************************************
//
// Manually set modulation characteristic
// based on the tested values at customer side.
// manually set frequency offset for 10101010 or 01010101 pattern
// parameter default value is 0x34, increase to get larger frequency offset
//
//*****************************************************************************
uint32_t
am_hal_ble_transmitter_modex_set(void *pHandle, uint8_t ui8ModFrqOffset)
{
    am_hal_ble_state_t *pBLE = pHandle;
    uint32_t RegValueMCGR, RegValueBACKCR, RegValueSTCR, RegValueDACSPICR, temp = 0;

    ui8ModFrqOffset &= 0x7F;

    am_hal_ble_plf_reg_read(pBLE, 0x43000004, &RegValueMCGR);

    //
    // Unlock the BLE registers.
    //
    am_hal_ble_plf_reg_write(pBLE, 0x43000004, 0xFFFFFFFF);
    am_hal_ble_plf_reg_read(pBLE, 0x52000008, &temp);
    temp |= 0x08;
    am_hal_ble_plf_reg_read(pBLE, 0x52000000, &RegValueSTCR);
    RegValueSTCR |= (1 << 10);
    am_hal_ble_plf_reg_write(pBLE, 0x52000000, RegValueSTCR);

    am_hal_ble_plf_reg_read(pBLE, 0x45800070, &RegValueBACKCR);
    am_hal_ble_plf_reg_write(pBLE, 0x45800070, (RegValueBACKCR | 0x8));
    RegValueDACSPICR = (ui8ModFrqOffset << 1) | 0x1;
    am_hal_ble_plf_reg_write(pBLE, 0x52000014, RegValueDACSPICR);

    am_hal_ble_plf_reg_write(pBLE, 0x52000008, temp);

    if (APOLLO3_B0)
    {
        am_hal_ble_plf_reg_write(pBLE, AM_HAL_BLE_IP_RAM_MODEX_TRIM_ADDR_B0, ui8ModFrqOffset);
    }
    else
    {
        am_hal_ble_plf_reg_write(pBLE, AM_HAL_BLE_IP_RAM_MODEX_TRIM_ADDR_A1, ui8ModFrqOffset);
    }
    am_hal_ble_plf_reg_write(pBLE, 0x43000004, RegValueMCGR);

    return AM_HAL_STATUS_SUCCESS;
} // am_hal_ble_transmitter_modex_set()

//*****************************************************************************
//
// Set BLE sleep enable/disable for the BLE core.
// enable = 'true' set sleep enable, enable = 'false' set sleep disable
//
//*****************************************************************************
uint32_t
am_hal_ble_sleep_set(void *pHandle, bool enable)
{
    am_hal_ble_state_t *pBLE = pHandle;
    uint32_t sleepenable = 0;

    if (APOLLO3_B0)
    {
        am_hal_ble_plf_reg_read(pBLE, AM_HAL_BLE_IP_RAM_SLEEP_ENABLE_ADDR_B0, &sleepenable);
    }
    else
    {
        am_hal_ble_plf_reg_read(pBLE, AM_HAL_BLE_IP_RAM_SLEEP_ENABLE_ADDR_A1, &sleepenable);
    }

    sleepenable &= 0xffff0100;

    if ( enable )
    {
        sleepenable |= 0x0101;
    }

    if (APOLLO3_B0)
    {
        am_hal_ble_plf_reg_write(pBLE, AM_HAL_BLE_IP_RAM_SLEEP_ENABLE_ADDR_B0, sleepenable);
    }
    else
    {
        am_hal_ble_plf_reg_write(pBLE, AM_HAL_BLE_IP_RAM_SLEEP_ENABLE_ADDR_A1, sleepenable);
    }

    return AM_HAL_STATUS_SUCCESS;
} // am_hal_ble_sleep_set()

//*****************************************************************************
//
// Get current sleep enable status
// return 'true'  = sleep enable , 'false' =  sleep disable
//
//*****************************************************************************
bool
am_hal_ble_sleep_get(void *pHandle)
{
    am_hal_ble_state_t *pBLE = pHandle;
    uint32_t sleepenable = 0;

    if (APOLLO3_B0)
    {
        am_hal_ble_plf_reg_read(pBLE, AM_HAL_BLE_IP_RAM_SLEEP_ENABLE_ADDR_B0, &sleepenable);
    }
    else
    {
        am_hal_ble_plf_reg_read(pBLE, AM_HAL_BLE_IP_RAM_SLEEP_ENABLE_ADDR_A1, &sleepenable);
    }

    if ( (sleepenable & 0xFFFF) > 0 )
    {
        return true;
    }

    return false;
} // am_hal_ble_sleep_get()

//*****************************************************************************
//
// set the tx power of BLE
// values.
// ui32TxPower: 0x03->-20dBm 0x04->-10dBm 0x05->-5dBm 0x08->0dBm 0x0F->3dBm
//
//*****************************************************************************
uint32_t
am_hal_ble_tx_power_set(void *pHandle, uint8_t ui32TxPower)
{
    am_hal_ble_state_t *pBLE = pHandle;
    uint32_t RegValueMCGR, tempreg = 0;
    uint32_t ui32PowerValue = 0x00000008;
    ui32PowerValue |= (ui32TxPower & 0xF) << 16;

    am_hal_ble_plf_reg_read(pBLE, 0x43000004, &RegValueMCGR);

    //
    // Unlock the BLE registers.
    //
    am_hal_ble_plf_reg_write(pBLE, 0x43000004, 0xFFFFFFFF);

    if (APOLLO3_B0)
    {
        am_hal_ble_plf_reg_read(pBLE, AM_HAL_BLE_IP_RAM_POWER_LEVEL_ADDR_B0, &tempreg);
    }
    else
    {
        am_hal_ble_plf_reg_read(pBLE, AM_HAL_BLE_IP_RAM_POWER_LEVEL_ADDR_A1, &tempreg);
    }

    tempreg &= 0xffffff00;
    tempreg |= ui32TxPower;
    am_hal_ble_plf_reg_write(pBLE, 0x52400018, ui32PowerValue);

    if (APOLLO3_B0)
    {
        am_hal_ble_plf_reg_write(pBLE, AM_HAL_BLE_IP_RAM_POWER_LEVEL_ADDR_B0, tempreg);
    }
    else
    {
        am_hal_ble_plf_reg_write(pBLE, AM_HAL_BLE_IP_RAM_POWER_LEVEL_ADDR_A1, tempreg);
    }

    am_hal_ble_plf_reg_write(pBLE, 0x43000004, RegValueMCGR);

    return AM_HAL_STATUS_SUCCESS;
} // am_hal_ble_tx_power_set()

//*****************************************************************************
//
// End Doxygen group.
//! @}
//
//*****************************************************************************
