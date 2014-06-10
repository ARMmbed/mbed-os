#include "mbed.h"

/* This test purpose is to verify the behaviour when the program does not link
 * any symbol from the mbed library.
 * In the past we had an issue where the stdio retargeting was not linked in.
 */

/*
    scanf("%d", &Value);
    fprintf(stdout, "Test 3: fprintf(stdout, ...) test\r\n");
    fprintf(stderr, "Test 4: fprintf(stderr, ...) test\r\n");
    fscanf(stdin, "%d", &Value);
    fprintf(stdout, "Test 3: fprintf(stdout, ...) test\r\n");

    fprintf(stderr, "Test 4: fprintf(stderr, ...) test\r\n");

    printf("Test 5: fscanf(stdin, ...) test\r\n");
    printf("    Type number and press Enter: \n");
    fscanf(stdin, "%d", &Value);
    printf("\n    Your value was: %d\r\n", Value);

    printf("Test complete\r\n");

*/

int main() {
    union {
        int value_int;
    };

    while (true)
    {
        // SCANFm PRINTF family
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
