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
#ifndef CELLULARSTATEMACHINE_STUB_H_
#define CELLULARSTATEMACHINE_STUB_H_

#include "CellularStateMachine.h"
#include <vector>

enum CellularStubState {
    STATE_INIT = 0,
    STATE_POWER_ON,
    STATE_DEVICE_READY,
    STATE_SIM_PIN,
    STATE_SIGNAL_QUALITY,
    STATE_REGISTERING_NETWORK,
    STATE_ATTACHING_NETWORK,
    STATE_MAX_FSM_STATE
};

namespace CellularStateMachine_stub {
extern nsapi_error_t nsapi_error_value;
extern CellularStubState get_current_target_state;
extern CellularStubState get_current_current_state;
extern bool bool_value;
extern std::vector<uint16_t> timeouts;
}


#endif /* CELLULARSTATEMACHINE_STUB_H_ */
