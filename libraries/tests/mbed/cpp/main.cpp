#include "test_env.h"

#define CHECK    0xF0F0F0F0

class Test {

private:
    char* name;
    int i;

public:
    Test(char *name) {
        this->i = 0xF0F0F0F0;
        this->name = name;
        this->print("init");
    }
    
    void print(char *message) {
        printf("%s::%s\n", this->name, message);
    }
    
    void check_init(void) {
        if (this->i == CHECK) {
            this->print("check_init: OK");
        } else {
            this->print("check_init: ERROR");
            notify_completion(false);
        }
    }
    
    void stack_test(void) {
        this->print("stack_test");
        Test t("Stack");
        t.hello();
    }
    
    void hello(void) {
        this->print("hello");
    }
    
    ~Test() {
        this->print("destroy");
    }
};

/* Check C++ startup initialisation */
Test s("Static");

/* EXPECTED OUTPUT:
*******************
Static::init
Static::stack_test
Stack::init
Stack::hello
Stack::destroy
Static::check_init: OK
Heap::init
Heap::hello
Heap::destroy
*******************/
int main (void) {
    s.stack_test();
    s.check_init();
    
    /* Heap Test */
    Test *m = new Test("Heap");
    m->hello();
    delete m;
    
    notify_completion(true);
}
