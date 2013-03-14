#include "mbed.h"

DigitalOut out(p5);
AnalogIn in(p19);

int main() {
    while(1) {
        if(in > 0.5) {
            out = !out;
        }
    }
}
