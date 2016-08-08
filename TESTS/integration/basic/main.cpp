#include "test_env.h"

int main() {
    GREENTEA_SETUP(15, "default_auto");
    GREENTEA_TESTCASE_START("Integration: Basic");
    GREENTEA_TESTCASE_FINISHED("Basic", 1, 0);
    GREENTEA_TESTSUITE_RESULT(true);
}
