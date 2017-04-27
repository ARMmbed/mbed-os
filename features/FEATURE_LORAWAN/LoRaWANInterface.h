/* LoRaWAN API Class
 * Copyright (c) 2016 ARM Limited
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
 
// https://stackforce.github.io/LoRaMac-doc/
// https://www.lora-alliance.org/What-Is-LoRa/Technology
// http://iot.semtech.com/help/
// https://partner.orange.com/wp-content/uploads/2016/04/LoRa-Device-Developer-Guide-Orange.pdf
// https://developer.mbed.org/teams/sandbox/code/LoRaWANInterface/

#ifndef LORAWANINTERFACE_H
#define LORAWANINTERFACE_H
 
#include "mbed.h"

/** LoRaWAN API
 */
class LoRaWANInterface
{ 
public:
    typedef enum lora_event {
        LORA_CONNECT, LORA_UP_CNT, LORA_DOWN_CNT, LORA_SEND_ACK, LORA_SEND_NACK,
        LORA_SEND_READY, LORA_SEND_TIMEOUT, LORA_RECV_READY,
        LORA_TIMEOUT = 0xffffffff,
    } lora_event_t;
    
    // Sequence counter is a security mechanism in LoRaWAN which prevents
    // replay attacks. Adding a simple way to restore the counters from non-volatile storage.
    LoRaWANInterface(LoRaRadio *radio, int16_t seq_down_cnt, int16_t seq_up_cnt);
    LoRaWANInterface(LoRaRadio *radio);
    int set_seq_up_cnt(int16_t cntr);
    int set_seq_down_cnt(int16_t cntr);
    int16_t get_seq_up_cnt(void);
    int16_t get_seq_down_cnt(void);

    // method for notification on events
    void attach(Callback<void(lora_event_t)> cb);
    void enable_callback(lora_event_t event);
    void disable_callback(lora_event_t event);

    //Unique (device) Network key (EUI64) and ensure security on network level
    //Device address for the network (32 bits)
    //Network session (AES-128) root keys specific to the end-device
    //applicaiton session (AES-128) root keys specific to the end-device
    int8_t abp_connect(char *dev_eui, uint32_t dev_addr, char *nwk_skey, char *app_skey);    
    
    //Unique (device) Network key (EUI64) and ensure security on network level
    //Unique Application key (EUI64) ensure end to end security on application level
    //Device specific key (EUI128)
    int8_t otaa_connect(char *dev_eui, char *app_eui, char *app_key);
    char *get_dev_eui(void);
    char *get_app_eui(void);
    char *get_nwk_skey(void);
    char *get_app_skey(void);
    uint32_t get_dev_addr(void);

    // Send a message over the network, we need a port and data
    // ACK can be enabled, and when it is we provide a callback method.
    // The callback method will also be invoked if timeout happens and we no
    // longer listen for ACK.
    uint32_t send_ready(void);
    int8_t send(int8_t port, uint8_t *data, size_t len, uint8_t ack = 0);

    // Register a function which will be invoked when we receive a message.
    // Pass in a callback which will receive two parameters: port and data.
    bool recv_ready(void);
    void recv(int8_t port, uint8_t *data, size_t len);

    void enable_adr(bool en);

    // spread factor comes from channel plan
    // should be a few of these based on the region to be deployed
    void set_spreadfactor(lora_channelplan_t sf);
    lora_channelplan_t get_spreadfactor();

private:
    Callback<void(lora_event_t)> _cb;
    lora_event_t _cb_event;
};

#endif
