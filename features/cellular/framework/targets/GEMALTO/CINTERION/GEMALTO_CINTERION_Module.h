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

#ifndef GEMALTO_CINTERION_MODULE_H_
#define GEMALTO_CINTERION_MODULE_H_

#include "nsapi_types.h"

namespace mbed {

class FileHandle;

class GEMALTO_CINTERION_Module {
public:
    /** Actual model of cellular module is needed to make AT command adaptation at runtime
     *  to support many different models in one cellular driver.
     */
    enum Model {
        ModelUnknown = 0,
        ModelELS61,
        ModelBGS2,
        ModelEMS31,
    };
    static nsapi_error_t detect_model(const char *model);
    static Model get_model();

private:
    static Model _model;
};

} // namespace mbed

#endif // GEMALTO_CINTERION_MODULE_H_
