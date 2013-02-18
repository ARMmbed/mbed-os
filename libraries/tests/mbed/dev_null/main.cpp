#include "mbed.h"

class DevNull : public Stream {

public:
    DevNull(const char *name=NULL) : Stream(name) {}

protected:
    virtual int _getc()      {return 0;}
    virtual int _putc(int c) {return c;}
};

DevNull null("null");

int main() {
    printf("re-routing stdout to /null\n");
    
    freopen("/null", "w", stdout);
    printf("printf redirected to /null\n");
    
    DigitalOut led(LED1);
    while (true) {
        led = !led;
        wait(1);
    }
}
