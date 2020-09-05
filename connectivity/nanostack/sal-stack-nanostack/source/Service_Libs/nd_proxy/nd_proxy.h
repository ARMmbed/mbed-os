/*
 * Copyright (c) 2015-2018, Arm Limited and affiliates.
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

#ifndef ND_PROXY_H_
#define ND_PROXY_H_

#ifdef HAVE_ND_PROXY
/**
 * Validation callback for destination address validation or Upstream route onlink check
 *
 *\param interface_id Mesh interface Id
 *\param address IPv6 address which need to validate
 *
 *\return 0 This address is behind this interface or Route is not onLink at Upstream
 *\return <0 This address is not registered to this interface or this address is OnLink at this interface
 */
typedef int (nd_proxy_req_cb)(int8_t interface_id, uint8_t *address);

/**
 * Inform Downstram Interface when proxy is enabled and disabled
 *
 *\param upstream_id Upstream interface id
 *\param downstream_id Downstram Interface ID
 *\param status True when proxy is is enabled and false when proxy is disabled
 *
 *\return 0 Process OK
 *\return <0 Process not valid
 */
typedef int (bridge_state_update_cb)(int8_t upstream_id, int8_t downstream_id, bool status);

/**
 * Register connected Downstram interface to nd proxy
 *
 * When Downstram Interface bootstrap interface is ready it can call this.
 * After call proxy will connect this interfaces to all registered Upstream interface's.
 *
 *\param interface_id interface id
 *\param nd_proxy_req function pointer for address validation (Must parameter)
 *\param bridge_state_update function pointer for proxy notify Downstram interface when proxy is enabled and disabled
 *
 *\return 0 Register OK
 *\return <0 Registeration fail
 */
int nd_proxy_downstream_interface_register(int8_t interface_id, nd_proxy_req_cb *nd_proxy_req, bridge_state_update_cb *bridge_state_update);

/**
 * Remove connected Downstram interface from nd proxy
 *
 * This should call when interface close interface or loose connection.
 * After call proxy remove connection from all connected proxy where interface is linked.
 *
 *\param interface_id interface id
 *
 *\return 0 Remove OK
 *\return <0 Remove fail
 */
int nd_proxy_downstream_interface_unregister(int8_t interface_id);

/**
 * Register Upstream interface to proxy
 *
 * When Mesh Interface bootstrap interface is ready it can call this
 * After call proxy will connect all registered mesh interfaces to this interface.
 *
 *\param interface_id interface id
 *
 *\return 0 Remove OK
 *\return <0 Remove fail
 */
int nd_proxy_upstream_interface_register(int8_t interface_id, nd_proxy_req_cb *route_validation_req);
/**
 * Remove connected Upstream interface from nd proxy
 *
 * This should call when interface close interface or loose connection.
 * After call proxy remove connection from all connected proxy where interface is linked.
 *
 *\param interface_id interface id
 *
 *\return 0 Remove OK
 *\return <0 Remove fail
 */
int nd_proxy_upstream_interface_unregister(int8_t interface_id);

/**
 * Upstream interface verify is proxy enabled for this interface
 *
 *\param interface_id interface id
 *
 *\return true Proxy is enabled
 *\return false proxy is disabled
 */
bool nd_proxy_enabled_for_downstream(int8_t interface_id);

/**
 * Downstream interface verify is proxy enabled for this interface
 *
 *\param interface_id interface id
 *
 *\return true Proxy is enabled
 *\return false proxy is disabled
 */
bool nd_proxy_enabled_for_upstream(int8_t interface_id);

/**
 * Upstream interface validate target address from proxy
 *
 *\param upstream_id interface id
 *\param address target address for verify
 *\param downstream_id_ptr pointer where proxy save mesh interface id where target is registered
 *
 *\return true Address validated behind downstream_id_ptr interface
 *\return false Unknown address for this proxy
 */
bool nd_proxy_target_address_validation(int8_t upstream_id, uint8_t *address);
/**
 * Downstream interface validate prefix route on Link status from connected Upstream interface
 *
 *\param downstream_id interface id
 *\param address Validated route prefix
 *
 *\return true Route is On-link at Downstream mesh should remove current route
 *\return false Route is not On-Link Downstream can add On-Link route
 */
bool nd_proxy_upstream_route_onlink(int8_t downstream_id, uint8_t *address);

#else

NS_DUMMY_DEFINITIONS_OK

#define nd_proxy_downstream_interface_register(interface_id, nd_proxy_req, bridge_state_update) -1
#define nd_proxy_downstream_interface_unregister(interface_id) -1
#define nd_proxy_upstream_interface_register(interface_id, route_validation_req) -1
#define nd_proxy_upstream_interface_unregister(interface_id) -1
#define nd_proxy_enabled_for_downstream(interface_id) false
#define nd_proxy_enabled_for_upstream(interface_id) false
#define nd_proxy_target_address_validation(upstream_id, address) false
#define nd_proxy_upstream_route_onlink(downstream_id, address) false

#endif /* HAVE_ND_PROXY */

#endif /* ND_PROXY_H_ */
