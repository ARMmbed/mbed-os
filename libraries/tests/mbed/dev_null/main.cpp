#include "mbed.h"
#include "test_env.h"

class DevNull : public Stream {
public:
    DevNull(const char *name = NULL) : Stream(name) {}

protected:
    virtual int _getc() {
        return 0;
    }
    virtual int _putc(int c) {
        return c;
    }
};

DevNull null("null");

int main() {
    TEST_TIMEOUT(20);
    TEST_HOSTTEST(dev_null_auto);
    TEST_DESCRIPTION(stdout redirected to dev null);
    TEST_START("EXAMPLE_1");
    
    printf("MBED: re-routing stdout to /null\r\n");
    freopen("/null", "w", stdout);
    printf("MBED: printf redirected to /null\r\n");   // This shouldn't appear
    // If failure message can be seen test should fail :)
    TEST_RESULT(false);   // This is 'false' on purpose
}
