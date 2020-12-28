/*
 * Copyright (c) 2017, Arm Limited and affiliates.
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

#ifndef NANOSTACK_LOCK_GUARD_H_
#define NANOSTACK_LOCK_GUARD_H_

#include "eventOS_scheduler.h"

/**
 * RAII style Nanostack mutex acquisition.
 * Mutex is held until object leaves scope.
 */

class NanostackLockGuard {
public:
    NanostackLockGuard()
    {
        eventOS_scheduler_mutex_wait();
    }

    ~NanostackLockGuard()
    {
        eventOS_scheduler_mutex_release();
    }

    NanostackLockGuard(const NanostackLockGuard &) = delete;
    NanostackLockGuard &operator=(const NanostackLockGuard &) = delete;
};

#endif /* NANOSTACK_LOCK_GUARD_H_ */
