#include "mbed.h"
#include "MMA8451Q.h"

#define MMA8451_I2C_ADDRESS (0x1d<<1)

int main(void) {
    DigitalOut led(LED_GREEN);
    MMA8451Q acc(PTE25, PTE24, MMA8451_I2C_ADDRESS);
    printf("WHO AM I: 0x%2X\r\n", acc.getWhoAmI());

    while (true) {
        printf("-----------\r\n");
        printf("acc_x: %d\r\n", acc.getAccX());
        printf("acc_y: %d\r\n", acc.getAccY());
        printf("acc_z: %d\r\n", acc.getAccZ());

        wait(1);
        led = !led;
    }
}
