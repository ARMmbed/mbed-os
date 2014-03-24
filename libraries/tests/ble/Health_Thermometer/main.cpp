/* mbed Microcontroller Library
 * Copyright (c) 2006-2014 ARM Limited
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

#include "mbed.h"
#include "TMP102.h"
#include "nRF51822n.h"

nRF51822n   nrf;                               /* BLE radio driver */
TMP102      healthThemometer(p22, p20, 0x90);  /* The TMP102 connected to our board */

/* LEDs for indication: */
DigitalOut  oneSecondLed(LED1);        /* LED1 is toggled every second. */
DigitalOut  advertisingStateLed(LED2); /* LED2 is on when we are advertising, otherwise off. */


/* Health Thermometer Service */ 
uint8_t             thermTempPayload[5] = { 0, 0, 0, 0, 0 };
GattService         thermService (GattService::UUID_HEALTH_THERMOMETER_SERVICE);
GattCharacteristic  thermTemp (GattCharacteristic::UUID_TEMPERATURE_MEASUREMENT_CHAR,
                               5, 5, GattCharacteristic::BLE_GATT_CHAR_PROPERTIES_INDICATE);

/* Battery Level Service */
uint8_t            batt = 100;     /* Battery level */
uint8_t            read_batt = 0; /* Variable to hold battery level reads */
GattService        battService ( GattService::UUID_BATTERY_SERVICE );
GattCharacteristic battLevel   ( GattCharacteristic::UUID_BATTERY_LEVEL_CHAR, 1, 1,
                                 GattCharacteristic::BLE_GATT_CHAR_PROPERTIES_NOTIFY |
                                 GattCharacteristic::BLE_GATT_CHAR_PROPERTIES_READ);


/* Advertising data and parameters */
GapAdvertisingData   advData;
GapAdvertisingData   scanResponse;
GapAdvertisingParams advParams ( GapAdvertisingParams::ADV_CONNECTABLE_UNDIRECTED );

uint16_t             uuid16_list[] = {GattService::UUID_HEALTH_THERMOMETER_SERVICE,
                                      GattService::UUID_BATTERY_SERVICE};

uint32_t quick_ieee11073_from_float(float temperature);
void updateServiceValues(void);

/**************************************************************************/
/*!
    @brief  This custom class can be used to override any GapEvents
            that you are interested in handling on an application level.
*/
/**************************************************************************/
class GapEventHandler : public GapEvents
{
    //virtual void onTimeout(void) {}   
     
    virtual void onConnected(void)
    {
        advertisingStateLed = 0;
    }

    /* When a client device disconnects we need to start advertising again. */
    virtual void onDisconnected(void)
    {
        nrf.getGap().startAdvertising(advParams);
        advertisingStateLed = 1;
    }
};

/**************************************************************************/
/*!
    @brief  Program entry point
*/
/**************************************************************************/
int main(void)
{
    
    /* Setup blinky led */
    oneSecondLed=1;
    
    /* Setup an event handler for GAP events i.e. Client/Server connection events. */
    nrf.getGap().setEventHandler(new GapEventHandler());
    
    /* Initialise the nRF51822 */
    nrf.init();

    /* Make sure we get a clean start */
    nrf.reset();    

    /* Add BLE-Only flag and complete service list to the advertising data */
    advData.addFlags(GapAdvertisingData::BREDR_NOT_SUPPORTED);
    advData.addData(GapAdvertisingData::COMPLETE_LIST_16BIT_SERVICE_IDS, 
                   (uint8_t*)uuid16_list, sizeof(uuid16_list));
    advData.addAppearance(GapAdvertisingData::GENERIC_THERMOMETER);
    nrf.getGap().setAdvertisingData(advData, scanResponse);

    /* Health Thermometer Service */
    thermService.addCharacteristic(thermTemp);
    nrf.getGattServer().addService(thermService);
    
    /* Add the Battery Level service */
    battService.addCharacteristic(battLevel);
    nrf.getGattServer().addService(battService);

    /* Start advertising (make sure you've added all your data first) */
    nrf.getGap().startAdvertising(advParams);
    advertisingStateLed = 1;

    for (;;)
    {
        /* Now that we're live, update the battery level & temperature characteristics */
        updateServiceValues();
        wait(1);
    }
}

/**************************************************************************/
/*!
    @brief  Ticker callback to switch advertisingStateLed state
*/
/**************************************************************************/
void updateServiceValues(void)
{
      /* Toggle the one second LEDs */
      oneSecondLed = !oneSecondLed;
      
      /* Update battery level */
      nrf.getGattServer().updateValue(battLevel.handle, (uint8_t*)&batt, sizeof(batt));
      /* Decrement the battery level. */
      batt <=50 ? batt=100 : batt--;;
      
      /* Update the temperature. Note that we need to convert to an ieee11073 format float. */
      float temperature = healthThemometer.read();
      uint32_t temp_ieee11073 = quick_ieee11073_from_float(temperature);
      memcpy(thermTempPayload+1, &temp_ieee11073, 4);
      nrf.getGattServer().updateValue(thermTemp.handle, thermTempPayload, sizeof(thermTempPayload));
}

/**
 * @brief A very quick conversion between a float temperature and 11073-20601 FLOAT-Type.
 * @param temperature The temperature as a float.
 * @return The temperature in 11073-20601 FLOAT-Type format.
 */
uint32_t quick_ieee11073_from_float(float temperature)
{
    uint8_t  exponent = 0xFF; //exponent is -1
    uint32_t mantissa = (uint32_t)(temperature*10);
    
    return ( ((uint32_t)exponent) << 24) | mantissa;
}

