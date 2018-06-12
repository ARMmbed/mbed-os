#include "gtest/gtest.h"
#include "features/netsocket/UDPSocket.h"
#include "features/netsocket/nsapi_dns.h"
#include "NetworkStackstub.h"

/**
 * This test needs to access a private function
 * which is why it is declared as friend
 * following the documentation of google test.
 * [*](https://github.com/google/googletest/blob/master/googletest/docs/advanced.md)
 */
class stubUDPSocket : public UDPSocket {
    friend class TestUDPSocket;
    FRIEND_TEST(TestUDPSocket, get_proto);
};

class TestUDPSocket : public testing::Test {
protected:
    stubUDPSocket *socket;
    NetworkStackstub stack;

    virtual void SetUp()
    {
        socket = new stubUDPSocket;
        stack.return_value = NSAPI_ERROR_OK;
        socket->open((NetworkStack *)&stack);
    }

    virtual void TearDown()
    {
        delete socket;
    }
};

TEST_F(TestUDPSocket, get_proto)
{
    EXPECT_EQ(socket->get_proto(), NSAPI_UDP);
}

TEST_F(TestUDPSocket, sendto_addr_port)
{
    stack.return_value = NSAPI_ERROR_PARAMETER;
    EXPECT_EQ(socket->sendto("localhost", 0, 0, 0), NSAPI_ERROR_DNS_FAILURE);
    stack.return_value = NSAPI_ERROR_OK;
    EXPECT_EQ(socket->sendto("localhost", 0, 0, 0), 0);
}

TEST_F(TestUDPSocket, sendto)
{
    SocketAddress addr("127.0.0.1", 1024);
    stack.return_value = NSAPI_ERROR_OK;

    socket->close();
    EXPECT_EQ(socket->sendto(addr, 0, 0), NSAPI_ERROR_NO_SOCKET);

    socket->open((NetworkStack *)&stack);
    stack.return_value = 100;
    EXPECT_EQ(socket->sendto(addr, 0, 100), 100);

    socket->set_timeout(0);
    stack.return_value = NSAPI_ERROR_WOULD_BLOCK;
    EXPECT_EQ(socket->sendto(addr, 0, 100), NSAPI_ERROR_WOULD_BLOCK);

    socket->set_timeout(-1);
    stack.return_value = NSAPI_ERROR_NO_CONNECTION;
    EXPECT_EQ(socket->sendto(addr, 0, 100), NSAPI_ERROR_NO_CONNECTION);
}