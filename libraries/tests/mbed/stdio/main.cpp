#include "mbed.h"
#include "test_env.h"

/* This test purpose is to verify the behaviour when the program does not link
 * any symbol from the mbed library.
 * In the past we had an issue where the stdio retargeting was not linked in.
 */

int main() {
    union {
        int value_int;
    };

    notify_start();

    while (true)
    {
        // SCANF PRINTF family
        value_int = 0;
        scanf("%d", &value_int);
        printf("Your value was: %d\r\n", value_int);

        // FSCANF, FPRINTF family
        value_int = 0;
        fscanf(stdin, "%d", &value_int);
        fprintf(stdout, "Your value was: %d\r\n", value_int);

        //...
    }
}
