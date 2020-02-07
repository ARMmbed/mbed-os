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
#include "gtest/gtest.h"
#include <string.h>

#include "CellularContext.h"
#include "CellularDevice_stub.h"
#include "ControlPlane_netif_stub.h"
#include "myCellularDevice.h"

using namespace mbed;

// AStyle ignored as the definition is not clear due to preprocessor usage
// *INDENT-OFF*
class TestCellularContext : public testing::Test {
protected:

    void SetUp()
    {
        CellularDevice_stub::create_in_get_default = true;
    }

    void TearDown()
    {
    }
};

class testContext : public CellularContext
{
public:

	testContext(CellularDevice *dev = NULL)
	{
		_device = dev;
		_cp_netif = new ControlPlane_netif_stub();

		nonip_pdp_string = NULL;
	}

	~testContext()
	{
		delete _cp_netif;
	}

	int get_retry_count()
	{
		return _retry_count;
	}
	CellularContext::AuthenticationType get_auth_type()
	{
		return _authentication_type;
	}
	nsapi_error_t set_blocking(bool blocking)
	{
		_is_blocking = blocking;
		return NSAPI_ERROR_OK;
	}

	NetworkStack *get_stack()
	{
		return NULL;
	}

	nsapi_error_t get_ip_address(SocketAddress *address)
    {
        return NSAPI_ERROR_UNSUPPORTED;
    }
	const char *get_ip_address()
	{
		return NULL;
	}
	virtual void attach(mbed::Callback<void(nsapi_event_t, intptr_t)> status_cb)
	{
		_status_cb = status_cb;
	}
	virtual nsapi_error_t connect()
	{
		return NSAPI_ERROR_OK;
	}

	virtual nsapi_error_t disconnect()
	{
		return NSAPI_ERROR_OK;
	}

	virtual void set_plmn(const char *plmn)
	{
	}
	virtual void set_sim_pin(const char *sim_pin)
	{
	}
	virtual nsapi_error_t connect(const char *sim_pin, const char *apn = 0, const char *uname = 0,
								  const char *pwd = 0)
	{
		return NSAPI_ERROR_OK;
	}
	virtual void set_credentials(const char *apn, const char *uname = 0, const char *pwd = 0)
	{
	}
	virtual bool is_connected()
	{
		return false;
	}
	nsapi_error_t set_device_ready()
	{
		return NSAPI_ERROR_OK;
	}
	nsapi_error_t set_sim_ready()
	{
		return NSAPI_ERROR_OK;
	}
	nsapi_error_t register_to_network()
	{
		return NSAPI_ERROR_OK;
	}
	nsapi_error_t attach_to_network()
	{
		return NSAPI_ERROR_OK;
	}
	nsapi_error_t get_rate_control(CellularContext::RateControlExceptionReports &reports,
	                                           CellularContext::RateControlUplinkTimeUnit &time_unit, int &uplink_rate)
	{
		return NSAPI_ERROR_OK;
	}
	nsapi_error_t get_pdpcontext_params(pdpContextList_t &params_list)
	{
		return NSAPI_ERROR_OK;
	}
	nsapi_error_t get_apn_backoff_timer(int &backoff_timer)
	{
		return NSAPI_ERROR_OK;
	}
	void set_file_handle(FileHandle *fh)
	{

	}
#if (DEVICE_SERIAL && DEVICE_INTERRUPTIN) || defined(DOXYGEN_ONLY)
    virtual nsapi_error_t configure_hup(PinName dcd_pin = NC, bool active_high = false)
    {
        return NSAPI_ERROR_OK;
    }
#endif
	ControlPlane_netif *get_cp_netif()
	{
		return _cp_netif;
	}
	void cellular_callback(nsapi_event_t ev, intptr_t ptr)
	{

	}
	void enable_hup(bool enable)
	{

	}

    const char *get_nonip_context_type_str()
    {
        return nonip_pdp_string;
    }

	void cp_data_received()
	{
		CellularContext::cp_data_received();
	}

	virtual void do_connect()
	{
	}

	void set_cell_callback_data(cell_callback_data_t &cb_data)
	{
		_cb_data = cb_data;
	}

	void do_connect_with_retry()
	{
		CellularContext::do_connect_with_retry();
	}

	void test_string_to_pdp_type()
	{
	    pdp_type_t type = string_to_pdp_type("IPV4V6");
	    ASSERT_EQ(type, IPV4V6_PDP_TYPE);

	    type = string_to_pdp_type("IPV6");
	    ASSERT_EQ(type, IPV6_PDP_TYPE);

	    type = string_to_pdp_type("IP");
	    ASSERT_EQ(type, IPV4_PDP_TYPE);

	    type = string_to_pdp_type("Non-IP");
	    ASSERT_EQ(type, NON_IP_PDP_TYPE);

	    nonip_pdp_string = NULL;
	    type = string_to_pdp_type("diipadaapa");
	    ASSERT_EQ(type, DEFAULT_PDP_TYPE);
	}

    void test_nonip_context_type_str()
    {
        nonip_pdp_string = "NONIP";

        pdp_type_t type = string_to_pdp_type("diipadaapa");
        ASSERT_EQ(type, DEFAULT_PDP_TYPE);

        type = string_to_pdp_type("NONIP");
        ASSERT_EQ(type, NON_IP_PDP_TYPE);

        type = string_to_pdp_type("nonip");
        ASSERT_EQ(type, DEFAULT_PDP_TYPE);

        type = string_to_pdp_type("IPV6");
        ASSERT_EQ(type, IPV6_PDP_TYPE);

        nonip_pdp_string = "testnonip";

        type = string_to_pdp_type("diipadaapa");
        ASSERT_EQ(type, DEFAULT_PDP_TYPE);

        type = string_to_pdp_type("testnonip");
        ASSERT_EQ(type, NON_IP_PDP_TYPE);

        type = string_to_pdp_type("nonip");
        ASSERT_EQ(type, DEFAULT_PDP_TYPE);

        type = string_to_pdp_type("IPV6");
        ASSERT_EQ(type, IPV6_PDP_TYPE);
    }

	const char *nonip_pdp_string;
};

static int network_cb_count = 0;
static void network_cb(nsapi_event_t ev, intptr_t intptr)
{
    network_cb_count++;
}

// *INDENT-ON*
TEST_F(TestCellularContext, test_create_delete)
{
    CellularDevice_stub::create_in_get_default = false;
    CellularContext *ctx = CellularContext::get_default_instance();
    EXPECT_TRUE(ctx == NULL);

    ctx = CellularContext::get_default_nonip_instance();
    EXPECT_TRUE(ctx == NULL);

    CellularDevice_stub::create_in_get_default = true;
    ctx = CellularContext::get_default_instance();
    EXPECT_TRUE(ctx != NULL);

    ctx = CellularContext::get_default_nonip_instance();
    EXPECT_TRUE(ctx != NULL);
}

TEST_F(TestCellularContext, get_device)
{
    CellularContext *ctx = CellularContext::get_default_instance();
    EXPECT_TRUE(ctx != NULL);

    CellularDevice *dev = ctx->get_device();
    EXPECT_TRUE(dev != NULL);
}

TEST_F(TestCellularContext, get_cid)
{
    CellularContext *ctx = CellularContext::get_default_instance();
    EXPECT_TRUE(ctx != NULL);

    int cid = ctx->get_cid();
    ASSERT_EQ(cid,  -1);
}

TEST_F(TestCellularContext, set_authentication_type)
{
    testContext *ctx = new testContext();
    EXPECT_TRUE(ctx != NULL);

    ASSERT_EQ(ctx->get_auth_type(), CellularContext::CHAP);
    ctx->set_authentication_type(CellularContext::PAP);
    ASSERT_EQ(ctx->get_auth_type(), CellularContext::PAP);

    delete ctx;
}

TEST_F(TestCellularContext, cp_data_received)
{
    testContext *ctx = new testContext();
    EXPECT_TRUE(ctx != NULL);

    ControlPlane_netif_stub *netif = (ControlPlane_netif_stub *)ctx->get_cp_netif();
    EXPECT_TRUE(!netif->cp_data_received_called);
    ctx->cp_data_received();
    EXPECT_TRUE(netif->cp_data_received_called);

    delete ctx;
}

TEST_F(TestCellularContext, do_connect_with_retry)
{
    testContext *ctx = new testContext();
    EXPECT_TRUE(ctx != NULL);
    ctx->attach(network_cb);

    cell_callback_data_t cb_data;
    cb_data.final_try = true;
    ctx->set_cell_callback_data(cb_data);
    ctx->do_connect_with_retry();
    ASSERT_EQ(network_cb_count, 0);

    cb_data.error = NSAPI_ERROR_OK;
    cb_data.final_try = false;
    ctx->set_cell_callback_data(cb_data);
    ctx->do_connect_with_retry();

    CellularDevice_stub::retry_array_length = 2;
    cb_data.error = NSAPI_ERROR_DEVICE_ERROR;
    cb_data.final_try = false;
    ctx->set_cell_callback_data(cb_data);
    ctx->do_connect_with_retry();
    ASSERT_EQ(ctx->get_retry_count(), 2);

    delete ctx;
}

TEST_F(TestCellularContext, do_connect_with_retry_async)
{
    myCellularDevice *dev = new myCellularDevice(0);
    testContext *ctx = new testContext(dev);
    EXPECT_TRUE(ctx != NULL);
    ctx->attach(network_cb);
    ASSERT_EQ(NSAPI_ERROR_OK, ctx->set_blocking(false));

    cell_callback_data_t cb_data;
    CellularDevice_stub::retry_array_length = 2;
    cb_data.error = NSAPI_ERROR_DEVICE_ERROR;
    cb_data.final_try = false;
    ctx->set_cell_callback_data(cb_data);
    ctx->do_connect_with_retry();
    ASSERT_EQ(ctx->get_retry_count(), 1);

    delete ctx;
    delete dev;
}

TEST_F(TestCellularContext, string_to_pdp_type)
{
    testContext *ctx = new testContext();
    EXPECT_TRUE(ctx != NULL);

    ctx->test_string_to_pdp_type();
    delete ctx;
}

TEST_F(TestCellularContext, nonip_context_type_str)
{
    testContext *ctx = new testContext();
    EXPECT_TRUE(ctx != NULL);

    ctx->test_nonip_context_type_str();
    delete ctx;
}
