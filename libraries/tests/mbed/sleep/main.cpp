#include "test_env.h"

InterruptIn wkp(p14);

void flip() {
    printf("button pressed\n");
}

int main() {
    wkp.rise(&flip);
    
    while (true) {
        // sleep();
        deepsleep();
    }
}
