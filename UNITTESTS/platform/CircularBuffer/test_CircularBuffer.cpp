#include "gtest/gtest.h"
#include "platform/CircularBuffer.h"

class TestCircularBuffer : public testing::Test {
protected:
    mbed::CircularBuffer<int, 10> *buf;

    virtual void SetUp()
    {
        buf = new mbed::CircularBuffer<int, 10>;
    }

    virtual void TearDown()
    {
        delete buf;
    }
};

TEST_F(TestCircularBuffer, constructor)
{
    EXPECT_TRUE(buf);
}