//*****************************************************************************
//
//! @file am_hal_ble.h
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

#ifndef AM_HAL_BLE_H
#define AM_HAL_BLE_H

#include "am_hal_global.h"
#include "am_hal_status.h"

//*****************************************************************************
//
// CMSIS-style macro for handling a variable BLEIF module number.
//
#define BLEIFn(n)   ((BLEIF_Type*)(BLEIF_BASE + (n * (BLEIF_BASE - BLEIF_BASE))))
//*****************************************************************************

//*****************************************************************************
//
// BLE-specific status values.
//
//*****************************************************************************
typedef enum
{
    //
    // This error occurs when an HCI read or write function is called while
    // another HCI communication function is already in progress.
    //
    AM_HAL_BLE_STATUS_BUS_BUSY = AM_HAL_STATUS_MODULE_SPECIFIC_START,

    //
    // This error happens when the MCU tries to execute an HCI read, but the
    // BLE core hasn't asserted the BLEIRQ line. Try waiting for a BLEIRQ
    // interrupt, or polling the BLECIRQ bit in the BSTATUS register before
    // calling an HCI read function.
    //
    AM_HAL_BLE_STATUS_IRQ_LOW,

    //
    // This error means that the MCU tried to execute an HCI write, but the BLE
    // core didn't assert its SPI_STATUS signal within the allotted timeout.
    // This might mean that there has been some error inside the BLE core. This
    // may require a reboot of the BLE core.
    //
    AM_HAL_BLE_STATUS_SPI_NOT_READY,

    //
    // This error means we were trying to write, but the BLE core has requested
    // a READ instead. We will need to perform a read before we can retry this
    // write.
    //
    AM_HAL_BLE_REQUESTING_READ,

    //
    // We are expecting an HCI response to a packet we just sent, but the BLE
    // core isn't asserting BLEIRQ. Its software may have crashed, and it may
    // need to restart.
    //
    AM_HAL_BLE_NO_HCI_RESPONSE,

    //
    // Any of these errors indicate a problem with the BLE hardware that
    // requires a complete restart.
    //
    AM_HAL_BLE_FEATURE_DISABLED,
    AM_HAL_BLE_SHUTDOWN_FAILED,
    AM_HAL_BLE_REGULATOR_FAILED,
    AM_HAL_BLE_POWERUP_INCOMPLETE,
    AM_HAL_BLE_HCI_PACKET_INCOMPLETE,
    AM_HAL_BLE_FIFO_ERROR,
    AM_HAL_BLE_32K_CLOCK_UNSTABLE,
}
am_ble_status_e;

//*****************************************************************************
//
// BLE power modes.
//
//*****************************************************************************
typedef enum
{
    AM_HAL_BLE_POWER_ACTIVE,
    AM_HAL_BLE_POWER_OFF,
}
am_hal_ble_power_state_e;

//*****************************************************************************
//
// BLE SPI Clock settings.
//
//*****************************************************************************
typedef enum
{
    AM_HAL_BLE_HCI_CLK_DIV2  = BLEIF_CLKCFG_FSEL_HFRC_DIV2,
    AM_HAL_BLE_HCI_CLK_DIV4  = BLEIF_CLKCFG_FSEL_HFRC_DIV4,
    AM_HAL_BLE_HCI_CLK_DIV8  = BLEIF_CLKCFG_FSEL_HFRC_DIV8,
    AM_HAL_BLE_HCI_CLK_DIV16 = BLEIF_CLKCFG_FSEL_HFRC_DIV16,
    AM_HAL_BLE_HCI_CLK_DIV32 = BLEIF_CLKCFG_FSEL_HFRC_DIV32,
    AM_HAL_BLE_HCI_CLK_DIV64 = BLEIF_CLKCFG_FSEL_HFRC_DIV8,
}
am_hal_ble_hci_clock_e;

//*****************************************************************************
//
// BLE Core Clock settings.
//
//*****************************************************************************
typedef enum
{
    AM_HAL_BLE_CORE_MCU_CLK      = 0x02,
    AM_HAL_BLE_CORE_INTERNAL_CLK = 0x00,
}
am_hal_ble_core_clock_e;

//*****************************************************************************
//
// Interrupts.
//
//*****************************************************************************
// The B2M_STATE went into the shutdown state
#define AM_BLEIF_INT_B2MSHUTDN               AM_REG_BLEIF_INTEN_B2MSHUTDN_M
// The B2M_STATE went into the active state
#define AM_BLEIF_INT_B2MACTIVE               AM_REG_BLEIF_INTEN_B2MACTIVE_M
// The B2M_STATE went into the sleep state
#define AM_BLEIF_INT_B2MSLEEP                AM_REG_BLEIF_INTEN_B2MSLEEP_M
// command queue received and error
#define AM_BLEIF_INT_CQERR                   AM_REG_BLEIF_INTEN_CQERR_M
// CQ write operation performed a register write with the register address bit
// 0 set to 1.  The low address bits in the CQ address fields are unused and
// bit 0 can be used to trigger an interrupt to indicate when this register
// write is performed by the CQ operation.
#define AM_BLEIF_INT_CQUPD                   AM_REG_BLEIF_INTEN_CQUPD_M
// The command queue is waiting interrupt
#define AM_BLEIF_INT_CQPAUSED                AM_REG_BLEIF_INTEN_CQPAUSED_M
// DMA Error
#define AM_BLEIF_INT_DERR                    AM_REG_BLEIF_INTEN_DERR_M
// DMA Complete
#define AM_BLEIF_INT_DCMP                    AM_REG_BLEIF_INTEN_DCMP_M
// THis is the BLE Core IRQ signal
#define AM_BLEIF_INT_BLECIRQ                 AM_REG_BLEIF_INTEN_BLECIRQ_M
// This is the illegal command interrupt.
#define AM_BLEIF_INT_ICMD                    AM_REG_BLEIF_INTEN_ICMD_M
// This is the illegal FIFO access interrupt.
#define AM_BLEIF_INT_IACC                    AM_REG_BLEIF_INTEN_IACC_M
// Any change in the B2M_STATE signal from the BLE Core will set this interrupt
#define AM_BLEIF_INT_B2MST                   AM_REG_BLEIF_INTEN_B2MST_M
// This is the Write FIFO Overflow interrupt.
#define AM_BLEIF_INT_FOVFL                   AM_REG_BLEIF_INTEN_FOVFL_M
// This is the Read FIFO Underflow interrupt.
#define AM_BLEIF_INT_FUNDFL                  AM_REG_BLEIF_INTEN_FUNDFL_M
// This is the FIFO Threshold interrupt.
#define AM_BLEIF_INT_THR                     AM_REG_BLEIF_INTEN_THR_M
// This is the Command Complete interrupt.
#define AM_BLEIF_INT_CMDCMP                  AM_REG_BLEIF_INTEN_CMDCMP_M

#define AM_HAL_BLE_INT_B2MSHUTDN    BLEIF_INTEN_B2MSHUTDN_Msk // The B2M_STATE went into the shutdown state
#define AM_HAL_BLE_INT_B2MACTIVE    BLEIF_INTEN_B2MACTIVE_Msk // The B2M_STATE went into the active state
#define AM_HAL_BLE_INT_B2MSLEEP     BLEIF_INTEN_B2MSLEEP_Msk  // The B2M_STATE went into the sleep state
#define AM_HAL_BLE_INT_CQERR        BLEIF_INTEN_CQERR_Msk     // command queue received and error

// CQ write operation performed a register write with the register address bit
// 0 set to 1.  The low address bits in the CQ address fields are unused and
// bit 0 can be used to trigger an interrupt to indicate when this register
// write is performed by the CQ operation.
#define AM_HAL_BLE_INT_CQUPD        BLEIF_INTEN_CQUPD_Msk

#define AM_HAL_BLE_INT_CQPAUSED     BLEIF_INTEN_CQPAUSED_Msk  // The command queue is waiting interrupt
#define AM_HAL_BLE_INT_DERR         BLEIF_INTEN_DERR_Msk      // DMA Error
#define AM_HAL_BLE_INT_DCMP         BLEIF_INTEN_DCMP_Msk      // DMA Complete
#define AM_HAL_BLE_INT_BLECSSTAT    BLEIF_INTEN_BLECSSTAT_Msk // This is the BLE Core SPI STATUS signal.
#define AM_HAL_BLE_INT_BLECIRQ      BLEIF_INTEN_BLECIRQ_Msk   // This is the BLE Core IRQ signal
#define AM_HAL_BLE_INT_ICMD         BLEIF_INTEN_ICMD_Msk      // This is the illegal command interrupt.
#define AM_HAL_BLE_INT_IACC         BLEIF_INTEN_IACC_Msk      // This is the illegal FIFO access interrupt.
#define AM_HAL_BLE_INT_B2MST        BLEIF_INTEN_B2MST_Msk     // Any change in the B2M_STATE signal from the BLE Core will set this interrupt
#define AM_HAL_BLE_INT_FOVFL        BLEIF_INTEN_FOVFL_Msk     // This is the Write FIFO Overflow interrupt.
#define AM_HAL_BLE_INT_FUNDFL       BLEIF_INTEN_FUNDFL_Msk    // This is the Read FIFO Underflow interrupt.
#define AM_HAL_BLE_INT_THR          BLEIF_INTEN_THR_Msk       // This is the FIFO Threshold interrupt.
#define AM_HAL_BLE_INT_CMDCMP       BLEIF_INTEN_CMDCMP_Msk    // This is the Command Complete interrupt.

#define AM_HAL_BLE_INT_BLECSSTATN       BLEIF_INTSTAT_B2MSHUTDN_Msk
#define AM_HAL_BLE_INT_BLECIRQN         BLEIF_INTSTAT_B2MACTIVE_Msk

//*****************************************************************************
//
// Type definitions.
//
//*****************************************************************************
#define am_hal_ble_buffer(A)                                                  \
    union                                                                     \
    {                                                                         \
        uint32_t words[(A + 3) >> 2];                                         \
        uint8_t bytes[A];                                                     \
    }

// Function pointer for non-blocking ble read callbacks.
typedef void (*am_hal_ble_transfer_complete_cb_t)(uint8_t *pui8Data, uint32_t ui32Length, void *pvContext);

//
// Patch container
//
typedef struct
{
    uint32_t ui32Type;
    uint32_t ui32Length;
    uint32_t ui32CRC;
    const uint32_t *pui32Data;
}
am_hal_ble_patch_t;

//
// Configuration structure for the BLE module.
//
typedef struct
{
    // HCI interface options.
    uint32_t ui32SpiClkCfg;         // Configure the HCI interface clock.
    uint32_t ui32ReadThreshold;     // Internal HCI READ FIFO size
    uint32_t ui32WriteThreshold;    // Internal HCI WRITE FIFO size.

    // BLE options.
    uint32_t ui32BleClockConfig;    // Configure the BLE core clock.
    uint32_t ui32ClockDrift;        // Set the expected BLE clock drift.
    uint32_t ui32SleepClockDrift;   // Set the expected sleep clock accuracy.
    bool bAgcEnabled;               // Enable/Disable AGC
    bool bSleepEnabled;             // Enable/Disable Sleep Algorithm

    // Patches
    bool bUseDefaultPatches;        // Apply the default patches?
}
am_hal_ble_config_t;

//
// Default options for the BLE module.
//
extern const am_hal_ble_config_t am_hal_ble_default_config;

//*****************************************************************************
//
// Structure for sending SPI commands.
//
//*****************************************************************************
typedef struct
{
    uint32_t *pui32Data;
    uint8_t pui8Offset[3];
    uint8_t ui8OffsetLen;
    uint16_t ui16Length;
    uint8_t ui8Command;
    uint8_t ui8RepeatCount;
    bool bContinue;
    am_hal_ble_transfer_complete_cb_t pfnTransferCompleteCB;
    void *pvContext;
}
am_hal_ble_transfer_t;

//*****************************************************************************
//
// Vendor Specific commands.
//
// Note: Lengths are reported as "4 + <parameter length>". Each vendor-specific
// header is 4 bytes long. This definition allows the macro version of the
// length to be used in all BLE APIs.
//
//*****************************************************************************
#define AM_HAL_BLE_SET_BD_ADDR_OPCODE               0xFC32
#define AM_HAL_BLE_SET_BD_ADDR_LENGTH               (4 + 6)

#define AM_HAL_BLE_SET_TX_POWER_OPCODE              0xFC3B
#define AM_HAL_BLE_SET_TX_POWER_LENGTH              (4 + 3)

#define AM_HAL_BLE_READ_VERSIONS_OPCODE             0xFD01
#define AM_HAL_BLE_READ_VERSIONS_LENGTH             (4 + 0)

#define AM_HAL_BLE_PLF_REGISTER_READ_OPCODE         0xFD02
#define AM_HAL_BLE_PLF_REGISTER_READ_LENGTH         (4 + 4)

#define AM_HAL_BLE_PLF_REGISTER_WRITE_OPCODE        0xFD03
#define AM_HAL_BLE_PLF_REGISTER_WRITE_LENGTH        (4 + 8)

#define AM_HAL_BLE_GET_RSSI_OPCODE                  0x1405
#define AM_HAL_BLE_GET_RSSI_LENGTH                  (4 + 0)

#define AM_HAL_BLE_SET_SLEEP_OPCODE                 0xFD09
#define AM_HAL_BLE_SET_SLEEP_LENGTH                 (4 + 0)

#define AM_HAL_BLE_SPI_SENDFRAME_OPCODE             0xFD04
#define AM_HAL_BLE_SPI_SENDFRAME_LENGTH             (4 + 1)

#define AM_HAL_BLE_SET_BD_ADDR_CMD(...)         {0x01, 0x32, 0xFC, 0x06, __VA_ARGS__}
#define AM_HAL_BLE_SET_TX_POWER_CMD(...)        {0x01, 0x3B, 0xFC, 0x03, __VA_ARGS__}
#define AM_HAL_BLE_SET_READ_VERSIONS_CMD()      {0x01, 0x01, 0xFD, 0x00}
#define AM_HAL_BLE_PLF_REGISTER_READ_CMD(...)   {0x01, 0x02, 0xFD, 0x04, __VA_ARGS__}
#define AM_HAL_BLE_PLF_REGISTER_WRITE_CMD(...)  {0x01, 0x03, 0xFD, 0x08, __VA_ARGS__}
#define AM_HAL_BLE_GET_RSSI_CMD()               {0x01, 0x05, 0x14, 0x00}
#define AM_HAL_BLE_SET_SLEEP_CMD()              {0x01, 0x09, 0xFD, 0x00}
#define AM_HAL_BLE_SPI_SENDFRAME_CMD(...)       {0x01, 0x04, 0xFD, 0x01, __VA_ARGS__}

//*****************************************************************************
//
// State variables for the BLE module.
//
//*****************************************************************************
typedef struct
{
    // Handle validation prefix.
    am_hal_handle_prefix_t prefix;

    // Which BLE module instance is this?
    uint32_t ui32Module;

    // Apply the default patches during the "boot" function?
    bool bUseDefaultPatches;

    // What was the last command that we started?
    am_hal_ble_transfer_t sCurrentTransfer;

    // If a write is interrupted by a read, we have to save the write
    // transaction to execute later. That saved write goes here.
    am_hal_ble_transfer_t sSavedTransfer;

    // How far along are we?
    uint32_t ui32TransferIndex;

    // Has this radio already been patched?
    bool bPatchComplete;

    // Are we in the middle of a continue packet?
    bool bContinuePacket;

    // Was our last operation to send a TX packet? If we have two TX packets in
    // a row, we need special handling to get the timing right.
    bool bLastPacketWasTX;

    // Do we have a saved packet?
    bool bSavedPacket;

    // Is the bus already in use?
    bool bBusy;

    // Has the last command completed?
    bool bCmdComplete;

    // Has the last DMA action completed?
    bool bDmaComplete;

    // Has the BLE core's flow control signal been reset?
    bool bFlowControlComplete;
}
am_hal_ble_state_t;

//*****************************************************************************
//
// SPI command macros.
//
//*****************************************************************************
#define AM_HAL_BLE_WRITE                    1
#define AM_HAL_BLE_READ                     2

//*****************************************************************************
//
// HCI packet types.
//
//*****************************************************************************
#define AM_HAL_BLE_RAW                      0x0
#define AM_HAL_BLE_CMD                      0x1
#define AM_HAL_BLE_ACL                      0x2
#define AM_HAL_BLE_EVT                      0x4

//*****************************************************************************
//
// External function declarations.
//
//*****************************************************************************
#ifdef __cplusplus
extern "C"
{
#endif

//*****************************************************************************
//
// Basics
//
// Initialization, enable/disable, and general configuration.
//
//*****************************************************************************

//*****************************************************************************
//
//! @brief Initialize the internal state variables for the BLE module.
//!
//! @param ui32Module - Which BLE module to use.
//! @param ppHandle - Pointer to a handle variable to be initialized.
//!
//! This function initializes the internal state variables associated with a
//! particular BLE module and yields a handle that may be used to perform
//! additional operations with that BLE module.
//!
//! This function must be called before any other BLE module operation.
//!
//! @return BLE status code.
//
//*****************************************************************************
extern uint32_t am_hal_ble_initialize(uint32_t ui32Module, void **ppHandle);

//*****************************************************************************
//
//! @brief De-initialize the internal state variables for the BLE module.
//!
//! @param pHandle - Handle variable to be de-initialized.
//!
//! This function invalidates a previously initialized BLE module handle and
//! deletes the contents of the internal state variables associated with it.
//! This could be used in situations where the caller wants to prevent future
//! function calls to a particular BLE module.
//!
//! @return BLE status code.
//
//*****************************************************************************
extern uint32_t am_hal_ble_deinitialize(void *pHandle);

//*****************************************************************************
//
//! @brief Configure a BLE module.
//!
//! @param pHandle - Handle for the BLE module.
//! @param psConfig - Pointer to a BLE configuration structure.
//!
//! This routine performs the necessary configuration steps to prepare the
//! physical BLE interface for operation. This function should be called after
//! \e am_hal_ble_enable() and before any other BLE operation. The \e psConfig
//! parameter may be used to set a specific interface clock frequency or modify
//! the FIFO read and write thresholds, but most users will get the best
//! results from the default settings stored in the global configuration
//! structure \e am_hal_ble_default_config.
//!
//! @note This function will only work if the BLE module has previously been
//! enabled.
//!
//! @return BLE status code.
//
//*****************************************************************************
extern uint32_t am_hal_ble_config(void *pHandle, const am_hal_ble_config_t *psConfig);

//*****************************************************************************
//
//! @brief Enable the BLE module.
//!
//! @param pHandle - Handle for the BLE module.
//! @param ui32PowerState - Determines whether BLE is powered on or off.
//!
//! Performs the power-up or power-down sequence for the BLE module referred to
//! be \e pHandle. This should be called after am_hal_ble_initialize(), but
//! before am_hal_ble_config().
//!
//! The ui32PowerState variable must be set to either AM_HAL_BLE_POWER_ACTIVE
//! or AM_HAL_BLE_POWER_OFF.
//!
//! After this function is called, the BLE core will be in its startup or
//! "patching" mode.
//!
//! @return BLE status code.
//
//*****************************************************************************
uint32_t am_hal_ble_power_control(void *pHandle, uint32_t ui32PowerState);

//*****************************************************************************
//
//! @brief Boot the BLE module
//!
//! @param pHandle - Handle for the BLE module.
//!
//! This function performs the complete patching process for the BLE core and
//! returns with the BLE core in HCI mode. If you ask for the default patches
//! your am_hal_ble_config_t structure, then this is the last function you need
//! to call on startup. You don't need to call any of the patching functions.
//!
//! @return BLE status code.
//
//*****************************************************************************
extern uint32_t am_hal_ble_boot(void *pHandle);

//*****************************************************************************
//
// Patching functions.
//
// The following functions allow the caller to apply "patches" to the BLE core
// during its startup phase. These are pre-made configuration files that change
// the operation parameters of the BLE radio. If you have received a patch file
// from the manufacturer, you may use the \e am_hal_ble_patch_apply() function
// during startup to apply these settings to the BLE core. Otherwise, you may
// skip this step by calling the \e am_hal_ble_patch_complete() function.
//
//*****************************************************************************

//*****************************************************************************
//
//! @brief Apply a patch to the BLE core.
//!
//! @param pHandle Handle for the BLE module.
//! @param psPatch Pointer to a structure describing the patch.
//!
//! The BLE core is an independent processor that executes code from an
//! on-board ROM. Its behavior can be altered through "patches" which are
//! binary snippets of code that may be loaded at startup to overlay or replace
//! sections of the original ROM (for instance, to modify trim settings). This
//! function allows the caller to apply one of these patches.
//!
//! Patches must be applied after the BLE module is enabled and configured, but
//! before standard HCI operation begins. This is the only time where the BLE
//! core is able to accept patch files.
//!
//! @return BLE status code.
//
//*****************************************************************************
extern uint32_t am_hal_ble_patch_apply(void *pHandle, am_hal_ble_patch_t *psPatch);

extern uint32_t am_hal_ble_default_copy_patch_apply(void *pHandle);

//*****************************************************************************
//
//! @brief Apply the default manufacturer patch to the BLE core.
//!
//! @param pHandle Handle for the BLE module.
//! @param psPatch Pointer to a structure describing the patch.
//!
//! The BLE core is an independent processor that executes code from an
//! on-board ROM. Its behavior can be altered through "patches" which are
//! binary snippets of code that may be loaded at startup to overlay or replace
//! sections of the original ROM (for instance, to modify trim settings). This
//! function allows the caller to apply one of these patches.
//!
//! Patches must be applied after the BLE module is enabled and configured, but
//! before standard HCI operation begins. This is the only time where the BLE
//! core is able to accept patch files.
//!
//! @return BLE status code.
//
//*****************************************************************************
extern uint32_t am_hal_ble_default_patch_apply(void *pHandle);

//*****************************************************************************
//
//! @brief Complete the patching phase.
//!
//! @param pHandle Handle for the BLE module.
//!
//! After the BLE core is enabled and configured, it enters a "patching mode"
//! where it can accept patches from the main CPU. Once all patches have been
//! applied using the \e am_hal_ble_patch_apply() function. The application
//! must call this function to command the BLE core to switch to standard HCI
//! mode.
//!
//! @return BLE status code.
//
//*****************************************************************************
extern uint32_t am_hal_ble_patch_complete(void *pHandle);

//*****************************************************************************
//
// Manually enable/disable transmitter
// set ui8TxCtrl as 1 to manually enale transmitter, 0 back to default
//
//*****************************************************************************
extern uint32_t am_hal_ble_transmitter_control(void *pHandle, uint8_t ui8TxCtrl);

//*****************************************************************************
//
// Manually enable/disable transmitter to output carrier signal
// set ui8TxChannel as 0 to 0x27 for each transmit channel, 0xFF back to normal modulate mode
//
//*****************************************************************************
extern uint32_t am_hal_ble_transmitter_control_ex(void *pHandle, uint8_t ui8TxChannel);
//*****************************************************************************
//
// Manually set modulation characteristic
// based on the tested values at customer side.
// manually set frequency offset for 10101010 or 01010101 pattern
//
//*****************************************************************************
extern uint32_t am_hal_ble_transmitter_modex_set(void *pHandle, uint8_t ui8ModFrqOffset);

//*****************************************************************************
//
//! @brief Performs a blocking read or write to the BLE core.
//!
//! @param pHandle - Handle for the BLE module.
//! @param psTransfer - Structure describing the transaction to execute.
//!
//! Send or receive data from the
//!
//! @return BLE status code.
//
//*****************************************************************************
extern uint32_t am_hal_ble_blocking_transfer(void *pHandle, am_hal_ble_transfer_t *psTransfer);

//*****************************************************************************
//
//! @brief Complete the patching phase.
//!
//! @param pHandle Handle for the BLE module.
//!
//! After the BLE core is enabled and configured, it enters a "patching mode"
//! where it can accept patches from the main CPU. Once all patches have been
//! applied using the \e am_hal_ble_patch_apply() function. The application
//! must call this function to command the BLE core to switch to standard HCI
//! mode.
//!
//! @return BLE status code.
//
//*****************************************************************************
extern uint32_t am_hal_ble_nonblocking_transfer(void *pHandle, am_hal_ble_transfer_t *psTransfer);

// High-level HCI APIs
extern uint32_t am_hal_ble_vs_command_build(uint32_t *pui32Command,
                                            uint32_t ui32OpCode,
                                            uint32_t ui32TotalLength,
                                            uint8_t *pui8Parameters);

extern uint32_t am_hal_ble_blocking_hci_read(void *pHandle,
                                             uint32_t *pui32Data,
                                             uint32_t *pui32BytesReceived);

extern uint32_t am_hal_ble_blocking_hci_write(void *pHandle,
                                              uint8_t ui8Type,
                                              uint32_t *pui32Data,
                                              uint32_t ui32NumBytes);

extern uint32_t am_hal_ble_nonblocking_hci_read(void *pHandle,
                                                uint32_t *pui32Data,
                                                am_hal_ble_transfer_complete_cb_t pfnCallback,
                                                void *pvContext);

extern uint32_t am_hal_ble_nonblocking_hci_write(void *pHandle,
                                                 uint8_t ui8Type,
                                                 uint32_t *pui32Data,
                                                 uint32_t ui32NumBytes,
                                                 am_hal_ble_transfer_complete_cb_t pfnCallback,
                                                 void *pvContext);

//*****************************************************************************
//
//! @brief Set one of the trim values for the BLE core.
//!
//! @param pHandle is the BLE module handle
//! @param ui32BleCoreAddress is the target address for the trim value.
//! @param ui32TrimValue is the trim value to write to the BLE core.
//!
//! This function takes a BLE core trim value from the MCU memory and writes it
//! to a trim register in the BLE core.
//!
//! @return BLE status code.
//
//*****************************************************************************
extern uint32_t am_hal_ble_trim_set(void *pHandle, uint32_t ui32BleCoreAddress,
                                    uint32_t ui32TrimValue, uint32_t ui32TrimMask);

//*****************************************************************************
//
//! @brief Sets the default trim values for the BLE core.
//!
//! @param pHandle is the BLE module handle
//!
//! This function reads the default trim values for the BLE core from
//! non-volatile memory, and writes them to the BLE core registers.
//! Specifically, this function adjusts the BLE core bandgap voltage, bandgap
//! current, and memory-retention LDO voltage based on chip-specific,
//! manufacturer-determined settings.
//!
//! For best performance and power consumption, this function should be called
//! after the patching process is complete, but before normal HCI operation
//! begins.
//!
//! @return BLE status code.
//
//*****************************************************************************
extern uint32_t am_hal_ble_default_trim_set(void *pHandle);

uint32_t am_hal_ble_default_trim_set_ramcode(void *pHandle);

//*****************************************************************************
//
//! @brief Change the TX power setting.
//!
//! @param pHandle is the Handle for the BLE module.
//! @param ui32TxPower is the desired power setting.
//!     0x03->-20dBm 0x04->-10dBm 0x05->-5dBm 0x08->0dBm 0x0F->4dBm
//!
//! This function sends a vendor-specific command to change the TX power level
//! setting for the BLE core.
//!
//! @return BLE status code.
//
//*****************************************************************************
extern uint32_t am_hal_ble_tx_power_set(void *pHandle, uint8_t ui32TxPower);

//*****************************************************************************
//
//! @brief Generate continuously moderated signal for SRRC/CE test.
//!
//! @param pHandle is the Handle for the BLE module.
//! @param enable, true for enabling continous signal, false for disable
//!
//! This function programs an internal register to control transmit mode in
//! BLE controller.
//!
//! @return BLE status code.
//
//*****************************************************************************

extern uint32_t am_hal_ble_set_constant_transmission(void *pHandle, bool enable);

//*****************************************************************************
//
//! @brief Generate continuously moderated signal for SRRC/CE test on a
//! specified rf channel.
//!
//! @param pHandle is the Handle for the BLE module.
//! @param channel, 0 to 0x27 for a valid radio channnel while 0xff to set
//! radio transmit mode to normal.
//!
//! This function calls am_hal_ble_set_constant_transmission() and send HCI
//! test command with channel information to BLE controller.
//!
//! @return BLE status code.
//
//*****************************************************************************
extern uint32_t am_hal_ble_set_constant_transmission_ex(void *pHandle, uint8_t channel);

//*****************************************************************************
//
//! @brief This is to workaround a bug for channel 1 in DTM mode.
//!
//! @param pHandle is the Handle for the BLE module.
//!
//! @return BLE status code.
//
//
extern uint32_t am_hal_ble_init_rf_channel(void *pHandle);

//*****************************************************************************
//
//! @brief This function should be called with enable set to true for
//! BQB testing.
//!
//! @param pHandle is the Handle for the BLE module.
//! @param enable, true for enabling BQB test mode, false for normal mode
//!
//! @return BLE status code.
//
//*****************************************************************************
extern uint32_t am_hal_ble_BQB_test_init(void *pHandle, bool enable);

//*****************************************************************************
//
//! @brief Set BLE sleep enable/disable for the BLE core.
//!
//! @param pHandle is the Handle for the BLE module.
//! @param enable 'true' set sleep enable, 'false' set sleep disable
//!
//! @return BLE status code.
//
//*****************************************************************************
extern uint32_t am_hal_ble_sleep_set(void *pHandle, bool enable);

//*****************************************************************************
//
//! @brief Sends a signal to wake up the BLE controller
//!
//! @param pHandle is the Handle for the BLE module.
//! @param ui32Mode is determines the value of the WAKE signal.
//!
//! The BLE core needs to be awake before we send data to it. This function
//! sends a signal to the BLE core that tells it that we intend to send it
//! data. When the BLE core wakes up, it will generate a BLECSSTAT interrupt,
//! and the SPISTATUS bit in the BSTATUS register will be set.
//!
//! @return BLE status code.
//
//*****************************************************************************
extern uint32_t am_hal_ble_wakeup_set(void *pHandle, uint32_t ui32Mode);

//*****************************************************************************
//
//! @brief Read a register value directly from the BLE Core.
//!
//! @param pHandle is the Handle for the BLE module.
//! @param ui32Address is the address of the register.
//! @param *pui32Value is a pointer where the register value will be stored.
//!
//! This function uses a vendor-specific sequence of blocking HCI commands to
//! read one of the internal registers within the BLE Core. The value stored in
//! this register will be written to the location specified by \e pui32Value.
//!
//! This function is mostly used during initial radio setup or for internal
//! test commands. Standard applications will not need to call this function
//! directly.
//!
//! @note This function uses multiple blocking HCI commands in sequence. It
//! should not be used in any situation where blocking commands are not
//! desired. Do not use it in applications where interrupt-driven BLE
//! operations have already started.
//!
//! @return BLE status code.
//
//*****************************************************************************
extern uint32_t am_hal_ble_plf_reg_read(void *pHandle, uint32_t ui32Address, uint32_t *pui32Value);

//*****************************************************************************
//
//! @brief Write a register value directly to the BLE Core.
//!
//! @param pHandle is the Handle for the BLE module.
//! @param ui32Address is the address of the register.
//! @param ui32Value is the value to write.
//!
//! This function uses a vendor-specific sequence of blocking HCI commands to
//! write one of the internal registers within the BLE Core.
//!
//! This function is mostly used during initial radio setup or for internal
//! test commands. Standard applications will not need to call this function
//! directly.
//!
//! @note This function uses multiple blocking HCI commands in sequence. It
//! should not be used in any situation where blocking commands are not
//! desired. Do not use it in applications where interrupt-driven BLE
//! operations have already started.
//!
//! @return BLE status code.
//
//*****************************************************************************
extern uint32_t am_hal_ble_plf_reg_write(void *pHandle, uint32_t ui32Address, uint32_t ui32Value);

//*****************************************************************************
//
//! @brief Change the sleep behavior of the BLE core.
//!
//! @param pHandle is the Handle for the BLE module.
//! @param enable sets the desired sleep behavior.
//!
//! This function uses a vendor-specific sequence of blocking HCI commands to
//! change the default behavior of the BLE core between radio events. Set \e
//! enable to true to allow the BLE core to sleep between radio events, or
//! false to keep the BLE core awake at all times. The default behavior on
//! startup allows the BLE core to sleep. Most applications will not need to
//! modify this setting.
//!
//! @note This function uses multiple blocking HCI commands in sequence. It
//! should not be used in any situation where blocking commands are not
//! desired. Do not use it in applications where interrupt-driven BLE
//! operations have already started.
//!
//! @return BLE status code.
//
//*****************************************************************************
extern uint32_t am_hal_ble_sleep_set(void *pHandle, bool enable);

//*****************************************************************************
//
//! @brief Check the sleep behavior of the BLE core.
//!
//! @param pHandle is the Handle for the BLE module.
//!
//! This function uses a vendor-specific sequence of blocking HCI commands to
//! check whether the BLE core is set to go to sleep between BLE transactions.
//! This function will return "true" if BLE sleep is enabled, or "false" if it
//! is disabled.
//!
//! @note This function uses multiple blocking HCI commands in sequence. It
//! should not be used in any situation where blocking commands are not
//! desired. Do not use it in applications where interrupt-driven BLE
//! operations have already started.
//!
//! @return BLE status code.
//
//*****************************************************************************
extern bool am_hal_ble_sleep_get(void *pHandle);

//*****************************************************************************
//
//! @brief Change the TX power setting of the BLE core.
//!
//! @param pHandle is the Handle for the BLE module.
//! @param uint8_t is the desired power setting.
//!
//! This function uses a vendor-specific sequence of blocking HCI commands to
//! change the TX power setting of the radio.
//!
//! @note This function uses multiple blocking HCI commands in sequence. It
//! should not be used in any situation where blocking commands are not
//! desired. Do not use it in applications where interrupt-driven BLE
//! operations have already started.
//!
//! @return BLE status code.
//
//*****************************************************************************
extern uint32_t am_hal_ble_tx_power_set(void *pHandle, uint8_t ui32TxPower);

//*****************************************************************************
//
// Interrupts.
//
//*****************************************************************************
extern uint32_t am_hal_ble_int_service(void *pHandle, uint32_t ui32Status);
extern uint32_t am_hal_ble_int_enable(void *pHandle, uint32_t ui32InterruptMask);
extern uint32_t am_hal_ble_int_disable(void *pHandle, uint32_t ui32InterruptMask);
extern uint32_t am_hal_ble_int_status(void *pHandle, bool bEnabledOnly);
extern uint32_t am_hal_ble_int_clear(void *pHandle, uint32_t ui32InterruptMask);
extern uint32_t am_hal_ble_check_32k_clock(void *pHandle);
//*****************************************************************************
//
// Debug trace pins.
//
//*****************************************************************************
#ifdef AM_DEBUG_BLE_TIMING

#define BLE_DEBUG_TRACE_01            11
#define BLE_DEBUG_TRACE_02            28
#define BLE_DEBUG_TRACE_03            26
#define BLE_DEBUG_TRACE_04            4
#define BLE_DEBUG_TRACE_05            18
#define BLE_DEBUG_TRACE_06            14
#define BLE_DEBUG_TRACE_07            6
#define BLE_DEBUG_TRACE_08            45
#define BLE_DEBUG_TRACE_09            12
#define BLE_DEBUG_TRACE_10            13
#define BLE_DEBUG_TRACE_11            10
#define BLE_LOCK_TRACE_PIN            BLE_DEBUG_TRACE_11

#define am_hal_debug_gpio_set(x)    am_hal_gpio_state_write(x, AM_HAL_GPIO_OUTPUT_SET)

#define am_hal_debug_gpio_clear(x)    am_hal_gpio_state_write(x, AM_HAL_GPIO_OUTPUT_CLEAR)

#define am_hal_debug_gpio_toggle(x)    am_hal_gpio_state_write(x, AM_HAL_GPIO_OUTPUT_TOGGLE)

#define am_hal_debug_gpio_pinconfig(x)    am_hal_gpio_pinconfig(x, g_AM_HAL_GPIO_OUTPUT)

#else

#define am_hal_debug_gpio_set(...)
#define am_hal_debug_gpio_clear(...)
#define am_hal_debug_gpio_toggle(...)
#define am_hal_debug_gpio_pinconfig(...)

#endif // AM_DEBUG_BLE_TIMING

#ifdef __cplusplus
}
#endif

#endif // AM_HAL_BLE_H

//*****************************************************************************
//
// End Doxygen group.
//! @}
//
//*****************************************************************************
