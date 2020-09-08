//*****************************************************************************
//
//  am_hal_tpiu.c
//! @file
//!
//! @brief Support functions for the ARM TPIU module
//!
//! Provides support functions for configuring the ARM TPIU module
//!
//! @addtogroup tpiu3 Trace Port Interface Unit (TPIU)
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
//! @brief Enable the clock to the TPIU module.
//!
//! This function enables the clock to the TPIU module.
//!
//! @return None.
//
//*****************************************************************************
void
am_hal_tpiu_clock_enable(void)
{
    //
    // Enable the TPIU clock
    //
    MCUCTRL->TPIUCTRL |= MCUCTRL_TPIUCTRL_ENABLE_Msk;
}

//*****************************************************************************
//
//! @brief Disable the clock to the TPIU module.
//!
//! This function disables the clock to the TPIU module.
//!
//! @return None.
//
//*****************************************************************************
void
am_hal_tpiu_clock_disable(void)
{
    //
    // Disable the TPIU clock
    //
    MCUCTRL->TPIUCTRL &= ~MCUCTRL_TPIUCTRL_ENABLE_Msk;
}

//*****************************************************************************
//
//! @brief Set the output port width of the TPIU
//!
//! @param ui32PortWidth - The desired port width (in bits)
//!
//! This function uses the TPIU_CSPSR register to set the desired output port
//! width of the TPIU.
//!
//! @return None.
//
//*****************************************************************************
void
am_hal_tpiu_port_width_set(uint32_t ui32PortWidth)
{
    TPI->CSPSR = 1 << (ui32PortWidth - 1);
}

//*****************************************************************************
//
//! @brief Read the supported_output port width of the TPIU
//!
//! This function uses the \e TPIU_SSPSR register to set the supported output
//! port widths of the TPIU.
//!
//! @return Current width of the TPIU output port
//
//*****************************************************************************
uint32_t
am_hal_tpiu_supported_port_width_get(void)
{
    uint32_t i, ui32WidthValue;

    //
    // Read the supported width register.
    //
    ui32WidthValue = TPI->SSPSR;

    //
    // The register value is encoded in a one-hot format, so the position of
    // the single set bit determines the actual width of the port.
    //
    for (i = 1; i < 32; i++)
    {
        //
        // Check each bit for a '1'. When we find it, our current loop index
        // will be equal to the port width.
        //
        if (ui32WidthValue == (0x1 << (i - 1)))
        {
            return i;
        }
    }

    //
    // We should never get here, but if we do, just return the smallest
    // possible value for a supported trace port width.
    //
    return 1;
}

//*****************************************************************************
//
//! @brief Read the output port width of the TPIU
//!
//! This function uses the \e TPIU_CSPSR register to set the desired output
//! port width of the TPIU.
//!
//! @return Current width of the TPIU output port
//
//*****************************************************************************
uint32_t
am_hal_tpiu_port_width_get(void)
{
    uint32_t ui32Temp;
    uint32_t ui32Width;

    ui32Width = 1;
    ui32Temp = TPI->CSPSR;

    while ( !(ui32Temp & 1) )
    {
        ui32Temp = ui32Temp >> 1;
        ui32Width++;

        if (ui32Width > 32)
        {
            ui32Width = 0;
            break;
        }
    }

    //
    // Current width of the TPIU output port.
    //
    return ui32Width;
}

//*****************************************************************************
//
//! @brief Configure the TPIU based on the values in the configuration struct.
//!
//! @param psConfig - pointer to an am_hal_tpiu_config_t structure containing
//! the desired configuration information.
//!
//! This function reads the provided configuration structure, and sets the
//! relevant TPIU registers to achieve the desired configuration.
//!
//! @return None.
//
//*****************************************************************************
void
am_hal_tpiu_configure(am_hal_tpiu_config_t *psConfig)
{
    //
    // Set the clock freq in the MCUCTRL register.
    //
    MCUCTRL->TPIUCTRL |= psConfig->ui32TraceClkIn;

    //
    // Set the desired protocol.
    //
    TPI->SPPR = psConfig->ui32PinProtocol;

    //
    // Set the parallel port width. This may be redundant if the user has
    // selected a serial protocol, but we'll set it anyway.
    //
    TPI->CSPSR = (1 << (psConfig->ui32ParallelPortSize - 1));

    //
    // Set the clock prescaler.
    //
    TPI->ACPR = psConfig->ui32ClockPrescaler;
}

//*****************************************************************************
//
//! @brief Enables the TPIU
//!
//! This function enables the ARM TPIU by setting the TPIU registers and then
//! enabling the TPIU clock source in MCU control register.
//!
//! @param psConfig - structure for configuration.
//!     If ui32SetItmBaud, the other structure members are used to set the
//!      TPIU configuration.
//!     But for simplicity, ui32SetItmBaud can be set to one of the
//!      following, in which case all other structure members are ignored.
//!      In this case, the given BAUD rate is based on a div-by-8 HFRC clock.
//!         AM_HAL_TPIU_BAUD_57600
//!         AM_HAL_TPIU_BAUD_115200
//!         AM_HAL_TPIU_BAUD_230400
//!         AM_HAL_TPIU_BAUD_460800
//!         AM_HAL_TPIU_BAUD_500000
//!         AM_HAL_TPIU_BAUD_1M
//!
//! @return None.
//
//*****************************************************************************
void
am_hal_tpiu_enable(am_hal_tpiu_config_t *psConfig)
{
    am_hal_clkgen_status_t sClkGenStatus;
    uint32_t ui32HFRC, ui32SWOscaler, ui32ITMbitrate;

    ui32ITMbitrate = psConfig->ui32SetItmBaud;

    //
    // TPIU formatter & flush control register.
    //
    TPI->FFCR = 0;

    if ( ui32ITMbitrate )
    {
        //
        // Set the Current Parallel Port Size (note - only 1 bit can be set).
        //
        TPI->CSPSR = TPI_CSPSR_CWIDTH_1BIT;

        //
        // Use some default assumptions to set the ITM frequency.
        //
        if ( (ui32ITMbitrate < AM_HAL_TPIU_BAUD_57600 )  ||
             (ui32ITMbitrate > AM_HAL_TPIU_BAUD_2M ) )
        {
            ui32ITMbitrate = AM_HAL_TPIU_BAUD_DEFAULT;
        }

        //
        // Get the current HFRC frequency.
        //
        am_hal_clkgen_status_get(&sClkGenStatus);
        ui32HFRC = sClkGenStatus.ui32SysclkFreq;

        //
        // Compute the SWO scaler value.
        //
        if ( ui32HFRC != 0xFFFFFFFF )
        {
            ui32SWOscaler = ((ui32HFRC / 8) / ui32ITMbitrate) - 1;
        }
        else
        {
            ui32SWOscaler = ( (AM_HAL_CLKGEN_FREQ_MAX_HZ / 8) /
                              AM_HAL_TPIU_BAUD_DEFAULT ) - 1;
        }

        //
        // Set the scaler value.
        //
        TPI->ACPR = _VAL2FLD(TPI_ACPR_SWOSCALER, ui32SWOscaler);

        //
        // Set for UART mode
        //
        TPI->SPPR = _VAL2FLD( TPI_SPPR_TXMODE, TPI_SPPR_TXMODE_UART);

        //
        // Make sure we are not in test mode (important for proper deep sleep
        // operation).
        //
        TPI->ITCTRL = _VAL2FLD(TPI_ITCTRL_Mode, TPI_ITCTRL_Mode_NORMAL);

        //
        // Enable the TPIU clock source in MCU control.
        // Set TPIU clock for HFRC/8 (6MHz) operation.
        //
        MCUCTRL->TPIUCTRL =
            _VAL2FLD(MCUCTRL_TPIUCTRL_CLKSEL, MCUCTRL_TPIUCTRL_CLKSEL_HFRCDIV8) |
            _VAL2FLD(MCUCTRL_TPIUCTRL_ENABLE, MCUCTRL_TPIUCTRL_ENABLE_EN);
    }
    else
    {
        //
        // Set the configuration according to the structure values.
        //

        //
        // Set the Asynchronous Clock Prescaler Register.
        //
        TPI->ACPR = psConfig->ui32ClockPrescaler;

        //
        // Set the Selected Pin Protocol Register.
        //  e.g. AM_REG_TPIU_SPPR_TXMODE_UART
        //
        TPI->SPPR = psConfig->ui32PinProtocol;

        //
        // Set the Current Parallel Port Size (note - only 1 bit can be set).
        // This may be redundant if the user has selected a serial protocol,
        // but we'll set it anyway.
        //
        TPI->CSPSR = (1 << (psConfig->ui32ParallelPortSize - 1));

        //
        // Make sure we are not in test mode (important for proper deep sleep
        // operation).
        //
        TPI->ITCTRL = _VAL2FLD(TPI_ITCTRL_Mode, TPI_ITCTRL_Mode_NORMAL);

        //
        // Set the clock freq and enable fields in the MCUCTRL register.
        //
        MCUCTRL->TPIUCTRL = psConfig->ui32TraceClkIn;
    }

    //
    // Wait for 50us for the data to flush out.
    //
    am_hal_flash_delay(FLASH_CYCLES_US(50));
}

//*****************************************************************************
//
//! @brief Disables the TPIU
//!
//! This function disables the ARM TPIU by disabling the TPIU clock source
//! in MCU control register.
//!
//! @return None.
//
//*****************************************************************************
void
am_hal_tpiu_disable(void)
{
    //
    // Disable the TPIU clock source in MCU control.
    //
    MCUCTRL->TPIUCTRL =
        _VAL2FLD(MCUCTRL_TPIUCTRL_CLKSEL, MCUCTRL_TPIUCTRL_CLKSEL_LOWPWR) |
        _VAL2FLD(MCUCTRL_TPIUCTRL_ENABLE, MCUCTRL_TPIUCTRL_ENABLE_DIS);
}

//*****************************************************************************
//
// End Doxygen group.
//! @}
//
//*****************************************************************************
