#include "mbed.h"

DigitalOut led(P1_9);

int main() {
	while (1) {
		led = 1;
		wait(0.5);
		led = 0;
		wait(0.5);
	}
}