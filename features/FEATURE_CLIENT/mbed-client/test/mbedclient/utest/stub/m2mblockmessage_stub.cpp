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
#include "m2mblockmessage_stub.h"

bool m2mblockmessage_stub::is_block_message;
bool m2mblockmessage_stub::is_last_block;
void m2mblockmessage_stub::clear()
{

}

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
#include "mbed-client/m2mblockmessage.h"
#include <assert.h>
#include <cstdlib>
#include <cstring>
M2MBlockMessage::M2MBlockMessage()
{
}

M2MBlockMessage::~M2MBlockMessage()
{

}

void M2MBlockMessage::set_message_info(sn_coap_hdr_s *coap_header)
{

}

void M2MBlockMessage::clear_values()
{

}

bool M2MBlockMessage::is_block_message() const
{
    return m2mblockmessage_stub::is_block_message;
}

uint16_t M2MBlockMessage::block_number() const
{

}

uint32_t M2MBlockMessage::total_message_size() const
{

}

bool M2MBlockMessage::is_last_block() const
{
    return m2mblockmessage_stub::is_last_block;
}

uint8_t* M2MBlockMessage::block_data() const
{

}

uint32_t M2MBlockMessage::block_data_len() const
{

}

M2MBlockMessage::Error M2MBlockMessage::error_code() const
{

}
