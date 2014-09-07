#include "mbed.h"
#include "test_env.h"
#include "MMA7660.h"

MMA7660 MMA(p28, p27);

int main() {
    if (!MMA.testConnection())
        notify_completion(false);

    for(int i = 0; i < 5; i++) {
        printf("x: %f, y: %f, z: %f\r\n", MMA.x(), MMA.y(), MMA.z());
        wait(0.2);
    }

    notify_completion(true);
}
