/**
 * \page asfdoc_sam0_tcc_dma_use_case Quick Start Guide for Using DMA with TCC
 *
 * The supported board list:
 *    - SAM D21/R21/L21 Xplained Pro
 *
 * In this use case, the TCC will be used to generate a PWM signal. Here
 * the pulse width varies through following values with the help of DMA
 * transfer: one quarter of the period, half of the period, and three quarters
 * of the period.
 * The PWM output can be used to drive an LED. The waveform can also be
 * viewed using an oscilloscope.
 * The output signal is also fed back to another TCC channel by event system,
 * the event stamps are captured and transferred to a buffer by DMA.
 *
 * The PWM output is set up as follows:
 * <table>
 *  <tr><th> Board        </td><th> Pin  </td><th> Connect to </td></tr>
 *  <tr><td> SAMD21 Xpro  </td><td> PB30 </td><td> LED0       </td></tr>
 *  <tr><td> SAMR21 Xpro  </td><td> PA19 </td><td> LED0       </td></tr>
 *  <tr><td> SAML21 Xpro  </td><td> PB10 </td><td> LED0       </td></tr>
 * </table>
 *
 * The TCC module will be setup as follows:
 * - GCLK generator 0 (GCLK main) clock source
 * - Use double buffering write when set top, compare, or pattern through API
 * - No dithering on the counter or compare
 * - No prescaler
 * - Single Slope PWM wave generation
 * - GCLK reload action
 * - Don't run in standby
 * - No fault or waveform extensions
 * - No inversion of waveform output
 * - No capture enabled
 * - Count upward
 * - Don't perform one-shot operations
 * - Counter starts on 0
 * - Counter top set to 0x1000
 * - Channel 0 (on SAM D21 Xpro) or 3 (on SAM R21 Xpro) is set to
 *   compare and match value 0x1000*3/4 and generate event
 * - Channel 1 is set to capture on input event
 *
 * The event resource of EVSYS module will be setup as follows:
 * - TCC match capture channel 0 (on SAM D21 Xpro) or 3 (on SAM R21 Xpro) is
 *   selected as event generator
 * - Event generation is synchronous, with rising edge detected
 * - TCC match capture channel 1 is the event user
 *
 * The DMA resource of DMAC module will be setup as follows:
 * - Two DMA resources are used
 * - Both DMA resources use peripheral trigger
 * - Both DMA resources perform beat transfer on trigger
 * - Both DMA resources use beat size of 16 bits
 * - Both DMA resources are configured to transfer three beats and
 *   then repeat again in same buffer
 * - On DMA resource which controls the compare value
 *   - TCC0 overflow triggers DMA transfer
 *   - The source address increment is enabled
 *   - The destination address is fixed to TCC channel 0 Compare/Capture
 *register
 * - On DMA resource which reads the captured value
 *   - TCC0 capture on channel 1 triggers DMA transfer
 *   - The source address is fixed to TCC channel 1 Compare/Capture register
 *   - The destination address increment is enabled
 *   - The captured value is transferred to an array in SRAM
 *
 * \section asfdoc_sam0_tcc_dma_use_case_setup Quick Start
 *
 * \subsection asfdoc_sam0_tcc_dma_use_case_prereq Prerequisites
 * There are no prerequisites for this use case.
 *
 * \subsection asfdoc_sam0_tcc_dma_use_case_setup_code Code
 *
 * Add to the main application source file, before any functions, according to
 * the kit used:
 * - SAM D21 Xplained Pro.
 * \snippet samd21_xplained_pro/conf_quick_start_dma.h definition_pwm
 * \snippet samd21_xplained_pro/conf_quick_start_dma.h definition_feedback
 * \snippet samd21_xplained_pro/conf_quick_start_dma.h definition_dma_compare_trigger
 * \snippet samd21_xplained_pro/conf_quick_start_dma.h definition_dma_capture_trigger
 * - SAM R21 Xplained Pro.
 * \snippet samr21_xplained_pro/conf_quick_start_dma.h definition_pwm
 * \snippet samr21_xplained_pro/conf_quick_start_dma.h definition_feedback
 * \snippet samr21_xplained_pro/conf_quick_start_dma.h definition_dma_compare_trigger
 * \snippet samr21_xplained_pro/conf_quick_start_dma.h definition_dma_capture_trigger
 * - SAM L21 Xplained Pro.
 * \snippet saml21_xplained_pro/conf_quick_start_dma.h definition_pwm
 * \snippet saml21_xplained_pro/conf_quick_start_dma.h definition_feedback
 * \snippet saml21_xplained_pro/conf_quick_start_dma.h definition_dma_compare_trigger
 *
 * Add to the main application source file, outside of any functions:
 * \snippet qs_tcc_dma.c module_inst
 * \snippet qs_tcc_dma.c capture_variables
 * \snippet qs_tcc_dma.c compare_variables
 *
 * Copy-paste the following setup code to your user application:
 * \snippet qs_tcc_dma.c config_event_for_capture
 * \snippet qs_tcc_dma.c config_dma_for_capture
 * \snippet qs_tcc_dma.c config_dma_for_wave
 * \snippet qs_tcc_dma.c setup
 *
 * Add to user application initialization (typically the start of \c main()):
 * \snippet qs_tcc_dma.c setup_init
 *
 * \subsection asfdoc_sam0_tcc_dma_use_case_setup_flow Workflow
 * \subsubsection asfdoc_sam0_tcc_dma_use_case_setup_flow_tcc Configure the TCC
 * -# Create a module software instance structure for the TCC module to store
 *    the TCC driver state while it is in use.
 *    \snippet qs_tcc_dma.c module_inst
 *    \note This should never go out of scope as long as the module is in use.
 *          In most cases, this should be global.
 * -# Create a TCC module configuration struct, which can be filled out to
 *     adjust the configuration of a physical TCC peripheral.
 *     \snippet qs_tcc_dma.c setup_config
 * -# Initialize the TCC configuration struct with the module's default values.
 *     \snippet qs_tcc_dma.c setup_config_defaults
 *     \note This should always be performed before using the configuration
 *           struct to ensure that all values are initialized to known default
 *           settings.
 * -# Alter the TCC settings to configure the counter width, wave generation
 *     mode and the compare channel 0 value.
 *     \snippet qs_tcc_dma.c setup_change_config
 * -# Alter the TCC settings to configure the PWM output on a physical device
 *     pin.
 *     \snippet qs_tcc_dma.c setup_change_config_pwm
 * -# Configure the TCC module with the desired settings.
 *     \snippet qs_tcc_dma.c setup_set_config
 * -# Configure and enable the desired events for the TCC module.
 *     \snippet qs_tcc_dma.c setup_events
 * \subsubsection asfdoc_sam0_tcc_dma_use_case_setup_flow_event Configure the Event System
 * Configure the EVSYS module to wire channel 0 event to channel 1.
 * -# Create an event resource instance.
 *     \snippet qs_tcc_dma.c capture_event_resource
 *     \note This should never go out of scope as long as the resource is in
 *     use. In most cases, this should be global.
 *
 * -# Create an event resource configuration struct.
 *      \snippet qs_tcc_dma.c event_setup_1
 * -# Initialize the event resource configuration struct with default values.
 *      \snippet qs_tcc_dma.c event_setup_2
 *      \note This should always be performed before using the configuration
 *            struct to ensure that all values are initialized to known default
 *            settings.
 * -# Adjust the event resource configuration to desired values.
 *      \snippet qs_tcc_dma.c event_setup_3
 * -# Allocate and configure the resource using the configuration structure.
 *      \snippet qs_tcc_dma.c event_setup_4
 * -# Attach a user to the resource.
 *      \snippet qs_tcc_dma.c event_setup_5
 * \subsubsection asfdoc_sam0_tcc_dma_use_case_setup_flow_dma_capture Configure the DMA for Capture TCC Channel 1
 * Configure the DMAC module to obtain captured value from TCC channel 1.
 * -# Create a DMA resource instance.
 *     \snippet qs_tcc_dma.c capture_dma_resource
 *     \note This should never go out of scope as long as the resource is in
 *             use. In most cases, this should be global.
 * -# Create a DMA resource configuration struct.
 *       \snippet qs_tcc_dma.c dma_setup_1
 * -# Initialize the DMA resource configuration struct with default values.
 *       \snippet qs_tcc_dma.c dma_setup_2
 *       \note This should always be performed before using the configuration
 *             struct to ensure that all values are initialized to known default
 *             settings.
 * -# Adjust the DMA resource configurations.
 *       \snippet qs_tcc_dma.c dma_setup_3
 * -# Allocate a DMA resource with the configurations.
 *       \snippet qs_tcc_dma.c dma_setup_4
 * -# Prepare DMA transfer descriptor.
 *  -# Create a DMA transfer descriptor.
 *       \snippet qs_tcc_dma.c capture_dma_descriptor
 *       \note When multiple descriptors are linked, the linked item should
 *             never go out of scope before it is loaded (to DMA Write-Back
 *             memory section). In most cases, if more than one descriptors are
 *             used, they should be global except the very first one.
 *  -# Create a DMA transfer descriptor struct.
 *  -# Create a DMA transfer descriptor configuration structure, which can be
 *       filled out to adjust the configuration of a single DMA transfer.
 *       \snippet qs_tcc_dma.c dma_setup_5
 *  -# Initialize the DMA transfer descriptor configuration struct with
 *       default values.
 *       \snippet qs_tcc_dma.c dma_setup_6
 *       \note This should always be performed before using the configuration
 *             struct to ensure that all values are initialized to known default
 *             settings.
 *  -# Adjust the DMA transfer descriptor configurations.
 *       \snippet qs_tcc_dma.c dma_setup_7
 *  -# Create the DMA transfer descriptor with the given configuration.
 *       \snippet qs_tcc_dma.c dma_setup_8
 * -# Start DMA transfer job with prepared descriptor.
 *  -# Add the DMA transfer descriptor to the allocated DMA resource.
 *       \snippet qs_tcc_dma.c dma_setup_10
 *       \note When adding multiple descriptors, the last added one is linked
 *             at the end of descriptor queue. If ringed list is needed, just
 *             add the first descriptor again to build the circle.
 *  -# Start the DMA transfer job with the allocated DMA resource and
 *       transfer descriptor.
 *       \snippet qs_tcc_dma.c dma_setup_11
 * \subsubsection asfdoc_sam0_tcc_dma_use_case_setup_flow_dma_compare Configure the DMA for Compare TCC Channel 0
 * Configure the DMAC module to update TCC channel 0 compare value.
 * The flow is similar to last DMA configure step for capture.
 * -# Allocate and configure the DMA resource.
 *     \snippet qs_tcc_dma.c compare_dma_resource
 *     \snippet qs_tcc_dma.c config_dma_resource_for_wave
 * -# Prepare DMA transfer descriptor.
 *     \snippet qs_tcc_dma.c compare_dma_descriptor
 *     \snippet qs_tcc_dma.c config_dma_descriptor_for_wave
 * -# Start DMA transfer job with prepared descriptor.
 *     \snippet qs_tcc_dma.c config_dma_job_for_wave
 * -# Enable the TCC module to start the timer and begin PWM signal generation.
 *     \snippet qs_tcc_dma.c setup_enable
 *
 * \section asfdoc_sam0_tcc_dma_use_case_main Use Case
 *
 * \subsection asfdoc_sam0_tcc_dma_use_case_main_code Code
 * Copy-paste the following code to your user application:
 * \snippet qs_tcc_dma.c main
 *
 * \subsection asfdoc_sam0_tcc_dma_use_case_main_flow Workflow
 * -# Enter an infinite loop while the PWM wave is generated via the TCC module.
 *  \snippet qs_tcc_dma.c main_loop
 */
