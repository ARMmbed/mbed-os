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

nRF51822n   nrf;                /* BLE radio driver */

DigitalOut  led1(LED1);
DigitalOut  led2(LED2);
Ticker      flipper;
Serial      pc(USBTX,USBRX);

/* Battery Level Service */
uint8_t            batt = 72;     /* Battery level */
uint8_t            read_batt = 0; /* Variable to hold battery level reads */
GattService        battService ( GattService::UUID_BATTERY_SERVICE );
GattCharacteristic battLevel   ( GattCharacteristic::UUID_BATTERY_LEVEL_CHAR, 1, 1,
                                 GattCharacteristic::BLE_GATT_CHAR_PROPERTIES_NOTIFY |
                                 GattCharacteristic::BLE_GATT_CHAR_PROPERTIES_READ);

/* Heart Rate Service */
/* Service:  https://developer.bluetooth.org/gatt/services/Pages/ServiceViewer.aspx?u=org.bluetooth.service.heart_rate.xml */
/* HRM Char: https://developer.bluetooth.org/gatt/characteristics/Pages/CharacteristicViewer.aspx?u=org.bluetooth.characteristic.heart_rate_measurement.xml */
/* Location: https://developer.bluetooth.org/gatt/characteristics/Pages/CharacteristicViewer.aspx?u=org.bluetooth.characteristic.body_sensor_location.xml */
GattService        hrmService    ( GattService::UUID_HEART_RATE_SERVICE );
GattCharacteristic hrmRate       ( GattCharacteristic::UUID_HEART_RATE_MEASUREMENT_CHAR, 2, 3, GattCharacteristic::BLE_GATT_CHAR_PROPERTIES_NOTIFY );
GattCharacteristic hrmLocation   ( GattCharacteristic::UUID_BODY_SENSOR_LOCATION_CHAR, 1, 1, GattCharacteristic::BLE_GATT_CHAR_PROPERTIES_READ );

/* Device Information service */
uint8_t            deviceName[4] = { 'm', 'b', 'e', 'd' };
GattService        deviceInformationService ( GattService::UUID_DEVICE_INFORMATION_SERVICE );
GattCharacteristic deviceManufacturer ( GattCharacteristic::UUID_MANUFACTURER_NAME_STRING_CHAR,
                                        sizeof(deviceName), sizeof(deviceName),
                                        GattCharacteristic::BLE_GATT_CHAR_PROPERTIES_READ);

/* Health Thermometer Service */ 
uint32_t            temp_ieee11073 = 0xFF00016C; // 36.4C in IEEE-11073 32-bit float!
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
                                       GattService::UUID_HEART_RATE_SERVICE,
                                       GattService::UUID_HEALTH_THERMOMETER_SERVICE };

void tickerCallback(void);

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
    //virtual void onDataSent(uint16_t charHandle) {}
    //virtual void onDataWritten(uint16_t charHandle) {}
    
    virtual void onUpdatesEnabled(uint16_t charHandle)
    {
      if (charHandle == thermTemp.handle)
      {
        pc.printf("Temperature indication enabled\n\r");
      }
      if (charHandle == hrmRate.handle)
      {
        pc.printf("Heart rate notify enabled\n\r");
      }          
    }

    virtual void onUpdatesDisabled(uint16_t charHandle)
    {
      if (charHandle == thermTemp.handle)
      {
        pc.printf("Temperature indication disabled\n\r");
      }
      if (charHandle == hrmRate.handle)
      {
        pc.printf("Heart rate notify disabled\n\r");
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
    *(uint32_t *)0x40000504 = 0xC007FFDF;
    *(uint32_t *)0x40006C18 = 0x00008000;
    
    /* Setup blinky: led1 is toggled in main, led2 is toggled via Ticker */
    led1=1;
    led2=1;
    flipper.attach(&tickerCallback, 1.0);

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
    advData.addAppearance(GapAdvertisingData::HEART_RATE_SENSOR_HEART_RATE_BELT);
    nrf.getGap().setAdvertisingData(advData, scanResponse);

    /* Add the Battery Level service */
    battService.addCharacteristic(battLevel);
    nrf.getGattServer().addService(battService);

    /* Add the Device Information service */
    deviceInformationService.addCharacteristic(deviceManufacturer);
    nrf.getGattServer().addService(deviceInformationService);
    
    /* Add the Heart Rate service */
    hrmService.addCharacteristic(hrmRate);
    hrmService.addCharacteristic(hrmLocation);
    nrf.getGattServer().addService(hrmService);
    
    /* Health Thermometer Service */
    thermService.addCharacteristic(thermTemp);
    nrf.getGattServer().addService(thermService);

    /* Start advertising (make sure you've added all your data first) */
    nrf.getGap().startAdvertising(advParams);
    
    /* Now that we're live, update the battery level characteristic, and */
    /* change the device manufacturer characteristic to 'mbed' */
    nrf.getGattServer().updateValue(battLevel.handle, (uint8_t*)&batt, sizeof(batt));
    nrf.getGattServer().updateValue(deviceManufacturer.handle, deviceName, sizeof(deviceName));
    nrf.getGattServer().updateValue(thermTemp.handle, thermTempPayload, sizeof(thermTempPayload));

    /* Set the heart rate monitor location (one time only) */
    /* See --> https://developer.bluetooth.org/gatt/characteristics/Pages/CharacteristicViewer.aspx?u=org.bluetooth.characteristic.body_sensor_location.xml */
    uint8_t location = 0x03; /* Finger */
    uint8_t hrmCounter = 100;
    nrf.getGattServer().updateValue(hrmLocation.handle, (uint8_t*)&location, sizeof(location));

    /* Do blinky on LED1 while we're waiting for BLE events */
    for (;;)
    {
      led1 = !led1;
      wait(1);
      
      //nrf.getGattServer().readValue(battLevel.handle, (uint8_t*)&read_batt, sizeof(read_batt));
      //pc.printf("Battery Level = %d\n\r", read_batt);
      
      /* Update battery level */
      batt++;
      if (batt > 100) batt = 72;
      nrf.getGattServer().updateValue(battLevel.handle, (uint8_t*)&batt, sizeof(batt));

      /* Update the HRM measurement */
      /* First byte = 8-bit values, no extra info, Second byte = uint8_t HRM value */
      /* See --> https://developer.bluetooth.org/gatt/characteristics/Pages/CharacteristicViewer.aspx?u=org.bluetooth.characteristic.heart_rate_measurement.xml */
      hrmCounter++;
      if (hrmCounter == 175) hrmCounter = 100;
      uint8_t bpm[2] = { 0x00, hrmCounter };
      nrf.getGattServer().updateValue(hrmRate.handle, bpm, sizeof(bpm));
      
      /* Update the temperature */
      memcpy(thermTempPayload+1, &temp_ieee11073, 4);
      nrf.getGattServer().updateValue(thermTemp.handle, thermTempPayload, sizeof(thermTempPayload));
      temp_ieee11073++;
      if (temp_ieee11073 > 0xFF000200) temp_ieee11073 = 0xFF00016C; 
    }
}

/**************************************************************************/
/*!
    @brief  Ticker callback to switch led2 state
*/
/**************************************************************************/
void tickerCallback(void)
{
    led2 = !led2;
}
