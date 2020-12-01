/**
 * \file
 *
 * \brief Uart Serial for SAM.
 *
 * Copyright (c) 2011-2018 Microchip Technology Inc. and its subsidiaries.
 *
 * \asf_license_start
 *
 * \page License
 *
 * Subject to your compliance with these terms, you may use Microchip
 * software and any derivatives exclusively with Microchip products.
 * It is your responsibility to comply with third party license terms applicable
 * to your use of third party software (including open source software) that
 * may accompany Microchip software.
 *
 * THIS SOFTWARE IS SUPPLIED BY MICROCHIP "AS IS". NO WARRANTIES,
 * WHETHER EXPRESS, IMPLIED OR STATUTORY, APPLY TO THIS SOFTWARE,
 * INCLUDING ANY IMPLIED WARRANTIES OF NON-INFRINGEMENT, MERCHANTABILITY,
 * AND FITNESS FOR A PARTICULAR PURPOSE. IN NO EVENT WILL MICROCHIP BE
 * LIABLE FOR ANY INDIRECT, SPECIAL, PUNITIVE, INCIDENTAL OR CONSEQUENTIAL
 * LOSS, DAMAGE, COST OR EXPENSE OF ANY KIND WHATSOEVER RELATED TO THE
 * SOFTWARE, HOWEVER CAUSED, EVEN IF MICROCHIP HAS BEEN ADVISED OF THE
 * POSSIBILITY OR THE DAMAGES ARE FORESEEABLE.  TO THE FULLEST EXTENT
 * ALLOWED BY LAW, MICROCHIP'S TOTAL LIABILITY ON ALL CLAIMS IN ANY WAY
 * RELATED TO THIS SOFTWARE WILL NOT EXCEED THE AMOUNT OF FEES, IF ANY,
 * THAT YOU HAVE PAID DIRECTLY TO MICROCHIP FOR THIS SOFTWARE.
 *
 * \asf_license_stop
 *
 */

/*
 * Support and FAQ: visit <a href="https://www.microchip.com/support/">Microchip Support</a>
 */
#ifndef _UART_SERIAL_H_
#define _UART_SERIAL_H_

#include "compiler.h"
#include "sysclk.h"
#include "flexcom.h"
#include "usart.h"
#include "uart.h"

/**
 * \name Serial Management Configuration
 */
/* ! @{ */
//#include "conf_uart_serial.h"

/* ! @} */

/** Input parameters when initializing RS232 and similar modes. */
typedef struct usart_rs232_options {
	/** Set baud rate of the USART (unused in slave modes). */
	uint32_t baudrate;

	/** Number of bits to transmit as a character (5-bit to 9-bit). */
	uint32_t charlength;

	/** Parity type: EVEN, ODD, SPACE, MARK, NONE. */
	uint32_t paritytype;

	/** 1, 1.5 or 2 stop bits. */
	uint32_t stopbits;
} usart_rs232_options_t;

typedef usart_rs232_options_t usart_serial_options_t;

typedef Usart *usart_if;

/**
 * \brief Initializes the Usart in master mode.
 *
 * \param p_usart  Base address of the USART instance.
 * \param opt      Options needed to set up RS232 communication (see
 * \ref usart_options_t).
 */
static inline void usart_serial_init(usart_if p_usart,
		usart_serial_options_t *opt)
{
	if (UART == (Uart *)p_usart) {
		sam_uart_opt_t uart_settings;
		uart_settings.ul_mck = sysclk_get_peripheral_hz();
		uart_settings.ul_baudrate = opt->baudrate;
		uart_settings.ul_mode = opt->paritytype;

		sysclk_enable_peripheral_clock(ID_UART);
		/* Configure UART */
		uart_init((Uart *)p_usart, &uart_settings);
	}
	else {
		uint32_t ul_clk;
		uint32_t ul_div;
		
		/* Fexcomm USART */
		sam_usart_opt_t usart_settings;
		usart_settings.baudrate = opt->baudrate;
		usart_settings.char_length = opt->charlength;
		usart_settings.parity_type = opt->paritytype;
		usart_settings.stop_bits = opt->stopbits;
		usart_settings.channel_mode = US_MR_CHMODE_NORMAL;
		ul_clk = sysclk_get_peripheral_hz();
		if (ul_clk > 100000000) {
			ul_div = 2;
			ul_clk >>= 1;
		} else {
			ul_div = 1;
		}

		if (USART0 == p_usart) {
			/* Configure GCLK */
			pmc_configure_generic(ID_FLEXCOM0, PMC_PCR_GCLKCSS_MCK0, ul_div);
			pmc_enable_generic_clk(ID_FLEXCOM0);
			usart_set_clk_source(USART0, USART_SRC_CLK_GCLK);
			/* Enable Flexcom */
			flexcom_enable(FLEXCOM0);
			flexcom_set_opmode(FLEXCOM0, FLEXCOM_USART);
			/* Configure USART */
			usart_init_rs232(p_usart, &usart_settings, ul_clk);
			/* Enable the receiver and transmitter. */
			usart_enable_tx(p_usart);
			usart_enable_rx(p_usart);
		} else if (USART1 == p_usart) {
			/* Configure GCLK */
			pmc_configure_generic(ID_FLEXCOM1, PMC_PCR_GCLKCSS_MCK0, ul_div);
			pmc_enable_generic_clk(ID_FLEXCOM1);
			usart_set_clk_source(USART1, USART_SRC_CLK_GCLK);
			/* Enable Flexcom */
			flexcom_enable(FLEXCOM1);
			flexcom_set_opmode(FLEXCOM1, FLEXCOM_USART);
			/* Configure USART */
			usart_init_rs232(p_usart, &usart_settings,
					sysclk_get_peripheral_hz());
			/* Enable the receiver and transmitter. */
			usart_enable_tx(p_usart);
			usart_enable_rx(p_usart);
		} else if (USART2 == p_usart) {
			/* Configure GCLK */
			pmc_configure_generic(ID_FLEXCOM2, PMC_PCR_GCLKCSS_MCK0, ul_div);
			pmc_enable_generic_clk(ID_FLEXCOM2);
			usart_set_clk_source(USART2, USART_SRC_CLK_GCLK);
			/* Enable Flexcom */
			flexcom_enable(FLEXCOM2);
			flexcom_set_opmode(FLEXCOM2, FLEXCOM_USART);
			/* Configure USART */
			usart_init_rs232(p_usart, &usart_settings,
					sysclk_get_peripheral_hz());
			/* Enable the receiver and transmitter. */
			usart_enable_tx(p_usart);
			usart_enable_rx(p_usart);
		} else if (USART3 == p_usart) {
			/* Configure GCLK */
			pmc_configure_generic(ID_FLEXCOM3, PMC_PCR_GCLKCSS_MCK0, ul_div);
			pmc_enable_generic_clk(ID_FLEXCOM3);
			usart_set_clk_source(USART3, USART_SRC_CLK_GCLK);
			/* Enable Flexcom */
			flexcom_enable(FLEXCOM3);
			flexcom_set_opmode(FLEXCOM3, FLEXCOM_USART);
			/* Configure USART */
			usart_init_rs232(p_usart, &usart_settings,
					sysclk_get_peripheral_hz());
			/* Enable the receiver and transmitter. */
			usart_enable_tx(p_usart);
			usart_enable_rx(p_usart);
		} else if (USART4 == p_usart) {
			/* Configure GCLK */
			pmc_configure_generic(ID_FLEXCOM4, PMC_PCR_GCLKCSS_MCK0, ul_div);
			pmc_enable_generic_clk(ID_FLEXCOM4);
			usart_set_clk_source(USART4, USART_SRC_CLK_GCLK);
			/* Enable Flexcom */
			flexcom_enable(FLEXCOM4);
			flexcom_set_opmode(FLEXCOM4, FLEXCOM_USART);
			/* Configure USART */
			usart_init_rs232(p_usart, &usart_settings,
					sysclk_get_peripheral_hz());
			/* Enable the receiver and transmitter. */
			usart_enable_tx(p_usart);
			usart_enable_rx(p_usart);
		} else if (USART5 == p_usart) {
			/* Configure GCLK */
			pmc_configure_generic(ID_FLEXCOM5, PMC_PCR_GCLKCSS_MCK0, ul_div);
			pmc_enable_generic_clk(ID_FLEXCOM5);
			usart_set_clk_source(USART5, USART_SRC_CLK_GCLK);
			/* Enable Flexcom */
			flexcom_enable(FLEXCOM5);
			flexcom_set_opmode(FLEXCOM5, FLEXCOM_USART);
			/* Configure USART */
			usart_init_rs232(p_usart, &usart_settings,
					sysclk_get_peripheral_hz());
			/* Enable the receiver and transmitter. */
			usart_enable_tx(p_usart);
			usart_enable_rx(p_usart);
		} else if (USART6 == p_usart) {
			/* Configure GCLK */
			pmc_configure_generic(ID_FLEXCOM6, PMC_PCR_GCLKCSS_MCK0, ul_div);
			pmc_enable_generic_clk(ID_FLEXCOM6);
			usart_set_clk_source(USART6, USART_SRC_CLK_GCLK);
			/* Enable Flexcom */
			flexcom_enable(FLEXCOM6);
			flexcom_set_opmode(FLEXCOM6, FLEXCOM_USART);
			/* Configure USART */
			usart_init_rs232(p_usart, &usart_settings,
					sysclk_get_peripheral_hz());
			/* Enable the receiver and transmitter. */
			usart_enable_tx(p_usart);
			usart_enable_rx(p_usart);
		} else if (USART7 == p_usart) {
			/* Configure GCLK */
			pmc_configure_generic(ID_FLEXCOM7, PMC_PCR_GCLKCSS_MCK0, ul_div);
			pmc_enable_generic_clk(ID_FLEXCOM7);
			usart_set_clk_source(USART7, USART_SRC_CLK_GCLK);
			/* Enable Flexcom */
			flexcom_enable(FLEXCOM7);
			flexcom_set_opmode(FLEXCOM7, FLEXCOM_USART);
			/* Configure USART */
			usart_init_rs232(p_usart, &usart_settings,
					sysclk_get_peripheral_hz());
			/* Enable the receiver and transmitter. */
			usart_enable_tx(p_usart);
			usart_enable_rx(p_usart);
		}
	}
}

/**
 * \brief Sends a character with the USART.
 *
 * \param p_usart   Base address of the USART instance.
 * \param c       Character to write.
 *
 * \return Status.
 *   \retval 1  The character was written.
 *   \retval 0  The function timed out before the USART transmitter became
 * ready to send.
 */
static inline int usart_serial_putchar(usart_if p_usart, const uint8_t c)
{
	if (UART == (Uart *)p_usart) {
		while (uart_write((Uart *)p_usart, c) != 0) {
		}
		return 1;
	}
	else {
		/* Flexcomm USART */
		if ((USART0 == p_usart) || (USART1 == p_usart) ||
				(USART2 == p_usart) || (USART3 == p_usart) ||
				(USART4 == p_usart) || (USART5 == p_usart) ||
				(USART6 == p_usart) || (USART7 == p_usart)) {
			while (usart_write(p_usart, c) != 0) {
			}
			return 1;
		}
	}

	return 0;
}

/**
 * \brief Waits until a character is received, and returns it.
 *
 * \param p_usart   Base address of the USART instance.
 * \param data   Data to read
 *
 */
static inline void usart_serial_getchar(usart_if p_usart, uint8_t *data)
{
	if (UART == (Uart *)p_usart) {
		while (uart_read((Uart *)p_usart, data)) {
		}
	}
	else {
		uint32_t val = 0;

		/* Flexcomm USART */
		if ((USART0 == p_usart) || (USART1 == p_usart) ||
				(USART2 == p_usart) || (USART3 == p_usart) ||
				(USART4 == p_usart) || (USART5 == p_usart) ||
				(USART6 == p_usart) || (USART7 == p_usart)) {
			while (usart_read(p_usart, &val)) {
			}
			*data = (uint8_t)(val & 0xFF);
		}
	}
}

/**
 * \brief Check if Received data is ready.
 *
 * \param p_usart   Base address of the USART instance.
 *
 * \retval 1 One data has been received.
 * \retval 0 No data has been received.
 */
static inline uint32_t usart_serial_is_rx_ready(usart_if p_usart)
{
	if (UART == (Uart *)p_usart) {
		return uart_is_rx_ready((Uart *)p_usart);
	}
	else {
		/* Flexcomm USART */
		if ((USART0 == p_usart) || (USART1 == p_usart) ||
				(USART2 == p_usart) || (USART3 == p_usart) ||
				(USART4 == p_usart) || (USART5 == p_usart) ||
				(USART6 == p_usart) || (USART7 == p_usart)) {
			return usart_is_rx_ready(p_usart);
		}
	}
	return 0;
}

/**
 * \brief Send a sequence of bytes to a USART device
 *
 * \param usart Base address of the USART instance.
 * \param data   data buffer to write
 * \param len    Length of data
 *
 */
status_code_t usart_serial_write_packet(usart_if usart, const uint8_t *data,
		size_t len);

/**
 * \brief Receive a sequence of bytes to a USART device
 *
 * \param usart Base address of the USART instance.
 * \param data   data buffer to write
 * \param len    Length of data
 *
 */
status_code_t usart_serial_read_packet(usart_if usart, uint8_t *data,
		size_t len);

#endif  /* _UART_SERIAL_H_ */
