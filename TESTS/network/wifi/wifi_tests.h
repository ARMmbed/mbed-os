#ifndef WIFI_TESTS_H
#define WIFI_TESTS_H

#include "WiFiInterface.h"

WiFiInterface *get_interface(void);

/* Test cases */
void wifi_constructor(void);
void wifi_set_credential(void);
void wifi_set_channel(void);
void wifi_get_rssi(void);
void wifi_connect_params_null(void);
void wifi_connect_params_valid_unsecure(void);
void wifi_connect_params_valid_secure(void);
void wifi_connect_params_channel(void);
void wifi_connect_params_channel_fail(void);
void wifi_connect(void);
void wifi_connect_nocredentials(void);
void wifi_connect_secure(void);
void wifi_connect_secure_fail(void);
void wifi_connect_disconnect_repeat(void);
void wifi_scan_null(void);
void wifi_scan(void);

#endif //WIFI_TESTS_H
