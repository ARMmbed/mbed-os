/**
 * \file
 *
 * \brief SAM D21/R21/L21 Quick Start Guide for Using SPI driver with DMA
 *
 * Copyright (C) 2014 Atmel Corporation. All rights reserved.
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
 * \page asfdoc_sam0_sercom_spi_dma_use_case Quick Start Guide for Using DMA with SERCOM SPI
 *
 * The supported board list:
 *    - SAMD21 Xplained Pro
 *    - SAMR21 Xplained Pro
 *    - SAML21 Xplained Pro
 *
 * This quick start will transmit a buffer data from master to slave through DMA.
 * In this use case the SPI master will be configured with the following
 * settings on SAM Xplained Pro:
 * - Master Mode enabled
 * - MSB of the data is transmitted first
 * - Transfer mode 0
 * - 8-bit character size
 * - Not enabled in sleep mode
 * - Baudrate 100000
 * - GLCK generator 0
 *
 * The SPI slave will be configured with the following settings:
 * - Slave mode enabled
 * - Preloading of shift register enabled
 * - MSB of the data is transmitted first
 * - Transfer mode 0
 * - 8-bit character size
 * - Not enabled in sleep mode
 * - GLCK generator 0
 *
 * Note that the pinouts on other boards may different, see next sector for
 * details.
 *
 * \section asfdoc_sam0_sercom_spi_dma_use_case_setup Setup
 *
 * \subsection asfdoc_sam0_sercom_spi_dma_use_case_prereq Prerequisites
 * The following connections has to be made using wires:
 * - SAM D21 Xplained Pro.
 *  - \b SS_0:  EXT1 PIN15 (PA05) <--> EXT2 PIN15 (PA17)
 *  - \b DO/DI: EXT1 PIN16 (PA06) <--> EXT2 PIN17 (PA16)
 *  - \b DI/DO: EXT1 PIN17 (PA04) <--> EXT2 PIN16 (PA18)
 *  - \b SCK:   EXT1 PIN18 (PA07) <--> EXT2 PIN18 (PA19)
 * - SAM R21 Xplained Pro.
 *  - \b SS_0:  EXT1 PIN15 (PB03) <--> EXT1 PIN10 (PA23)
 *  - \b DO/DI: EXT1 PIN16 (PB22) <--> EXT1 PIN9  (PA22)
 *  - \b DI/DO: EXT1 PIN17 (PB02) <--> EXT1 PIN7  (PA18)
 *  - \b SCK:   EXT1 PIN18 (PB23) <--> EXT1 PIN8  (PA19)
 * - SAM L21 Xplained Pro.
 *  - \b SS_0:  EXT1 PIN15 (PA05) <--> EXT1 PIN12 (PA09)
 *  - \b DO/DI: EXT1 PIN16 (PA06) <--> EXT1 PIN11 (PA08)
 *  - \b DI/DO: EXT1 PIN17 (PA04) <--> EXT2 PIN03 (PA10)
 *  - \b SCK:   EXT1 PIN18 (PA07) <--> EXT2 PIN04 (PA11)
 *
 * \subsection asfdoc_sam0_spi_dma_use_case_setup_code Code
 *
 * Add to the main application source file, before user definitions and
 * functions according to your board:
 *
 * For SAMD21 Xplained Pro:
 * \snippet samd21_xplained_pro/conf_quick_start.h definition_master
 * \snippet samd21_xplained_pro/conf_quick_start.h definition_slave
 * \snippet samd21_xplained_pro/conf_quick_start.h definition_peripheral_trigger
 * For SAMR21 Xplained Pro:
 * \snippet samr21_xplained_pro/conf_quick_start.h definition_master
 * \snippet samr21_xplained_pro/conf_quick_start.h definition_slave
 * \snippet samr21_xplained_pro/conf_quick_start.h definition_peripheral_trigger
 * For SAML21 Xplained Pro:
 * \snippet saml21_xplained_pro/conf_quick_start.h definition_master
 * \snippet saml21_xplained_pro/conf_quick_start.h definition_slave
 * \snippet saml21_xplained_pro/conf_quick_start.h definition_peripheral_trigger

 * Add to the main application source file, outside of any functions:
 * \snippet qs_spi_dma_use.c buf_length
 * \snippet qs_spi_dma_use.c spi_baudrate
 * \snippet qs_spi_dma_use.c slave_select_pin
 * \snippet qs_spi_dma_use.c spi_buffer
 * \snippet qs_spi_dma_use.c spi_module_inst
 * \snippet qs_spi_dma_use.c dma_transfer_done_flag
 * \snippet qs_spi_dma_use.c slave_dev_inst
 * \snippet qs_spi_dma_use.c dma_transfer_descriptor
 *
 * Copy-paste the following setup code to your user application:
 * \snippet qs_spi_dma_use.c setup
 *
 * Add to user application initialization (typically the start of \c main()):
 * \snippet qs_spi_dma_use.c setup_init
 *
 * \subsection asfdoc_sam0_spi_dma_use_case_setup_flow Workflow
 * -# Create a module software instance structure for the SPI module to store
 *    the SPI driver state while it is in use.
 *    \snippet qs_spi_dma_use.c spi_module_inst
 *    \note This should never go out of scope as long as the module is in use.
 *          In most cases, this should be global.
 *
 * -# Create a module software instance structure for DMA resource to store
 *    the DMA resource state while it is in use.
 *    \snippet qs_spi_dma_use.c dma_resource
 *    \note This should never go out of scope as long as the module is in use.
 *          In most cases, this should be global.
 *
 * -# Create transfer done flag to indication DMA transfer done.
 *    \snippet qs_spi_dma_use.c dma_transfer_done_flag
 * -# Define the buffer length for TX/RX.
 *    \snippet qs_spi_dma_use.c buf_length
 * -# Create buffer to store the data to be transferred.
 *    \snippet qs_spi_dma_use.c spi_buffer
 * -# Create SPI module configuration struct, which can be filled out to
 *     adjust the configuration of a physical SPI peripheral.
 *     \snippet qs_spi_dma_use.c spi_master_config
 *     \snippet qs_spi_dma_use.c spi_slave_config
 * -# Initialize the SPI configuration struct with the module's default values.
 *     \snippet qs_spi_dma_use.c spi_master_conf_defaults
 *     \snippet qs_spi_dma_use.c spi_slave_conf_defaults
 *     \note This should always be performed before using the configuration
 *           struct to ensure that all values are initialized to known default
 *           settings.
 *
 * -# Alter the SPI settings to configure the physical pinout, baudrate and
 *     other relevant parameters.
 *     \snippet qs_spi_dma_use.c spi_master_mux_setting
 *     \snippet qs_spi_dma_use.c spi_slave_mux_setting
 * -# Configure the SPI module with the desired settings, retrying while the
 *     driver is busy until the configuration is stressfully set.
 *     \snippet qs_spi_dma_use.c spi_master_init
 *     \snippet qs_spi_dma_use.c spi_slave_init
 * -# Enable the SPI module.
 *     \snippet qs_spi_dma_use.c spi_master_enable
 *     \snippet qs_spi_dma_use.c spi_slave_enable
 *
 * -# Create DMA resource configuration structure, which can be filled out to
 *    adjust the configuration of a single DMA transfer.
 *    \snippet qs_spi_dma_use.c dma_tx_setup_1
 *    \snippet qs_spi_dma_use.c dma_rx_setup_1
 *
 * -# Initialize the DMA resource configuration struct with the module's
 *    default values.
 *    \snippet qs_spi_dma_use.c dma_tx_setup_2
 *    \snippet qs_spi_dma_use.c dma_rx_setup_2
 *    \note This should always be performed before using the configuration
 *          struct to ensure that all values are initialized to known default
 *          settings.
 *
 * -# Set extra configurations for the DMA resource. It is using peripheral
 *    trigger. SERCOM TX empty and RX complete trigger causes a beat transfer in
 *    this example.
 *    \snippet qs_spi_dma_use.c dma_tx_setup_3
 *    \snippet qs_spi_dma_use.c dma_rx_setup_3
 *
 * -# Allocate a DMA resource with the configurations.
 *    \snippet qs_spi_dma_use.c dma_tx_setup_4
 *    \snippet qs_spi_dma_use.c dma_rx_setup_4
 *
 * -# Create a DMA transfer descriptor configuration structure, which can be
 *    filled out to adjust the configuration of a single DMA transfer.
 *    \snippet qs_spi_dma_use.c dma_tx_setup_5
 *    \snippet qs_spi_dma_use.c dma_rx_setup_5
 *
 * -# Initialize the DMA transfer descriptor configuration struct with the module's
 *    default values.
 *    \snippet qs_spi_dma_use.c dma_tx_setup_6
 *    \snippet qs_spi_dma_use.c dma_rx_setup_6
 *    \note This should always be performed before using the configuration
 *          struct to ensure that all values are initialized to known default
 *          settings.
 *
 * -# Set the specific parameters for a DMA transfer with transfer size, source
 *    address, and destination address.
 *    \snippet qs_spi_dma_use.c dma_tx_setup_7
 *    \snippet qs_spi_dma_use.c dma_rx_setup_7
 *
 * -# Create the DMA transfer descriptor.
 *    \snippet qs_spi_dma_use.c dma_tx_setup_8
 *    \snippet qs_spi_dma_use.c dma_rx_setup_8
 *
 * \section asfdoc_sam0_spi_dma_use_case_main Use Case
 *
 * \subsection asfdoc_sam0_spi_dma_use_case_main_code Code
 * Copy-paste the following code to your user application:
 * \snippet qs_spi_dma_use.c main
 *
 * \subsection asfdoc_sam0_spi_dma_use_case_main_flow Workflow
 * -# Select the slave.
 *    \snippet qs_spi_dma_use.c select_slave
 *
 * -# Start the transfer job.
 *    \snippet qs_spi_dma_use.c main_1
 *
 * -# Wait for transfer done.
 *    \snippet qs_spi_dma_use.c main_2
 *
 * -# Deselect the slave.
 *    \snippet qs_spi_dma_use.c deselect_slave
 *
 * -# Enter endless loop.
 *    \snippet qs_spi_dma_use.c endless_loop
 */
 /**
 * Support and FAQ: visit <a href="http://www.atmel.com/design-support/">Atmel Support</a>
 */
