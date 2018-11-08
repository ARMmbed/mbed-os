#include "mbed.h"
#include "test_env.h"

#if defined(TARGET_VK_RZ_A1H)
#define LOOPCNT    1
namespace {
BusOut bus_out(LED1);
PinName led_pins[1] = {LED1}; // Temp, used to map pins in bus_out
}
#elif defined(TARGET_DISCO_F429ZI)
#define LOOPCNT    2
namespace {
BusOut bus_out(LED1, LED2);
PinName led_pins[2] = {LED1, LED2}; // Temp, used to map pins in bus_out
}
#else
#define LOOPCNT    4
namespace {
BusOut bus_out(LED1, LED2, LED3, LED4);
PinName led_pins[4] = {LED1, LED2, LED3, LED4}; // Temp, used to map pins in bus_out
}
#endif

int main()
{
    notify_start();

    bool result = false;

    for (;;) {
        const int mask = bus_out.mask();
        int led_mask = 0x00;
        if (LED1 != NC) {
            led_mask |= 0x01;
        }
#if !defined(TARGET_VK_RZ_A1H)
        if (LED2 != NC) {
            led_mask |= 0x02;
        }
#if !defined(TARGET_DISCO_F429ZI)
        if (LED3 != NC) {
            led_mask |= 0x04;
        }
        if (LED4 != NC) {
            led_mask |= 0x08;
        }
#endif
#endif

        printf("MBED: BusIn mask: 0x%X\r\n", mask);
        printf("MBED: BusIn LED mask: 0x%X\r\n", led_mask);

        // Let's check bus's connected pins mask
        if (mask != led_mask) {
            break;
        }

        // Checking if DigitalOut is correctly set as connected
        for (int i = 0; i < LOOPCNT; i++) {
            printf("MBED: BusOut.bit[%d] is %s\r\n",
                   i,
                   (led_pins[i] != NC && bus_out[i].is_connected())
                   ? "connected"
                   : "not connected");
        }

        for (int i = 0; i < LOOPCNT; i++) {
            if (led_pins[i] != NC && bus_out[0].is_connected() == 0) {
                break;
            }
        }

        // Write mask all LEDs
        bus_out.write(mask);    // Set all LED's pins in high state
        if (bus_out.read() != mask) {
            break;
        }
        // Zero all LEDs and see if mask is correctly cleared on all bits
        bus_out.write(~mask);
        if (bus_out.read() != 0x00) {
            break;
        }

        result = true;
        break;
    }

    printf("MBED: Blinking LEDs: \r\n");

    // Just a quick LED blinking...
    for (int i = 0; i < LOOPCNT; i++) {
        if (led_pins[i] != NC && bus_out[i].is_connected()) {
            bus_out[i] = 1;
            printf("%c", 'A' + i);
        } else {
            printf(".");
        }
        wait(0.2);
        if (led_pins[i] != NC && bus_out[i].is_connected()) {
            bus_out[i] = 0;
            printf("%c", 'a' + i);
        } else {
            printf(".");
        }
    }
    printf("\r\n");

    notify_completion(result);
}
