// Interrupt table relocation test, based on the 'interrupt_in' test
// It will test an interrupt pin before and after the interrupt table is relocated
// Works only on LPC1768

#include "test_env.h"
#include "cmsis_nvic.h"
#include <string.h>

#define PIN_IN      (p5)
#define PIN_OUT     (p25)
#define NUM_VECTORS (16+33)

DigitalOut out(PIN_OUT);
DigitalOut myled(LED1);

static volatile int checks;
static uint32_t int_table[NUM_VECTORS];

void flipper() {
    for (int i = 0; i < 5; i++) {
        out = 1;
        wait(0.2);
        out = 0;
        wait(0.2);
    }
}

void in_handler() {
    checks++;
    myled = !myled;
}

static bool test_once() {
    InterruptIn in(PIN_IN);
    checks = 0;
    printf("Interrupt table location: 0x%08X\r\n", SCB->VTOR);
    in.rise(NULL);
    in.fall(in_handler);
    flipper();
    in.fall(NULL);
    bool result = (checks == 5);
    printf(result ? "Test passed.\r\n" : "Test failed.\r\n");
    return result;
}

int main() {
    printf("Starting first test (interrupts not relocated).\r\n");
    if (!test_once()) {
        notify_completion(false);
        return 1;
    }

    // Relocate interrupt table and test again
    memcpy(int_table, (void*)SCB->VTOR, sizeof(int_table));
    SCB->VTOR = (uint32_t)int_table;
    printf("Starting second test (interrupts relocated).\r\n");

    notify_completion(test_once());
    return 0;
}

