#include "mbed.h"
#include "rtos.h"

/* Mail */
typedef struct {
  float    voltage; /* AD result of measured voltage */
  float    current; /* AD result of measured current */
  uint32_t counter; /* A counter value               */
} mail_t;

Mail<mail_t, 16> mail_box;

void send_thread (void const *argument) {
    uint32_t i = 0;
    while (true) {
        i++; // fake data update
        mail_t *mail = mail_box.alloc();
        mail->voltage = (i * 0.1) * 33; 
        mail->current = (i * 0.1) * 11;
        mail->counter = i;
        mail_box.put(mail);
        Thread::wait(1000);
    }
}

int main (void) {
    Thread thread(send_thread);
    
    while (true) {
        osEvent evt = mail_box.get();
        if (evt.status == osEventMail) {
            mail_t *mail = (mail_t*)evt.value.p;
            printf("\nVoltage: %.2f V\n\r"   , mail->voltage);
            printf("Current: %.2f A\n\r"     , mail->current);
            printf("Number of cycles: %u\n\r", mail->counter);
            
            mail_box.free(mail);
        }
    }
}
