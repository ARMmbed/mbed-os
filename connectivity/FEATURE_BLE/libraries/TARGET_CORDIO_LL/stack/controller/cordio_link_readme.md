Cordio Link Release Notes
=========================

Legal
-----

Copyright (c) 2013-2019 Arm Ltd. All Rights Reserved.
Arm Ltd. confidential and proprietary.

Compliance
----------

This release complies with the Bluetooth Core.TCRL.2018-2 definitions using the following test
specification versions:

    * LL.TS.5.1.0, Link Layer (LL) Bluetooth Test Specification
    * HCI.TS.5.1.0, Host Controller Interface (HCI) Bluetooth Test Specification

Compliance is verified with the Ellisys EBQ compliance tester using the following version:

    * EBQ 2017-2.6969
      Note : The following test cases will fail because of tester issue : Sync packet offset calculation is wrong in LL_PERIODIC_SYNC_IND PDU when offset_adjust field is 1.
             LL/CON/MAS/BV-99-C, LL/CON/MAS/BV-100-C, LL/CON/MAS/BV-101-C, LL/CON/SLA/BV-103-C, LL/CON/SLA/BV-104-C, LL/CON/SLA/BV-105-C

Compliance is verified with the Harmony compliance tester using the following version:

    * 19.1.16916.18388


Changes
-------

The following changes were made in r19.02

Enhancements in this release:

    WBUSW-2402  MEP16 Allow ADI field to be optional in the AUX_SCAN_RSP
    WBUSW-2938  Enable ADI field by default in the AUX_SCAN_RSP after Madrid spec is adopted
    WBUSW-3108  Update HCI definition and LL definition to conform Madrid spec
    WBUSW-3139  CIS master needs to send additional ack if there is still subevent
    WBUSW-3144  PAST implementation update for Madrid version r15
    WBUSW-3193  Add support of adjusting CIS offset in the LL_CIS_RSP on the CIS slave
    WBUSW-3195  Add support of multiple CIS streams phase I
    WBUSW-3203  Add op flag for CIS master to skip sending additional NULL PDU
    WBUSW-3208  LE Extended Advertising Enable returns incorrect status when no scan response data provided
    WBUSW-3227  MEP9 HCI support for debug keys in LE Secure Connections
    WBUSW-3228  Add Opflag to optionally include/exclude advertising address in AUX_ADV_IND PDU
    WBUSW-3229  Add opflag to include/exclude the ADI field in AUX_SCAN_RSP
    WBUSW-3242  Add test command LE Modify Sleep Clock Accuracy command
    WBUSW-3256  Add support of multiple CIS stream phases II
    WBUSW-3258  Add test command to change the advertising address in AUX_CONNECT_RSP
    WBUSW-3261  Update window widening calculation algorithm
    WBUSW-3268  Add support of verifying connection parameter update on the slave when the instance is the current connection event
    WBUSW-3279  Add check for HCI_LE_Periodic_Adv_Enable
    WBUSW-3281  Add check for HCI_LE_Extended_Create_Connection
    WBUSW-3292  Add support of multiple CIS streams phase III
    WBUSW-3324  Update LL_CIS_EST_EVT to spec version 18a
    WBUSW-3327  Update HCI_SET_CIG_PARAMETER to spec version 18a
    WBUSW-3331  Update LL_CIS_REQ PDU to spec version 18a
    WBUSW-3333  Add support of checking if random address is initialized for some HCI command
    WBUSW-3348  Add interleaved packing scheme for multiple CISs

Resolved defects in this release:

    WBUSW-2981  Connection fails to be established with coded phy when connection interval is smaller than 30ms
    WBUSW-3080  HCI_LE_Periodic_Advertising_Sync_Established event has wrong info in it
    WBUSW-3103  LL_PERIODIC_SYNC_IND PDU has wrong info in it
    WBUSW-3119  One variable size definition is incorrect
    WBUSW-3120  CIS slave sometime transmits at the incorrect timing
    WBUSW-3141  ACL sometimes gets lost when CIS is present
    WBUSW-3142  CIS crashes sometimes when doing uni-directional data transfer
    WBUSW-3154  Flush scheme sometimes doesn't work correctly on the slave when RX timeout happens
    WBUSW-3167  Advertiser cannot handle connection indication with connection interval of 0
    WBUSW-3177  Return incorrect error code when creating connection with invalid interval
    WBUSW-3199  Sometime CIS bod is incorrectly terminated twice on master or slave
    WBUSW-3206  Sometimes there is memory leak for the CIS master Rx operation
    WBUSW-3213  HCI_LE_Periodic_Advertising_Sync_Transfer_Received event is sent too early
    WBUSW-3216  CIS link drops when CIS bod can't be executed in time
    WBUSW-3218  Wrong address type in the LL_PERIODIC_SYNC_IND PDU when address is RPA
    WBUSW-3219  Local RPA sometimes is incorrect for the enhanced connection complete event
    WBUSW-3222  ADV_SCAN_IND is not using RPA for advertising address when local IRK is provided
    WBUSW-3235  Failed to create sync with RPA address
    WBUSW-3260  Not enough memory for PCA10040 with large periodic advertising data size
    WBUSW-3262  High duty cycle connectable directed advertising shall use fixed sequential channels for primary advertising operation
    WBUSW-3280  Wrong address type in LE_PER_ADV_SYNC_TRANS_REC_EVT when RPA address is used
    WBUSW-3291  Seeing unexpected periodic advertising sync lost event
    WBUSW-3295  HCI_Reset is not completed when periodic sync transfer is pending
    WBUSW-3304  Incomplete Periodic Advertising Report event is received
    WBUSW-3306  Assertion error when dynamically update extended advertising data
    WBUSW-3307  Tx PDU sometimes is not cleaned up correctly after it is being flushed on CIS slave
    WBUSW-3314  Failed to disable advertising for multiple advertising sets
    WBUSW-3338  Flush scheme doesn't work for the second CIS in sequential packing scheme with multiple CIS.
    WBUSW-3363  Buffer is not freed correctly sometimes when restarting advertising
    WBUSW-3369  Command complete event for scan disable is received too late in some cases
    WBUSW-3376  IUT receives incorrect status for LE_Periodic_Advertising_Sync_Transfer_Received event
    WBUSW-3418  Connection update with invalid handle returns incorrect status
    WBUSW-3420  HCI_LE_Set_PHY with invalid handle returns invalid status
    WBUSW-3423  HCI LE Set Data Length with invalid handle returns incorrect status
    WBUSW-3451  CIS slave sometimes fails to receive
    WBUSW-3479  There is unused value in the code
    WBUSW-3505  Intermittent periodic sync lost when PAST transfer happens back to back
    WBUSW-3511  Scheduling conflict between AUX ADV and periodic ADV is not handled correctly

Known limitations in this release:

    WBUSW-3183  Scanner fails to receive scan request intermittently when scan interval and scan window are same
    WBUSW-3367  Primary scan fails to receive ADV packets intermittently when 6 periodic scan is ongoing with maximum periodic data size

Incomplete features in this release:

    None


Change History
--------------

Changes in r18.11

    WBUSW-2025  Madrid - Periodic Advertising Sync Transfer
    WBUSW-2026  Madrid - Control Length Extension
    WBUSW-2401  [MFE] MEP12 Add sleep clock accuracy update machanism
    WBUSW-2816  Add functionality to pack ACAD field for TX
    WBUSW-2817  Add functionality to parse ACAD field for rx
    WBUSW-2818  Add ACAD structure for channel map
    WBUSW-2835  Reservation manager enhancement to support common interval
    WBUSW-2907  Optimize ACAD skip
    WBUSW-2925  Add new test command to set trigger tx mode and invalid CRC init at the same time
    WBUSW-2927  [MFE] MEP21 Host channel classification for secondary advertising
    WBUSW-2983  Modify BbTesterSetInvalidAccessAddress() to support changes in TSE 10530
    WBUSW-3044  Need to add timeout event for CIS LLCP procedure
    WBUSW-3054  Update HCI_SET_CIG_PARAM command
    WBUSW-3081  Add support of encryption for CIS
    WBUSW-3082  Add support of MIC failure in the CIS state machine
    WBUSW-3095  PAST commands are updated to swap connection handle and sync handle
    WBUSW-3108  Update HCI def and LL def to Madrid spec
    WBUSW-2339  [ESR11] Avoid adding duplicate address to the white list
    WBUSW-2429  Active extended scanning sometimes delivers incorrect advertising report
    WBUSW-2756  Failed to establish connection with the second device in multi-connection role
    WBUSW-2796  IUT supporting symmetric PHY responds with incorret error code when UT sends HCI LE SET PHY with asymmetric parameters
    WBUSW-2806  Incorrect error code for LE set extended advertising enable command when scannable adv pdu is set
    WBUSW-2821  Controller sometimes crashes when enabling periodic advertising
    WBUSW-2838  AUX_CHAIN_IND incorrectly includes ADI field if corresponding field in PDU pointed to this PDU is not present
    WBUSW-2839  Advertising DID is not updated correctly whenever host provides new scan response data
    WBUSW-2863  AUX_ADV_IND doesn't get updated when there is a change for the advertising data
    WBUSW-2866  TesterSetTxAuxReqPdu() command is not implemented in the LL
    WBUSW-2882  Controller fails to execute connection BOD when connection BOD collides with advertising BOD
    WBUSW-2887  No data observed in ae directed or undirected connectable advertisements
    WBUSW-2918  Extended advertiser sometimes doesn't include advertising data in the connectable advertising packet
    WBUSW-2921  Aborting slave latency doesn't work when ACL data arrives at channel map update instance
    WBUSW-2922  Periodic scanner incorrectly updates the anchor point when receiving a packet with CRC error
    WBUSW-2926  Reservation manager returns wrong offset when BB_CLK_RATE_HZ is not 1M
    WBUSW-3056  LE Set Periodic Adv Sync Transfer Param command returns incorrect status when wrong connection handle is provided
    WBUSW-3060  HCI_Periodic_Adv_Terminate_Sync command does not return error when sync handle is invalid
    WBUSW-3065  LE_PER_ADV_SYNC_TRANS_REC_EVT sometimes is not received in time
    WBUSW-3079  Periodic interval is not correct in periodic advertising sync transfer received event
    WBUSW-3085  Incorrect bits received in the channel selection algorithm event
    WBUSW-3094  Sometimes the TX power for the extended advertising is too low
    WBUSW-3105  Compiler warning when feature bit are more than 32 bits
    WBUSW-3106  Tx power on the secondary channel for the 2018 nordic board is too low
    WBUSW-3116  Corrupted Advertising reports are received when payload > 251
    WBUSW-3147  With 2M, AUX_SCAN_RSP has wrong auxPtr value
    WBUSW-3149  ADI field is sometimes 0 after setting periodic advertising data
    WBUSW-3155  Failed to create connection with some connection interval when ACL connection is on coded PHY
    WBUSW-3171  Incorrect assertion in scheduler when there is no hardware timer
    WBUSW-3175  Slave sometimes fails to remove the advertising BOD
    WBUSW-3180  Sometimes there is assertion error after cancelling the ACL connection establishment procedure

Changes in r2p4-00rel0.

    WBUSW-2340  [ESR11] LL needs to return with error code of unsupported features if the txPhys or rxPhys in the set default PHY and set PHY
    WBUSW-2341  [ESR11] LL needs to return unsupported features error code if the phys in the set extended adv parameter are not supported
    WBUSW-2343  [ESR11] Scan response data length range
    WBUSW-2344  [ESR11] Shall not allow removing adv set or clear adv sets when periodic advertising is enabled
    WBUSW-2345  [ESR11] Filter_Duplicates when Duration is zero
    WBUSW-2346  [ESR11] Return error code when there is no more resource to create periodic sync
    WBUSW-2347  [ESR11] Some extended PDU shall not include Tx power level
    WBUSW-2348  [ESR11] LL shall reject the control PDU with incorrect length with LL_UNKNOWN_RSP
    WBUSW-2394  [ESR11] LL is recommended to change advertising DID when periodic advertising is enabled on a  advertising set
    WBUSW-2395  [ESR11] Periodic scanner shall terminate create sync procedure if the periodic event is not received within the first 6 periodic interval
    WBUSW-2396  [ESR11] Add LL_FEAT_MIN_NUM_USED_CHAN to LCTR_FEAT_PEER_MASK
    WBUSW-2476  [ESR11] LE Set Random Address, not allowed when advertising or scanning is enabled
    WBUSW-2494  Advertising scheduling enhancement
    WBUSW-2507  Scan BOD scheduling enhancement
    WBUSW-2634  Add OpFlag to disable slave latency wakeup upon data pending.
    WBUSW-2696  Core paring update - Validate the public key in LL
    WBUSW-2732  Added runtime config to ignore timestamp with CRC-error packet
    WBUSW-2223  HCI resets doesn't complete sometimes
    WBUSW-2376  Connection complete event is missing when reservation manager is full
    WBUSW-2383  Adjusted min duration for the scan operation to support simultaneous scanning for multiple phy
    WBUSW-2413  Connection Interval is not a multiple of preferred periodicity
    WBUSW-2414  [UPF59] Channel selection bit is not set to reserved in the aux_conn_req
    WBUSW-2415  [UPF59] The scanA in the aux_scan_req is not the same as the targetA in the extended advertising packet
    WBUSW-2418  [UPF59] Slave is not handling the extended reject indication after connection parameter response is sent.
    WBUSW-2419  Slave abort slave latency incorrectly between the last CE with old connection parameter and the first instant with the connection parameter
    WBUSW-2431  Disconnection happens when advertising event conflicts with connection event.
    WBUSW-2435  Connection drops when ADV BOD runs over connection bod
    WBUSW-2448  Channel selection method is not correct in the LL_CONNECT_IND when extended API is used
    WBUSW-2449  Rx window opens too late for coded packets
    WBUSW-2454  Delay in receiving HCI_ADV_REPORT_EVENT for multiple adv sets.
    WBUSW-2460  Each extended advertising command is not returning "Command Disallowed" with no scan response data.
    WBUSW-2461  Initiating LL_CONN_PARAM_REQ - Unsupported Without Feature exchange
    WBUSW-2462  Initiate LL_CONN_PARAM_REQ when the feature is not supported
    WBUSW-2480  Periodic scanner uses incorrect filter policy
    WBUSW-2481  Incorrect values in HCI_LE_Data_Length_Change event
    WBUSW-2496  Sync established event with status ERR_OP_CANCELLED_BY_HOST is not received when sync is cancelled
    WBUSW-2512  HCI_LE_Set_PHY return status depends on connection established state
    WBUSW-2517  Sending LL_AUX_CHAIN_IND earlier than the time indicated by the aux pointer offset
    WBUSW-2522  order of receiving adv report event (with bit 1 and 2) and adv report event (with bit 3) is incorrect
    WBUSW-2533  Scan on multiple PHYs sometimes is not working
    WBUSW-2537  Cannot start three advertising sets with interval of 20ms.
    WBUSW-2546  Periodic Interval calculation is not correct.
    WBUSW-2547  [qual] Incorrect error code for invalid set phy command.
    WBUSW-2556  Periodic advertising interval is calculated incorrectly when min_int equals max_int.
    WBUSW-2629  Periodic scan minimum duration is too long.
    WBUSW-2636  Missing ADV_EXT_IND packets when advertising interval is small (Ex. 20 ms)
    WBUSW-2643  No data transmit when slave latency is applied during channel map update procedure
    WBUSW-2700  Extended advertising state is incorrect when restarts
    WBUSW-2705  Connectable extended adv is sending the truncated data.
    WBUSW-2707  Assertion failures when simultaneous connection and scanning state
    WBUSW-2711  Missing sync established event for the third periodic advertising set.
    WBUSW-2718  Slave sometimes allows connection from the device with un-resolvable RPA.
    WBUSW-2721  Coverity issues
    WBUSW-2741  Sometimes extended scan on multiple PHY does not work
    WBUSW-2756  Failed to establish connection with the second device in multi-connection role
    WBUSW-2779  Sometimes miss advertising report events for multi-set and mixed role

Changes in r2p3-03rel0.

    WBUSW-1820  LL frees the connect context incorrectly
    WBUSW-2113  [EBQ] ADV Data ID should be different from the previous round
    WBUSW-2126  Adjust scheduling priority for Periodic Advertising and Scanning
    WBUSW-2128  Aux scan operation uses incorrect minimum and maximum scan duration
    WBUSW-2185  [EBQ] Symmetric phys are not set correctly OTA
    WBUSW-2199  Extended ADV report sometimes contains more than 1650
    WBUSW-2216  [feat] Address is not refreshed when advertising with non-connectable events using RPA
    WBUSW-2232  LLCP with instance should honor slave latency
    WBUSW-2262  Wrong LE extended advertising report when RX error happens with AUX_CHAIN_IND
    WBUSW-2283  llValidatePhyPreferences parameter size incorrect
    WBUSW-2284  [EBQ] IUT should respond with Command Disallowed to an LE Set Random Address command when advertising or scanning is enabled
    WBUSW-2287  Return incorrect status for the extended adv set termination event when extended API is used for the legacy high duty cycle connectable adv
    WBUSW-2298  Multi-slave connection case, BOD due time is wrong when rescheduled because of conflict
    WBUSW-2299  Slave doesn't wake up to Rx when slave latency is used and event counter wraps around
    WBUSW-2311  AccessAddress should be unique in each connection for different device.
    WBUSW-2316  [Qual] AdvA in AUX_ADV_IND on coded phy should be reserved with event type 0x0000
    WBUSW-2318  [Qual] "Unused" parameter is incorrect from LE Periodic Advertising Report Event
    WBUSW-2333  doxigen fixes for LL
    WBUSW-2362  Check extended adv data length shall be done when extended adv is enabled
    WBUSW-2367  Add support of extended scan on mulitple PHYs at the same time.
    WBUSW-2370  Change the ceJitterDelay from 32us to 0us
    WBUSW-2374  [FEAT] Connection dropped while connection update with multiple testers
    WBUSW-2377  Add maximum connection event deviation to the calculation of CE window widening
    WBUSW-2383  Adjust min duration for the scan operation to improve schedule for simultaneous operation at the same time.
    WBUSW-2389  Change LL_MAX_PER_SCAN from 4 to 6 to pass qualification test

Changes in r2p3-02eac0.

    WBUSW-1626  Reading Tx power levels should return actual power used
    WBUSW-1709  AUX Offset field in AuxPtr sometimes violates MAFS (300-us)
    WBUSW-1932  LE_EXT_ADV_REPORT_EVT event sometimes contains extra AdvData
    WBUSW-1933  Establish connection may fail with status CONN_FAILED_TO_ESTABLISH (0x3e)
    WBUSW-2019  LE_[Read|Write]_RF_Path_Compensation commands should use transceiver as reference
    WBUSW-2035  Missing LE Advertising Set Terminated Event when AdvSet duration expires
    WBUSW-2046  Fail to establish connection when PHY Options are different between two devices
    WBUSW-2050  [EBQ] No LL_FEATURE_RSP when established connection as master with slave latency 5
    WBUSW-2098  [EBQ] Disconnection Complete Event is received before supervision timeout
    WBUSW-2127  Large gaps while scanning reduces opportunities to receive advertising packets
    WBUSW-2140  Data Length Exchange should limit MaxRxTime and MaxTxTime with unsupported LE Coded PHY peers
    WBUSW-2143  UPF58 vendor compatibility fixes
    WBUSW-2147  Incorrect padding when aligning uint64_t on 8-byte boundary
    WBUSW-2172  Fix the inaccuracy of the fast divisions
    WBUSW-2173  Fixed unit test errors
    WBUSW-2174  Use the fast division function when calculating the aux pointer offset
    WBUSW-2175  Coded operation cannot sustain
    WBUSW-2179  Fix ADV_BV_30 and ADV_BV_31
    WBUSW-2181  Prevent multiplication overflow issue.
    WBUSW-2186  Window Widening too small when there is unsync'ed time before a connection update
    WBUSW-2197  Double TX done IRQ from PPL
    WBUSW-2198  IUT as initiator should connect to devices using RPA when the address is resolvable
    WBUSW-2202  Collision resolution sometimes fails with simultaneous PHY updates
    WBUSW-2248  [nRF52] maxAdvReports adjusted based on RAM availability

Changes in r2p3-01eac0.

    WBUSW-1289  Pended host connection or PHY update receives no update completion if parameters unchanged
    WBUSW-1421  Slave may not complete HCI_LE_Connection_Update
    WBUSW-1534  AdvData length limited to 31 bytes for Extended Connectable Advertising
    WBUSW-1751  HCI_LE_Extended_Create_Connection with Coded PHY fails to establish with small CI
    WBUSW-1937  Scanner sometimes sends incorrect LE_Extended_Advertising_Report
    WBUSW-1953  Extended create connection sometimes connect to the incorrect peer device
    WBUSW-1955  Scanner ignores SCAN_RSP/AUX_SCAN_RSP when the AdvA does not match SCAN_REQ/AUX_SCAN_REQ
    WBUSW-1967  Extended scanner fails to filter anonymous advertising
    WBUSW-1970  PDU filtering enables local address resolution when local address is public
    WBUSW-1972  Scanner ignores SCAN_RSP and AUX_SCAN_RSP when the AdvA does not match SCAN_REQ/AUX_SCAN_REQ
    WBUSW-1996  Incorrect Peer_Address when High Duty Cycle Directed Advertising times out
    WBUSW-2041  TesterSendConnUpdInd command does not set ConnInterval
    WBUSW-2053  MediaTek interoperability with Connection Parameter Update Procedure
    WBUSW-2089  Incorrect timing for AUX_CHAIN_IND packet with Coded PHY
    WBUSW-2096  Extended advertiser sends incorrect AuxPtrOffset in AUX_SCAN_RSP
    WBUSW-2112  [EBQ] Should not send Command Disallowed when disabling an advertising set which is already disabled
    WBUSW-2125  Incorrect initialization of Channel Selection 1 with master role only

Changes in r2p3-00eac0.

    WBUSW-456   Slave with latency should transmit queued data in next CE
    WBUSW-1717  LE_Extended_Advertising_Report may not send complete periodic reports
    WBUSW-1762  Disconnect_Complete with reason connection timeout while updating PHY
    WBUSW-1835  LE_Enhanced_Connection_Complete may incorrectly report Peer_Address as 0
    WBUSW-1841  Periodic advertising report should send in task context
    WBUSW-1850  Periodic advertising sometimes starts long time later
    WBUSW-1854  Extended Advertising Set sometimes not terminated with Coded PHY
    WBUSW-1856  Scanner cannot receive anonymous non-connectable non-scannable adv packet with auxiliary packet

Changes in r2p2-00bet0.

    WBUSW-1283  Extended advertising and scanning resolves private addresses
    WBUSW-1489  BLE5 - Synchronized scanning
    WBUSW-1591  Improve device interoperability with LENGTH_REQ startup collisions
    WBUSW-1602  Do not send extended advertising reports in ISR
    WBUSW-1641  Coordinate Periodic Advertising operations with Reservation Manager
    WBUSW-1662  LE_Extended_Advertising_Report reassembles data fragments
    WBUSW-1663  SCH_CHECK_LIST_INTEGRITY option for all scheduler insertions
    WBUSW-1677  Reduce execution time of LctrRxAcl()
    WBUSW-1678  Cannot update the DID when advertising
    WBUSW-1682  Architecture-based folder organization
    WBUSW-1685  LE_Set_PHY while length procedure in progress may not complete
    WBUSW-1695  Add support of simultaneous scan and initiate using the extended API
    WBUSW-1724  UPF57 vendor compatibility fixes
    WBUSW-1750  Optimize calculation of advertising operation duration
    WBUSW-1754  Allow handling the chain packet if scan is not disabled
    WBUSW-1773  Cross-Protocol project interface for all platform components
    WBUSW-1792  Controller projects uses maximum AdvData size of 1650
    WBUSW-1844  Dedicated extended and periodic advertising data
    WBUSW-1329  LE_Read_Tx_Power handled in AE slave command handler
    WBUSW-1407  LLCP procedure does not reject already pended or active procedures
    WBUSW-1511  HCI_Reset timeouts in extended scanning state
    WBUSW-1532  Extended ADVB PDUs use incorrect RxAdd
    WBUSW-1537  HCI_Reset sometimes does not respond with command completion after disconnect
    WBUSW-1545  Extended advertising should allow legacy high duty cycle connectable direct advertising
    WBUSW-1567  LE_Extended_Advertising_Report sometimes sends duplicate events when filter is enabled
    WBUSW-1577  WSF_MIN() / WSF_MAX() multiple evaluation
    WBUSW-1599  HCI_Reset timeouts in extended initiating state
    WBUSW-1600  An unknown private device should not be filtered
    WBUSW-1601  LL should not modify Suggested Default Data Length value
    WBUSW-1623  BB should not specify an invalid BbBleDrvTxBufDesc_t::pBuf
    WBUSW-1629  bbGetTxData() use in example code may corrupt memory
    WBUSW-1630  LE_Extended_Advertising_Report truncate AdvData with 2M_PHY
    WBUSW-1631  AuxPtr::offset contains invalid value in first EXT_ADV_IND packet
    WBUSW-1634  Invalid BB state when BB driver calls callback with failure
    WBUSW-1638  HCI_LE_Extended_Create_Connection should allow initiating with 2M PHY
    WBUSW-1640  LE_Extended_Advertising_Report sometimes corrupted when receiving AUX_CHAIN_IND
    WBUSW-1645  HCI_LE_Extended_Create_Connection cannot establish connection with large Auxiliary Frame Spacing
    WBUSW-1649  Failed HCI_LE_Set_Random_Address should not modify state
    WBUSW-1650  HCI_LE_Extended_Create_Connection should be rejected if already in progress
    WBUSW-1657  Extended advertiser sometimes allows connections with non-matching AdvA
    WBUSW-1659  Address resolution sometimes fails to resolve
    WBUSW-1660  Extended connection sometimes established with wrong PHY
    WBUSW-1661  Master uses incorrect fast termination timeout with Extended Create Connection
    WBUSW-1664  Paring fails with legacy advertisers using directed advertising
    WBUSW-1665  Initialization sequence should initialize handlers before using them
    WBUSW-1666  Extended scanner does not update parameters when already in progress
    WBUSW-1668  HCI_LE_Set_Extended_Advertising_Parameters returns incorrect error code when memory is exceeded
    WBUSW-1672  LE_Extended_Advertising_Report may use incorrect Data_Length
    WBUSW-1687  HCI_LE_Set_Extended_Scan_Enable sometimes does not complete
    WBUSW-1688  Some LLCP collision cases fail to complete
    WBUSW-1689  LE_Set_Extended_Advertising_Enable does not adhere to duration limits for high duty cycle directed advertising
    WBUSW-1711  LE_Extended_Advertising_Report may use incorrect values in truncated report
    WBUSW-1716  Maximum data PDU length doesn't support 251 bytes in Coded PHY
    WBUSW-1729  HCI_Reset fails after LE_Extended_Create_Connection command is canceled
    WBUSW-1740  Periodic scanner sometimes loses sync with the periodic advertiser
    WBUSW-1745  Improve diversity with auxiliary channel selection
    WBUSW-1747  HCI_LE_Set_Extended_Advertising_Parameters uses incorrect error code with invalid Primary_Advertising_Interval
    WBUSW-1753  Coded PHY connections may disconnect
    WBUSW-1766  Failed to enable extended advertising on multiple sets
    WBUSW-1774  Connection may not establish due to PDU filtering
    WBUSW-1802  Extended advertiser sometimes incorrectly filters AUX_SCAN_REQ
    WBUSW-1803  LT does not modify AUX_CONN_REQ
    WBUSW-1805  Extended initiator cannot establish with Coded PHY
    WBUSW-1810  Periodic scanner may lose sync with the periodic advertiser

Changes in r2p1-00eac0.

    WBUSW-840   Simultaneous initiate and scan state
    WBUSW-1077  Validate static random address from host
    WBUSW-1135  Extended scanning on multiple PHYs
    WBUSW-1391  Improve compatibility when peer incorrectly responds to unknown packets during encryption procedure
    WBUSW-1408  Separate auto-start feature exchange Op Mode flag
    WBUSW-1422  Improve interoperability with masters that send initial LLCP
    WBUSW-1432  [nRF52] Driver supports BB_PHY_BLE_CODED PHY type
    WBUSW-1433  [nRF5x] Update platform to nRF5 SDK revision 13.0.0 alpha
    WBUSW-1447  Add extended advertising statistics
    WBUSW-1488  BLE5 - Periodic Advertising
    WBUSW-1490  BLE5 - Stable modulation index
    WBUSW-1519  Compute packet duration for all extended BOD operations
    WBUSW-1541  Allow ADV_DIRECT_IND high duty cycle with extended advertising API
    WBUSW-1558  UPF56 vendor compatibility fixes
    WBUSW-1561  Use runtime configuration to configure PHY features
    WBUSW-1562  Improve interoperability by defaulting length extension to minimum
    WBUSW-733   Enabling scan while scan enabled does not change filter duplicates
    WBUSW-1180  Feature exchange only valid controller to controller bits
    WBUSW-1366  [nRF52] RF Compensation does not dynamically adjust
    WBUSW-1387  [nRF5x] BbBleRfGetSupTxPower() should report power referenced at the antenna
    WBUSW-1405  Minimum advertising interval is BT version dependent
    WBUSW-1409  Connection sometimes fails with 32KHz BB clock
    WBUSW-1416  LT cannot start auxiliary channel exactly at MAFS
    WBUSW-1445  Reset may not complete after HCI_Clear_Advertising_Sets
    WBUSW-1485  Failed to receive the CONNECT_IND with the random address and extended advertising
    WBUSW-1495  IUT in extended legacy advertising mode does not respond to SCAN_REQ
    WBUSW-1502  LE_Enhanced_Connection_Complete event may contain incorrect peer address
    WBUSW-1508  [nRF52] Long range operation is sometimes unstable
    WBUSW-1509  Extended advertiser does not report legacy SCAN_REQ with Scan_Request_Notification_Enable
    WBUSW-1517  Remove lhciPackEvtHdr() redundant definition
    WBUSW-1518  Path compensation API organized with device management
    WBUSW-1538  Master may fail to establish connection with short connInterval
    WBUSW-1543  Fail to locate the correct resolving list entry when local IRK is the same for the entries
    WBUSW-1560  Example LL initialization does not set extended feature bits
    WBUSW-1563  Incorrect Adv filtering when AdvA is not resolved and white list filtering is enabled
    WBUSW-1564  Connection sometimes fails if CONNECT_IND does not match ADV_DIRECT_IND's InitA
    WBUSW-1582  RF Path Compensation example code incorrectly stores values
    WBUSW-1585  AUX Offset is incorrect between AUX_SCAN_RSP and AUX_CHAIN_IND in 2M and Coded PHY
    WBUSW-1587  Incorrect TxWindowDelay with 2M or Coded PHY
    WBUSW-1628  Extended advertiser sometimes does not respond to SCAN_REQ

Changes in r2p0-10eac0.

    WBUSW-438   BLE5 - LE Coded PHY
    WBUSW-841   Simultaneous initiate and connectable advertising
    WBUSW-1057  BLE5 - Minimum Used Channels
    WBUSW-1058  BLE5 - LE Privacy 1.2.1
    WBUSW-1103  BLE5 - White list anonymous advertisers
    WBUSW-1128  BLE5 - Extended Scanning
    WBUSW-1130  BLE5 - RF Path Compensation
    WBUSW-1147  Extended Advertising should limit maximum interval
    WBUSW-1148  Centralized reset logic
    WBUSW-1150  BLE5 - Read Transmit Power
    WBUSW-1187  Core IOP fixes - San Diego
    WBUSW-1196  Add BSP_DIR command line override option for Nordic platform builds
    WBUSW-1197  Allow Makefile command line override BAUD value for Nordic platform
    WBUSW-1199  Consistent logic for validating connection parameters
    WBUSW-1238  Extended advertising LT VS commands
    WBUSW-1273  Add maximum request length to pool statistics
    WBUSW-1292  Set local default maxTx to be the largest (251)
    WBUSW-1312  BLE5 - LE Extended Create Connection
    WBUSW-1323  BLE5 - Shanghai r10 HCI commands and events
    WBUSW-1324  Return error if random address required but not initialized
    WBUSW-1351  Nordic PCA10040 LEDs displays Tx PHY state
    WBUSW-1385  LL_OP_MODE_FLAG_ENA_LLCP_STARTUP includes slave initiated feature request
    WBUSW-1392  Do not allow mixed use of legacy and extended advertising, scan or initiate commands
    WBUSW-790   Distinguish pseudo random from crytographic random generation
    WBUSW-867   [ll-tester] does not adjust TIFS down to +/-0.5us
    WBUSW-962   Initiator with white list connects to other RPAs
    WBUSW-975   Scheduler should not allow overlapped BODs
    WBUSW-998   Indicate completed packets only for active connections
    WBUSW-1040  Validate parameters in the LL_CHANNEL_MAP_IND
    WBUSW-1044  Valid parameter for the connection update indication
    WBUSW-1117  Improve collision resolution between different LLCP
    WBUSW-1185  Disconnect when receiving unknown or invalid PDU during encryption start procedure
    WBUSW-1194  Resolve peer addresses without specifying own identity address
    WBUSW-1250  Disallow supervision timeout exactly twice connection interval
    WBUSW-1261  Version exchange procedure collision with procedure with instant
    WBUSW-1276  Initiator's RPA does not change after expected time.
    WBUSW-1286  UPF55 vendor compatibility fixes
    WBUSW-1290  Advertising data corrupted with undirected advertisement and peer IRK
    WBUSW-1303  LL will connect to peer identity address even with peer IRK
    WBUSW-1305  LL requires peer match even for undirected advertising
    WBUSW-1335  Resolve peer addresses even if peer is not in resolving list
    WBUSW-1336  Initiator will not respond to the previous local RPA associated with a peer
    WBUSW-1340  Resolve local address without transmitting local RPAs
    WBUSW-1342  Nordic BB receives with Access Address bit errors
    WBUSW-1394  [nRF52] VS_GET_SYS_STATS does not report CS watermark

Changes in r2p0-00bet0.

    WBUSW-431   BLE5 - 2Mbps LE PHY
    WBUSW-432   BLE5 - Extended Advertising (see note in Limitations)
    WBUSW-507   Remove lctrMaxConsCrc variable (use constant LCTR_MAX_CONS_CRC)
    WBUSW-754   BLE5 - LE Channel Selection #2
    WBUSW-822   Broadcaster build should not require connection sources
    WBUSW-915   LL should reject PDUs with invalid length
    WBUSW-1005  Improve the diversity of Access Address
    WBUSW-1009  Missing and incorrect documentation parameters
    WBUSW-1011  Obsolete ISR scheduler execution
    WBUSW-1013  Remove duplicate symbols between hci_defs.h and lhci_int.h
    WBUSW-1021  HCI reset should not reset scheduler
    WBUSW-1037  LL shall mask out the feature bits that should not be sent over the air
    WBUSW-1038  Align tester advertising PDU buffers
    WBUSW-1055  Avoid double decoding of PDUs
    WBUSW-1056  BLE5 - High Duty Cycle Non-Connectable Advertising
    WBUSW-1073  Move LL PDU defines to ll_defs.h
    WBUSW-1081  [ESR10] #6672 rename some PDUs
    WBUSW-1106  Suppress length exchange at startup with operation mode
    WBUSW-1125  Unaligned scan response buffer
    WBUSW-1127  Clear LL Tester transmit packet trigger on HCI reset
    WBUSW-1134  Advertising not terminated immediately with CONNECT_IND
    WBUSW-1149  Legacy and extended advertising commands are mutually exclusive
    WBUSW-1159  Legacy advertising does not allow dynamic data updates
    WBUSW-1188  Length excluded from advertising packet length calculation
    WBUSW-736   LHCI transport for multi-protocol operation
    WBUSW-959   Multi-protocol BB initialization
    WBUSW-965   Relocate BLE-specific scheduler functionality
    WBUSW-1006  Controller sources reorganization
    WBUSW-1060  Remove the redundant local feature flag and fix the script issue
    WBUSW-1061  [Coverity] Fix Coverity issue under mac-ble/sw
    WBUSW-1072  Corrections to source reorganization
    WBUSW-1115  Update documentation to ARM format
    WBUSW-1039  System and diagnostic WSF_ASSERT interface

Changes in LL_16.06.

    [#681] LL ACL test statistics
    [#762] Enter termination state after ACK is transmitted following LL_TERMINATE_IND
    [#830] Master sometimes starts CE outside Tx Window
    [#854] Reset should clear state variables
    [#859] Receive PDU buffer should limit to buffer allocation size
    [#880] Slave receiving LL_TERMINATE_IND with pending data does not terminate
    [#881] Connection that receives any packet, even with CRC failure, is established
    [#892] Scan BOD may be scheduled very far in the future
    [#893] Master connection may corrupt advertising report filter
    [#905] Calculate scan due time after TIFS cancel
    [#908] LL connection parameter events do not match "thin HCI" fields
    [#943] Return peer device address in LE_Connection_Complete event with privacy
    [#946] Improve LL test mode
    [#951] Connection initiation requires additional scheduler setup
    [#956] LL_CONNECTION_PARAM_REQ is not reject with invalid parameters
    [#961] Cannot create high density connections
    [#963] Reservation manager should avoid overlapping connection events

Changes in LL_16.03.

    [#568] Master selects connection parameters with least conflicts
    [#610] Inform BBP about TIFS setup requirement
    [#727] Handler performance metrics
    [#728] UPF53 vendor compatibility fixes
    [#730] Reject connection parameter request with REJECT_IND_EXT
    [#732] PRBS9 test packets calculated wrong
    [#734] InitA not refreshed in directed advertisements
    [#748] LL API to get connection context size
    [#755] VS command to set private key for LE SC
    [#759] Incorrect us-to-BB ticks conversion for 32k BB clock
    [#780] LE Secure Connections parameters should be little-endian
    [#787] VS event mask to disable trace events
    [#788] Maximum entries cannot be added to resolving list
    [#791] upperLimit in scan backoff algorithm changes after pairs of failures or successes
    [#831] Allow LlSetOpFlags() to set Connection Parameter operation flags
