/* mbed Microcontroller Library
 * Copyright (c) 2017 ARM Limited
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */
#include "greentea-client/test_env.h"

#define PATTERN_CHECK_VALUE  0xF0F0ADAD

class Test {

private:
    const char* name;
    const int pattern;

public:
    Test(const char* _name, bool print_message=true) : name(_name), pattern(PATTERN_CHECK_VALUE)  {
        if (print_message) {
            print("init");
        }
    }

    void print(const char *message) {
        printf("%s::%s\n", name, message);
    }

    bool check_init(void) {
        bool result = (pattern == PATTERN_CHECK_VALUE);
        print(result ? "check_init: OK" : "check_init: ERROR");
        return result;
    }

    void stack_test(void) {
        print("stack_test");
        Test t("Stack");
        t.hello();
    }

    void hello(void) {
        print("hello");
    }

    ~Test() {
        print("destroy");
    }
};

/* Check C++ startup initialisation */
Test s("Static", false);

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
    GREENTEA_SETUP(10, "default_auto");

    bool result = true;
    for (;;)
    {
        s.print("init");
        // Global stack object simple test
        s.stack_test();
        if (s.check_init() == false)
        {
            result = false;
            break;
        }

        // Heap test object simple test
        Test *m = new Test("Heap");
        m->hello();

        if (m->check_init() == false)
        {
            result = false;
        }
        delete m;
        break;
    }

    GREENTEA_TESTSUITE_RESULT(result);
}
