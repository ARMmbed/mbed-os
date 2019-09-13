#include "gtest/gtest.h"
#include "dirname/template.h"

class Testtemplate : public testing::Test {
    virtual void SetUp()
    {
    }

    virtual void TearDown()
    {
    }
};

TEST_F(Testtemplate, constructor)
{
    EXPECT_TRUE(true);
}
