## Nanostack MAC RF tests

This folder contains nanostack MAC RF tests for Icetea.
The test located under this folder is dependent of the application [`nanostack_mac_tester`](https://github.com/ARMmbed/mbed-os/blob/master/TEST_APPS/device/nanostack_mac_tester).
The nanostack MAC tester is disabled by default. To run the test cases with the application, either remove the preprocessor macro from main.cpp or add `-DICETEA_MAC_TESTER_ENABLED` to the `mbed test` command.

Icetea test cases are processed by passing commands through the `mbed-client-cli` command line. It is possible to manually replicate most test cases by following the instructions below.

In test cases with more than one device under test (DUT) the target device is given in the instructions as DUT1, DUT2 or DUT3.

## Test cases

### `address_read_and_write`

**Description:**

Write addresses to MAC interface, verify successful write.

**Preconditions:**

None

**Test steps:**

1.  Set 64-bit MAC address to 01:02:03:00:00:00:00:01:
    `addr --64-bit 01:02:03:00:00:00:00:01`
2.  Set 16-bit MAC address to 0xABCD:
    `addr --16-bit 0xABCD`
3.  Set MAC PAN ID to 0xCDCD:
    `mlme-set --attr 0x50 --value_bytes CD:CD --value_size 2`
4.  Read and verify 64-bit address:
    `addr`

**Expected result:**

The address that was read is the same as the one that was written.

The test exits with status `PASS` without timeouts.

### `create_and_join_PAN`

**Description:**

Create two Personal Area Networks (PAN) and verify beacon transmission.

Requires 3 devices.

**Preconditions:**

1.  All devices have 802.15.4 tranceivers and are in sufficient proximity.
2.  RF channel and adjacent channel are sufficiently clear.
    The RF channel is set in the test case method setUp(). The adjacent
    channel is automatically the next higher one, except for channel 26,
    where the adjacent channel becomes the next lower one. This is because
    26 is the highest available of the 16 channels in the 2.4-GHz band.

**Test steps:**

1.  DUT1: Set MAC beacon payload to "mac-tester":
    `mlme-set --attr 0x45 --value_ascii mac-tester --value_size 10`
2.  DUT1: Set payload length:
    `mlme-set --attr 0x46 --value_uint8 10 --value_size 1`
3.  DUT2: Set MAC beacon payload to "second-mac-tester":
    `mlme-set --attr 0x45 --value_ascii second-mac-tester --value_size 17`
4.  DUT2: Set payload length:
    `mlme-set --attr 0x46 --value_uint8 17 --value_size 1`
5.  DUT1: Start PAN coordinator in chosen channel:
    `start --pan_coordinator true --logical_channel <channel 1>`
6.  DUT2: Start PAN coordinator in adjacent channel:
    `start --pan_coordinator true --logical_channel <channel 2>`
7.  DUT3: Scan with channel mask:
    `scan --channel_mask <hex channel mask>`
    Channel mask is given as a left bitshifted channel number in hexadecimal format.
    For channels 11 and 12, the channel mask becomes 0x1800.
8.  DUT3: Search beacon data for DUT1's beacon:
    `find-beacon --data mac-tester`
9.  DUT3: Search beacon data for DUT2's beacon:
    `find-beacon --data second-mac-tester`

**Expected result:**

DUT1 and DUT2 start beacon transmissions on respective channels.

DUT3 receives and parses beacon transmissions.

The test exits with status `PASS` without timeouts.

### `ED_scan`

**Description:**

Perform Energy Detection (ED) scan to find Personal Area Networks (PAN).

The test case requires that a lot of data is sent on the RF channel. In the test case
this is done automatically at a rate that is hard to perform manually, which makes
it difficult to run the test case manually.

Requires 3 devices.

**Preconditions:**

1.  All devices have 802.15.4 tranceivers and are in sufficient proximity.
2.  RF channel is sufficiently clear.

**Test steps:**

1.  Set addresses for devices:
    DUT1: `addr --64-bit 01:02:03:00:00:00:00:01`
    DUT2: `addr --64-bit 01:02:03:00:00:00:00:02`
    DUT3: `addr --64-bit 01:02:03:00:00:00:00:03`
2.  DUT1: Start PAN coordinator:
    `start --pan_coordinator true --logical_channel <channel>`
3.  DUT2,DUT3: Start PAN beacon:
    `start --pan_coordinator false --logical_channel <channel>`
4.  (optional) Silence beacon traces:
    `silent-mode on`
5.  Start repeatedly sending data between DUT1 and DUT3:
    DUT1: `data --dst_addr 01:02:03:00:00:00:00:03 --msdu <payload> --msdu_length <payload_length> --wait_for_confirm false`
    DUT3: `data --dst_addr 01:02:03:00:00:00:00:01 --msdu <payload> --msdu_length <payload_length> --wait_for_confirm false`
    In the test case this repeats every 100ms.
6.  DUT1,DUT3: Reset MAC settings:
    `mlme-reset`
7.  DUT1: Start PAN coordinator:
    `start --pan_coordinator true --logical_channel <channel>`
8.  DUT3: Start PAN beacon:
    `start --pan_coordinator false --logical_channel <channel>`
9.  DUT2: Scan channels 11-26:
    `scan --scan_type 0 --scan_duration 7 --channel_mask 0x7fff800`
    Channel mask is given as a left bitshifted channel numbers in hexadecimal format.
10. DUT2: Do Energy Detection analysis on data:
    `analyze-ed --channel <channel> --above 100`

Test case repeats steps 5 to 10 three times.

**Expected result:**

Devices start beacon transmissions and send data on channel.

DUT2 scans channels for activity, and verifies that ED value for test channel is above 100.

The test exits with status `PASS` without timeouts.

### `send_data`

**Description:**

Send data between two devices over single RF hop.

Requires 2 devices.

**Preconditions:**

1.  All devices have 802.15.4 tranceivers and are in sufficient proximity.
2.  RF channel is sufficiently clear.

**Test steps:**

1.  DUT1: Set 64-bit MAC address to 01:02:03:00:00:00:00:01:
    `addr --64-bit 01:02:03:00:00:00:00:01`
2.  DUT2: Set 64-bit MAC address to 01:02:03:00:00:00:00:02:
    `addr --64-bit 01:02:03:00:00:00:00:02`
3.  DUT1: Start PAN coordinator:
    `start --pan_coordinator true --logical_channel <channel>`
4.  DUT2: Start PAN beacon:
    `start --pan_coordinator false --logical_channel <channel>`
5.  DUT1: Send data to DUT2:
    `data --dst_addr 01:02:03:00:00:00:00:02 --msdu_length 5 --msdu abcde`
6.  DUT1: Send data to DUT2:
    `data --dst_addr 01:02:03:00:00:00:00:01 --msdu_length 5 --msdu 12345`

**Expected result:**

Data send commands succeeds.

The test exits with status `PASS` without timeouts.

### `send_data_indirect`

**Description:**

Send data between two devices over two RF hops with one relay.

Requires 3 devices.

**Preconditions:**

1.  All devices have 802.15.4 tranceivers and are in sufficient proximity.
2.  RF channel is sufficiently clear.

**Test steps:**

1.  DUT1: Set 64-bit MAC address to 01:02:03:00:00:00:00:01:
    `addr --64-bit 01:02:03:00:00:00:00:01`
2.  DUT2: Set 64-bit MAC address to 01:02:03:00:00:00:00:02:
    `addr --64-bit 01:02:03:00:00:00:00:02`
3.  DUT3: Set 64-bit MAC address to 01:02:03:00:00:00:00:03:
    `addr --64-bit 01:02:03:00:00:00:00:03`
4.  DUT1: Start PAN coordinator:
    `start --pan_coordinator true --logical_channel <channel>`
5.  DUT2: Start PAN beacon:
    `start --pan_coordinator false --logical_channel <channel>`
6.  DUT3: Start PAN beacon:
    `start --pan_coordinator false --logical_channel <channel>`
7.  DUT2,DUT3: Set MAC RX on-while-idle off:
    `mlme-set --attr 0x52 --value_uint8 0 --value_size 1`
8.  DUT1: Add DUT2 and DUT3 as neighbours:
    `add-neigh --frame_ctr 0 --mac16 0xFFFF --mac64 01:02:03:00:00:00:00:02 --pan_id 0x1234 --index 0`
    `add-neigh --frame_ctr 0 --mac16 0xFFFF --mac64 01:02:03:00:00:00:00:03 --pan_id 0x1234 --index 1`
9.  DUT2,DUT3: Add DUT1 as neighbour:
    `add-neigh --frame_ctr 0 --mac16 0xFFFF --mac64 01:02:03:00:00:00:00:01 --pan_id 0x1234 --index 0`
10. DUT2: Configure indirect data:
    `config-status --data_ind abcde`
10. DUT3: Configure indirect data:
    `config-status --data_ind 12345`
11. DUT1,DUT2,DUT3: Mute traces (can cause timing issues):
    `silent-mode on`
12. DUT1: Send data indirectly to DUT2:
    `data --dst_addr 01:02:03:00:00:00:00:02 --msdu_length 5 --msdu abcde --indirect_tx true --wait_for_confirm false`
13. DUT2: Poll DUT1 (coordinator) for data:
    `poll --coord_address 01:02:03:00:00:00:00:01`
14. DUT1: Resend data twice:
    `data`
    `data`
15. DUT2: Poll data twice:
    `poll`
    `poll`
16. DUT2: Set expected poll return status to 0xEB (No data after poll):
    `config-status --poll 235")`
17. DUT2: Expected fail for poll command:
    `poll`
16. DUT2: Set expected poll return status to 0xEB (No data after poll):
    `config-status --poll 235")`
17. DUT1: Send data indirectly to DUT3 (other values set before are preserved):
    `data --dst_addr 01:02:03:00:00:00:00:03 --msdu 12345`
19. DUT2: Expected fail for poll command:
    `poll`
20. DUT3: Poll DUT1 (coordinator) for data:
    `poll --coord_address 01:02:03:00:00:00:00:01`

**Expected result:**

DUT2 receives data from DUT1 when polling after send.

DUT2 does not receive data from DUT1 when no data available.

DUT2 does not receive data from DUT1 when data available for DUT3.

DUT3 receives data from DUT1 when polling after send.

The test exits with status `PASS` without timeouts.

### `send_large_payloads`

**Description:**

Send large payloads between two devices.

Requires 2 devices.

**Preconditions:**

1.  All devices have 802.15.4 tranceivers and are in sufficient proximity.
2.  RF channel is sufficiently clear.

**Test steps:**

1.  DUT1: Set 64-bit MAC address to 01:02:03:00:00:00:00:01:
    `addr --64-bit 01:02:03:00:00:00:00:01`
2.  DUT2: Set 64-bit MAC address to 01:02:03:00:00:00:00:02:
    `addr --64-bit 01:02:03:00:00:00:00:02`
3.  DUT1: Start PAN coordinator:
    `start --pan_coordinator true --logical_channel <channel>`
4.  DUT2: Start PAN beacon:
    `start --pan_coordinator false --logical_channel <channel>`
5.  Create large payload:
    104 characters, headers are 2+1+2+8+8+2=23 bytes, resulting in a packet size of 127 (max):
    `large_payload = "0123456789abcdefghjiklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZZZZZZZZZ0123456789012345678901234567891234"`
6.  DUT1,DUT2: Set indirect data:
    `config-status --data_ind <large_payload>`
7.  DUT1: Send payload:
    `data --dst_addr 01:02:03:00:00:00:00:02 --msdu_length <large_payload length> --msdu <large_payload>`
8.  DUT2: wait for transmission to finish:
    `wait --timeout 500`
9.  DUT2: send data :
    `data --dst_addr 01:02:03:00:00:00:00:01 --msdu_length <large_payload length> --msdu <large_payload>`
10. DUT1: wait for transmission to finish:
    `wait --timeout 500`
11. DUT1,DUT2: Take turns waiting and transmitting:
    `data`
    `wait`

Test case repeats step 11 25 times for both devices.

**Expected result:**

Data send commands succeed.

The test exits with status `PASS` without timeouts.
