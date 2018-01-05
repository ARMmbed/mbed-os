#ifndef WIFI_TESTS_H
#define WIFI_TESTS_H

#include "WiFiInterface.h"

/** Get WiFiInterface based on provided
 * app_json. */
WiFiInterface *get_interface(void);

/*
 * Test cases
 */

/** Test that constructor of the driver works. */
void wifi_constructor(void);

/** This test case is to test whether the driver accepts valid credentials and reject ones that are not valid. */
void wifi_set_credential(void);

/** Test validity of WiFiInterface::set_channel(). */
void wifi_set_channel(void);

/** Test WiFiInterface::get_rssi() API.
 * When connected, it should return valid RSSI value. When unconnected it should return 0. */
void wifi_get_rssi(void);

/** Test WiFiInterface::connect(ssid, pass, security, channel) with NULL parameters */
void wifi_connect_params_null(void);

/** Test WiFiInterface::connect(ssid, pass, security) with valid parameters for unsecure network */
void wifi_connect_params_valid_unsecure(void);

/** Test WiFiInterface::connect(ssid, pass, security) with valid parameters for secure network */
void wifi_connect_params_valid_secure(void);

/** Test WiFiInterface::connect(ssid, pass, security, channel) with valid parameters for secure network using channel specified. */
void wifi_connect_params_channel(void);

/** Test WiFiInterface::connect(ssid, pass, security, channel) with valid parameters for secure network using wrong channel number. */
void wifi_connect_params_channel_fail(void);

/** Test WiFiInterface::connect() without parameters. Use set_credentials() for setting parameters. */
void wifi_connect(void);

/** Test WiFiInterface::connect() without parameters. Don't set parameters with set_credentials() */
void wifi_connect_nocredentials(void);

/** Test WiFiInterface::connect() without parameters. Use secure settings for set_credentials. */
void wifi_connect_secure(void);

/** Test WiFiInterface::connect() failing with wrong password. */
void wifi_connect_secure_fail(void);

/** Test WiFiInterface::connect() - disconnect() repeatition works. */
void wifi_connect_disconnect_repeat(void);

/** Call WiFiInterface::scan() with null parameters to get number of networks available. */
void wifi_scan_null(void);

/** Call WiFiInterface::scan() with valid accesspoint list allocated */
void wifi_scan(void);

#endif //WIFI_TESTS_H
