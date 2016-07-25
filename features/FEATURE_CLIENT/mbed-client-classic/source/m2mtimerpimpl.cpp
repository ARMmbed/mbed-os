/*
 * Copyright (c) 2015-2016 ARM Limited. All rights reserved.
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

#include <assert.h>
#include <time.h>

#include "mbed-client-classic/m2mtimerpimpl.h"
#include "mbed-client/m2mtimerobserver.h"
#include "mbed-client/m2mvector.h"

#include "eventOS_event.h"
#include "eventOS_event_timer.h"
#include "eventOS_scheduler.h"
#include "ns_hal_init.h"
#include "mbed-trace/mbed_trace.h"

#define TRACE_GROUP "mClt"

#define MBED_CLIENT_TIMER_EVENT 10

#ifdef MBED_CONF_MBED_CLIENT_EVENT_LOOP_SIZE
#define MBED_CLIENT_EVENT_LOOP_SIZE MBED_CONF_MBED_CLIENT_EVENT_LOOP_SIZE
#else
#define MBED_CLIENT_EVENT_LOOP_SIZE 1024
#endif

int8_t M2MTimerPimpl::_tasklet_id = -1;

int8_t M2MTimerPimpl::_next_timer_id = 1;

static m2m::Vector<M2MTimerPimpl*> timer_impl_list;

extern "C" void tasklet_func(arm_event_s *event)
{
    // skip the init event as there will be a timer event after
    if (event->event_type == MBED_CLIENT_TIMER_EVENT) {

        bool timer_found = false;
        eventOS_scheduler_mutex_wait();
        int timer_count = timer_impl_list.size();
        for (int index = 0; index < timer_count; index++) {
            M2MTimerPimpl* timer = timer_impl_list[index];
            if (timer->get_timer_id() == event->event_id) {
                eventOS_scheduler_mutex_release();
                timer_found = true;
                timer->timer_expired();
                break;
            }
        }
        if(!timer_found) {
            eventOS_scheduler_mutex_release();
        }
    }
}

M2MTimerPimpl::M2MTimerPimpl(M2MTimerObserver& observer)
: _observer(observer),
  _single_shot(true),
  _interval(0),
  _type(M2MTimerObserver::Notdefined),
  _intermediate_interval(0),
  _total_interval(0),
  _status(0),
  _dtls_type(false)
{
    ns_hal_init(NULL, MBED_CLIENT_EVENT_LOOP_SIZE, NULL, NULL);
    eventOS_scheduler_mutex_wait();
    if (_tasklet_id < 0) {
        _tasklet_id = eventOS_event_handler_create(tasklet_func, MBED_CLIENT_TIMER_EVENT);
        assert(_tasklet_id >= 0);
    }

    // XXX: this wraps over quite soon
    _timer_id = M2MTimerPimpl::_next_timer_id++;

    timer_impl_list.push_back(this);
    eventOS_scheduler_mutex_release();
}

M2MTimerPimpl::~M2MTimerPimpl()
{
    // cancel the timer request, if any is pending
    cancel();

    // there is no turning back, event os does not have eventOS_event_handler_delete() or similar,
    // so the tasklet is lost forever. Same goes with timer_impl_list, which leaks now memory.

    // remove the timer from object list
    eventOS_scheduler_mutex_wait();
    int timer_count = timer_impl_list.size();
    for (int index = 0; index < timer_count; index++) {

        const M2MTimerPimpl* timer = timer_impl_list[index];
        if (timer->get_timer_id() == _timer_id) {

            timer_impl_list.erase(index);
            break;
        }
    }
    eventOS_scheduler_mutex_release();
}

void M2MTimerPimpl::start_timer( uint64_t interval,
                                 M2MTimerObserver::Type type,
                                 bool single_shot)
{
    assert(interval <= INT32_MAX);

    _dtls_type = false;
    _intermediate_interval = 0;
    _total_interval = 0;
    _status = 0;
    _single_shot = single_shot;
    _interval = interval;
    _type = type;
    start();
}

void M2MTimerPimpl::start_dtls_timer(uint64_t intermediate_interval, uint64_t total_interval, M2MTimerObserver::Type type)
{
    assert(intermediate_interval <= INT32_MAX);
    assert(intermediate_interval <= total_interval);

    _dtls_type = true;
    _intermediate_interval = intermediate_interval;
    _total_interval = total_interval;
    _interval = _intermediate_interval;
    _status = 0;
    _single_shot = false;
    _type = type;
    start();
}

void M2MTimerPimpl::start()
{
    int status;

    status = eventOS_event_timer_request(_timer_id, MBED_CLIENT_TIMER_EVENT,
                                            M2MTimerPimpl::_tasklet_id,
                                            _interval);
    assert(status == 0);
}

void M2MTimerPimpl::cancel()
{
    eventOS_event_timer_cancel(_timer_id, M2MTimerPimpl::_tasklet_id);
}

void M2MTimerPimpl::stop_timer()
{
    _interval = 0;
    _single_shot = true;
    cancel();
}

void M2MTimerPimpl::timer_expired()
{
    _status++;
    _observer.timer_expired(_type);

    if ((!_dtls_type) && (!_single_shot)) {
        // start next round of periodic timer
        start();
    } else if ((_dtls_type) && (!is_total_interval_passed())) {
        // if only the intermediate time has passed, we need still wait up to total time
        _interval = _total_interval - _intermediate_interval;
        start();
    }
}

bool M2MTimerPimpl::is_intermediate_interval_passed()
{
    if (_status > 0) {
        return true;
    }
    return false;
}

bool M2MTimerPimpl::is_total_interval_passed()
{
    if (_status > 1) {
        return true;
    }
    return false;
}
