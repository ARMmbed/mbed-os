#include "mbed.h"
#include "test_env.h"

#if !DEVICE_I2C || !DEVICE_I2C_ASYNCH
  #error [NOT_SUPPORTED] I2C ASYNCH is not supported
#endif

#define SIZE (10)
#define ADDR (0x90)
#define FREQ 100000

#if defined(TARGET_KL25Z)
I2C i2c(PTE0, PTE1);
#elif defined(TARGET_nRF51822)
I2C i2c(p22,p20);
#elif defined(TARGET_NUCLEO_F411RE) || defined (TARGET_DISCO_F469NI)
#define TEST_SDA_PIN PB_9
#define TEST_SCL_PIN PB_8
I2C i2c(TEST_SDA_PIN, TEST_SCL_PIN); // I2C_1 (Arduino: D14/D15)
#elif defined(TARGET_FF_ARDUINO) || defined(TARGET_MAXWSNENV)
I2C i2c(I2C_SDA, I2C_SCL);
#define TEST_SDA_PIN PB_9
#define TEST_SCL_PIN PB_8
#elif defined(TARGET_EFM32LG_STK3600) || defined(TARGET_EFM32GG_STK3700) || defined(TARGET_EFM32WG_STK3800)
#define TEST_SDA_PIN PD6
#define TEST_SCL_PIN PD7
I2C i2c(TEST_SDA_PIN, TEST_SCL_PIN);
#elif defined(TARGET_EFM32ZG_STK3200)
#define TEST_SDA_PIN PE12
#define TEST_SCL_PIN PE13
I2C i2c(TEST_SDA_PIN, TEST_SCL_PIN);
#elif defined(TARGET_EFM32HG_STK3400)
#define TEST_SDA_PIN PD6
#define TEST_SCL_PIN PD7
I2C i2c(TEST_SDA_PIN, TEST_SCL_PIN);
#elif defined(TARGET_EFM32PG_STK3401)
#define TEST_SDA_PIN PC10
#define TEST_SCL_PIN PC11
I2C i2c(TEST_SDA_PIN, TEST_SCL_PIN);
#elif defined(TARGET_SAMR21G18A)
#define TEST_SDA_PIN PA16
#define TEST_SCL_PIN PA17
I2C i2c(TEST_SDA_PIN, TEST_SCL_PIN);
#elif defined(TARGET_SAMD21J18A) || defined(TARGET_SAMD21G18A) || defined(TARGET_SAML21J18A)
#define TEST_SDA_PIN PA08
#define TEST_SCL_PIN PA09
I2C i2c(TEST_SDA_PIN, TEST_SCL_PIN);
#elif defined(TARGET_SAMG55J19)
#define TEST_SDA_PIN PA10
#define TEST_SCL_PIN PA09
I2C i2c(TEST_SDA_PIN, TEST_SCL_PIN);
#else
//I2C i2c(p28, p27);
#endif

volatile int why;
volatile bool complete = false;
void cbdone(int event) {
    complete = true;
    why = event;
}


int main() {
    
    event_callback_t callback;
    char buf[] = {3, 2, 1, 4, 5, 6, 7, 8, 9, 10};
    char res[SIZE];
    char buf_master_tx[] = {9, 8, 7, 6, 5, 4, 3, 2, 1, 0};
    char buf_master_rx[SIZE];
    int rc;
    
    printf("\n\nTest init\n");
    
    callback.attach(cbdone);

    i2c.frequency(FREQ);
    
    // First transfer: Tx/Rx
    printf("\nFirst transfer: Master Tx\n");    
    rc = i2c.transfer(ADDR, buf, SIZE, 0, 0, callback, I2C_EVENT_ALL, true);
    if (rc != 0) {
        notify_completion(false);
    }
    while (!complete) {
        sleep();
    }
    if (why != I2C_EVENT_TRANSFER_COMPLETE) {
        notify_completion(false);
    }    
    printf("Why: %d\n", (int)why);
    why = 0;
    complete = false;    

    // Second transfer: Rx
    printf("\nSecond transfer: Master Rx\n");    
    rc = i2c.transfer(ADDR, 0, 0, res, SIZE, callback, I2C_EVENT_ALL, false);
    if (rc != 0) {
        notify_completion(false);
    }
    while (!complete) {
        sleep();
    }
    if (why != I2C_EVENT_TRANSFER_COMPLETE) {
        notify_completion(false);
    }
    printf("Why: %d\n", (int)why);
    
    // Check buffers
    for(int i = 0; i < SIZE; i++) {
        if (res[i] != (buf[i] + 1)) {
            printf("Buffer check KO\n");
            notify_completion(false);
            break;
        }
    }

    // Third transfer: Tx/Rx
    printf("\nThird transfer: Master Tx/Rx\n");
    rc = i2c.transfer(ADDR, buf_master_tx, SIZE, buf_master_rx, SIZE, callback, I2C_EVENT_ALL, false);
    if (rc != 0) {
        notify_completion(false);
    }
    while (!complete) {
    sleep();
    }
    if (why != I2C_EVENT_TRANSFER_COMPLETE) {
        printf("Transfer result: 0x%x\n", why);
        notify_completion(false);
    }
    complete = false;
    why = 0;
    printf("Transfer result: OK\n");

    for(int i = 0; i < SIZE; i++) {
        if (buf_master_rx[i] != (buf_master_tx[i] + 1)) {
            printf("Buffer check KO\n");
            printf("buf_master_rx[%d]: %d, buf_master_tx[%d]: %d\n",i,buf_master_rx[i],i,buf_master_tx[i]);
            notify_completion(false);
            break;
        }
    }
    printf("Buffer check OK\n");
    notify_completion(true);
}
