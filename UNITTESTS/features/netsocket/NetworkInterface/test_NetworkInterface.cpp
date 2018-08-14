#include "gtest/gtest.h"
#include "features/netsocket/NetworkInterface.h"

class stubNetworkInterface : public NetworkInterface {
    virtual nsapi_error_t connect()
    {
        return NSAPI_ERROR_OK;
    };
    virtual nsapi_error_t disconnect()
    {
        return NSAPI_ERROR_OK;
    };
    virtual NetworkStack *get_stack()
    {
        return NULL;
    };
};

class TestNetworkInterface : public testing::Test {
protected:
    NetworkInterface *iface;

    virtual void SetUp()
    {
        iface = new stubNetworkInterface();
    }

    virtual void TearDown()
    {
        delete iface;
    }
};

TEST_F(TestNetworkInterface, constructor)
{
    EXPECT_TRUE(iface);
}

TEST_F(TestNetworkInterface, get_mac_address)
{
    char *n = 0;
    EXPECT_EQ(iface->get_mac_address(), n);
}