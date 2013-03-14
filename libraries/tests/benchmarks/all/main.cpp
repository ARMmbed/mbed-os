#include "mbed.h"

DigitalOut out(p5);
AnalogIn in(p19);

volatile float w, x, y, z;
int main() {
    while(1) {
        z = x * y / w;
        printf("Hello World %d %f\n", out.read(), z); 
        if(in > 0.5) {
            out = !out;
        }
    }
}
