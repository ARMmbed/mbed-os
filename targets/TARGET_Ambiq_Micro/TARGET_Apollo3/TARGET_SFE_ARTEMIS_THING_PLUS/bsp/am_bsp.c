//*****************************************************************************
//
//  am_bsp.c
//! @file
//!
//! @brief Top level functions for performing board initialization.
//!
//! @addtogroup BSP Board Support Package (BSP)
//! @addtogroup apollo3_eb_bsp BSP for the Apollo3 Engineering Board
//! @ingroup BSP
//! @{
//
//*****************************************************************************

//*****************************************************************************
//
// Copyright (c) 2019, Ambiq Micro
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
// This is part of revision v2.0.0 of the AmbiqSuite Development Package.
//
//*****************************************************************************

#include "am_bsp.h"
#include "am_util.h"

//*****************************************************************************
//
// Power tracking structures for IOM and UART
//
//*****************************************************************************
am_bsp_uart_pwrsave_t am_bsp_uart_pwrsave[AM_REG_UART_NUM_MODULES];

//*****************************************************************************
//
// LEDs
//
//*****************************************************************************
#ifdef AM_BSP_NUM_LEDS
am_devices_led_t am_bsp_psLEDs[AM_BSP_NUM_LEDS] =
{
    {AM_BSP_GPIO_LED0, AM_DEVICES_LED_ON_HIGH | AM_DEVICES_LED_POL_DIRECT_DRIVE_M},
};
#endif

#ifdef AM_BSP_NUM_BUTTONS
//*****************************************************************************
//
// Buttons.
//
//*****************************************************************************
am_devices_button_t am_bsp_psButtons[AM_BSP_NUM_BUTTONS] =
{
    AM_DEVICES_BUTTON(AM_BSP_GPIO_BUTTON0, AM_DEVICES_BUTTON_NORMAL_HIGH)
};
#endif

//*****************************************************************************
//
// Print interface tracking variable.
//
//*****************************************************************************
static uint32_t g_ui32PrintInterface = AM_BSP_PRINT_INFC_UART0;

//*****************************************************************************
//
// Default UART configuration settings.
//
//*****************************************************************************
static void *g_sCOMUART;

static const am_hal_uart_config_t g_sBspUartConfig =
{
    //
    // Standard UART settings: 115200-8-N-1
    //
    .ui32BaudRate = 115200,
    .ui32DataBits = AM_HAL_UART_DATA_BITS_8,
    .ui32Parity = AM_HAL_UART_PARITY_NONE,
    .ui32StopBits = AM_HAL_UART_ONE_STOP_BIT,
    .ui32FlowControl = AM_HAL_UART_FLOW_CTRL_NONE,

    //
    // Set TX and RX FIFOs to interrupt at half-full.
    //
    .ui32FifoLevels = (AM_HAL_UART_TX_FIFO_1_2 |
                       AM_HAL_UART_RX_FIFO_1_2),

    //
    // The default interface will just use polling instead of buffers.
    //
    .pui8TxBuffer = 0,
    .ui32TxBufferSize = 0,
    .pui8RxBuffer = 0,
    .ui32RxBufferSize = 0,
};

#ifndef AM_BSP_DISABLE_BUFFERED_UART
//*****************************************************************************
//
// Default UART configuration settings if using buffers.
//
//*****************************************************************************
#define AM_BSP_UART_BUFFER_SIZE     1024
static uint8_t pui8UartTxBuffer[AM_BSP_UART_BUFFER_SIZE];
static uint8_t pui8UartRxBuffer[AM_BSP_UART_BUFFER_SIZE];

static am_hal_uart_config_t g_sBspUartBufferedConfig =
{
    //
    // Standard UART settings: 115200-8-N-1
    //
    .ui32BaudRate = 115200,
    .ui32DataBits = AM_HAL_UART_DATA_BITS_8,
    .ui32Parity = AM_HAL_UART_PARITY_NONE,
    .ui32StopBits = AM_HAL_UART_ONE_STOP_BIT,
    .ui32FlowControl = AM_HAL_UART_FLOW_CTRL_NONE,

    //
    // Set TX and RX FIFOs to interrupt at half-full.
    //
    .ui32FifoLevels = (AM_HAL_UART_TX_FIFO_1_2 |
                       AM_HAL_UART_RX_FIFO_1_2),

    //
    // The default interface will just use polling instead of buffers.
    //
    .pui8TxBuffer = pui8UartTxBuffer,
    .ui32TxBufferSize = sizeof(pui8UartTxBuffer),
    .pui8RxBuffer = pui8UartRxBuffer,
    .ui32RxBufferSize = sizeof(pui8UartRxBuffer),
};
#endif // AM_BSP_DISABLE_BUFFERED_UART

//*****************************************************************************
//
//! @brief Prepare the MCU for low power operation.
//!
//! This function enables several power-saving features of the MCU, and
//! disables some of the less-frequently used peripherals. It also sets the
//! system clock to 24 MHz.
//!
//! @return None.
//
//*****************************************************************************
void
am_bsp_low_power_init(void)
{
    //
    // Initialize for low power in the power control block
    //
    am_hal_pwrctrl_low_power_init();

    //
    // Disable the RTC.
    //
    am_hal_rtc_osc_disable();

    //
    // Stop the XTAL.
    //
    am_hal_clkgen_control(AM_HAL_CLKGEN_CONTROL_XTAL_STOP, 0);

    //
    // Make sure SWO/ITM/TPIU is disabled.
    // SBL may not get it completely shut down.
    //
    am_bsp_itm_printf_disable();

#ifdef AM_BSP_NUM_LEDS
    //
    // Initialize the LEDs.
    // On the apollo3_evb, when the GPIO outputs are disabled (the default at
    // power up), the FET gates are floating and partially illuminating the LEDs.
    //
    uint32_t ux, ui32GPIONumber;
    for (ux = 0; ux < AM_BSP_NUM_LEDS; ux++)
    {
        ui32GPIONumber = am_bsp_psLEDs[ux].ui32GPIONumber;

        //
        // Configure the pin as a push-pull GPIO output
        // (aka AM_DEVICES_LED_POL_DIRECT_DRIVE_M).
        //
        am_hal_gpio_pinconfig(ui32GPIONumber, g_AM_HAL_GPIO_OUTPUT);

        //
        // Turn off the LED.
        //
        am_hal_gpio_state_write(ui32GPIONumber, AM_HAL_GPIO_OUTPUT_TRISTATE_DISABLE);
        am_hal_gpio_state_write(ui32GPIONumber, AM_HAL_GPIO_OUTPUT_CLEAR);
    }
#endif // AM_BSP_NUM_LEDS

} // am_bsp_low_power_init()

//*****************************************************************************
//
//! @brief Enable the TPIU and ITM for debug printf messages.
//!
//! This function enables TPIU registers for debug printf messages and enables
//! ITM GPIO pin to SWO mode. This function should be called after reset and
//! after waking up from deep sleep.
//!
//! @return None.
//
//*****************************************************************************
void
am_bsp_debug_printf_enable(void)
{
    if (g_ui32PrintInterface == AM_BSP_PRINT_INFC_SWO)
    {
#ifdef AM_BSP_GPIO_ITM_SWO
        am_bsp_itm_printf_enable();
#endif
    }
    else if (g_ui32PrintInterface == AM_BSP_PRINT_INFC_UART0)
    {
        am_bsp_uart_printf_enable();
    }
#ifndef AM_BSP_DISABLE_BUFFERED_UART
    else if (g_ui32PrintInterface == AM_BSP_PRINT_INFC_BUFFERED_UART0)
    {
        am_bsp_buffered_uart_printf_enable();
    }
#endif // AM_BSP_DISABLE_BUFFERED_UART
} // am_bsp_debug_printf_enable()

//*****************************************************************************
//
//! @brief Enable the TPIU and ITM for debug printf messages.
//!
//! This function disables TPIU registers for debug printf messages and
//! enables ITM GPIO pin to GPIO mode and prepares the MCU to go to deep sleep.
//!
//! @return None.
//
//*****************************************************************************
void
am_bsp_debug_printf_disable(void)
{
    if (g_ui32PrintInterface == AM_BSP_PRINT_INFC_SWO)
    {
        am_bsp_itm_printf_disable();
    }
    else if (g_ui32PrintInterface == AM_BSP_PRINT_INFC_UART0)
    {
        am_bsp_uart_printf_disable();
    }
} // am_bsp_debug_printf_disable()

//*****************************************************************************
//
// @brief Enable printing over ITM.
//
//*****************************************************************************
void
#ifdef AM_BSP_GPIO_ITM_SWO
am_bsp_itm_printf_enable(void)
#else
am_bsp_itm_printf_enable(uint32_t ui32Pin, am_hal_gpio_pincfg_t sPincfg)
#endif
{
    am_hal_tpiu_config_t TPIUcfg;

    //
    // Set the global print interface.
    //
    g_ui32PrintInterface = AM_BSP_PRINT_INFC_SWO;

    //
    // Enable the ITM interface and the SWO pin.
    //
    am_hal_itm_enable();

    //
    // Enable the ITM and TPIU
    // Set the BAUD clock for 1M
    //
    TPIUcfg.ui32SetItmBaud = AM_HAL_TPIU_BAUD_2M;
    am_hal_tpiu_enable(&TPIUcfg);
    #ifdef AM_BSP_GPIO_ITM_SWO
    am_hal_gpio_pinconfig(AM_BSP_GPIO_ITM_SWO, g_AM_BSP_GPIO_ITM_SWO);
    #else
    am_hal_gpio_pinconfig(ui32Pin, sPincfg);
    #endif

    //
    // Attach the ITM to the STDIO driver.
    //
    am_util_stdio_printf_init(am_hal_itm_print);
} // am_bsp_itm_printf_enable()

//*****************************************************************************
//
//! @brief ITM-based string print function.
//!
//! This function is used for printing a string via the ITM.
//!
//! @return None.
//
//*****************************************************************************
void
am_bsp_itm_string_print(char *pcString)
{
    am_hal_itm_print(pcString);
}

//*****************************************************************************
//
// @brief Disable printing over ITM.
//
//*****************************************************************************
void
am_bsp_itm_printf_disable(void)
{
    //
    // Disable the ITM/TPIU
    //
    am_hal_itm_disable();

    //
    // Detach the ITM interface from the STDIO driver.
    //
    am_util_stdio_printf_init(0);

    // //
    // // Disconnect the SWO pin
    // //
    // am_hal_gpio_pinconfig(AM_BSP_GPIO_ITM_SWO, g_AM_HAL_GPIO_DISABLE);
} // am_bsp_itm_printf_disable()

//*****************************************************************************
//
//! @brief Set up the IOM pins based on mode and module.
//!
//! This function configures up to 10-pins for MSPI serial, dual, quad,
//! dual-quad, and octal operation.
//!
//! @return None.
//
//*****************************************************************************
void
am_bsp_iom_pins_enable(uint32_t ui32Module, am_hal_iom_mode_e eIOMMode)
{
    uint32_t ui32Combined;

    //
    // Validate parameters
    //
    if ( ui32Module >= AM_REG_IOM_NUM_MODULES )
    {
        //
        // FPGA supports only IOM0 and 1.
        //
        return;
    }

    ui32Combined = ((ui32Module << 2) | eIOMMode);

    switch ( ui32Combined )
    {
        case ((0 << 2) | AM_HAL_IOM_SPI_MODE):
            am_hal_gpio_pinconfig(AM_BSP_GPIO_IOM0_SCK,  g_AM_BSP_GPIO_IOM0_SCK);
            am_hal_gpio_pinconfig(AM_BSP_GPIO_IOM0_MISO, g_AM_BSP_GPIO_IOM0_MISO);
            am_hal_gpio_pinconfig(AM_BSP_GPIO_IOM0_MOSI, g_AM_BSP_GPIO_IOM0_MOSI);
            am_hal_gpio_pinconfig(AM_BSP_GPIO_IOM0_CS,   g_AM_BSP_GPIO_IOM0_CS);
            break;

        case ((1 << 2) | AM_HAL_IOM_SPI_MODE):
            am_hal_gpio_pinconfig(AM_BSP_GPIO_IOM1_SCK,  g_AM_BSP_GPIO_IOM1_SCK);
            am_hal_gpio_pinconfig(AM_BSP_GPIO_IOM1_MISO, g_AM_BSP_GPIO_IOM1_MISO);
            am_hal_gpio_pinconfig(AM_BSP_GPIO_IOM1_MOSI, g_AM_BSP_GPIO_IOM1_MOSI);
            am_hal_gpio_pinconfig(AM_BSP_GPIO_IOM1_CS,   g_AM_BSP_GPIO_IOM1_CS);
            break;

        case ((2 << 2) | AM_HAL_IOM_SPI_MODE):
            am_hal_gpio_pinconfig(AM_BSP_GPIO_IOM2_SCK,  g_AM_BSP_GPIO_IOM2_SCK);
            am_hal_gpio_pinconfig(AM_BSP_GPIO_IOM2_MISO, g_AM_BSP_GPIO_IOM2_MISO);
            am_hal_gpio_pinconfig(AM_BSP_GPIO_IOM2_MOSI, g_AM_BSP_GPIO_IOM2_MOSI);
            am_hal_gpio_pinconfig(AM_BSP_GPIO_IOM2_CS,   g_AM_BSP_GPIO_IOM2_CS);
            break;

        case ((3 << 2) | AM_HAL_IOM_SPI_MODE):
            am_hal_gpio_pinconfig(AM_BSP_GPIO_IOM3_SCK,  g_AM_BSP_GPIO_IOM3_SCK);
            am_hal_gpio_pinconfig(AM_BSP_GPIO_IOM3_MISO, g_AM_BSP_GPIO_IOM3_MISO);
            am_hal_gpio_pinconfig(AM_BSP_GPIO_IOM3_MOSI, g_AM_BSP_GPIO_IOM3_MOSI);
            am_hal_gpio_pinconfig(AM_BSP_GPIO_IOM3_CS,   g_AM_BSP_GPIO_IOM3_CS);
            break;

        case ((4 << 2) | AM_HAL_IOM_SPI_MODE):
            am_hal_gpio_pinconfig(AM_BSP_GPIO_IOM4_SCK,  g_AM_BSP_GPIO_IOM4_SCK);
            am_hal_gpio_pinconfig(AM_BSP_GPIO_IOM4_MISO, g_AM_BSP_GPIO_IOM4_MISO);
            am_hal_gpio_pinconfig(AM_BSP_GPIO_IOM4_MOSI, g_AM_BSP_GPIO_IOM4_MOSI);
            am_hal_gpio_pinconfig(AM_BSP_GPIO_IOM4_CS,   g_AM_BSP_GPIO_IOM4_CS);
            break;

        case ((5 << 2) | AM_HAL_IOM_SPI_MODE):
            am_hal_gpio_pinconfig(AM_BSP_GPIO_IOM5_SCK,  g_AM_BSP_GPIO_IOM5_SCK);
            am_hal_gpio_pinconfig(AM_BSP_GPIO_IOM5_MISO, g_AM_BSP_GPIO_IOM5_MISO);
            am_hal_gpio_pinconfig(AM_BSP_GPIO_IOM5_MOSI, g_AM_BSP_GPIO_IOM5_MOSI);
            am_hal_gpio_pinconfig(AM_BSP_GPIO_IOM5_CS,   g_AM_BSP_GPIO_IOM5_CS);
            break;

        case ((0 << 2) | AM_HAL_IOM_I2C_MODE):
            am_hal_gpio_pinconfig(AM_BSP_GPIO_IOM0_SCL,  g_AM_BSP_GPIO_IOM0_SCL);
            am_hal_gpio_pinconfig(AM_BSP_GPIO_IOM0_SDA,  g_AM_BSP_GPIO_IOM0_SDA);
            break;

        case ((1 << 2) | AM_HAL_IOM_I2C_MODE):
            am_hal_gpio_pinconfig(AM_BSP_GPIO_IOM1_SCL,  g_AM_BSP_GPIO_IOM1_SCL);
            am_hal_gpio_pinconfig(AM_BSP_GPIO_IOM1_SDA,  g_AM_BSP_GPIO_IOM1_SDA);
            break;

        case ((2 << 2) | AM_HAL_IOM_I2C_MODE):
            am_hal_gpio_pinconfig(AM_BSP_GPIO_IOM2_SCL,  g_AM_BSP_GPIO_IOM2_SCL);
            am_hal_gpio_pinconfig(AM_BSP_GPIO_IOM2_SDA,  g_AM_BSP_GPIO_IOM2_SDA);
            break;

        case ((3 << 2) | AM_HAL_IOM_I2C_MODE):
            am_hal_gpio_pinconfig(AM_BSP_GPIO_IOM3_SCL,  g_AM_BSP_GPIO_IOM3_SCL);
            am_hal_gpio_pinconfig(AM_BSP_GPIO_IOM3_SDA,  g_AM_BSP_GPIO_IOM3_SDA);
            break;

        case ((4 << 2) | AM_HAL_IOM_I2C_MODE):
            am_hal_gpio_pinconfig(AM_BSP_GPIO_IOM4_SCL,  g_AM_BSP_GPIO_IOM4_SCL);
            am_hal_gpio_pinconfig(AM_BSP_GPIO_IOM4_SDA,  g_AM_BSP_GPIO_IOM4_SDA);
            break;

        case ((5 << 2) | AM_HAL_IOM_I2C_MODE):
            am_hal_gpio_pinconfig(AM_BSP_GPIO_IOM5_SCL,  g_AM_BSP_GPIO_IOM5_SCL);
            am_hal_gpio_pinconfig(AM_BSP_GPIO_IOM5_SDA,  g_AM_BSP_GPIO_IOM5_SDA);
            break;

        default:
            break;
    }
} // am_bsp_iom_pins_enable()

//*****************************************************************************
//
//! @brief Disable the IOM pins based on mode and module.
//!
//! @return None.
//
//*****************************************************************************
void
am_bsp_iom_pins_disable(uint32_t ui32Module, am_hal_iom_mode_e eIOMMode)
{
    uint32_t ui32Combined;

    //
    // Validate parameters
    //
    if ( ui32Module >= AM_REG_IOM_NUM_MODULES )
    {
        //
        // FPGA supports only IOM0 and 1.
        //
        return;
    }

    ui32Combined = ((ui32Module << 2) | eIOMMode);

    switch ( ui32Combined )
    {
        case ((0 << 2) | AM_HAL_IOM_SPI_MODE):
            am_hal_gpio_pinconfig(AM_BSP_GPIO_IOM0_SCK,  g_AM_HAL_GPIO_DISABLE);
            am_hal_gpio_pinconfig(AM_BSP_GPIO_IOM0_MISO, g_AM_HAL_GPIO_DISABLE);
            am_hal_gpio_pinconfig(AM_BSP_GPIO_IOM0_MOSI, g_AM_HAL_GPIO_DISABLE);
            am_hal_gpio_pinconfig(AM_BSP_GPIO_IOM0_CS,   g_AM_HAL_GPIO_DISABLE);
            break;

        case ((1 << 2) | AM_HAL_IOM_SPI_MODE):
            am_hal_gpio_pinconfig(AM_BSP_GPIO_IOM1_SCK,  g_AM_HAL_GPIO_DISABLE);
            am_hal_gpio_pinconfig(AM_BSP_GPIO_IOM1_MISO, g_AM_HAL_GPIO_DISABLE);
            am_hal_gpio_pinconfig(AM_BSP_GPIO_IOM1_MOSI, g_AM_HAL_GPIO_DISABLE);
            am_hal_gpio_pinconfig(AM_BSP_GPIO_IOM1_CS,   g_AM_HAL_GPIO_DISABLE);
            break;

        case ((2 << 2) | AM_HAL_IOM_SPI_MODE):
            am_hal_gpio_pinconfig(AM_BSP_GPIO_IOM2_SCK,  g_AM_HAL_GPIO_DISABLE);
            am_hal_gpio_pinconfig(AM_BSP_GPIO_IOM2_MISO, g_AM_HAL_GPIO_DISABLE);
            am_hal_gpio_pinconfig(AM_BSP_GPIO_IOM2_MOSI, g_AM_HAL_GPIO_DISABLE);
            am_hal_gpio_pinconfig(AM_BSP_GPIO_IOM2_CS,   g_AM_HAL_GPIO_DISABLE);
            break;

        case ((3 << 2) | AM_HAL_IOM_SPI_MODE):
            am_hal_gpio_pinconfig(AM_BSP_GPIO_IOM3_SCK,  g_AM_HAL_GPIO_DISABLE);
            am_hal_gpio_pinconfig(AM_BSP_GPIO_IOM3_MISO, g_AM_HAL_GPIO_DISABLE);
            am_hal_gpio_pinconfig(AM_BSP_GPIO_IOM3_MOSI, g_AM_HAL_GPIO_DISABLE);
            am_hal_gpio_pinconfig(AM_BSP_GPIO_IOM3_CS,   g_AM_HAL_GPIO_DISABLE);
            break;

        case ((4 << 2) | AM_HAL_IOM_SPI_MODE):
            am_hal_gpio_pinconfig(AM_BSP_GPIO_IOM4_SCK,  g_AM_HAL_GPIO_DISABLE);
            am_hal_gpio_pinconfig(AM_BSP_GPIO_IOM4_MISO, g_AM_HAL_GPIO_DISABLE);
            am_hal_gpio_pinconfig(AM_BSP_GPIO_IOM4_MOSI, g_AM_HAL_GPIO_DISABLE);
            am_hal_gpio_pinconfig(AM_BSP_GPIO_IOM4_CS,   g_AM_HAL_GPIO_DISABLE);
            break;

        case ((5 << 2) | AM_HAL_IOM_SPI_MODE):
            am_hal_gpio_pinconfig(AM_BSP_GPIO_IOM5_SCK,  g_AM_HAL_GPIO_DISABLE);
            am_hal_gpio_pinconfig(AM_BSP_GPIO_IOM5_MISO, g_AM_HAL_GPIO_DISABLE);
            am_hal_gpio_pinconfig(AM_BSP_GPIO_IOM5_MOSI, g_AM_HAL_GPIO_DISABLE);
            am_hal_gpio_pinconfig(AM_BSP_GPIO_IOM5_CS,   g_AM_HAL_GPIO_DISABLE);
            break;

        case ((0 << 2) | AM_HAL_IOM_I2C_MODE):
            am_hal_gpio_pinconfig(AM_BSP_GPIO_IOM0_SCL,  g_AM_HAL_GPIO_DISABLE);
            am_hal_gpio_pinconfig(AM_BSP_GPIO_IOM0_SDA,  g_AM_HAL_GPIO_DISABLE);
            break;

        case ((1 << 2) | AM_HAL_IOM_I2C_MODE):
            am_hal_gpio_pinconfig(AM_BSP_GPIO_IOM1_SCL,  g_AM_HAL_GPIO_DISABLE);
            am_hal_gpio_pinconfig(AM_BSP_GPIO_IOM1_SDA,  g_AM_HAL_GPIO_DISABLE);
            break;

        case ((2 << 2) | AM_HAL_IOM_I2C_MODE):
            am_hal_gpio_pinconfig(AM_BSP_GPIO_IOM2_SCL,  g_AM_HAL_GPIO_DISABLE);
            am_hal_gpio_pinconfig(AM_BSP_GPIO_IOM2_SDA,  g_AM_HAL_GPIO_DISABLE);
            break;

        case ((3 << 2) | AM_HAL_IOM_I2C_MODE):
            am_hal_gpio_pinconfig(AM_BSP_GPIO_IOM3_SCL,  g_AM_HAL_GPIO_DISABLE);
            am_hal_gpio_pinconfig(AM_BSP_GPIO_IOM3_SDA,  g_AM_HAL_GPIO_DISABLE);
            break;

        case ((4 << 2) | AM_HAL_IOM_I2C_MODE):
            am_hal_gpio_pinconfig(AM_BSP_GPIO_IOM4_SCL,  g_AM_HAL_GPIO_DISABLE);
            am_hal_gpio_pinconfig(AM_BSP_GPIO_IOM4_SDA,  g_AM_HAL_GPIO_DISABLE);
            break;

        case ((5 << 2) | AM_HAL_IOM_I2C_MODE):
            am_hal_gpio_pinconfig(AM_BSP_GPIO_IOM5_SCL,  g_AM_HAL_GPIO_DISABLE);
            am_hal_gpio_pinconfig(AM_BSP_GPIO_IOM5_SDA,  g_AM_HAL_GPIO_DISABLE);
            break;
        default:
            break;
    }
} // am_bsp_iom_pins_disable()

//*****************************************************************************
//
//! @brief Set up the MSPI pins based on the external flash device type.
//!
//! This function configures up to 10-pins for MSPI serial, dual, quad,
//! dual-quad, and octal operation.
//!
//! @return None.
//
//*****************************************************************************
void
am_bsp_mspi_pins_enable(am_hal_mspi_device_e eMSPIDevice)
{
    switch ( eMSPIDevice )
    {
        case AM_HAL_MSPI_FLASH_SERIAL_CE0:
            am_hal_gpio_pinconfig(AM_BSP_GPIO_MSPI_CE0, g_AM_BSP_GPIO_MSPI_CE0);
            am_hal_gpio_pinconfig(AM_BSP_GPIO_MSPI_D0,  g_AM_BSP_GPIO_MSPI_D0);
            am_hal_gpio_pinconfig(AM_BSP_GPIO_MSPI_D1,  g_AM_BSP_GPIO_MSPI_D1);
            am_hal_gpio_pinconfig(AM_BSP_GPIO_MSPI_SCK, g_AM_BSP_GPIO_MSPI_SCK);
            break;
        case AM_HAL_MSPI_FLASH_SERIAL_CE1:
            am_hal_gpio_pinconfig(AM_BSP_GPIO_MSPI_CE1, g_AM_BSP_GPIO_MSPI_CE1);
            am_hal_gpio_pinconfig(AM_BSP_GPIO_MSPI_D4,  g_AM_BSP_GPIO_MSPI_D4);
            am_hal_gpio_pinconfig(AM_BSP_GPIO_MSPI_D5,  g_AM_BSP_GPIO_MSPI_D5);
            am_hal_gpio_pinconfig(AM_BSP_GPIO_MSPI_SCK, g_AM_BSP_GPIO_MSPI_SCK);
            break;
        case AM_HAL_MSPI_FLASH_DUAL_CE0:
            am_hal_gpio_pinconfig(AM_BSP_GPIO_MSPI_CE0, g_AM_BSP_GPIO_MSPI_CE0);
            am_hal_gpio_pinconfig(AM_BSP_GPIO_MSPI_D0,  g_AM_BSP_GPIO_MSPI_D0);
            am_hal_gpio_pinconfig(AM_BSP_GPIO_MSPI_D1,  g_AM_BSP_GPIO_MSPI_D1);
            am_hal_gpio_pinconfig(AM_BSP_GPIO_MSPI_SCK, g_AM_BSP_GPIO_MSPI_SCK);
            break;
        case AM_HAL_MSPI_FLASH_DUAL_CE1:
            am_hal_gpio_pinconfig(AM_BSP_GPIO_MSPI_CE1, g_AM_BSP_GPIO_MSPI_CE1);
            am_hal_gpio_pinconfig(AM_BSP_GPIO_MSPI_D4,  g_AM_BSP_GPIO_MSPI_D4);
            am_hal_gpio_pinconfig(AM_BSP_GPIO_MSPI_D5,  g_AM_BSP_GPIO_MSPI_D5);
            am_hal_gpio_pinconfig(AM_BSP_GPIO_MSPI_SCK, g_AM_BSP_GPIO_MSPI_SCK);
            break;
        case AM_HAL_MSPI_FLASH_QUAD_CE0:
            am_hal_gpio_pinconfig(AM_BSP_GPIO_MSPI_CE0, g_AM_BSP_GPIO_MSPI_CE0);
            am_hal_gpio_pinconfig(AM_BSP_GPIO_MSPI_D0,  g_AM_BSP_GPIO_MSPI_D0);
            am_hal_gpio_pinconfig(AM_BSP_GPIO_MSPI_D1,  g_AM_BSP_GPIO_MSPI_D1);
            am_hal_gpio_pinconfig(AM_BSP_GPIO_MSPI_D2,  g_AM_BSP_GPIO_MSPI_D2);
            am_hal_gpio_pinconfig(AM_BSP_GPIO_MSPI_D3,  g_AM_BSP_GPIO_MSPI_D3);
            am_hal_gpio_pinconfig(AM_BSP_GPIO_MSPI_SCK, g_AM_BSP_GPIO_MSPI_SCK);
            break;
        case AM_HAL_MSPI_FLASH_QUAD_CE1:
            am_hal_gpio_pinconfig(AM_BSP_GPIO_MSPI_CE1, g_AM_BSP_GPIO_MSPI_CE1);
            am_hal_gpio_pinconfig(AM_BSP_GPIO_MSPI_D4,  g_AM_BSP_GPIO_MSPI_D4);
            am_hal_gpio_pinconfig(AM_BSP_GPIO_MSPI_D5,  g_AM_BSP_GPIO_MSPI_D5);
            am_hal_gpio_pinconfig(AM_BSP_GPIO_MSPI_D6,  g_AM_BSP_GPIO_MSPI_D6);
            am_hal_gpio_pinconfig(AM_BSP_GPIO_MSPI_D7,  g_AM_BSP_GPIO_MSPI_D7);
            am_hal_gpio_pinconfig(AM_BSP_GPIO_MSPI_SCK, g_AM_BSP_GPIO_MSPI_SCK);
            break;
        case AM_HAL_MSPI_FLASH_OCTAL_CE0:
            am_hal_gpio_pinconfig(AM_BSP_GPIO_MSPI_CE0, g_AM_BSP_GPIO_MSPI_CE0);
            am_hal_gpio_pinconfig(AM_BSP_GPIO_MSPI_D0,  g_AM_BSP_GPIO_MSPI_D0);
            am_hal_gpio_pinconfig(AM_BSP_GPIO_MSPI_D1,  g_AM_BSP_GPIO_MSPI_D1);
            am_hal_gpio_pinconfig(AM_BSP_GPIO_MSPI_D2,  g_AM_BSP_GPIO_MSPI_D2);
            am_hal_gpio_pinconfig(AM_BSP_GPIO_MSPI_D3,  g_AM_BSP_GPIO_MSPI_D3);
            am_hal_gpio_pinconfig(AM_BSP_GPIO_MSPI_D4,  g_AM_BSP_GPIO_MSPI_D4);
            am_hal_gpio_pinconfig(AM_BSP_GPIO_MSPI_D5,  g_AM_BSP_GPIO_MSPI_D5);
            am_hal_gpio_pinconfig(AM_BSP_GPIO_MSPI_D6,  g_AM_BSP_GPIO_MSPI_D6);
            am_hal_gpio_pinconfig(AM_BSP_GPIO_MSPI_D7,  g_AM_BSP_GPIO_MSPI_D7);
            am_hal_gpio_pinconfig(AM_BSP_GPIO_MSPI_SCK, g_AM_BSP_GPIO_MSPI_SCK);
            break;
        case AM_HAL_MSPI_FLASH_OCTAL_CE1:
            am_hal_gpio_pinconfig(AM_BSP_GPIO_MSPI_CE1, g_AM_BSP_GPIO_MSPI_CE1);
            am_hal_gpio_pinconfig(AM_BSP_GPIO_MSPI_D0,  g_AM_BSP_GPIO_MSPI_D0);
            am_hal_gpio_pinconfig(AM_BSP_GPIO_MSPI_D1,  g_AM_BSP_GPIO_MSPI_D1);
            am_hal_gpio_pinconfig(AM_BSP_GPIO_MSPI_D2,  g_AM_BSP_GPIO_MSPI_D2);
            am_hal_gpio_pinconfig(AM_BSP_GPIO_MSPI_D3,  g_AM_BSP_GPIO_MSPI_D3);
            am_hal_gpio_pinconfig(AM_BSP_GPIO_MSPI_D4,  g_AM_BSP_GPIO_MSPI_D4);
            am_hal_gpio_pinconfig(AM_BSP_GPIO_MSPI_D5,  g_AM_BSP_GPIO_MSPI_D5);
            am_hal_gpio_pinconfig(AM_BSP_GPIO_MSPI_D6,  g_AM_BSP_GPIO_MSPI_D6);
            am_hal_gpio_pinconfig(AM_BSP_GPIO_MSPI_D7,  g_AM_BSP_GPIO_MSPI_D7);
            am_hal_gpio_pinconfig(AM_BSP_GPIO_MSPI_SCK, g_AM_BSP_GPIO_MSPI_SCK);
            break;
        case AM_HAL_MSPI_FLASH_QUADPAIRED:
        case AM_HAL_MSPI_FLASH_QUADPAIRED_SERIAL:
            am_hal_gpio_pinconfig(AM_BSP_GPIO_MSPI_CE0, g_AM_BSP_GPIO_MSPI_CE0);
            am_hal_gpio_pinconfig(AM_BSP_GPIO_MSPI_CE1, g_AM_BSP_GPIO_MSPI_CE1);
            am_hal_gpio_pinconfig(AM_BSP_GPIO_MSPI_D0,  g_AM_BSP_GPIO_MSPI_D0);
            am_hal_gpio_pinconfig(AM_BSP_GPIO_MSPI_D1,  g_AM_BSP_GPIO_MSPI_D1);
            am_hal_gpio_pinconfig(AM_BSP_GPIO_MSPI_D2,  g_AM_BSP_GPIO_MSPI_D2);
            am_hal_gpio_pinconfig(AM_BSP_GPIO_MSPI_D3,  g_AM_BSP_GPIO_MSPI_D3);
            am_hal_gpio_pinconfig(AM_BSP_GPIO_MSPI_D4,  g_AM_BSP_GPIO_MSPI_D4);
            am_hal_gpio_pinconfig(AM_BSP_GPIO_MSPI_D5,  g_AM_BSP_GPIO_MSPI_D5);
            am_hal_gpio_pinconfig(AM_BSP_GPIO_MSPI_D6,  g_AM_BSP_GPIO_MSPI_D6);
            am_hal_gpio_pinconfig(AM_BSP_GPIO_MSPI_D7,  g_AM_BSP_GPIO_MSPI_D7);
            am_hal_gpio_pinconfig(AM_BSP_GPIO_MSPI_SCK, g_AM_BSP_GPIO_MSPI_SCK);
            break;
    }
} // am_bsp_mspi_pins_enable()

//*****************************************************************************
//
//! @brief Disable the MSPI pins based on the external flash device type.
//!
//! This function configures up to 10-pins for MSPI serial, dual, quad,
//! dual-quad, and octal operation.
//!
//! @return None.
//
//*****************************************************************************
void
am_bsp_mspi_pins_disable(am_hal_mspi_device_e eMSPIDevice)
{
    switch ( eMSPIDevice )
    {
        case AM_HAL_MSPI_FLASH_SERIAL_CE0:
            am_hal_gpio_pinconfig(AM_BSP_GPIO_MSPI_CE0, g_AM_HAL_GPIO_DISABLE);
            am_hal_gpio_pinconfig(AM_BSP_GPIO_MSPI_D0,  g_AM_HAL_GPIO_DISABLE);
            am_hal_gpio_pinconfig(AM_BSP_GPIO_MSPI_D1,  g_AM_HAL_GPIO_DISABLE);
            am_hal_gpio_pinconfig(AM_BSP_GPIO_MSPI_SCK, g_AM_HAL_GPIO_DISABLE);
            break;
        case AM_HAL_MSPI_FLASH_SERIAL_CE1:
            am_hal_gpio_pinconfig(AM_BSP_GPIO_MSPI_CE1, g_AM_HAL_GPIO_DISABLE);
            am_hal_gpio_pinconfig(AM_BSP_GPIO_MSPI_D4,  g_AM_HAL_GPIO_DISABLE);
            am_hal_gpio_pinconfig(AM_BSP_GPIO_MSPI_D5,  g_AM_HAL_GPIO_DISABLE);
            am_hal_gpio_pinconfig(AM_BSP_GPIO_MSPI_SCK, g_AM_HAL_GPIO_DISABLE);
            break;
        case AM_HAL_MSPI_FLASH_DUAL_CE0:
            am_hal_gpio_pinconfig(AM_BSP_GPIO_MSPI_CE0, g_AM_HAL_GPIO_DISABLE);
            am_hal_gpio_pinconfig(AM_BSP_GPIO_MSPI_D0,  g_AM_HAL_GPIO_DISABLE);
            am_hal_gpio_pinconfig(AM_BSP_GPIO_MSPI_D1,  g_AM_HAL_GPIO_DISABLE);
            am_hal_gpio_pinconfig(AM_BSP_GPIO_MSPI_SCK, g_AM_HAL_GPIO_DISABLE);
            break;
        case AM_HAL_MSPI_FLASH_DUAL_CE1:
            am_hal_gpio_pinconfig(AM_BSP_GPIO_MSPI_CE1, g_AM_HAL_GPIO_DISABLE);
            am_hal_gpio_pinconfig(AM_BSP_GPIO_MSPI_D4,  g_AM_HAL_GPIO_DISABLE);
            am_hal_gpio_pinconfig(AM_BSP_GPIO_MSPI_D5,  g_AM_HAL_GPIO_DISABLE);
            am_hal_gpio_pinconfig(AM_BSP_GPIO_MSPI_SCK, g_AM_HAL_GPIO_DISABLE);
            break;
        case AM_HAL_MSPI_FLASH_QUAD_CE0:
            am_hal_gpio_pinconfig(AM_BSP_GPIO_MSPI_CE0, g_AM_HAL_GPIO_DISABLE);
            am_hal_gpio_pinconfig(AM_BSP_GPIO_MSPI_D0,  g_AM_HAL_GPIO_DISABLE);
            am_hal_gpio_pinconfig(AM_BSP_GPIO_MSPI_D1,  g_AM_HAL_GPIO_DISABLE);
            am_hal_gpio_pinconfig(AM_BSP_GPIO_MSPI_D2,  g_AM_HAL_GPIO_DISABLE);
            am_hal_gpio_pinconfig(AM_BSP_GPIO_MSPI_D3,  g_AM_HAL_GPIO_DISABLE);
            am_hal_gpio_pinconfig(AM_BSP_GPIO_MSPI_SCK, g_AM_HAL_GPIO_DISABLE);
            break;
        case AM_HAL_MSPI_FLASH_QUAD_CE1:
            am_hal_gpio_pinconfig(AM_BSP_GPIO_MSPI_CE1, g_AM_HAL_GPIO_DISABLE);
            am_hal_gpio_pinconfig(AM_BSP_GPIO_MSPI_D4,  g_AM_HAL_GPIO_DISABLE);
            am_hal_gpio_pinconfig(AM_BSP_GPIO_MSPI_D5,  g_AM_HAL_GPIO_DISABLE);
            am_hal_gpio_pinconfig(AM_BSP_GPIO_MSPI_D6,  g_AM_HAL_GPIO_DISABLE);
            am_hal_gpio_pinconfig(AM_BSP_GPIO_MSPI_D7,  g_AM_HAL_GPIO_DISABLE);
            am_hal_gpio_pinconfig(AM_BSP_GPIO_MSPI_SCK, g_AM_HAL_GPIO_DISABLE);
            break;
        case AM_HAL_MSPI_FLASH_OCTAL_CE0:
            am_hal_gpio_pinconfig(AM_BSP_GPIO_MSPI_CE0, g_AM_HAL_GPIO_DISABLE);
            am_hal_gpio_pinconfig(AM_BSP_GPIO_MSPI_D0,  g_AM_HAL_GPIO_DISABLE);
            am_hal_gpio_pinconfig(AM_BSP_GPIO_MSPI_D1,  g_AM_HAL_GPIO_DISABLE);
            am_hal_gpio_pinconfig(AM_BSP_GPIO_MSPI_D2,  g_AM_HAL_GPIO_DISABLE);
            am_hal_gpio_pinconfig(AM_BSP_GPIO_MSPI_D3,  g_AM_HAL_GPIO_DISABLE);
            am_hal_gpio_pinconfig(AM_BSP_GPIO_MSPI_D4,  g_AM_HAL_GPIO_DISABLE);
            am_hal_gpio_pinconfig(AM_BSP_GPIO_MSPI_D5,  g_AM_HAL_GPIO_DISABLE);
            am_hal_gpio_pinconfig(AM_BSP_GPIO_MSPI_D6,  g_AM_HAL_GPIO_DISABLE);
            am_hal_gpio_pinconfig(AM_BSP_GPIO_MSPI_D7,  g_AM_HAL_GPIO_DISABLE);
            am_hal_gpio_pinconfig(AM_BSP_GPIO_MSPI_SCK, g_AM_HAL_GPIO_DISABLE);
            break;
        case AM_HAL_MSPI_FLASH_OCTAL_CE1:
            am_hal_gpio_pinconfig(AM_BSP_GPIO_MSPI_CE1, g_AM_HAL_GPIO_DISABLE);
            am_hal_gpio_pinconfig(AM_BSP_GPIO_MSPI_D0,  g_AM_HAL_GPIO_DISABLE);
            am_hal_gpio_pinconfig(AM_BSP_GPIO_MSPI_D1,  g_AM_HAL_GPIO_DISABLE);
            am_hal_gpio_pinconfig(AM_BSP_GPIO_MSPI_D2,  g_AM_HAL_GPIO_DISABLE);
            am_hal_gpio_pinconfig(AM_BSP_GPIO_MSPI_D3,  g_AM_HAL_GPIO_DISABLE);
            am_hal_gpio_pinconfig(AM_BSP_GPIO_MSPI_D4,  g_AM_HAL_GPIO_DISABLE);
            am_hal_gpio_pinconfig(AM_BSP_GPIO_MSPI_D5,  g_AM_HAL_GPIO_DISABLE);
            am_hal_gpio_pinconfig(AM_BSP_GPIO_MSPI_D6,  g_AM_HAL_GPIO_DISABLE);
            am_hal_gpio_pinconfig(AM_BSP_GPIO_MSPI_D7,  g_AM_HAL_GPIO_DISABLE);
            am_hal_gpio_pinconfig(AM_BSP_GPIO_MSPI_SCK, g_AM_HAL_GPIO_DISABLE);
            break;
        case AM_HAL_MSPI_FLASH_QUADPAIRED:
        case AM_HAL_MSPI_FLASH_QUADPAIRED_SERIAL:
            am_hal_gpio_pinconfig(AM_BSP_GPIO_MSPI_CE0, g_AM_HAL_GPIO_DISABLE);
            am_hal_gpio_pinconfig(AM_BSP_GPIO_MSPI_CE1, g_AM_HAL_GPIO_DISABLE);
            am_hal_gpio_pinconfig(AM_BSP_GPIO_MSPI_D0,  g_AM_HAL_GPIO_DISABLE);
            am_hal_gpio_pinconfig(AM_BSP_GPIO_MSPI_D1,  g_AM_HAL_GPIO_DISABLE);
            am_hal_gpio_pinconfig(AM_BSP_GPIO_MSPI_D2,  g_AM_HAL_GPIO_DISABLE);
            am_hal_gpio_pinconfig(AM_BSP_GPIO_MSPI_D3,  g_AM_HAL_GPIO_DISABLE);
            am_hal_gpio_pinconfig(AM_BSP_GPIO_MSPI_D4,  g_AM_HAL_GPIO_DISABLE);
            am_hal_gpio_pinconfig(AM_BSP_GPIO_MSPI_D5,  g_AM_HAL_GPIO_DISABLE);
            am_hal_gpio_pinconfig(AM_BSP_GPIO_MSPI_D6,  g_AM_HAL_GPIO_DISABLE);
            am_hal_gpio_pinconfig(AM_BSP_GPIO_MSPI_D7,  g_AM_HAL_GPIO_DISABLE);
            am_hal_gpio_pinconfig(AM_BSP_GPIO_MSPI_SCK, g_AM_HAL_GPIO_DISABLE);
            break;
    }
} // am_bsp_mspi_pins_disable()

//*****************************************************************************
//
//! @brief Set up the IOS pins based on mode and module.
//!
//! @return None.
//
//*****************************************************************************
void am_bsp_ios_pins_enable(uint32_t ui32Module, uint32_t ui32IOSMode)
{
    uint32_t ui32Combined;

    //
    // Validate parameters
    //
    if ( ui32Module >= AM_REG_IOSLAVE_NUM_MODULES )
    {
        return;
    }

    ui32Combined = ((ui32Module << 2) | ui32IOSMode);

    switch ( ui32Combined )
    {
        case ((0 << 2) | AM_HAL_IOS_USE_SPI):
            am_hal_gpio_pinconfig(AM_BSP_GPIO_IOS_SCK,  g_AM_BSP_GPIO_IOS_SCK);
            am_hal_gpio_pinconfig(AM_BSP_GPIO_IOS_MISO, g_AM_BSP_GPIO_IOS_MISO);
            am_hal_gpio_pinconfig(AM_BSP_GPIO_IOS_MOSI, g_AM_BSP_GPIO_IOS_MOSI);
            am_hal_gpio_pinconfig(AM_BSP_GPIO_IOS_CE,   g_AM_BSP_GPIO_IOS_CE);
            break;

        case ((0 << 2) | AM_HAL_IOS_USE_I2C):
            am_hal_gpio_pinconfig(AM_BSP_GPIO_IOS_SCL,  g_AM_BSP_GPIO_IOS_SCL);
            am_hal_gpio_pinconfig(AM_BSP_GPIO_IOS_SDA,  g_AM_BSP_GPIO_IOS_SDA);
            break;
        default:
            break;
    }
} // am_bsp_ios_pins_enable()

//*****************************************************************************
//
//! @brief Disable the IOS pins based on mode and module.
//!
//! @return None.
//
//*****************************************************************************
void am_bsp_ios_pins_disable(uint32_t ui32Module, uint32_t ui32IOSMode)
{
    uint32_t ui32Combined;

    //
    // Validate parameters
    //
    if ( ui32Module >= AM_REG_IOSLAVE_NUM_MODULES )
    {
        return;
    }

    ui32Combined = ((ui32Module << 2) | ui32IOSMode);

    switch ( ui32Combined )
    {
        case ((0 << 2) | AM_HAL_IOS_USE_SPI):
            am_hal_gpio_pinconfig(AM_BSP_GPIO_IOS_SCK,  g_AM_HAL_GPIO_DISABLE);
            am_hal_gpio_pinconfig(AM_BSP_GPIO_IOS_MISO, g_AM_HAL_GPIO_DISABLE);
            am_hal_gpio_pinconfig(AM_BSP_GPIO_IOS_MOSI, g_AM_HAL_GPIO_DISABLE);
            am_hal_gpio_pinconfig(AM_BSP_GPIO_IOS_CE,   g_AM_HAL_GPIO_DISABLE);
            break;

        case ((0 << 2) | AM_HAL_IOS_USE_I2C):
            am_hal_gpio_pinconfig(AM_BSP_GPIO_IOS_SCL,  g_AM_HAL_GPIO_DISABLE);
            am_hal_gpio_pinconfig(AM_BSP_GPIO_IOS_SDA,  g_AM_HAL_GPIO_DISABLE);
            break;
        default:
            break;
    }
} // am_bsp_ios_pins_disable()

//*****************************************************************************
//
//! @brief UART-based string print function.
//!
//! This function is used for printing a string via the UART, which for some
//! MCU devices may be multi-module.
//!
//! @return None.
//
//*****************************************************************************
void
am_bsp_uart_string_print(char *pcString)
{
    uint32_t ui32StrLen = 0;
    uint32_t ui32BytesWritten = 0;

    //
    // Measure the length of the string.
    //
    while (pcString[ui32StrLen] != 0)
    {
        ui32StrLen++;
    }

    //
    // Print the string via the UART.
    //
    const am_hal_uart_transfer_t sUartWrite =
    {
        .ui32Direction = AM_HAL_UART_WRITE,
        .pui8Data = (uint8_t *) pcString,
        .ui32NumBytes = ui32StrLen,
        .ui32TimeoutMs = AM_HAL_UART_WAIT_FOREVER,
        .pui32BytesTransferred = &ui32BytesWritten,
    };

    am_hal_uart_transfer(g_sCOMUART, &sUartWrite);

    if (ui32BytesWritten != ui32StrLen)
    {
        //
        // Couldn't send the whole string!!
        //
        while(1);
    }
} // am_bsp_uart_string_print()

//*****************************************************************************
//
// Pass-through function to let applications access the COM UART.
//
//*****************************************************************************
uint32_t
am_bsp_com_uart_transfer(const am_hal_uart_transfer_t *psTransfer)
{
    return am_hal_uart_transfer(g_sCOMUART, psTransfer);
} // am_bsp_com_uart_transfer()

//*****************************************************************************
//
// Initialize and configure the UART
//
//*****************************************************************************
void
am_bsp_uart_printf_enable(void)
{
    //
    // Save the information that we're using the UART for printing.
    //
    g_ui32PrintInterface = AM_BSP_PRINT_INFC_UART0;

    //
    // Initialize, power up, and configure the communication UART. Use the
    // custom configuration if it was provided. Otherwise, just use the default
    // configuration.
    //
    am_hal_uart_initialize(AM_BSP_UART_PRINT_INST, &g_sCOMUART);
    am_hal_uart_power_control(g_sCOMUART, AM_HAL_SYSCTRL_WAKE, false);
    am_hal_uart_configure(g_sCOMUART, &g_sBspUartConfig);

    //
    // Enable the UART pins.
    //
    am_hal_gpio_pinconfig(AM_BSP_GPIO_COM_UART_TX, g_AM_BSP_GPIO_COM_UART_TX);
    am_hal_gpio_pinconfig(AM_BSP_GPIO_COM_UART_RX, g_AM_BSP_GPIO_COM_UART_RX);

    //
    // Register the BSP print function to the STDIO driver.
    //
    am_util_stdio_printf_init(am_bsp_uart_string_print);
} // am_bsp_uart_printf_enable()

//*****************************************************************************
//
// Initialize and configure the UART with a custom configuration
//
//*****************************************************************************
void
am_bsp_uart_printf_enable_custom(const am_hal_uart_config_t* p_config)
{
    //
    // Save the information that we're using the UART for printing.
    //
    g_ui32PrintInterface = AM_BSP_PRINT_INFC_UART0;

    //
    // Initialize, power up, and configure the communication UART. Use the
    // custom configuration if it was provided. Otherwise, just use the default
    // configuration.
    //
    am_hal_uart_initialize(AM_BSP_UART_PRINT_INST, &g_sCOMUART);
    am_hal_uart_power_control(g_sCOMUART, AM_HAL_SYSCTRL_WAKE, false);
    am_hal_uart_configure(g_sCOMUART, p_config);

    //
    // Enable the UART pins.
    //
    am_hal_gpio_pinconfig(AM_BSP_GPIO_COM_UART_TX, g_AM_BSP_GPIO_COM_UART_TX);
    am_hal_gpio_pinconfig(AM_BSP_GPIO_COM_UART_RX, g_AM_BSP_GPIO_COM_UART_RX);

    //
    // Register the BSP print function to the STDIO driver.
    //
    am_util_stdio_printf_init(am_bsp_uart_string_print);
} // am_bsp_uart_printf_enable()

//*****************************************************************************
//
// Disable the UART
//
//*****************************************************************************
void
am_bsp_uart_printf_disable(void)
{
    //
    // Make sure the UART has finished sending everything it's going to send.
    //
    am_hal_uart_tx_flush(g_sCOMUART);

    //
    // Detach the UART from the stdio driver.
    //
    am_util_stdio_printf_init(0);

    //
    // Power down the UART, and surrender the handle.
    //
    am_hal_uart_power_control(g_sCOMUART, AM_HAL_SYSCTRL_DEEPSLEEP, false);
    am_hal_uart_deinitialize(g_sCOMUART);

    //
    // Disable the UART pins.
    //
    am_hal_gpio_pinconfig(AM_BSP_GPIO_COM_UART_TX, g_AM_HAL_GPIO_DISABLE);
    am_hal_gpio_pinconfig(AM_BSP_GPIO_COM_UART_RX, g_AM_HAL_GPIO_DISABLE);

} // am_bsp_uart_printf_disable()

#ifndef AM_BSP_DISABLE_BUFFERED_UART
//*****************************************************************************
//
// Initialize and configure the UART
//
//*****************************************************************************
void
am_bsp_buffered_uart_printf_enable(void)
{
    //
    // Save the information that we're using the UART for printing.
    //
    g_ui32PrintInterface = AM_BSP_PRINT_INFC_UART0;

    //
    // Initialize, power up, and configure the communication UART. Use the
    // custom configuration if it was provided. Otherwise, just use the default
    // configuration.
    //
    am_hal_uart_initialize(AM_BSP_UART_PRINT_INST, &g_sCOMUART);
    am_hal_uart_power_control(g_sCOMUART, AM_HAL_SYSCTRL_WAKE, false);
    am_hal_uart_configure(g_sCOMUART, &g_sBspUartBufferedConfig);

    //
    // Enable the UART pins.
    //
    am_hal_gpio_pinconfig(AM_BSP_GPIO_COM_UART_TX, g_AM_BSP_GPIO_COM_UART_TX);
    am_hal_gpio_pinconfig(AM_BSP_GPIO_COM_UART_RX, g_AM_BSP_GPIO_COM_UART_RX);

    //
    // Register the BSP print function to the STDIO driver.
    //
    am_util_stdio_printf_init(am_bsp_uart_string_print);

    //
    // Enable the interrupts for the UART.
    //
    NVIC_EnableIRQ((IRQn_Type)(UART0_IRQn + AM_BSP_UART_PRINT_INST));
} // am_bsp_buffered_uart_printf_enable()

//*****************************************************************************
//
// Interrupt routine for the buffered UART interface.
//
//*****************************************************************************
void
am_bsp_buffered_uart_service(void)
{
    uint32_t ui32Status, ui32Idle;
    am_hal_uart_interrupt_status_get(g_sCOMUART, &ui32Status, true);
    am_hal_uart_interrupt_clear(g_sCOMUART, ui32Status);
    am_hal_uart_interrupt_service(g_sCOMUART, ui32Status, &ui32Idle);
} // am_bsp_buffered_uart_service()
#endif // AM_BSP_DISABLE_BUFFERED_UART



//*****************************************************************************
//
// End Doxygen group.
//! @}
//
//*****************************************************************************
