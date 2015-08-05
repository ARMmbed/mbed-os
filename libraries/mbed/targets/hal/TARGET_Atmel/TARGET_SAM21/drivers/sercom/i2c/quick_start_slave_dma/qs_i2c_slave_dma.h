/**
 * \page asfdoc_sam0_sercom_i2c_slave_dma_use_case Quick Start Guide for Using DMA with SERCOM I2C Slave
 *
 * The supported board list:
 *    - SAMD21 Xplained Pro
 *    - SAMR21 Xplained Pro
 *    - SAML21 Xplained Pro
 *
 * In this use case, the I<SUP>2</SUP>C will used and set up as follows:
 *  - Slave mode
 *  - 100KHz operation speed
 *  - Not operational in standby
 *  - 65535 unknown bus state timeout value
 *
 *
 * \section asfdoc_sam0_sercom_i2c_slave_dma_use_case_prereq Prerequisites
 * The device must be connected to an I<SUP>2</SUP>C slave.
 *
 * \section asfdoc_sam0_sercom_i2c_slave_dma_use_setup Setup
 *
 * \subsection asfdoc_sam0_sercom_i2c_slave_dma_use_setup_code Code
 * The following must be added to the user application:
 *
 * - Address to respond to:
 * \snippet qs_i2c_slave_dma.c address
 *
 * - A sample buffer to send, number of entries to send and address of slave:
 * \snippet qs_i2c_slave_dma.c packet_data
 *
 * - Globally accessible module structure:
 * \snippet qs_i2c_slave_dma.c module
 *
 * - Function for setting up the module:
 * \snippet qs_i2c_slave_dma.c initialize_i2c
 *
 * - Globally accessible DMA module structure:
 * \snippet qs_i2c_slave_dma.c dma_resource
 *
 * - Globally accessible DMA transfer descriptor:
 * \snippet qs_i2c_slave_dma.c transfer_descriptor
 *
 * - Function for setting up the DMA resource:
 * \snippet qs_i2c_slave_dma.c config_dma_resource
 *
 * - Function for setting up the DMA transfer descriptor:
 * \snippet qs_i2c_slave_dma.c setup_dma_transfer_descriptor
 *
 * - Add to user application \c main():
 * \snippet qs_i2c_slave_dma.c init
 *
 * \subsection asfdoc_sam0_sercom_i2c_slave_dma_use_setup_workflow Workflow
 * \subsubsection asfdoc_sam0_sercom_i2c_slave_dma_use_setup_workflow_i2c Configure and enable SERCOM:
 * \snippet qs_i2c_slave_dma.c initialize_i2c
 * -# Create and initialize configuration structure.
 *    \snippet qs_i2c_slave_dma.c init_conf
 * -# Change settings in the configuration.
 *    \snippet qs_i2c_slave_dma.c conf_changes
 * -# Initialize the module with the set configurations.
 *    \snippet qs_i2c_slave_dma.c init_module
 * -# Enable the module.
 *    \snippet qs_i2c_slave_dma.c enable_module
 *
 * \subsubsection asfdoc_sam0_sercom_i2c_slave_dma_use_setup_workflow_dma Configure DMA
 * -# Create a DMA resource configuration structure, which can be filled out to
 *    adjust the configuration of a single DMA transfer.
 *    \snippet qs_i2c_slave_dma.c dma_setup_1
 *
 * -# Initialize the DMA resource configuration struct with the module's
 *    default values.
 *    \snippet qs_i2c_slave_dma.c dma_setup_2
 *    \note This should always be performed before using the configuration
 *          struct to ensure that all values are initialized to known default
 *          settings.
 *
 * -# Set extra configurations for the DMA resource. It is using peripheral
 *    trigger. SERCOM RX trigger causes a beat transfer in this
 *    example.
 *    \snippet qs_i2c_slave_dma.c dma_setup_3
 *
 * -# Allocate a DMA resource with the configurations.
 *    \snippet qs_i2c_slave_dma.c dma_setup_4
 *
 * -# Create a DMA transfer descriptor configuration structure, which can be
 *    filled out to adjust the configuration of a single DMA transfer.
 *    \snippet qs_i2c_slave_dma.c dma_setup_5
 *
 * -# Initialize the DMA transfer descriptor configuration struct with the module's
 *    default values.
 *    \snippet qs_i2c_slave_dma.c dma_setup_6
 *    \note This should always be performed before using the configuration
 *          struct to ensure that all values are initialized to known default
 *          settings.
 *
 * -# Set the specific parameters for a DMA transfer with transfer size, source
 *    address, and destination address.
 *    \snippet qs_i2c_slave_dma.c dma_setup_7
 *
 * -# Create the DMA transfer descriptor.
 *    \snippet qs_i2c_slave_dma.c dma_setup_8
 *
 * \section asfdoc_sam0_sercom_i2c_slave_dma_use_implemenation Implementation
 * \subsection asfdoc_sam0_sercom_i2c_slave_dma_use_implemenation_code Code
 * Add to user application \c main():
 * \snippet qs_i2c_slave_dma.c main
 *
 * \subsection asfdoc_sam0_sercom_i2c_slave_dma_use_implemenation_workflow Workflow
 * -# Start to wait a packet from master.
 *    \snippet qs_i2c_slave_dma.c wait_packet
 *
 * -# Once data ready, clear the address match status.
 *    \snippet qs_i2c_slave_dma.c clear_status
 *
 */
/**
* Support and FAQ: visit <a href="http://www.atmel.com/design-support/">Atmel Support</a>
*/
