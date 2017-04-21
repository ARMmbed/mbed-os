/**
 * \file
 *
 * \brief SAMG55 Xplained Pro board initialization
 *
 * Copyright (c) 2014-2015 Atmel Corporation. All rights reserved.
 *
 * \asf_license_start
 *
 * \page License
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 * 1. Redistributions of source code must retain the above copyright notice,
 *    this list of conditions and the following disclaimer.
 *
 * 2. Redistributions in binary form must reproduce the above copyright notice,
 *    this list of conditions and the following disclaimer in the documentation
 *    and/or other materials provided with the distribution.
 *
 * 3. The name of Atmel may not be used to endorse or promote products derived
 *    from this software without specific prior written permission.
 *
 * 4. This software may only be redistributed and used in connection with an
 *    Atmel microcontroller product.
 *
 * THIS SOFTWARE IS PROVIDED BY ATMEL "AS IS" AND ANY EXPRESS OR IMPLIED
 * WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF
 * MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NON-INFRINGEMENT ARE
 * EXPRESSLY AND SPECIFICALLY DISCLAIMED. IN NO EVENT SHALL ATMEL BE LIABLE FOR
 * ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS
 * OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT,
 * STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN
 * ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 *
 * \asf_license_stop
 *
 */
/*
 * Support and FAQ: visit <a href="http://www.atmel.com/design-support/">Atmel Support</a>
 */

#include <compiler.h>
#include <board.h>
#include <conf_board.h>
#include <ioport.h>

/**
 * \addtogroup samg55_xplained_pro_group
 * @{
 */

/**
 * \brief Set peripheral mode for IOPORT pins.
 * It will configure port mode and disable pin mode (but enable peripheral).
 * \param port IOPORT port to configure
 * \param masks IOPORT pin masks to configure
 * \param mode Mode masks to configure for the specified pin (\ref ioport_modes)
 */
#define ioport_set_port_peripheral_mode(port, masks, mode) \
	do {\
		ioport_set_port_mode(port, masks, mode);\
		ioport_disable_port(port, masks);\
	} while (0)

/**
 * \brief Set peripheral mode for one single IOPORT pin.
 * It will configure port mode and disable pin mode (but enable peripheral).
 * \param pin IOPORT pin to configure
 * \param mode Mode masks to configure for the specified pin (\ref ioport_modes)
 */
#define ioport_set_pin_peripheral_mode(pin, mode) \
	do {\
		ioport_set_pin_mode(pin, mode);\
		ioport_disable_pin(pin);\
	} while (0)

#if defined(__GNUC__)
void board_init(void) WEAK __attribute__((alias("system_board_init")));
#elif defined(__ICCARM__)
void board_init(void);
#  pragma weak board_init=system_board_init
#endif

void system_board_init(void)
{

#ifndef CONF_BOARD_KEEP_WATCHDOG_AT_INIT
    WDT->WDT_MR = WDT_MR_WDDIS;
#endif
    ioport_init();

    /* Initialize LED0, turned off */
    ioport_set_pin_dir(LED_0_PIN, IOPORT_DIR_OUTPUT);
    ioport_set_pin_level(LED_0_PIN, IOPORT_PIN_LEVEL_HIGH);

    /* Initialize SW0 */
    ioport_set_pin_dir(BUTTON_0_PIN, IOPORT_DIR_INPUT);
    ioport_set_pin_mode(BUTTON_0_PIN, IOPORT_MODE_PULLUP);

    /* Initialize EXT3 LED0, LED1 & LED2, turned off */
    ioport_set_pin_level(OLED1_LED1_PIN, !OLED1_LED1_ACTIVE);
    ioport_set_pin_dir(OLED1_LED1_PIN, IOPORT_DIR_OUTPUT);
    ioport_set_pin_level(OLED1_LED2_PIN, !OLED1_LED2_ACTIVE);
    ioport_set_pin_dir(OLED1_LED2_PIN, IOPORT_DIR_OUTPUT);
    ioport_set_pin_level(OLED1_LED3_PIN, !OLED1_LED3_ACTIVE);
    ioport_set_pin_dir(OLED1_LED3_PIN, IOPORT_DIR_OUTPUT);

#if defined (CONF_BOARD_UART_CONSOLE)
    /* Configure UART pins */
    ioport_set_port_peripheral_mode(PINS_USART7_PORT, PINS_USART7,
                                    PINS_USART7_FLAGS);
#endif

#ifdef CONF_BOARD_USART_RXD
    /* Configure USART RXD pin */
    ioport_set_pin_peripheral_mode(EXT3_PIN_UART_RX,
                                   IOPORT_MODE_MUX_B);
#endif

#ifdef CONF_BOARD_USART_TXD
    /* Configure USART TXD pin */
    ioport_set_pin_peripheral_mode(EXT3_PIN_UART_TX,
                                   IOPORT_MODE_MUX_B);
#endif

#ifdef CONF_BOARD_USART_SCK
    /* Configure USART synchronous communication SCK pin */
    ioport_set_pin_peripheral_mode(EXT3_PIN_8,
                                   IOPORT_MODE_MUX_B);
#endif

#if defined(CONF_BOARD_SPI) || defined(CONF_BOARD_SD_MMC_SPI)
    ioport_set_pin_peripheral_mode(SPI_MISO_GPIO, SPI_MISO_FLAGS);
    ioport_set_pin_peripheral_mode(SPI_MOSI_GPIO, SPI_MOSI_FLAGS);
    ioport_set_pin_peripheral_mode(SPI_SPCK_GPIO, SPI_SPCK_FLAGS);

#ifdef CONF_BOARD_SD_MMC_SPI
    /* Setting SD detection pin */
    ioport_set_pin_dir(SD_MMC_0_CD_GPIO, IOPORT_DIR_INPUT);
    ioport_set_pin_mode(SD_MMC_0_CD_GPIO, IOPORT_MODE_PULLUP);
#endif

#ifdef CONF_BOARD_SPI_NPCS0
    ioport_set_pin_peripheral_mode(SPI_NPCS0_GPIO, SPI_NPCS0_FLAGS);
#endif

#ifdef CONF_BOARD_SPI_NPCS1
    ioport_set_pin_peripheral_mode(SPI_NPCS1_GPIO, SPI_NPCS1_FLAGS);
#endif
#endif

#ifdef CONF_BOARD_OLED_UG_2832HSWEG04
    ioport_set_pin_dir(UG_2832HSWEG04_DATA_CMD_GPIO, IOPORT_DIR_OUTPUT);
    ioport_set_pin_mode(UG_2832HSWEG04_DATA_CMD_GPIO, IOPORT_MODE_PULLUP);
    ioport_set_pin_dir(UG_2832HSWEG04_RESET_GPIO, IOPORT_DIR_OUTPUT);
    ioport_set_pin_mode(UG_2832HSWEG04_RESET_GPIO, IOPORT_MODE_PULLUP);
#endif

#ifdef CONF_BOARD_TWI0
    ioport_set_pin_peripheral_mode(TWI0_DATA_GPIO, TWI0_DATA_FLAGS);
    ioport_set_pin_peripheral_mode(TWI0_CLK_GPIO, TWI0_CLK_FLAGS);
#endif

#ifdef CONF_BOARD_TWI1
    ioport_set_pin_peripheral_mode(TWI1_DATA_GPIO, TWI1_DATA_FLAGS);
    ioport_set_pin_peripheral_mode(TWI1_CLK_GPIO, TWI1_CLK_FLAGS);
#endif

#ifdef CONF_BOARD_TWI2
    ioport_set_pin_peripheral_mode(TWI2_DATA_GPIO, TWI2_DATA_FLAGS);
    ioport_set_pin_peripheral_mode(TWI2_CLK_GPIO, TWI2_CLK_FLAGS);
#endif

#if defined(CONF_BOARD_TWI4) ||defined(CONF_BOARD_AT30TSE)
    ioport_set_pin_peripheral_mode(TWI4_DATA_GPIO, TWI4_DATA_FLAGS);
    ioport_set_pin_peripheral_mode(TWI4_CLK_GPIO, TWI4_CLK_FLAGS);
#endif

#ifdef CONF_BOARD_I2S0
    ioport_set_pin_peripheral_mode(I2S0_SCK_GPIO, I2S0_SCK_FLAGS);
    ioport_set_pin_peripheral_mode(I2S0_MCK_GPIO, I2S0_MCK_FLAGS);
    ioport_set_pin_peripheral_mode(I2S0_SDI_GPIO, I2S0_SDI_FLAGS);
    ioport_set_pin_peripheral_mode(I2S0_SDO_GPIO, I2S0_SDO_FLAGS);
    ioport_set_pin_peripheral_mode(I2S0_WS_GPIO, I2S0_WS_FLAGS);
#endif

#ifdef CONF_BOARD_I2S1
    ioport_set_pin_peripheral_mode(I2S1_SCK_GPIO, I2S1_SCK_FLAGS);
    ioport_set_pin_peripheral_mode(I2S1_MCK_GPIO, I2S1_MCK_FLAGS);
    ioport_set_pin_peripheral_mode(I2S1_SDI_GPIO, I2S1_SDI_FLAGS);
    ioport_set_pin_peripheral_mode(I2S1_SDO_GPIO, I2S1_SDO_FLAGS);
    ioport_set_pin_peripheral_mode(I2S1_WS_GPIO, I2S1_WS_FLAGS);
#endif

#ifdef CONF_BOARD_PDM
    ioport_set_pin_peripheral_mode(PDM_CLK_GPIO, PDM_CLK_FLAGS);
    ioport_set_pin_peripheral_mode(PDM_DAT_GPIO, PDM_DAT_FLAGS);
#endif

#ifdef CONF_BOARD_USART0
    ioport_set_pin_peripheral_mode(USART0_RXD_GPIO, USART0_RXD_FLAGS);
    ioport_set_pin_peripheral_mode(USART0_TXD_GPIO, USART0_TXD_FLAGS);
    ioport_set_pin_peripheral_mode(USART0_SCK_GPIO, USART0_SCK_FLAGS);
    ioport_set_pin_peripheral_mode(USART0_CTS_GPIO, USART0_CTS_FLAGS);
    ioport_set_pin_peripheral_mode(USART0_RTS_GPIO, USART0_RTS_FLAGS);
#endif

#ifdef CONF_BOARD_USART6
    ioport_set_pin_peripheral_mode(USART6_RXD_GPIO, USART6_RXD_FLAGS);
    ioport_set_pin_peripheral_mode(USART6_TXD_GPIO, USART6_TXD_FLAGS);
    ioport_set_pin_peripheral_mode(USART6_SCK_GPIO, USART6_SCK_FLAGS);
    ioport_set_pin_peripheral_mode(USART6_CTS_GPIO, USART6_CTS_FLAGS);
    ioport_set_pin_peripheral_mode(USART6_RTS_GPIO, USART6_RTS_FLAGS);
#endif

#if defined(CONF_BOARD_USB_PORT)
#  if defined(CONF_BOARD_USB_VBUS_DETECT)
    gpio_configure_pin(USB_VBUS_PIN, USB_VBUS_FLAGS);
#  endif
#endif
}

/** @} */
