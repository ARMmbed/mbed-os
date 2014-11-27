#include "mbed.h"
#include "SDFileSystem.h"
#include "test_env.h"
#include "rtos.h"

DigitalOut led2(LED2);

#define SIZE 120

void sd_thread(void const *argument)
{
    const char *FILE_NAME = "/sd/out.txt";

#if defined(TARGET_KL25Z)
    SDFileSystem sd(PTD2, PTD3, PTD1, PTD0, "sd");

#elif defined(TARGET_KL46Z)
    SDFileSystem sd(PTD6, PTD7, PTD5, PTD4, "sd");

#elif defined(TARGET_K64F)
    SDFileSystem sd(PTD2, PTD3, PTD1, PTD0, "sd");

#elif defined(TARGET_RZ_A1H)
    SDFileSystem sd(P8_5, P8_6, P8_3, P8_4, "sd");

#else
    SDFileSystem sd(p11, p12, p13, p14, "sd");
#endif

    // Allocate data buffers
    uint8_t data_written[SIZE] = { 0 };
    uint8_t data_read[SIZE] = { 0 };

    {
        // fill data_written buffer with random data
        FILE *f = fopen(FILE_NAME, "w");
        if (f) {
            // write these data into the file
            printf("Writing %d bytes to file:\r\n", SIZE);
            for (int i = 0; i < SIZE; i++) {
                data_written[i] = rand() % 0xff;
                fprintf(f, "%c", data_written[i]);
                printf("%02X ", data_written[i]);
                if (i && ((i % 20) == 19))
                    printf("\r\n");
            }
            fclose(f);
        } else {
            notify_completion(false);
            return;
        }
    }
    printf("\r\n\r\n");

    {
        // read back the data from the file and store them in data_read
        FILE *f = fopen(FILE_NAME, "r");
        if (f) {
            printf("Reading %d bytes from file:\r\n", SIZE);
            for (int i = 0; i < SIZE; i++) {
                data_read[i] = fgetc(f);
                printf("%02X ", data_read[i]);
                if (i && ((i % 20) == 19))
                    printf("\r\n");
            }
            fclose(f);
        } else {
            notify_completion(false);
            return;
        }
    }
    printf("\r\nDone.\r\n");

    // check that the data written == data read
    for (int i = 0; i < SIZE; i++) {
        if (data_written[i] != data_read[i]) {
            notify_completion(false);
            return;
        }
    }
    notify_completion(true);
}

int main()
{
    Thread t(sd_thread, NULL, osPriorityNormal, (DEFAULT_STACK_SIZE * 2.25));

    while (true) {
        led2 = !led2;
        Thread::wait(1000);
    }
}
