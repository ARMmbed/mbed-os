#include "cmsis.h"
#include "objects.h"
#include "platform/mbed_error.h"
#include "mbed_interface.h"

#include "hardware/resets.h"
#include "hardware/clocks.h"
#include "hardware/sync.h"
#include "pico/binary_info.h"

#include <stdarg.h>

int mbed_sdk_inited = 0;

uint32_t __attribute__((section(".ram_vector_table"))) ram_vector_table[48];

void mbed_sdk_init()
{
    // This function is adapted from the Pico SDK's runtime_init()

    // Reset all peripherals to put system into a known state,
    // - except for QSPI pads and the XIP IO bank, as this is fatal if running from flash
    // - and the PLLs, as this is fatal if clock muxing has not been reset on this boot
    reset_block(~(
            RESETS_RESET_IO_QSPI_BITS |
            RESETS_RESET_PADS_QSPI_BITS |
            RESETS_RESET_PLL_USB_BITS |
            RESETS_RESET_PLL_SYS_BITS
    ));

    // Remove reset from peripherals which are clocked only by clk_sys and
    // clk_ref. Other peripherals stay in reset until we've configured clocks.
    unreset_block_wait(RESETS_RESET_BITS & ~(
            RESETS_RESET_ADC_BITS |
            RESETS_RESET_RTC_BITS |
            RESETS_RESET_SPI0_BITS |
            RESETS_RESET_SPI1_BITS |
            RESETS_RESET_UART0_BITS |
            RESETS_RESET_UART1_BITS |
            RESETS_RESET_USBCTRL_BITS
    ));

    // After calling preinit we have enough runtime to do the exciting maths
    // in clocks_init
    clocks_init();
    SystemCoreClockUpdate();

    // Peripheral clocks should now all be running
    unreset_block_wait(RESETS_RESET_BITS);

#if !PICO_IE_26_29_UNCHANGED_ON_RESET
    // after resetting BANK0 we should disable IE on 26-29
    hw_clear_alias(padsbank0_hw)->io[26] = hw_clear_alias(padsbank0_hw)->io[27] =
            hw_clear_alias(padsbank0_hw)->io[28] = hw_clear_alias(padsbank0_hw)->io[29] = PADS_BANK0_GPIO0_IE_BITS;
#endif

#if !(PICO_NO_RAM_VECTOR_TABLE || PICO_NO_FLASH)
    __builtin_memcpy(ram_vector_table, (uint32_t *) SCB->VTOR, sizeof(ram_vector_table));
    SCB->VTOR = (intptr_t) ram_vector_table;
#endif

#ifndef NDEBUG
    uint32_t xpsr;
    __asm volatile ("mrs %0, XPSR" : "=r" (xpsr)::);
    if (xpsr & 0xffu) {
        // crap; started in exception handler
        __asm ("bkpt #0");
    }
#endif

    spin_locks_reset();

	mbed_sdk_inited = 1;
}

// Add a basic "binary info" note which says that this is an Mbed OS program
#define reset_section_attr __attribute__((section(".reset")))
bi_decl_with_attr(bi_program_name("Mbed OS CE Program"), reset_section_attr)

// Pico SDK panic handlers
void __attribute__((noreturn)) panic_unsupported() {
    panic("not supported");
}

void hard_assertion_failure(void) {
    panic("Hard assert");
}

void __attribute__((noreturn)) __printflike(1, 0) panic(const char *fmt, ...) {
    mbed_error_printf("\n*** PANIC ***\n");

    // Try and format the message to the stack
    const size_t MSG_BUF_LEN = 256;
    char msgBuf[MSG_BUF_LEN];
    va_list args;
    va_start(args, fmt);
    snprintf(msgBuf, MSG_BUF_LEN, fmt, args);
    va_end(args);

    MBED_ERROR(
            MBED_MAKE_ERROR(
                MBED_MODULE_HAL,
                MBED_ERROR_CODE_UNKNOWN
            ),
            msgBuf
        );
}