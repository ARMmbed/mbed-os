#include "mbed.h"
#include "SDFileSystem.h"
#include "test_env.h"

#if defined(TARGET_KL25Z)
SDFileSystem sd(PTD2, PTD3, PTD1, PTD0, "sd");
#elif defined(TARGET_nRF51822)
SDFileSystem sd(p12, p13, p15, p14, "sd");
#elif defined(TARGET_NUCLEO_F103RB)
SDFileSystem sd(D11, D12, D13, D10, "sd");
#else
SDFileSystem sd(p11, p12, p13, p14, "sd");
#endif

#define SIZE 120

DigitalOut led1(LED1);
int main() {
    FILE *f = fopen("/sd/out.txt", "w");
    
    // allocate buffers
    uint8_t data_written[SIZE];
    uint8_t data_read[SIZE];
    
    // fill data_written buffer with random data
    // write these data into the file
    printf("written: [");
    for (int i = 0; i < SIZE; i++) {
        data_written[i] = rand() % 0xff;
        fprintf(f, "%c", data_written[i]);
        printf("%d ", data_written[i]);
    }
    printf("]\r\nclosing\r\n");
    fclose(f);
    
    // read back the data from the file and store them in data_read
    f = fopen("/sd/out.txt", "r");
    printf("read: [");
    for (int i=0; i<SIZE; i++) {
        data_read[i] = fgetc(f);
        printf("%d ", data_read[i]);
    }
    printf("]\r\nclosing\r\n");
    fclose(f);
    
    // check that the data written == data read
    for (int i = 0; i < SIZE; i++) {
        if (data_written[i] != data_read[i]) {
            notify_completion(false);
        }
    }
    notify_completion(true);
}
