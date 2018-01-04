/* mbed Microcontroller Library
 * Copyright (c) 2016 ARM Limited
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

#include "mbed.h"
#include "utest/utest.h"
#include "unity/unity.h"
#include "greentea-client/test_env.h"
#include "wd_logging.h"

using namespace utest::v1;

void test_version_encoding_read_version() {
	
	uint8_t version = 
		((uint8_t) DigitalIn(Version16) << 4) |
		((uint8_t) DigitalIn(Version8) << 3) |
		((uint8_t) DigitalIn(Version4) << 2) |
		((uint8_t) DigitalIn(Version2) << 1) |
		((uint8_t) DigitalIn(Version1));
	
	wd_log_info("ROUTINGMAX version: %d", version);
	
	// test if read was successful
	TEST_ASSERT_TRUE(version > 0);
}

utest::v1::status_t greentea_failure_handler(const Case *const source, const failure_t reason) {
	greentea_case_failure_abort_handler(source, reason);
	return STATUS_CONTINUE;
}

Case cases[] = {
	Case("VERSION ENCODING read version", test_version_encoding_read_version, greentea_failure_handler)
};

utest::v1::status_t greentea_test_setup(const size_t number_of_cases) {
	GREENTEA_SETUP(20, "default_auto");
	return greentea_test_setup_handler(number_of_cases);
}

Specification specification(greentea_test_setup, cases, greentea_test_teardown_handler);

int main() {
	Harness::run(specification);
}
