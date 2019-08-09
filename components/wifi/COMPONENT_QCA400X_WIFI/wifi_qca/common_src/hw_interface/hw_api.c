// Copyright (c) Qualcomm Atheros, Inc.
// All rights reserved.
// Redistribution and use in source and binary forms, with or without modification, are permitted (subject to
// the limitations in the disclaimer below) provided that the following conditions are met:
//
// · Redistributions of source code must retain the above copyright notice, this list of conditions and the
//   following disclaimer.
// · Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the
//   following disclaimer in the documentation and/or other materials provided with the distribution.
// · Neither the name of nor the names of its contributors may be used to endorse or promote products derived
//   from this software without specific prior written permission.
//
// NO EXPRESS OR IMPLIED LICENSES TO ANY PARTY'S PATENT RIGHTS ARE GRANTED BY THIS LICENSE. THIS SOFTWARE IS
// PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING,
// BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
// DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT,
// INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
// SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON
// ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
// ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
//==============================================================================
// Author(s): ="Atheros"
//==============================================================================

#include <a_config.h>
#include <a_types.h>
#include <a_osapi.h>
#include <driver_cxt.h>
#include <common_api.h>
#include <custom_wlan_api.h>
#include <hcd_api.h>
#include <spi_hcd_if.h>
#include <hif_internal.h>
#include <atheros_wifi_api.h>
#include <atheros_wifi_internal.h>
#include <wlan_api.h>

A_STATUS HW_ProcessPendingInterrupts(void *pCxt)
{
    A_DRIVER_CONTEXT *pDCxt = GET_DRIVER_COMMON(pCxt);
    uint8_t cpuIntrCause;

    pDCxt->driver_state = DRIVER_STATE_PENDING_CONDITION_A;

    /* identify the interrupt and mask it */
    if (pDCxt->spi_hcd.IrqDetected == true)
    {
        pDCxt->driver_state = DRIVER_STATE_INTERRUPT_PROCESSING;
        pDCxt->spi_hcd.IrqDetected = false;

        if (false == Hcd_BusInterrupt(pCxt))
        {
            /* FIXME: HANDLE ERROR CONDITION */
        }
    }

    pDCxt->driver_state = DRIVER_STATE_PENDING_CONDITION_B;
    /* check to see if a deferred bus request has completed. if so
     * process it. */
    if (pDCxt->booleans & SDHD_BOOL_DMA_COMPLETE)
    {
        /* get the request */
        if (A_OK == Driver_CompleteRequest(pCxt, pDCxt->spi_hcd.pCurrentRequest))
        {
            /* clear the pending request and the boolean */
            pDCxt->spi_hcd.pCurrentRequest = NULL;
            pDCxt->booleans &= ~SDHD_BOOL_DMA_COMPLETE;
        }
    }

    if (pDCxt->spi_hcd.CpuInterrupt)
    {
        cpuIntrCause = 0x0;
        if (Hcd_ReadCPUInterrupt(pCxt, &cpuIntrCause))
        {
            pDCxt->spi_hcd.CpuInterruptCause = cpuIntrCause;
        }
        pDCxt->driver_state = DRIVER_STATE_INTERRUPT_PROCESSING;
        Hcd_ClearCPUInterrupt(pCxt);
        Hcd_UnmaskInterrupts(pCxt, ATH_SPI_INTR_LOCAL_CPU_INTR);
        HTC_ProcessCpuInterrupt(pCxt);
        pDCxt->spi_hcd.CpuInterrupt = 0;
    }

    pDCxt->driver_state = DRIVER_STATE_PENDING_CONDITION_C;
    return A_OK;
}

void HW_PowerUpDown(void *pCxt, uint8_t powerUp)
{
#if WLAN_CONFIG_ENABLE_CHIP_PWD_GPIO
    /* NOTE: when powering down it is necessary that the SPI MOSI on the MCU
     * be configured high so that current does not leak from the AR6003.
     * Hence because the SPI interface retains its last state we write a single
     * 0xf out the SPI bus prior to powering the chip down.
     */
    if (powerUp == 0)
    {
        Bus_InOutToken(pCxt, (uint32_t)0xff, 1, NULL);
    }

    HW_USEC_DELAY(pCxt, 1000);
    CUSTOM_HW_POWER_UP_DOWN(pCxt, powerUp);

    if (powerUp)
    {
        if (ath_custom_init.Custom_reset_measure_timer != NULL)
        {
            ath_custom_init.Custom_reset_measure_timer();
        }

        // delay to give chip time to come alive
        HW_USEC_DELAY(pCxt, 2000);
    }
#endif
}

void HW_InterruptHandler(void *pCxt)
{
    if (pCxt)
    {
        GET_DRIVER_COMMON(pCxt)->spi_hcd.IrqDetected = true;

        /* disable IRQ while we process it */
        HW_EnableDisableIRQ(pCxt, false, true);
        /* startup work item to process IRQs */
        DRIVER_WAKE_DRIVER(pCxt);
    }
}

void HW_SetClock(void *pCxt, uint32_t *pClockRate)
{
    UNUSED_ARGUMENT(pCxt);
    UNUSED_ARGUMENT(pClockRate);
}

uint32_t HW_GetMboxAddress(void *pCxt, uint16_t mbox, uint32_t length)
{
    uint32_t address = 0;

    if (mbox == 0)
    {
        address = GET_DRIVER_COMMON(pCxt)->mboxAddress + HIF_MBOX_WIDTH - length;
    }
    else
    {
        A_ASSERT(0);
    }

    return address;
}
