#include "cmsis.h"

void NVIC_SystemReset () {
    // no-op
}

void __enable_irq () {

}

void __disable_irq () {

}

int __get_PRIMASK () {
    return 0; // IRQ enabled
}

void __CLREX () {
    // no-op
}

uint8_t __LDREXB (volatile uint8_t *ptr) {
    return *ptr;
}

uint16_t __LDREXH (volatile uint16_t *ptr) {
    return *ptr;
}

uint32_t __LDREXW (volatile uint32_t *ptr) {
    return *ptr;
}

uint32_t __get_IPSR	(void) {
    return 0; // Thread Mode (https://www.keil.com/pack/doc/CMSIS/Core/html/group__Core__Register__gr.html#ga2c32fc5c7f8f07fb3d436c6f6fe4e8c8)
}

uint32_t __STREXB (uint8_t value, volatile uint8_t *addr) {
    *addr = value;
    return 0;
}

uint32_t __STREXH (uint16_t value, volatile uint16_t *addr) {
    *addr = value;
    return 0;
}

uint32_t __STREXW (uint32_t value, volatile uint32_t *addr) {
    *addr = value;
    return 0;
}
