#include "mbed.h"
#include "USBHostMSD.h"
DigitalOut led(LED1);
#include "FATFileSystem.h"
#include <stdlib.h>


void msd_task(void const *)
{

    USBHostMSD msd;
    int i = 0;
    FATFileSystem fs("usb");
    int err;
    printf("wait for usb memory stick insertion\n");
    while(1) {

        // try to connect a MSD device
        while(!msd.connect()) {
            Thread::wait(500);
        }
        if (fs.mount(&msd) != 0) {
            continue;
        } else {
            printf("file system mounted\n");
        }

        if  (!msd.connect()) {
            continue;
        }

        // in a loop, append a file
        // if the device is disconnected, we try to connect it again

        // append a file
        File file;
        err = file.open(&fs, "test1.txt", O_WRONLY | O_CREAT |O_APPEND);

        if (err == 0) {
            char tmp[100];
            sprintf(tmp,"Hello fun USB stick  World: %d!\r\n", i++);
            file.write(tmp,strlen(tmp));
            sprintf(tmp,"Goodbye World!\r\n");
            file.write(tmp,strlen(tmp));
            file.close();
        } else {
            printf("FILE == NULL\r\n");
        }
        Thread::wait(500);
        printf("again\n");
        // if device disconnected, try to connect again
        while (msd.connected()) {
            Thread::wait(500);
        }
        while (fs.unmount() < 0) {
            Thread::wait(500);
            printf("unmount\n");
        }
    }
}

int main()
{
    Thread msdTask(msd_task, NULL, osPriorityNormal, 1024 * 4);

    while(1) {
        led=!led;
        Thread::wait(500);
    }
}
