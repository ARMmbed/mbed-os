/* mbed Microcontroller Library
 * Copyright (c) 2018-2018 ARM Limited
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

#ifndef USBKEYBOARD_H
#define USBKEYBOARD_H

#include "USBHID.h"
#include "Stream.h"

/* Modifiers, left keys then right keys. */
enum MODIFIER_KEY {
    KEY_CTRL = 0x01,
    KEY_SHIFT = 0x02,
    KEY_ALT = 0x04,
    KEY_LOGO = 0x08,
    KEY_RCTRL = 0x10,
    KEY_RSHIFT = 0x20,
    KEY_RALT = 0x40,
    KEY_RLOGO = 0x80,
};


enum MEDIA_KEY {
    KEY_NEXT_TRACK,     /*!< next Track Button */
    KEY_PREVIOUS_TRACK, /*!< Previous track Button */
    KEY_STOP,           /*!< Stop Button */
    KEY_PLAY_PAUSE,     /*!< Play/Pause Button */
    KEY_MUTE,           /*!< Mute Button */
    KEY_VOLUME_UP,      /*!< Volume Up Button */
    KEY_VOLUME_DOWN,    /*!< Volume Down Button */
};

enum FUNCTION_KEY {
    KEY_F1 = 128,   /* F1 key */
    KEY_F2,         /* F2 key */
    KEY_F3,         /* F3 key */
    KEY_F4,         /* F4 key */
    KEY_F5,         /* F5 key */
    KEY_F6,         /* F6 key */
    KEY_F7,         /* F7 key */
    KEY_F8,         /* F8 key */
    KEY_F9,         /* F9 key */
    KEY_F10,        /* F10 key */
    KEY_F11,        /* F11 key */
    KEY_F12,        /* F12 key */

    KEY_PRINT_SCREEN,   /* Print Screen key */
    KEY_SCROLL_LOCK,    /* Scroll lock */
    KEY_CAPS_LOCK,      /* caps lock */
    KEY_NUM_LOCK,       /* num lock */
    KEY_INSERT,         /* Insert key */
    KEY_HOME,           /* Home key */
    KEY_PAGE_UP,        /* Page Up key */
    KEY_PAGE_DOWN,      /* Page Down key */

    RIGHT_ARROW,        /* Right arrow */
    LEFT_ARROW,         /* Left arrow */
    DOWN_ARROW,         /* Down arrow */
    UP_ARROW,           /* Up arrow */
};

/**
 * USBKeyboard example
 * @code
 *
 * #include "mbed.h"
 * #include "USBKeyboard.h"
 *
 * USBKeyboard key;
 *
 * int main(void)
 * {
 *   while (1) {
 *       key.printf("Hello World\r\n");
 *       wait(1);
 *   }
 * }
 *
 * @endcode
 */
class USBKeyboard: public USBHID, public Stream {
public:

    /**
    *   Constructor
    *
    * @param vendor_id Your vendor_id (default: 0x1235)
    * @param product_id Your product_id (default: 0x0050)
    * @param product_release Your preoduct_release (default: 0x0001)
    *
    */
    USBKeyboard(uint16_t vendor_id = 0x1235, uint16_t product_id = 0x0050, uint16_t product_release = 0x0001):
            USBHID(0, 0, vendor_id, product_id, product_release, false) {
        _lock_status = 0;
        connect();
    };

    /**
    *   Constructor
    *
    * @param phy USB phy to use
    * @param vendor_id Your vendor_id (default: 0x1235)
    * @param product_id Your product_id (default: 0x0050)
    * @param product_release Your preoduct_release (default: 0x0001)
    *
    */
    USBKeyboard(USBPhy *phy, uint16_t vendor_id = 0x1235, uint16_t product_id = 0x0050, uint16_t product_release = 0x0001):
            USBHID(phy, 0, 0, vendor_id, product_id, product_release, false) {
        _lock_status = 0;
        connect();
    };

    /**
    * To send a character defined by a modifier(CTRL, SHIFT, ALT) and the key
    *
    * @code
    * //To send CTRL + s (save)
    *  keyboard.keyCode('s', KEY_CTRL);
    * @endcode
    *
    * @param modifier bit 0: KEY_CTRL, bit 1: KEY_SHIFT, bit 2: KEY_ALT (default: 0)
    * @param key character to send
    * @returns true if there is no error, false otherwise
    */
    bool key_code(uint8_t key, uint8_t modifier = 0);

    /**
    * Send a character
    *
    * @param c character to be sent
    * @returns true if there is no error, false otherwise
    */
    virtual int _putc(int c);

    /**
    * Control media keys
    *
    * @param key media key pressed (KEY_NEXT_TRACK, KEY_PREVIOUS_TRACK, KEY_STOP, KEY_PLAY_PAUSE, KEY_MUTE, KEY_VOLUME_UP, KEY_VOLUME_DOWN)
    * @returns true if there is no error, false otherwise
    */
    bool media_control(MEDIA_KEY key);

    /*
    * To define the report descriptor. Warning: this method has to store the length of the report descriptor in reportLength.
    *
    * @returns pointer to the report descriptor
    */
    virtual const uint8_t * report_desc();

    /*
    * Called when a data is received on the OUT endpoint. Useful to switch on LED of LOCK keys
    */
    virtual void report_rx();

    /**
    * Read status of lock keys. Useful to switch-on/off leds according to key pressed. Only the first three bits of the result is important:
    *   - First bit: NUM_LOCK
    *   - Second bit: CAPS_LOCK
    *   - Third bit: SCROLL_LOCK
    *
    * @returns status of lock keys
    */
    uint8_t lock_status();

protected:
    /*
    * Get configuration descriptor
    *
    * @returns pointer to the configuration descriptor
    */
    virtual const uint8_t * configuration_desc();

private:
    //dummy otherwise it doesn,t compile (we must define all methods of an abstract class)
    virtual int _getc() {
        return -1;
    };

    uint8_t configurationDescriptor[41];
    uint8_t _lock_status;

};

#endif
