/*
 * Copyright (c) 2018, Arm Limited and affiliates.
 * SPDX-License-Identifier: Apache-2.0
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

#if !defined(MBED_CONF_NSAPI_PRESENT)
#error [NOT_SUPPORTED] A json configuration file is needed. Skipping this build.
#endif

#include "CellularUtil.h" // for CELLULAR_ helper macros
#include "CellularTargets.h"

#ifndef CELLULAR_DEVICE
#error [NOT_SUPPORTED] CELLULAR_DEVICE must be defined
#endif

#ifndef MBED_CONF_APP_CELLULAR_SIM_PIN
#error [NOT_SUPPORTED] SIM pin code is needed. Skipping this build.
#endif

#include "greentea-client/test_env.h"
#include "unity.h"
#include "utest.h"

#include "mbed.h"

#include "CellularConnectionFSM.h"

#if MBED_CONF_CELLULAR_USE_APN_LOOKUP || MBED_CONF_PPP_CELL_IFACE_APN_LOOKUP
#include "APN_db.h"
#endif //MBED_CONF_CELLULAR_USE_APN_LOOKUP || MBED_CONF_PPP_CELL_IFACE_APN_LOOKUP

#include "CellularLog.h"

#define NETWORK_TIMEOUT (180*1000)
#define SOCKET_TIMEOUT (30*1000)

#define ECHO_SERVER_NAME "echo.mbedcloudtesting.com"
#define ECHO_SERVER_UDP_PORT 7

static CellularConnectionFSM::CellularState cellular_target_state;
static UARTSerial cellular_serial(MDMTXD, MDMRXD, MBED_CONF_PLATFORM_DEFAULT_SERIAL_BAUD_RATE);
static rtos::Semaphore network_semaphore(0);
static CellularConnectionFSM cellular;

static SocketAddress echo_server_addr;

class EchoSocket : public UDPSocket {
public:
  EchoSocket(int size) : UDPSocket(), _async_flag(0), _data(0), _size(size) {
  }
  virtual ~EchoSocket() {
    delete _data;
  }
  void set_async(int async) {
      _async_flag = async;
        if (_async_flag) {
            set_blocking(false);
            sigio(callback(this, &EchoSocket::async_callback));
        } else {
            set_blocking(true);
            set_timeout(SOCKET_TIMEOUT);
            sigio(NULL);
        }

	}
	void test_sendto(const char *const hostname = NULL) {
		_data = new uint8_t[_size];
		for (int i=0; i<_size; i++) {
			_data[i] = (uint8_t)rand();
		}
		// clear pending events
		TEST_ASSERT(!(EchoSocket::eventFlags.clear(_async_flag) & osFlagsError));
		if (hostname) {
			TEST_ASSERT(sendto(hostname, ECHO_SERVER_UDP_PORT, _data, _size) == _size);
		} else {
			TEST_ASSERT(sendto(echo_server_addr, _data, _size) == _size);
		}
	}
	void test_recvfrom() {
		if (_async_flag) {
			TEST_ASSERT((EchoSocket::eventFlags.wait_any(_async_flag, SOCKET_TIMEOUT) & (osFlagsError | _async_flag)) == _async_flag);
		}
		uint8_t *buf = new uint8_t[_size];
		memset(buf, 0, _size);
		SocketAddress recv_address;

		TEST_ASSERT(recvfrom(&recv_address, buf, _size) == _size);

		TEST_ASSERT(recv_address == echo_server_addr);
		TEST_ASSERT(memcmp(_data, buf, _size) == 0);
		delete buf;
		delete _data;
		_data = 0;
	}
private:
	void async_callback() {
		EchoSocket::eventFlags.set(_async_flag);
	}
	uint8_t *_data;
	int _size;
	uint32_t _async_flag; // 0 for blocking socket, signal bit for async
	static rtos::EventFlags eventFlags;
};

rtos::EventFlags EchoSocket::eventFlags;

static void network_callback(nsapi_event_t ev, intptr_t ptr)
{
    if (ev == NSAPI_EVENT_CONNECTION_STATUS_CHANGE) {
        if (ptr == NSAPI_STATUS_GLOBAL_UP) {
        	MBED_ASSERT(network_semaphore.release() == osOK);
        }
    }
}

static void udp_network_stack()
{
    cellular.set_serial(&cellular_serial);
    TEST_ASSERT(cellular.init() == NSAPI_ERROR_OK);
#if defined (MDMRTS) && defined (MDMCTS)
	cellular_serial.set_flow_control(SerialBase::RTSCTS, MDMRTS, MDMCTS);
#endif
    cellular.attach(&network_callback);
    TEST_ASSERT(cellular.start_dispatch() == NSAPI_ERROR_OK);
    cellular.set_sim_pin(MBED_CONF_APP_CELLULAR_SIM_PIN);
    cellular_target_state = CellularConnectionFSM::STATE_CONNECTED;
    TEST_ASSERT(cellular.continue_to_state(cellular_target_state) == NSAPI_ERROR_OK);
	TEST_ASSERT(network_semaphore.wait(NETWORK_TIMEOUT) == 1);
}

static void udp_gethostbyname()
{
    TEST_ASSERT(cellular.get_network()->gethostbyname(ECHO_SERVER_NAME, &echo_server_addr) == 0);
    tr_info("HOST: %s", echo_server_addr.get_ip_address());
	echo_server_addr.set_port(7);
	wait(1);
}

static void udp_socket_send_receive()
{
    EchoSocket echo_socket(4);
    TEST_ASSERT(echo_socket.open(cellular.get_network()) == NSAPI_ERROR_OK);
    echo_socket.set_async(0);
    echo_socket.test_sendto();
    echo_socket.test_recvfrom();
    TEST_ASSERT(echo_socket.close() == NSAPI_ERROR_OK);
    wait(1);
}

static void udp_socket_send_receive_async()
{
    EchoSocket echo_socket(4);
    TEST_ASSERT(echo_socket.open(cellular.get_network()) == NSAPI_ERROR_OK);
    echo_socket.set_async(1);
    echo_socket.test_sendto();
    echo_socket.test_recvfrom();
    TEST_ASSERT(echo_socket.close() == NSAPI_ERROR_OK);
    wait(1);
}

using namespace utest::v1;

static utest::v1::status_t greentea_failure_handler(const Case *const source, const failure_t reason)
{
    greentea_case_failure_abort_handler(source, reason);
    return STATUS_ABORT;
}

static Case cases[] = {
	Case("UDP network stack", udp_network_stack, greentea_failure_handler),
	Case("UDP gethostbyname", udp_gethostbyname, greentea_failure_handler),
	Case("UDP socket send/receive", udp_socket_send_receive, greentea_failure_handler),
	Case("UDP socket send/receive async", udp_socket_send_receive_async, greentea_failure_handler),
	//Case("UDP socket multiple simultaneous", udp_socket_multiple_simultaneous, greentea_failure_handler),
};

static utest::v1::status_t test_setup(const size_t number_of_cases)
{
    GREENTEA_SETUP(10*60, "default_auto"); // network registration may take up to 180 seconds, DNS query a couple of minutes, etc.
    return verbose_test_setup_handler(number_of_cases);
}

static Specification specification(test_setup, cases);

int main()
{
	mbed_trace_init();

	return Harness::run(specification);
}
