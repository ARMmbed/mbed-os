#include "gtest/gtest.h"
#include "features/netsocket/TCPSocket.h"

class TestTCPSocket : public testing::Test {
protected:
    TCPSocket *socket;

    virtual void SetUp()
    {
        socket = new TCPSocket();
    }

    virtual void TearDown()
    {
        delete socket;
    }
};

TEST_F(TestTCPSocket, constructor)
{
    EXPECT_TRUE(socket);
}