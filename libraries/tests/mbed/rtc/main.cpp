#include "mbed.h"

#define CUSTOM_TIME  1256729737

int main() {
    char buffer[32] = {0};
    set_time(CUSTOM_TIME);  // Set RTC time to Wed, 28 Oct 2009 11:35:37
    while(1) {
        time_t seconds = time(NULL);
        strftime(buffer, 32, "%Y-%m-%d %H:%M:%S %p", localtime(&seconds));
        printf("MBED: [%ld] [%s]\r\n", seconds, buffer);
        wait(1);
    }
}
