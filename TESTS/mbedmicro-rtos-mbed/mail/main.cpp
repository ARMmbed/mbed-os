#include "mbed.h"
#include "greentea-client/test_env.h"
#include "rtos.h"

#if defined(MBED_RTOS_SINGLE_THREAD)
  #error [NOT_SUPPORTED] test not supported
#endif

typedef struct {
    float    voltage;   /* AD result of measured voltage */
    float    current;   /* AD result of measured current */
    uint32_t counter;   /* A counter value               */
} mail_t;

#define CREATE_VOLTAGE(COUNTER) (COUNTER * 0.1) * 33
#define CREATE_CURRENT(COUNTER) (COUNTER * 0.1) * 11
#define QUEUE_SIZE       16
#define QUEUE_PUT_DELAY  100

/*
 * The stack size is defined in cmsis_os.h mainly dependent on the underlying toolchain and
 * the C standard library. For GCC, ARM_STD and IAR it is defined with a size of 2048 bytes
 * and for ARM_MICRO 512. Because of reduce RAM size some targets need a reduced stacksize.
 */
#if (defined(TARGET_STM32L053R8) || defined(TARGET_STM32L053C8)) && defined(TOOLCHAIN_GCC)
    #define STACK_SIZE DEFAULT_STACK_SIZE/4
#elif (defined(TARGET_STM32F030R8)) && defined(TOOLCHAIN_GCC)
    #define STACK_SIZE DEFAULT_STACK_SIZE/2
#elif (defined(TARGET_STM32F030R8)) && defined(TOOLCHAIN_IAR)
    #define STACK_SIZE DEFAULT_STACK_SIZE/2
#elif (defined(TARGET_EFM32HG_STK3400)) && !defined(TOOLCHAIN_ARM_MICRO)
    #define STACK_SIZE 512
#elif (defined(TARGET_EFM32LG_STK3600) || defined(TARGET_EFM32WG_STK3800) || defined(TARGET_EFM32PG_STK3401)) && !defined(TOOLCHAIN_ARM_MICRO)
    #define STACK_SIZE 768
#elif (defined(TARGET_EFM32GG_STK3700)) && !defined(TOOLCHAIN_ARM_MICRO)
    #define STACK_SIZE 1536
#elif defined(TARGET_MCU_NRF51822) || defined(TARGET_MCU_NRF52832)
    #define STACK_SIZE 768
#else
    #define STACK_SIZE DEFAULT_STACK_SIZE
#endif

Mail<mail_t, QUEUE_SIZE> mail_box;

void send_thread (void const *argument) {
    uint32_t i = 10;
    GenericMail<mail_t> *p_mail = (GenericMail<mail_t>*)argument;

    while (true) {
        i++; // fake data update
        mail_t *mail = p_mail->alloc();
        mail->voltage = CREATE_VOLTAGE(i);
        mail->current = CREATE_CURRENT(i);
        mail->counter = i;
        p_mail->put(mail);
        Thread::wait(QUEUE_PUT_DELAY);
    }
}

bool test_once(GenericMail<mail_t>& mail_box) {
    bool result = true;
    int result_counter = 0;

    while (true) {
        osEvent evt = mail_box.get();
        if (evt.status == osEventMail) {
            mail_t *mail = (mail_t*)evt.value.p;
            const float expected_voltage = CREATE_VOLTAGE(mail->counter);
            const float expected_current = CREATE_CURRENT(mail->counter);
            // Check using macros if received values correspond to values sent via queue
            bool expected_values = (expected_voltage == mail->voltage) &&
                                   (expected_current == mail->current);
            result = result && expected_values;
            const char *result_msg = expected_values ? "OK" : "FAIL";
            printf("%3d %.2fV %.2fA ... [%s]\r\n", mail->counter,
                                                   mail->voltage,
                                                   mail->current,
                                                   result_msg);
            mail_box.free(mail);
            if (result == false || ++result_counter == QUEUE_SIZE) {
                break;
            }
        }
    }
    return result;
}

int main (void) {
    GREENTEA_SETUP(20, "default_auto");

    Thread thread(osPriorityNormal, STACK_SIZE);

    printf("Testing Mail\r\n");
    thread.start(callback(&mail_box, send_thread));
    bool result = test_once(mail_box);
    thread.terminate();
    if (!result) {
        printf("Mail tests failed!\r\n");
        GREENTEA_TESTSUITE_RESULT(false);
        return 0;
    }

    printf("Testing DynamicMail\r\n");
    DynamicMail<mail_t> dyn_mail(QUEUE_SIZE);
    thread.start(callback(&dyn_mail, send_thread));
    result = test_once(dyn_mail);
    if (!result) {
        printf("DynamicMail tests failed!\r\n");
    }
    GREENTEA_TESTSUITE_RESULT(result);
    return 0;
}
