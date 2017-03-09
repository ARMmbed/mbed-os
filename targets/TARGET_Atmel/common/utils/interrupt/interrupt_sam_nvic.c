#include "interrupt_sam_nvic.h"

#if !defined(__DOXYGEN__)
/* Deprecated - global flag to determine the global interrupt state. Required by
 * QTouch library, however new applications should use cpu_irq_is_enabled()
 * which probes the true global interrupt state from the CPU special registers.
 */
volatile bool g_interrupt_enabled = true;
#endif

void cpu_irq_enter_critical(void)
{
    if (cpu_irq_critical_section_counter == 0) {
        if (cpu_irq_is_enabled()) {
            cpu_irq_disable();
            cpu_irq_prev_interrupt_state = true;
        } else {
            /* Make sure the to save the prev state as false */
            cpu_irq_prev_interrupt_state = false;
        }

    }

    cpu_irq_critical_section_counter++;
}

void cpu_irq_leave_critical(void)
{
    /* Check if the user is trying to leave a critical section when not in a critical section */
    Assert(cpu_irq_critical_section_counter > 0);

    cpu_irq_critical_section_counter--;

    /* Only enable global interrupts when the counter reaches 0 and the state of the global interrupt flag
       was enabled when entering critical state */
    if ((cpu_irq_critical_section_counter == 0) && (cpu_irq_prev_interrupt_state)) {
        cpu_irq_enable();
    }
}

