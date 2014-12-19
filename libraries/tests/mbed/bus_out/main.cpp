#include "mbed.h"
#include "test_env.h"

BusOut bus_out(LED1, LED2, LED3, LED4);

int main()
{
    notify_start();

    bool result = false;

    for (;;) {
        const int mask = bus_out.mask();
        int led_mask = 0x00;
        if (LED1 != NC) led_mask |= 0x01;
        if (LED2 != NC) led_mask |= 0x02;
        if (LED3 != NC) led_mask |= 0x04;
        if (LED4 != NC) led_mask |= 0x08;

        printf("MBED: BusIn mask: 0x%X\r\n", mask);
        printf("MBED: BusIn LED mask: 0x%X\r\n", led_mask);

        // Let's check bus's connected pins mask
        if (mask != led_mask) {
            break;
        }

        // Checking if DigitalOut is correctly set as connected
        for (int i=0; i<4; i++) {
            printf("MBED: BusOut.bit[%d] is %s\r\n", i, bus_out[i].is_connected() ? "connected" : "not connected");
        }

        if (LED1 != NC && bus_out[0].is_connected() == 0) {
            break;
        }
        if (LED1 != NC && bus_out[1].is_connected() == 0) {
            break;
        }
        if (LED1 != NC && bus_out[2].is_connected() == 0) {
            break;
        }
        if (LED1 != NC && bus_out[3].is_connected() == 0) {
            break;
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

    printf("MBED: Blinking LEDs...\r\n");

    // Just a quick LED blinking...
    for (int i=0; i<4; i++) {
        if (bus_out[i].is_connected()) {
            bus_out[i] = 1;
        }
        wait(0.2);
        if (bus_out[i].is_connected()) {
            bus_out[i] = 0;
        }
    }

    notify_completion(result);
}
