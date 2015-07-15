/**
 * \page asfdoc_sam0_dma_basic_use_case Quick Start Guide for Memory to Memory Data Transfer Using DMAC
 *
 * The supported board list:
 *    - SAMD21 Xplained Pro
 *    - SAMR21 Xplained Pro
 *    - SAMD11 Xplained Pro
 *    - SAML21 Xplained Pro
 *
 * In this use case, the DMAC is configured for:
 *  \li Moving data from memory to memory
 *  \li Using software trigger
 *  \li Using DMA priority level 0
 *  \li Transaction as DMA trigger action
 *  \li No action on input events
 *  \li Output event not enabled
 *
 * \section asfdoc_sam0_dma_basic_use_case_setup Setup
 *
 * \subsection asfdoc_sam0_dma_basic_use_casesetup_prereq Prerequisites
 * There are no special setup requirements for this use-case.
 *
 * \subsection asfdoc_sam0_dma_basic_use_casesetup_code Code
 * Copy-paste the following setup code to your user application:
 * \snippet qs_dma_basic.c setup
 *
 * Add the below section to user application initialization (typically the
 * start of \c main()):
 * \snippet qs_dma_basic.c setup_init
 *
 * \subsection asfdoc_sam0_dma_basic_use_casesetup_flow Workflow
 * -# Create a DMA resource configuration structure, which can be filled out to
 *    adjust the configuration of a single DMA transfer.
 *    \snippet qs_dma_basic.c setup_1
 *    \br
 *
 * -# Initialize the DMA resource configuration struct with the module's
 *    default values.
 *    \snippet qs_dma_basic.c setup_2
 *    \note This should always be performed before using the configuration
 *          struct to ensure that all values are initialized to known default
 *          settings.
 *
 * -# Allocate a DMA resource with the configurations.
 *    \snippet qs_dma_basic.c setup_3
 *    \br

 * -# Declare a DMA transfer descriptor configuration structure, which can be
 *    filled out to adjust the configuration of a single DMA transfer.
 *    \snippet qs_dma_basic.c setup_4
 *    \br
 *
 * -# Initialize the DMA transfer descriptor configuration struct with the
 * module's  default values.
 *    \snippet qs_dma_basic.c setup_5
 *    \note This should always be performed before using the configuration
 *          struct to ensure that all values are initialized to known default
 *          settings.
 *
 * -# Set the specific parameters for a DMA transfer with transfer size, source
 *    address, and destination address. In this example, we have enabled the
 *    source and destination address increment.
 *    The source and destination addresses to be stored into descriptor_config
 *    must correspond to the end of the transfer.
 *
 *    \snippet qs_dma_basic.c setup_6
 *    \br
 *
 * -# Create the DMA transfer descriptor.
 *    \snippet qs_dma_basic.c setup_7
 *    \br
 *
 * -# Add the DMA transfer descriptor to the allocated DMA resource.
 *    \snippet qs_dma_basic.c add_descriptor_to_dma_resource
 *    \br
 *
 * -# Register a callback to indicate transfer status.
 *    \snippet qs_dma_basic.c setup_callback_register
 *    \br
 *
 * -# Set the transfer done flag in the registered callback function.
 *    \snippet qs_dma_basic.c _transfer_done
 *    \br
 *
 * -# Enable the registered callbacks.
 *    \snippet qs_dma_basic.c setup_enable_callback
 *    \br
 *
 * \section asfdoc_sam0_dma_basic_use_case_main Use Case
 *
 * \subsection asfdoc_sam0_dma_basic_use_casecode_code Code
 * Add the following code at the start of \c main():
 * \snippet qs_dma_basic.c sample_resource
 * Copy the following code to your user application:
 * \snippet qs_dma_basic.c main
 *
 * \subsection dma_basic_use_case_code_flow Workflow
 * -# Start the DMA transfer job with the allocated DMA resource and
 *    transfer descriptor.
 *    \snippet qs_dma_basic.c main_1
 *
 * -# Set the software trigger for the DMA channel. This can be done before
 *    or after the DMA job is started. Note that all transfers needs a trigger
 *    to start.
 *    \snippet qs_dma_basic.c main_1_1
 *
 * -# Waiting for the setting of the transfer done flag.
 *    \snippet qs_dma_basic.c main_2
 */
/**
* Support and FAQ: visit <a href="http://www.atmel.com/design-support/">Atmel Support</a>
*/
