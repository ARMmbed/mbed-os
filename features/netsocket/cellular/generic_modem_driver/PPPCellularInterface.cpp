/* Copyright (c) 2017 ARM Limited
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
#include "PPPCellularInterface.h"

#if NSAPI_PPP_AVAILABLE

#include <string.h>
#include "nsapi_ppp.h"
#if MBED_CONF_PPP_CELL_IFACE_APN_LOOKUP
#include "utils/APN_db.h"
#endif //MBED_CONF_PPP_CELL_IFACE_APN_LOOKUP
#if defined(FEATURE_COMMON_PAL)
#include "mbed_trace.h"
#define TRACE_GROUP "UCID"
#else
#define tr_debug(...) (void(0)) //dummies if feature common pal is not added
#define tr_info(...)  (void(0)) //dummies if feature common pal is not added
#define tr_error(...) (void(0)) //dummies if feature common pal is not added
#endif //defined(FEATURE_COMMON_PAL)

/**
 * PDP (packet data profile) Context
 */
#define CTX "1"

/**
 * Output Enter sequence for the modem , default CR
 */
#define OUTPUT_ENTER_KEY  "\r"

#if MBED_CONF_PPP_CELL_IFACE_AT_PARSER_BUFFER_SIZE
#define AT_PARSER_BUFFER_SIZE   MBED_CONF_PPP_CELL_IFACE_AT_PARSER_BUFFER_SIZE //bytes
#else
#define AT_PARSER_BUFFER_SIZE   256 //bytes
#endif //MBED_CONF_PPP_CELL_IFACE_AT_PARSER_BUFFER_SIZE

#if MBED_CONF_PPP_CELL_IFACE_AT_PARSER_TIMEOUT
#define AT_PARSER_TIMEOUT       MBED_CONF_PPP_CELL_IFACE_AT_PARSER_TIMEOUT
#else
#define AT_PARSER_TIMEOUT       8*1000 //miliseconds
#endif //MBED_CONF_PPP_CELL_IFACE_AT_PARSER_TIMEOUT

static bool initialized;
static bool set_credentials_api_used;
static bool set_sim_pin_check_request;
static bool change_pin;
static device_info dev_info;

static void parser_abort(ATCmdParser *at)
{
    at->abort();
}

static bool get_CCID(ATCmdParser *at)
{
    // Returns the ICCID (Integrated Circuit Card ID) of the SIM-card.
    // ICCID is a serial number identifying the SIM.
    bool success = at->send("AT+CCID") && at->recv("+CCID: %20[^\n]\nOK\n", dev_info.ccid);
    tr_debug("DevInfo: CCID=%s", dev_info.ccid);
    return success;
}

static bool get_IMSI(ATCmdParser *at)
{
    // International mobile subscriber identification
    bool success = at->send("AT+CIMI") && at->recv("%15[^\n]\nOK\n", dev_info.imsi);
    tr_debug("DevInfo: IMSI=%s", dev_info.imsi);
    return success;
}

static bool get_IMEI(ATCmdParser *at)
{
    // International mobile equipment identifier
    bool success = at->send("AT+CGSN") && at->recv("%15[^\n]\nOK\n", dev_info.imei);
    tr_debug("DevInfo: IMEI=%s", dev_info.imei);
    return success;
}

static bool get_MEID(ATCmdParser *at)
{
    // Mobile equipment identifier
    bool success = at->send("AT+GSN")
            && at->recv("%18[^\n]\nOK\n", dev_info.meid);
    tr_debug("DevInfo: MEID=%s", dev_info.meid);
    return success;
}

static bool set_CMGF(ATCmdParser *at)
{
    // Preferred message format
    // set AT+CMGF=[mode] , 0 PDU mode, 1 text mode
    bool success = at->send("AT+CMGF=1") && at->recv("OK");
    return success;
}

static bool set_CNMI(ATCmdParser *at)
{
    // New SMS indication configuration
    // set AT+CMTI=[mode, index] , 0 PDU mode, 1 text mode
    // Multiple URCs for SMS, i.e., CMT, CMTI, UCMT, CBMI, CDSI as DTE could be following any of these SMS formats
    bool success = at->send("AT+CNMI=2," CTX) && at->recv("OK");
    return success;
}

static void CMTI_URC(ATCmdParser *at)
{
    // our CMGF = 1, i.e., text mode. So we expect response in this format:
    //+CMTI: <mem>,<index>,
    at->recv(": %*u,%*u");
    tr_info("New SMS received");

}

static void CMT_URC(ATCmdParser *at)
{
    // our CMGF = 1, i.e., text mode. So we expect response in this format:
    //+CMT: <oa>,[<alpha>],<scts>[,<tooa>,
    //<fo>,<pid>,<dcs>,<sca>,<tosca>,
    //<length>]<CR><LF><data>
    // By default detailed SMS header CSDH=0 , so we are not expecting  [,<tooa>,
    //<fo>,<pid>,<dcs>,<sca>,<tosca>
    char sms[50];
    char service_timestamp[15];
    at->recv(": %49[^\"]\",,%14[^\"]\"\n", sms, service_timestamp);

    tr_info("SMS:%s, %s", service_timestamp, sms);

}

static bool set_atd(ATCmdParser *at)
{
    bool success = at->send("ATD*99***" CTX "#") && at->recv("CONNECT");

    return success;
}

/**
 * Enables or disables SIM pin check lock
 */
static nsapi_error_t do_sim_pin_check(ATCmdParser *at, const char *pin)
{
    bool success;
    if (set_sim_pin_check_request) {
        /* use the SIM locked */
        success = at->send("AT+CLCK=\"SC\",1,\"%s\"", pin) && at->recv("OK");
    } else {
        /* use the SIM unlocked */
        success = at->send("AT+CLCK=\"SC\",0,\"%s\"",pin) && at->recv("OK");
    }

    if (success) return NSAPI_ERROR_OK;

    return NSAPI_ERROR_AUTH_FAILURE;
}

/**
 * Change the pin code for the SIM card
 */
static nsapi_error_t do_change_sim_pin(ATCmdParser *at, const char *old_pin, const char *new_pin)
{
    /* changes the SIM pin */
       bool success = at->send("AT+CPWD=\"SC\",\"%s\",\"%s\"", old_pin, new_pin) && at->recv("OK");
       if (success) {
           return NSAPI_ERROR_OK;
       }

       return NSAPI_ERROR_AUTH_FAILURE;
}

static void set_nwk_reg_status_csd(unsigned int status)
{
    switch (status) {
        case CSD_NOT_REGISTERED_NOT_SEARCHING:
        case CSD_NOT_REGISTERED_SEARCHING:
            break;
        case CSD_REGISTERED:
        case CSD_REGISTERED_ROAMING:
            tr_debug("Registered for circuit switched service");
            break;
        case CSD_REGISTRATION_DENIED:
            tr_debug("Circuit switched service denied");
            break;
        case CSD_UNKNOWN_COVERAGE:
            tr_debug("Out of circuit switched service coverage");
            break;
        case CSD_SMS_ONLY:
            tr_debug("SMS service only");
            break;
        case CSD_SMS_ONLY_ROAMING:
            tr_debug("SMS service only");
            break;
        case CSD_CSFB_NOT_PREFERRED:
            tr_debug("Registered for circuit switched service with CSFB not preferred");
            break;
        default:
            tr_debug("Unknown circuit switched service registration status. %u", status);
            break;
    }

    dev_info.reg_status_csd = static_cast<nwk_registration_status_csd>(status);
}

static void set_nwk_reg_status_psd(unsigned int status)
{
    switch (status) {
        case PSD_NOT_REGISTERED_NOT_SEARCHING:
        case PSD_NOT_REGISTERED_SEARCHING:
            break;
        case PSD_REGISTERED:
        case PSD_REGISTERED_ROAMING:
            tr_debug("Registered for packet switched service");
            break;
        case PSD_REGISTRATION_DENIED:
            tr_debug("Packet switched service denied");
            break;
        case PSD_UNKNOWN_COVERAGE:
            tr_debug("Out of packet switched service coverage");
            break;
        case PSD_EMERGENCY_SERVICES_ONLY:
            tr_debug("Limited access for packet switched service. Emergency use only.");
            break;
        default:
            tr_debug("Unknown packet switched service registration status. %u", status);
            break;
    }

    dev_info.reg_status_psd = static_cast<nwk_registration_status_psd>(status);
}

static bool is_registered_csd()
{
  return (dev_info.reg_status_csd == CSD_REGISTERED) ||
          (dev_info.reg_status_csd == CSD_REGISTERED_ROAMING) ||
          (dev_info.reg_status_csd == CSD_CSFB_NOT_PREFERRED);
}

static bool is_registered_psd()
{
    return (dev_info.reg_status_psd == PSD_REGISTERED) ||
            (dev_info.reg_status_psd == PSD_REGISTERED_ROAMING);
}

PPPCellularInterface::PPPCellularInterface(FileHandle *fh, bool debug)
{
    _new_pin = NULL;
    _pin = NULL;
    _at = NULL;
    _apn = "internet";
    _uname = NULL;
    _pwd = NULL;
    _fh = fh;
    _debug_trace_on = debug;
    _stack = DEFAULT_STACK;
    dev_info.reg_status_csd = CSD_NOT_REGISTERED_NOT_SEARCHING;
    dev_info.reg_status_psd = PSD_NOT_REGISTERED_NOT_SEARCHING;
    dev_info.ppp_connection_up = false;
}


PPPCellularInterface::~PPPCellularInterface()
{
    delete _at;
}

void PPPCellularInterface::enable_hup(bool)
{
    //meant to be overridden
}

void PPPCellularInterface::modem_init()
{
    //meant to be overridden
}

void PPPCellularInterface::modem_deinit()
{
    //meant to be overridden
}

void PPPCellularInterface::modem_power_up()
{
    //meant to be overridden
}

void PPPCellularInterface::modem_power_down()
{
    //meant to be overridden
}

void PPPCellularInterface::modem_debug_on(bool on)
{
    _debug_trace_on = on;
}

void PPPCellularInterface::connection_status_cb(Callback<void(nsapi_error_t)> cb)
{
    _connection_status_cb = cb;
}

/**
 * Public API. Sets up the flag for the driver to enable or disable SIM pin check
 * at the next boot.
 */
void PPPCellularInterface::set_sim_pin_check(bool check)
{
    set_sim_pin_check_request = check;
}

/**
 * Public API. Sets up the flag for the driver to change pin code for SIM card
 */
void PPPCellularInterface::set_new_sim_pin(const char *new_pin)
{
    change_pin = true;
    _new_pin = new_pin;
}

bool PPPCellularInterface::nwk_registration(uint8_t nwk_type)
{
    bool success = false;
     bool registered = false;

     char str[35];
     int retcode;
     int retry_counter = 0;
     unsigned int reg_status;

     success = nwk_type == PACKET_SWITCHED ?
                     _at->send("AT+CGREG=0") :
                     _at->send("AT+CREG=0") && _at->recv("OK\n");

     success = _at->send("AT+COPS=0") //initiate auto-registration
                    && _at->recv("OK");
     if (!success) {
         tr_error("Modem not responding.");
         return false;
     }

     //Network search
     //If not registered after 60 attempts, i.e., 30 seconds wait, give up
     tr_debug("Searching Network ...");

     while (!registered) {

         if (retry_counter > 60) {
             success = false;
             goto give_up;
         }

         success = nwk_type == PACKET_SWITCHED ?
                         _at->send("AT+CGREG?")
                         && _at->recv("+CGREG: %34[^\n]\n", str)
                         && _at->recv("OK\n") :
                         _at->send("AT+CREG?")
                         && _at->recv("+CREG: %34[^\n]\n", str)
                         && _at->recv("OK\n");

         retcode = sscanf(str, "%*u,%u", &reg_status);

         if (retcode >= 1) {
             if (nwk_type == PACKET_SWITCHED) {
                 set_nwk_reg_status_psd(reg_status);
                 if (is_registered_psd()) {
                     registered = true;
                 }
             } else if (nwk_type == CIRCUIT_SWITCHED) {
                 set_nwk_reg_status_csd(reg_status);
                 if (is_registered_csd()) {
                     registered = true;
                 }
             }
         }

         if (registered) {
             break;
         } else {
             wait_ms(500);
         }

         retry_counter++;
     }

 give_up:
     return registered;
}

bool PPPCellularInterface::is_connected()
{
    return dev_info.ppp_connection_up;
}

// Get the SIM card going.
nsapi_error_t PPPCellularInterface::initialize_sim_card()
{
    nsapi_error_t nsapi_error = NSAPI_ERROR_AUTH_FAILURE;
    int retry_count = 0;
    bool done = false;

    /* SIM initialization may take a significant amount, so an error is
     * kind of expected. We should retry 10 times until we succeed or timeout. */
    for (retry_count = 0; !done && (retry_count < 10); retry_count++) {
        char pinstr[16];

        if (_at->send("AT+CPIN?") && _at->recv("+CPIN: %15[^\n]\nOK\n", pinstr)) {
            if (strcmp(pinstr, "SIM PIN") == 0) {
                if (!_at->send("AT+CPIN=\"%s\"", _pin) || !_at->recv("OK")) {
                    tr_debug("PIN correct");
                    nsapi_error = NSAPI_ERROR_OK;
                }
            } else if (strcmp(pinstr, "READY") == 0) {
                tr_debug("SIM Ready");
                nsapi_error = NSAPI_ERROR_OK;
                done = true;
            } else {
                tr_debug("Unexpected response from SIM: \"%s\"", pinstr);
            }
        }

        /* wait for a second before retry */
        wait_ms(1000);
    }

    if (!done) {
        tr_error("SIM not ready.");
    }

    return nsapi_error;
}

void PPPCellularInterface::set_sim_pin(const char *pin) {
    /* overwrite the default pin by user provided pin */
    _pin = pin;
}

nsapi_error_t PPPCellularInterface::setup_context_and_credentials()
{
    bool success;

    if (!_apn) {
        return NSAPI_ERROR_PARAMETER;
    }

#if NSAPI_PPP_IPV4_AVAILABLE && NSAPI_PPP_IPV6_AVAILABLE
    const char ipv4v6_pdp_type[] = {"IPV4V6"};
    const char ipv4_pdp_type[] = {"IP"};
    const char *pdp_type = ipv4v6_pdp_type;
    _stack = IPV4V6_STACK;
#elif NSAPI_PPP_IPV6_AVAILABLE
    const char pdp_type[] = {"IPV6"};
#elif NSAPI_PPP_IPV4_AVAILABLE
    const char pdp_type[] = {"IP"};
#endif
    const char *auth = _uname && _pwd ? "CHAP:" : "";

#if NSAPI_PPP_IPV4_AVAILABLE && NSAPI_PPP_IPV6_AVAILABLE
retry_without_dual_stack:
#endif
    success = _at->send("AT"
                          "+FCLASS=0;" // set to connection (ATD) to data mode
                          "+CGDCONT=" CTX ",\"%s\",\"%s%s\"",
                          pdp_type, auth, _apn
                         )
                   && _at->recv("OK");

#if NSAPI_PPP_IPV4_AVAILABLE && NSAPI_PPP_IPV6_AVAILABLE
    if (_stack == IPV4V6_STACK) {
        if (!success) {
            // fallback to ipv4
            pdp_type = ipv4_pdp_type;
            _stack = IPV4_STACK;
            goto retry_without_dual_stack;
        }
    }
#endif

    if (!success) {
        _at->recv("OK");
    }

    return success ? NSAPI_ERROR_OK : NSAPI_ERROR_PARAMETER;

}

void  PPPCellularInterface::set_credentials(const char *apn, const char *uname,
                                                               const char *pwd)
{
    _apn = apn;
    _uname = uname;
    _pwd = pwd;
    set_credentials_api_used = true;
}



void PPPCellularInterface::setup_at_parser()
{
    if (_at) {
        return;
    }

    _at = new ATCmdParser(_fh, OUTPUT_ENTER_KEY, AT_PARSER_BUFFER_SIZE, AT_PARSER_TIMEOUT,
                         _debug_trace_on ? true : false);

    /* Error cases, out of band handling  */
    _at->oob("ERROR", callback(parser_abort, _at));
    _at->oob("+CME ERROR", callback(parser_abort, _at));
    _at->oob("+CMS ERROR", callback(parser_abort, _at));
    _at->oob("NO CARRIER", callback(parser_abort, _at));

    /* URCs, handled out of band */
    _at->oob("+CMT", callback(CMT_URC, _at));
    _at->oob("+CMTI", callback(CMTI_URC, _at));
}

void PPPCellularInterface::shutdown_at_parser()
{
    delete _at;
    _at = NULL;
}

nsapi_error_t PPPCellularInterface::connect(const char *sim_pin, const char *apn, const char *uname, const char *pwd)
{
    if (!sim_pin) {
        return NSAPI_ERROR_PARAMETER;
    }

    if (apn) {
        _apn = apn;
    }

    if (uname && pwd) {
        _uname = uname;
        _pwd = pwd;
    } else {
        _uname = NULL;
        _pwd = NULL;
    }

    _pin = sim_pin;

    return connect();
}

nsapi_error_t PPPCellularInterface::connect()
{
    nsapi_error_t retcode;
    bool success;
    bool did_init = false;
    const char *apn_config = NULL;

    if (dev_info.ppp_connection_up) {
        return NSAPI_ERROR_IS_CONNECTED;
    }

    do {
        retry_init:

        /* setup AT parser */
        setup_at_parser();

        if (!initialized) {

            /* If we have hangup (eg DCD) detection, we don't want it active
             * as long as we are using ATCmdParser.
             * As soon as we get into data mode, we will turn it back on. */
            enable_hup(false);

            if (!power_up()) {
                return NSAPI_ERROR_DEVICE_ERROR;
            }

            retcode = initialize_sim_card();
            if (retcode != NSAPI_ERROR_OK) {
                return retcode;
            }

            success = nwk_registration(PACKET_SWITCHED) //perform network registration
            && get_CCID(_at)//get integrated circuit ID of the SIM
            && get_IMSI(_at)//get international mobile subscriber information
            && get_IMEI(_at)//get international mobile equipment identifier
            && get_MEID(_at)//its same as IMEI
            && set_CMGF(_at)//set message format for SMS
            && set_CNMI(_at);//set new SMS indication

            if (!success) {
                return NSAPI_ERROR_NO_CONNECTION;
            }

#if MBED_CONF_PPP_CELL_IFACE_APN_LOOKUP
            if (!apn_config) {
                apn_config = apnconfig(dev_info.imsi);
            }
#endif

            /* Check if user want skip SIM pin checking on boot up */
            if (set_sim_pin_check_request) {
                retcode = do_sim_pin_check(_at, _pin);
                if (retcode != NSAPI_ERROR_OK) {
                    return retcode;
                }
                /* set this request to false, as it is unnecessary to repeat in case of retry */
                set_sim_pin_check_request = false;
            }

            /* check if the user requested a sim pin change */
            if (change_pin) {
                retcode = do_change_sim_pin(_at, _pin, _new_pin);
                if (retcode != NSAPI_ERROR_OK) {
                    return retcode;
                }
                /* set this request to false, as it is unnecessary to repeat in case of retry */
                change_pin = false;
            }

#if MBED_CONF_PPP_CELL_IFACE_APN_LOOKUP
            if (apn_config) {
                _apn = _APN_GET(apn_config);
                _uname = _APN_GET(apn_config);
                _pwd = _APN_GET(apn_config);
                tr_info("Looked up APN %s.", _apn);
            }
#endif

            //sets up APN and IP protocol for external PDP context
            retcode = setup_context_and_credentials();
            if (retcode != NSAPI_ERROR_OK) {
                return retcode;
            }

            if (!success) {
                shutdown_at_parser();
                return NSAPI_ERROR_NO_CONNECTION;
            }

            initialized = true;
            did_init = true;
        } else {
            /* If we were already initialized, we expect to receive NO_CARRIER response
             * from the modem as we were kicked out of Data mode */
            _at->recv("NO CARRIER");
            success = _at->send("AT") && _at->recv("OK");
        }

        /* Attempt to enter data mode */
        success = set_atd(_at); //enter into Data mode with the modem
        if (!success) {
            power_down();
            initialized = false;

            /* if we were previously initialized , i.e., not in this particular attempt,
             * we want to re-initialize */
            if (!did_init) {
                goto retry_init;
            }

            /* shutdown AT parser before notifying application of the failure */
            shutdown_at_parser();

            return NSAPI_ERROR_NO_CONNECTION;
        }

        /* This is the success case.
         * Save RAM, discard AT Parser as we have entered Data mode. */
        shutdown_at_parser();

        /* We now want hangup (e.g., DCD) detection if available */
        enable_hup(true);

        /* Initialize PPP
         * mbed_ppp_init() is a blocking call, it will block until
         * connected, or timeout after 30 seconds*/
        retcode = nsapi_ppp_connect(_fh, _connection_status_cb, _uname, _pwd, _stack);
        if (retcode == NSAPI_ERROR_OK) {
            dev_info.ppp_connection_up = true;
        }

    }while(!dev_info.ppp_connection_up && apn_config && *apn_config);

    return retcode;
}

/**
 * User initiated disconnect
 *
 * Disconnects from PPP connection only and brings down the underlying network
 * interface
 */
nsapi_error_t PPPCellularInterface::disconnect()
{
    nsapi_error_t ret = nsapi_ppp_disconnect(_fh);
    if (ret == NSAPI_ERROR_OK) {
        dev_info.ppp_connection_up = false;
        return NSAPI_ERROR_OK;
    }

    return ret;
}

const char *PPPCellularInterface::get_ip_address()
{
    return nsapi_ppp_get_ip_addr(_fh);
}

const char *PPPCellularInterface::get_netmask()
{
    return nsapi_ppp_get_netmask(_fh);
}

const char *PPPCellularInterface::get_gateway()
{
    return nsapi_ppp_get_ip_addr(_fh);
}

/** Power down modem
 *  Uses AT command to do it */
void PPPCellularInterface::power_down()
{
    modem_power_down();
    modem_deinit();
}

/**
 * Powers up the modem
 *
 * Enables the GPIO lines to the modem and then wriggles the power line in short pulses.
 */
bool PPPCellularInterface::power_up()
{
    /* Initialize GPIO lines */
    modem_init();
    /* Give modem a little time to settle down */
    wait(0.25);

    bool success = false;

    int retry_count = 0;
    while (true) {
        modem_power_up();
        /* Modem tends to spit out noise during power up - don't confuse the parser */
        _at->flush();
        /* It is mandatory to avoid sending data to the serial port during the first 200 ms
         * of the module startup. Telit_xE910 Global form factor App note.
         * Not necessary for all types of modems however. Let's wait just to be on the safe side */
        wait_ms(200);
        _at->set_timeout(1000);
        if (_at->send("AT") && _at->recv("OK")) {
            tr_info("Modem Ready.");
            break;
        }

        if (++retry_count > 10) {
            goto failure;
        }
    }

    _at->set_timeout(8000);

    /*For more details regarding DCD and DTR circuitry, please refer to Modem AT manual */
    success = _at->send("AT"
                        "E0;" //turn off modem echoing
                        "+CMEE=2;"//turn on verbose responses
                        "&K0"//turn off RTC/CTS handshaking
                        "+IPR=115200;"//setup baud rate
                        "&C1;"//set DCD circuit(109), changes in accordance with the carrier detect status
                        "&D0")//set DTR circuit, we ignore the state change of DTR
              && _at->recv("OK");

    if (!success) {
        goto failure;
    }

    /* If everything alright, return from here with success*/
    return success;

failure:
    tr_error("Preliminary modem setup failed.");
    return false;
}

/**
 * Get a pointer to the underlying network stack
 */
NetworkStack *PPPCellularInterface::get_stack()
{
    return nsapi_ppp_get_stack();
}

#endif // NSAPI_PPP_AVAILABLE
