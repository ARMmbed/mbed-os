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

#ifndef GENERIC_AT3GPP_H_
#define GENERIC_AT3GPP_H_

#include "AT_CellularDevice.h"

namespace mbed {

/**
 * Generic Cellular module which can be used as a default module when porting new cellular module.
 * GENERIC_AT3GPP uses standard 3GPP AT commands (3GPP TS 27.007 V14.5.0 (2017-09)) to communicate with the modem.
 *
 * GENERIC_AT3GPP can be used as a shield for example on top K64F.
 * Cellular example can be used for testing: https://github.com/ARMmbed/mbed-os-example-cellular
 * Add line to mbed_app.json where you define this class as CELLULAR_DEVICE and the correct pins. In cellular example
 * line would be for example just above "target_overrides": {...
 * For example:
 * "macros": ["CELLULAR_DEVICE=GENERIC_AT3GPP", "MDMRXD=PTC16", "MDMTXD=PTC17","MDMRTS=NC", "MDMCTS=NC"],
 * You can define CELLULAR_DEVICE and pins also in ../../../common/CellularTargets.h
 *
 * If new target don't work with GENERIC_AT3GPP then it needs some customizations.
 * First thing to try can be checking/modifying cellular_properties array in GENERIC_AT3GPP.cpp, does the module support
 * these commands or not? Modify array and if that's not enough then some AT_xxx classes might need to be created and
 * methods overridden. Check help how other modules are done what methods they have overridden. Happy porting!
 */
class GENERIC_AT3GPP : public AT_CellularDevice {
public:
    GENERIC_AT3GPP(FileHandle *fh);
    virtual ~GENERIC_AT3GPP();

};
} // namespace mbed
#endif // GENERIC_AT3GPP_H_
