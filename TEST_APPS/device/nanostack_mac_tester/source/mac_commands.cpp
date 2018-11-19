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
#include "mac_commands.h"

//Need to disable filtering until mac_filter_api.h in mbed-os is updated
#define DISABLE_FILTERING

#define TRACE_GROUP "MAC"

#define MAN_MAC_START   "start <options> --- Starts a PAN\n"\
                        "   --pan_id <value>                PAN id in hex, default: 0x1234\n"\
                        "   --logical_channel <value>       Operated logical channel, default: 11 (0-26)\n"\
                        "   --channel_page <value>          Operated channel page, default: 0 (0-2)\n"\
                        "   --start_time <value>            Time at which to start sending beacons\n"\
                        "                                   default: 0\n"\
                        "   --beacon_order <value>          How often are beacons transmitted\n"\
                        "                                   default: 15 (0-15, 15 = never)\n"\
                        "   --super_frame_order <value>     Length of the superframe's active portion\n"\
                        "                                   if beacon_order is 15, this option is ignored\n"\
                        "                                   default: 15 (0-15)\n"\
                        "   --pan_coordinator <bool>        Is this device the PAN coordinator?\n"\
                        "                                   default: true\n"\
                        "   --battery_life_extension <bool> Disable beaconing device periodically\n"\
                        "                                   to save power? default: false\n"\
                        "   --coord_realignment <bool>      If true, coordinator realignment command\n"\
                        "                                   is sent prior to changing\n"\
                        "                                   the superframe configuration default: false\n"

#define MAN_MAC_SCAN    "scan <options> --- Perform a scan\n"\
                        "   --scan_type <value>         What type of scan to perform, 0=ED, 1=active\n"\
                        "                               2=passive, 3=orphan, default: 1\n"\
                        "   --channel_page_enum <value> default: 0 (0-10)\n"\
                        "   --channel_mask <value>      Bitmap of the channels on which to perform\n"\
                        "                               the scan, lower 27-bits are used\n"\
                        "                               bit 0 corresponds to channel 0 and so forth\n"\
                        "                               default: 0x07FFF800 (channels 11-26)\n"\
                        "   --scan_duration <value>     How long to spend in each channel,\n"\
                        "                               aBaseSuperFrameDuration * (2^n + 1) symbols\n"\
                        "                               default: 5 (0-14)\n"\
                        "   --channel_page <value>      The channel page on which to perform the scan\n"\
                        "                               default: 0 (0-31)\n"\
                        MAN_MAC_SECURITY

#define MAN_MAC_DATA    "data <options> --- Send arbitrary data\n"\
                        "   --src_addr_mode <value>     Source addressing mode, 0=no address, 1=reserved\n"\
                        "                               2=16-bit address, 3=64-bit address\n"\
                        "                               default: 3 (0-3)\n"\
                        "   --dst_addr_mode <value>     Destination addressing mode, same as above\n"\
                        "                               default: 3 (0-3)\n"\
                        "   --dst_pan_id <value>        Destination PAN id in hex\n"\
                        "                               default: 0x1234 (0x0-0xFFFF)\n"\
                        "   --dst_addr <bytes>          Destination address, default: 00:00:00:...\n"\
                        "   --msdu_length <value>       Length of the data to send, default: 0 (0-255)\n"\
                        "   --msdu <text>               Data to transmit, default: <NULL>\n"\
                        "   --msdu_handle <value>       Handle of this MSDU, default: 0 (0-255)\n"\
                        "   --tx_ack_req <bool>         Is ack required for this transmission?\n"\
                        "                               default: true\n"\
                        "   --indirect_tx <bool>        Transmit indirectly? default: false\n"\
                        "   --pending_bit <bool>        Specifies whether more fragments (higher layer)\n"\
                        "                               are to be sent, default: false\n"\
                        "   --wait_for_confirm <bool>   Should we block further commands until we have\n"\
                        "                               received a data confirmation, default: true\n"\
                        MAN_MAC_SECURITY

#define MAN_MAC_POLL    "poll <options> --- Poll the coordinator for data\n"\
                        "   --coord_addr_mode <value>   Coordinator addressing mode, 2=16-bit address\n"\
                        "                               3=64-bit address, default: 3 (2-3)\n"\
                        "   --coord_pan_id <value>      Coordinator PAN id in hex\n"\
                        "                               default: 0x1234 (0x0-0xFFFF)\n"\
                        "   --coord_address <bytes>     Coordinator address, default 00:00:00:...\n"\
                        MAN_MAC_SECURITY

#define MAN_MAC_PURGE   "purge <options> --- Remove a transmission from the queue\n"\
                        "   --msdu_handle <value>       Handle of the MSDU to be removed\n"\
                        "                               default: 0 (0-255)\n"\

#define MAN_MAC_SET     "mlme-set <options> --- Set a specified PIB attribute\n"\
                        "   --attr <value>              ID of the attribute to set in hex (0x0-0xFF)\n"\
                        "   --attr_index <0-255>        Index of the attribute, default: 0 (0-255)\n"\
                        "   --value_ascii <text>        Specify the set value as an ASCII string\n"\
                        "   --value_bytes <bytes>       Value as a string of bytes\n"\
                        "   --value_uint8/16/32 <value> Value as a uint8, uint16, or uint32\n"\
                        "                               Max value for uint32 is the max value for int32\n"\
                        "   --value_size <value>        Size of the value in bytes (0-255)\n"

#define MAN_MAC_GET     "mlme-get <options> --- Get a specified PIB attribute\n"\
                        "   --attr <value>          ID of the attribute we want to get in hex (0x0-0xFF)\n"\
                        "   --attr_index <value>    Index of the attribute, default: 0 (0-255)\n"

#define MAN_MAC_RESET   "mlme-reset --- Reset the MAC sublayer\n"\
                        "   --set_default_pib <bool>    When set to true, PIB attributes are set to\n"\
                        "                               their default values\n"\
                        "                               If set to false, PIB attributes retain\n"\
                        "                               their values, default: true\n"

#define MAN_MAC_ADDR    "addr <options> --- Configure 16/64-bit MAC addresses\n"\
                        "                   having no options will display the addresses\n"\
                        "   --16-bit <value>    16-bit MAC address in hex (0x0-0xFFFF)\n"\
                        "   --64-bit <bytes>    64-bit MAC address\n"

#define MAN_MAC_SECURITY    "   --security_level <value>    0=no security, 1=MIC32, 2=MIC64, 3=MIC128,\n"\
                            "                               4=ENC, 5=ENC+MIC32, 6=ENC+MIC64, 7=ENC+MIC128\n"\
                            "                               default: 0\n"\
                            "   --key_id_mode <value>       0=implicit, 1=default key source\n"\
                            "                               2=2-byte key source\n"\
                            "                               3=8-byte key source, default: 0 (0-3)\n"\
                            "   --key_index <value>         Which key to use, default: 0 (0-255)\n"\
                            "   --key_source <AA:BB...>     The originator of the key to be used\n"

#define MAN_MAC_KEY "key <cmd> <options> --- Configure or add key descriptors\n"\
                    "config --- Configure parameters for key descriptor\n"\
                    "   --key <bytes>                           Actual security key, 16 bytes\n"\
                    "                                           default: C0:C1:C2:...:CF\n"\
                    "   --key_id_lookup_list_entries <value>    Amount of entries in the key's lookup\n"\
                    "   --key_device_list_entries <value>       ...device...\n"\
                    "   --key_usage_list_entries <value>        and usage list, default: 2 (0-255)\n"\
                    "   --lookup_index <value>                  Which entry of the lookup list\n"\
                    "                                           are we accessing? default: 0 (0-255)\n"\
                    "   --lookup_data <bytes>                   The lookup data for this key\n"\
                    "                                           length is 9 bytes regardless of\n"\
                    "                                           the next option\n"\
                    "   --lookup_data_size <value>              How large is the lookup data? (0-1)\n"\
                    "                                           0=5 bytes, 1=9 bytes\n"\
                    "   --device_list_index <value>             Which entry of the device list\n"\
                    "                                           are we accessing, default: 0 (0-255)\n"\
                    "   --device_descriptor_handle <value>      Which entry in our neighbour table\n"\
                    "                                           do we want to use this key with\n"\
                    "                                           default: 0 (0-255)\n"\
                    "   --unique_device <bool>                  Is the device unique to the key?\n"\
                    "   --blacklisted <bool>                    Is the device blacklisted?\n"\
                    "   --usage_list_index <value>              Which entry of the usage list\n"\
                    "                                           are we accessing, default: 0 (0-255)\n"\
                    "   --frame_type <value>                    What type of frame do we want to\n"\
                    "                                           use this key with? (0-3)\n"\
                    "                                           0=beacon, 1=data, 2=ACK, 3=command\n"\
                    "   --command_frame_identifier <value>      Type of the command frame (1-9)\n"\
                    "                                           1=association request\n"\
                    "                                           2=association response\n"\
                    "                                           3=disassociation notification\n"\
                    "                                           4=data request\n"\
                    "                                           5=PAN id conflict notification\n"\
                    "                                           6=orphan notification\n"\
                    "                                           7=beacon request\n"\
                    "                                           8=coordinator realigment\n"\
                    "                                           9=GTS request\n"\
                    "add --- Add the key into the key descriptor table\n"\
                    "   --index <value>                         Index in the key table (0-255)\n"

#define MAN_MAC_ADD_NEIGHBOUR   "add-neigh <options> --- Add an entry to the neighbour table\n"\
                                "   --frame_ctr <value>     Frame counter for this neighbour\n"\
                                "   --mac16 <value>         16-bit MAC address in hex (0x0-0xFFFF)\n"\
                                "   --mac64 <bytes>         64-bit MAC address\n"\
                                "   --pan_id <value>        PAN id (0x0-0xFFFF)\n"\
                                "   --index <value>         Index in the neighbour table (0-255)\n"

#define MAN_MAC_FILTER  "filter <cmd> <options> --- Configure MAC layer filtering\n"\
                        "start --- Start a generic filter in blacklist, whitelist or fixed mode\n"\
                        "   --mode <value>              Set the filtering mode, values: allow|block|fixed\n"\
                        "       --lqi_m <value>         LQI multiplier (fixed mode only)\n"\
                        "       --dbm_m <value>         dBm multiplier (fixed mode only)\n"\
                        "add --- Add a filter by manually supplying values, or using a preset mode\n"\
                        "   --lqi_m <value>             LQI multiplier\n"\
                        "   --lqi_add <value>           Value added to the LQI\n"\
                        "   --dbm_m <value>             dBm multiplier\n"\
                        "   --dbm_add <value>           Value added to the dBm\n"\
                        "   --mode <value>              Filtering mode, values: allow|block|fixed\n"\
                        "   --short <value>             16-bit address in hex to filter (0x0-0xFFFF)\n"\
                        "   --long <bytes>              64-bit address as bytes to filter\n"\
                        "remove --- Remove a filter\n"\
                        "   --short <value>             16-bit address to remove from filter (0x0-0xFFFF)\n"\
                        "   --long <bytes>              64-bit address to remove from filter\n"\
                        "clear --- Clear all filters excluding the default one\n"\
                        "stop --- Stop filtering completely\n"\

#define MAN_MAC_CONFIG_STATUS   "config-status <options> --- Configure expected status codes\n"\
                                "                            having no options will display configured statuses\n"\
                                "                            default: 0 (MLME_SUCCESS) for all\n"\
                                "   --data_conf <value>     MCPS-DATA.confirm (0-255)\n"\
                                "   --data_ind <text>       MCPS-DATA.indication (0-255)\n"\
                                "   --get <value>           MLME-GET.confirm (0-255)\n"\
                                "   --scan <value>          MLME-SCAN.confirm (0-255)\n"\
                                "   --poll <value>          MLME-POLL.confirm (0-255)\n"\
                                "   --purge <value>         MCPS-PURGE.confirm (0-255)\n"\
                                "   --comm_status <value>   MLME-COMM-STATUS.indication (0-255)\n"\
                                "   --list                  List all statuses\n"\
                                "   --reset                 Reset all statuses to default values\n"

#define MAN_MAC_FIND_BEACON     "find-beacon <options> --- Search for a PAN in the\n"\
                                "                          results of the last scan\n"\
                                "   --data <text>   Beacon data transmitted in the beacon\n"

#define MAN_MAC_WAIT    "wait <options> --- Wait for data sent directly for a\n"\
                        "                   specified amount of milliseconds\n"\
                        "   --timeout <value>   Milliseconds to wait, default: 1000\n"

#define MAN_MAC_ED_ANALYZE  "analyze-ed <options --- Analyze the results of the last ED scan\n"\
                            "   --channel <value>   Channel to analyze (11-26)\n"\
                            "   --above <value>     Check if the ED value is above a given value\n"\
                            "   --below <value>     Check if the ED value is below a given value\n"

#define MAN_RESET   "reset <options> --- Reset MCPS and MLME structures to default values\n"

#define MAN_SILENT  "silent-mode <on/off> --- When enabled, doesn't print extended information\n"\
                    "                         of MCPS/MLME primitives\n"

mac_api_s *mac_interface = NULL;
static bool wait_for_confirm = true;
static bool silent_mode = false;
static volatile unsigned int data_count = 0;

static mlme_start_t start_req = {
    0x1234, /*PANId*/
    11,     /*LogicalChannel*/
    0,      /*ChannelPage*/
    0,      /*StartTime*/
    15,     /*BeaconOrder*/
    15,     /*SuperframeOrder*/
    true,   /*PANCoordinator*/
    false,  /*BatteryLifeExtension*/
    false,  /*CoordRealignment*/
};

static mlme_scan_t scan_req = {
    MAC_ACTIVE_SCAN,    /*ScanType*/
    {
        CHANNEL_PAGE_0, /*channel_page (enum)*/
        0x07FFF800      /*channel_mask*/
    },
    5,  /*ScanDuration*/
    0,  /*ChannelPage*/
    {
        0,  /*SecurityLevel*/
        0,  /*KeyIdMode*/
        0,  /*KeyIndex*/
        {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00}    /*Keysource*/
    }
};

static mcps_data_req_t data_req = {
    3,      /*SrcAddrMode*/
    3,      /*DstAddrMode*/
    0x1234, /*DstPANId*/
    {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00},       /*DstAddr*/
    0,      /*msduLength*/
    NULL,   /*msdu*/
    0,      /*msduHandle*/
    true,   /*TxAckReq*/
    false,  /*IndirectTx*/
    false,  /*PendingBit*/
    false,  /*SeqNumSuppressed*/
    false,  /*PanIdSuppressed*/
    {
        0,  /*SecurityLevel*/
        0,  /*KeyIdMode*/
        0,  /*KeyIndex*/
        {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00}    /*Keysource*/
    }
};

static mlme_poll_t poll_req = {
    3,      /*CoordAddrMode*/
    0x1234, /*CoordPANId*/
    {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00},       /*CoordAddress*/
    {
        0,  /*SecurityLevel*/
        0,  /*KeyIdMode*/
        0,  /*KeyIndex*/
        {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00}    /*Keysource*/
    }
};

static mcps_purge_t purge_req = {
    0   /*msduHandle*/
};

static mlme_set_t set_req = {
    (mlme_attr_t)0x39,  /*attr*/
    0,                  /*attr_index*/
    NULL,               /*value_pointer*/
    0                   /*value_size*/
};

static mlme_get_t get_req = {
    (mlme_attr_t)0x39,  /*attr*/
    0                   /*attr_index*/
};

static mlme_reset_t reset_req = {
    true    /*SetDefaultPIB*/
};

static mlme_key_id_lookup_descriptor_t lookup_descriptors[LOOKUP_DESCRIPTOR_TABLE_SIZE];
static mlme_key_device_descriptor_t device_descriptors[DEVICE_DESCRIPTOR_TABLE_SIZE];
static mlme_key_usage_descriptor_t usage_descriptors[USAGE_DESCRIPTOR_TABLE_SIZE];
static mlme_key_descriptor_entry_t key_descriptor = {
    lookup_descriptors,             /*KeyIdLookupList*/
    LOOKUP_DESCRIPTOR_TABLE_SIZE,   /*KeyIdLookupListEntries*/
    device_descriptors,             /*KeyDeviceList*/
    DEVICE_DESCRIPTOR_TABLE_SIZE,   /*KeyDeviceListEntries*/
    usage_descriptors,              /*KeyUsageList*/
    USAGE_DESCRIPTOR_TABLE_SIZE,    /*KeyUsageListEntries*/
    {0xC0, 0xC1, 0xC2, 0xC3, 0xC4, 0xC5, 0xC6, 0xC7, 0xC8, 0xC9, 0xCA, 0xCB, 0xCC, 0xCD, 0xCE, 0xCF} /*Key*/
};

struct status_list_t {
    uint8_t data_conf;
    uint8_t get_conf;
    uint8_t scan_conf;
    uint8_t poll_conf;
    uint8_t purge_conf;
    uint8_t comm_status_ind;
    uint8_t data_ind_len;
    uint8_t *data_ind;
};

static struct status_list_t expected_statuses = {
    MLME_SUCCESS,
    MLME_SUCCESS,
    MLME_SUCCESS,
    MLME_SUCCESS,
    MLME_SUCCESS,
    MLME_SUCCESS,
    0,
    NULL
};

struct beacon_list_t {
    size_t count;
    char *beacons[MLME_MAC_RES_SIZE_MAX];
    uint8_t beacon_lengths[MLME_MAC_RES_SIZE_MAX];
};

static struct beacon_list_t received_beacons = {};

struct ed_scan_result_list_t {
    uint8_t count;
    uint8_t channel[MLME_MAC_RES_SIZE_MAX];
    uint8_t ED_values[MLME_MAC_RES_SIZE_MAX];
};

static struct ed_scan_result_list_t last_ed_results;

static void print_security(const mlme_security_t *key)
{
    if (key->SecurityLevel > 0) {
        cmd_printf("Key.SecurityLevel:  %u\n", key->SecurityLevel);
        cmd_printf("Key.KeyIdMode:      %u\n", key->KeyIdMode);
        cmd_printf("Key.KeyIndex:       %hhu\n", key->KeyIndex);
        cmd_printf("Key.Keysource       %s\n", trace_array(key->Keysource, 8));
    }
}

static void print_PAN_descriptor(const mlme_pan_descriptor_t *desc)
{
    cmd_printf("PANDescriptor.CoordAddrMode:    %u\n", desc->CoordAddrMode);
    cmd_printf("PANDescriptor.CoordPANId:       0x%04X\n", desc->CoordPANId);
    cmd_printf("PANDescriptor.CoordAddress:     %s\n", trace_array(desc->CoordAddress, 8));
    cmd_printf("PANDescriptor.LogicalChannel:   %hhu\n", desc->LogicalChannel);
    cmd_printf("PANDescriptor.ChannelPage:      %hhu\n", desc->ChannelPage);
    cmd_printf("PANDescriptor.SuperframeSpec:   %02x:%02x\n", desc->SuperframeSpec[0], desc->SuperframeSpec[1]);
    cmd_printf("PANDescriptor.GTSPermit:        %s\n", desc->GTSPermit ? "true" : "false");
    cmd_printf("PANDescriptor.LinkQuality:      %hhu\n", desc->LinkQuality);
    cmd_printf("PANDescriptor.Timestamp:        %lu\n", desc->Timestamp);
    cmd_printf("PANDescriptor.SecurityFailure:  %hhu\n", desc->SecurityFailure);
    print_security(&desc->Key);
}

static int handle_security_args(int argc, char *argv[], mlme_security_t *key)
{
    char *str;
    int32_t val;

    if (cmd_parameter_int(argc, argv, "--security_level", &val)) {
        if (val >= 0 && val <= 7) {
            key->SecurityLevel = val;
        } else {
            return CMDLINE_RETCODE_INVALID_PARAMETERS;
        }
    }
    if (cmd_parameter_int(argc, argv, "--key_id_mode", &val)) {
        if (val >= 0 && val <= 3) {
            key->KeyIdMode = val;
        } else {
            return CMDLINE_RETCODE_INVALID_PARAMETERS;
        }
    }
    if (cmd_parameter_int(argc, argv, "--key_index", &val)) {
        if (val >= 0 && val <= 255) {
            key->KeyIndex = val;
        } else {
            return CMDLINE_RETCODE_INVALID_PARAMETERS;
        }
    }
    if (cmd_parameter_val(argc, argv, "--key_source", &str)) {
        if (strlen(str) == 2 * 8 + 7) {
            if (string_to_bytes(str, key->Keysource, 8) != 0) {
                return CMDLINE_RETCODE_INVALID_PARAMETERS;
            }
        } else {
            return CMDLINE_RETCODE_INVALID_PARAMETERS;
        }
    }
    return CMDLINE_RETCODE_SUCCESS;
}

static void add_beacon(const uint8_t *beacon, uint8_t len)
{
    if (received_beacons.count >= MLME_MAC_RES_SIZE_MAX) {
        tr_warn("List of received beacons is full. Discarding %s <%.*s>", trace_array(beacon, len), len, beacon);
        return;
    }
    unsigned int cur_beacon = received_beacons.count;
    received_beacons.beacon_lengths[cur_beacon] = len;
    received_beacons.beacons[cur_beacon] = (char *)ns_dyn_mem_temporary_alloc(len <= 75 ? 75 : len);
    if (len != 0) {
        std::memcpy(received_beacons.beacons[cur_beacon], beacon, len);
    }
    ++received_beacons.count;
}

static void clear_beacons(void)
{
    for (unsigned int i = 0; i < received_beacons.count; ++i) {
        ns_dyn_mem_free(received_beacons.beacons[i]);
        received_beacons.beacons[i] = NULL;
        received_beacons.beacon_lengths[i] = 0;
    }
    received_beacons.count = 0;
}

void mac_commands_init(void)
{
    cmd_add("start", mac_start_command, "Start a PAN", MAN_MAC_START);
    cmd_add("scan", mac_scan_command, "Perform a scan", MAN_MAC_SCAN);
    cmd_add("data", mac_data_command, "Send data", MAN_MAC_DATA);
    cmd_add("poll", mac_poll_command, "Poll for data", MAN_MAC_POLL);
    cmd_add("purge", mac_purge_command, "Purge data from the transmission queue", MAN_MAC_PURGE);
    cmd_add("mlme-set", mac_set_command, "Writes a given value to the PIB attribute", MAN_MAC_SET);
    cmd_add("mlme-get", mac_get_command, "Gets the value of a given PIB attribute", MAN_MAC_GET);
    cmd_add("mlme-reset", mac_reset_command, "Resets the MAC sublayer to default values", MAN_MAC_RESET);
    cmd_add("addr", mac_address_command, "Configure MAC addresses", MAN_MAC_ADDR);
    cmd_add("key", mac_key_command, "Configure or add security keys", MAN_MAC_KEY);
    cmd_add("add-neigh", mac_add_neighbour_command, "Add a device to the neighbour table", MAN_MAC_ADD_NEIGHBOUR);
    cmd_add("filter", mac_filter_command, "Add or remove filters based on MAC addresses", MAN_MAC_FILTER);
    cmd_add("config-status", mac_config_status_command, "Set expected return statuses for confirmations and indications", MAN_MAC_CONFIG_STATUS);
    cmd_add("find-beacon", mac_find_beacon_command, "Find a PAN by beacon data", MAN_MAC_FIND_BEACON);
    cmd_add("wait", mac_wait_command, "Wait for data", MAN_MAC_WAIT);
    cmd_add("analyze-ed", mac_analyze_ed_command, "Analyze the results of the last ED scan", MAN_MAC_ED_ANALYZE);
    cmd_add("reset", reset_command, "Reset MCPS/MLME structures to default values", MAN_RESET);
    cmd_add("silent-mode", silent_mode_command, "Stop printing fields of MCPS/MLME structures", MAN_SILENT);
}

void mac_data_confirm_handler(const mac_api_t *api, const mcps_data_conf_t *data)
{
    cmd_printf("MCPS-DATA.confirm\n");
    if (!silent_mode) {
        cmd_printf("msduHandle: %hhu\n", data->msduHandle);
        cmd_printf("status:     %hhu (%s)\n", data->status, mlme_status_string(data->status));
        cmd_printf("timestamp:  %lu\n", data->timestamp);
        cmd_printf("cca_retries:%hhu\n", data->cca_retries);
        cmd_printf("tx_retries: %hhu\n", data->tx_retries);
    }
    if (data->status == expected_statuses.data_conf) {
        cmd_ready(CMDLINE_RETCODE_SUCCESS);
    } else {
        cmd_printf("CMD failed, status: %hhu (%s)\n", data->status, mlme_status_string(data->status));
        cmd_ready(CMDLINE_RETCODE_FAIL);
    }
}

void mac_data_indication_handler(const mac_api_t *api, const mcps_data_ind_t *data)
{
    cmd_printf("MCPS-DATA.indication\n");
    if (!silent_mode) {
        cmd_printf("SrcAddrMode:    %u\n", data->SrcAddrMode);
        cmd_printf("SrcPANId:       0x%04X\n", data->SrcPANId);
        cmd_printf("SrcAddr:        %s\n", trace_array(data->SrcAddr, 8));
        cmd_printf("DstAddrMode:    %u\n", data->DstAddrMode);
        cmd_printf("DstPANId:       0x%04X\n", data->DstPANId);
        cmd_printf("DstAddr:        %s\n", trace_array(data->DstAddr, 8));
        cmd_printf("mpduLinkQuality:%hhu\n", data->mpduLinkQuality);
        cmd_printf("signal_dbm:     %hhi\n", data->signal_dbm);
        cmd_printf("timestamp:      %lu\n", data->timestamp);
        cmd_printf("DSN:            %hhi\n", data->DSN);
        print_security(&data->Key);
        cmd_printf("msduLength      %hu\n", data->msduLength);
        cmd_printf("msdu_ptr:       %s <%.*s>\n", trace_array(data->msdu_ptr, data->msduLength), data->msduLength, data->msdu_ptr);
    }
    if (data->msdu_ptr && expected_statuses.data_ind) {
        if (data->msduLength != expected_statuses.data_ind_len) {
            cmd_printf("Bad recv length %d != %d!\n", data->msduLength, expected_statuses.data_ind_len);
            return;
        }
        if (strncmp((const char *)data->msdu_ptr, (const char *)expected_statuses.data_ind, expected_statuses.data_ind_len) == 0) {
            ++data_count;
            cmd_printf("Data count %d\n", data_count);
        } else {
            cmd_printf("Received unexpected data!\n");
        }
    }
}

void mac_purge_confirm_handler(const mac_api_t *api, mcps_purge_conf_t *data)
{
    cmd_printf("MCPS-PURGE.confirm\n");
    if (!silent_mode) {
        cmd_printf("msduHandle: %hhu\n", data->msduHandle);
        cmd_printf("status:     %hhu (%s)\n", data->status, mlme_status_string(data->status));
    }
    if (data->status == expected_statuses.purge_conf) {
        cmd_ready(CMDLINE_RETCODE_SUCCESS);
    } else {
        cmd_printf("CMD failed, status: %hhu (%s)\n", data->status, mlme_status_string(data->status));
        cmd_ready(CMDLINE_RETCODE_FAIL);
    }
}

void mac_mlme_confirm_handler(const mac_api_t *api, mlme_primitive id, const void *data)
{
    switch (id) {
        case MLME_ASSOCIATE: {
            break;
        }
        case MLME_DISASSOCIATE: {
            break;
        }
        case MLME_GET: {
            mlme_get_conf_t *get_data = (mlme_get_conf_t *)data;
            cmd_printf("MLME-GET.confirm\n");
            if (!silent_mode) {
                cmd_printf("status:         %hhu (%s)\n", get_data->status, mlme_status_string(get_data->status));
                cmd_printf("attr:           %hhu\n", get_data->attr);
                cmd_printf("attr_index:     %hhu\n", get_data->attr_index);
                cmd_printf("value_pointer:  %s\n", trace_array((uint8_t *)get_data->value_pointer, get_data->value_size));
                cmd_printf("value_size:     %hhu\n", get_data->value_size);
            }
            if (get_data->status == expected_statuses.get_conf) {
                cmd_ready(CMDLINE_RETCODE_SUCCESS);
            } else {
                cmd_printf("CMD failed, status: %hhu (%s)\n", get_data->status, mlme_status_string(get_data->status));
                cmd_ready(CMDLINE_RETCODE_FAIL);
            }
            break;
        }
        case MLME_GTS: {
            break;
        }
        case MLME_RESET: {
            break;
        }
        case MLME_RX_ENABLE: {
            break;
        }
        case MLME_SCAN: {
            mlme_scan_conf_t *scan_data = (mlme_scan_conf_t *)data;
            cmd_printf("MLME-SCAN.confirm\n");
            if (!silent_mode) {
                cmd_printf("status:             %hhu (%s)\n", scan_data->status, mlme_status_string(scan_data->status));
                cmd_printf("ScanType:           %u\n", scan_data->ScanType);
                cmd_printf("ChannelPage:        %hhu\n", scan_data->ChannelPage);
                cmd_printf("UnscannedChannels:  0x%08lX\n", scan_data->UnscannedChannels.channel_mask[0]);
                cmd_printf("ResultListSize:     %hhu\n", scan_data->ResultListSize);
            }
            for (unsigned int i = 0; i < scan_data->ResultListSize; ++i) {
                if (scan_data->ScanType == MAC_ED_SCAN_TYPE) {
                    cmd_printf("Channel %d ED value:  %hhu\n", channel_from_mask(scan_req.ScanChannels.channel_mask[0], i), scan_data->ED_values[i]);
                    memcpy(last_ed_results.ED_values, scan_data->ED_values, scan_data->ResultListSize);
                    last_ed_results.count = scan_data->ResultListSize;
                    for (int i = 0; i < scan_data->ResultListSize; ++i) {
                        last_ed_results.channel[i] = channel_from_mask(scan_req.ScanChannels.channel_mask[0], i);
                    }
                } else if (scan_data->ScanType == MAC_PASSIVE_SCAN) {
                    print_PAN_descriptor(scan_data->PAN_values[i]);
                }
            }
            if (scan_data->status == expected_statuses.scan_conf || scan_data->status == MLME_LIMIT_REACHED) {
                cmd_ready(CMDLINE_RETCODE_SUCCESS);
            } else {
                cmd_printf("CMD failed, status: %hhu (%s)\n", scan_data->status, mlme_status_string(scan_data->status));
                cmd_ready(CMDLINE_RETCODE_FAIL);
            }
            break;
        }
        case MLME_SET: {
            break;
        }
        case MLME_START: {
            break;
        }
        case MLME_POLL: {
            mlme_poll_conf_t *poll_data = (mlme_poll_conf_t *)data;
            cmd_printf("MLME-POLL.confirm\n");
            if (!silent_mode) {
                cmd_printf("status:          %hhu (%s)\n", poll_data->status, mlme_status_string(poll_data->status));
                cmd_printf("expected status: %hhu (%s)\n", expected_statuses.poll_conf, mlme_status_string(expected_statuses.poll_conf));
                cmd_printf("data_count       %u\n", data_count);
            }
            if (expected_statuses.poll_conf == MLME_SUCCESS) {
                if (data_count == 1 && poll_data->status == MLME_SUCCESS) {
                    cmd_ready(CMDLINE_RETCODE_SUCCESS);
                } else {
                    cmd_printf("CMD failed, data_count = %u, status:%hhu\n", data_count, poll_data->status);
                    cmd_ready(CMDLINE_RETCODE_FAIL);
                }
            } else if (expected_statuses.poll_conf == poll_data->status) {
                cmd_ready(CMDLINE_RETCODE_SUCCESS);
            } else {
                cmd_printf("CMD failed, data_count = %u, status:%hhu, expected ret:%hhu\n", data_count, poll_data->status, expected_statuses.poll_conf);
                cmd_ready(CMDLINE_RETCODE_FAIL);
            }
            break;
        }
        default: {
            cmd_ready(CMDLINE_RETCODE_COMMAND_NOT_IMPLEMENTED);
            cmd_printf("CMD failed, not implemented\n");
            break;
        }
    }
}

void mac_mlme_indication_handler(const mac_api_t *api, mlme_primitive id, const void *data)
{
    switch (id) {
        case MLME_ASSOCIATE: {
            break;
        }
        case MLME_DISASSOCIATE: {
            break;
        }
        case MLME_BEACON_NOTIFY: {
            const mlme_beacon_ind_t *beacon_ind = (mlme_beacon_ind_t *)data;
            cmd_printf("MLME-BEACON-NOTIFY.indication\n");
            if (!silent_mode) {
                cmd_printf("BSN:                                %hhu\n", beacon_ind->BSN);
                print_PAN_descriptor(&beacon_ind->PANDescriptor);
                cmd_printf("PendAddrSpec.short_address_count    %u\n", beacon_ind->PendAddrSpec.short_address_count);
                cmd_printf("PendAddrSpec.extended_address_count %u\n", beacon_ind->PendAddrSpec.extended_address_count);
                cmd_printf("AddrList                            %s\n", trace_array(beacon_ind->AddrList, beacon_ind->PendAddrSpec.short_address_count * 2 +
                                                                                   beacon_ind->PendAddrSpec.extended_address_count * 8));
                cmd_printf("beacon_data_length                  %hu\n", beacon_ind->beacon_data_length);
                cmd_printf("beacon_data                         %s\n", trace_array(beacon_ind->beacon_data, beacon_ind->beacon_data_length));
            }
            add_beacon(beacon_ind->beacon_data, beacon_ind->beacon_data_length);
            break;
        }
        case MLME_GTS: {
            break;
        }
        case MLME_ORPHAN: {
            break;
        }
        case MLME_COMM_STATUS: {
            cmd_printf("MLME-COMM-STATUS.indication\n");
            const mlme_comm_status_t *comm_status_ind_data = (mlme_comm_status_t *)data;
            if (!silent_mode) {
                cmd_printf("PANId:          0x%04X\n", comm_status_ind_data->PANId);
                cmd_printf("SrcAddrMode:    %u\n", comm_status_ind_data->SrcAddrMode);
                cmd_printf("SrcAddr:        %s\n", trace_array(comm_status_ind_data->SrcAddr, 8));
                cmd_printf("DstAddrMode:    %u\n", comm_status_ind_data->DstAddrMode);
                cmd_printf("DstAddr:        %s\n", trace_array(comm_status_ind_data->DstAddr, 8));
                cmd_printf("status:         %hhu (%s)\n", comm_status_ind_data->status, mlme_status_string(comm_status_ind_data->status));
                print_security(&comm_status_ind_data->Key);
            }
            break;
        }
        case MLME_SYNC_LOSS: {
            break;
        }
        default:
            break;
    }
}

int mac_start_command(int argc, char *argv[])
{
    char *str;
    int32_t val;
    bool boolean;

    cmd_printf("MLME-START.request\n");
    if (cmd_parameter_val(argc, argv, "--pan_id", &str)) {
        uint32_t pan_id = strtoul(str, NULL, 16);
        if (pan_id <= 0xFFFF) {
            start_req.PANId = pan_id;
        } else {
            return CMDLINE_RETCODE_INVALID_PARAMETERS;
        }
    }
    if (cmd_parameter_int(argc, argv, "--logical_channel", &val)) {
        if (val >= 0 && val <= 26) {
            start_req.LogicalChannel = val;
        } else {
            return CMDLINE_RETCODE_INVALID_PARAMETERS;
        }
    }
    if (cmd_parameter_int(argc, argv, "--channel_page", &val)) {
        if (val >= 0 && val <= 2) {
            start_req.ChannelPage = val;
        } else {
            return CMDLINE_RETCODE_INVALID_PARAMETERS;
        }
    }
    if (cmd_parameter_int(argc, argv, "--start_time", &val)) {
        start_req.StartTime = val;
    }
    if (cmd_parameter_int(argc, argv, "--beacon_order", &val)) {
        if (val >= 0 && val <= 15) {
            start_req.BeaconOrder = val;
        } else {
            return CMDLINE_RETCODE_INVALID_PARAMETERS;
        }
    }
    if (cmd_parameter_int(argc, argv, "--super_frame_order", &val)) {
        if (val >= 0 && val <= 15) {
            start_req.SuperframeOrder = val;
        } else {
            return CMDLINE_RETCODE_INVALID_PARAMETERS;
        }
    }
    if (cmd_parameter_bool(argc, argv, "--pan_coordinator", &boolean)) {
        start_req.PANCoordinator = boolean;
    }
    if (cmd_parameter_bool(argc, argv, "--battery_life_extension", &boolean)) {
        start_req.BatteryLifeExtension = boolean;
    }
    if (cmd_parameter_bool(argc, argv, "--coord_realignment", &boolean)) {
        start_req.CoordRealignment = boolean;
    }
    mac_interface->mlme_req(mac_interface, MLME_START, &start_req);
    return CMDLINE_RETCODE_SUCCESS;
}

int mac_scan_command(int argc, char *argv[])
{
    char *str;
    int32_t val;

    cmd_printf("MLME-SCAN.request\n");
    if (cmd_parameter_int(argc, argv, "--scan_type", &val)) {
        if (val >= 0 && val <= 3) {
            scan_req.ScanType = (mac_scan_type_t)val;
        } else {
            return CMDLINE_RETCODE_INVALID_PARAMETERS;
        }
    }
    if (cmd_parameter_int(argc, argv, "--channel_page_enum", &val)) {
        if (val >= 0 && val <= 10) {
            scan_req.ScanChannels.channel_page = (channel_page_e)val;
        } else {
            return CMDLINE_RETCODE_INVALID_PARAMETERS;
        }
    }
    if (cmd_parameter_val(argc, argv, "--channel_mask", &str)) {
        scan_req.ScanChannels.channel_mask[0] = strtoul(str, NULL, 16);
    }
    if (cmd_parameter_int(argc, argv, "--scan_duration", &val)) {
        if (val >= 0 && val <= 14) {
            scan_req.ScanDuration = val;
        } else {
            return CMDLINE_RETCODE_INVALID_PARAMETERS;
        }
    }
    if (cmd_parameter_int(argc, argv, "--channel_page", &val)) {
        if (val >= 0 && val <= 2) {
            scan_req.ChannelPage = val;
        } else {
            return CMDLINE_RETCODE_INVALID_PARAMETERS;
        }
    }
    if (handle_security_args(argc, argv, &scan_req.Key) != CMDLINE_RETCODE_SUCCESS) {
        return CMDLINE_RETCODE_INVALID_PARAMETERS;
    }
    clear_beacons();
    mac_interface->mlme_req(mac_interface, MLME_SCAN, &scan_req);
    return CMDLINE_RETCODE_EXCUTING_CONTINUE;
}

int mac_data_command(int argc, char *argv[])
{
    char *str;
    int32_t val;
    bool boolean;

    cmd_printf("MCPS-DATA.request\n");
    if (cmd_parameter_int(argc, argv, "--src_addr_mode", &val)) {
        if (val == 0 || val == 2 || val == 3) {
            data_req.SrcAddrMode = val;
        } else {
            return CMDLINE_RETCODE_INVALID_PARAMETERS;
        }
    }
    if (cmd_parameter_int(argc, argv, "--dst_addr_mode", &val)) {
        if (val == 0 || val == 2 || val == 3) {
            data_req.DstAddrMode = val;
        } else {
            return CMDLINE_RETCODE_INVALID_PARAMETERS;
        }
    }
    if (cmd_parameter_val(argc, argv, "--dst_pan_id", &str)) {
        uint32_t pan_id = strtoul(str, NULL, 16);
        if (pan_id <= 0xFFFF) {
            data_req.DstPANId = pan_id;
        } else {
            return CMDLINE_RETCODE_INVALID_PARAMETERS;
        }
    }
    if (cmd_parameter_val(argc, argv, "--dst_addr", &str)) {
        int len = (data_req.DstAddrMode == 2 ? 2 : 8);
        if (string_to_bytes(str, data_req.DstAddr, len) != 0) {
            return CMDLINE_RETCODE_INVALID_PARAMETERS;
        }
    }
    if (cmd_parameter_int(argc, argv, "--msdu_length", &val)) {
        data_req.msduLength = val;
    }
    if (cmd_parameter_val(argc, argv, "--msdu", &str)) {
        ns_dyn_mem_free(data_req.msdu);
        if (strlen(str) != data_req.msduLength) {
            return CMDLINE_RETCODE_INVALID_PARAMETERS;
        }
        data_req.msdu = (uint8_t *)ns_dyn_mem_temporary_alloc(data_req.msduLength);
        if (data_req.msdu == NULL) {
            tr_error("Failed to allocate memory for the msdu");
            return CMDLINE_RETCODE_FAIL;
        }
        std::memcpy(data_req.msdu, str, data_req.msduLength);
    }
    if (cmd_parameter_int(argc, argv, "--msdu_handle", &val)) {
        if (val >= 0 && val <= 255) {
            data_req.msduHandle = val;
        }
    }
    if (cmd_parameter_bool(argc, argv, "--tx_ack_req", &boolean)) {
        data_req.TxAckReq = boolean;
    }
    if (cmd_parameter_bool(argc, argv, "--indirect_tx", &boolean)) {
        data_req.InDirectTx = boolean;
    }
    if (cmd_parameter_bool(argc, argv, "--pending_bit", &boolean)) {
        data_req.PendingBit = boolean;
    }
    if (cmd_parameter_bool(argc, argv, "--wait_for_confirm", &boolean)) {
        wait_for_confirm = boolean;
    }
    if (handle_security_args(argc, argv, &data_req.Key) != CMDLINE_RETCODE_SUCCESS) {
        return CMDLINE_RETCODE_INVALID_PARAMETERS;
    }
    mac_interface->mcps_data_req(mac_interface, &data_req);
    if (wait_for_confirm) {
        return CMDLINE_RETCODE_EXCUTING_CONTINUE;
    }
    return CMDLINE_RETCODE_SUCCESS;
}

int mac_poll_command(int argc, char *argv[])
{
    char *str;
    int32_t val;

    cmd_printf("MLME-POLL.request\n");
    data_count = 0;
    if (cmd_parameter_int(argc, argv, "--coord_addr_mode", &val)) {
        if (val == 2 || val == 3) {
            poll_req.CoordAddrMode = val;
        } else {
            return CMDLINE_RETCODE_INVALID_PARAMETERS;
        }
    }
    if (cmd_parameter_val(argc, argv, "--coord_pan_id", &str)) {
        unsigned long pan_id = strtoul(str, NULL, 16);
        if (pan_id <= 0xFFFF) {
            poll_req.CoordPANId = pan_id;
        } else {
            return CMDLINE_RETCODE_INVALID_PARAMETERS;
        }
    }
    if (cmd_parameter_val(argc, argv, "--coord_address", &str)) {
        int len = (poll_req.CoordAddrMode == 2 ? 2 : 8);
        if (string_to_bytes(str, poll_req.CoordAddress, len) != 0) {
            return CMDLINE_RETCODE_INVALID_PARAMETERS;
        }
    }
    if (handle_security_args(argc, argv, &poll_req.Key) != CMDLINE_RETCODE_SUCCESS) {
        return CMDLINE_RETCODE_INVALID_PARAMETERS;
    }
    mac_interface->mlme_req(mac_interface, MLME_POLL, &poll_req);
    return CMDLINE_RETCODE_EXCUTING_CONTINUE;
}

int mac_purge_command(int argc, char *argv[])
{
    int32_t val;

    cmd_printf("MCPS-PURGE.request\n");
    if (cmd_parameter_int(argc, argv, "--msdu_handle", &val)) {
        if (val >= 0 && val <= 255) {
            purge_req.msduHandle = val;
        } else {
            return CMDLINE_RETCODE_INVALID_PARAMETERS;
        }
    }
    mac_interface->mcps_purge_req(mac_interface, &purge_req);
    return CMDLINE_RETCODE_EXCUTING_CONTINUE;
}

int mac_set_command(int argc, char *argv[])
{
    char *str;
    int32_t val;
    uint8_t val_uint8 = 0;
    uint16_t val_uint16 = 0;
    uint32_t val_uint32 = 0;
    uint8_t *val_ptr_array = NULL;

    cmd_printf("MLME-SET.request\n");
    if (cmd_parameter_val(argc, argv, "--attr", &str)) {
        uint32_t attribute = strtoul(str, NULL, 16);
        if (attribute <= 255) {
            set_req.attr = (mlme_attr_t)attribute;
        } else {
            return CMDLINE_RETCODE_INVALID_PARAMETERS;
        }
    }
    if (cmd_parameter_int(argc, argv, "--attr_index", &val)) {
        if (val >= 0 && val <= 255) {
            set_req.attr_index = val;
        } else {
            return CMDLINE_RETCODE_INVALID_PARAMETERS;
        }
    }
    if (cmd_parameter_val(argc, argv, "--value_ascii", &str)) {
        val_ptr_array = (uint8_t *)ns_dyn_mem_temporary_alloc(strlen(str));
        if (val_ptr_array == NULL) {
            tr_error("Failed to allocate memory for MLME-SET.request");
            return CMDLINE_RETCODE_FAIL;
        }
        std::memcpy(val_ptr_array, str, strlen(str));
        set_req.value_pointer = val_ptr_array;
    } else if (cmd_parameter_val(argc, argv, "--value_bytes", &str)) {
        size_t bytes = (strlen(str) + 1) / 3;
        val_ptr_array = (uint8_t *)ns_dyn_mem_temporary_alloc(bytes);
        if (val_ptr_array == NULL) {
            tr_error("Failed to allocate memory for MLME-SET.request");
            return CMDLINE_RETCODE_FAIL;
        }
        if (string_to_bytes(str, val_ptr_array, bytes) != 0) {
            ns_dyn_mem_free(val_ptr_array);
            return CMDLINE_RETCODE_INVALID_PARAMETERS;
        }
        set_req.value_pointer = val_ptr_array;
    } else if (cmd_parameter_int(argc, argv, "--value_uint8", &val)) {
        if (val >= 0 && val <= 0xFF) {
            val_uint8 = val;
            set_req.value_pointer = &val_uint8;
        } else {
            return CMDLINE_RETCODE_INVALID_PARAMETERS;
        }
    } else if (cmd_parameter_int(argc, argv, "--value_uint16", &val)) {
        if (val >= 0 && val <= 0xFFFF) {
            val_uint16 = val;
            set_req.value_pointer = &val_uint16;
        } else {
            return CMDLINE_RETCODE_INVALID_PARAMETERS;
        }
    } else if (cmd_parameter_int(argc, argv, "--value_uint32", &val)) {
        if (val >= 0 && val <= 0x7FFFFFFF) {
            val_uint32 = val;
            set_req.value_pointer = &val_uint32;
        } else {
            return CMDLINE_RETCODE_INVALID_PARAMETERS;
        }
    }
    if (cmd_parameter_int(argc, argv, "--value_size", &val)) {
        if (val >= 0 && val <= 255) {
            set_req.value_size = val;
        }
    }

    mac_interface->mlme_req(mac_interface, MLME_SET, &set_req);
    ns_dyn_mem_free(val_ptr_array);
    set_req.value_pointer = NULL;
    return CMDLINE_RETCODE_SUCCESS;
}

int mac_get_command(int argc, char *argv[])
{
    char *str;
    int32_t val;

    cmd_printf("MLME-GET.request\n");
    if (cmd_parameter_val(argc, argv, "--attr", &str)) {
        uint32_t attribute = strtoul(str, NULL, 16);
        if (attribute <= 255) {
            get_req.attr = (mlme_attr_t)attribute;
        } else {
            return CMDLINE_RETCODE_INVALID_PARAMETERS;
        }
    }
    if (cmd_parameter_int(argc, argv, "--attr_index", &val)) {
        if (val >= 0 && val <= 255) {
            get_req.attr_index = val;
        } else {
            return CMDLINE_RETCODE_INVALID_PARAMETERS;
        }
    }
    mac_interface->mlme_req(mac_interface, MLME_GET, &get_req);
    return CMDLINE_RETCODE_SUCCESS;
}

int mac_reset_command(int argc, char *argv[])
{
    bool boolean;

    cmd_printf("MLME-RESET.request\n");
    if (cmd_parameter_bool(argc, argv, "--set_default_pib", &boolean)) {
        reset_req.SetDefaultPIB = boolean;
    }
    mac_interface->mlme_req(mac_interface, MLME_RESET, &reset_req);
    return CMDLINE_RETCODE_SUCCESS;
}

int mac_address_command(int argc, char *argv[])
{
    char *str;
    uint8_t ext_addr[8];

    if (cmd_parameter_val(argc, argv, "--64-bit", &str)) {
        if (string_to_bytes(str, ext_addr, 8) != 0) {
            return CMDLINE_RETCODE_INVALID_PARAMETERS;
        }
        mac_interface->mac64_set(mac_interface, ext_addr);
        cmd_printf("64-bit MAC address set to: %s\n", trace_array(ext_addr, 8));
    } else if (cmd_parameter_val(argc, argv, "--16-bit", &str)) {
        uint32_t short_addr_32 = strtoul(str, NULL,  16);
        if (short_addr_32 <= 0xFFFF) {
            uint16_t short_addr = short_addr_32;
            mlme_set_t set_req;
            set_req.attr = macShortAddress;
            set_req.value_pointer = &short_addr;
            set_req.value_size = 2;
            mac_interface->mlme_req(mac_interface, MLME_SET, &set_req);
            cmd_printf("16-bit MAC address set to: 0x%04X\n", short_addr);
        } else {
            tr_warn("Invalid 16-bit MAC address given: %lu", short_addr_32);
        }
    } else if (argc == 1) {
        if (mac_interface->mac64_get(mac_interface, MAC_EXTENDED_READ_ONLY, ext_addr) == 0) {
            cmd_printf("EUI64: %s\n", trace_array(ext_addr, 8));
        } else {
            tr_warn("Failed to read EUI64");
            return CMDLINE_RETCODE_FAIL;
        }
        if (mac_interface->mac64_get(mac_interface, MAC_EXTENDED_DYNAMIC, ext_addr) == 0) {
            cmd_printf("MAC64: %s\n", trace_array(ext_addr, 8));
        } else {
            tr_warn("Failed to read MAC64");
            return CMDLINE_RETCODE_FAIL;
        }
    } else {
        return CMDLINE_RETCODE_INVALID_PARAMETERS;
    }
    return CMDLINE_RETCODE_SUCCESS;
}

static int key_config_command(int argc, char *argv[])
{
    char *str;
    int32_t val;
    bool boolean;
    int lookup_index = 0, device_index = 0, usage_index = 0;

    if (cmd_parameter_val(argc, argv, "--key", &str)) {
        if (strlen(str) == 2 * 16 + 15) {
            if (string_to_bytes(str, key_descriptor.Key, 16) != 0) {
                return CMDLINE_RETCODE_INVALID_PARAMETERS;
            }
        } else {
            return CMDLINE_RETCODE_INVALID_PARAMETERS;
        }
    }
    if (cmd_parameter_int(argc, argv, "--key_id_lookup_list_entries", &val)) {
        if (val >= 0 && val < LOOKUP_DESCRIPTOR_TABLE_SIZE) {
            key_descriptor.KeyIdLookupListEntries = val;
        } else {
            return CMDLINE_RETCODE_INVALID_PARAMETERS;
        }
    }
    if (cmd_parameter_int(argc, argv, "--key_device_list_entries", &val)) {
        if (val >= 0 && val < DEVICE_DESCRIPTOR_TABLE_SIZE) {
            key_descriptor.KeyDeviceListEntries = val;
        } else {
            return CMDLINE_RETCODE_INVALID_PARAMETERS;
        }
    }
    if (cmd_parameter_int(argc, argv, "--key_usage_list_entries", &val)) {
        if (val >= 0 && val < USAGE_DESCRIPTOR_TABLE_SIZE) {
            key_descriptor.KeyUsageListEntries = val;
        } else {
            return CMDLINE_RETCODE_INVALID_PARAMETERS;
        }
    }
    if (cmd_parameter_int(argc, argv, "--lookup_index", &val)) {
        if (val >= 0 && val < LOOKUP_DESCRIPTOR_TABLE_SIZE) {
            lookup_index = val;
        } else {
            return CMDLINE_RETCODE_INVALID_PARAMETERS;
        }
    }
    if (cmd_parameter_val(argc, argv, "--lookup_data", &str)) {
        if (strlen(str) == 2 * 9 + 8) {
            if (string_to_bytes(str, key_descriptor.KeyIdLookupList[lookup_index].LookupData, 9) != 0) {
                return CMDLINE_RETCODE_INVALID_PARAMETERS;
            }
        } else {
            return CMDLINE_RETCODE_INVALID_PARAMETERS;
        }
    }
    if (cmd_parameter_int(argc, argv, "--lookup_data_size", &val)) {
        if (val == 0 || val == 1) {
            key_descriptor.KeyIdLookupList[lookup_index].LookupDataSize = val;
        } else {
            return CMDLINE_RETCODE_INVALID_PARAMETERS;
        }
    }
    if (cmd_parameter_int(argc, argv, "--device_list_index", &val)) {
        if (val >= 0 && val < DEVICE_DESCRIPTOR_TABLE_SIZE) {
            device_index = val;
        } else {
            return CMDLINE_RETCODE_INVALID_PARAMETERS;
        }
    }
    if (cmd_parameter_int(argc, argv, "--device_descriptor_handle", &val)) {
        if (val >= 0 && val <= 255) {
            key_descriptor.KeyDeviceList[device_index].DeviceDescriptorHandle = val;
        } else {
            return CMDLINE_RETCODE_INVALID_PARAMETERS;
        }
    }
    if (cmd_parameter_bool(argc, argv, "--unique_device", &boolean)) {
        key_descriptor.KeyDeviceList[device_index].UniqueDevice = boolean;
    }
    if (cmd_parameter_bool(argc, argv, "--blacklisted", &boolean)) {
        key_descriptor.KeyDeviceList[device_index].Blacklisted = boolean;
    }
    if (cmd_parameter_int(argc, argv, "--usage_index", &val)) {
        if (val >= 0 && val <= USAGE_DESCRIPTOR_TABLE_SIZE) {
            usage_index = val;
        } else {
            return CMDLINE_RETCODE_INVALID_PARAMETERS;
        }
    }
    if (cmd_parameter_int(argc, argv, "--frame_type", &val)) {
        if (val >= 0 && val <= 3) {
            key_descriptor.KeyUsageList[usage_index].FrameType = val;
        } else {
            return CMDLINE_RETCODE_INVALID_PARAMETERS;
        }
    }
    if (key_descriptor.KeyUsageList[usage_index].FrameType == 3) {
        if (cmd_parameter_int(argc, argv, "--command_frame_identifier", &val)) {
            if (val >= 1 && val <= 9) {
                key_descriptor.KeyUsageList[usage_index].CommandFrameIdentifier = val;
            } else {
                return CMDLINE_RETCODE_INVALID_PARAMETERS;
            }
        }
    }
    return CMDLINE_RETCODE_SUCCESS;
}

static int key_add_command(int argc, char *argv[])
{
    mlme_set_t set_req;
    int32_t val;
    int key_index = 0;

    if (cmd_parameter_int(argc, argv, "--index", &val)) {
        if (val >= 0 && val <= 255) {
            key_index = val;
        } else {
            return CMDLINE_RETCODE_INVALID_PARAMETERS;
        }
    }

    set_req.attr = macKeyTable;
    set_req.attr_index = key_index;
    set_req.value_pointer = &key_descriptor;
    set_req.value_size = sizeof(mlme_key_descriptor_entry_t);
    mac_interface->mlme_req(mac_interface, MLME_SET, &set_req);
    return CMDLINE_RETCODE_SUCCESS;
}

int mac_key_command(int argc, char *argv[])
{
    char *cmd = argv[1];

    if (strcmp(cmd, "config") == 0) {
        return key_config_command(argc, argv);
    } else if (strcmp(cmd, "add") == 0) {
        return key_add_command(argc, argv);
    }
    return CMDLINE_RETCODE_INVALID_PARAMETERS;
}

int mac_add_neighbour_command(int argc, char *argv[])
{
    char *str;
    int32_t val;
    mlme_device_descriptor_t neighbour;
    mlme_set_t set_req;

    neighbour.Exempt = false;
    if (cmd_parameter_int(argc, argv, "--frame_ctr", &val)) {
        neighbour.FrameCounter = val;
    }
    if (cmd_parameter_val(argc, argv, "--mac16", &str)) {
        uint32_t short_addr = strtoul(str, NULL, 16);
        if (short_addr <= 0xFFFF) {
            neighbour.ShortAddress = short_addr;
        } else {
            return CMDLINE_RETCODE_INVALID_PARAMETERS;
        }
    }
    if (cmd_parameter_val(argc, argv, "--mac64", &str)) {
        if (strlen(str) == 2 * 8 + 7) {
            if (string_to_bytes(str, neighbour.ExtAddress, 8) != 0) {
                return CMDLINE_RETCODE_INVALID_PARAMETERS;
            }
        } else {
            return CMDLINE_RETCODE_INVALID_PARAMETERS;
        }
    }
    if (cmd_parameter_val(argc, argv, "--pan_id", &str)) {
        uint32_t pan_id = strtoul(str, NULL, 16);
        if (pan_id <= 0xFFFF) {
            neighbour.PANId = val;
        } else {
            return CMDLINE_RETCODE_INVALID_PARAMETERS;
        }
    }
    if (cmd_parameter_int(argc, argv, "--index", &val)) {
        if (val >= 0 && val <= 255) {
            set_req.attr_index = val;
        } else {
            return CMDLINE_RETCODE_INVALID_PARAMETERS;
        }
    }

    set_req.attr = macDeviceTable;
    set_req.value_pointer = &neighbour;
    set_req.value_size = sizeof(mlme_device_descriptor_t);
    mac_interface->mlme_req(mac_interface, MLME_SET, &set_req);
    return CMDLINE_RETCODE_SUCCESS;
}

#ifndef DISABLE_FILTERING
static int filter_start(int argc, char *argv[])
{
    char *str;

    if (cmd_parameter_val(argc, argv, "--mode", &str)) {
        if (strcmp(str, "allow") == 0) {
            return mac_filter_start(mac_interface->parent_id, MAC_FILTER_ALLOWED) < 0 ? CMDLINE_RETCODE_FAIL : CMDLINE_RETCODE_SUCCESS;
        } else if (strcmp(str, "block") == 0) {
            return mac_filter_start(mac_interface->parent_id, MAC_FILTER_BLOCKED) < 0 ? CMDLINE_RETCODE_FAIL : CMDLINE_RETCODE_SUCCESS;;
        } else if (strcmp(str, "fixed") == 0) {
            int32_t lqi, dbm;
            if (cmd_parameter_int(argc, argv, "--lqi_m", &lqi) &&
                    cmd_parameter_int(argc, argv, "--dbm_m", &dbm)) {
                return mac_filter_start(mac_interface->parent_id, MAC_FILTER_FIXED(lqi, dbm)) < 0 ? CMDLINE_RETCODE_FAIL : CMDLINE_RETCODE_SUCCESS;
            }
        }
    }
    return CMDLINE_RETCODE_INVALID_PARAMETERS;
}

static int filter_add(int argc, char *argv[])
{
    char *str;
    uint32_t short_addr;
    uint8_t long_addr[8];
    int32_t lqi_m, lqi_add, dbm_m, dbm_add;

    if (cmd_parameter_int(argc, argv, "--lqi_m", &lqi_m) &&
            cmd_parameter_int(argc, argv, "--lqi_add", &lqi_add) &&
            cmd_parameter_int(argc, argv, "--dbm_m", &dbm_m) &&
            cmd_parameter_int(argc, argv, "--dbm_add", &dbm_add)) {
    } else if (cmd_parameter_val(argc, argv, "--mode", &str)) {
        if (strcmp(str, "allow")) {
            lqi_m = dbm_m = 256;
            lqi_add = dbm_add = 0;
        } else if (strcmp(str, "block")) {
            lqi_m = lqi_add = dbm_m = dbm_add = 0;
        } else if (strcmp(str, "fixed")) {
            lqi_add = dbm_add = 0;
            if (cmd_parameter_int(argc, argv, "--lqi_m", &lqi_m) &&
                    cmd_parameter_int(argc, argv, "--dbm_m", &dbm_m)) {
            } else {
                return CMDLINE_RETCODE_INVALID_PARAMETERS;
            }
        } else {
            return CMDLINE_RETCODE_INVALID_PARAMETERS;
        }
    } else {
        return CMDLINE_RETCODE_INVALID_PARAMETERS;
    }

    if (cmd_parameter_val(argc, argv, "--short", &str)) {
        short_addr = strtoul(str, NULL, 16);
        if (short_addr <= 0xFFFF) {
            return mac_filter_add_short(mac_interface->parent_id, short_addr, lqi_m, lqi_add, dbm_m, dbm_add) < 0 ? CMDLINE_RETCODE_FAIL : CMDLINE_RETCODE_SUCCESS;
        } else {
            return CMDLINE_RETCODE_INVALID_PARAMETERS;
        }
    } else if (cmd_parameter_val(argc, argv, "--long", &str)) {
        if (string_to_bytes(str, long_addr, 8) != 0) {
            return CMDLINE_RETCODE_INVALID_PARAMETERS;
        }
        return mac_filter_add_long(mac_interface->parent_id, long_addr, lqi_m, lqi_add, dbm_m, dbm_add) < 0 ? CMDLINE_RETCODE_FAIL : CMDLINE_RETCODE_SUCCESS;
    } else {
        return CMDLINE_RETCODE_INVALID_PARAMETERS;
    }
}

static int filter_remove(int argc, char *argv[])
{
    char *str;
    uint32_t short_addr;
    uint8_t long_addr[8];

    if (cmd_parameter_val(argc, argv, "--short", &str)) {
        short_addr = strtoul(str, NULL, 16);
        if (short_addr <= 0xFFFF) {
            return mac_filter_delete_short(mac_interface->parent_id, short_addr) < 0 ? CMDLINE_RETCODE_FAIL : CMDLINE_RETCODE_SUCCESS;
        } else {
            return CMDLINE_RETCODE_INVALID_PARAMETERS;
        }
    } else if (cmd_parameter_val(argc, argv, "--long", &str)) {
        if (string_to_bytes(str, long_addr, 8) != 0) {
            return CMDLINE_RETCODE_INVALID_PARAMETERS;
        }
        return mac_filter_delete_long(mac_interface->parent_id, long_addr) < 0 ? CMDLINE_RETCODE_FAIL : CMDLINE_RETCODE_SUCCESS;
    } else {
        return CMDLINE_RETCODE_INVALID_PARAMETERS;
    }
}

static int filter_clear(int argc, char *argv[])
{
    return mac_filter_clear(mac_interface->parent_id) < 0 ? CMDLINE_RETCODE_FAIL : CMDLINE_RETCODE_SUCCESS;
}

static int filter_stop(int argc, char *argv[])
{
    mac_filter_stop(mac_interface->parent_id);
    return CMDLINE_RETCODE_SUCCESS;
}
#else
static int filter_start(int argc, char *argv[])
{
    (void)argc;
    (void)argv;
    return 0;
}

static int filter_add(int argc, char *argv[])
{
    (void)argc;
    (void)argv;
    return 0;
}
static int filter_remove(int argc, char *argv[])
{
    (void)argc;
    (void)argv;
    return 0;
}
static int filter_clear(int argc, char *argv[])
{
    (void)argc;
    (void)argv;
    return 0;
}
static int filter_stop(int argc, char *argv[])
{
    (void)argc;
    (void)argv;
    return 0;
}
#endif
int mac_filter_command(int argc, char *argv[])
{
    char *cmd = argv[1];

    if (strcmp(cmd, "start") == 0) {
        return filter_start(argc, argv);
    } else if (strcmp(cmd, "add") == 0) {
        return filter_add(argc, argv);
    } else if (strcmp(cmd, "remove") == 0) {
        return filter_remove(argc, argv);
    } else if (strcmp(cmd, "clear") == 0) {
        return filter_clear(argc, argv);
    } else if (strcmp(cmd, "stop") == 0) {
        return filter_stop(argc, argv);
    }
    return CMDLINE_RETCODE_INVALID_PARAMETERS;
}

int mac_config_status_command(int argc, char *argv[])
{
    int32_t val;
    char *str;

    if (cmd_parameter_int(argc, argv, "--data_conf", &val)) {
        if (val >= 0 && val <= 255) {
            expected_statuses.data_conf = val;
        } else {
            return CMDLINE_RETCODE_INVALID_PARAMETERS;
        }
    } else if (cmd_parameter_val(argc, argv, "--data_ind", &str)) {
        size_t len = strlen(str);
        ns_dyn_mem_free(expected_statuses.data_ind);
        expected_statuses.data_ind = (uint8_t *)ns_dyn_mem_temporary_alloc(len);
        expected_statuses.data_ind_len = len;
        std::memcpy(expected_statuses.data_ind, str, len);
    } else if (cmd_parameter_int(argc, argv, "--get", &val)) {
        if (val >= 0 && val <= 255) {
            expected_statuses.get_conf = val;
        } else {
            return CMDLINE_RETCODE_INVALID_PARAMETERS;
        }
    } else if (cmd_parameter_int(argc, argv, "--scan", &val)) {
        if (val >= 0 && val <= 255) {
            expected_statuses.scan_conf = val;
        } else {
            return CMDLINE_RETCODE_INVALID_PARAMETERS;
        }
    } else if (cmd_parameter_int(argc, argv, "--poll", &val)) {
        if (val >= 0 && val <= 255) {
            expected_statuses.poll_conf = val;
        } else {
            return CMDLINE_RETCODE_INVALID_PARAMETERS;
        }
    } else if (cmd_parameter_int(argc, argv, "--purge", &val)) {
        if (val >= 0 && val <= 255) {
            expected_statuses.purge_conf = val;
        } else {
            return CMDLINE_RETCODE_INVALID_PARAMETERS;
        }
    } else if (cmd_parameter_int(argc, argv, "--comm_status", &val)) {
        if (val >= 0 && val <= 255) {
            expected_statuses.comm_status_ind = val;
        } else {
            return CMDLINE_RETCODE_INVALID_PARAMETERS;
        }
    } else if (cmd_parameter_index(argc, argv, "--list") != -1) {
        for (unsigned int i = 0; i < 256; ++i) {
            const char *status = mlme_status_string(i);
            if (status) {
                cmd_printf("%hhu\t%s\n", i, status);
            }
        }
    } else if (cmd_parameter_index(argc, argv, "--reset") != -1) {
        expected_statuses.data_conf = MLME_SUCCESS;
        expected_statuses.get_conf = MLME_SUCCESS;
        expected_statuses.scan_conf = MLME_SUCCESS;
        expected_statuses.poll_conf = MLME_SUCCESS;
        expected_statuses.purge_conf = MLME_SUCCESS;
        expected_statuses.comm_status_ind = MLME_SUCCESS;
        expected_statuses.data_ind_len = 0;
        expected_statuses.data_ind = NULL;
    } else if (argc == 1) {
        cmd_printf("MCPS-DATA.confirm:              %d (%s)\n", expected_statuses.data_conf, mlme_status_string(expected_statuses.data_conf));
        cmd_printf("MLME-GET.confirm:               %d (%s)\n", expected_statuses.get_conf, mlme_status_string(expected_statuses.get_conf));
        cmd_printf("MLME-SCAN.confirm:              %d (%s)\n", expected_statuses.scan_conf, mlme_status_string(expected_statuses.scan_conf));
        cmd_printf("MLME-POLL.confirm:              %d (%s)\n", expected_statuses.poll_conf, mlme_status_string(expected_statuses.poll_conf));
        cmd_printf("MCPS.PURGE.confirm.             %d (%s)\n", expected_statuses.purge_conf, mlme_status_string(expected_statuses.purge_conf));
        cmd_printf("MLME-COMM-STATUS.indication:    %d (%s)\n", expected_statuses.comm_status_ind, mlme_status_string(expected_statuses.comm_status_ind));
        cmd_printf("MCPS-DATA.indication:           %s <%.*s>\n", trace_array(expected_statuses.data_ind, expected_statuses.data_ind_len), expected_statuses.data_ind_len, expected_statuses.data_ind);
    } else {
        return CMDLINE_RETCODE_INVALID_PARAMETERS;
    }
    return CMDLINE_RETCODE_SUCCESS;
}

int mac_find_beacon_command(int argc, char *argv[])
{
    char *str;

    if (cmd_parameter_val(argc, argv, "--data", &str)) {
        for (int i = 0; i < MLME_MAC_RES_SIZE_MAX; ++i) {
            if (received_beacons.beacons[i] == NULL) {
                continue;
            }
            if (strncmp(received_beacons.beacons[i], str, received_beacons.beacon_lengths[i]) == 0) {
                return CMDLINE_RETCODE_SUCCESS;
            }
        }
        return CMDLINE_RETCODE_FAIL;
    } else {
        return CMDLINE_RETCODE_INVALID_PARAMETERS;
    }
}

int mac_wait_command(int argc, char *argv[])
{
    int32_t val;
    static uint32_t timeout_ms = 1000;
    int remaining_ms = timeout_ms;
    if (cmd_parameter_int(argc, argv, "--timeout", &val)) {
        if (val >= 0) {
            timeout_ms = val;
        } else {
            return CMDLINE_RETCODE_INVALID_PARAMETERS;
        }
    }
    while (data_count < 1) {
        ThisThread::sleep_for(10);
        remaining_ms -= 10;
        if (remaining_ms <= 0) {
            return CMDLINE_RETCODE_FAIL;
        }
    }
    data_count = 0;
    return CMDLINE_RETCODE_SUCCESS;
}

int mac_analyze_ed_command(int argc, char *argv[])
{
    int32_t val;
    int channel;

    if (cmd_parameter_int(argc, argv, "--channel", &val)) {
        if (val >= 0 && val <= 26) {
            channel = val;
        } else {
            return CMDLINE_RETCODE_INVALID_PARAMETERS;
        }
    } else {
        return CMDLINE_RETCODE_INVALID_PARAMETERS;
    }

    if (cmd_parameter_int(argc, argv, "--above", &val)) {
        for (int i = 0; i < last_ed_results.count; ++i) {
            if (last_ed_results.channel[i] == channel) {
                return last_ed_results.ED_values[i] >= val ? CMDLINE_RETCODE_SUCCESS : CMDLINE_RETCODE_FAIL;
            }
        }
    } else if (cmd_parameter_int(argc, argv, "--below", &val)) {
        for (int i = 0; i < last_ed_results.count; ++i) {
            if (last_ed_results.channel[i] == channel) {
                return last_ed_results.ED_values[i] <= val ? CMDLINE_RETCODE_SUCCESS : CMDLINE_RETCODE_FAIL;
            }
        }
    } else {
        return CMDLINE_RETCODE_INVALID_PARAMETERS;
    }
    return CMDLINE_RETCODE_FAIL;
}

static void reset_security(mlme_security_t *sec)
{
    sec->SecurityLevel = 0;
    sec->KeyIdMode = 0;
    sec->KeyIndex = 0;
    memset(sec->Keysource, 0, 8);
}

int reset_command(int argc, char *argv[])
{
    wait_for_confirm = true;
    silent_mode = false;
    data_count = 0;

    start_req.PANId = 0x1234;
    start_req.LogicalChannel = 11;
    start_req.ChannelPage = 0;
    start_req.StartTime = 0;
    start_req.BeaconOrder = 15;
    start_req.SuperframeOrder = 15;
    start_req.PANCoordinator = true;
    start_req.BatteryLifeExtension = false;
    start_req.CoordRealignment = false;
    reset_security(&start_req.CoordRealignKey);
    reset_security(&start_req.BeaconRealignKey);

    scan_req.ScanType = MAC_ACTIVE_SCAN;
    scan_req.ScanChannels.channel_page = CHANNEL_PAGE_0;
    scan_req.ScanChannels.channel_mask[0] = 0x07FFF800;
    reset_security(&scan_req.Key);

    data_req.SrcAddrMode = 3;
    data_req.DstAddrMode = 3;
    data_req.DstPANId = 0x1234;
    memset(data_req.DstAddr, 0, 8);
    data_req.msduLength = 0;
    data_req.msdu = NULL;
    data_req.msduHandle = 0;
    data_req.TxAckReq = true;
    data_req.InDirectTx = false;
    data_req.PendingBit = false;
    reset_security(&data_req.Key);

    poll_req.CoordAddrMode = 3;
    poll_req.CoordPANId = 0x1234;
    memset(poll_req.CoordAddress, 0, 8);
    reset_security(&poll_req.Key);

    purge_req.msduHandle = 0;

    set_req.attr = (mlme_attr_t)0x39;
    set_req.attr_index = 0;
    set_req.value_pointer = NULL;
    set_req.value_size = 0;

    get_req.attr = (mlme_attr_t)0x39;
    get_req.attr_index = 0;

    reset_req.SetDefaultPIB = true;

    return CMDLINE_RETCODE_SUCCESS;
}

int silent_mode_command(int argc, char *argv[])
{
    char *cmd;
    if (argc < 2) {
        return CMDLINE_RETCODE_FAIL;
    }
    cmd = argv[1];
    if (strcmp(cmd, "on") == 0) {
        silent_mode = true;
        return CMDLINE_RETCODE_SUCCESS;
    } else if (strcmp(cmd, "off") == 0) {
        silent_mode = false;
        return CMDLINE_RETCODE_SUCCESS;
    } else {
        return CMDLINE_RETCODE_INVALID_PARAMETERS;
    }
}
