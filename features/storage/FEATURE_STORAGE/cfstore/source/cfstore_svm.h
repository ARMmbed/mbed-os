/*
 * mbed Microcontroller Library
 * Copyright (c) 2006-2016 ARM Limited
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
 *
 */

/** @file  cfstore_svm.h
 *
 * This is the interface file to configuration store storage volume manager.
 */

#ifndef __CFSTORE_SVM_H_
#define __CFSTORE_SVM_H_

#include <Driver_Storage.h>

#ifdef __cplusplus
extern "C" {
#endif


int32_t cfstore_svm_init(struct _ARM_DRIVER_STORAGE *mtd);


#ifdef __cplusplus
}
#endif

#endif /*__CFSTORE_SVM_H_ */
