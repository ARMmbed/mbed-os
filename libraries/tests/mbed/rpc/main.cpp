#include "mbed.h"
#include "mbed_rpc.h"

float f = 0;

void foo(Arguments *args, Reply *reply) {
    reply->putData<float>(args->getArg<float>() * 3.3);
}

char outbuf[RPC_MAX_STRING];
void test(const char *input, const char *expected) {
    printf("> %s\n", input);
    bool result = RPC::call(input, outbuf);
    
    if (!result) {
        printf("[ERROR] The remote procedure call failed\n");
    } else if (strcmp(outbuf, expected) != 0) {
        printf("[ERROR] \"%s\" != \"%s\"\n", outbuf, expected);
    } else {
        printf("{%s}\n", outbuf);
    }
}

int main() {
    // Variable
    RPCVariable<float> rpc_f(&f, "f");
    test("/f/write 1", "");
    test("/f/read", "1");
    
    // Function
    RPCFunction rpc_foo(&foo, "foo");
    test("/foo/run 1", "3.2999999523162842");
    
    // Class
    RPC::add_rpc_class<RpcDigitalOut>();
    test("/DigitalOut/new LED2 led2", "led2");
    test("/led2/write 1", "");
    test("/led2/read", "1");
    
    // Instance
    RpcDigitalOut rpc_led(LED1, "led1");
    test("/led1/write 1", "");
    test("/led1/read", "1");
    
    // Introspection
    test("/", "led1 led2 foo f DigitalOut RPC");
    test("/f", "read write delete");
    test("/foo", "run delete");
    test("/DigitalOut", "new");
    test("/led1", "write read delete");
    
    // Delete instance
    test("/led2/delete", "");
    test("/", "led1 foo f DigitalOut RPC");
    
    while (true) {
        wait(1);
    }
}
