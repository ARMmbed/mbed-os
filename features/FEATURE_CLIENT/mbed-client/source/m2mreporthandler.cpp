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
#include "mbed-client/m2mreportobserver.h"
#include "mbed-client/m2mconstants.h"
#include "mbed-client/m2mtimer.h"
#include "include/m2mreporthandler.h"
#include "mbed-trace/mbed_trace.h"
#include <stdio.h>
#include <string.h>

#define TRACE_GROUP "mClt"

M2MReportHandler::M2MReportHandler(M2MReportObserver &observer)
: _observer(observer),
  _pmax(-1.0f),
  _pmin(1.0f),
  _gt(0.0f),
  _lt(0.0f),
  _st(0.0f),
  _pmin_exceeded(false),
  _pmax_exceeded(false),
  _pmin_timer(NULL),
  _pmax_timer(NULL),  
  _high_step(0.0f),
  _low_step(0.0f),
  _current_value(0.0f),
  _last_value(-1.0f),
  _attribute_state(0),
  _notify(false)
{
    tr_debug("M2MReportHandler::M2MReportHandler()");
}

M2MReportHandler::~M2MReportHandler()
{
    tr_debug("M2MReportHandler::~M2MReportHandler()");

    delete _pmax_timer;
    delete _pmin_timer;
}

void M2MReportHandler::set_under_observation(bool observed)
{
    tr_debug("M2MReportHandler::set_under_observation(observed %d)", (int)observed);
    stop_timers();
    if(observed) {
        handle_timers();
    }
    else {
        set_default_values();
    }
}

void M2MReportHandler::set_value(float value)
{
    tr_debug("M2MReportHandler::set_value() - current %f, last %f", value, _last_value);
    _current_value = value;    
    if(_current_value != _last_value) {
        tr_debug("M2MReportHandler::set_value() - UNDER OBSERVATION");
        if (check_threshold_values()) {
            schedule_report();
        }
        else {
            tr_debug("M2MReportHandler::set_value - value not in range");
            _notify = false;
            _last_value = _current_value;
            if ((_attribute_state & M2MReportHandler::Lt) == M2MReportHandler::Lt ||
                    (_attribute_state & M2MReportHandler::Gt) == M2MReportHandler::Gt ||
                    (_attribute_state & M2MReportHandler::St) == M2MReportHandler::St) {
                tr_debug("M2MReportHandler::set_value - stop pmin timer");
                if (_pmin_timer) {
                    _pmin_timer->stop_timer();
                    _pmin_exceeded = true;
                }
            }
        }
        _high_step = _current_value + _st;
        _low_step = _current_value - _st;
    }
}

void M2MReportHandler::set_notification_trigger(uint16_t obj_instance_id)
{
    tr_debug("M2MReportHandler::set_notification_trigger(): %d", obj_instance_id);
    // Add to array if not there yet
    m2m::Vector<uint16_t>::const_iterator it;
    it = _changed_instance_ids.begin();
    bool found = false;
    for ( ; it != _changed_instance_ids.end(); it++) {
        if ((*it) == obj_instance_id) {
            found = true;
            break;
        }
    }
    if (!found) {
        _changed_instance_ids.push_back(obj_instance_id);
    }

    _current_value = 0.0f;
    _last_value = 1.0f;
    schedule_report();
}

bool M2MReportHandler::parse_notification_attribute(char *&query,
                                                    M2MBase::BaseType type,
                                                    M2MResourceInstance::ResourceType resource_type)
{
    tr_debug("M2MReportHandler::parse_notification_attribute(Query %s, Base type %d)", query, (int)type);
    bool success = false;
    char* sep_pos = strchr(query, '&');
    char* rest = query;
    if( sep_pos != NULL ){
        char query_options[5][20];
        float pmin = _pmin;
        float pmax = _pmax;
        float lt = _lt;
        float gt = _gt;
        float st = _st;
        float high = _high_step;
        float low = _low_step;
        uint8_t attr = _attribute_state;

        memset(query_options, 0, sizeof(query_options[0][0]) * 5 * 20);
        uint8_t num_options = 0;
        while( sep_pos != NULL && num_options < 5){
            size_t len = (size_t)(sep_pos-rest);
            if( len > 19 ){
                len = 19;
            }
            memcpy(query_options[num_options], rest, len);
            sep_pos++;            
            rest = sep_pos;
            sep_pos = strchr(rest, '&');
            num_options++;
        }
        if( num_options < 5 && strlen(rest) > 0){
            size_t len = (size_t)strlen(rest);
            if( len > 19 ){
                len = 19;
            }
            memcpy(query_options[num_options++], rest, len);
        }

        for (int option = 0; option < num_options; option++) {
            success = set_notification_attribute(query_options[option],type, resource_type);
            if (!success) {
                tr_debug("M2MReportHandler::parse_notification_attribute - break");
                break;
            }
        }

        if(success) {
             success = check_attribute_validity();
        }
        else {
            tr_debug("M2MReportHandler::parse_notification_attribute - not valid query");
            _pmin = pmin;
            _pmax = pmax;
            _st = st;
            _lt = lt;
            _gt = gt;
            _high_step = high;
            _low_step = low;
            _attribute_state = attr;
        }
    }
    else {
        if(set_notification_attribute(query, type, resource_type)) {
            success = check_attribute_validity();
        }
    }

    return success;
}

void M2MReportHandler::timer_expired(M2MTimerObserver::Type type)
{    
    switch(type) {
        case M2MTimerObserver::PMinTimer: {
            tr_debug("M2MReportHandler::timer_expired - PMIN");
            if (_notify ||
                    (_pmin > 0 &&
                     (_attribute_state & M2MReportHandler::Pmax) != M2MReportHandler::Pmax)){
                report();
            }
            else{                
                _pmin_exceeded = true;
            }
        }
        break;
        case M2MTimerObserver::PMaxTimer: {
            tr_debug("M2MReportHandler::timer_expired - PMAX");
            _pmax_exceeded = true;
            if (_pmin_exceeded ||
                    (_attribute_state & M2MReportHandler::Pmin) != M2MReportHandler::Pmin ) {
                report();
            }
        }
        break;
        default:
            break;
    }
}

bool M2MReportHandler::set_notification_attribute(char* option,
                                                  M2MBase::BaseType type,
                                                  M2MResourceInstance::ResourceType resource_type)
{
    tr_debug("M2MReportHandler::set_notification_attribute()");
    bool success = false;
    char attribute[20];
    char value[20];
    memset(&attribute, 0, 20);
    memset(&value, 0, 20);

    char* pos = strstr(option, EQUAL);
    if( pos != NULL ){        
        memcpy(attribute, option, (size_t)(pos-option));
        pos++;
        memcpy(value, pos, strlen(pos));
    }else{
        memcpy(attribute, option, (size_t)strlen(option) + 1);
    }

    if (strlen(value)) {
        if (strcmp(attribute, PMIN) == 0) {
           _pmin = atoi(value);
            success = true;
            _attribute_state |= M2MReportHandler::Pmin;
            tr_debug("M2MReportHandler::set_notification_attribute %s to %d", attribute, _pmin);
        }
        else if(strcmp(attribute, PMAX) == 0) {
            _pmax = atoi(value);
            success = true;
            _attribute_state |= M2MReportHandler::Pmax;
            tr_debug("M2MReportHandler::set_notification_attribute %s to %d", attribute, _pmax);
        }
        else if(strcmp(attribute, GT) == 0 &&
                (M2MBase::Resource == type)){
            _gt = atof(value);
            success = true;
            _attribute_state |= M2MReportHandler::Gt;
            tr_debug("M2MReportHandler::set_notification_attribute %s to %f", attribute, _gt);
        }
        else if(strcmp(attribute, LT) == 0 &&
                (M2MBase::Resource == type)){
            _lt = atof(value);
            success = true;
            _attribute_state |= M2MReportHandler::Lt;
            tr_debug("M2MReportHandler::set_notification_attribute %s to %f", attribute, _lt);
        }
        else if((strcmp(attribute, ST_SIZE) == 0 || (strcmp(attribute, STP) == 0))
                && (M2MBase::Resource == type)){
            _st = atof(value);
            success = true;
            _high_step = _current_value + _st;
            _low_step = _current_value - _st;
            _attribute_state |= M2MReportHandler::St;
            tr_debug("M2MReportHandler::set_notification_attribute %s to %f", attribute, _st);
        }
        // Return false if try to set gt,lt or st when the resource type is something else than numerical
        if ((resource_type != M2MResourceInstance::INTEGER &&
                resource_type != M2MResourceInstance::FLOAT) &&
                ((_attribute_state & M2MReportHandler::Gt) == M2MReportHandler::Gt ||
                (_attribute_state & M2MReportHandler::Lt) == M2MReportHandler::Lt ||
                (_attribute_state & M2MReportHandler::St) == M2MReportHandler::St)) {
            tr_debug("M2MReportHandler::set_notification_attribute - not numerical resource");
            success = false;
        }
    }
    return success;
}

void M2MReportHandler::schedule_report()
{
    tr_debug("M2MReportHandler::schedule_report()");
    _notify = true;
    if ((_attribute_state & M2MReportHandler::Pmin) != M2MReportHandler::Pmin ||
         _pmin_exceeded) {
        report();
    }
}

void M2MReportHandler::report()
{
    tr_debug("M2MReportHandler::report()");
    if(_current_value != _last_value && _notify) {
        tr_debug("M2MReportHandler::report()- send with PMIN");
        _pmin_exceeded = false;
        _pmax_exceeded = false;
        _notify = false;
        _observer.observation_to_be_sent(_changed_instance_ids);
        _changed_instance_ids.clear();
        if (_pmax_timer) {
            _pmax_timer->stop_timer();
        }
    }
    else {
        if (_pmax_exceeded) {
            tr_debug("M2MReportHandler::report()- send with PMAX");
            _observer.observation_to_be_sent(_changed_instance_ids, true);
            _changed_instance_ids.clear();
        }
        else {
            tr_debug("M2MReportHandler::report()- no need to send");
        }
    }
    handle_timers();
    _last_value = _current_value;
}

void M2MReportHandler::handle_timers()
{
    tr_debug("M2MReportHandler::handle_timers()");
    uint64_t time_interval = 0;
    if ((_attribute_state & M2MReportHandler::Pmin) == M2MReportHandler::Pmin) {
        if (_pmin == _pmax) {
            _pmin_exceeded = true;
        } else {
            _pmin_exceeded = false;
            time_interval = (uint64_t)(_pmin * 1000);
            tr_debug("M2MReportHandler::handle_timers() - Start PMIN interval: %d", (int)time_interval);
            if (!_pmin_timer) {
                _pmin_timer = new M2MTimer(*this);
            }
            _pmin_timer->start_timer(time_interval,
                                     M2MTimerObserver::PMinTimer,
                                     true);
        }
    }
    if ((_attribute_state & M2MReportHandler::Pmax) == M2MReportHandler::Pmax) {
        if (_pmax > 0) {
            if (!_pmax_timer) {
                _pmax_timer = new M2MTimer(*this);
            }
            time_interval = (uint64_t)(_pmax * 1000);
            tr_debug("M2MReportHandler::handle_timers() - Start PMAX interval: %d", (int)time_interval);
            _pmax_timer->start_timer(time_interval,
                                     M2MTimerObserver::PMaxTimer,
                                     true);
        }
    }
}

bool M2MReportHandler::check_attribute_validity()
{
    bool success = true;
    if ((_attribute_state & M2MReportHandler::Pmax) == M2MReportHandler::Pmax &&
            ((_pmax >= -1.0f) && (_pmin > _pmax))) {
        success = false;
    }
    float low = _lt + 2 * _st;
    if ((_attribute_state & M2MReportHandler::Gt) == M2MReportHandler::Gt &&
            (low >= _gt)) {
        success = false;
    }
    return success;
}

void M2MReportHandler::stop_timers()
{
    tr_debug("M2MReportHandler::stop_timers()");
    if (_pmin_timer) {
        _pmin_exceeded = false;
        _pmin_timer->stop_timer();

        delete _pmin_timer;
        _pmin_timer = NULL;
    }
    if (_pmax_timer) {
        _pmax_exceeded = false;
        _pmax_timer->stop_timer();
        delete _pmax_timer;
        _pmax_timer = NULL;
    }
    tr_debug("M2MReportHandler::stop_timers() - out");
}

void M2MReportHandler::set_default_values()
{
    tr_debug("M2MReportHandler::set_default_values");
    _pmax = -1.0f;
    _pmin = 1.0f;
    _gt = 0.0f;
    _lt = 0.0f;
    _st = 0.0f;
    _high_step = 0.0f;
    _low_step = 0.0f;
    _pmin_exceeded = false;
    _pmax_exceeded = false;
    _last_value = -1.0f;
    _attribute_state = 0;
    _changed_instance_ids.clear();
}

bool M2MReportHandler::check_threshold_values()
{
    tr_debug("M2MReportHandler::check_threshold_values");
    tr_debug("Current value: %f", _current_value);
    tr_debug("High step: %f", _high_step);
    tr_debug("Low step: %f", _low_step);
    tr_debug("Less than: %f", _lt);
    tr_debug("Greater than: %f", _gt);
    tr_debug("Step: %f", _st);
    bool can_send = false;
    // Check step condition
    if ((_attribute_state & M2MReportHandler::St) == M2MReportHandler::St) {
        if ((_current_value >= _high_step ||
            _current_value <= _low_step)) {                  
            can_send = true;
        }
        else {            
            if ((_attribute_state & M2MReportHandler::Lt) == M2MReportHandler::Lt ||
                    (_attribute_state & M2MReportHandler::Gt) == M2MReportHandler::Gt ) {
                can_send = check_gt_lt_params();
            }
            else {
                can_send = false;
            }
        }
    }
    else {        
        can_send = check_gt_lt_params();
    }
    tr_debug("M2MReportHandler::check_threshold_values - value in range = %d", (int)can_send);
    return can_send;
}

bool M2MReportHandler::check_gt_lt_params()
{
    tr_debug("M2MReportHandler::check_gt_lt_params");
    bool can_send = false;
    // GT & LT set.
    if ((_attribute_state & (M2MReportHandler::Lt | M2MReportHandler::Gt))
             == (M2MReportHandler::Lt | M2MReportHandler::Gt)) {
        if (_current_value > _gt || _current_value < _lt) {            
            can_send = true;
        }
        else {            
            can_send = false;
        }
    }
    // Only LT
    else if ((_attribute_state & M2MReportHandler::Lt) == M2MReportHandler::Lt &&
           (_attribute_state & M2MReportHandler::Gt) == 0 ) {
        if (_current_value < _lt) {            
            can_send = true;
        }
        else {            
            can_send = false;
        }
    }
    // Only GT
    else if ((_attribute_state & M2MReportHandler::Gt) == M2MReportHandler::Gt &&
           (_attribute_state & M2MReportHandler::Lt) == 0 ) {
        if (_current_value > _gt) {            
            can_send = true;
        }
        else {            
            can_send = false;
        }
    }
    // GT & LT not set.
    else {        
        can_send = true;
    }
    tr_debug("M2MReportHandler::check_gt_lt_params - value in range = %d", (int)can_send);
    return can_send;
}

uint8_t M2MReportHandler::attribute_flags()
{
    return _attribute_state;
}
