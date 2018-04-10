/* mbed Microcontroller Library
 * Copyright (c) 2017 ARM Limited
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

#define __STDC_LIMIT_MACROS
#include <stdint.h>
#include <algorithm>

#include "utest/utest.h"
#include "unity/unity.h"
#include "greentea-client/test_env.h"

#include "mbed.h"
#include "ticker_api.h"

using namespace utest::v1;

#define MBED_ARRAY_SIZE(array) (sizeof(array)/sizeof(array[0]))

#define TIMESTAMP_MAX_DELTA_BITS(bits)  ((uint64_t)(0x7 << ((bits) - 4)))
#define TIMESTAMP_MAX_DELTA TIMESTAMP_MAX_DELTA_BITS(32)

struct ticker_interface_stub_t { 
    ticker_interface_t interface;
    bool initialized; 
    bool interrupt_flag;
    timestamp_t timestamp ;
    timestamp_t interrupt_timestamp; 
    unsigned int init_call;
    unsigned int read_call;
    unsigned int disable_interrupt_call;
    unsigned int clear_interrupt_call;
    unsigned int set_interrupt_call;
    unsigned int fire_interrupt_call;
    unsigned int get_info_call;
};

static ticker_interface_stub_t interface_stub = { 0 };
static ticker_info_t interface_info_stub = { 0 };

static void ticker_interface_stub_init()
{
    ++interface_stub.init_call;
    interface_stub.initialized = true;
}

static uint32_t ticker_interface_stub_read()
{
    ++interface_stub.read_call;
    return interface_stub.timestamp;
} 

static void ticker_interface_stub_disable_interrupt()
{ 
    ++interface_stub.disable_interrupt_call;
}

static void ticker_interface_stub_clear_interrupt()
{
    ++interface_stub.clear_interrupt_call;
    interface_stub.interrupt_flag = false;
}

static void ticker_interface_stub_set_interrupt(timestamp_t timestamp)
{
    ++interface_stub.set_interrupt_call;
    interface_stub.interrupt_timestamp = timestamp; 
}

static void ticker_interface_stub_fire_interrupt()
{
    ++interface_stub.fire_interrupt_call;
}

static const ticker_info_t *ticker_interface_stub_get_info()
{
    ++interface_stub.get_info_call;
    return &interface_info_stub;
}

static void reset_ticker_interface_stub()
{
    interface_stub.interface.init = ticker_interface_stub_init;
    interface_stub.interface.read = ticker_interface_stub_read;
    interface_stub.interface.disable_interrupt = 
        ticker_interface_stub_disable_interrupt;
    interface_stub.interface.clear_interrupt = 
        ticker_interface_stub_clear_interrupt;
    interface_stub.interface.set_interrupt =ticker_interface_stub_set_interrupt;
    interface_stub.interface.fire_interrupt = ticker_interface_stub_fire_interrupt;
    interface_stub.interface.get_info = ticker_interface_stub_get_info;
    interface_stub.initialized = false;
    interface_stub.interrupt_flag = false;
    interface_stub.timestamp = 0;
    interface_stub.interrupt_timestamp = 0;
    interface_stub.init_call = 0;
    interface_stub.read_call = 0;
    interface_stub.disable_interrupt_call = 0;
    interface_stub.clear_interrupt_call = 0;
    interface_stub.set_interrupt_call = 0;
    interface_stub.fire_interrupt_call = 0;

    interface_info_stub.frequency = 1000000;
    interface_info_stub.bits = 32;
}

// stub of the event queue 
static ticker_event_queue_t queue_stub = {
    /* event handler */ NULL,
    /* head */ NULL,
    /* timestamp */ 0,
    /* initialized */ false
};

static void reset_queue_stub()
{
    queue_stub.event_handler = NULL;
    queue_stub.head = NULL,
    queue_stub.tick_last_read = 0;
    queue_stub.tick_remainder = 0;
    queue_stub.frequency = 0;
    queue_stub.bitmask = 0;
    queue_stub.max_delta = 0;
    queue_stub.max_delta_us = 0;
    queue_stub.present_time = 0;
    queue_stub.initialized = false;
}

// stub of the ticker
static ticker_data_t ticker_stub = {
    /* interface */ &interface_stub.interface,
    /* queue */ &queue_stub
};

static void reset_ticker_stub()
{
    reset_queue_stub();
    reset_ticker_interface_stub();
}

const uint32_t test_frequencies[] = {
    1,
    32768,      // 2^15
    1000000,
    0xFFFFFFFF  // 2^32 - 1
};

const uint32_t test_bitwidths[] = {
    32,
    31,
    16,
    8
};

template < void (F)(uint32_t a, uint32_t b)>
static void test_over_frequency_and_width(void)
{
    for (unsigned int i = 0; i < MBED_ARRAY_SIZE(test_frequencies); i++) {
        for (unsigned int j = 0; j < MBED_ARRAY_SIZE(test_bitwidths); j++) {
            reset_ticker_stub();
            interface_info_stub.frequency = test_frequencies[i];
            interface_info_stub.bits = test_bitwidths[j];

            F(test_frequencies[i], test_bitwidths[j]);
        }
    }
}

static utest::v1::status_t case_setup_handler(
    const Case *const source, const size_t index_of_case
) { 
    utest::v1::status_t status = greentea_case_setup_handler(source, index_of_case);
    reset_ticker_stub();
    return status;
}

static utest::v1::status_t case_teardown_handler(
    const Case *const source, const size_t passed, const size_t failed, const failure_t reason
) { 
    reset_ticker_stub();
    utest::v1::status_t status = greentea_case_teardown_handler(
        source, passed, failed, reason
    );
    return status;
}

static utest::v1::status_t greentea_failure_handler(
    const Case *const source, const failure_t reason
) {
    utest::v1::status_t status = greentea_case_failure_abort_handler(
        source, reason
    );
    return status;
}

#define MAKE_TEST_CASE(description, handler) \
    { \
        description, \
        handler, \
        NULL, \
        NULL, \
        case_setup_handler, \
        case_teardown_handler, \
        greentea_failure_handler \
    }

/**
 * Given an unitialized ticker_data instance. 
 * When the ticker is initialized 
 * Then: 
 *   - The ticker interface should be initialized 
 *   - The queue handler should be set to the handler provided in parameter
 *   - The internal ticker timestamp should be zero
 *   - interrupt should be scheduled in current timestamp + 
 *     TIMESTAMP_MAX_DELTA
 *   - The queue should not contains any event
 */
static void test_ticker_initialization()
{
    ticker_event_handler dummy_handler = (ticker_event_handler)0xDEADBEEF;

    // setup of the stub 
    interface_stub.timestamp = 0xFEEDBABE;

    ticker_set_handler(&ticker_stub, dummy_handler);

    TEST_ASSERT_TRUE(interface_stub.initialized);
    TEST_ASSERT_EQUAL_PTR(dummy_handler, queue_stub.event_handler);
    TEST_ASSERT_EQUAL_UINT64(0, queue_stub.present_time);
    TEST_ASSERT_EQUAL(1, interface_stub.set_interrupt_call);
    TEST_ASSERT_EQUAL_UINT32(
        interface_stub.timestamp + TIMESTAMP_MAX_DELTA,
        interface_stub.interrupt_timestamp
    );
    TEST_ASSERT_EQUAL_PTR(NULL, queue_stub.head);
    TEST_ASSERT_EQUAL(0, interface_stub.disable_interrupt_call);
}

/**
 * Given an initialized ticker_data instance. 
 * When the ticker handler is set to a new value    
 * Then: 
 *   - The ticker interface initialization function should not be called.
 *   - The queue handler should be set to the new handler.
 *   - The events in the queue should remains the same.
 */
static void test_ticker_re_initialization()
{
    ticker_event_handler dummy_handler = (ticker_event_handler) 0xDEADBEEF;
    ticker_event_handler expected_handler = (ticker_event_handler) 0xFEEDDEAF;
    
    ticker_event_t first_event = { 0 }; 
    ticker_event_t second_event  = { 0 }; 
    ticker_event_t third_event  = { 0 }; 

    first_event.next = &second_event;
    second_event.next = &third_event;

    // initialize the ticker and put few events in the queue.
    ticker_set_handler(&ticker_stub, dummy_handler);
    // simulate insertion, it shouldn't affect the queue behaviour for this test
    queue_stub.head = &first_event;
    interface_stub.init_call = 0;

    ticker_set_handler(&ticker_stub, expected_handler);

    TEST_ASSERT_TRUE(interface_stub.initialized);
    TEST_ASSERT_EQUAL(0, interface_stub.init_call);
    TEST_ASSERT_EQUAL(expected_handler, queue_stub.event_handler);
    TEST_ASSERT_EQUAL(&first_event, queue_stub.head);
    TEST_ASSERT_EQUAL(&second_event, queue_stub.head->next);
    TEST_ASSERT_EQUAL(&third_event, queue_stub.head->next->next);
    TEST_ASSERT_EQUAL(NULL, queue_stub.head->next->next->next);
    TEST_ASSERT_EQUAL(0, interface_stub.disable_interrupt_call);
}

/**
 * Given an initialized ticker_data instance. 
 * When the ticker is read 
 * Then it should return the value present in the ticker interface
 */
static void test_ticker_read()
{
    ticker_set_handler(&ticker_stub, NULL);

    timestamp_t timestamps[] = { 
        0xA,
        0xAA,
        0xAAA,
        0xAAAA,
        0xAAAAA,
        0xAAAAAA,
        0xAAAAAAA,
        0xAAAAAAAA
    };

    for (size_t i = 0; i < MBED_ARRAY_SIZE(timestamps); ++i) {
        interface_stub.timestamp = timestamps[i];
        TEST_ASSERT_EQUAL_UINT32(timestamps[i], ticker_read(&ticker_stub));
        TEST_ASSERT_EQUAL_UINT64(timestamps[i], ticker_read_us(&ticker_stub));
    }

    TEST_ASSERT_EQUAL(0, interface_stub.disable_interrupt_call);
}

/**
 * Given an initialized ticker_data instance. 
 * When the ticker is read and the value read is less than the previous 
 * value read.
 * Then:
 *   - ticker_read should return the value read in the ticker interface 
 *   - ticker_read_us should return a value where: 
 *     + lower 8 bytes should be equal to the value in the ticker interface
 *     + upper 8 bytes should be equal to the previous value of upper 8 bytes
 *       plus one.
 */
static void test_ticker_read_overflow()
{
    const timestamp_t timestamps[] = { 
        0xAAAAAAAA,
        0xAAAAAAA,
        0xAAAAAA,
        0xAAAAA,
        0xAAAA,
        0xAAA,
        0xAA,
        0xA
    };

    ticker_set_handler(&ticker_stub, NULL);

    uint32_t upper_bytes_begin = ticker_read_us(&ticker_stub) >> 32;

    for (size_t i = 0; i < MBED_ARRAY_SIZE(timestamps); ++i) {
        interface_stub.timestamp = timestamps[i];
        TEST_ASSERT_EQUAL_UINT32(timestamps[i], ticker_read(&ticker_stub));
        TEST_ASSERT_EQUAL_UINT32(timestamps[i], ticker_read_us(&ticker_stub));
        TEST_ASSERT_EQUAL_UINT64(
            upper_bytes_begin + i, ticker_read_us(&ticker_stub) >> 32
        );
    }

    TEST_ASSERT_EQUAL(0, interface_stub.disable_interrupt_call);
}

/**
 * Given an initialized ticker without user registered events.  
 * When an event is inserted with ticker_insert_event and the timestamp passed 
 * in parameter is in range [ticker_timestamp : ticker_timestamp + 
 * TIMESTAMP_MAX_DELTA[.
 * Then 
 *   - The event should be in the queue
 *   - The interrupt timestamp should be equal to the timestamp of the event 
 *   - The timestamp of the event should reflect the timestamp requested. 
 *   - The id of the event should be equal to the id passed in parameter. 
 */
static void test_legacy_insert_event_outside_overflow_range()
{
    ticker_set_handler(&ticker_stub, NULL);
    interface_stub.set_interrupt_call = 0;

    // test the end of the range
    ticker_event_t last_event = { 0 };
    const timestamp_t timestamp_last_event = 
        interface_stub.timestamp + TIMESTAMP_MAX_DELTA; 
    const uint32_t id_last_event = 0xDEADDEAF;

    ticker_insert_event(
        &ticker_stub, 
        &last_event, timestamp_last_event, id_last_event
    );

    TEST_ASSERT_EQUAL_PTR(&last_event, queue_stub.head);
    TEST_ASSERT_EQUAL(1, interface_stub.set_interrupt_call);    
    TEST_ASSERT_EQUAL_UINT32(
        timestamp_last_event, interface_stub.interrupt_timestamp
    );
    TEST_ASSERT_EQUAL_PTR(NULL, queue_stub.head->next);
    TEST_ASSERT_EQUAL_UINT32(timestamp_last_event, last_event.timestamp);
    TEST_ASSERT_EQUAL_UINT32(id_last_event, last_event.id);

    // test the beginning of the range
    ticker_event_t first_event = { 0 };
    const timestamp_t timestamp_first_event = interface_stub.timestamp + 1;
    const uint32_t id_first_event = 0xAAAAAAAA;

    ticker_insert_event(
        &ticker_stub, 
        &first_event, timestamp_first_event, id_first_event
    );

    TEST_ASSERT_EQUAL_PTR(&first_event, queue_stub.head);
    TEST_ASSERT_EQUAL(2, interface_stub.set_interrupt_call);    
    TEST_ASSERT_EQUAL_UINT32(
        timestamp_first_event, interface_stub.interrupt_timestamp
    );
    TEST_ASSERT_EQUAL_PTR(&last_event, queue_stub.head->next);
    TEST_ASSERT_EQUAL_UINT32(
        timestamp_first_event, first_event.timestamp
    );
    TEST_ASSERT_EQUAL_UINT32(id_first_event, first_event.id);

    TEST_ASSERT_EQUAL(0, interface_stub.disable_interrupt_call);
}  

/**
 * Given an initialized ticker without user registered events.  
 * When an event is inserted with ticker_insert_event and a timestamp in the 
 * range [ticker_timestamp + TIMESTAMP_MAX_DELTA + 1 : 
 * ticker_timestamp + UINT32MAX [
 * Then 
 *   - The event should be in the queue
 *   - The interrupt timestamp should be equal to 
 *     TIMESTAMP_MAX_DELTA 
 *   - The timestamp of the event should reflect the timestamp requested. 
 *   - The id of the event should be equal to the id passed in parameter. 
 */
static void test_legacy_insert_event_in_overflow_range()
{
    ticker_set_handler(&ticker_stub, NULL);
    interface_stub.set_interrupt_call = 0;

    // test the end of the range
    ticker_event_t last_event = { 0 };
    const timestamp_t timestamp_last_event = 
        interface_stub.timestamp + UINT32_MAX; 
    const uint32_t id_last_event = 0xDEADDEAF;

    ticker_insert_event(
        &ticker_stub, 
        &last_event, timestamp_last_event, id_last_event
    );

    TEST_ASSERT_EQUAL_PTR(&last_event, queue_stub.head);
    TEST_ASSERT_EQUAL(1, interface_stub.set_interrupt_call);    
    TEST_ASSERT_EQUAL_UINT32(
        interface_stub.timestamp + TIMESTAMP_MAX_DELTA, 
        interface_stub.interrupt_timestamp
    );
    TEST_ASSERT_EQUAL_PTR(NULL, queue_stub.head->next);
    TEST_ASSERT_EQUAL_UINT32(timestamp_last_event, last_event.timestamp);
    TEST_ASSERT_EQUAL_UINT32(id_last_event, last_event.id);

    // test the beginning of the range
    ++interface_stub.timestamp;

    ticker_event_t first_event = { 0 };
    const timestamp_t timestamp_first_event = 
        interface_stub.timestamp + TIMESTAMP_MAX_DELTA + 1; 
    const uint32_t id_first_event = 0xAAAAAAAA;

    ticker_insert_event(
        &ticker_stub, 
        &first_event, timestamp_first_event, id_first_event
    );

    TEST_ASSERT_EQUAL_PTR(&first_event, queue_stub.head);
    TEST_ASSERT_EQUAL(2, interface_stub.set_interrupt_call);    
    TEST_ASSERT_EQUAL_UINT32(
        interface_stub.timestamp + TIMESTAMP_MAX_DELTA, 
        interface_stub.interrupt_timestamp
    );
    TEST_ASSERT_EQUAL_PTR(&last_event, queue_stub.head->next);
    TEST_ASSERT_EQUAL_UINT32(
        timestamp_first_event, first_event.timestamp
    );
    TEST_ASSERT_EQUAL_UINT32(id_first_event, first_event.id);

    TEST_ASSERT_EQUAL(0, interface_stub.disable_interrupt_call);
}  

/**
 * Given an initialized ticker without user registered events.  
 * When an event is inserted with ticker_insert_event and the timestamp in 
 * parameter is less than the current timestamp value.
 * Then 
 *   - The event should be in the queue
 *   - The timestamp of the event should reflect the timestamp requested:
 *     + lower 8 bytes should be equal to the timestamp in input. 
 *     + upper 8 bytes should be equal to the upper of the upper 8 bytes of the
 *       timestamp state stored in the queue plus one.
 *   - The id of the event should be equal to the id passed in parameter. 
 */
static void test_legacy_insert_event_overflow(){
    ticker_set_handler(&ticker_stub, NULL);
    interface_stub.set_interrupt_call = 0;

    interface_stub.timestamp = 0x20000000;
    ticker_read(&ticker_stub);

    ticker_event_t event = { 0 };
    const timestamp_t expected_timestamp = 
        interface_stub.timestamp + 
        TIMESTAMP_MAX_DELTA + 
        1; 
    const us_timestamp_t expected_us_timestamp = 
        (((queue_stub.present_time >> 32) + 1) << 32) | expected_timestamp;
    const uint32_t expected_id = 0xDEADDEAF;

    ticker_insert_event(
        &ticker_stub, 
        &event, expected_timestamp, expected_id
    );

    TEST_ASSERT_EQUAL_PTR(&event, queue_stub.head);
    TEST_ASSERT_EQUAL(1, interface_stub.set_interrupt_call);    
    TEST_ASSERT_EQUAL_UINT32(
        interface_stub.timestamp + TIMESTAMP_MAX_DELTA, 
        interface_stub.interrupt_timestamp
    );
    TEST_ASSERT_EQUAL_PTR(NULL, queue_stub.head->next);
    TEST_ASSERT_EQUAL_UINT32(expected_us_timestamp, event.timestamp);
    TEST_ASSERT_EQUAL_UINT32(expected_id, event.id);

    TEST_ASSERT_EQUAL(0, interface_stub.disable_interrupt_call);
}  

/**
 * Given an initialized ticker.
 * When an event is inserted with ticker_insert_event and a timestamp less than 
 * the one for the next scheduled timestamp.
 * Then 
 *   - The event inserted should be the first in the queue
 *   - The interrupt timestamp should be equal to the timestamp of the event or 
 *     TIMESTAMP_MAX_DELTA if in the overflow range.
 *   - The timestamp of the event should reflect the timestamp requested.
 *   - The id of the event should be equal to the id passed in parameter. 
 *   - Events in the queue should remained ordered by timestamp.
 */
static void test_legacy_insert_event_head()
{
    ticker_set_handler(&ticker_stub, NULL);
    interface_stub.set_interrupt_call = 0;

    const timestamp_t timestamps[] = { 
        UINT32_MAX,
        TIMESTAMP_MAX_DELTA + 1,
        TIMESTAMP_MAX_DELTA,
        TIMESTAMP_MAX_DELTA / 2,
        TIMESTAMP_MAX_DELTA / 4,
        TIMESTAMP_MAX_DELTA / 8,
        TIMESTAMP_MAX_DELTA / 16,
    };
    ticker_event_t events[MBED_ARRAY_SIZE(timestamps)] = { 0 };

    for (size_t i = 0; i < MBED_ARRAY_SIZE(events); ++i) { 
        ticker_insert_event(
            &ticker_stub, 
            &events[i], timestamps[i], i
        );

        TEST_ASSERT_EQUAL_PTR(&events[i], queue_stub.head);
        TEST_ASSERT_EQUAL(i + 1, interface_stub.set_interrupt_call);  
        if (timestamps[i] < TIMESTAMP_MAX_DELTA) { 
            TEST_ASSERT_EQUAL_UINT32(
                timestamps[i], 
                interface_stub.interrupt_timestamp
            );
        } else { 
            TEST_ASSERT_EQUAL_UINT32(
                TIMESTAMP_MAX_DELTA, 
                interface_stub.interrupt_timestamp
            );
        }

        TEST_ASSERT_EQUAL_UINT32(
            timestamps[i], events[i].timestamp
        );
        TEST_ASSERT_EQUAL_UINT32(i, events[i].id);

        ticker_event_t* e = &events[i];
        while (e) { 
            TEST_ASSERT_EQUAL_UINT32(timestamps[e->id], e->timestamp);
            if (e->next) { 
                TEST_ASSERT_TRUE(e->id > e->next->id);
                TEST_ASSERT_TRUE(e->timestamp < e->next->timestamp);
            } else { 
                TEST_ASSERT_EQUAL_UINT32(0, e->id);
            }
            e = e->next;
        }
    }

    TEST_ASSERT_EQUAL(0, interface_stub.disable_interrupt_call);
}  

/** 
 * Given an initialized ticker.
 * When an event is inserted with ticker_insert_event and its timestamp is bigger 
 * than the one of the last event in the queue.
 * Then 
 *   - The event inserted should be the last in the queue
 *   - The interrupt timestamp should remains equal to the interrupt timestamp 
 *     of the head event .
 *   - The timestamp of the event should reflect the timestamp requested.
 *   - The id of the event should be equal to the id passed in parameter. 
 *   - Events in the queue should remained ordered by timestamp.
 */
static void test_legacy_insert_event_tail()
{
    ticker_set_handler(&ticker_stub, NULL);
    interface_stub.set_interrupt_call = 0;

    const timestamp_t timestamps[] = { 
        0xA,
        0xAA,
        0xAAA,
        0xAAAA,
        0xAAAAA,
        0xAAAAAA,
        0xAAAAAAA,
        0xAAAAAAAA,
    };
    ticker_event_t events[MBED_ARRAY_SIZE(timestamps)] = { 0 };

    for (size_t i = 0; i < MBED_ARRAY_SIZE(events); ++i) { 
        ticker_insert_event(
            &ticker_stub, 
            &events[i], timestamps[i], i
        );

        TEST_ASSERT_EQUAL_PTR(&events[0], queue_stub.head);
        TEST_ASSERT_EQUAL_UINT32(
            timestamps[0], interface_stub.interrupt_timestamp
        );

        TEST_ASSERT_EQUAL_UINT32(timestamps[i], events[i].timestamp);
        TEST_ASSERT_EQUAL_UINT32(i, events[i].id);

        ticker_event_t* e = queue_stub.head;
        while (e) { 
            TEST_ASSERT_EQUAL_UINT32(timestamps[e->id], e->timestamp);
            if (e->next) { 
                TEST_ASSERT_TRUE(e->id < e->next->id);
                TEST_ASSERT_TRUE(e->timestamp < e->next->timestamp);
            } else { 
                TEST_ASSERT_EQUAL_UINT32(&events[i], e);
            }
            e = e->next;
        }
    }

    TEST_ASSERT_EQUAL(0, interface_stub.disable_interrupt_call);
} 

/**
 * Given an initialized ticker.
 * When an event is inserted with ticker_insert_event and a timestamp less 
 * than the current timestamp in the interface and less than the relative 
 * timestamp of the next event to execute.
 * Then 
 *   - The event inserted should be after the head
 *   - The interrupt timestamp should remains equal to the interrupt timestamp 
 *     of the head event .
 *   - The timestamp of the event should reflect the timestamp requested (overflow)
 *   - The id of the event should be equal to the id passed in parameter. 
 *   - Events in the queue should remained ordered by timestamp.
 */
static void test_legacy_insert_event_multiple_overflow()
{
    ticker_set_handler(&ticker_stub, NULL);
    interface_stub.set_interrupt_call = 0;

    const timestamp_t timestamps[] = { 
        0xA,
        0xAA,
        0xAAA,
        0xAAAA,
        0xAAAAA,
        0xAAAAAA,
        0xAAAAAAA,
        0xAAAAAAAA
    };
    ticker_event_t events[MBED_ARRAY_SIZE(timestamps)] = { 0 };

    ticker_event_t ref_event;
    timestamp_t ref_event_timestamp = 0xCCCCCCCC;
    ticker_insert_event(
        &ticker_stub, 
        &ref_event, ref_event_timestamp, 0xDEADBEEF
    );

    timestamp_t last_timestamp_to_insert = 
        timestamps[MBED_ARRAY_SIZE(timestamps) - 1];
    interface_stub.timestamp = 
        last_timestamp_to_insert + 
        ((ref_event_timestamp - last_timestamp_to_insert) / 2);
    ticker_irq_handler(&ticker_stub);

    for (size_t i = 0; i < MBED_ARRAY_SIZE(events); ++i) { 
        ticker_insert_event(
            &ticker_stub, 
            &events[i], timestamps[i], i
        );

        TEST_ASSERT_EQUAL_PTR(&ref_event, queue_stub.head);
        TEST_ASSERT_EQUAL_PTR(&events[0], queue_stub.head->next);
        TEST_ASSERT_EQUAL_UINT32(
            ref_event_timestamp, interface_stub.interrupt_timestamp
        );

        TEST_ASSERT_EQUAL_UINT32(timestamps[i], events[i].timestamp);
        TEST_ASSERT_EQUAL_UINT32(i, events[i].id);

        ticker_event_t* e = queue_stub.head->next;
        while (e) { 
            TEST_ASSERT_EQUAL_UINT32(timestamps[e->id], e->timestamp);
            if (e->next) { 
                TEST_ASSERT_TRUE(e->id < e->next->id);
                TEST_ASSERT_TRUE(e->timestamp < e->next->timestamp);
            } else { 
                TEST_ASSERT_EQUAL_UINT32(&events[i], e);
            }
            e = e->next;
        }
    }

    TEST_ASSERT_EQUAL(0, interface_stub.disable_interrupt_call);
}

/**
 * Given an initialized ticker.
 * When an event is inserted with ticker_insert_event.
 * Then 
 *   - The event inserted should be at the correct position in the queue
 *   - The event queue should remain ordered by timestamp
 *   - The interrupt timestamp should be equal to the interrupt timestamp 
 *     of the head event or TIMESTAMP_MAX_DELTA if the 
 *     timestamp is in the overflow range.
 *   - The timestamp of the event should reflect the timestamp requested (overflow)
 *   - The id of the event should be equal to the id passed in parameter. 
 *   - Events in the queue should remained ordered by timestamp.
 */
static void test_legacy_insert_event_multiple_random()
{
    ticker_set_handler(&ticker_stub, NULL);
    interface_stub.set_interrupt_call = 0;

    const timestamp_t ref_timestamp = UINT32_MAX / 2;
    interface_stub.timestamp = ref_timestamp;

    // insert first event at the head of the queue 
    ticker_event_t first_event;
    const timestamp_t first_event_timestamp = 
        ref_timestamp + TIMESTAMP_MAX_DELTA + 100;

    ticker_insert_event(
        &ticker_stub,
        &first_event, first_event_timestamp, (uint32_t) &first_event
    );

    TEST_ASSERT_EQUAL_PTR(&first_event, queue_stub.head);
    TEST_ASSERT_EQUAL_PTR(NULL, first_event.next);
    TEST_ASSERT_EQUAL_UINT32(
        ref_timestamp + TIMESTAMP_MAX_DELTA, interface_stub.interrupt_timestamp
    );
    TEST_ASSERT_EQUAL_UINT32(first_event_timestamp, first_event.timestamp);
    TEST_ASSERT_EQUAL_UINT64(
        first_event.timestamp,
        first_event_timestamp + 
        ((first_event_timestamp < ref_timestamp) ? (1ULL << 32) : 0)
    );
    TEST_ASSERT_EQUAL_UINT32((uint32_t) &first_event, first_event.id);

    // insert second event at the tail of the queue
    ticker_event_t second_event;
    const timestamp_t second_event_timestamp = first_event_timestamp + 1;

    ticker_insert_event(
        &ticker_stub,
        &second_event, second_event_timestamp, (uint32_t) &second_event
    );

    TEST_ASSERT_EQUAL_PTR(&first_event, queue_stub.head);
    TEST_ASSERT_EQUAL_PTR(&second_event, first_event.next);
    TEST_ASSERT_EQUAL_PTR(NULL, second_event.next);
    TEST_ASSERT_EQUAL_UINT32(
        ref_timestamp + TIMESTAMP_MAX_DELTA, interface_stub.interrupt_timestamp
    );
    TEST_ASSERT_EQUAL_UINT32(second_event_timestamp, second_event.timestamp);
    TEST_ASSERT_EQUAL_UINT64(
        second_event.timestamp,
        second_event_timestamp + 
        ((second_event_timestamp < ref_timestamp) ? (1ULL << 32) : 0)
    );
    TEST_ASSERT_EQUAL_UINT32((uint32_t) &second_event, second_event.id);


    // insert third event at the head of the queue out the overflow zone
    ticker_event_t third_event;
    const timestamp_t third_event_timestamp = 
        ref_timestamp + TIMESTAMP_MAX_DELTA - 100;

    ticker_insert_event(
        &ticker_stub,
        &third_event, third_event_timestamp, (uint32_t) &third_event
    );

    TEST_ASSERT_EQUAL_PTR(&third_event, queue_stub.head);
    TEST_ASSERT_EQUAL_PTR(&first_event, third_event.next);
    TEST_ASSERT_EQUAL_PTR(&second_event, first_event.next);
    TEST_ASSERT_EQUAL_PTR(NULL, second_event.next);
    TEST_ASSERT_EQUAL_UINT32(
        third_event_timestamp, interface_stub.interrupt_timestamp
    );
    TEST_ASSERT_EQUAL_UINT32(third_event_timestamp, third_event.timestamp);
    TEST_ASSERT_EQUAL_UINT64(
        third_event.timestamp,
        third_event_timestamp + 
        ((third_event_timestamp < ref_timestamp) ? (1ULL << 32) : 0)
    );
    TEST_ASSERT_EQUAL_UINT32((uint32_t) &third_event, third_event.id);

    // insert fourth event right after the third event
    ticker_event_t fourth_event;
    const timestamp_t fourth_event_timestamp = third_event_timestamp + 50;

    ticker_insert_event(
        &ticker_stub,
        &fourth_event, fourth_event_timestamp, (uint32_t) &fourth_event
    );

    TEST_ASSERT_EQUAL_PTR(&third_event, queue_stub.head);
    TEST_ASSERT_EQUAL_PTR(&fourth_event, third_event.next);
    TEST_ASSERT_EQUAL_PTR(&first_event, fourth_event.next);
    TEST_ASSERT_EQUAL_PTR(&second_event, first_event.next);
    TEST_ASSERT_EQUAL_PTR(NULL, second_event.next);
    TEST_ASSERT_EQUAL_UINT32(
        third_event_timestamp, interface_stub.interrupt_timestamp
    );
    TEST_ASSERT_EQUAL_UINT32(fourth_event_timestamp, fourth_event.timestamp);
    TEST_ASSERT_EQUAL_UINT64(
        fourth_event.timestamp,
        fourth_event_timestamp + 
        ((fourth_event_timestamp < ref_timestamp) ? (1ULL << 32) : 0)
    );
    TEST_ASSERT_EQUAL_UINT32((uint32_t) &fourth_event, fourth_event.id);

    TEST_ASSERT_EQUAL(0, interface_stub.disable_interrupt_call);
}

/**
 * Given an initialized ticker without user registered events.  
 * When an event is inserted with ticker_insert_event_us and the timestamp passed 
 * in parameter is in range [ticker_timestamp : ticker_timestamp + 
 * TIMESTAMP_MAX_DELTA[.
 * Then 
 *   - The event should be in the queue
 *   - The interrupt timestamp should be equal to the lower 8 bytes of the event. 
 *   - The timestamp of the event should be equal to the timestamp requested. 
 *   - The id of the event should be equal to the id passed in parameter. 
 */
static void test_insert_event_us_outside_overflow_range()
{
    ticker_set_handler(&ticker_stub, NULL);
    interface_stub.set_interrupt_call = 0;
    interface_stub.timestamp = 0xAAAAAAAA;
    queue_stub.tick_last_read = interface_stub.timestamp;
    queue_stub.present_time = 10ULL << 32 | interface_stub.timestamp;

    // test the end of the range
    ticker_event_t last_event = { 0 };
    const us_timestamp_t timestamp_last_event = 
        queue_stub.present_time + TIMESTAMP_MAX_DELTA;
    const uint32_t id_last_event = 0xDEADDEAF;

    ticker_insert_event_us(
        &ticker_stub, 
        &last_event, timestamp_last_event, id_last_event
    );

    TEST_ASSERT_EQUAL_PTR(&last_event, queue_stub.head);
    TEST_ASSERT_EQUAL(1, interface_stub.set_interrupt_call);    
    TEST_ASSERT_EQUAL_UINT32(
        timestamp_last_event, interface_stub.interrupt_timestamp
    );
    TEST_ASSERT_EQUAL_PTR(NULL, queue_stub.head->next);
    TEST_ASSERT_EQUAL_UINT64(timestamp_last_event, last_event.timestamp);
    TEST_ASSERT_EQUAL_UINT32(id_last_event, last_event.id);

    // test the beginning of the range
    ticker_event_t first_event = { 0 };
    const us_timestamp_t timestamp_first_event = queue_stub.present_time + 1;
    const uint32_t id_first_event = 0xAAAAAAAA;

    ticker_insert_event_us(
        &ticker_stub, 
        &first_event, timestamp_first_event, id_first_event
    );

    TEST_ASSERT_EQUAL_PTR(&first_event, queue_stub.head);
    TEST_ASSERT_EQUAL(2, interface_stub.set_interrupt_call);    
    TEST_ASSERT_EQUAL_UINT32(
        timestamp_first_event, interface_stub.interrupt_timestamp
    );
    TEST_ASSERT_EQUAL_PTR(&last_event, queue_stub.head->next);
    TEST_ASSERT_EQUAL_UINT64(
        timestamp_first_event, first_event.timestamp
    );
    TEST_ASSERT_EQUAL_UINT32(id_first_event, first_event.id);

    TEST_ASSERT_EQUAL(0, interface_stub.disable_interrupt_call);
}  

/**
 * Given an initialized ticker without user registered events.  
 * When an event is inserted with ticker_insert_event_us and a timestamp in the 
 * range [ticker_timestamp + TIMESTAMP_MAX_DELTA + 1 : UINT64_MAX [ 
 * Then 
 *   - The event should be in the queue
 *   - The interrupt timestamp should be equal to TIMESTAMP_MAX_DELTA 
 *   - The timestamp of the event should be equal to the timestamp in parameter. 
 *   - The id of the event should be equal to the id passed in parameter. 
 */
static void test_insert_event_us_in_overflow_range()
{
    ticker_set_handler(&ticker_stub, NULL);
    interface_stub.set_interrupt_call = 0;
    interface_stub.timestamp = 0xAAAAAAAA;
    queue_stub.tick_last_read = interface_stub.timestamp;
    queue_stub.present_time = 10ULL << 32 | interface_stub.timestamp;

    // test the end of the range
    ticker_event_t last_event = { 0 };
    const us_timestamp_t timestamp_last_event = UINT64_MAX; 
    const uint32_t id_last_event = 0xDEADDEAF;

    ticker_insert_event_us(
        &ticker_stub, 
        &last_event, timestamp_last_event, id_last_event
    );

    TEST_ASSERT_EQUAL_PTR(&last_event, queue_stub.head);
    TEST_ASSERT_EQUAL(1, interface_stub.set_interrupt_call);    
    TEST_ASSERT_EQUAL_UINT32(
        interface_stub.timestamp + TIMESTAMP_MAX_DELTA, 
        interface_stub.interrupt_timestamp
    );
    TEST_ASSERT_EQUAL_PTR(NULL, queue_stub.head->next);
    TEST_ASSERT_EQUAL_UINT64(timestamp_last_event, last_event.timestamp);
    TEST_ASSERT_EQUAL_UINT32(id_last_event, last_event.id);

    // test the beginning of the range
    ++interface_stub.timestamp;
    ++queue_stub.present_time;

    ticker_event_t first_event = { 0 };
    const us_timestamp_t timestamp_first_event = 
        queue_stub.present_time + TIMESTAMP_MAX_DELTA + 1;
    uint32_t id_first_event = 0xAAAAAAAA;

    ticker_insert_event_us(&ticker_stub, 
        &first_event, timestamp_first_event, id_first_event
    );

    TEST_ASSERT_EQUAL_PTR(&first_event, queue_stub.head);
    TEST_ASSERT_EQUAL(2, interface_stub.set_interrupt_call);    
    TEST_ASSERT_EQUAL_UINT32(
        interface_stub.timestamp + TIMESTAMP_MAX_DELTA, 
        interface_stub.interrupt_timestamp
    );
    TEST_ASSERT_EQUAL_PTR(&last_event, queue_stub.head->next);
    TEST_ASSERT_EQUAL_UINT64(timestamp_first_event, first_event.timestamp);
    TEST_ASSERT_EQUAL_UINT32(id_first_event, first_event.id);

    TEST_ASSERT_EQUAL(0, interface_stub.disable_interrupt_call);
} 

/**
 * Given an initialized ticker without user registered events.  
 * When an event is inserted with ticker_insert_event_us and a timestamp less 
 * than timestamp value in the ticker interface.
 * Then 
 *   - The event should be in the queue
 *   - The interrupt timestamp should be set to interface_stub.timestamp so it
 *     is scheduled immediately.
 */
static void test_insert_event_us_underflow()
{
    ticker_set_handler(&ticker_stub, NULL);
    interface_stub.set_interrupt_call = 0;

    interface_stub.timestamp = 0xAAAAAAAA;
    queue_stub.tick_last_read = interface_stub.timestamp;
    queue_stub.present_time = 10ULL << 32 | interface_stub.timestamp;

    // test the end of the range
    ticker_event_t event = { 0 };
    const timestamp_t expected_timestamp = queue_stub.present_time - 1;
    const uint32_t expected_id = 0xDEADDEAF;

    ticker_insert_event_us(
        &ticker_stub,
        &event, expected_timestamp, expected_id
    );

    TEST_ASSERT_EQUAL_PTR(&event, queue_stub.head);
    TEST_ASSERT_EQUAL(1, interface_stub.fire_interrupt_call);

    TEST_ASSERT_EQUAL(0, interface_stub.disable_interrupt_call);
}  

/**
 * Given an initialized ticker.
 * When an event is inserted with ticker_insert_event_us and a timestamp less 
 * than the one for the next scheduled timestamp.
 * Then 
 *   - The event inserted should be the first in the queue
 *   - The interrupt timestamp should be equal to the timestamp of the event or 
 *     TIMESTAMP_MAX_DELTA if in the overflow range.
 *   - The timestamp of the event should be equal to the timestamp in parameter. 
 *   - The id of the event should be equal to the id passed in parameter. 
 *   - Events in the queue should remained ordered by timestamp.
 */
static void test_insert_event_us_head()
{
    ticker_set_handler(&ticker_stub, NULL);
    interface_stub.set_interrupt_call = 0;
    interface_stub.timestamp = 0xAAAAAAAA;
    queue_stub.tick_last_read = interface_stub.timestamp;
    queue_stub.present_time = 10ULL << 32 | interface_stub.timestamp;

    const us_timestamp_t timestamps[] = { 
        UINT64_MAX,
        queue_stub.present_time + TIMESTAMP_MAX_DELTA + 1,
        queue_stub.present_time + TIMESTAMP_MAX_DELTA,
        queue_stub.present_time + (TIMESTAMP_MAX_DELTA / 2),
        queue_stub.present_time + (TIMESTAMP_MAX_DELTA / 4),
        queue_stub.present_time + (TIMESTAMP_MAX_DELTA / 8),
        queue_stub.present_time + (TIMESTAMP_MAX_DELTA / 16),
    };
    ticker_event_t events[MBED_ARRAY_SIZE(timestamps)] = { 0 };

    for (size_t i = 0; i < MBED_ARRAY_SIZE(events); ++i) { 
        ticker_insert_event_us(
            &ticker_stub, 
            &events[i], timestamps[i], i
        );

        TEST_ASSERT_EQUAL_PTR(&events[i], queue_stub.head);
        if ((timestamps[i] - queue_stub.present_time) < TIMESTAMP_MAX_DELTA) {
            TEST_ASSERT_EQUAL_UINT32(
                timestamps[i], 
                interface_stub.interrupt_timestamp
            );
        } else { 
            TEST_ASSERT_EQUAL_UINT32(
                queue_stub.present_time + TIMESTAMP_MAX_DELTA,
                interface_stub.interrupt_timestamp
            );
        }

        TEST_ASSERT_EQUAL_UINT64(timestamps[i], events[i].timestamp);
        TEST_ASSERT_EQUAL_UINT32(i, events[i].id);

        ticker_event_t* e = &events[i];
        while (e) { 
            TEST_ASSERT_EQUAL_UINT32(timestamps[e->id], e->timestamp);
            if (e->next) { 
                TEST_ASSERT_TRUE(e->id > e->next->id);
                TEST_ASSERT_TRUE(e->timestamp < e->next->timestamp);
            } else { 
                TEST_ASSERT_EQUAL_UINT32(0, e->id);
            }
            e = e->next;
        }
    }

    TEST_ASSERT_EQUAL(0, interface_stub.disable_interrupt_call);
}  

/** 
 * Given an initialized ticker.
 * When an event is inserted with ticker_insert_event_us and its timestamp is 
 * bigger than the one of the last event in the queue.
 * Then 
 *   - The event inserted should be the last in the queue
 *   - The interrupt timestamp should remains equal to the interrupt timestamp 
 *     of the head event .
 *   - The timestamp of the event should reflect the timestamp requested.
 *   - The id of the event should be equal to the id passed in parameter. 
 *   - Events in the queue should remained ordered by timestamp.
 */
static void test_insert_event_us_tail()
{
    ticker_set_handler(&ticker_stub, NULL);
    interface_stub.set_interrupt_call = 0;

    const us_timestamp_t timestamps[] = { 
        0xA,
        (1ULL << 32),
        (2ULL << 32),
        (4ULL << 32),
        (8ULL << 32),
        (16ULL << 32),
        (32ULL << 32),
        (64ULL << 32),
    };
    ticker_event_t events[MBED_ARRAY_SIZE(timestamps)] = { 0 };

    for (size_t i = 0; i < MBED_ARRAY_SIZE(events); ++i) { 
        ticker_insert_event_us(
            &ticker_stub, 
            &events[i], timestamps[i], i
        );

        TEST_ASSERT_EQUAL_PTR(&events[0], queue_stub.head); 
        TEST_ASSERT_EQUAL_UINT32(
            timestamps[0], interface_stub.interrupt_timestamp
        );
        TEST_ASSERT_EQUAL_UINT64(timestamps[i], events[i].timestamp);
        TEST_ASSERT_EQUAL_UINT32(i, events[i].id);

        ticker_event_t* e = queue_stub.head;
        while (e) { 
            TEST_ASSERT_EQUAL_UINT32(timestamps[e->id], e->timestamp);
            if (e->next) { 
                TEST_ASSERT_TRUE(e->id < e->next->id);
                TEST_ASSERT_TRUE(e->timestamp < e->next->timestamp);
            } else { 
                TEST_ASSERT_EQUAL_UINT32(&events[i], e);
            }
            e = e->next;
        }
    }

    TEST_ASSERT_EQUAL(0, interface_stub.disable_interrupt_call);
} 

/**
 * Given an initialized ticker.
 * When an event is inserted with ticker_insert_event_us.
 * Then 
 *   - The event inserted should be at the correct position in the queue
 *   - The event queue should remain ordered by timestamp
 *   - The interrupt timestamp should be equal to the interrupt timestamp 
 *     of the head event or TIMESTAMP_MAX_DELTA if the 
 *     timestamp is in the overflow range.
 *   - The timestamp of the event should be equal to the timestamp parameter.
 *   - The id of the event should be equal to the id passed in parameter. 
 *   - Events in the queue should remained ordered by timestamp.
 */
static void test_insert_event_us_multiple_random()
{
    ticker_set_handler(&ticker_stub, NULL);
    interface_stub.set_interrupt_call = 0;

    const timestamp_t ref_timestamp = UINT32_MAX / 2;
    interface_stub.timestamp = ref_timestamp;

    // insert first event at the head of the queue 
    ticker_event_t first_event;
    const us_timestamp_t first_event_timestamp = 
        ref_timestamp + TIMESTAMP_MAX_DELTA + 100;

    ticker_insert_event_us(
        &ticker_stub,
        &first_event, first_event_timestamp, (uint32_t) &first_event
    );

    TEST_ASSERT_EQUAL_PTR(&first_event, queue_stub.head);
    TEST_ASSERT_EQUAL_PTR(NULL, first_event.next);
    TEST_ASSERT_EQUAL_UINT32(
        ref_timestamp + TIMESTAMP_MAX_DELTA, interface_stub.interrupt_timestamp
    );
    TEST_ASSERT_EQUAL_UINT64(first_event.timestamp, first_event_timestamp);
    TEST_ASSERT_EQUAL_UINT32((uint32_t) &first_event, first_event.id);

    // insert second event at the tail of the queue
    ticker_event_t second_event;
    const us_timestamp_t second_event_timestamp = first_event_timestamp + 1;

    ticker_insert_event_us(
        &ticker_stub,
        &second_event, second_event_timestamp, (uint32_t) &second_event
    );

    TEST_ASSERT_EQUAL_PTR(&first_event, queue_stub.head);
    TEST_ASSERT_EQUAL_PTR(&second_event, first_event.next);
    TEST_ASSERT_EQUAL_PTR(NULL, second_event.next);
    TEST_ASSERT_EQUAL_UINT32(
        ref_timestamp + TIMESTAMP_MAX_DELTA, interface_stub.interrupt_timestamp
    );
    TEST_ASSERT_EQUAL_UINT64(second_event_timestamp, second_event.timestamp);
    TEST_ASSERT_EQUAL_UINT32((uint32_t) &second_event, second_event.id);


    // insert third event at the head of the queue out the overflow zone
    ticker_event_t third_event;
    const us_timestamp_t third_event_timestamp = 
        ref_timestamp + TIMESTAMP_MAX_DELTA - 100;

    ticker_insert_event_us(
        &ticker_stub,
        &third_event, third_event_timestamp, (uint32_t) &third_event
    );

    TEST_ASSERT_EQUAL_PTR(&third_event, queue_stub.head);
    TEST_ASSERT_EQUAL_PTR(&first_event, third_event.next);
    TEST_ASSERT_EQUAL_PTR(&second_event, first_event.next);
    TEST_ASSERT_EQUAL_PTR(NULL, second_event.next);
    TEST_ASSERT_EQUAL_UINT32(
        third_event_timestamp, interface_stub.interrupt_timestamp
    );
    TEST_ASSERT_EQUAL_UINT64(third_event_timestamp, third_event.timestamp);
    TEST_ASSERT_EQUAL_UINT32((uint32_t) &third_event, third_event.id);

    // insert fourth event right after the third event
    ticker_event_t fourth_event;
    const us_timestamp_t fourth_event_timestamp = third_event_timestamp + 50;

    ticker_insert_event_us(
        &ticker_stub,
        &fourth_event, fourth_event_timestamp, (uint32_t) &fourth_event
    );

    TEST_ASSERT_EQUAL_PTR(&third_event, queue_stub.head);
    TEST_ASSERT_EQUAL_PTR(&fourth_event, third_event.next);
    TEST_ASSERT_EQUAL_PTR(&first_event, fourth_event.next);
    TEST_ASSERT_EQUAL_PTR(&second_event, first_event.next);
    TEST_ASSERT_EQUAL_PTR(NULL, second_event.next);
    TEST_ASSERT_EQUAL_UINT32(
        third_event_timestamp, interface_stub.interrupt_timestamp
    );
    TEST_ASSERT_EQUAL_UINT64(fourth_event_timestamp, fourth_event.timestamp);
    TEST_ASSERT_EQUAL_UINT32((uint32_t) &fourth_event, fourth_event.id);

    TEST_ASSERT_EQUAL(0, interface_stub.disable_interrupt_call);
}

/**
 * Given an initialized ticker with multiple events registered. 
 * When the event at the tail of the queue is removed from the queue.
 * Then: 
 *    - The event should not be in the queue.
 *    - The events in the queue should remain ordered
 *    - The interrupt timestamp should be unchanged. 
 */
static void test_remove_event_tail()
{
    ticker_set_handler(&ticker_stub, NULL);
    const us_timestamp_t timestamps[] = { 
        0xA,
        (1ULL << 32),
        (2ULL << 32),
        (4ULL << 32),
        (8ULL << 32),
        (16ULL << 32),
        (32ULL << 32),
        (64ULL << 32),
    };
    ticker_event_t events[MBED_ARRAY_SIZE(timestamps)] = { 0 };

    for (size_t i = 0; i < MBED_ARRAY_SIZE(events); ++i) { 
        ticker_insert_event_us(
            &ticker_stub, 
            &events[i], timestamps[i], i
        );
    }

    for (ssize_t i = MBED_ARRAY_SIZE(events) - 1; i >= 0; --i) { 
        ticker_remove_event(&ticker_stub, &events[i]);

        ticker_event_t* e = queue_stub.head;
        size_t event_count = 0;
        while (e) { 
            TEST_ASSERT_NOT_EQUAL(e, &events[i]);
            if (e->next) { 
                TEST_ASSERT_TRUE(e->timestamp <= e->next->timestamp);
            }
            e = e->next;
            ++event_count;
        }

        TEST_ASSERT_EQUAL(i, event_count);

        if (i != 0 ) { 
            TEST_ASSERT_EQUAL(
                timestamps[0],
                interface_stub.interrupt_timestamp
            );
        } else { 
            TEST_ASSERT_EQUAL(
                interface_stub.timestamp + TIMESTAMP_MAX_DELTA,
                interface_stub.interrupt_timestamp
            );
        }
    }

    TEST_ASSERT_EQUAL(0, interface_stub.disable_interrupt_call);
}

/**
 * Given an initialized ticker with multiple events registered. 
 * When the event at the head of the queue is removed from the queue.
 * Then: 
 *    - The event should not be in the queue.
 *    - The event at the head of the queue should be the equal to the one 
 *      after the event removed.
 *    - The interrupt timestamp should be equal to the interrupt timestamp 
 *      of the head event or TIMESTAMP_MAX_DELTA if the 
 *      timestamp is in the overflow range.
 */
static void test_remove_event_head()
{
    ticker_set_handler(&ticker_stub, NULL);
    const us_timestamp_t timestamps[] = { 
        TIMESTAMP_MAX_DELTA / 8,
        TIMESTAMP_MAX_DELTA / 4,
        TIMESTAMP_MAX_DELTA / 2,
        TIMESTAMP_MAX_DELTA - 1,
        TIMESTAMP_MAX_DELTA,
        TIMESTAMP_MAX_DELTA + 1,
        (1ULL << 32) | TIMESTAMP_MAX_DELTA,
        UINT64_MAX
    };
    ticker_event_t events[MBED_ARRAY_SIZE(timestamps)] = { 0 };

    for (size_t i = 0; i < MBED_ARRAY_SIZE(events); ++i) { 
        ticker_insert_event_us(&ticker_stub, 
            &events[i], timestamps[i], i
        );
    }

    for (size_t i = 0; i < MBED_ARRAY_SIZE(events); ++i) { 
        ticker_remove_event(&ticker_stub, &events[i]);

        ticker_event_t* e = queue_stub.head;
        size_t event_count = 0;
        while (e) { 
            TEST_ASSERT_NOT_EQUAL(e, &events[i]);
            if (e->next) { 
                TEST_ASSERT_TRUE(e->timestamp <= e->next->timestamp);
            }
            e = e->next;
            ++event_count;
        }

        TEST_ASSERT_EQUAL(MBED_ARRAY_SIZE(events) - i - 1, event_count);

        if (event_count) { 
            TEST_ASSERT_EQUAL(
                std::min(
                    timestamps[i + 1], 
                    interface_stub.timestamp + TIMESTAMP_MAX_DELTA
                ),
                interface_stub.interrupt_timestamp
            );
        } else { 
            TEST_ASSERT_EQUAL(
                interface_stub.timestamp + TIMESTAMP_MAX_DELTA,
                interface_stub.interrupt_timestamp
            );
        }

    }

    TEST_ASSERT_EQUAL(0, interface_stub.disable_interrupt_call);
}

/**
 * Given an initialized ticker with multiple events registered. 
 * When an event not in the queue is attempted to be removed.
 * Then the queue should remains identical as before.
 */
static void test_remove_event_invalid()
{
    ticker_set_handler(&ticker_stub, NULL);
    const us_timestamp_t timestamps[] = { 
        TIMESTAMP_MAX_DELTA / 8,
        TIMESTAMP_MAX_DELTA / 4,
        TIMESTAMP_MAX_DELTA / 2,
        TIMESTAMP_MAX_DELTA - 1,
        TIMESTAMP_MAX_DELTA,
        TIMESTAMP_MAX_DELTA + 1,
        (1ULL << 32) | TIMESTAMP_MAX_DELTA,
        UINT64_MAX
    };
    ticker_event_t events[MBED_ARRAY_SIZE(timestamps)] = { 0 };

    for (size_t i = 0; i < MBED_ARRAY_SIZE(events); ++i) { 
        ticker_insert_event_us(
            &ticker_stub, 
            &events[i], timestamps[i], i
        );
    }

    ticker_event_t invalid_event;
    ticker_remove_event(&ticker_stub, &invalid_event);

    TEST_ASSERT_EQUAL(&events[0], queue_stub.head);

    ticker_event_t* e = queue_stub.head;
    size_t event_count = 0;
    while (e) { 
        TEST_ASSERT_EQUAL(e, &events[event_count]);
        e = e->next;
        ++event_count;
    }
    TEST_ASSERT_EQUAL(MBED_ARRAY_SIZE(events), event_count);
}

/**
 * Given an initialized ticker with multiple events inserted.
 * When an event is remoced
 * Then:
 *   - the event should not be in the queue
 *   - the queue should remain ordered
 *   - the interrupt timestamp should be set to either head->timestamp or 
 *     TIMESTAMP_MAX_DELTA depending on the distance between the current time 
 *     ans the timestamp of the event at the head of the queue.
 */
static void test_remove_random()
{
    ticker_set_handler(&ticker_stub, NULL);
    interface_stub.set_interrupt_call = 0;

    const timestamp_t ref_timestamp = UINT32_MAX / 2;
    interface_stub.timestamp = ref_timestamp;

    // insert all events 
    ticker_event_t first_event;
    const us_timestamp_t first_event_timestamp = 
        ref_timestamp + TIMESTAMP_MAX_DELTA + 100;

    ticker_insert_event_us(
        &ticker_stub,
        &first_event, first_event_timestamp, (uint32_t) &first_event
    );


    ticker_event_t second_event;
    const us_timestamp_t second_event_timestamp = first_event_timestamp + 1;

    ticker_insert_event_us(
        &ticker_stub,
        &second_event, second_event_timestamp, (uint32_t) &second_event
    );

    ticker_event_t third_event;
    const us_timestamp_t third_event_timestamp = 
        ref_timestamp + TIMESTAMP_MAX_DELTA - 100;

    ticker_insert_event_us(
        &ticker_stub,
        &third_event, third_event_timestamp, (uint32_t) &third_event
    );

    ticker_event_t fourth_event;
    const us_timestamp_t fourth_event_timestamp = third_event_timestamp + 50;

    ticker_insert_event_us(
        &ticker_stub,
        &fourth_event, fourth_event_timestamp, (uint32_t) &fourth_event
    );

    // test that the queue is in the correct state 
    TEST_ASSERT_EQUAL_PTR(&third_event, queue_stub.head);
    TEST_ASSERT_EQUAL_PTR(&fourth_event, third_event.next);
    TEST_ASSERT_EQUAL_PTR(&first_event, fourth_event.next);
    TEST_ASSERT_EQUAL_PTR(&second_event, first_event.next);
    TEST_ASSERT_EQUAL_PTR(NULL, second_event.next);
    TEST_ASSERT_EQUAL_UINT32(
        third_event_timestamp, interface_stub.interrupt_timestamp
    );
    TEST_ASSERT_EQUAL_UINT64(fourth_event_timestamp, fourth_event.timestamp);
    TEST_ASSERT_EQUAL_UINT32((uint32_t) &fourth_event, fourth_event.id);

    // remove fourth event 
    ticker_remove_event(&ticker_stub, &fourth_event);

    TEST_ASSERT_EQUAL_PTR(&third_event, queue_stub.head);
    TEST_ASSERT_EQUAL_PTR(&first_event, third_event.next);
    TEST_ASSERT_EQUAL_PTR(&second_event, first_event.next);
    TEST_ASSERT_EQUAL_PTR(NULL, second_event.next);
    TEST_ASSERT_EQUAL_UINT32(
        third_event_timestamp, interface_stub.interrupt_timestamp
    );
    TEST_ASSERT_EQUAL_UINT64(third_event_timestamp, third_event.timestamp);
    TEST_ASSERT_EQUAL_UINT32((uint32_t) &third_event, third_event.id);

    // remove third event 
    ticker_remove_event(&ticker_stub, &third_event);

    TEST_ASSERT_EQUAL_PTR(&first_event, queue_stub.head);
    TEST_ASSERT_EQUAL_PTR(&second_event, first_event.next);
    TEST_ASSERT_EQUAL_PTR(NULL, second_event.next);
    TEST_ASSERT_EQUAL_UINT32(
        ref_timestamp + TIMESTAMP_MAX_DELTA, interface_stub.interrupt_timestamp
    );
    TEST_ASSERT_EQUAL_UINT64(second_event_timestamp, second_event.timestamp);
    TEST_ASSERT_EQUAL_UINT32((uint32_t) &second_event, second_event.id);

    // remove second event 
    ticker_remove_event(&ticker_stub, &second_event);

    TEST_ASSERT_EQUAL_PTR(&first_event, queue_stub.head);
    TEST_ASSERT_EQUAL_PTR(NULL, first_event.next);
    TEST_ASSERT_EQUAL_UINT32(
        ref_timestamp + TIMESTAMP_MAX_DELTA, interface_stub.interrupt_timestamp
    );
    TEST_ASSERT_EQUAL_UINT64(first_event.timestamp, first_event_timestamp);
    TEST_ASSERT_EQUAL_UINT32((uint32_t) &first_event, first_event.id);

    // remove first event 
    ticker_remove_event(&ticker_stub, &first_event);

    TEST_ASSERT_EQUAL_PTR(NULL, queue_stub.head);
    TEST_ASSERT_EQUAL_PTR(NULL, first_event.next);
    TEST_ASSERT_EQUAL_UINT32(
        ref_timestamp + TIMESTAMP_MAX_DELTA, interface_stub.interrupt_timestamp
    );

    TEST_ASSERT_EQUAL(0, interface_stub.disable_interrupt_call);
}

/** 
 * Given an initialized ticker without user registered events and a ticker 
 * interface timestamp equal or bigger than the one registered by the overflow 
 * event.
 * When the interrupt handler is called.
 * Then:
 *   - The interrupt timestamp should be updated to the timestamp of the ticker 
 *     interface plus TIMESTAMP_MAX_DELTA. 
 *   - The irq handler registered should not be called.
 */
static void test_overflow_event_update()
{
    static uint32_t handler_call = 0;
    struct irq_handler_stub_t { 
        static void event_handler(uint32_t id) { 
            ++handler_call;
        }
    };
    handler_call = 0;

    ticker_set_handler(&ticker_stub, irq_handler_stub_t::event_handler);
    interface_stub.set_interrupt_call = 0;

    for (size_t i = 0; i < 8; ++i) {
        us_timestamp_t previous_timestamp = queue_stub.present_time;
        timestamp_t interface_timestamp = 
            previous_timestamp + (TIMESTAMP_MAX_DELTA + i * 100);
        interface_stub.timestamp = interface_timestamp;
            
        ticker_irq_handler(&ticker_stub);
        TEST_ASSERT_EQUAL(i + 1, interface_stub.clear_interrupt_call);

        TEST_ASSERT_EQUAL(i + 1, interface_stub.set_interrupt_call);
        TEST_ASSERT_EQUAL_UINT32(
            interface_timestamp + TIMESTAMP_MAX_DELTA,
            interface_stub.interrupt_timestamp
        );
        TEST_ASSERT_EQUAL(0, handler_call);
    }

    TEST_ASSERT_EQUAL(0, interface_stub.disable_interrupt_call);
}  

/** 
 * Given an initialized ticker without user registered events and a ticker 
 * interface timestamp less than the one registered to handle overflow.
 * When the interrupt handler is called.
 * Then:
 *   - The interrupt timestamp should be updated to the timestamp of the ticker 
 *     interface plus TIMESTAMP_MAX_DELTA. 
 *   - The irq handler registered should not be called.
 */
static void test_overflow_event_update_when_spurious_interrupt()
{
    static uint32_t handler_call = 0;
    struct irq_handler_stub_t { 
        static void event_handler(uint32_t id) { 
            ++handler_call;
        }
    };
    handler_call = 0;

    ticker_set_handler(&ticker_stub, irq_handler_stub_t::event_handler);
    interface_stub.set_interrupt_call = 0;

    for (size_t i = 0; i < 8; ++i) {
        us_timestamp_t previous_timestamp = queue_stub.present_time;
        timestamp_t interface_timestamp = 
            previous_timestamp + (TIMESTAMP_MAX_DELTA / (2 + i));
        interface_stub.timestamp = interface_timestamp;
            
        ticker_irq_handler(&ticker_stub);

        TEST_ASSERT_EQUAL(i + 1, interface_stub.clear_interrupt_call);
        TEST_ASSERT_EQUAL(i + 1, interface_stub.set_interrupt_call);
        TEST_ASSERT_EQUAL_UINT32(
            interface_timestamp + TIMESTAMP_MAX_DELTA,
            interface_stub.interrupt_timestamp
        );
        TEST_ASSERT_EQUAL(0, handler_call);
    }

    TEST_ASSERT_EQUAL(0, interface_stub.disable_interrupt_call);
}  

/**
 * Given an initialized ticker with a single ticker event inserted and a ticker 
 * interface timestamp bigger than the one set for interrupt. 
 * When ticker_irq_handler is called.
 * Then: 
 *   - The IRQ handler should be called with the id of the event at the head of 
 *     the queue.
 *   - The event at the head of the queue should be replaced by the next event.
 *   - The interrupt timestamp in the ticker interface should be set to the 
 *     value of the interface timestamp + TIMESTAMP_MAX_DELTA  
 */
static void test_irq_handler_single_event()
{
    static const timestamp_t event_timestamp = 0xAAAAAAAA;
    static const timestamp_t interface_timestamp_after_irq = event_timestamp + 100;

    uint32_t handler_call = 0;
    struct irq_handler_stub_t { 
        static void event_handler(uint32_t id) { 
            ++ (*((uint32_t*) id));
            interface_stub.timestamp = interface_timestamp_after_irq;
        }
    };

    ticker_set_handler(&ticker_stub, irq_handler_stub_t::event_handler);
    interface_stub.set_interrupt_call = 0;

    ticker_event_t e; 
    ticker_insert_event(&ticker_stub, &e, event_timestamp, (uint32_t) &handler_call);

    interface_stub.timestamp = event_timestamp;
    interface_stub.set_interrupt_call = 0;

    ticker_irq_handler(&ticker_stub);

    TEST_ASSERT_EQUAL(1, interface_stub.clear_interrupt_call);
    TEST_ASSERT_EQUAL(1, interface_stub.set_interrupt_call);
    TEST_ASSERT_EQUAL(1, handler_call);
    TEST_ASSERT_EQUAL_UINT32(
        interface_timestamp_after_irq + TIMESTAMP_MAX_DELTA,
        interface_stub.interrupt_timestamp
    );

    TEST_ASSERT_NULL(queue_stub.head);

    TEST_ASSERT_EQUAL(0, interface_stub.disable_interrupt_call);
}

/**
 * Given an initialized ticker with at least a ticker event inserted and a ticker 
 * interface timestamp less than the one set for interrupt. 
 * When ticker_irq_handler is called.
 * Then: 
 *   - The IRQ handler should not be called.
 *   - The event at the head of the queue should remains the same.
 *   - The interrupt timestamp in the ticker interface should be set to the 
 *     value of the event timestamp  
 */
static void test_irq_handler_single_event_spurious()
{
    struct irq_handler_stub_t { 
        static void event_handler(uint32_t id) { 
            TEST_FAIL();
        }
    };

    ticker_set_handler(&ticker_stub, irq_handler_stub_t::event_handler);
    interface_stub.set_interrupt_call = 0;

    const us_timestamp_t timestamps [] = {
        UINT32_MAX, 
        TIMESTAMP_MAX_DELTA + 1, 
        TIMESTAMP_MAX_DELTA,
        TIMESTAMP_MAX_DELTA - 1
    };

    ticker_event_t events[MBED_ARRAY_SIZE(timestamps)] = { 0 };

    for (size_t i = 0; i < MBED_ARRAY_SIZE(events); ++i) { 
        ticker_insert_event_us(
            &ticker_stub, 
            &events[i], timestamps[i], timestamps[i]
        );
        interface_stub.set_interrupt_call = 0;
        interface_stub.clear_interrupt_call = 0;

        ticker_irq_handler(&ticker_stub);

        TEST_ASSERT_EQUAL(1, interface_stub.clear_interrupt_call);
        TEST_ASSERT_EQUAL(1, interface_stub.set_interrupt_call);
        TEST_ASSERT_EQUAL_UINT32(
            std::min(timestamps[i], TIMESTAMP_MAX_DELTA),
            interface_stub.interrupt_timestamp
        );
    }

    TEST_ASSERT_EQUAL(0, interface_stub.disable_interrupt_call);
}

/**
 * Given an initialized ticker with multiple ticker event inserted, its 
 * interface timestamp at greater than the timestamp of the next schedule event 
 * and all event execution time taking at least the time befor ethe next event.
 * When ticker_irq_handler is called.
 * Then: 
 *   - The IRQ handler should have been called for every event.
 *   - The head of the queue should be set to NULL.
 *   - The interrupt timestamp in the ticker interface should be scheduled in
 *     TIMESTAMP_MAX_DELTAs
 */
static void test_irq_handler_multiple_event_multiple_dequeue()
{
    const us_timestamp_t timestamps [] = {
        10, 
        10 + TIMESTAMP_MAX_DELTA - 1,
        10 + TIMESTAMP_MAX_DELTA,
        10 + TIMESTAMP_MAX_DELTA + 1, 
        UINT32_MAX
    };

    static size_t handler_called = 0;
    struct irq_handler_stub_t { 
        static void event_handler(uint32_t id) { 
            ++handler_called;
            ticker_event_t* e = (ticker_event_t*) id;
            if (e->next) { 
                interface_stub.timestamp = e->next->timestamp;
            }
        }
    };
    handler_called = 0;

    ticker_set_handler(&ticker_stub, irq_handler_stub_t::event_handler);
    interface_stub.set_interrupt_call = 0;

    ticker_event_t events[MBED_ARRAY_SIZE(timestamps)] = { 0 };

    for (size_t i = 0; i < MBED_ARRAY_SIZE(events); ++i) { 
        ticker_insert_event_us(
            &ticker_stub, 
            &events[i], timestamps[i], (uint32_t) &events[i]
        );
    }

    interface_stub.set_interrupt_call = 0;
    interface_stub.clear_interrupt_call = 0;
    interface_stub.timestamp = timestamps[0];
    
    ticker_irq_handler(&ticker_stub);

    TEST_ASSERT_EQUAL(1, interface_stub.clear_interrupt_call);
    TEST_ASSERT_EQUAL(1, interface_stub.set_interrupt_call);
    TEST_ASSERT_EQUAL_UINT32(MBED_ARRAY_SIZE(timestamps), handler_called);
    TEST_ASSERT_EQUAL_UINT32(
        timestamps[MBED_ARRAY_SIZE(timestamps) - 1] + TIMESTAMP_MAX_DELTA,
        interface_stub.interrupt_timestamp
    );
    TEST_ASSERT_NULL(queue_stub.head);

    TEST_ASSERT_EQUAL(0, interface_stub.disable_interrupt_call);
}

/**
 * Given an initialized ticker with two ticker event inserted scheduled from more 
 * than TIMESTAMP_MAX_DELTA from one another. The interface 
 * timestamp is equal to the timestamp of the first event. 
 * When ticker_irq_handler is called.
 * Then: 
 *   - The IRQ handler should have been called for the first event.
 *   - The head of the queue should be set to the event after the first event.
 *   - The interrupt timestamp in the ticker interface should be scheduled in
 *     TIMESTAMP_MAX_DELTA.
 */
static void test_irq_handler_multiple_event_single_dequeue_overflow()
{
    const us_timestamp_t timestamps [] = {
        10, 
        10 + TIMESTAMP_MAX_DELTA + 1
    };

    size_t handler_called = 0;
    struct irq_handler_stub_t { 
        static void event_handler(uint32_t id) { 
            ++ (*((size_t*) id));
        }
    };

    ticker_set_handler(&ticker_stub, irq_handler_stub_t::event_handler);
    interface_stub.set_interrupt_call = 0;

    ticker_event_t events[MBED_ARRAY_SIZE(timestamps)] = { 0 };

    for (size_t i = 0; i < MBED_ARRAY_SIZE(events); ++i) { 
        ticker_insert_event_us(
            &ticker_stub, 
            &events[i], timestamps[i], (uint32_t) &handler_called
        );
    }

    interface_stub.set_interrupt_call = 0;
    interface_stub.clear_interrupt_call = 0;
    interface_stub.timestamp = timestamps[0];
    
    ticker_irq_handler(&ticker_stub);

    TEST_ASSERT_EQUAL(1, interface_stub.clear_interrupt_call);
    TEST_ASSERT_EQUAL(1, interface_stub.set_interrupt_call);
    TEST_ASSERT_EQUAL_UINT32(1, handler_called);
    TEST_ASSERT_EQUAL_UINT32(
        timestamps[0] + TIMESTAMP_MAX_DELTA,
        interface_stub.interrupt_timestamp
    );
    TEST_ASSERT_EQUAL_PTR(&events[1], queue_stub.head);

    TEST_ASSERT_EQUAL(0, interface_stub.disable_interrupt_call);
}

/**
 * Given an initialized ticker with two ticker event inserted scheduled from less 
 * than TIMESTAMP_MAX_DELTA from one another. The interface 
 * timestamp is equal to the timestamp of the first event. 
 * When ticker_irq_handler is called.
 * Then: 
 *   - The IRQ handler should have been called for the first event.
 *   - The head of the queue should be set to second event.
 *   - The interrupt timestamp in the ticker interface should be equal to the 
 *     timestamp of the second event.
 */
static void test_irq_handler_multiple_event_single_dequeue()
{
    const us_timestamp_t timestamps [] = {
        10, 
        10 + TIMESTAMP_MAX_DELTA - 1
    };

    size_t handler_called = 0;
    struct irq_handler_stub_t { 
        static void event_handler(uint32_t id) { 
            ++ (*((size_t*) id));
        }
    };

    ticker_set_handler(&ticker_stub, irq_handler_stub_t::event_handler);
    interface_stub.set_interrupt_call = 0;

    ticker_event_t events[MBED_ARRAY_SIZE(timestamps)] = { 0 };

    for (size_t i = 0; i < MBED_ARRAY_SIZE(events); ++i) { 
        ticker_insert_event_us(
            &ticker_stub, 
            &events[i], timestamps[i], (uint32_t) &handler_called
        );
    }

    interface_stub.set_interrupt_call = 0;
    interface_stub.clear_interrupt_call = 0;
    interface_stub.timestamp = timestamps[0];
    
    ticker_irq_handler(&ticker_stub);

    TEST_ASSERT_EQUAL(1, interface_stub.clear_interrupt_call);
    TEST_ASSERT_EQUAL(1, interface_stub.set_interrupt_call);
    TEST_ASSERT_EQUAL_UINT32(1, handler_called);
    TEST_ASSERT_EQUAL_UINT32(
        timestamps[1],
        interface_stub.interrupt_timestamp
    );
    TEST_ASSERT_EQUAL_PTR(&events[1], queue_stub.head);

    TEST_ASSERT_EQUAL(0, interface_stub.disable_interrupt_call);
}

/**
 * Given an initialized ticker with multiple ticker event inserted and the 
 * interface timestamp is equal to the timestamp of the first event. The first
 * event to execute will insert an events in the ticker which have to be executed 
 * immediately.
 * When ticker_irq_handler is called.
 * Then: 
 *   - The IRQ handler should have been called for the first event and the event 
 *     inserted during irq.
 *   - The head of the queue should be set correctly.
 *   - The interrupt timestamp in the ticker interface should be equal to 
 *     timestamp of the head event.
 */
static void test_irq_handler_insert_immediate_in_irq()
{
    static const us_timestamp_t timestamps [] = {
        10, 
        10 + TIMESTAMP_MAX_DELTA - 1
    };

    static const us_timestamp_t expected_timestamp = 
        ((timestamps[1] - timestamps[0]) / 2) + timestamps[0];

    struct ctrl_block_t { 
        bool irq_event_called;
        ticker_event_t immediate_event; 
        size_t handler_called;
    };

    ctrl_block_t ctrl_block = { 0 };

    struct irq_handler_stub_t { 
        static void event_handler(uint32_t id) { 
            ctrl_block_t*  ctrl_block = (ctrl_block_t*) id;

            if (ctrl_block->handler_called == 0) {
                ticker_insert_event(
                    &ticker_stub, 
                    &ctrl_block->immediate_event, expected_timestamp, id
                );
                interface_stub.timestamp = expected_timestamp;
            } else if (ctrl_block->handler_called > 1) { 
                TEST_FAIL();
            }
            ++ctrl_block->handler_called;
        }
    };

    ticker_set_handler(&ticker_stub, irq_handler_stub_t::event_handler);
    interface_stub.set_interrupt_call = 0;

    ticker_event_t events[MBED_ARRAY_SIZE(timestamps)] = { 0 };

    for (size_t i = 0; i < MBED_ARRAY_SIZE(events); ++i) { 
        ticker_insert_event_us(
            &ticker_stub, 
            &events[i], timestamps[i], (uint32_t) &ctrl_block
        );
    }

    interface_stub.set_interrupt_call = 0;
    interface_stub.clear_interrupt_call = 0;
    interface_stub.timestamp = timestamps[0];
    
    ticker_irq_handler(&ticker_stub);

    TEST_ASSERT_EQUAL(1, interface_stub.clear_interrupt_call);
    TEST_ASSERT_EQUAL_UINT32(2, ctrl_block.handler_called);
    TEST_ASSERT_EQUAL_UINT32(
        timestamps[1],
        interface_stub.interrupt_timestamp
    );
    TEST_ASSERT_EQUAL_PTR(&events[1], queue_stub.head);

    TEST_ASSERT_EQUAL(0, interface_stub.disable_interrupt_call);
}

/**
 * Given an initialized ticker with multiple ticker event inserted and the 
 * interface timestamp is equal to the timestamp of the first event. The first
 * event to execute will insert an events in the ticker which does not have to 
 * be executed  immediately.
 * When ticker_irq_handler is called.
 * Then: 
 *   - The IRQ handler should have been called for the first event.
 *   - The head of the queue should be set to the event inserted in IRQ.
 *   - The interrupt timestamp in the ticker interface should be equal to 
 *     timestamp of the head event.
 */
static void test_irq_handler_insert_non_immediate_in_irq()
{
    static const us_timestamp_t timestamps [] = {
        10, 
        10 + TIMESTAMP_MAX_DELTA - 1
    };

    static const us_timestamp_t expected_timestamp = 
        ((timestamps[1] - timestamps[0]) / 2) + timestamps[0];

    struct ctrl_block_t { 
        bool irq_event_called;
        ticker_event_t non_immediate_event; 
        size_t handler_called;
    };

    ctrl_block_t ctrl_block = { 0 };

    struct irq_handler_stub_t { 
        static void event_handler(uint32_t id) { 
            ctrl_block_t*  ctrl_block = (ctrl_block_t*) id;

            if (ctrl_block->handler_called == 0) {
                ticker_insert_event(
                    &ticker_stub, 
                    &ctrl_block->non_immediate_event, expected_timestamp, id
                );
            } else { 
                TEST_FAIL();
            }
            ++ctrl_block->handler_called;
        }
    };


    ticker_set_handler(&ticker_stub, irq_handler_stub_t::event_handler);
    interface_stub.set_interrupt_call = 0;

    ticker_event_t events[MBED_ARRAY_SIZE(timestamps)] = { 0 };

    for (size_t i = 0; i < MBED_ARRAY_SIZE(events); ++i) { 
        ticker_insert_event_us(
            &ticker_stub, 
            &events[i], timestamps[i], (uint32_t) &ctrl_block
        );
    }

    interface_stub.set_interrupt_call = 0;
    interface_stub.clear_interrupt_call = 0;
    interface_stub.timestamp = timestamps[0];
    
    ticker_irq_handler(&ticker_stub);

    TEST_ASSERT_EQUAL(1, interface_stub.clear_interrupt_call);
    TEST_ASSERT_EQUAL_UINT32(1, ctrl_block.handler_called);
    TEST_ASSERT_EQUAL_UINT32(
        expected_timestamp,
        interface_stub.interrupt_timestamp
    );
    TEST_ASSERT_EQUAL_PTR(&ctrl_block.non_immediate_event, queue_stub.head);
    TEST_ASSERT_EQUAL_PTR(&events[1], queue_stub.head->next);

    TEST_ASSERT_EQUAL(0, interface_stub.disable_interrupt_call);
}

static uint32_t ticker_interface_stub_read_interrupt_time()
{
    ++interface_stub.read_call;
    // only if set interrupt call, to test the condition afterwards
    if (interface_stub.set_interrupt_call) {
        return interface_stub.interrupt_timestamp;
    } else {
        return interface_stub.timestamp;
    }
} 

/**
 * Test to insert an event that is already in the past, the fire_interrupt should
 * be invoked, instead of set_interrupt
 */
static void test_set_interrupt_past_time()
{
    ticker_set_handler(&ticker_stub, NULL);

    interface_stub.set_interrupt_call = 0;
    interface_stub.fire_interrupt_call = 0;
    interface_stub.timestamp = 0xFF;


    // This tests fire now functinality when next_event_timestamp <= present
    ticker_event_t event = { 0 };
    const timestamp_t event_timestamp = interface_stub.timestamp;
    const uint32_t id_last_event = 0xDEADDEAF;

    ticker_insert_event(&ticker_stub, &event, event_timestamp, id_last_event);
    TEST_ASSERT_EQUAL(0, interface_stub.set_interrupt_call);
    TEST_ASSERT_EQUAL(1, interface_stub.fire_interrupt_call);
}
/**
 * Test to insert an event that is being delayed, set_interrupt is set
 * but then event is already in the past, thus fire_interrupt should be invoked right-away
 */
static void test_set_interrupt_past_time_with_delay()
{
    ticker_set_handler(&ticker_stub, NULL);

    interface_stub.set_interrupt_call = 0;
    interface_stub.fire_interrupt_call = 0;
    interface_stub.timestamp = 0xFF;

    // This tests fire now functionality when present time >= new_match_time
    interface_stub.interface.read = ticker_interface_stub_read_interrupt_time;
    ticker_event_t event = { 0 };
    const timestamp_t event_timestamp = interface_stub.timestamp + 5;
    const uint32_t id_last_event = 0xDEADDEAF;

    ticker_insert_event(&ticker_stub, &event, event_timestamp, id_last_event);
    TEST_ASSERT_EQUAL(1, interface_stub.set_interrupt_call);
    TEST_ASSERT_EQUAL(1, interface_stub.fire_interrupt_call);
}

/**
 * Convert ticks at a given frequency to time in microseconds
 *
 * Assert if there is a 64-bit overflow
 */
static uint64_t convert_to_us(uint64_t ticks, uint32_t frequency)
{
    uint64_t scaled_ticks = ticks * 1000000;
    // Assert that there was not an overflow
    TEST_ASSERT_EQUAL(ticks, scaled_ticks / 1000000);
    return scaled_ticks / frequency;
}

/**
 * Given an uninitialized ticker instance and an interface of a
 * certain frequency and bit width.
 * Then the time returned the ticker should match the cumulative time.
 */
void test_frequencies_and_masks(uint32_t frequency, uint32_t bits)
{
    const uint32_t bitmask = ((uint64_t)1 << bits) - 1;

    ticker_set_handler(&ticker_stub, NULL);
    uint64_t ticks = 0;

    // Single tick
    ticks += 1;
    interface_stub.timestamp = ticks & bitmask;
    TEST_ASSERT_EQUAL_UINT32(convert_to_us(ticks, frequency), ticker_read(&ticker_stub));
    TEST_ASSERT_EQUAL_UINT64(convert_to_us(ticks, frequency), ticker_read_us(&ticker_stub));

    // Run until the loop before 64-bit overflow (worst case with frequency=1hz, bits=32)
    for (unsigned int k = 0; k < 4294; k++) {

        // Largest value possible tick
        ticks += ((uint64_t)1 << bits) - 1;
        interface_stub.timestamp = ticks & bitmask;
        TEST_ASSERT_EQUAL_UINT32(convert_to_us(ticks, frequency), ticker_read(&ticker_stub));
        TEST_ASSERT_EQUAL_UINT64(convert_to_us(ticks, frequency), ticker_read_us(&ticker_stub));
    }
}

/**
 * Given an uninitialized ticker_data instance.
 * When the ticker is initialized
 * Then:
 *   - The internal ticker timestamp should be zero
 *   - interrupt should be scheduled in current (timestamp +
 *          TIMESTAMP_MAX_DELTA_BITS(bitwidth)) % modval
 *   - The queue should not contains any event
 */
static void test_ticker_max_value()
{
    for (int bitwidth = 8; bitwidth <= 32; bitwidth++) {
        const uint64_t modval = 1ULL << bitwidth;

        // setup of the stub
        reset_ticker_stub();
        interface_info_stub.bits = bitwidth;
        interface_stub.timestamp = 0xBA;

        ticker_set_handler(&ticker_stub, NULL);

        TEST_ASSERT_EQUAL_UINT64(0, queue_stub.present_time);
        TEST_ASSERT_EQUAL(1, interface_stub.set_interrupt_call);
        TEST_ASSERT_EQUAL_UINT32(
            (interface_stub.timestamp + TIMESTAMP_MAX_DELTA_BITS(bitwidth)) % modval,
            interface_stub.interrupt_timestamp
        );
        TEST_ASSERT_EQUAL_PTR(NULL, queue_stub.head);
        TEST_ASSERT_EQUAL(0, interface_stub.disable_interrupt_call);
    }
}

/**
 * Check that _ticker_match_interval_passed correctly detects matches
 *
 * Brute force test that _ticker_match_interval_passed returns the correct match value
 * for all cominations of values within a small range.
 */
static void test_match_interval_passed()
{

    for (int modval = 1; modval <= 5; modval++) {
        for (int prev = 0; prev < modval; prev++) {
            for (int cur = 0; cur < modval; cur++) {
                for (int match = 0; match < modval; match++) {
                    uint32_t delta = (cur - prev) % modval;
                    uint32_t delta_to_match = (match - prev) % modval;
                    bool match_expected = false;
                    if (delta_to_match) {
                        match_expected = delta >= delta_to_match;
                    }

                    // Sanity checks
                    if (prev == cur) {
                        // No time has passed
                        TEST_ASSERT_EQUAL(false, match_expected);
                    } else if (match == prev) {
                        // Match can't occur without an overflow occurring
                        TEST_ASSERT_EQUAL(false, match_expected);
                    } else if (cur == match) {
                        // All other cases where cur == match a match should be expected
                        TEST_ASSERT_EQUAL(true, match_expected);
                    }

                    // Actual test
                    TEST_ASSERT_EQUAL(match_expected, _ticker_match_interval_passed(prev, cur, match));
                }
            }
        }
    }
}

typedef struct  {
    timestamp_t prev;
    timestamp_t cur;
    timestamp_t match;
    bool result;
} match_interval_entry_t;

/**
 * Check that _ticker_match_interval_passed correctly detects matches
 *
 * Use a table of pre-computed values to check that _ticker_match_interval_passed
 * returns the correct match value.
 */
static void test_match_interval_passed_table()
{
    static const match_interval_entry_t test_values[] = {
          /* prev,       cur,        match,      result */
            {0x00000000, 0x00000000, 0x00000000, false},
            {0x00000000, 0x00000000, 0xffffffff, false},
            {0x00000000, 0x00000000, 0x00000001, false},
            {0x00000000, 0xffffffff, 0x00000000, false},
            {0x00000000, 0x00000001, 0x00000000, false},
            {0xffffffff, 0x00000000, 0x00000000, true},
            {0x00000001, 0x00000000, 0x00000000, true},
            {0x00005555, 0x00005555, 0x00005555, false},
            {0x00005555, 0x00005555, 0x00005554, false},
            {0x00005555, 0x00005555, 0x00005556, false},
            {0x00005555, 0x00005554, 0x00005555, false},
            {0x00005555, 0x00005556, 0x00005555, false},
            {0x00005554, 0x00005555, 0x00005555, true},
            {0x00005556, 0x00005555, 0x00005555, true},
            {0xffffffff, 0xffffffff, 0xffffffff, false},
            {0xffffffff, 0xffffffff, 0xfffffffe, false},
            {0xffffffff, 0xffffffff, 0x00000000, false},
            {0xffffffff, 0xfffffffe, 0xffffffff, false},
            {0xffffffff, 0x00000000, 0xffffffff, false},
            {0xfffffffe, 0xffffffff, 0xffffffff, true},
            {0x00000000, 0xffffffff, 0xffffffff, true},
    };
    for (int i = 0; i < MBED_ARRAY_SIZE(test_values); i++) {
        const uint32_t prev = test_values[i].prev;
        const uint32_t cur = test_values[i].cur;
        const uint32_t match = test_values[i].match;
        const uint32_t result = test_values[i].result;
        TEST_ASSERT_EQUAL(result, _ticker_match_interval_passed(prev, cur, match));
    }
}

static const case_t cases[] = {
    MAKE_TEST_CASE("ticker initialization", test_ticker_initialization),
    MAKE_TEST_CASE(
        "ticker multiple initialization",  test_ticker_re_initialization
    ),
    MAKE_TEST_CASE("ticker read", test_ticker_read),
    MAKE_TEST_CASE("ticker read overflow", test_ticker_read_overflow),
    MAKE_TEST_CASE(
        "legacy insert event outside overflow range", 
        test_legacy_insert_event_outside_overflow_range
    ),
    MAKE_TEST_CASE(
        "legacy insert event in overflow range", 
        test_legacy_insert_event_in_overflow_range
    ),
    MAKE_TEST_CASE(
        "legacy insert event overflow", test_legacy_insert_event_overflow
    ),
    MAKE_TEST_CASE(
        "legacy insert event head", test_legacy_insert_event_head
    ),
    MAKE_TEST_CASE(
        "legacy insert event tail", test_legacy_insert_event_tail
    ),
    MAKE_TEST_CASE(
        "legacy insert event multiple overflow", 
        test_legacy_insert_event_multiple_overflow
    ),
    MAKE_TEST_CASE(
        "test_legacy_insert_event_multiple_random", 
        test_legacy_insert_event_multiple_random
    ),
    MAKE_TEST_CASE(
        "test_insert_event_us_outside_overflow_range", 
        test_insert_event_us_outside_overflow_range
    ),
    MAKE_TEST_CASE(
        "test_insert_event_us_in_overflow_range", 
        test_insert_event_us_in_overflow_range
    ),
    MAKE_TEST_CASE(
        "test_insert_event_us_underflow", test_insert_event_us_underflow
    ),
    MAKE_TEST_CASE("test_insert_event_us_head", test_insert_event_us_head),
    MAKE_TEST_CASE("test_insert_event_us_tail", test_insert_event_us_tail),
    MAKE_TEST_CASE(
        "test_insert_event_us_multiple_random", 
        test_insert_event_us_multiple_random
    ),
    MAKE_TEST_CASE("test_remove_event_tail", test_remove_event_tail),
    MAKE_TEST_CASE("test_remove_event_head", test_remove_event_head),
    MAKE_TEST_CASE("test_remove_event_invalid", test_remove_event_invalid),
    MAKE_TEST_CASE("test_remove_random", test_remove_random),
    MAKE_TEST_CASE("update overflow guard", test_overflow_event_update),
    MAKE_TEST_CASE(
        "update overflow guard in case of spurious interrupt", 
        test_overflow_event_update_when_spurious_interrupt
    ),
    MAKE_TEST_CASE(
        "test_irq_handler_single_event", test_irq_handler_single_event
    ),
    MAKE_TEST_CASE(
        "test_irq_handler_single_event_spurious", 
        test_irq_handler_single_event_spurious
    ),
    MAKE_TEST_CASE(
        "test_irq_handler_multiple_event_multiple_dequeue", 
        test_irq_handler_multiple_event_multiple_dequeue
    ),
    MAKE_TEST_CASE(
        "test_irq_handler_multiple_event_single_dequeue_overflow", 
        test_irq_handler_multiple_event_single_dequeue_overflow
    ),
    MAKE_TEST_CASE(
        "test_irq_handler_multiple_event_single_dequeue", 
        test_irq_handler_multiple_event_single_dequeue
    ),
    MAKE_TEST_CASE(
        "test_irq_handler_insert_immediate_in_irq", 
        test_irq_handler_insert_immediate_in_irq
    ),
    MAKE_TEST_CASE(
        "test_irq_handler_insert_non_immediate_in_irq", 
        test_irq_handler_insert_non_immediate_in_irq
    ),
    MAKE_TEST_CASE(
        "test_set_interrupt_past_time", 
        test_set_interrupt_past_time
    ),
    MAKE_TEST_CASE(
        "test_set_interrupt_past_time_with_delay", 
        test_set_interrupt_past_time_with_delay
    ),
    MAKE_TEST_CASE(
        "test_frequencies_and_masks",
        test_over_frequency_and_width<test_frequencies_and_masks>
    ),
    MAKE_TEST_CASE(
        "test_ticker_max_value",
        test_ticker_max_value
    ),
    MAKE_TEST_CASE(
        "test_match_interval_passed",
        test_match_interval_passed
    ),
    MAKE_TEST_CASE(
        "test_match_interval_passed_table",
        test_match_interval_passed_table
    )
};

static utest::v1::status_t greentea_test_setup(const size_t number_of_cases)
{
    GREENTEA_SETUP(60, "default_auto");
    return verbose_test_setup_handler(number_of_cases);
}

int main()
{
    Specification specification(greentea_test_setup, cases, greentea_test_teardown_handler);
    return !Harness::run(specification);
}
