/**
 * \file
 *
 * \brief SAM SPI Quick Start
 *
 * Copyright (C) 2012-2014 Atmel Corporation. All rights reserved.
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

/**
 * \page asfdoc_sam0_sercom_spi_master_basic_use Quick Start Guide for SERCOM SPI Master - Polled
 *
 * In this use case, the SPI on extension header 1 of the Xplained Pro board
 * will configured with the following settings:
 * - Master Mode enabled
 * - MSB of the data is transmitted first
 * - Transfer mode 0
 * - 8-bit character size
 * - Not enabled in sleep mode
 * - Baudrate 100000
 * - GLCK generator 0
 *
 *
 * \section asfdoc_sam0_sercom_spi_master_basic_use_setup Setup
 *
 * \subsection asfdoc_sam0_sercom_spi_master_basic_use_prereq Prerequisites
 * There are no special setup requirements for this use-case.
 *
 * \subsection asfdoc_sam0_sercom_spi_master_basic_use_setup_code Code
 * The following must be added to the user application:
 *
 * A sample buffer to send via SPI.
 * \snippet qs_spi_master_basic.c buffer
 * Number of entries in the sample buffer.
 * \snippet qs_spi_master_basic.c buf_length
 * GPIO pin to use as Slave Select.
 * \snippet qs_spi_master_basic.c slave_select_pin
 * A globally available software device instance struct to store the SPI driver
 * state while it is in use.
 * \snippet qs_spi_master_basic.c dev_inst
 * A globally available peripheral slave software device instance struct.
 * \snippet qs_spi_master_basic.c slave_dev_inst
 * A function for configuring the SPI.
 * \snippet qs_spi_master_basic.c configure_spi
 *
 * Add to user application \c main().
 * \snippet qs_spi_master_basic.c main_setup
 *
 * \section asfdoc_sam0_sercom_spi_master_basic_use_workflow Workflow
 * -# Initialize system.
 *    \snippet qs_spi_master_basic.c system_init
 * -# Setup the SPI.
 *    \snippet qs_spi_master_basic.c run_config
 *   -# Create configuration struct.
 *      \snippet qs_spi_master_basic.c config
 *   -# Create peripheral slave configuration struct.
 *      \snippet qs_spi_master_basic.c slave_config
 *   -# Create peripheral slave software device instance struct.
 *      \snippet qs_spi_master_basic.c slave_dev_inst
 *   -# Get default peripheral slave configuration.
 *      \snippet qs_spi_master_basic.c slave_conf_defaults
 *   -# Set Slave Select pin.
 *      \snippet qs_spi_master_basic.c ss_pin
 *   -# Initialize peripheral slave software instance with configuration.
 *      \snippet qs_spi_master_basic.c slave_init
 *   -# Get default configuration to edit.
 *      \snippet qs_spi_master_basic.c conf_defaults
 *   -# Set MUX setting E.
 *      \snippet qs_spi_master_basic.c mux_setting
 *   -# Set pinmux for pad 0 (data in (MISO)).
 *      \snippet qs_spi_master_basic.c di
 *   -# Set pinmux for pad 1 as unused, so the pin can be used for other purposes.
 *      \snippet qs_spi_master_basic.c ss
 *   -# Set pinmux for pad 2 (data out (MOSI)).
 *      \snippet qs_spi_master_basic.c do
 *   -# Set pinmux for pad 3 (SCK).
 *      \snippet qs_spi_master_basic.c sck
 *   -# Initialize SPI module with configuration.
 *      \snippet qs_spi_master_basic.c init
 *   -# Enable SPI module.
 *      \snippet qs_spi_master_basic.c enable
 *
 * \section asfdoc_sam0_sercom_spi_master_basic_use_case Use Case
 * \subsection asfdoc_sam0_sercom_spi_master_basic_use_case_code Code
 * Add the following to your user application \c main().
 * \snippet qs_spi_master_basic.c main_use_case
 * \subsection asfdoc_sam0_sercom_spi_master_basic_use_case_workflow Workflow
 * -# Select slave.
 *    \snippet qs_spi_master_basic.c select_slave
 * -# Write buffer to SPI slave.
 *    \snippet qs_spi_master_basic.c write
 * -# Deselect slave.
 *    \snippet qs_spi_master_basic.c deselect_slave
 * -# Infinite loop.
 *    \snippet qs_spi_master_basic.c inf_loop
 */
 /**
 * Support and FAQ: visit <a href="http://www.atmel.com/design-support/">Atmel Support</a>
 */
