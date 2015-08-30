#ifndef UTILS_INTERRUPT_INTERRUPT_H
#define UTILS_INTERRUPT_INTERRUPT_H

#include <compiler.h>
#include <parts.h>

#ifdef __cplusplus
extern "C" {
#endif

/**
 * \weakgroup interrupt_group
 *
 * @{
 */

/**
 * \name Interrupt Service Routine definition
 *
 * @{
 */

/**
 * \brief Define service routine
 *
 * \note For NVIC devices the interrupt service routines are predefined to
 *       add to vector table in binary generation, so there is no service
 *       register at run time. The routine collections are in exceptions.h.
 *
 * Usage:
 * \code
	ISR(foo_irq_handler)
	{
	     // Function definition
	     ...
	}
\endcode
 *
 * \param func Name for the function.
 */
#  define ISR(func)   \
	void func (void)

/**
 * \brief Initialize interrupt vectors
 *
 * For NVIC the interrupt vectors are put in vector table. So nothing
 * to do to initialize them, except defined the vector function with
 * right name.
 *
 * This must be called prior to \ref irq_register_handler.
 */
#  define irq_initialize_vectors()   \
	do {                             \
	} while(0)

/**
 * \brief Register handler for interrupt
 *
 * For NVIC the interrupt vectors are put in vector table. So nothing
 * to do to register them, except defined the vector function with
 * right name.
 *
 * Usage:
 * \code
	irq_initialize_vectors();
	irq_register_handler(foo_irq_handler);
\endcode
 *
 * \note The function \a func must be defined with the \ref ISR macro.
 * \note The functions prototypes can be found in the device exception header
 *       files (exceptions.h).
 */
#  define irq_register_handler(int_num, int_prio)                      \
	NVIC_ClearPendingIRQ(    (IRQn_Type)int_num);                      \
	NVIC_SetPriority(    (IRQn_Type)int_num, int_prio);                \
	NVIC_EnableIRQ(      (IRQn_Type)int_num);                          \

//@}

#  define cpu_irq_enable()                     \
	do {                                       \
		g_interrupt_enabled = true;            \
		__DMB();                               \
		__enable_irq();                        \
	} while (0)
#  define cpu_irq_disable()                    \
	do {                                       \
		__disable_irq();                       \
		__DMB();                               \
		g_interrupt_enabled = false;           \
	} while (0)

typedef uint32_t irqflags_t;

#if !defined(__DOXYGEN__)
extern volatile bool g_interrupt_enabled;
#endif

#define cpu_irq_is_enabled()    (__get_PRIMASK() == 0)

static volatile uint32_t cpu_irq_critical_section_counter;
static volatile bool     cpu_irq_prev_interrupt_state;

static inline irqflags_t cpu_irq_save(void)
{
    irqflags_t flags = cpu_irq_is_enabled();
    cpu_irq_disable();
    return flags;
}

static inline bool cpu_irq_is_enabled_flags(irqflags_t flags)
{
    return (flags);
}

static inline void cpu_irq_restore(irqflags_t flags)
{
    if (cpu_irq_is_enabled_flags(flags))
        cpu_irq_enable();
}

void cpu_irq_enter_critical(void);
void cpu_irq_leave_critical(void);

/**
 * \weakgroup interrupt_deprecated_group
 * @{
 */

#define Enable_global_interrupt()            cpu_irq_enable()
#define Disable_global_interrupt()           cpu_irq_disable()
#define Is_global_interrupt_enabled()        cpu_irq_is_enabled()

//@}

//@}

#ifdef __cplusplus
}
#endif

#endif /* UTILS_INTERRUPT_INTERRUPT_H */
