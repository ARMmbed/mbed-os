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

