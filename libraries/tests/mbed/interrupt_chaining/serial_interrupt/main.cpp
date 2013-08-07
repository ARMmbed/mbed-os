#include "mbed.h"
 
DigitalOut led1(LED1);
DigitalOut led2(LED2);

Serial computer(USBTX, USBRX);
 
// This function is called when a character goes into the TX buffer.
void txCallback() {
    led1 = !led1;
}
 
// This function is called when a character goes into the RX buffer.
void rxCallback() {
    led2 = !led2;
    computer.putc(computer.getc());
}

class Counter {
public:
    Counter(const char* name): _name(name), _cnt(0) {}
    void inc() { _cnt++; }
    void show() const { printf("%s: %d\n", _name, _cnt); }
    int get() const { return _cnt; }
    ~Counter() { show(); }
private:
    const char *_name;
    volatile int _cnt;
};
 
int main() {
    printf("start test\n");
    Counter rx("RX bytes"), tx("TX bytes");

    computer.attach(&txCallback, Serial::TxIrq);
    computer.add_handler(&tx, &Counter::inc, Serial::TxIrq);
    computer.attach(&rxCallback, Serial::RxIrq);
    computer.add_handler_front(&rx, &Counter::inc, Serial::RxIrq);

    while (rx.get() < 40);
}
