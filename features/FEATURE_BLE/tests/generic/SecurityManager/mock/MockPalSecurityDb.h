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

#ifndef TESTS_GENERIC_SECURITYMANAGER_MOCK_MOCKPALSECURITYDB_H_
#define TESTS_GENERIC_SECURITYMANAGER_MOCK_MOCKPALSECURITYDB_H_

#include "gmock/gmock.h"
#include "ble/pal/SecurityDB.h"

namespace ble {
namespace pal {
namespace vendor {
namespace mock {

/*
 * Mock of ble::pal::SecurityDB
 */
class MockPalSecurityDb : public ble::pal::SecurityDb {
public:
	MockPalSecurityDb();

	virtual ~MockPalSecurityDb();

	MOCK_METHOD1(get_entry, SecurityEntry_t*(connection_handle_t));

	MOCK_METHOD1(get_entry, SecurityEntry_t*(const address_t &));

	MOCK_METHOD4(
        get_entry_local_keys,
        void(
            SecurityEntryKeysDbCb_t,
            connection_handle_t,
            const ediv_t &,
            const rand_t &
        )
    );

	MOCK_METHOD2(
        get_entry_local_keys,
        void(SecurityEntryKeysDbCb_t, connection_handle_t)
    );

	MOCK_METHOD2(
        set_entry_local_ltk,
        void(connection_handle_t, const ltk_t &)
    );

	MOCK_METHOD3(
        set_entry_local_ediv_rand,
        void(connection_handle_t, const ediv_t &, const rand_t &)
    );

	MOCK_METHOD2(
        get_entry_peer_csrk,
        void(SecurityEntryCsrkDbCb_t, connection_handle_t)
    );

	MOCK_METHOD2(
        get_entry_peer_keys,
        void(SecurityEntryKeysDbCb_t, connection_handle_t)
    );

	MOCK_METHOD2(
        set_entry_peer_ltk,
        void(connection_handle_t, const ltk_t &)
    );

	MOCK_METHOD3(
        set_entry_peer_ediv_rand,
        void(connection_handle_t, const ediv_t &, const rand_t &)
    );

	MOCK_METHOD2(
        set_entry_peer_irk,
        void(connection_handle_t, const irk_t &)
    );

	MOCK_METHOD3(
        set_entry_peer_bdaddr,
        void(connection_handle_t, bool, const address_t &)
    );

	MOCK_METHOD2(
        set_entry_peer_csrk,
        void(connection_handle_t, const csrk_t &)
    );

	MOCK_METHOD0(get_local_csrk, const csrk_t*());

	MOCK_METHOD1(set_local_csrk, void(const csrk_t &));

	MOCK_METHOD0(get_public_key_x, const public_key_t&());

    MOCK_METHOD0(get_public_key_y, const public_key_t&());

    MOCK_METHOD2(set_public_key, void(const public_key_t &, const public_key_t &));

    MOCK_METHOD0(get_peer_sc_oob_address, const address_t&());

    MOCK_METHOD0(get_peer_sc_oob_random, const oob_rand_t&());

    MOCK_METHOD0(get_peer_sc_oob_confirm, const oob_confirm_t&());

    MOCK_METHOD4(
        get_sc_oob_data,
        void(
            address_t &,
            oob_rand_t &,
            oob_confirm_t &,
            oob_rand_t &
        )
    );

    MOCK_METHOD0(get_local_sc_oob_random, const oob_rand_t&());

    MOCK_METHOD3(
        set_peer_sc_oob_data,
        void(
            const address_t &,
            const oob_rand_t &,
            const oob_confirm_t &
        )
    );

    MOCK_METHOD1(set_local_sc_oob_random, void(const oob_rand_t&));

    MOCK_METHOD4(
        connect_entry,
        SecurityEntry_t*(
            connection_handle_t,
            BLEProtocol::AddressType_t,
            const address_t &,
            const address_t &
        )
    );

    MOCK_METHOD1(disconnect_entry, void(connection_handle_t));

    MOCK_METHOD1(remove_entry, void(const address_t));

    MOCK_METHOD0(clear_entries, void());

    MOCK_METHOD2(get_whitelist, void(WhitelistDbCb_t, Gap::Whitelist_t *));

    MOCK_METHOD2(
        generate_whitelist_from_bond_table,
        void(WhitelistDbCb_t, Gap::Whitelist_t *)
    );

    MOCK_METHOD2(
        set_whitelist,
        void(WhitelistDbCb_t, const Gap::Whitelist_t &)
    );

    MOCK_METHOD1(
        add_whitelist_entry,
        void(const address_t &)
    );

    MOCK_METHOD1(
        remove_whitelist_entry,
        void(const address_t &)
    );

    MOCK_METHOD0(clear_whitelist, void());

    MOCK_METHOD0(restore, void());

    MOCK_METHOD0(sync, void());

    MOCK_METHOD1(set_restore, void(bool));
};

} // namespace ble
} // namespace pal
} // namespace vendor
} // namespace mock

#endif /* TESTS_GENERIC_SECURITYMANAGER_MOCK_MOCKPALSECURITYDB_H_ */
