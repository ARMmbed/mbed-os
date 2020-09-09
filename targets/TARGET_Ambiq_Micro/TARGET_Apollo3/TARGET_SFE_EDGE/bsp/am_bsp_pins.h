//*****************************************************************************
//
//  am_bsp_pins.h
//! @file
//!
//! @brief BSP pin configuration definitions.
//!
//! @addtogroup BSP Board Support Package (BSP)
//! @addtogroup apollo3_bsp BSP for the Apollo3 EVB.
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
// This is part of revision 2.2.0-hotfix-2.2.1 of the AmbiqSuite Development Package.
//
//*****************************************************************************
// SPDX-License-Identifier: BSD-3-Clause

#ifndef AM_BSP_PINS_H
#define AM_BSP_PINS_H

#include <stdint.h>
#include <stdbool.h>
#include "am_mcu_apollo.h"

#ifdef __cplusplus
extern "C"
{
#endif

//*****************************************************************************
//
//  CAMERA_HM01B0_D0 pin.
//
//*****************************************************************************
#define AM_BSP_GPIO_CAMERA_HM01B0_D0    	24
extern const am_hal_gpio_pincfg_t       g_AM_BSP_GPIO_CAMERA_HM01B0_D0;

//*****************************************************************************
//
//  CAMERA_HM01B0_D1 pin.
//
//*****************************************************************************
#define AM_BSP_GPIO_CAMERA_HM01B0_D1    	25
extern const am_hal_gpio_pincfg_t       g_AM_BSP_GPIO_CAMERA_HM01B0_D1;

//*****************************************************************************
//
//  CAMERA_HM01B0_D2 pin.
//
//*****************************************************************************
#define AM_BSP_GPIO_CAMERA_HM01B0_D2    	26
extern const am_hal_gpio_pincfg_t       g_AM_BSP_GPIO_CAMERA_HM01B0_D2;

//*****************************************************************************
//
//  CAMERA_HM01B0_D3 pin.
//
//*****************************************************************************
#define AM_BSP_GPIO_CAMERA_HM01B0_D3    	27
extern const am_hal_gpio_pincfg_t       g_AM_BSP_GPIO_CAMERA_HM01B0_D3;

//*****************************************************************************
//
//  CAMERA_HM01B0_D4 pin.
//
//*****************************************************************************
#define AM_BSP_GPIO_CAMERA_HM01B0_D4    	28
extern const am_hal_gpio_pincfg_t       g_AM_BSP_GPIO_CAMERA_HM01B0_D4;

//*****************************************************************************
//
//  CAMERA_HM01B0_D5 pin.
//
//*****************************************************************************
#define AM_BSP_GPIO_CAMERA_HM01B0_D5    	5
extern const am_hal_gpio_pincfg_t       g_AM_BSP_GPIO_CAMERA_HM01B0_D5;

//*****************************************************************************
//
//  CAMERA_HM01B0_D6 pin.
//
//*****************************************************************************
#define AM_BSP_GPIO_CAMERA_HM01B0_D6    	6
extern const am_hal_gpio_pincfg_t       g_AM_BSP_GPIO_CAMERA_HM01B0_D6;

//*****************************************************************************
//
//  CAMERA_HM01B0_D7 pin.
//
//*****************************************************************************
#define AM_BSP_GPIO_CAMERA_HM01B0_D7    	7
extern const am_hal_gpio_pincfg_t       g_AM_BSP_GPIO_CAMERA_HM01B0_D7;

//*****************************************************************************
//
//  CAMERA_HM01B0_VSYNC pin: Also called FVLD on the HM01B0 module.
//
//*****************************************************************************
#define AM_BSP_GPIO_CAMERA_HM01B0_VSYNC 	15
extern const am_hal_gpio_pincfg_t       g_AM_BSP_GPIO_CAMERA_HM01B0_VSYNC;

//*****************************************************************************
//
//  CAMERA_HM01B0_HSYNC pin: Also called LVLD on the HM01B0 module.
//
//*****************************************************************************
#define AM_BSP_GPIO_CAMERA_HM01B0_HSYNC 	22
extern const am_hal_gpio_pincfg_t       g_AM_BSP_GPIO_CAMERA_HM01B0_HSYNC;

//*****************************************************************************
//
//  CAMERA_HM01B0_PCLK pin.
//
//*****************************************************************************
#define AM_BSP_GPIO_CAMERA_HM01B0_PCLK  	23
extern const am_hal_gpio_pincfg_t       g_AM_BSP_GPIO_CAMERA_HM01B0_PCLK;

//*****************************************************************************
//
//  CAMERA_HM01B0_TRIG pin.
//
//*****************************************************************************
#define AM_BSP_GPIO_CAMERA_HM01B0_TRIG  	12
extern const am_hal_gpio_pincfg_t       g_AM_BSP_GPIO_CAMERA_HM01B0_TRIG;

//*****************************************************************************
//
//  CAMERA_HM01B0_INT pin.
//
//*****************************************************************************
#define AM_BSP_GPIO_CAMERA_HM01B0_INT   	4
extern const am_hal_gpio_pincfg_t       g_AM_BSP_GPIO_CAMERA_HM01B0_INT;

//*****************************************************************************
//
//  CAMERA_HM01B0_DVDDEN pin.
//
//*****************************************************************************
#define AM_BSP_GPIO_CAMERA_HM01B0_DVDDEN	10
extern const am_hal_gpio_pincfg_t       g_AM_BSP_GPIO_CAMERA_HM01B0_DVDDEN;

//*****************************************************************************
//
//  MIC0 pin: Analog microphone near camera connector.
//
//*****************************************************************************
#define AM_BSP_GPIO_MIC0                	11
extern const am_hal_gpio_pincfg_t       g_AM_BSP_GPIO_MIC0;

//*****************************************************************************
//
//  MIC1 pin: Analog microphone near LEDs.
//
//*****************************************************************************
#define AM_BSP_GPIO_MIC1                	29
extern const am_hal_gpio_pincfg_t       g_AM_BSP_GPIO_MIC1;

//*****************************************************************************
//
//  BUTTON14 pin: Labeled 14 on the SparkFun Edge.
//
//*****************************************************************************
#define AM_BSP_GPIO_BUTTON14            	14
extern const am_hal_gpio_pincfg_t       g_AM_BSP_GPIO_BUTTON14;

//*****************************************************************************
//
//  LED_RED pin: The RED LED labelled 46.
//
//*****************************************************************************
#define AM_BSP_GPIO_LED_RED             	46
extern const am_hal_gpio_pincfg_t       g_AM_BSP_GPIO_LED_RED;

//*****************************************************************************
//
//  LED_BLUE pin: The BLUE LED labelled 37.
//
//*****************************************************************************
#define AM_BSP_GPIO_LED_BLUE            	37
extern const am_hal_gpio_pincfg_t       g_AM_BSP_GPIO_LED_BLUE;

//*****************************************************************************
//
//  LED_GREEN pin: The GREEN LED labelled 44.
//
//*****************************************************************************
#define AM_BSP_GPIO_LED_GREEN           	44
extern const am_hal_gpio_pincfg_t       g_AM_BSP_GPIO_LED_GREEN;

//*****************************************************************************
//
//  LED_YELLOW pin: The YELLOW LED labelled 47.
//
//*****************************************************************************
#define AM_BSP_GPIO_LED_YELLOW          	47
extern const am_hal_gpio_pincfg_t       g_AM_BSP_GPIO_LED_YELLOW;

//*****************************************************************************
//
//  COM_UART_TX pin: This pin is the COM_UART transmit pin.
//
//*****************************************************************************
#define AM_BSP_GPIO_COM_UART_TX         	48
extern const am_hal_gpio_pincfg_t       g_AM_BSP_GPIO_COM_UART_TX;

//*****************************************************************************
//
//  COM_UART_RX pin: This pin is the COM_UART receive pin.
//
//*****************************************************************************
#define AM_BSP_GPIO_COM_UART_RX         	49
extern const am_hal_gpio_pincfg_t       g_AM_BSP_GPIO_COM_UART_RX;

//*****************************************************************************
//
//  IOM0_CS pin: I/O Master 0 chip select.
//
//*****************************************************************************
#define AM_BSP_GPIO_IOM0_CS             	11
extern const am_hal_gpio_pincfg_t       g_AM_BSP_GPIO_IOM0_CS;
#define AM_BSP_IOM0_CS_CHNL             0

//*****************************************************************************
//
//  IOM0_CS3 pin: I/O Master 0 chip select.
//
//*****************************************************************************
#define AM_BSP_GPIO_IOM0_CS3            	15
extern const am_hal_gpio_pincfg_t       g_AM_BSP_GPIO_IOM0_CS3;
#define AM_BSP_IOM0_CS3_CHNL            3

//*****************************************************************************
//
//  IOM0_MISO pin: I/O Master 0 SPI MISO signal.
//
//*****************************************************************************
#define AM_BSP_GPIO_IOM0_MISO           	6
extern const am_hal_gpio_pincfg_t       g_AM_BSP_GPIO_IOM0_MISO;

//*****************************************************************************
//
//  IOM0_MOSI pin: I/O Master 0 SPI MOSI signal.
//
//*****************************************************************************
#define AM_BSP_GPIO_IOM0_MOSI           	7
extern const am_hal_gpio_pincfg_t       g_AM_BSP_GPIO_IOM0_MOSI;

//*****************************************************************************
//
//  IOM0_SCK pin: I/O Master 0 SPI SCK signal.
//
//*****************************************************************************
#define AM_BSP_GPIO_IOM0_SCK            	5
extern const am_hal_gpio_pincfg_t       g_AM_BSP_GPIO_IOM0_SCK;

//*****************************************************************************
//
//  IOM0_SCL pin: I/O Master 0 I2C clock signal.
//
//*****************************************************************************
#define AM_BSP_GPIO_IOM0_SCL            	5
extern const am_hal_gpio_pincfg_t       g_AM_BSP_GPIO_IOM0_SCL;

//*****************************************************************************
//
//  IOM0_SDA pin: I/O Master 0 I2C data signal.
//
//*****************************************************************************
#define AM_BSP_GPIO_IOM0_SDA            	6
extern const am_hal_gpio_pincfg_t       g_AM_BSP_GPIO_IOM0_SDA;

//*****************************************************************************
//
//  IOM1_CS pin: I/O Master 1 chip select.
//
//*****************************************************************************
#define AM_BSP_GPIO_IOM1_CS             	14
extern const am_hal_gpio_pincfg_t       g_AM_BSP_GPIO_IOM1_CS;
#define AM_BSP_IOM1_CS_CHNL             2

//*****************************************************************************
//
//  IOM1_MISO pin: I/O Master 1 SPI MISO signal.
//
//*****************************************************************************
#define AM_BSP_GPIO_IOM1_MISO           	9
extern const am_hal_gpio_pincfg_t       g_AM_BSP_GPIO_IOM1_MISO;

//*****************************************************************************
//
//  IOM1_MOSI pin: I/O Master 1 SPI MOSI signal.
//
//*****************************************************************************
#define AM_BSP_GPIO_IOM1_MOSI           	10
extern const am_hal_gpio_pincfg_t       g_AM_BSP_GPIO_IOM1_MOSI;

//*****************************************************************************
//
//  IOM1_SCK pin: I/O Master 1 SPI SCK signal.
//
//*****************************************************************************
#define AM_BSP_GPIO_IOM1_SCK            	8
extern const am_hal_gpio_pincfg_t       g_AM_BSP_GPIO_IOM1_SCK;

//*****************************************************************************
//
//  IOM1_SCL pin: I/O Master 1 I2C clock signal.
//
//*****************************************************************************
#define AM_BSP_GPIO_IOM1_SCL            	8
extern const am_hal_gpio_pincfg_t       g_AM_BSP_GPIO_IOM1_SCL;

//*****************************************************************************
//
//  IOM1_SDA pin: I/O Master 1 I2C data signal.
//
//*****************************************************************************
#define AM_BSP_GPIO_IOM1_SDA            	9
extern const am_hal_gpio_pincfg_t       g_AM_BSP_GPIO_IOM1_SDA;

//*****************************************************************************
//
//  IOM2_CS pin: I/O Master 2 chip select.
//
//*****************************************************************************
#define AM_BSP_GPIO_IOM2_CS             	15
extern const am_hal_gpio_pincfg_t       g_AM_BSP_GPIO_IOM2_CS;
#define AM_BSP_IOM2_CS_CHNL             3

//*****************************************************************************
//
//  IOM2_MISO pin: I/O Master 2 SPI MISO signal.
//
//*****************************************************************************
#define AM_BSP_GPIO_IOM2_MISO           	25
extern const am_hal_gpio_pincfg_t       g_AM_BSP_GPIO_IOM2_MISO;

//*****************************************************************************
//
//  IOM2_MOSI pin: I/O Master 2 SPI MOSI signal.
//
//*****************************************************************************
#define AM_BSP_GPIO_IOM2_MOSI           	28
extern const am_hal_gpio_pincfg_t       g_AM_BSP_GPIO_IOM2_MOSI;

//*****************************************************************************
//
//  IOM2_SCK pin: I/O Master 2 SPI SCK signal.
//
//*****************************************************************************
#define AM_BSP_GPIO_IOM2_SCK            	27
extern const am_hal_gpio_pincfg_t       g_AM_BSP_GPIO_IOM2_SCK;

//*****************************************************************************
//
//  IOM2_SCL pin: I/O Master 2 I2C clock signal.
//
//*****************************************************************************
#define AM_BSP_GPIO_IOM2_SCL            	27
extern const am_hal_gpio_pincfg_t       g_AM_BSP_GPIO_IOM2_SCL;

//*****************************************************************************
//
//  IOM2_SDA pin: I/O Master 2 I2C data signal.
//
//*****************************************************************************
#define AM_BSP_GPIO_IOM2_SDA            	25
extern const am_hal_gpio_pincfg_t       g_AM_BSP_GPIO_IOM2_SDA;

//*****************************************************************************
//
//  IOM3_CS pin: I/O Master 3 chip select.
//
//*****************************************************************************
#define AM_BSP_GPIO_IOM3_CS             	12
extern const am_hal_gpio_pincfg_t       g_AM_BSP_GPIO_IOM3_CS;
#define AM_BSP_IOM3_CS_CHNL             0

//*****************************************************************************
//
//  IOM3_MISO pin: I/O Master 3 SPI MISO signal.
//
//*****************************************************************************
#define AM_BSP_GPIO_IOM3_MISO           	43
extern const am_hal_gpio_pincfg_t       g_AM_BSP_GPIO_IOM3_MISO;

//*****************************************************************************
//
//  IOM3_MOSI pin: I/O Master 3 SPI MOSI signal.
//
//*****************************************************************************
#define AM_BSP_GPIO_IOM3_MOSI           	38
extern const am_hal_gpio_pincfg_t       g_AM_BSP_GPIO_IOM3_MOSI;

//*****************************************************************************
//
//  IOM3_SCK pin: I/O Master 3 SPI SCK signal.
//
//*****************************************************************************
#define AM_BSP_GPIO_IOM3_SCK            	42
extern const am_hal_gpio_pincfg_t       g_AM_BSP_GPIO_IOM3_SCK;

//*****************************************************************************
//
//  IOM3_SCL pin: I/O Master 3 I2C clock signal.
//
//*****************************************************************************
#define AM_BSP_GPIO_IOM3_SCL            	42
extern const am_hal_gpio_pincfg_t       g_AM_BSP_GPIO_IOM3_SCL;

//*****************************************************************************
//
//  IOM3_SDA pin: I/O Master 3 I2C data signal.
//
//*****************************************************************************
#define AM_BSP_GPIO_IOM3_SDA            	43
extern const am_hal_gpio_pincfg_t       g_AM_BSP_GPIO_IOM3_SDA;

//*****************************************************************************
//
//  IOM4_CS pin: I/O Master 4 chip select.
//
//*****************************************************************************
#define AM_BSP_GPIO_IOM4_CS             	13
extern const am_hal_gpio_pincfg_t       g_AM_BSP_GPIO_IOM4_CS;
#define AM_BSP_IOM4_CS_CHNL             1

//*****************************************************************************
//
//  IOM4_MISO pin: I/O Master 4 SPI MISO signal.
//
//*****************************************************************************
#define AM_BSP_GPIO_IOM4_MISO           	40
extern const am_hal_gpio_pincfg_t       g_AM_BSP_GPIO_IOM4_MISO;

//*****************************************************************************
//
//  IOM4_MOSI pin: I/O Master 4 SPI MOSI signal.
//
//*****************************************************************************
#define AM_BSP_GPIO_IOM4_MOSI           	44
extern const am_hal_gpio_pincfg_t       g_AM_BSP_GPIO_IOM4_MOSI;

//*****************************************************************************
//
//  IOM4_SCK pin: I/O Master 4 SPI SCK signal.
//
//*****************************************************************************
#define AM_BSP_GPIO_IOM4_SCK            	39
extern const am_hal_gpio_pincfg_t       g_AM_BSP_GPIO_IOM4_SCK;

//*****************************************************************************
//
//  IOM4_SCL pin: I/O Master 4 I2C clock signal.
//
//*****************************************************************************
#define AM_BSP_GPIO_IOM4_SCL            	39
extern const am_hal_gpio_pincfg_t       g_AM_BSP_GPIO_IOM4_SCL;

//*****************************************************************************
//
//  IOM4_SDA pin: I/O Master 4 I2C data signal.
//
//*****************************************************************************
#define AM_BSP_GPIO_IOM4_SDA            	40
extern const am_hal_gpio_pincfg_t       g_AM_BSP_GPIO_IOM4_SDA;

//*****************************************************************************
//
//  IOM5_CS pin: I/O Master 5 chip select.
//
//*****************************************************************************
#define AM_BSP_GPIO_IOM5_CS             	16
extern const am_hal_gpio_pincfg_t       g_AM_BSP_GPIO_IOM5_CS;
#define AM_BSP_IOM5_CS_CHNL             0

//*****************************************************************************
//
//  IOM5_MISO pin: I/O Master 5 SPI MISO signal.
//
//*****************************************************************************
#define AM_BSP_GPIO_IOM5_MISO           	49
extern const am_hal_gpio_pincfg_t       g_AM_BSP_GPIO_IOM5_MISO;

//*****************************************************************************
//
//  IOM5_MOSI pin: I/O Master 5 SPI MOSI signal.
//
//*****************************************************************************
#define AM_BSP_GPIO_IOM5_MOSI           	47
extern const am_hal_gpio_pincfg_t       g_AM_BSP_GPIO_IOM5_MOSI;

//*****************************************************************************
//
//  IOM5_SCK pin: I/O Master 5 SPI SCK signal.
//
//*****************************************************************************
#define AM_BSP_GPIO_IOM5_SCK            	48
extern const am_hal_gpio_pincfg_t       g_AM_BSP_GPIO_IOM5_SCK;

//*****************************************************************************
//
//  IOM5_SCL pin: I/O Master 5 I2C clock signal.
//
//*****************************************************************************
#define AM_BSP_GPIO_IOM5_SCL            	48
extern const am_hal_gpio_pincfg_t       g_AM_BSP_GPIO_IOM5_SCL;

//*****************************************************************************
//
//  IOM5_SDA pin: I/O Master 5 I2C data signal.
//
//*****************************************************************************
#define AM_BSP_GPIO_IOM5_SDA            	49
extern const am_hal_gpio_pincfg_t       g_AM_BSP_GPIO_IOM5_SDA;

//*****************************************************************************
//
//  MSPI_CE0 pin: MSPI chip select.
//
//*****************************************************************************
#define AM_BSP_GPIO_MSPI_CE0            	19
extern const am_hal_gpio_pincfg_t       g_AM_BSP_GPIO_MSPI_CE0;
#define AM_BSP_MSPI_CE0_CHNL            0

//*****************************************************************************
//
//  MSPI_CE1 pin: MSPI chip select.
//
//*****************************************************************************
#define AM_BSP_GPIO_MSPI_CE1            	41
extern const am_hal_gpio_pincfg_t       g_AM_BSP_GPIO_MSPI_CE1;
#define AM_BSP_MSPI_CE1_CHNL            1

//*****************************************************************************
//
//  MSPI_D0 pin: MSPI data 0.
//
//*****************************************************************************
#define AM_BSP_GPIO_MSPI_D0             	22
extern const am_hal_gpio_pincfg_t       g_AM_BSP_GPIO_MSPI_D0;

//*****************************************************************************
//
//  MSPI_D1 pin: MSPI data 1.
//
//*****************************************************************************
#define AM_BSP_GPIO_MSPI_D1             	26
extern const am_hal_gpio_pincfg_t       g_AM_BSP_GPIO_MSPI_D1;

//*****************************************************************************
//
//  MSPI_D2 pin: MSPI data 2.
//
//*****************************************************************************
#define AM_BSP_GPIO_MSPI_D2             	4
extern const am_hal_gpio_pincfg_t       g_AM_BSP_GPIO_MSPI_D2;

//*****************************************************************************
//
//  MSPI_D3 pin: MSPI data 3.
//
//*****************************************************************************
#define AM_BSP_GPIO_MSPI_D3             	23
extern const am_hal_gpio_pincfg_t       g_AM_BSP_GPIO_MSPI_D3;

//*****************************************************************************
//
//  MSPI_D4 pin: MSPI data 4.
//
//*****************************************************************************
#define AM_BSP_GPIO_MSPI_D4             	0
extern const am_hal_gpio_pincfg_t       g_AM_BSP_GPIO_MSPI_D4;

//*****************************************************************************
//
//  MSPI_D5 pin: MSPI data 5.
//
//*****************************************************************************
#define AM_BSP_GPIO_MSPI_D5             	1
extern const am_hal_gpio_pincfg_t       g_AM_BSP_GPIO_MSPI_D5;

//*****************************************************************************
//
//  MSPI_D6 pin: MSPI data 6.
//
//*****************************************************************************
#define AM_BSP_GPIO_MSPI_D6             	2
extern const am_hal_gpio_pincfg_t       g_AM_BSP_GPIO_MSPI_D6;

//*****************************************************************************
//
//  MSPI_D7 pin: MSPI data 7.
//
//*****************************************************************************
#define AM_BSP_GPIO_MSPI_D7             	3
extern const am_hal_gpio_pincfg_t       g_AM_BSP_GPIO_MSPI_D7;

//*****************************************************************************
//
//  MSPI_SCK pin: MSPI clock.
//
//*****************************************************************************
#define AM_BSP_GPIO_MSPI_SCK            	24
extern const am_hal_gpio_pincfg_t       g_AM_BSP_GPIO_MSPI_SCK;

//*****************************************************************************
//
//  IOS_CE pin: I/O Slave chip select.
//
//*****************************************************************************
#define AM_BSP_GPIO_IOS_CE              	3
extern const am_hal_gpio_pincfg_t       g_AM_BSP_GPIO_IOS_CE;
#define AM_BSP_IOS_CE_CHNL              0

//*****************************************************************************
//
//  IOS_MISO pin: I/O Slave SPI MISO signal.
//
//*****************************************************************************
#define AM_BSP_GPIO_IOS_MISO            	2
extern const am_hal_gpio_pincfg_t       g_AM_BSP_GPIO_IOS_MISO;

//*****************************************************************************
//
//  IOS_MOSI pin: I/O Slave SPI MOSI signal.
//
//*****************************************************************************
#define AM_BSP_GPIO_IOS_MOSI            	1
extern const am_hal_gpio_pincfg_t       g_AM_BSP_GPIO_IOS_MOSI;

//*****************************************************************************
//
//  IOS_SCK pin: I/O Slave SPI SCK signal.
//
//*****************************************************************************
#define AM_BSP_GPIO_IOS_SCK             	0
extern const am_hal_gpio_pincfg_t       g_AM_BSP_GPIO_IOS_SCK;

//*****************************************************************************
//
//  IOS_SCL pin: I/O Slave I2C clock signal.
//
//*****************************************************************************
#define AM_BSP_GPIO_IOS_SCL             	0
extern const am_hal_gpio_pincfg_t       g_AM_BSP_GPIO_IOS_SCL;

//*****************************************************************************
//
//  IOS_SDA pin: I/O Slave I2C data signal.
//
//*****************************************************************************
#define AM_BSP_GPIO_IOS_SDA             	1
extern const am_hal_gpio_pincfg_t       g_AM_BSP_GPIO_IOS_SDA;

//*****************************************************************************
//
//  ITM_SWO pin: ITM Serial Wire Output.
//
//*****************************************************************************
#define AM_BSP_GPIO_ITM_SWO             	33
extern const am_hal_gpio_pincfg_t       g_AM_BSP_GPIO_ITM_SWO;

//*****************************************************************************
//
//  SWDCK pin: Cortex Serial Wire DCK.
//
//*****************************************************************************
#define AM_BSP_GPIO_SWDCK               	20
extern const am_hal_gpio_pincfg_t       g_AM_BSP_GPIO_SWDCK;

//*****************************************************************************
//
//  SWDIO pin: Cortex Serial Wire DIO.
//
//*****************************************************************************
#define AM_BSP_GPIO_SWDIO               	21
extern const am_hal_gpio_pincfg_t       g_AM_BSP_GPIO_SWDIO;


#ifdef __cplusplus
}
#endif

#endif // AM_BSP_PINS_H

//*****************************************************************************
//
// End Doxygen group.
//! @}
//
//*****************************************************************************
