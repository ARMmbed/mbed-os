/*
 * Copyright (c) 2021, Arm Limited and affiliates
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
#include "netsocket/MsgHeader.h"
#include "netsocket/nsapi_types.h"

struct custom_type_t {
    nsapi_msghdr_t hdr;
    uint64_t custom_field1;
    uint64_t custom_field2;
    uint64_t custom_field3;
    uint64_t custom_field4;
};

struct pktinfo_buffer_t {
    pktinfo_buffer_t()
    {
        pkt_info_1.hdr.len = sizeof(nsapi_pktinfo_t);
        pkt_info_2.hdr.len = sizeof(nsapi_pktinfo_t);
    }
    nsapi_pktinfo_t pkt_info_1;
    nsapi_pktinfo_t pkt_info_2;
};

struct custom_buffer_t {
    custom_buffer_t()
    {
        pkt_info.hdr.len = sizeof(nsapi_pktinfo_t);
        msg_hdr.len = sizeof(nsapi_msghdr_t);
        custom.hdr.len = sizeof(custom_type_t);
    }
    nsapi_pktinfo_t pkt_info;
    nsapi_msghdr_t msg_hdr;
    custom_type_t custom;
};

class TestMsgHeaderIterator : public testing::Test {
protected:
    pktinfo_buffer_t p_buff;
    custom_buffer_t c_buff;

    virtual void SetUp() {}

    virtual void TearDown() {}
};

TEST_F(TestMsgHeaderIterator, pktinfo_list)
{
    nsapi_msghdr_t *hdr = reinterpret_cast<nsapi_msghdr_t *>(&p_buff);
    MsgHeaderIterator it(hdr, sizeof(p_buff));

    EXPECT_EQ(it.next(), &p_buff.pkt_info_1.hdr);
    EXPECT_EQ(it.next(), &p_buff.pkt_info_2.hdr);
    EXPECT_TRUE(it.next() == nullptr);
}

TEST_F(TestMsgHeaderIterator, custom_list)
{
    nsapi_msghdr_t *hdr_c = reinterpret_cast<nsapi_msghdr_t *>(&c_buff);
    MsgHeaderIterator it(hdr_c, sizeof(c_buff));

    EXPECT_EQ(it.next(), &c_buff.pkt_info.hdr);
    EXPECT_EQ(it.next(), &c_buff.msg_hdr);
    EXPECT_EQ(it.next(), &c_buff.custom.hdr);
    EXPECT_TRUE(it.next() == nullptr);
}

TEST_F(TestMsgHeaderIterator, null_list)
{
    MsgHeaderIterator it(nullptr, sizeof(p_buff));

    EXPECT_TRUE(it.next() == nullptr);
}

TEST_F(TestMsgHeaderIterator, wrong_list_size)
{
    nsapi_msghdr_t *hdr = reinterpret_cast<nsapi_msghdr_t *>(&p_buff);
    MsgHeaderIterator it(hdr, 0);

    EXPECT_TRUE(it.next() == nullptr);
}

TEST_F(TestMsgHeaderIterator, wrong_member_len)
{
    nsapi_msghdr_t *hdr = reinterpret_cast<nsapi_msghdr_t *>(&p_buff);
    MsgHeaderIterator it(hdr, sizeof(p_buff));
    p_buff.pkt_info_1.hdr.len = 0;

    EXPECT_TRUE(it.next() == nullptr);
}
