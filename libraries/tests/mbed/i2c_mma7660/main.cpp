#include "mbed.h"
#include "test_env.h"
#include "MMA7660.h"

//MMA7660 MMA(p28, p27);
I2C i2c(PTB3, PTB2);

int main() {
for (int i = 0; i<128; i++)
    printf("Write to %d = %d\r\n", i, i2c.write(i * 2, NULL, 0));
/*
    if (!MMA.testConnection())
        notify_completion(false);

    for(int i = 0; i < 5; i++) {
        printf("x: %f, y: %f, z: %f\r\n", MMA.x(), MMA.y(), MMA.z());
        wait(0.2);
    }
*/
    notify_completion(true);
}
