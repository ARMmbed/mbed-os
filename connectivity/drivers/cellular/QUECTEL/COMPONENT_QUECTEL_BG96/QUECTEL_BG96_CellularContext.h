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
#ifndef QUECTEL_BG96_CELLULARCONTEXT_H_
#define QUECTEL_BG96_CELLULARCONTEXT_H_

#include "AT_CellularContext.h"

namespace mbed {

class QUECTEL_BG96_CellularContext: public AT_CellularContext {
public:
    QUECTEL_BG96_CellularContext(ATHandler &at, CellularDevice *device, const char *apn, bool cp_req = false, bool nonip_req = false);
    virtual ~QUECTEL_BG96_CellularContext();

protected:
#if !NSAPI_PPP_AVAILABLE
    virtual NetworkStack *get_stack();
#endif // #if !NSAPI_PPP_AVAILABLE
    virtual ControlPlane_netif *get_cp_netif();
    virtual nsapi_error_t do_user_authentication();
    virtual nsapi_error_t activate_non_ip_context();
    virtual nsapi_error_t setup_control_plane_opt();
    virtual void deactivate_non_ip_context();
    virtual void deactivate_context();
    virtual void activate_context();

private:
    void urc_nidd();
    void urc_nidd_open();
    void urc_nidd_close();
};

} /* namespace mbed */

#endif // QUECTEL_BG96_CELLULARCONTEXT_H_
