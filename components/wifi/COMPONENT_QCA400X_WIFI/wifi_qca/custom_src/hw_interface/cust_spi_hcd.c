//------------------------------------------------------------------------------
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
//------------------------------------------------------------------------------
//==============================================================================
// Author(s): ="Atheros"
//==============================================================================

#include <a_config.h>
#include <a_types.h>
#include <a_osapi.h>
#include <driver_cxt.h>
#include <common_api.h>
#include <custom_wlan_api.h>
#include <hif_internal.h>
#include "spi_hcd_if.h"
#include <targaddrs.h>
#include "hw20_mbox_host_reg.h"
#include <atheros_wifi.h>
#include <stdlib.h>
#include "atheros_wifi_api.h"

// ============================================================================
//
// ============================================================================

#define SPI_POWER_UP_DELAY (1)
#define SPI_HW_CAPS (HW_SPI_FRAME_WIDTH_8 | HW_SPI_NO_DMA | HW_SPI_INT_EDGE_DETECT)
#define SPI_MAX_ALLOWED_BAUD_RATE 25000000

// ============================================================================
//
// ============================================================================

// DROPPED
// void Custom_Hcd_EnableDisableSPIIRQ(void *pCxt, boolean enable)
//{
//	A_CUSTOM_DRIVER_CONTEXT *pCustCxt = (A_CUSTOM_DRIVER_CONTEXT *) pCxt;
//	wlan_hardware_config_t *wlanHardwareConfig = (wlan_hardware_config_t *) pCustCxt->pDriverParam;
//	IRQn_Type irqNumber = wlanHardwareConfig->wlan_irq_number;
//
//	if (enable) {
//		INT_SYS_EnableIRQ(irqNumber);
//	}
//	else {
//		INT_SYS_DisableIRQ(irqNumber);
//	}
//}

// A_STATUS Custom_HW_Init(void *pCxt)
//{
//	// TODO: Check endianess of SPI
//
//	A_CUSTOM_DRIVER_CONTEXT *pCustCxt = (A_CUSTOM_DRIVER_CONTEXT *) pCxt;
//	wlan_hardware_config_t *wlanHardwareConfig = (wlan_hardware_config_t *) pCustCxt->pDriverParam;
//
//	A_SPI_CTX *spiCxt = &pCustCxt->customhcdcxt.spi_cxt;
//
//	// Copy the SPI instance number from the configuration to the local copy
//	spiCxt->spi_instance = wlanHardwareConfig->spi_instance;
//	//uint32_t spiClk = CLOCK_SYS_GetSpiFreq(spiCxt->spi_instance);
//
//    uint32_t baudrate = wlanHardwareConfig->spi_baudrate;
//    //if (baudrate > SPI_MAX_ALLOWED_BAUD_RATE)
//    //    baudrate = SPI_MAX_ALLOWED_BAUD_RATE;
//
//
//
////    spiCxt->userConfig.isChipSelectContinuous = false;
////    spiCxt->userConfig.isSckContinuous = false;
////    spiCxt->userConfig.pcsPolarity = kDspiPcs_ActiveLow;
////    spiCxt->userConfig.whichCtar = kDspiCtar0;
////    spiCxt->userConfig.whichPcs = wlanHardwareConfig->spi_cs;
//    spiCxt->masterDevice.dataBusConfig.bitsPerFrame = 8;
//    // spiCxt->masterDevice.dataBusConfig.clkPhase = kDspiClockPhase_FirstEdge;
//    spiCxt->masterDevice.dataBusConfig.clkPhase = kDspiClockPhase_SecondEdge;
//    spiCxt->masterDevice.dataBusConfig.clkPolarity = kDspiClockPolarity_ActiveLow;
//    spiCxt->masterDevice.dataBusConfig.direction = kDspiMsbFirst;
//
//    spiCxt->masterDevice.bitsPerSec = baudrate;
//
//    // Initialize master driver
//    dspi_status_t res;
//    res = DSPI_DRV_MasterInit(spiCxt->spi_instance,
//                              &spiCxt->masterState,
//                              &spiCxt->userConfig);
//    if (spiCxt->spiResult != kStatus_DSPI_Success) {
//        return res;
//    }
//
//    res = DSPI_DRV_MasterConfigureBus(spiCxt->spi_instance,
//                                      &spiCxt->masterDevice,
//                                      &spiCxt->calculatedBaudRate);
//    if (res != kStatus_DSPI_Success) {
//        return res;
//    }
//
//
//	// It is necessary for the custom code to populate the following parameters
//	// so that the common code knows what kind of hardware it is working with
//	A_DRIVER_CONTEXT *pDCxt = GET_DRIVER_COMMON(pCxt);
//	pDCxt->spi_hcd.OperationalClock = spiCxt->calculatedBaudRate;
//	pDCxt->spi_hcd.PowerUpDelay = SPI_POWER_UP_DELAY;
//	pDCxt->spi_hcd.SpiHWCapabilitiesFlags = SPI_HW_CAPS;
//	pDCxt->spi_hcd.MiscFlags |= MISC_FLAG_RESET_SPI_IF_SHUTDOWN;
//
//	return A_OK;
//}
