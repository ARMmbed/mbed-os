#include "mbed.h"
#include "test_env.h"

#if !DEVICE_I2C
  #error [NOT_SUPPORTED] I2C is not supported
#endif

#define SIZE (10)
#define ADDR (0x90)

#if defined(TARGET_KL25Z)
I2C i2c(PTE0, PTE1);
#elif defined(TARGET_nRF51822)
I2C i2c(p22,p20);
#elif defined(TARGET_NUCLEO_F411RE) || defined(TARGET_NUCLEO_F429ZI) || defined(TARGET_NUCLEO_F446RE)
#define TEST_SDA_PIN PB_9
#define TEST_SCL_PIN PB_8
I2C i2c(TEST_SDA_PIN, TEST_SCL_PIN); // I2C_1 (Arduino: D14/D15)
#elif defined(TARGET_FF_ARDUINO) || defined(TARGET_MAXWSNENV)
I2C i2c(I2C_SDA, I2C_SCL);
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
I2C i2c(p28, p27);
#endif

int main() {
    bool success = true;
    char buf[] = {3, 2, 1, 4, 5, 6, 7, 8, 9, 10};
    char res[SIZE];

    i2c.write(ADDR, buf, SIZE);
    i2c.read(ADDR, res, SIZE);

    // here should be buf[all]++
    i2c.write(ADDR, res, SIZE);
    i2c.read(ADDR, res, SIZE);

    // here should be buf[all]+=2
    i2c.write(ADDR, res, SIZE);
    i2c.write(ADDR, res, SIZE);

    // here should be buf[all]+=3
    i2c.read(ADDR, res, SIZE);
    i2c.read(ADDR, res, SIZE);

    for(int i = 0; i < SIZE; i++) {
        if (res[i] != (buf[i] + 3)) {
            success = false;
            break;
        }
    }

    notify_completion(success);
}
