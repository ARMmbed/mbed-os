/*
 * Copyright (c) 2015 ARM Limited. All rights reserved.
 * SPDX-License-Identifier: Apache-2.0
 * Licensed under the Apache License, Version 2.0 (the License); you may
 * not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an AS IS BASIS, WITHOUT
 * WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include <stdio.h>
#include "mbed-client/m2mstring.h"
#include "testconfig.h"
#include "security.h"

TestConfig::TestConfig() { }

TestConfig::~TestConfig() {	}

/*void TestConfig::set_endpoint_name(const char *name) {
	_endpointName = m2m::String("lwm2mtest-");
	_endpointName += name;

	printf("MBED: endpoint=%s", _endpointName.c_str());
}*/

void TestConfig::setup() {
	_bootstrapAddress = m2m::String("dummy");
	_mdsAddress = m2m::String("coap://ds-test.dev.mbed.com:5683");
	_endpointName = m2m::String(MBED_ENDPOINT_NAME);
	_endpointType = m2m::String("test2");
	_domain = m2m::String(MBED_DOMAIN);
	_port = 5683;
	_lifetime = 2222;

	printf("MBED: test configuration \n");
	printf("mds_server=%s\n", _mdsAddress.c_str());
	printf("endpoint name=%s\n", _endpointName.c_str());
	printf("endpoint type=%s\n", _endpointType.c_str());
	printf("domain=%s\n", _domain.c_str());
	printf("port=%d\n", _port);
	printf("lifetime=%d\n\n", _lifetime);
}

m2m::String& TestConfig::get_domain() {
	return _domain;
}

m2m::String& TestConfig::get_bootstrap_server() {
	return _bootstrapAddress;
}

m2m::String& TestConfig::get_mds_server() {
	return _mdsAddress;
}

m2m::String& TestConfig::get_endpoint_name() {
	return _endpointName;
}

m2m::String& TestConfig::get_endpoint_type() {
	return _endpointType;
}

int TestConfig::get_port() {
	//return _port++;
	return _port;
}

int TestConfig::get_lifetime() {
	return _lifetime;
}

