/**
 * \page asfdoc_sam0_sercom_i2c_slave_basic_use_case Quick Start Guide for SERCOM I2C Slave - Basic
 *
 * In this use case, the I<SUP>2</SUP>C will used and set up as follows:
 *  - Slave mode
 *  - 100KHz operation speed
 *  - Not operational in standby
 *  - 10000 packet timeout value
 *
 * \section asfdoc_sam0_sercom_i2c_slave_basic_use_case_prereq Prerequisites
 * The device must be connected to an I<SUP>2</SUP>C master.
 *
 * \section asfdoc_sam0_sercom_i2c_slave_basic_use_case_setup_code Setup
 *
 * \subsection asfdoc_sam0_sercom_i2c_slave_basic_use_setup_code Code
 * The following must be added to the user application:
 *
 * A sample buffer to write from, a sample buffer to read to and length of buffers:
 * \snippet qs_i2c_slave_basic_use.c packet_data
 *
 * Address to respond to:
 * \snippet qs_i2c_slave_basic_use.c address
 *
 * Globally accessible module structure:
 * \snippet qs_i2c_slave_basic_use.c module
 *
 * Function for setting up the module:
 * \snippet qs_i2c_slave_basic_use.c initialize_i2c
 *
 * Add to user application \c main():
 * \snippet qs_i2c_slave_basic_use.c run_initialize_i2c
 *
 * \subsection asfdoc_sam0_sercom_i2c_slave_basic_use_setup_workflow Workflow
 * -# Configure and enable module.
 *    \snippet qs_i2c_slave_basic_use.c config
 *   -# Create and initialize configuration structure.
 *      \snippet qs_i2c_slave_basic_use.c init_conf
 *   -# Change address and address mode settings in the configuration.
 *      \snippet qs_i2c_slave_basic_use.c conf_changes
 *   -# Initialize the module with the set configurations.
 *      \snippet qs_i2c_slave_basic_use.c init_module
 *   -# Enable the module.
 *      \snippet qs_i2c_slave_basic_use.c enable_module
 * -# Create variable to hold transfer direction.
 *    \snippet qs_i2c_slave_basic_use.c dir
 * -# Create packet variable to transfer.
 *    \snippet qs_i2c_slave_basic_use.c pack
 *
 * \section asfdoc_sam0_sercom_i2c_slave_basic_use_implementation Implementation
 * \subsection asfdoc_sam0_sercom_i2c_slave_basic_use_implementation_code Code
 * Add to user application \c main():
 * \snippet qs_i2c_slave_basic_use.c while
 * \subsection i2c_slave_basic_use_implementation_workflow Workflow
 * -# Wait for start condition from master and get transfer direction.
 *    \snippet qs_i2c_slave_basic_use.c get_dir
 * -# Depending on transfer direction, set up buffer to read to or write from,
 *    and write or read from master.
 *    \snippet qs_i2c_slave_basic_use.c transfer
 */
/**
* Support and FAQ: visit <a href="http://www.atmel.com/design-support/">Atmel Support</a>
*/

#include <asf.h>
#include <conf_clocks.h>

