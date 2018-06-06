/*
 * Copyright (c) 2014-2015, 2017, Arm Limited and affiliates.
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

#ifndef NS_ERROR_TYPES_H_
#define NS_ERROR_TYPES_H_

/**
 * \enum error_t
 * \brief System generic error.
 */
typedef enum error_t {
    eOK = 0,    /*!< no error */
    eFALSE = 1, /*!< no result */
    eBUSY = 2,  /*!< resource busy */
    eSYSTEM     /*!< error code readable in sys_error */
} error_t;

#endif /* NS_ERROR_TYPES_H_ */
