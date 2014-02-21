/* mbed Microcontroller Library
 * Copyright (c) 2006-2013 ARM Limited
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
#include "nRF51822n.h"
#include "TMP102.h"

nRF51822n   nrf;                /* BLE radio driver */

DigitalOut  led1(LED1);
DigitalOut  led2(LED2);
Serial      pc(USBTX,USBRX);
TMP102      healthThemometer(p22, p20, 0x90);

/* Device Information service */
uint8_t            manufacturerName[4] = { 'm', 'b', 'e', 'd' };
GattService        deviceInformationService ( GattService::UUID_DEVICE_INFORMATION_SERVICE );
GattCharacteristic deviceManufacturer ( GattCharacteristic::UUID_MANUFACTURER_NAME_STRING_CHAR,
                                        sizeof(manufacturerName), sizeof(manufacturerName),
                                        GattCharacteristic::BLE_GATT_CHAR_PROPERTIES_READ);

/* Battery Level Service */
uint8_t            batt = 100;     /* Battery level */
uint8_t            read_batt = 0; /* Variable to hold battery level reads */
GattService        battService ( GattService::UUID_BATTERY_SERVICE );
GattCharacteristic battLevel   ( GattCharacteristic::UUID_BATTERY_LEVEL_CHAR, 1, 1,
                                 GattCharacteristic::BLE_GATT_CHAR_PROPERTIES_NOTIFY |
                                 GattCharacteristic::BLE_GATT_CHAR_PROPERTIES_READ);


/* Health Thermometer Service */ 
uint8_t             thermTempPayload[5] = { 0, 0, 0, 0, 0 };
GattService         thermService (GattService::UUID_HEALTH_THERMOMETER_SERVICE);
GattCharacteristic  thermTemp (GattCharacteristic::UUID_TEMPERATURE_MEASUREMENT_CHAR,
                               5, 5, GattCharacteristic::BLE_GATT_CHAR_PROPERTIES_INDICATE);

/* Advertising data and parameters */
GapAdvertisingData   advData;
GapAdvertisingData   scanResponse;
GapAdvertisingParams advParams ( GapAdvertisingParams::ADV_CONNECTABLE_UNDIRECTED );
uint16_t             uuid16_list[] = { GattService::UUID_BATTERY_SERVICE, 
                                       GattService::UUID_DEVICE_INFORMATION_SERVICE,
                                       GattService::UUID_HEALTH_THERMOMETER_SERVICE };

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
    virtual void onTimeout(void) 
    {
        pc.printf("Advertising Timeout!\n\r");
        // Restart the advertising process with a much slower interval,
        // only start advertising again after a button press, etc.
    }    

    virtual void onConnected(void)
    {
        pc.printf("Connected!\n\r");
    }

    virtual void onDisconnected(void)
    {
        pc.printf("Disconnected!\n\r");
        pc.printf("Restarting the advertising process\n\r");
        nrf.getGap().startAdvertising(advParams);
    }
};

/**************************************************************************/
/*!
    @brief  This custom class can be used to override any GattServerEvents
            that you are interested in handling on an application level.
*/
/**************************************************************************/
class GattServerEventHandler : public GattServerEvents
{
    //virtual void onDataSent(void) {}
    //virtual void onDataWritten(void) {}
    
    virtual void onUpdatesEnabled(uint16_t charHandle)
    {
        if (charHandle == thermTemp.handle)
        {
            pc.printf("Temperature indication enabled\n\r");
        }
    }
    
    virtual void onUpdatesDisabled(uint16_t charHandle)
    {
        if (charHandle == thermTemp.handle)
        {
            pc.printf("Temperature indication disabled\n\r");
        }
    }
    
    virtual void onConfirmationReceived(uint16_t charHandle)
    {
        if (charHandle == thermTemp.handle)
        {
            pc.printf("Temperature indication received\n\r");
        }
    }
};

/**************************************************************************/
/*!
    @brief  Program entry point
*/
/**************************************************************************/
int main(void)
{
    pc.baud(115200);
    
    /* Setup blinky: led1 is toggled in main, led2 is toggled via Ticker */
    led1=1;
    led2=1;

    /* Setup the local GAP/GATT event handlers */
    nrf.getGap().setEventHandler(new GapEventHandler());
    nrf.getGattServer().setEventHandler(new GattServerEventHandler());

    /* Initialise the nRF51822 */
    pc.printf("Initialising the nRF51822\n\r");
    nrf.init();

    /* Make sure we get a clean start */
    nrf.reset();    

    /* Add BLE-Only flag and complete service list to the advertising data */
    advData.addFlags(GapAdvertisingData::BREDR_NOT_SUPPORTED);
    advData.addData(GapAdvertisingData::COMPLETE_LIST_16BIT_SERVICE_IDS, 
                   (uint8_t*)uuid16_list, sizeof(uuid16_list));
    advData.addAppearance(GapAdvertisingData::GENERIC_THERMOMETER);
    nrf.getGap().setAdvertisingData(advData, scanResponse);

    /* Add the Battery Level service */
    battService.addCharacteristic(battLevel);
    nrf.getGattServer().addService(battService);

    /* Add the Device Information service */
    deviceInformationService.addCharacteristic(deviceManufacturer);
    nrf.getGattServer().addService(deviceInformationService);
    
    
    /* Health Thermometer Service */
    thermService.addCharacteristic(thermTemp);
    nrf.getGattServer().addService(thermService);

    /* Start advertising (make sure you've added all your data first) */
    nrf.getGap().startAdvertising(advParams);
    
    /* Now that we're live, update the battery level characteristic, and */
    /* change the device manufacturer characteristic to 'mbed' */
    nrf.getGattServer().updateValue(battLevel.handle, (uint8_t*)&batt, sizeof(batt));
    nrf.getGattServer().updateValue(deviceManufacturer.handle, manufacturerName, sizeof(manufacturerName));
    nrf.getGattServer().updateValue(thermTemp.handle, thermTempPayload, sizeof(thermTempPayload));


    for (;;)
    {
        wait(1);
        updateServiceValues();
    }
}

/**************************************************************************/
/*!
    @brief  Ticker callback to switch led2 state
*/
/**************************************************************************/
void updateServiceValues(void)
{
      /* Toggle the LED */
      led1 = !led1;
      
      /* Update battery level */
      nrf.getGattServer().updateValue(battLevel.handle, (uint8_t*)&batt, sizeof(batt));
      
      /* Update the temperature */
      float temperature = healthThemometer.read();
      uint32_t temp_ieee11073 = quick_ieee11073_from_float(temperature);
      memcpy(thermTempPayload+1, &temp_ieee11073, 4);
      nrf.getGattServer().updateValue(thermTemp.handle, thermTempPayload, sizeof(thermTempPayload));
      printf("Temperature: %f Celsius\r\n", temperature);
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
