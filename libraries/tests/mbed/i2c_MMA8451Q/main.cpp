#include "mbed.h"
#include "MMA8451Q.h"

#define MMA8451_I2C_ADDRESS (0x1d<<1)

#ifdef TARGET_KL05Z
	#define SDA		PTB4
	#define SCL		PTB3
#else
	#define SDA		PTE25
	#define SCL		PTE24
#endif

int main(void) {
    DigitalOut led(LED_GREEN);
    MMA8451Q acc(SDA, SCL, MMA8451_I2C_ADDRESS);
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
