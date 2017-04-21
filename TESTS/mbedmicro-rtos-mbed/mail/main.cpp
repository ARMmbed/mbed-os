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

#define STACK_SIZE 1024

Mail<mail_t, QUEUE_SIZE> mail_box;

void send_thread (void const *argument) {
    static uint32_t i = 10;
    while (true) {
        i++; // fake data update
        mail_t *mail = mail_box.alloc();
        mail->voltage = CREATE_VOLTAGE(i);
        mail->current = CREATE_CURRENT(i);
        mail->counter = i;
        mail_box.put(mail);
        Thread::wait(QUEUE_PUT_DELAY);
    }
}

int main (void) {
    GREENTEA_SETUP(20, "default_auto");

    Thread thread(send_thread, NULL, osPriorityNormal, STACK_SIZE);
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
    GREENTEA_TESTSUITE_RESULT(result);
    return 0;
}
