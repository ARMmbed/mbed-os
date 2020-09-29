/*
 * Copyright (c) 2019, Arm Limited and affiliates.
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
#include "SAMSUNG_S5JS100_RIL_CellularContext.h"

#if defined(TARGET_S5JS100)
#include "SAMSUNG_S5JS100_L3IP_Driver.h"
#endif
#include "CellularContext.h"

extern char *get_env(const char *name);

namespace mbed {

SAMSUNG_S5JS100_RIL_CellularContext::SAMSUNG_S5JS100_RIL_CellularContext(RIL_CellularDevice &device, const char *apn,
                                                                         bool cp_req, bool nonip_req)
    : RIL_CellularContext(device, apn, cp_req, nonip_req),
      _cp_netif(NULL)
{
	char *env_pdp_type;
	env_pdp_type = get_env((char*)"PDN");
	if(env_pdp_type != 0) {
	 /*_pdp_type_string = get_env((char*)"PDN"); */
		if(!strcmp(env_pdp_type,"IPV4"))
			_pdp_type = IPV4_PDP_TYPE;
		else if(!strcmp(env_pdp_type,"IPV6"))
			_pdp_type = IPV6_PDP_TYPE;
		else if(!strcmp(env_pdp_type,"IPV4V6"))
			_pdp_type = IPV4V6_PDP_TYPE;
		else if(!strcmp(env_pdp_type,"Non-IP"))
			_pdp_type = NON_IP_PDP_TYPE;
		else
			_pdp_type = IPV4V6_PDP_TYPE;
	}
}

SAMSUNG_S5JS100_RIL_CellularContext::~SAMSUNG_S5JS100_RIL_CellularContext()
{
    delete _cp_netif;
    _cp_netif = NULL;

    delete_L3IP_driver();
}

ControlPlane_netif *SAMSUNG_S5JS100_RIL_CellularContext::get_cp_netif()
{
#if defined(TARGET_S5JS100)
    if (!_cp_netif) {
        _cp_netif = new SAMSUNG_S5JS100_RIL_ControlPlane_netif(_ifname);
    }
    return _cp_netif;
#else
    // This class can be used only with S5JS100 target
    MBED_ASSERT(0);
    return NULL;
#endif
}

L3IP *SAMSUNG_S5JS100_RIL_CellularContext::get_L3IP_driver()
{
#if defined(TARGET_S5JS100)
    return new SAMSUNG_S5JS100_L3IP_Driver(_ifname, _mtu);
#else
    // This class can be used only with S5JS100 target
    MBED_ASSERT(0);
    return NULL;
#endif
}

void SAMSUNG_S5JS100_RIL_CellularContext::delete_L3IP_driver()
{
#if defined(TARGET_S5JS100)
    delete (SAMSUNG_S5JS100_L3IP_Driver *)_l3ip_driver;
    _l3ip_driver = NULL;
#endif
}

} /* namespace mbed */
