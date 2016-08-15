/* This is a copy of the basic_test unit test without a custom setup handler being provided.
*/

#include "mbed.h"
#include "greentea-client/test_env.h"
#include "utest/utest.h"
#include "unity/unity.h"
#include "utest/utest_stack_trace.h"

using namespace utest::v1;

void test_simple() {
    UTEST_LOG_FUNCTION();
    TEST_ASSERT_EQUAL(0, 0);
}

utest::v1::status_t test_repeats_setup(const Case *const source, const size_t index_of_case) {
    UTEST_LOG_FUNCTION();
    // Call the default handler for proper reporting
    utest::v1::status_t status = greentea_case_setup_handler(source, index_of_case);
    utest_printf("Setting up for '%s'\n", source->get_description());
    return status;
}
control_t test_repeats(const size_t call_count) {
    UTEST_LOG_FUNCTION();
    TEST_ASSERT_NOT_EQUAL(3, call_count);
    // Specify how often this test is repeated ie. n total calls
    return (call_count < 2) ? CaseRepeatAll : CaseNext;
}


// Specify all your test cases here
Case cases[] = {
    Case("Simple Test", test_simple),
    Case("Repeating Test", test_repeats_setup, test_repeats)
};

// Declare your test specification. If no custom setup handler is provided then
// the default handler will be used.
Specification specification(cases);

extern void utest_run(const Specification& specification);

int main()
{
    UTEST_LOG_FUNCTION();
    Harness::run(specification);
}
