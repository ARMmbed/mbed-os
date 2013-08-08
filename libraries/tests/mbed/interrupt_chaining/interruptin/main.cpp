#include "test_env.h"

DigitalOut myled(LED1);
DigitalOut led2(LED2);
DigitalOut led3(LED3);

volatile int checks = 0;
volatile int total = 0;
void in_handler() {
    checks++;
    led2 = !led2;
}

#if defined(TARGET_KL25Z)
#define PIN_OUT     PTC6
#define PIN_IN      PTA5

#elif defined(TARGET_KL05Z)
#define PIN_OUT     PTB11
#define PIN_IN      PTB1

#elif defined(TARGET_LPC812)
#define PIN_OUT     D10
#define PIN_IN      D11

#elif defined(TARGET_LPC4088)
#define PIN_IN      (p11)
#define PIN_OUT     (p12)

#else
#define PIN_IN      (p5)
#define PIN_OUT     (p25)

#endif

DigitalOut out(PIN_OUT);
InterruptIn in(PIN_IN);

void flipper() {
    for (int i = 0; i < 5; i++) {
        out = 1; myled = 1; wait(0.2);

        out = 0; myled = 0; wait(0.2);
    }
}

void flipper2() {
    led3 = !led3;
}

void handler2() {
    total++;
}

int object_cnt = 0;

class Incrementer {
public:
    Incrementer() : _cnt(0) {}
    void inc() { _cnt++; }
    int get() const { return _cnt; }
private:
    int _cnt;
};

int main() {
    out = 0; myled = 0;

    //Test falling edges first
    Incrementer i1;
    in.rise(NULL);
    in.fall(in_handler);
    in.fall_add(handler2);
    in.fall_add(flipper2);
    in.fall_add_front(&i1, &Incrementer::inc);
    flipper();

    if(checks != 5 || i1.get() != 5) {
        printf("falling edges test failed\n");
        notify_completion(false);
    }

    //Now test rising edges
    Incrementer i2;
    in.rise(in_handler);
    in.rise_add(handler2);
    in.rise_add(&i2, &Incrementer::inc);
    in.rise_add_front(flipper2);
    in.fall(NULL);
    flipper();

    if (checks != 10 || i2.get() != 5) {
        printf("raising edges test failed\n");
        notify_completion(false);
    }

    //Finally test both
    in.rise(in_handler);
    in.rise_add(handler2);
    pFunctionPointer_t rise_led = in.rise_add(flipper2);
    in.fall(in_handler);
    in.fall_add(handler2);
    pFunctionPointer_t fall_led = in.fall_add(flipper2);
    if (!in.rise_remove(rise_led) ||  !in.fall_remove(fall_led)) {
        printf("remove handler failed\n");
        notify_completion(false);
    }
    flipper();

    if (checks != 20) {
        printf("Simultaneous rising and falling edges failed! check=%d\n", checks);
        notify_completion(false);
    }
    printf("Total: %d\n", total);
    notify_completion(true);
    return 0;
}
