#include "mbed_critical.h"

bool core_util_are_interrupts_enabled(void) {
    return true;
}

bool core_util_is_isr_active(void) {
    return false;
}

void core_util_critical_section_enter(void) {}

void core_util_critical_section_exit(void) {}
