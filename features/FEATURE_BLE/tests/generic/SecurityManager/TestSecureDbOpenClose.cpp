/* mbed Microcontroller Library
 * Copyright (c) 2018 ARM Limited
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

#include <algorithm>
#include <vector>
#include <array>
#include <initializer_list>
#include <tuple>
#include <cstdlib>
#include <array>

#include "gmock/gmock.h"
#include "gtest/gtest.h"

#include "ble/generic/GenericSecurityManager.h"

#include "stub/StubConnectionEventMonitor.h"
#include "mock/MockConnectionEventMonitor.h"
#include "mock/MockPalSecurityDb.h"
#include "mock/MockPalSecurityManager.h"
#include "mock/MockSecurityManagerEventHandler.h"

using ble::generic::GenericSecurityManager;
using ble::pal::vendor::mock::StubPalConnectionEventMonitor;
using ble::pal::vendor::mock::MockPalConnectionEventMonitor;
using ble::pal::vendor::mock::MockPalSecurityDb;
using ble::pal::vendor::mock::MockPalSecurityManager;
using ble::pal::vendor::mock::MockSecurityManagerEventHandler;

using ::testing::_;
using ::testing::Invoke;
using ::testing::InSequence;
using ::testing::Mock;
using ::testing::Pointee;
using ::testing::Return;
using ::testing::NiceMock;

class TestSecureDbAccessBase : public ::testing::Test {
protected:
    TestSecureDbAccessBase() :
        _pal_sm(),
        _db(),
        _connection_monitor(),
        _sm(_pal_sm, _db, _connection_monitor)
    {
    }

    virtual void SetUp() {
        ON_CALL(_db, sync()).WillByDefault(::testing::Return());
        ON_CALL(_db, restore()).WillByDefault(::testing::Return());
        ON_CALL(_db, get_local_csrk()).WillByDefault(Return(nullptr));
        ON_CALL(_db, set_local_csrk(_)).WillByDefault(Return());

        ON_CALL(_pal_sm, set_io_capability(_)).WillByDefault(Return(BLE_ERROR_NONE));
        ON_CALL(_pal_sm, set_display_passkey(_)).WillByDefault(Return(BLE_ERROR_NONE));
        ON_CALL(_pal_sm, get_secure_connections_support(_)).WillByDefault(Return(BLE_ERROR_NONE));
        ON_CALL(_pal_sm, get_random_data(_)).WillByDefault(Return(BLE_ERROR_NONE));
        ON_CALL(_pal_sm, generate_public_key()).WillByDefault(Return(BLE_ERROR_NONE));
        ON_CALL(_pal_sm, set_csrk(_)).WillByDefault(Return(BLE_ERROR_NONE));

        _sm.reset();
        _sm.init(/* enable bonding */ true);
    }


    virtual void TearDown() {
    }

    NiceMock<MockPalSecurityManager> _pal_sm;
    NiceMock<MockPalSecurityDb> _db;
    StubPalConnectionEventMonitor _connection_monitor;
    GenericSecurityManager _sm;
};

class TestSecureDbAccessOpen :
    public TestSecureDbAccessBase,
    public ::testing::WithParamInterface<
        std::tuple<BLEProtocol::AddressType_t, ble::address_t>
    > {
protected:
    TestSecureDbAccessOpen() :
        TestSecureDbAccessBase() {
    }

    virtual void SetUp() {
        TestSecureDbAccessBase::SetUp();
        std::tie(_peer_address_type, _peer_address) = GetParam();
    }

    BLEProtocol::AddressType_t _peer_address_type;
    ble::address_t _peer_address;

    // test constants
    const MockPalSecurityDb::entry_handle_t _entry_handle =
        (MockPalSecurityDb::entry_handle_t) 0xDEADBEEF;
    const connection_handle_t _connection = 42;
    const Gap::Role_t _role = Gap::PERIPHERAL;
    const BLEProtocol::AddressType_t _local_address_type =
        BLEProtocol::AddressType::PUBLIC;
    const BLEProtocol::AddressBytes_t _local_address = {
        0x11, 0x22, 0x33, 0x44, 0x55, 0x66
    };
    const Gap::ConnectionParams_t _connection_params = {
        /* min connection interval */ 0x0020,
        /* max connection interval */ 0x0020,
        /* latency */ 0x0000,
        /* supervision timeout */ 0x0100
    };
};

/*
 * Given an initialized SecurityManager
 * When a connection opening is signaled to the security manager.
 * Then the security manager open the corresponding entry in the database
 * according to the connection parameters.
 */
TEST_P(TestSecureDbAccessOpen, a_connection_opening_triggers_a_db_entry_opening) {
    EXPECT_CALL(_db, open_entry(_peer_address_type, _peer_address)).
        WillOnce(Return(_entry_handle));

    // trigger the opening
    EXPECT_NE(_connection_monitor.event_handler, nullptr);

    _connection_monitor.event_handler->on_connected(
        _connection,
        _role,
        _peer_address_type,
        _peer_address.data(),
        _local_address_type,
        _local_address,
        &_connection_params
    );
}

/*
 * Given an entry of the database opened
 * When a connection closed is signaled to the security manager.
 * Then the security manager close the corresponding entry in the database and
 * sync the database.
 */
TEST_P(TestSecureDbAccessOpen, a_connection_close_triggers_a_db_entry_opening) {
    ON_CALL(_db, open_entry(_peer_address_type, _peer_address)).
        WillByDefault(Return(_entry_handle));

    // trigger the opening
    EXPECT_NE(_connection_monitor.event_handler, nullptr);

    _connection_monitor.event_handler->on_connected(
        _connection,
        _role,
        _peer_address_type,
        _peer_address.data(),
        _local_address_type,
        _local_address,
        &_connection_params
    );

    // setup the expectation
    EXPECT_CALL(_db, close_entry(_entry_handle)).WillOnce(Return());
    EXPECT_CALL(_db, sync()).WillOnce(Return());

    // trigger connection close
    _connection_monitor.event_handler->on_disconnected(
        _connection,
        Gap::REMOTE_USER_TERMINATED_CONNECTION
    );
}

ble::address_t make_address(std::initializer_list<uint8_t> v) {
    return ble::address_t { v.begin() };
}

INSTANTIATE_TEST_CASE_P(
    TestSecureDbAccessOpen_combination,
    TestSecureDbAccessOpen,
    ::testing::Values(
        std::make_tuple(
            BLEProtocol::AddressType::PUBLIC,
            make_address({ 0x54, 0x45, 0x54, 0x45, 0x54, 0x45 })
        ),
        std::make_tuple(
            BLEProtocol::AddressType::RANDOM_STATIC,
            make_address({ 0x54, 0x45, 0x54, 0x45, 0x54, 0x45 })
        ),
        std::make_tuple(
            BLEProtocol::AddressType::RANDOM_PRIVATE_NON_RESOLVABLE,
            make_address({ 0x54, 0x45, 0x54, 0x45, 0x54, 0x45 })
        ),
        std::make_tuple(
            BLEProtocol::AddressType::RANDOM_PRIVATE_RESOLVABLE,
            make_address({ 0x54, 0x45, 0x54, 0x45, 0x54, 0x45 })
        )
    )
);

