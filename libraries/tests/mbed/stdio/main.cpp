#include "mbed.h"

/* This test purpose is to verify the behaviour when the program does not link
 * any symbol from the mbed library.
 * In the past we had an issue where the stdio retargeting was not linked in.
 */

int main() {
    int Value = -1;
    
    printf("\r\n\r\nGCC4MBED Test Suite\r\n");
    printf("Standard I/O Unit Tests\r\n");
    
    printf("Test 1: printf() test\r\n");
    
    printf("Test 2: scanf() test\r\n");
    printf("    Type number and press Enter: \n");
    scanf("%d", &Value);
    printf("\n    Your value was: %d\r\n", Value);
    
    fprintf(stdout, "Test 3: fprintf(stdout, ...) test\r\n");
    
    fprintf(stderr, "Test 4: fprintf(stderr, ...) test\r\n");
    
    printf("Test 5: fscanf(stdin, ...) test\r\n");
    printf("    Type number and press Enter: \n");
    fscanf(stdin, "%d", &Value);
    printf("\n    Your value was: %d\r\n", Value);
    
    printf("Test complete\r\n");
}
