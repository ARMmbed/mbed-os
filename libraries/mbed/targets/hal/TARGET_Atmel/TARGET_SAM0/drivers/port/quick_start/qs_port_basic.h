/**
 * \page asfdoc_sam0_port_basic_use_case Quick Start Guide for PORT - Basic
 *
 * In this use case, the PORT module is configured for:
 *  \li One pin in input mode, with pull-up enabled
 *  \li One pin in output mode
 *
 * This use case sets up the PORT to read the current state of a GPIO pin set as
 * an input, and mirrors the opposite logical state on a pin configured as an
 * output.
 *
 * \section asfdoc_sam0_port_basic_use_case_setup Setup
 *
 * \subsection asfdoc_sam0_port_basic_use_case_setup_prereq Prerequisites
 * There are no special setup requirements for this use-case.
 *
 * \subsection asfdoc_sam0_port_basic_use_case_setup_code Code
 * Copy-paste the following setup code to your user application:
 * \snippet qs_port_basic.c setup
 *
 * Add to user application initialization (typically the start of \c main()):
 * \snippet qs_port_basic.c setup_init
 *
 * \subsection asfdoc_sam0_port_basic_use_case_setup_flow Workflow
 * -# Create a PORT module pin configuration struct, which can be filled out to
 *    adjust the configuration of a single port pin.
 *    \snippet qs_port_basic.c setup_1
 * -# Initialize the pin configuration struct with the module's default values.
 *    \snippet qs_port_basic.c setup_2
 *    \note This should always be performed before using the configuration
 *          struct to ensure that all values are initialized to known default
 *          settings.
 *
 * -# Adjust the configuration struct to request an input pin.
 *    \snippet qs_port_basic.c setup_3
 * -# Configure push button pin with the initialized pin configuration struct, to enable
 *    the input sampler on the pin.
 *    \snippet qs_port_basic.c setup_4
 * -# Adjust the configuration struct to request an output pin.
 *    \snippet qs_port_basic.c setup_5
 *    \note The existing configuration struct may be re-used, as long as any
 *          values that have been altered from the default settings are taken
 *          into account by the user application.
 *
 * -# Configure LED pin with the initialized pin configuration struct, to enable
 *    the output driver on the pin.
 *    \snippet qs_port_basic.c setup_6
 *
 * \section asfdoc_sam0_port_basic_use_case_use_main Use Case
 *
 * \subsection asfdoc_sam0_port_basic_use_case_code Code
 * Copy-paste the following code to your user application:
 * \snippet qs_port_basic.c main
 *
 * \subsection asfdoc_sam0_port_basic_use_case_flow Workflow
 * -# Read in the current input sampler state of push button pin, which has been
 *    configured as an input in the use-case setup code.
 *    \snippet qs_port_basic.c main_1
 * -# Write the inverted pin level state to LED pin, which has been configured as
 *    an output in the use-case setup code.
 *    \snippet qs_port_basic.c main_2
 */
/**
* Support and FAQ: visit <a href="http://www.atmel.com/design-support/">Atmel Support</a>
*/
