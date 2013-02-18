#include "test_env.h"

DigitalOut out(p5);
DigitalIn   in(p25);

int main() {
    out = 0;
    wait(0.1);
    if (in != 0) {
        printf("ERROR: in != 0\n");
        notify_completion(false);
    }
    out = 1;
    wait(0.1);
    if (in != 1) {
        printf("ERROR: in != 1\n");
        notify_completion(false);
    }
    
    notify_completion(true);
}
