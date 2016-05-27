#ifndef UTILS_INTERRUPT_H
#define UTILS_INTERRUPT_H

#include <parts.h>

#if XMEGA || MEGA || TINY
#  include "interrupt/interrupt_avr8.h"
#elif UC3
#  include "interrupt/interrupt_avr32.h"
#elif SAM
#  include "interrupt/interrupt_sam_nvic.h"
#else
#  error Unsupported device.
#endif

/**
 * \defgroup interrupt_group Global interrupt management
 *
 * This is a driver for global enabling and disabling of interrupts.
 *
 * @{
 */

#if defined(__DOXYGEN__)
/**
 * \def CONFIG_INTERRUPT_FORCE_INTC
 * \brief Force usage of the ASF INTC driver
 *
 * Predefine this symbol when preprocessing to force the use of the ASF INTC driver.
 * This is useful to ensure compatibility across compilers and shall be used only when required
 * by the application needs.
 */
#  define CONFIG_INTERRUPT_FORCE_INTC
#endif

//! \name Global interrupt flags
//@{
/**
 * \typedef irqflags_t
 * \brief Type used for holding state of interrupt flag
 */

/**
 * \def cpu_irq_enable
 * \brief Enable interrupts globally
 */

/**
 * \def cpu_irq_disable
 * \brief Disable interrupts globally
 */

/**
 * \fn irqflags_t cpu_irq_save(void)
 * \brief Get and clear the global interrupt flags
 *
 * Use in conjunction with \ref cpu_irq_restore.
 *
 * \return Current state of interrupt flags.
 *
 * \note This function leaves interrupts disabled.
 */

/**
 * \fn void cpu_irq_restore(irqflags_t flags)
 * \brief Restore global interrupt flags
 *
 * Use in conjunction with \ref cpu_irq_save.
 *
 * \param flags State to set interrupt flag to.
 */

/**
 * \fn bool cpu_irq_is_enabled_flags(irqflags_t flags)
 * \brief Check if interrupts are globally enabled in supplied flags
 *
 * \param flags Currents state of interrupt flags.
 *
 * \return True if interrupts are enabled.
 */

/**
 * \def cpu_irq_is_enabled
 * \brief Check if interrupts are globally enabled
 *
 * \return True if interrupts are enabled.
 */
//@}

//! @}

/**
 * \ingroup interrupt_group
 * \defgroup interrupt_deprecated_group Deprecated interrupt definitions
 */

#endif /* UTILS_INTERRUPT_H */
