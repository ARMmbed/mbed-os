/**
 * \file
 *
 * \brief SAM SERCOM I2C Master Interface Driver
 *
 * Copyright (C) 2012-2014 Atmel Corporation. All rights reserved.
 *
 * \asf_license_start
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
 * \page asfdoc_sam0_sercom_i2c_master_callback_use_case Quick Start Guide for SERCOM I2C Master - Callback
 *
 * In this use case, the I<SUP>2</SUP>C will used and set up as follows:
 *  - Master mode
 *  - 100KHz operation speed
 *  - Not operational in standby
 *  - 65535 unknown bus state timeout value
 *
 * \section asfdoc_sam0_sercom_i2c_master_callback_use_case_prereq Prerequisites
 * The device must be connected to an I<SUP>2</SUP>C slave.
 *
 * \section asfdoc_sam0_sercom_i2c_master_callback_use_case_setup Setup
 *
 * \subsection asfdoc_sam0_sercom_i2c_master_callback_use_case_setup_code Code
 * The following must be added to the user application:
 *
 * A sample buffer to write from, a reversed buffer to write from and length of
 * buffers.
 * \snippet qs_i2c_master_callback.c packet_data
 *
 * Address of slave:
 * \snippet qs_i2c_master_callback.c address
 *
 * Globally accessible module structure:
 * \snippet qs_i2c_master_callback.c dev_inst
 *
 * Globally accessible packet:
 * \snippet qs_i2c_master_callback.c packet_glob
 *
 * Function for setting up module:
 * \snippet qs_i2c_master_callback.c initialize_i2c
 *
 * Callback function for write complete:
 * \snippet qs_i2c_master_callback.c callback_func
 *
 * Function for setting up the callback functionality of the driver:
 * \snippet qs_i2c_master_callback.c setup_callback
 *
 * Add to user application \c main():
 * \snippet qs_i2c_master_callback.c run_initialize_i2c
 *
 * \subsection asfdoc_sam0_sercom_i2c_master_callback_use_case_setup_workflow Workflow
 * -# Configure and enable module.
 *    \snippet qs_i2c_master_callback.c config
 *  -# Create and initialize configuration structure.
 *     \snippet qs_i2c_master_callback.c init_conf
 *  -# Change settings in the configuration.
 *     \snippet qs_i2c_master_callback.c conf_change
 *  -# Initialize the module with the set configurations.
 *     \snippet qs_i2c_master_callback.c init_module
 *  -# Enable the module.
 *     \snippet qs_i2c_master_callback.c enable_module
 * -# Configure callback functionality.
 *    \snippet qs_i2c_master_callback.c config_callback
 *  -# Register write complete callback.
 *     \snippet qs_i2c_master_callback.c callback_reg
 *  -# Enable write complete callback.
 *     \snippet qs_i2c_master_callback.c callback_en
 * -# Create a packet to send to slave.
 *    \snippet qs_i2c_master_callback.c write_packet
 *
 * \section asfdoc_sam0_sercom_i2c_master_callback_use_case_implementation Implementation
 * \subsection asfdoc_sam0_sercom_i2c_master_callback_use_case_code Code
 * Add to user application \c main():
 * \snippet qs_i2c_master_callback.c while
 * \subsection asfdoc_sam0_sercom_i2c_master_callback_use_case_implementation_workflow Workflow
 * -# Write packet to slave.
 *    \snippet qs_i2c_master_callback.c write_packet
 * -# Infinite while loop, while waiting for interaction with slave.
 *    \snippet qs_i2c_master_callback.c while
 *
 * \section asfdoc_sam0_sercom_i2c_master_callback_use_case_callback Callback
 * Each time a packet is sent, the callback function will be called.
 *
 * \subsection asfdoc_sam0_sercom_i2c_master_callback_use_case_callback_workflow Workflow
 * - Write complete callback:
 *  -# Send every other packet in reversed order.
 *     \snippet qs_i2c_master_callback.c revert_order
 *  -# Write new packet to slave.
 *     \snippet qs_i2c_master_callback.c write_packet
 *
 */
/**
* Support and FAQ: visit <a href="http://www.atmel.com/design-support/">Atmel Support</a>
*/

#include <asf.h>
#include <conf_clocks.h>

