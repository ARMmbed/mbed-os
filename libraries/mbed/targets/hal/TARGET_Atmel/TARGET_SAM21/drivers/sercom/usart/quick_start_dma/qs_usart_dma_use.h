/**
 * \file
 *
 * \brief SAM D21/R21/D10/D11 Quick Start Guide for Using Usart driver with DMA
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
 * \page asfdoc_sam0_sercom_usart_dma_use_case Quick Start Guide for Using DMA with SERCOM USART
 *
 * The supported board list:
 *    - SAML21 Xplained Pro 
 *    - SAMD21 Xplained Pro
 *    - SAMR21 Xplained Pro
 *    - SAMD11 Xplained Pro
 *
 * This quick start will receiving eight bytes of data from PC terminal and transmit back the string
 * to the terminal through DMA. In this use case the USART will be configured with the following
 * settings:
 * - Asynchronous mode
 * - 9600 Baudrate
 * - 8-bits, No Parity and one Stop Bit
 * - TX and RX enabled and connected to the Xplained Pro Embedded Debugger virtual COM port
 *
 * \section asfdoc_sam0_sercom_usart_dma_use_case_setup Setup
 *
 * \subsection asfdoc_sam0_sercom_usart_dma_use_case_prereq Prerequisites
 * There are no special setup requirements for this use-case.
 *
 * \subsection asfdoc_sam0_usart_dma_use_case_setup_code Code
 * Add to the main application source file, outside of any functions:
 * \snippet qs_usart_dma_use.c module_inst
 * \snippet qs_usart_dma_use.c dma_resource
 * \snippet qs_usart_dma_use.c usart_buffer
 * \snippet qs_usart_dma_use.c transfer_descriptor
 *
 * Copy-paste the following setup code to your user application:
 * \snippet qs_usart_dma_use.c setup
 *
 * Add to user application initialization (typically the start of \c main()):
 * \snippet qs_usart_dma_use.c setup_init
 *
 * \subsection asfdoc_sam0_usart_dma_use_case_setup_flow Workflow
 *
 * \subsubsection asfdoc_sam0_usart_dma_use_case_setup_flow_inst Create variables
 * -# Create a module software instance structure for the USART module to store
 *    the USART driver state while it is in use.
 *    \snippet qs_usart_dma_use.c module_inst
 *    \note This should never go out of scope as long as the module is in use.
 *          In most cases, this should be global.
 *
 * -# Create module software instance structures for DMA resources to store
 *    the DMA resource state while it is in use.
 *    \snippet qs_usart_dma_use.c dma_resource
 *    \note This should never go out of scope as long as the module is in use.
 *          In most cases, this should be global.
 *
 * -# Create a buffer to store the data to be transferred /received.
 *    \snippet qs_usart_dma_use.c usart_buffer
 * -# Create DMA transfer descriptors for RX/TX.
 *    \snippet qs_usart_dma_use.c transfer_descriptor
 *
 * \subsubsection asfdoc_sam0_usart_dma_use_case_setup_flow_usart Configure the USART
 * -# Create a USART module configuration struct, which can be filled out to
 *    adjust the configuration of a physical USART peripheral.
 *    \snippet qs_usart_dma_use.c setup_config
 * -# Initialize the USART configuration struct with the module's default values.
 *    \snippet qs_usart_dma_use.c setup_config_defaults
 *    \note This should always be performed before using the configuration
 *          struct to ensure that all values are initialized to known default
 *          settings.
 *
 * -# Alter the USART settings to configure the physical pinout, baudrate, and
 *    other relevant parameters.
 *    \snippet qs_usart_dma_use.c setup_change_config
 * -# Configure the USART module with the desired settings, retrying while the
 *    driver is busy until the configuration is stressfully set.
 *    \snippet qs_usart_dma_use.c setup_set_config
 * -# Enable the USART module.
 *    \snippet qs_usart_dma_use.c setup_enable
 *
 * \subsubsection asfdoc_sam0_usart_dma_use_case_setup_flow_dma Configure DMA
 * -# Create a callback function of receiver done.
 *    \snippet qs_usart_dma_use.c transfer_done_rx
 *
 * -# Create a callback function of transmission done.
 *    \snippet qs_usart_dma_use.c transfer_done_tx
 *
 * -# Create a DMA resource configuration structure, which can be filled out to
 *    adjust the configuration of a single DMA transfer.
 *    \snippet qs_usart_dma_use.c setup_rx_1
 *
 * -# Initialize the DMA resource configuration struct with the module's
 *    default values.
 *    \snippet qs_usart_dma_use.c setup_rx_2
 *    \note This should always be performed before using the configuration
 *          struct to ensure that all values are initialized to known default
 *          settings.
 *
 * -# Set extra configurations for the DMA resource. It is using peripheral
 *    trigger. SERCOM TX empty trigger causes a beat transfer in
 *    this example.
 *    \snippet qs_usart_dma_use.c setup_rx_3
 *
 * -# Allocate a DMA resource with the configurations.
 *    \snippet qs_usart_dma_use.c setup_rx_4
 *
 * -# Create a DMA transfer descriptor configuration structure, which can be
 *    filled out to adjust the configuration of a single DMA transfer.
 *    \snippet qs_usart_dma_use.c setup_rx_5
 *
 * -# Initialize the DMA transfer descriptor configuration struct with the module's
 *    default values.
 *    \snippet qs_usart_dma_use.c setup_rx_6
 *    \note This should always be performed before using the configuration
 *          struct to ensure that all values are initialized to known default
 *          settings.
 *
 * -# Set the specific parameters for a DMA transfer with transfer size, source
 *    address, and destination address.
 *    \snippet qs_usart_dma_use.c setup_rx_7
 *
 * -# Create the DMA transfer descriptor.
 *    \snippet qs_usart_dma_use.c setup_rx_8
 *
 * -# Create a DMA resource configuration structure for TX, which can be filled
 *    out to adjust the configuration of a single DMA transfer.
 *    \snippet qs_usart_dma_use.c setup_tx_1
 *
 * -# Initialize the DMA resource configuration struct with the module's
 *    default values.
 *    \snippet qs_usart_dma_use.c setup_tx_2
 *    \note This should always be performed before using the configuration
 *          struct to ensure that all values are initialized to known default
 *          settings.
 *
 * -# Set extra configurations for the DMA resource. It is using peripheral
 *    trigger. SERCOM RX Ready trigger causes a beat transfer in
 *    this example.
 *    \snippet qs_usart_dma_use.c setup_tx_3
 *
 * -# Allocate a DMA resource with the configurations.
 *    \snippet qs_usart_dma_use.c setup_tx_4
 *
 * -# Create a DMA transfer descriptor configuration structure, which can be
 *    filled out to adjust the configuration of a single DMA transfer.
 *    \snippet qs_usart_dma_use.c setup_tx_5
 *
 * -# Initialize the DMA transfer descriptor configuration struct with the module's
 *    default values.
 *    \snippet qs_usart_dma_use.c setup_tx_6
 *    \note This should always be performed before using the configuration
 *          struct to ensure that all values are initialized to known default
 *          settings.
 *
 * -# Set the specific parameters for a DMA transfer with transfer size, source
 *    address, and destination address.
 *    \snippet qs_usart_dma_use.c setup_tx_7
 *
 * -# Create the DMA transfer descriptor.
 *    \snippet qs_usart_dma_use.c setup_tx_8
 *
 * \section asfdoc_sam0_usart_dma_use_case_main Use Case
 *
 * \subsection asfdoc_sam0_usart_dma_use_case_main_code Code
 * Copy-paste the following code to your user application:
 * \snippet qs_usart_dma_use.c main
 *
 * \subsection asfdoc_sam0_usart_dma_use_case_main_flow Workflow
 * -# Wait for receiving data.
 *    \snippet qs_usart_dma_use.c main_1
 *
 * -# Enter endless loop.
 *    \snippet qs_usart_dma_use.c endless_loop
 */
 /**
 * Support and FAQ: visit <a href="http://www.atmel.com/design-support/">Atmel Support</a>
 */
