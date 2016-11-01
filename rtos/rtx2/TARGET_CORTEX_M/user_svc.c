/*
 * Copyright (c) 2013-2016 ARM Limited. All rights reserved.
 *
 * SPDX-License-Identifier: Apache-2.0
 *
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
 *
 * -----------------------------------------------------------------------------
 *
 * Project:     CMSIS-RTOS RTX
 * Title:       User SVC Table
 *
 * -----------------------------------------------------------------------------
 */

#define USER_SVC_COUNT  0       // Number of user SVC functions

extern void * const os_UserSVC_Table[1+USER_SVC_COUNT];
       void * const os_UserSVC_Table[1+USER_SVC_COUNT] = {
  (void *)USER_SVC_COUNT,
//(void *)user_function1,
// ...
};
