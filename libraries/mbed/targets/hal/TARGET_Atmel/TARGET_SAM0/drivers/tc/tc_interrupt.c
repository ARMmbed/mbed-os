#include "tc_interrupt.h"

void *_tc_instances[TC_INST_NUM];

void _tc_interrupt_handler(uint8_t instance);

/**
 * \brief Registers a callback.
 *
 * Registers a callback function which is implemented by the user.
 *
 * \note The callback must be enabled by \ref tc_enable_callback,
 * in order for the interrupt handler to call it when the conditions for the
 * callback type is met.
 *
 * \param[in]     module      Pointer to TC software instance struct
 * \param[in]     callback_func Pointer to callback function
 * \param[in]     callback_type Callback type given by an enum
 */
enum status_code tc_register_callback(
    struct tc_module *const module,
    tc_callback_t callback_func,
    const enum tc_callback callback_type)
{
    /* Sanity check arguments */
    Assert(module);
    Assert(callback_func);

    /* Register callback function */
    module->callback[callback_type] = callback_func;

    /* Set the bit corresponding to the callback_type */
    if (callback_type == TC_CALLBACK_CC_CHANNEL0) {
        module->register_callback_mask |= TC_INTFLAG_MC(1);
    } else if (callback_type == TC_CALLBACK_CC_CHANNEL1) {
        module->register_callback_mask |= TC_INTFLAG_MC(2);
    } else {
        module->register_callback_mask |= (1 << callback_type);
    }
    return STATUS_OK;
}

/**
 * \brief Unregisters a callback.
 *
 * Unregisters a callback function implemented by the user. The callback should be
 * disabled before it is unregistered.
 *
 * \param[in]     module Pointer to TC software instance struct
 * \param[in]     callback_type Callback type given by an enum
 */
enum status_code tc_unregister_callback(
    struct tc_module *const module,
    const enum tc_callback callback_type)
{
    /* Sanity check arguments */
    Assert(module);

    /* Unregister callback function */
    module->callback[callback_type] = NULL;

    /* Clear the bit corresponding to the callback_type */
    if (callback_type == TC_CALLBACK_CC_CHANNEL0) {
        module->register_callback_mask &= ~TC_INTFLAG_MC(1);
    } else if (callback_type == TC_CALLBACK_CC_CHANNEL1) {
        module->register_callback_mask &= ~TC_INTFLAG_MC(2);
    } else {
        module->register_callback_mask &= ~(1 << callback_type);
    }
    return STATUS_OK;
}

/**
 * \internal ISR handler for TC
 *
 * Auto-generate a set of interrupt handlers for each TC in the device.
 */
#define _TC_INTERRUPT_HANDLER(n, m) \
		void TC##n##_Handler(void) \
		{ \
			_tc_interrupt_handler(m); \
		}

#if (SAML21E) || (SAML21G)
_TC_INTERRUPT_HANDLER(0,0)
_TC_INTERRUPT_HANDLER(1,1)
_TC_INTERRUPT_HANDLER(4,2)
#else
MRECURSION(TC_INST_NUM, _TC_INTERRUPT_HANDLER, TC_INST_MAX_ID)
#endif


/**
 * \internal Interrupt Handler for TC module
 *
 * Handles interrupts as they occur, it will run the callback functions
 * that are registered and enabled.
 *
 * \param[in]  instance  ID of the TC instance calling the interrupt
 *                       handler.
 */
void _tc_interrupt_handler(
    uint8_t instance)
{
    /* Temporary variable */
    uint8_t interrupt_and_callback_status_mask;

    /* Get device instance from the look-up table */
    struct tc_module *module
        = (struct tc_module *)_tc_instances[instance];

    /* Read and mask interrupt flag register */
    interrupt_and_callback_status_mask = module->hw->COUNT8.INTFLAG.reg &
                                         module->register_callback_mask &
                                         module->enable_callback_mask;

    /* Check if an Overflow interrupt has occurred */
    if (interrupt_and_callback_status_mask & TC_INTFLAG_OVF) {
        /* Invoke registered and enabled callback function */
        (module->callback[TC_CALLBACK_OVERFLOW])(module);
        /* Clear interrupt flag */
        module->hw->COUNT8.INTFLAG.reg = TC_INTFLAG_OVF;
    }

    /* Check if an Error interrupt has occurred */
    if (interrupt_and_callback_status_mask & TC_INTFLAG_ERR) {
        /* Invoke registered and enabled callback function */
        (module->callback[TC_CALLBACK_ERROR])(module);
        /* Clear interrupt flag */
        module->hw->COUNT8.INTFLAG.reg = TC_INTFLAG_ERR;
    }

    /* Check if an Match/Capture Channel 0 interrupt has occurred */
    if (interrupt_and_callback_status_mask & TC_INTFLAG_MC(1)) {
        /* Invoke registered and enabled callback function */
        (module->callback[TC_CALLBACK_CC_CHANNEL0])(module);
        /* Clear interrupt flag */
        module->hw->COUNT8.INTFLAG.reg = TC_INTFLAG_MC(1);
    }

    /* Check if an Match/Capture Channel 1 interrupt has occurred */
    if (interrupt_and_callback_status_mask & TC_INTFLAG_MC(2)) {
        /* Invoke registered and enabled callback function */
        (module->callback[TC_CALLBACK_CC_CHANNEL1])(module);
        /* Clear interrupt flag */
        module->hw->COUNT8.INTFLAG.reg = TC_INTFLAG_MC(2);
    }
}
