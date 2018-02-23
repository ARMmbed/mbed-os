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

  MOCK_METHOD1(get_distribution_flags,
      const SecurityDistributionFlags_t*(entry_handle_t db_entry));
  MOCK_METHOD2(set_distribution_flags,
      void(entry_handle_t db_entry, const SecurityDistributionFlags_t& flags));
  MOCK_METHOD4(get_entry_local_keys,
      void(SecurityEntryKeysDbCb_t cb, entry_handle_t db_entry, const ediv_t &ediv, const rand_t &rand));
  MOCK_METHOD2(get_entry_local_keys,
      void(SecurityEntryKeysDbCb_t cb, entry_handle_t db_entry));
  MOCK_METHOD2(set_entry_local_ltk,
      void(entry_handle_t db_entry, const ltk_t &ltk));
  MOCK_METHOD3(set_entry_local_ediv_rand,
      void(entry_handle_t db_entry, const ediv_t &ediv, const rand_t &rand));
  MOCK_METHOD2(get_entry_peer_csrk,
      void(SecurityEntryCsrkDbCb_t cb, entry_handle_t db_entry));
  MOCK_METHOD2(get_entry_peer_keys,
      void(SecurityEntryKeysDbCb_t cb, entry_handle_t db_entry));
  MOCK_METHOD2(set_entry_peer_ltk,
      void(entry_handle_t db_entry, const ltk_t &ltk));
  MOCK_METHOD3(set_entry_peer_ediv_rand,
      void(entry_handle_t db_entry, const ediv_t &ediv, const rand_t &rand));
  MOCK_METHOD2(set_entry_peer_irk,
      void(entry_handle_t db_entry, const irk_t &irk));
  MOCK_METHOD3(set_entry_peer_bdaddr,
      void(entry_handle_t db_entry, bool address_is_public, const address_t &peer_address));
  MOCK_METHOD2(set_entry_peer_csrk,
      void(entry_handle_t db_entry, const csrk_t &csrk));
  MOCK_METHOD0(get_local_csrk,
      const csrk_t*());
  MOCK_METHOD1(set_local_csrk,
      void(const csrk_t &csrk));
  MOCK_METHOD0(get_public_key_x,
      const public_key_t&());
  MOCK_METHOD0(get_public_key_y,
      const public_key_t&());
  MOCK_METHOD2(set_public_key,
      void(const public_key_t &public_key_x, const public_key_t &public_key_y));
  MOCK_METHOD2(open_entry,
      entry_handle_t(BLEProtocol::AddressType_t peer_address_type, const address_t &peer_address));
  MOCK_METHOD1(close_entry,
      void(entry_handle_t db_entry));
  MOCK_METHOD1(remove_entry,
      void(const address_t peer_identity_address));
  MOCK_METHOD0(clear_entries,
      void());
  MOCK_METHOD2(get_whitelist,
      void(WhitelistDbCb_t cb, ::Gap::Whitelist_t *whitelist));
  MOCK_METHOD2(generate_whitelist_from_bond_table,
      void(WhitelistDbCb_t cb, ::Gap::Whitelist_t *whitelist));
  MOCK_METHOD1(set_whitelist,
      void(const ::Gap::Whitelist_t &whitelist));
  MOCK_METHOD1(add_whitelist_entry,
      void(const address_t &address));
  MOCK_METHOD1(remove_whitelist_entry,
      void(const address_t &address));
  MOCK_METHOD0(clear_whitelist,
      void());
  MOCK_METHOD0(restore,
      void());
  MOCK_METHOD0(sync,
      void());
  MOCK_METHOD1(set_restore,
      void(bool reload));
};

} // namespace ble
} // namespace pal
} // namespace vendor
} // namespace mock

#endif /* TESTS_GENERIC_SECURITYMANAGER_MOCK_MOCKPALSECURITYDB_H_ */
