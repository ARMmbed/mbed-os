#include "test_env.h"

int called = 0;

extern "C" void mbed_main() {
    printf("This should be called before main.\n");
    called = 1;
}

int main() {
    printf("main() starts now.\n");
    notify_completion(called == 1);
}
