#include "mbed.h"
#include "USBHostMSD.h"
DigitalOut led(LED1);
void msd_task(void const *) {
	printf("init msd\n");
	USBHostMSD msd("usb");
	int i = 0;
	printf("wait for usb memory stick insertion\n");
	while(1) {

		// try to connect a MSD device
		while(!msd.connect()) {
			Thread::wait(500);
		}

		// in a loop, append a file
		// if the device is disconnected, we try to connect it again

		// append a file
		FILE * fp = fopen("/usb/test1.txt", "a");

		if (fp != NULL) {
			fprintf(fp, "Hello fun SD Card World: %d!\r\n", i++);
			printf("Goodbye World!\r\n");
			fclose(fp);
		} else {
			printf("FILE == NULL\r\n");
		}
		Thread::wait(500);
		printf("again\n");
		// if device disconnected, try to connect again
		while (msd.connected()) {
			Thread::wait(500);
		}
	}
}

int main() {
    Thread msdTask(msd_task, NULL, osPriorityNormal, 1024 * 4);
    while(1) {
        led=!led;
        Thread::wait(500);
    }
}
