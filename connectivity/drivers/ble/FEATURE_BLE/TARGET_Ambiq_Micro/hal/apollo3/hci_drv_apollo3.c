//*****************************************************************************
//
//! @file hci_drv_apollo3.c
//!
//! @brief HCI driver interface.
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
#if USE_AMBIQ_DRIVER

#include <stdint.h>
#include <stdbool.h>

#include "wsf_types.h"
#include "wsf_timer.h"
#include "bstream.h"
#include "wsf_msg.h"
#include "wsf_cs.h"
#include "hci_core.h"
#include "dm_api.h"
#include "am_mcu_apollo.h"
#include "am_util.h"
#include "hci_drv_apollo3.h"

#include <string.h>
#include "stdio.h"

extern void CordioHCITransportDriver_on_data_received(uint8_t *data, uint16_t len);

//*****************************************************************************
//
// Use the interrupt-driven HCI driver?
//
//*****************************************************************************
#define USE_NONBLOCKING_HCI             1
#define SKIP_FALLING_EDGES              0

//*****************************************************************************
//
// Enable the heartbeat command?
//
// Setting this to 1 will cause the MCU to send occasional HCI packets to the
// BLE core if there hasn't been any activity for a while. This can help catch
// communication issues that might otherwise go unnoticed.
//
//*****************************************************************************
#define ENABLE_BLE_HEARTBEAT            1

//*****************************************************************************
//
// Configurable buffer sizes.
//
//*****************************************************************************
#define NUM_HCI_WRITE_BUFFERS           8
#define HCI_DRV_MAX_TX_PACKET           256
#define HCI_DRV_MAX_RX_PACKET           256

//*****************************************************************************
//
// Configurable error-detection thresholds.
//
//*****************************************************************************
#define HEARTBEAT_TIMEOUT_MS            (10000)   //milli-seconds
#define HCI_DRV_MAX_IRQ_TIMEOUT          2000
#define HCI_DRV_MAX_XTAL_RETRIES         10
#define HCI_DRV_MAX_TX_RETRIES           10000
#define HCI_DRV_MAX_HCI_TRANSACTIONS     10000
#define HCI_DRV_MAX_READ_PACKET          4   // max read in a row at a time

//*****************************************************************************
//
// Structure for holding outgoing HCI packets.
//
//*****************************************************************************
typedef struct
{
    uint32_t ui32Length;
    uint32_t pui32Data[HCI_DRV_MAX_TX_PACKET / 4];
}
hci_drv_write_t;

//*****************************************************************************
//
// Heartbeat implementation functions.
//
//*****************************************************************************
#if ENABLE_BLE_HEARTBEAT

#define BLE_HEARTBEAT_START()                                                 \
    do { WsfTimerStartMs(&g_HeartBeatTimer, HEARTBEAT_TIMEOUT_MS); } while (0)

#define BLE_HEARTBEAT_STOP()                                                  \
    do { WsfTimerStop(&g_HeartBeatTimer); } while (0)

#define BLE_HEARTBEAT_RESTART()                                               \
    do                                                                        \
    {                                                                         \
        WsfTimerStop(&g_HeartBeatTimer);                                      \
        WsfTimerStartMs(&g_HeartBeatTimer, HEARTBEAT_TIMEOUT_MS);             \
    } while (0)

#else

#define BLE_HEARTBEAT_START()
#define BLE_HEARTBEAT_STOP()
#define BLE_HEARTBEAT_RESTART()

#endif

//*****************************************************************************
//
// Global variables.
//
//*****************************************************************************

// BLE module handle
void *BLE;

//fixme: set the BLE MAC address to a special value
uint8_t g_BLEMacAddress[6] = {0x01,0x02,0x03,0x04,0x05,0x06};

// Global handle used to send BLE events about the Hci driver layer.
wsfHandlerId_t g_HciDrvHandleID = 0;
wsfTimer_t g_HeartBeatTimer;
wsfTimer_t g_WakeTimer;

// Buffers for HCI write data.
hci_drv_write_t g_psWriteBuffers[NUM_HCI_WRITE_BUFFERS];
am_hal_queue_t g_sWriteQueue;

// Buffers for HCI read data.
uint32_t g_pui32ReadBuffer[HCI_DRV_MAX_RX_PACKET / 4];
uint8_t *g_pui8ReadBuffer = (uint8_t *) g_pui32ReadBuffer;
volatile bool bReadBufferInUse = false;

uint32_t g_ui32NumBytes   = 0;
uint32_t g_consumed_bytes = 0;

// Counters for tracking read data.
volatile uint32_t g_ui32InterruptsSeen = 0;

void HciDrvEmptyWriteQueue(void);
//*****************************************************************************
//
// Forward declarations for HCI callbacks.
//
//*****************************************************************************
#if USE_NONBLOCKING_HCI
void hciDrvWriteCallback(uint8_t *pui8Data, uint32_t ui32Length, void *pvContext);
void hciDrvReadCallback(uint8_t *pui8Data, uint32_t ui32Length, void *pvContext);
#endif // USE_NONBLOCKING_HCI

//*****************************************************************************
//
// Events for the HCI driver interface.
//
//*****************************************************************************
#define BLE_TRANSFER_NEEDED_EVENT                   0x01
#define BLE_HEARTBEAT_EVENT                         0x02
#define BLE_SET_WAKEUP                              0x03

//*****************************************************************************
//
// Error-handling wrapper macro.
//
//*****************************************************************************
#define ERROR_CHECK_VOID(status)                                              \
    {                                                                         \
        uint32_t ui32ErrChkStatus;                                            \
        if (0 != (ui32ErrChkStatus = (status)))                               \
        {                                                                     \
            am_util_debug_printf("ERROR_CHECK_VOID "#status "\n");            \
            error_check(ui32ErrChkStatus);                                    \
            return;                                                           \
        }                                                                     \
    }

#define ERROR_RETURN(status, retval)                                          \
    if ((status))                                                             \
    {                                                                         \
        error_check(status);                                                  \
        return (retval);                                                      \
    }

#define ERROR_RECOVER(status)                                                 \
    if ((status))                                                             \
    {                                                                         \
        am_hal_debug_gpio_toggle(BLE_DEBUG_TRACE_10);                         \
        error_check(status);                                                  \
        HciDrvRadioShutdown();                                                \
        HciDrvRadioBoot(0);                                                   \
        HciDrvEmptyWriteQueue();                                              \
        DmDevReset();                                                         \
        return;                                                               \
    }

//*****************************************************************************
//
// Debug section.
//
//*****************************************************************************
#if 0
#define CRITICAL_PRINT(...)                                                   \
    do                                                                        \
    {                                                                         \
        AM_CRITICAL_BEGIN;                                                    \
        am_util_debug_printf(__VA_ARGS__);                                    \
        AM_CRITICAL_END;                                                      \
    } while (0)
#else
#if 0
#define CRITICAL_PRINT(...) printf(__VA_ARGS__);
#endif
#define CRITICAL_PRINT(...)
#endif

#define ENABLE_IRQ_PIN 0

#define TASK_LEVEL_DELAYS 0


//*****************************************************************************
//
// Function pointer for redirecting errors
//
//*****************************************************************************
hci_drv_error_handler_t g_hciDrvErrorHandler = 0;
static uint32_t g_ui32FailingStatus = 0;

//*****************************************************************************
//
// By default, errors will be printed. If there is an error handler defined,
// they will be sent there intead.
//
//*****************************************************************************
static void
error_check(uint32_t ui32Status)
{
    //
    // Don't do anything unless there's an error.
    //
    if (ui32Status)
    {
        //
        // Set the global error status. If there's an error handler function,
        // call it. Otherwise, just print the error status and wait.
        //
        g_ui32FailingStatus = ui32Status;

        if (g_hciDrvErrorHandler)
        {
            g_hciDrvErrorHandler(g_ui32FailingStatus);
        }
        else
        {
            CRITICAL_PRINT("Error detected: 0x%08x\n", g_ui32FailingStatus);
            CRITICAL_PRINT("BSTATUS: 0x%08x\n", BLEIF->BSTATUS);
        }
    }
}

//*****************************************************************************
//
// Other useful macros.
//
//*****************************************************************************

#define BLE_IRQ_CHECK()             (BLEIF->BSTATUS_b.BLEIRQ)

// Ellisys HCI SPI tapping support

// #define ELLISYS_HCI_LOG_SUPPORT 1

//*****************************************************************************
//
// Boot the radio.
//
//*****************************************************************************
void
HciDrvRadioBoot(bool bColdBoot)
{
    uint32_t ui32NumXtalRetries = 0;


    g_ui32NumBytes     = 0;
    g_consumed_bytes   = 0;
#if !defined(AM_DEBUG_BLE_TIMING) && defined(ELLISYS_HCI_LOG_SUPPORT)
    am_hal_gpio_pincfg_t pincfg = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
    pincfg.uFuncSel = 6;
    am_hal_gpio_pinconfig(30, pincfg);
    am_hal_gpio_pinconfig(31, pincfg);
    am_hal_gpio_pinconfig(32, pincfg);
    pincfg.uFuncSel = 4;
    am_hal_gpio_pinconfig(33, pincfg);
    pincfg.uFuncSel = 7;
    am_hal_gpio_pinconfig(35, pincfg);
#endif

#ifdef AM_DEBUG_BLE_TIMING
    //
    // Enable debug pins.
    //
    // 30.6 - SCLK
    // 31.6 - MISO
    // 32.6 - MOSI
    // 33.4 - CSN
    // 35.7 - SPI_STATUS
    //
    am_hal_gpio_pincfg_t pincfg = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
    pincfg.uFuncSel = 6;
    am_hal_gpio_pinconfig(30, pincfg);
    am_hal_gpio_pinconfig(31, pincfg);
    am_hal_gpio_pinconfig(32, pincfg);
    pincfg.uFuncSel = 4;
    am_hal_gpio_pinconfig(33, pincfg);
    pincfg.uFuncSel = 7;
    am_hal_gpio_pinconfig(35, pincfg);
    pincfg.uFuncSel = 1;
#if ENABLE_IRQ_PIN
    am_hal_gpio_pinconfig(41, pincfg);
    am_hal_debug_gpio_pinconfig(BLE_DEBUG_TRACE_08);
#endif

    am_hal_gpio_pinconfig(11, g_AM_HAL_GPIO_OUTPUT);

#endif // AM_DEBUG_BLE_TIMING

    //
    // This pin is also used to generate BLE interrupts in the current
    // implementation.
    //
    // 41.1 - BLE IRQ
    //
    //am_hal_gpio_pin_config(41, AM_HAL_GPIO_FUNC(1));


    //
    // Configure and enable the BLE interface.
    //
    uint32_t ui32Status = AM_HAL_STATUS_FAIL;
    while (ui32Status != AM_HAL_STATUS_SUCCESS)
    {
        ERROR_CHECK_VOID(am_hal_ble_initialize(0, &BLE));
        ERROR_CHECK_VOID(am_hal_ble_power_control(BLE, AM_HAL_BLE_POWER_ACTIVE));

        am_hal_ble_config_t sBleConfig =
        {
            // Configure the HCI interface clock for 6 MHz
            .ui32SpiClkCfg = AM_HAL_BLE_HCI_CLK_DIV8,

            // Set HCI read and write thresholds to 32 bytes each.
            .ui32ReadThreshold = 32,
            .ui32WriteThreshold = 32,

            // The MCU will supply the clock to the BLE core.
            .ui32BleClockConfig = AM_HAL_BLE_CORE_MCU_CLK,
#if 0
            // Default settings for expected BLE clock drift (measured in PPM).
            .ui32ClockDrift = 0,
            .ui32SleepClockDrift = 50,

            // Default setting - AGC Enabled
            .bAgcEnabled = true,

            // Default setting - Sleep Algo enabled
            .bSleepEnabled = true,
#endif
            // Apply the default patches when am_hal_ble_boot() is called.
            .bUseDefaultPatches = true,
        };

        ERROR_CHECK_VOID(am_hal_ble_config(BLE, &sBleConfig));
        //
        // Delay 1s for 32768Hz clock stability. This isn't required unless this is
        // our first run immediately after a power-up.
        //
        if ( bColdBoot )
        {
            am_util_delay_ms(1000);
        }
        //
        // Attempt to boot the radio.
        //
        ui32Status = am_hal_ble_boot(BLE);

        //
        // Check our status.
        //
        if (ui32Status == AM_HAL_STATUS_SUCCESS)
        {
            //
            // If the radio is running, we can exit this loop.
            //
            break;
        }
        else if (ui32Status == AM_HAL_BLE_32K_CLOCK_UNSTABLE)
        {
            //
            // If the radio is running, but the clock looks bad, we can try to
            // restart.
            //
            ERROR_CHECK_VOID(am_hal_ble_power_control(BLE, AM_HAL_BLE_POWER_OFF));
            ERROR_CHECK_VOID(am_hal_ble_deinitialize(BLE));

            //
            // We won't restart forever. After we hit the maximum number of
            // retries, we'll just return with failure.
            //
            if (ui32NumXtalRetries++ < HCI_DRV_MAX_XTAL_RETRIES)
            {
                am_util_delay_ms(1000);
            }
            else
            {
                return;
            }
        }
        else
        {
            ERROR_CHECK_VOID(am_hal_ble_power_control(BLE, AM_HAL_BLE_POWER_OFF));
            ERROR_CHECK_VOID(am_hal_ble_deinitialize(BLE));
            //
            // If the radio failed for some reason other than 32K Clock
            // instability, we should just report the failure and return.
            //
            error_check(ui32Status);
            return;
        }
    }

    //
    // Set the BLE TX Output power to 0dBm.
    //
    am_hal_ble_tx_power_set(BLE, 0x8);

    //
    // Enable interrupts for the BLE module.
    //
#if USE_NONBLOCKING_HCI
    am_hal_ble_int_clear(BLE, (AM_HAL_BLE_INT_CMDCMP |
                               AM_HAL_BLE_INT_DCMP |
                               AM_HAL_BLE_INT_BLECIRQ |
                               AM_HAL_BLE_INT_BLECSSTAT));

    am_hal_ble_int_enable(BLE, (AM_HAL_BLE_INT_CMDCMP |
                                AM_HAL_BLE_INT_DCMP |
                                AM_HAL_BLE_INT_BLECIRQ |
                                AM_HAL_BLE_INT_BLECSSTAT));

#if SKIP_FALLING_EDGES
#else
    if (APOLLO3_GE_B0)
    {
        am_hal_ble_int_clear(BLE, (AM_HAL_BLE_INT_BLECIRQN |
                                   AM_HAL_BLE_INT_BLECSSTATN));

        am_hal_ble_int_enable(BLE, (AM_HAL_BLE_INT_BLECIRQN |
                                    AM_HAL_BLE_INT_BLECSSTATN));
    }
#endif

#else

    am_hal_ble_int_clear(BLE, (AM_HAL_BLE_INT_CMDCMP |
                               AM_HAL_BLE_INT_DCMP |
                               AM_HAL_BLE_INT_BLECIRQ));

    am_hal_ble_int_enable(BLE, (AM_HAL_BLE_INT_CMDCMP |
                                AM_HAL_BLE_INT_DCMP |
                                AM_HAL_BLE_INT_BLECIRQ));
#endif

    CRITICAL_PRINT("INTEN:  %d\n", BLEIF->INTEN_b.BLECSSTAT);
    CRITICAL_PRINT("INTENREG:  %d\n", BLEIF->INTEN);

    NVIC_SetVector(BLE_IRQn, HciDrvIntService);
    NVIC_EnableIRQ(BLE_IRQn);

    //
    // Initialize a queue to help us keep track of HCI write buffers.
    //
    am_hal_queue_from_array(&g_sWriteQueue, g_psWriteBuffers);

    //WsfSetEvent(g_HciDrvHandleID, BLE_HEARTBEAT_EVENT);
    //
    // Reset the RX interrupt counter.
    //
    g_ui32InterruptsSeen = 0;

    return;
}

//*****************************************************************************
//
// Shut down the BLE core.
//
//*****************************************************************************
void
HciDrvRadioShutdown(void)
{
    BLE_HEARTBEAT_STOP();

    NVIC_DisableIRQ(BLE_IRQn);

    ERROR_CHECK_VOID(am_hal_ble_power_control(BLE, AM_HAL_BLE_POWER_OFF));

    while ( PWRCTRL->DEVPWREN_b.PWRBLEL )
    {
    }

    ERROR_CHECK_VOID(am_hal_ble_deinitialize(BLE));

    g_ui32NumBytes   = 0;
    g_consumed_bytes = 0;
}

#if USE_NONBLOCKING_HCI
//*****************************************************************************
//
// Short Description.
//
//*****************************************************************************
static void
update_wake(void)
{
    AM_CRITICAL_BEGIN;

    //
    // We want to set WAKE if there's something in the write queue, but not if
    // SPISTATUS or IRQ is high.
    //
    if ( !am_hal_queue_empty(&g_sWriteQueue) &&
         (BLEIFn(0)->BSTATUS_b.SPISTATUS == 0) &&
         (BLE_IRQ_CHECK() == false))
    {
        am_hal_ble_wakeup_set(BLE, 1);

        //
        // If we've set wakeup, but IRQ came up at the same time, we should
        // just lower WAKE again.
        //
        if (BLE_IRQ_CHECK() == true)
        {
            am_hal_ble_wakeup_set(BLE, 0);
        }
    }

    AM_CRITICAL_END;
}
#endif

//*****************************************************************************
//
// Function used by the BLE stack to send HCI messages to the BLE controller.
//
// Internally, the Cordio BLE stack will allocate memory for an HCI message,
//
//*****************************************************************************
uint16_t
ap3_hciDrvWrite(uint8_t type, uint16_t len, uint8_t *pData)
{
    uint8_t *pui8Wptr;
    hci_drv_write_t *psWriteBuffer;


    //
    // Check to see if we still have buffer space.
    //
    if (am_hal_queue_full(&g_sWriteQueue))
    {
        CRITICAL_PRINT("ERROR: Ran out of HCI transmit queue slots.\n");
        ERROR_RETURN(HCI_DRV_TRANSMIT_QUEUE_FULL, len);
    }

    if (len > (HCI_DRV_MAX_TX_PACKET-1))  // comparison compensates for the type byte at index 0.
    {
        CRITICAL_PRINT("ERROR: Trying to send an HCI packet larger than the hci driver buffer size (needs %d bytes of space).\n",
                       len);

        ERROR_RETURN(HCI_DRV_TX_PACKET_TOO_LARGE, len);
    }

    //
    // Get a pointer to the next item in the queue.
    //
    psWriteBuffer = am_hal_queue_next_slot(&g_sWriteQueue);

    //
    // Set all of the fields in the hci write structure.
    //
    psWriteBuffer->ui32Length = len + 1;

    pui8Wptr = (uint8_t *) psWriteBuffer->pui32Data;

    *pui8Wptr++ = type;

    for (uint32_t i = 0; i < len; i++)
    {
        pui8Wptr[i] = pData[i];
    }

    //
    // Advance the queue.
    //
    am_hal_queue_item_add(&g_sWriteQueue, 0, 1);

#if USE_NONBLOCKING_HCI
    //
    // Wake up the BLE controller.
    //
    CRITICAL_PRINT("INFO: HCI write requested.\n");

    update_wake();

#else
    //
    // Send an event to the BLE transfer handler function.
    //
    WsfSetEvent(g_HciDrvHandleID, BLE_TRANSFER_NEEDED_EVENT);
#endif

#ifdef AM_CUSTOM_BDADDR
    if (type == HCI_CMD_TYPE)
    {
        uint16_t opcode;
        BYTES_TO_UINT16(opcode, pData);

        if (HCI_OPCODE_RESET == opcode)
        {

            extern uint8_t g_BLEMacAddress[6];
            am_hal_mcuctrl_device_t sDevice;
            am_hal_mcuctrl_info_get(AM_HAL_MCUCTRL_INFO_DEVICEID, &sDevice);
            g_BLEMacAddress[0] = sDevice.ui32ChipID0;
            g_BLEMacAddress[1] = sDevice.ui32ChipID0 >> 8;
            g_BLEMacAddress[2] = sDevice.ui32ChipID0 >> 16;

            HciVendorSpecificCmd(0xFC32, 6, g_BLEMacAddress);
        }
    }
#endif

    return len;
}

//*****************************************************************************
//
// Save the handler ID of the HciDrvHandler so we can send it events through
// the WSF task system.
//
// Note: These two lines need to be added to the exactle initialization
// function at the beginning of all Cordio applications:
//
//     handlerId = WsfOsSetNextHandler(HciDrvHandler);
//     HciDrvHandler(handlerId);
//
//*****************************************************************************
void
HciDrvHandlerInit(wsfHandlerId_t handlerId)
{
    g_HciDrvHandleID = handlerId;

    g_HeartBeatTimer.handlerId = handlerId;
    g_HeartBeatTimer.msg.event = BLE_HEARTBEAT_EVENT;

    g_WakeTimer.handlerId = handlerId;
    g_WakeTimer.msg.event = BLE_SET_WAKEUP;
}

//*****************************************************************************
//
// Simple interrupt handler to call
//
// Note: These two lines need to be added to the exactle initialization
// function at the beginning of all Cordio applications:
//
//     handlerId = WsfOsSetNextHandler(HciDrvHandler);
//     HciDrvHandler(handlerId);
//
//*****************************************************************************
void
HciDrvIntService(void)
{
#if AM_DEBUG_BLE_TIMING
    am_hal_gpio_state_write(11, AM_HAL_GPIO_OUTPUT_SET);
#endif

    //
    // Read and clear the interrupt status.
    //
    uint32_t ui32Status = am_hal_ble_int_status(BLE, true);
    am_hal_ble_int_clear(BLE, ui32Status);

#if USE_NONBLOCKING_HCI
    //
    // Handle any DMA or Command Complete interrupts.
    //
    am_hal_ble_int_service(BLE, ui32Status);

    //
    // If this was a BLEIRQ interrupt, attempt to start a read operation. If it
    // was a STATUS interrupt, start a write operation.
    //
    if (ui32Status & AM_HAL_BLE_INT_BLECIRQ)
    {
        // CRITICAL_PRINT("INFO: IRQ INTERRUPT\n");

        //
        // Lower WAKE
        //
        //WsfTimerStop(&g_WakeTimer);
        // CRITICAL_PRINT("IRQ Drop\n");
        am_hal_ble_wakeup_set(BLE, 0);

        //
        // Prepare to read a message.
        //
        WsfSetEvent(g_HciDrvHandleID, BLE_TRANSFER_NEEDED_EVENT);
    }
    else if (ui32Status & AM_HAL_BLE_INT_BLECSSTAT)
    {
        // CRITICAL_PRINT("INFO: STATUS INTERRUPT\n");

        //
        // Check the queue and send the first message we have.
        //
        if ( !am_hal_queue_empty(&g_sWriteQueue) )
        {
            uint32_t ui32WriteStatus = 0;

            hci_drv_write_t *psWriteBuffer = am_hal_queue_peek(&g_sWriteQueue);

            ui32WriteStatus =
                am_hal_ble_nonblocking_hci_write(BLE,
                                                 AM_HAL_BLE_RAW,
                                                 psWriteBuffer->pui32Data,
                                                 psWriteBuffer->ui32Length,
                                                 hciDrvWriteCallback,
                                                 0);

            //
            // If it succeeded, we can pop the queue.
            //
            if (ui32WriteStatus == AM_HAL_STATUS_SUCCESS)
            {
                BLE_HEARTBEAT_RESTART();
                // CRITICAL_PRINT("INFO: HCI write sent.\n");
            }
            else
            {
                // CRITICAL_PRINT("INFO: HCI write failed: %d\n", ui32WriteStatus);
            }
        }
    }

#else
    //
    // Advance an event counter to make sure we're keeping track of edges
    // correctly.
    //
    g_ui32InterruptsSeen++;

    //
    // Send an event to get processed in the HCI handler.
    //
    WsfSetEvent(g_HciDrvHandleID, BLE_TRANSFER_NEEDED_EVENT);
#endif

#if AM_DEBUG_BLE_TIMING
    am_hal_gpio_state_write(11, AM_HAL_GPIO_OUTPUT_CLEAR);
#endif
}

#if USE_NONBLOCKING_HCI

//*****************************************************************************
//
// This function determines what to do when a write operation completes.
//
//*****************************************************************************
void
hciDrvWriteCallback(uint8_t *pui8Data, uint32_t ui32Length, void *pvContext)
{
    CRITICAL_PRINT("INFO: HCI physical write complete.\n");

    am_hal_queue_item_get(&g_sWriteQueue, 0, 1);

#if TASK_LEVEL_DELAYS

    // Set a WSF timer to update wake later.
    WsfTimerStartMs(&g_WakeTimer, 30);

#else // TASK_LEVEL_DELAYS

    while ( BLEIFn(0)->BSTATUS_b.SPISTATUS )
    {
        am_util_delay_us(5);
    }

    //
    // Check the write queue, and possibly set wake again.
    //
    if ( !am_hal_queue_empty(&g_sWriteQueue) )
    {
        //
        // In this case, we need to delay before setting wake. Instead of
        // delaying here, we'll post an event to do it later.
        //
        WsfSetEvent(g_HciDrvHandleID, BLE_TRANSFER_NEEDED_EVENT);
    }

#endif // TASK_LEVEL_DELAYS
}

//*****************************************************************************
//
// This function determines what to do when a read operation completes.
//
//*****************************************************************************
void
hciDrvReadCallback(uint8_t *pui8Data, uint32_t ui32Length, void *pvContext)
{
    //
    // Set a "transfer needed" event.
    //
    // CRITICAL_PRINT("INFO: HCI physical read complete.\n");
    g_ui32NumBytes = ui32Length;
    WsfSetEvent(g_HciDrvHandleID, BLE_TRANSFER_NEEDED_EVENT);

#if TASK_LEVEL_DELAYS

    // Set a WSF timer to update wake later.
    WsfTimerStartMs(&g_WakeTimer, 30);

#else // TASK_LEVEL_DELAYS

    while ( BLE_IRQ_CHECK() )
    {
        am_util_delay_us(5);
    }

    //
    // Check the write queue, and possibly set wake.
    //
    if ( !am_hal_queue_empty(&g_sWriteQueue) )
    {
        am_hal_ble_wakeup_set(BLE, 1);
    }

#endif // TASK_LEVEL_DELAYS
}

//*****************************************************************************
//
// Event handler for HCI-related events.
//
// This handler can perform HCI reads or writes, and keeps the actions in the
// correct order.
//
//*****************************************************************************
void
HciDrvHandler(wsfEventMask_t event, wsfMsgHdr_t *pMsg)
{
    uint32_t ui32ErrorStatus;
 
    //
    // If this handler was called in response to a heartbeat event, then it's
    // time to run a benign HCI command. Normally, the BLE controller should
    // handle this command without issue. If it doesn't acknowledge the
    // command, we will eventually get an HCI command timeout error, which will
    // alert us to the fact that the BLE core has become unresponsive in
    // general.
    //
    if (pMsg->event == BLE_HEARTBEAT_EVENT)
    {
        HciReadLocalVerInfoCmd();
        BLE_HEARTBEAT_START();
        return;
    }

    if (pMsg->event == BLE_SET_WAKEUP)
    {
        //
        // Attempt to set WAKE again.
        //
        update_wake();
        return;
    }

    //
    // Check to see if we read any bytes over the HCI interface that we haven't
    // already sent to the BLE stack.
    //
    if (g_ui32NumBytes > g_consumed_bytes)
    {
        CRITICAL_PRINT("INFO: HCI data transferred to stack.\n");
        //
        // If we have any bytes saved, we should send them to the BLE stack
        // now.
        //
        CordioHCITransportDriver_on_data_received(g_pui8ReadBuffer + g_consumed_bytes,
                                                  g_ui32NumBytes - g_consumed_bytes);
        g_consumed_bytes = g_ui32NumBytes;
        //g_consumed_bytes += hciTrSerialRxIncoming(g_pui8ReadBuffer + g_consumed_bytes,
        //                                          g_ui32NumBytes - g_consumed_bytes);

        //
        // If the stack doesn't accept all of the bytes we had, we will need to
        // keep the event set and come back later. Otherwise, we can just reset
        // our variables and exit the loop.
        //
        if (g_consumed_bytes != g_ui32NumBytes)
        {
            CRITICAL_PRINT("INFO: HCI data split up.\n");
            WsfSetEvent(g_HciDrvHandleID, BLE_TRANSFER_NEEDED_EVENT);
            return;
        }
        else
        {
            CRITICAL_PRINT("INFO: HCI RX packet complete.\n");
            g_ui32NumBytes   = 0;
            g_consumed_bytes = 0;
            bReadBufferInUse = false;
        }
    }

    if ( BLE_IRQ_CHECK() )
    {
        if (bReadBufferInUse == true)
        {
            CRITICAL_PRINT("Read buffer already in use.\n");
            WsfSetEvent(g_HciDrvHandleID, BLE_TRANSFER_NEEDED_EVENT);
            return;
        }

        //
        // If the stack has used up all of the saved data we've accumulated so
        // far, we should check to see if we need to read any *new* data.
        //
        CRITICAL_PRINT("INFO: HCI Read started.\n");
        bReadBufferInUse = true;
        ui32ErrorStatus = am_hal_ble_nonblocking_hci_read(BLE,
                                                          g_pui32ReadBuffer,
                                                          hciDrvReadCallback,
                                                          0);

        BLE_HEARTBEAT_RESTART();

        if (g_ui32NumBytes > HCI_DRV_MAX_RX_PACKET)
        {
            CRITICAL_PRINT("ERROR: Trying to receive an HCI packet "
                           "larger than the hci driver buffer size "
                           "(needs %d bytes of space).\n",
                           g_ui32NumBytes);

            ERROR_CHECK_VOID(HCI_DRV_RX_PACKET_TOO_LARGE);
        }

        if (ui32ErrorStatus != AM_HAL_STATUS_SUCCESS)
        {
            //
            // If the read didn't succeed for some physical reason, we need
            // to know. We shouldn't get failures here. We checked the IRQ
            // signal before calling the read function, and this driver
            // only contains a single call to the blocking read function,
            // so there shouldn't be any physical reason for the read to
            // fail.
            //
            CRITICAL_PRINT("HCI READ failed with status %d. "
                           "Try recording with a logic analyzer "
                           "to catch the error.\n",
                           ui32ErrorStatus);

            ERROR_RECOVER(ui32ErrorStatus);
        }
    }
}
#else
//*****************************************************************************
//
// Event handler for HCI-related events.
//
// This handler can perform HCI reads or writes, and keeps the actions in the
// correct order.
//
//*****************************************************************************
void
HciDrvHandler(wsfEventMask_t event, wsfMsgHdr_t *pMsg)
{
    uint32_t ui32ErrorStatus, ui32TxRetries = 0;
    uint32_t ui32NumHciTransactions = 0;
    uint32_t read_hci_packet_count = 0;

    //
    // If this handler was called in response to a heartbeat event, then it's
    // time to run a benign HCI command. Normally, the BLE controller should
    // handle this command without issue. If it doesn't acknowledge the
    // command, we will eventually get an HCI command timeout error, which will
    // alert us to the fact that the BLE core has become unresponsive in
    // general.
    //
    if (pMsg->event == BLE_HEARTBEAT_EVENT)
    {
        HciReadLocalVerInfoCmd();
        BLE_HEARTBEAT_START();
        return;
    }

    //
    // Check to see if we read any bytes over the HCI interface that we haven't
    // already sent to the BLE stack.
    //
    if (g_ui32NumBytes > g_consumed_bytes)
    {
        //
        // If we have any bytes saved, we should send them to the BLE stack
        // now.
        //
        CordioHCITransportDriver_on_data_received(g_pui8ReadBuffer + g_consumed_bytes,
                                                  g_ui32NumBytes - g_consumed_bytes);
        g_consumed_bytes = g_ui32NumBytes;
        // g_consumed_bytes += hciTrSerialRxIncoming(g_pui8ReadBuffer + g_consumed_bytes,
        //                                           g_ui32NumBytes - g_consumed_bytes);

        //
        // If the stack doesn't accept all of the bytes we had,
        //
        if (g_consumed_bytes != g_ui32NumBytes)
        {
            WsfSetEvent(g_HciDrvHandleID, BLE_TRANSFER_NEEDED_EVENT);
            return;
        }
        else
        {
            g_ui32NumBytes   = 0;
            g_consumed_bytes = 0;
        }
    }

    am_hal_debug_gpio_set(BLE_DEBUG_TRACE_01);

    //
    // Loop indefinitely, checking to see if there are still tranfsers we need
    // to complete.
    //
    while (ui32NumHciTransactions++ < HCI_DRV_MAX_HCI_TRANSACTIONS)
    {
        //
        // Figure out what kind of transfer the BLE core will accept.
        //
        if ( BLE_IRQ_CHECK() )
        {
            uint32_t ui32OldInterruptsSeen = g_ui32InterruptsSeen;

            am_hal_debug_gpio_set(BLE_DEBUG_TRACE_02);

            BLE_HEARTBEAT_RESTART();

            //
            // Is the BLE core asking for a read? If so, do that now.
            //
            g_ui32NumBytes = 0;
            ui32ErrorStatus = am_hal_ble_blocking_hci_read(BLE, (uint32_t*)g_pui32ReadBuffer, &g_ui32NumBytes);

            if (g_ui32NumBytes > HCI_DRV_MAX_RX_PACKET)
            {
                CRITICAL_PRINT("ERROR: Trying to receive an HCI packet larger than the hci driver buffer size (needs %d bytes of space).",
                               g_ui32NumBytes);

                ERROR_CHECK_VOID(HCI_DRV_RX_PACKET_TOO_LARGE);
            }

            if ( ui32ErrorStatus == AM_HAL_STATUS_SUCCESS)
            {

                //
                // If the read succeeded, we need to wait for the IRQ signal to
                // go back down. If we don't we might inadvertently try to read
                // the same packet twice.
                //
                uint32_t ui32IRQRetries;
                for (ui32IRQRetries = 0; ui32IRQRetries < HCI_DRV_MAX_IRQ_TIMEOUT; ui32IRQRetries++)
                {
                    if (BLE_IRQ_CHECK() == 0 || g_ui32InterruptsSeen != ui32OldInterruptsSeen)
                    {
                        break;
                    }

                    am_util_delay_us(1);
                }

                //
                // Pass the data along to the stack. The stack should be able
                // to read as much data as we send it.  If it can't, we need to
                // know that.
                //

                CordioHCITransportDriver_on_data_received(g_pui8ReadBuffer, g_ui32NumBytes);
                g_consumed_bytes = g_ui32NumBytes;
				// g_consumed_bytes = hciTrSerialRxIncoming(g_pui8ReadBuffer, g_ui32NumBytes);
                if (g_consumed_bytes != g_ui32NumBytes)
                {

                    // need to come back again
                    WsfSetEvent(g_HciDrvHandleID, BLE_TRANSFER_NEEDED_EVENT);
                    // take a break now

                    // worst case disable BLE_IRQ
                    break;
                }

                g_ui32NumBytes   = 0;
                g_consumed_bytes = 0;

                read_hci_packet_count++;
            }
            else
            {
                //
                // If the read didn't succeed for some physical reason, we need
                // to know. We shouldn't get failures here. We checked the IRQ
                // signal before calling the read function, and this driver
                // only contains a single call to the blocking read function,
                // so there shouldn't be any physical reason for the read to
                // fail.
                //
                CRITICAL_PRINT("HCI READ failed with status %d. Try recording with a logic analyzer to catch the error.\n",
                               ui32ErrorStatus);

                ERROR_RECOVER(ui32ErrorStatus);
            }

            am_hal_debug_gpio_clear(BLE_DEBUG_TRACE_02);

            if (read_hci_packet_count >= HCI_DRV_MAX_READ_PACKET)
            {
                // It looks like there's time that we won't get interrupt even though
                // there's packet waiting for host to grab.
                WsfSetEvent(g_HciDrvHandleID, BLE_TRANSFER_NEEDED_EVENT);

                break;
            }

        }
        else
        {
            //
            // If we don't have anything to read, we can start checking to see
            // if we have things to write.
            //
            if (am_hal_queue_empty(&g_sWriteQueue))
            {
                //
                // If not, we're done!
                //
                break;
            }
            else
            {
                //
                // If we do have something to write, just pop a single item
                // from the queue and send it.
                //
                am_hal_debug_gpio_set(BLE_DEBUG_TRACE_07);
                hci_drv_write_t *psWriteBuffer = am_hal_queue_peek(&g_sWriteQueue);

                ui32ErrorStatus = am_hal_ble_blocking_hci_write(BLE,
                                                                AM_HAL_BLE_RAW,
                                                                psWriteBuffer->pui32Data,
                                                                psWriteBuffer->ui32Length);

                //
                // If we managed to actually send a packet, we can go ahead and
                // advance the queue.
                //
                if (ui32ErrorStatus == AM_HAL_STATUS_SUCCESS)
                {
                    //
                    // Restart the heartbeat timer.
                    //
                    BLE_HEARTBEAT_RESTART();

                    am_hal_queue_item_get(&g_sWriteQueue, 0, 1);

                    ui32TxRetries = 0;
                    // Resetting the cumulative count
                    ui32NumHciTransactions = 0;
                }
                else
                {
                    //
                    // If we fail too many times in a row, we should throw an
                    // error to avoid a lock-up.
                    //
                    ui32TxRetries++;

                    if (ui32TxRetries > HCI_DRV_MAX_TX_RETRIES)
                    {
                        // we need to come back again later.
                        WsfSetEvent(g_HciDrvHandleID, BLE_TRANSFER_NEEDED_EVENT);
                        break;
                    }
                }

            }
        }
    }

    if (ui32NumHciTransactions == HCI_DRV_MAX_HCI_TRANSACTIONS)
    {
        CRITICAL_PRINT("ERROR: Maximum number of successive HCI transactions exceeded.\n");
        ERROR_RECOVER(HCI_DRV_TOO_MANY_PACKETS);
    }

    am_hal_debug_gpio_clear(BLE_DEBUG_TRACE_01);
}
#endif

//*****************************************************************************
//
// Register an error handler for the HCI driver.
//
//*****************************************************************************
void
HciDrvErrorHandlerSet(hci_drv_error_handler_t pfnErrorHandler)
{
    g_hciDrvErrorHandler = pfnErrorHandler;
}

/*************************************************************************************************/
/*!
 *  \fn     HciVsA3_SetRfPowerLevelEx
 *
 *  \brief  Vendor specific command for settting Radio transmit power level
 *          for Nationz.
 *
 *  \param  txPowerlevel    valid range from 0 to 15 in decimal.
 *
 *  \return true when success, otherwise false
 */
/*************************************************************************************************/
bool_t
HciVsA3_SetRfPowerLevelEx(txPowerLevel_t txPowerlevel)
{
    switch (txPowerlevel) {

        case TX_POWER_LEVEL_MINUS_10P0_dBm:
            am_hal_ble_tx_power_set(BLE,0x04);
            return true;
            break;
        case TX_POWER_LEVEL_0P0_dBm:
            am_hal_ble_tx_power_set(BLE,0x08);
            return true;
            break;
        case TX_POWER_LEVEL_PLUS_3P0_dBm:
            am_hal_ble_tx_power_set(BLE,0x0F);
            return true;
            break;
        default:
            return false;
            break;
    }
}

/*************************************************************************************************/
/*!
 *  \fn     HciDrvBleSleepSet
 *
 *  \brief  Set BLE sleep enable/disable for the BLE core.
 *
 *  \param  enable 'true' set sleep enable, 'false' set sleep disable
 *
 *  \return none
 */
/*************************************************************************************************/
void
HciDrvBleSleepSet(bool enable)
{
    am_hal_ble_sleep_set(BLE, enable);
}

//*****************************************************************************
//
// Clear the HCI write queue
//
//*****************************************************************************
void
HciDrvEmptyWriteQueue(void)
{
    am_hal_queue_from_array(&g_sWriteQueue, g_psWriteBuffers);
}

#endif
