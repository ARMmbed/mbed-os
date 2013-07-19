#include "mbed.h"

DigitalOut led(P1_9);

int main() {
	while (1) {
		printf("Testing...\r\n");
		led = 1;
		printf("LED on\r\n");
		wait(0.5);
		led = 0;
		wait(0.5);
	}
}