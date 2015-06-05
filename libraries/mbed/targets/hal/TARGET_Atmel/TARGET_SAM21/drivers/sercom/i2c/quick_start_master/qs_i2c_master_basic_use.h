/**
 * \file
 *
 * \brief SAM SERCOM I2C Master Quick Start Guide
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
 * \page asfdoc_sam0_sercom_i2c_master_basic_use_case Quick Start Guide for SERCOM I2C Master - Basic
 *
 * In this use case, the I<SUP>2</SUP>C will used and set up as follows:
 *  - Master mode
 *  - 100KHz operation speed
 *  - Not operational in standby
 *  - 10000 packet timeout value
 *  - 65535 unknown bus state timeout value
 *
 *
 * \section asfdoc_sam0_sercom_i2c_master_basic_use_case_prereq Prerequisites
 * The device must be connected to an I<SUP>2</SUP>C slave.
 *
 * \section asfdoc_sam0_sercom_i2c_master_basic_use_setup Setup
 *
 * \subsection asfdoc_sam0_sercom_i2c_master_basic_use_setup_code Code
 * The following must be added to the user application:
 *
 * - A sample buffer to send, a sample buffer to read:
 * \snippet qs_i2c_master_basic_use.c packet_data
 *
 * - Slave address to access:
 * \snippet qs_i2c_master_basic_use.c address
 *
 * - Number of times to try to send packet if it fails:
 * \snippet qs_i2c_master_basic_use.c timeout
 *
 * - Globally accessible module structure:
 * \snippet qs_i2c_master_basic_use.c dev_inst
 *
 * - Function for setting up the module:
 * \snippet qs_i2c_master_basic_use.c initialize_i2c
 *
 * - Add to user application \c main():
 * \snippet qs_i2c_master_basic_use.c init
 *
 * \subsection asfdoc_sam0_sercom_i2c_master_basic_use_setup_workflow Workflow
 * -# Configure and enable module.
 *    \snippet qs_i2c_master_basic_use.c initialize_i2c
 *   -# Create and initialize configuration structure.
 *      \snippet qs_i2c_master_basic_use.c init_conf
 *   -# Change settings in the configuration.
 *      \snippet qs_i2c_master_basic_use.c conf_change
 *   -# Initialize the module with the set configurations.
 *      \snippet qs_i2c_master_basic_use.c init_module
 *   -# Enable the module.
 *      \snippet qs_i2c_master_basic_use.c enable_module
 * -# Create a variable to see when we should stop trying to send packet.
 *    \snippet qs_i2c_master_basic_use.c timeout_counter
 * -# Create a packet to send.
 *    \snippet qs_i2c_master_basic_use.c packet
 *
 * \section asfdoc_sam0_sercom_i2c_master_basic_use_implemenation Implementation
 * \subsection asfdoc_sam0_sercom_i2c_master_basic_use_implemenation_code Code
 * Add to user application \c main():
 * \snippet qs_i2c_master_basic_use.c main
 *
 * \subsection asfdoc_sam0_sercom_i2c_master_basic_use_implemenation_workflow Workflow
 * -# Write packet to slave.
 *    \snippet qs_i2c_master_basic_use.c write_packet
 * The module will try to send the packet TIMEOUT number of times or until it is
 * successfully sent.
 * -# Read packet from slave.
 *    \snippet qs_i2c_master_basic_use.c read_packet
 * The module will try to read the packet TIMEOUT number of times or until it is
 * successfully read.
 */
 /**
 * Support and FAQ: visit <a href="http://www.atmel.com/design-support/">Atmel Support</a>
 */

#include <asf.h>
#include <conf_clocks.h>

