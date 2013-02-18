#include "mbed.h"

int main() {
    set_time(1256729737);  // Set RTC time to Wed, 28 Oct 2009 11:35:37

    while(1) {
        time_t seconds = time(NULL);

        printf("Time as seconds since January 1, 1970 = %d\n", seconds);

        printf("Time as a basic string = %s", ctime(&seconds));

        char buffer[32];
        strftime(buffer, 32, "%I:%M %p\n", localtime(&seconds));
        printf("Time as a custom formatted string = %s", buffer);

        wait(1);
    }
}
