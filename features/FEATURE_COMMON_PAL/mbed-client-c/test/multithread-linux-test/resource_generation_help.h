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

#ifndef RESOURCE_GENERATION_HELP_H_
#define RESOURCE_GENERATION_HELP_H_

/*
 * A helper macro to create a static resoure
 */
#define CREATE_STATIC_RESOURCE(resource_structure, pt_len, pt, rpp_len, rpp_ptr, rsc, rsc_len, handle) 		\
{																									\
	resource_structure->access = (sn_grs_resource_acl_e)0xff;										\
	resource_structure->mode = SN_GRS_STATIC;														\
	resource_structure->pathlen = pt_len; 															\
	resource_structure->path = pt; 																	\
	resource_structure->resource_parameters_ptr->resource_type_len = rpp_len; 						\
	resource_structure->resource_parameters_ptr->resource_type_ptr = rpp_ptr; 						\
	resource_structure->resource = rsc; 															\
	resource_structure->resourcelen = rsc_len;														\
    sn_nsdl_create_resource(handle,resource_structure); 													\
}


/*
 * A helper macro to create a dynamic resoure
 */
#define CREATE_DYNAMIC_RESOURCE(resource_structure, pt_len, pt, rpp_len, rpp_ptr, is_observable, callback_ptr, handle) 	\
{																									\
	resource_structure->access = (sn_grs_resource_acl_e)0xff;										\
	resource_structure->resource = 0;																\
	resource_structure->resourcelen = 0;															\
	resource_structure->sn_grs_dyn_res_callback = callback_ptr;								\
	resource_structure->mode = SN_GRS_DYNAMIC;														\
	resource_structure->pathlen = pt_len; 															\
	resource_structure->path = pt; 																	\
	resource_structure->resource_parameters_ptr->resource_type_len = rpp_len; 						\
	resource_structure->resource_parameters_ptr->resource_type_ptr = rpp_ptr; 						\
	resource_structure->resource_parameters_ptr->observable = is_observable; 						\
    sn_nsdl_create_resource(handle,resource_structure); 													\
}

#define INIT_REGISTER_NSDL_ENDPOINT(endpoint_structure, name, typename_ptr, lifetime_ptr)				\
{																									\
			if(!endpoint_structure)																	\
			{																						\
                endpoint_structure = own_alloc(sizeof(sn_nsdl_ep_parameters_s));					\
			}																						\
			if(endpoint_structure)																	\
			{																						\
				memset(endpoint_structure, 0, sizeof(sn_nsdl_ep_parameters_s));						\
				endpoint_structure->endpoint_name_ptr = name;										\
				endpoint_structure->endpoint_name_len = sizeof(name)-1;								\
				endpoint_structure->type_ptr = typename_ptr;										\
				endpoint_structure->type_len =  sizeof(typename_ptr)-1;								\
				endpoint_structure->lifetime_ptr = lifetime_ptr;									\
				endpoint_structure->lifetime_len =  sizeof(lifetime_ptr)-1;							\
			}																						\
}

#define CLEAN_REGISTER_NSDL_ENDPOINT(endpoint_structure)											\
{																									\
			if(endpoint_structure)																	\
			{																						\
				own_free(endpoint_structure);														\
				endpoint_structure = 0;																	\
			}																						\
}																									\

#endif /* RESOURCE_GENERATION_HELP_H_ */
