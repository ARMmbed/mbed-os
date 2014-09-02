#include "mbed.h"
#include "test_env.h"

class DevNull : public Stream
{
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

int main()
{
    printf("MBED: re-routing stdout to /null\r\n");
    freopen("/null", "w", stdout);
    printf("MBED: printf redirected to /null\r\n");   // This shouldn't appear
    // If failure message can be seen test should fail :)
    notify_completion(false);   // This is 'false' on purpose
    return 0;
}
