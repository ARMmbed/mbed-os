#include "mbed.h"
#include "SDFileSystem.h"
#include "test_env.h"

SDFileSystem sd(p11, p12, p13, p14, "sd");

#define SIZE 120

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
