/*
 * Copyright (c) 2019, Arm Limited and affiliates.
 * SPDX-License-Identifier: Apache-2.0
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#ifndef MBED_TESTER_H
#define MBED_TESTER_H

#include "DynamicPinList.h"
#include "platform/FileHandle.h"
#include "platform/Callback.h"
#include "drivers/DigitalInOut.h"

/**
 * The base class for controlling the FPGA CI Test Shield
 *
 * This is the primary interface to the FPGA CI Test Shield. It contains
 * all the code to communicate with the FPGA. It also provides high level
 * helper functions, such as functions to setup pin multiplexing, to
 * select the currently active peripheral and to perform software updates.
 *
 * Subclasses can inherit from this class and provide further functionality,
 * such as the ability to test SPI.
 *
 * @note Synchronization level: Not protected
 *
 * Example of how to toggle Arduino pin D6 from the FPGA cI Test Shield:
 * @code
 * #include "mbed.h"
 * #include "MbedTester.h"
 *
 * const PinList *form_factor = pinmap_ff_default_pins();
 * const PinList *restricted = pinmap_restricted_pins();
 * MbedTester tester(form_factor, restricted);
 *
 * int main() {
 *     // Reset the FPGA CI Test Shield to put it into a known state
 *     tester.reset();
 *
 *     // Select the GPIO peripheral
 *     tester.select_peripheral(MbedTester::PeripheralGPIO);
 *
 *     // Map D6 to LogicalPinGPIO0
 *     tester.pin_map_set(D6, MbedTester::LogicalPinGPIO0);
 *
 *     // Toggle pin D6
 *     int toggle = 0;
 *     while (1) {
 *         tester.gpio_write(MbedTester::LogicalPinGPIO0, toggle, true);
 *         ThisThread::sleep_for(500);
 *         toggle = !toggle;
 *     }
 * }
 * @endcode
 */
class MbedTester {
public:

    /*
     * This type represents the index of a physical pin on the FPGA.
     * This can be any value from 0 to 127. A form factor is used to
     * map a PinName to a physical pin index.
     */
    typedef uint8_t PhysicalIndex;

    /*
     * There are 8 logical pins connected to the peripherals inside the FPGA.
     * These are logical pins 0 through 7. All peripherals can read from these
     * pins and the currently active peripheral can output on these pins.
     *
     * Each logical pin has a fixed function for a given peripheral which is
     * why the same value is defined multiple times. For example logical pin
     * 2 is SPI clock (SPISclk) when SPI is the active peripheral and
     * UART clear to send (UARTCts) when UART is the active peripheral.
     *
     * A logic pin can be mapped to any physical pin (PinName type) by calling
     * the function MbedTester::pin_map_set.
     */
    enum LogicalPin {

        LogicalPinGPIO0 = 0,
        LogicalPinGPIO1 = 1,
        LogicalPinGPIO2 = 2,
        LogicalPinGPIO3 = 3,
        LogicalPinGPIO4 = 4,
        LogicalPinGPIO5 = 5,
        LogicalPinGPIO6 = 6,
        LogicalPinGPIO7 = 7,

        LogicalPinSPIMosi = 0,
        LogicalPinSPIMiso = 1,
        LogicalPinSPISclk = 2,
        LogicalPinSPISsel = 3,

        LogicalPinIOMetrics0 = 0,
        LogicalPinIOMetrics1 = 1,
        LogicalPinIOMetrics2 = 2,
        LogicalPinIOMetrics3 = 3,
        LogicalPinIOMetrics4 = 4,
        LogicalPinIOMetrics5 = 5,
        LogicalPinIOMetrics6 = 6,
        LogicalPinIOMetrics7 = 7,

        LogicalPinUARTTx = 0,
        LogicalPinUARTRx = 1,
        LogicalPinUARTCts = 2,
        LogicalPinUARTRts = 3,

        LogicalPinI2CSda = 0,
        LogicalPinI2CScl = 1,

        LogicalPinCount = 8,
        LogicalPinBanks = 2,
        LogicalPinTotal = LogicalPinCount * LogicalPinBanks
    };

    /*
     * These are the peripherals internal to the FPGA. A peripheral can be
     * selected by calling MbedTester::select_peripheral.
     */
    enum Peripheral {
        PeripheralGPIO = 1,
        PeripheralSPI = 2,
        PeripheralUART = 4,
        PeripheralI2C = 5,
        PeripheralSPISlave = 6
    };

    /**
     * Construct a new MbedTester object
     *
     * The form factor pins passed into this class must match the
     * physical MbedTester shield connected to this board or testing
     * will not work correctly. The order of pins in this list must
     * match the order of the shield.
     *
     * The exclude pins list should be used to exclude pins which you either
     * don't want to be tested or are not suitable for use as a control
     * channel. This list is allowed to contain pins that are not in the
     * form factor.
     *
     * @param form_factor The pins that are available on this form factor
     * @param exclude_pins The pins that must not be used
     */
    MbedTester(const PinList *form_factor, const PinList *exclude_pins);

    /**
     * Destroy and cleanup resources associated with this object
     */
    ~MbedTester();

    /**
     * Enable automatic selection and update of control pins
     *
     * Calling this function configures MbedTester to automatically select
     * and update the control pins. The control pins are moved if the
     * function MbedTester::pin_map_set is called and maps a pin that is being used
     * for control.
     *
     * @note Automatic selection and update of control pins is the default.
     * Unless MbedTester::set_control_pins_manual has been called to manually
     * set the control pins this function has no effect.
     */
    void set_control_pins_auto();

    /**
     * Set the control pins to use for communication
     *
     * Manually set the control pins. Calling this function
     * disables automatic control pin selection and updates.
     * The function MbedTester::pin_map_set must not be used to map over
     * control pins when in this mode.
     *
     * @param clk Clock pin to use as the control channel
     * @param mosi Mosi pin to use as the control channel
     * @param miso Miso pin to use as the control channel
     * @param aux Auxillary pin to use as the control cannel
     */
    void set_control_pins_manual(PinName clk, PinName mosi, PinName miso, PinName aux);

    /**
     * Read FPGA CI Test Shield firmware
     *
     * Read the firmware on the FPGA CI Test Shield. An optional progress callback
     * can be supplied to display progress while the firmware is being read.
     *
     * @param dest File to write the firmware to. This file must have been opened as writeable
     * @param progress Optional progress callback called when the percent complete changes
     * @return true if firmware was successfully read, false otherwise
     */
    bool firmware_dump(mbed::FileHandle *dest, mbed::Callback<void(uint8_t)> progress = mbed::Callback<void(uint8_t)>());

    /**
     * Read FPGA CI Test Shield flash
     *
     * Read the entire flash contents of the FPGA CI Test Shield. An optional progress callback
     * can be supplied to display progress while the firmware is being read.
     *
     * @param dest File to write the firmware to. This file must have been opened as writeable
     * @param progress Optional progress callback called when the percent complete changes
     * @return true if firmware was successfully read, false otherwise
     */
    bool firmware_dump_all(mbed::FileHandle *dest, mbed::Callback<void(uint8_t)> progress = mbed::Callback<void(uint8_t)>());

    /**
     * Program new FPGA CI Test Shield firmware
     *
     * Program firmware from the file given. The binary bitstream must be in the correct format
     * and contain a correct CRC or the update will fail. To correctly format a bitstream binary
     * the post_process_bitstream.py script in the fpga-ci-test-shield repository should be used.
     *
     * Note - release binaries for the FPGA CI Test Shield have already been formatted and
     *        can be loaded directly with this function
     *
     * @param src File containing the new firmware to program
     * @param progress Optional progress callback called when the percent complete changes
     * @return true if firmware was successfully applied, false otherwise
     */
    bool firmware_update(mbed::FileHandle *src, mbed::Callback<void(uint8_t)> progress = mbed::Callback<void(uint8_t)>());

    /**
     * Map a physical pin to the given logical pin
     *
     * This function will automatically move the control channel
     * pins to avoid interfering with the mapped pin.
     *
     * @param physical Physical pin on the board
     * @param logical Logical pin to map to
     */
    void pin_map_set(PinName physical, LogicalPin logical);

    /**
     * Reset all pin mappings
     *
     * After this call all pins will be unmapped
     */
    void pin_map_reset();

    /**
     * Reset all peripherals
     *
     * This does not reset the pin mappings
     */
    void peripherals_reset();

    /**
     * Reset everything
     *
     * This function resets the state of both the FPGA CI Test Shield
     * and the MbedTester object itself.
     *
     * Reset effects on the FPGA CI Test Shield include:
     * - All pins are tristated
     * - All pin mappings are reset
     * - All pullup/pulldown settings are reset
     * - All peripherals are reset
     *
     * Reset effects on the MbedTester object include
     * - Control channels tristated and freed
     * - Control channel selection set to automatic
     * - Most internal state reinitialized
     */
    void reset();

    /**
     * Reprogram the FPGA
     *
     * This function causes the FPGA to reboot and reload RAM contents.
     * This should be used after MbedTester::firmware_update to load the
     * new image.
     */
    void reprogram();

    /**
     * Get the running FPGA firmware version
     *
     * @return The version of firmware running on the FPGA.
     *
     */
    uint32_t version();

    /**
     * Select the currently active peripheral
     *
     * @param peripheral Active peripheral
     */
    void select_peripheral(Peripheral peripheral);

    /* **GPIO peripheral functions** */

    /**
     * Read a gpio pin
     *
     * @param gpio Logical pin to read from
     * @return 1 if the pin is high 0 if the pin is low
     */
    int gpio_read(LogicalPin gpio);

    /**
     * Set value and drive of a gpio pin
     *
     * @param gpio Logical pin to write to
     * @param value 0 to set the pin low or non-zero to set it high
     * @param driver 0 to set the pin to Hi-Z or non-zero to drive the pin
     */
    void gpio_write(LogicalPin gpio, int value, bool drive);

    /* **IO Metrics functions** */

    /**
     * Start recording metrics on all logical pins
     *
     * This function resets all past metrics to 0. To
     * preserve these call io_metrics_continue instead.
     */
    void io_metrics_start();

    /**
     * Stop recording metrics on all logical pins
     *
     * This function should be called before any metrics
     * are read to ensure the value does not change while
     * they are being read.
     */
    void io_metrics_stop();

    /**
     * Continue recording metrics on all logical pins
     *
     * Resume recording metrics.
     */
    void io_metrics_continue();

    /**
     * Get the shortest low pulse recorded
     *
     * @param pin Pin to read the metrics for
     * @return The shortest number of 100MHz clock cycles the pin was low
     */
    uint32_t io_metrics_min_pulse_low(LogicalPin pin);

    /**
     * Get the shortest high pulse recorded
     *
     * @param pin Pin to read the metrics for
     * @return The shortest number of 100MHz clock cycles the pin was high
     */
    uint32_t io_metrics_min_pulse_high(LogicalPin pin);

    /**
     * Get the longest low pulse recorded
     *
     * @param pin Pin to read the metrics for
     * @return The longest number of 100MHz clock cycles the pin was low
     */
    uint32_t io_metrics_max_pulse_low(LogicalPin pin);

    /**
     * Get the longest high pulse recorded
     *
     * @param pin Pin to read the metrics for
     * @return The longest number of 100MHz clock cycles the pin was high
     */
    uint32_t io_metrics_max_pulse_high(LogicalPin pin);

    /**
     * Get the number of rising edges
     *
     * @param pin Pin to read the metrics for
     * @return The number of rising edges
     */
    uint32_t io_metrics_rising_edges(LogicalPin pin);

    /**
     * Get the number of falling edges
     *
     * @param pin Pin to read the metrics for
     * @return The number of falling edges
     */
    uint32_t io_metrics_falling_edges(LogicalPin pin);

    /**
     * Reset the IO expander modules
     *
     */
    void pin_pull_reset_all();

    /**
     * FPGA Pullup mode
     */
    enum PullMode {
        PullUp,
        PullDown,
        PullNone
    };

    /**
     * Configure an Mbed pin for a pulldown resistor, pullup resistor, or tristate mode via PinName
     *
     * @param pin Mbed pin whose mode is being set
     * @param mode (MbedTester::PullUp, MbedTester::PullDown, or MbedTester::PullNone)
     * @return 0 on success, nonzero on failure
     */
    int pin_set_pull(PinName pin, PullMode mode);

    /**
     * Configure an Mbed pin for a pulldown resistor, pullup resistor, or tristate mode via pin index
     *
     * @param index Mbed pin index whose mode is being set
     * @param mode (MbedTester::PullUp, MbedTester::PullDown, or MbedTester::PullNone)
     * @return 0 on success, nonzero on failure
     */
    int pin_set_pull_index(int index, PullMode mode);

    /*
     * Register of the IO expander
     */
    enum IOExpanderReg {
        RegInput,
        RegOutput,
        RegConfig
    };

    /**
     * Read a bit for a specific Mbed pin that is set in the input, output, or configuration registers inside of the IO expander via PinName
     *
     * @param pin Mbed pin whose register bit is being read
     * @param reg_type Pin register to access, options are: MbedTester::RegInput, MbedTester::RegOutput, or MbedTester::RegConfig
     * @return The value of the bit read
     */
    uint8_t io_expander_read(PinName pin, IOExpanderReg reg_type);

    /**
     * Read a bit for a specific Mbed pin that is set in the input, output, or configuration registers inside of the IO expander via pin index
     *
     * @param index Mbed pin index whose register bit is being read
     * @param reg_type Pin register to access, options are: MbedTester::RegInput, MbedTester::RegOutput, or MbedTester::RegConfig
     * @return The value of the bit read
     */
    uint8_t io_expander_read_index(int index, IOExpanderReg reg_type);

    /**
     * Configure an Mbed pin for a pulldown resistor, pullup resistor, or tristate mode
     * (this version of the function uses io_expander_i2c_read_bb and io_expander_i2c_write_bb)
     *
     * @param pin Mbed pin whose mode is being set
     * @param mode (MbedTester::PullUp, MbedTester::PullDown, or MbedTester::PullNone)
     * @return 0 on success, nonzero on failure
     */
    int pin_set_pull_bb(PinName pin, PullMode mode);

    /**
     * Read a bit for a specific Mbed pin that is set in the input, output, or configuration registers inside of the IO expander
     * (this version of the function uses io_expander_i2c_read_bb)
     *
     * @param pin Mbed pin whose register bit is being read
     * @param reg_type Pin register to access, options are: MbedTester::RegInput, MbedTester::RegOutput, or MbedTester::RegConfig
     * @return The value of the bit read
     */
    uint8_t io_expander_read_bb(PinName pin, IOExpanderReg reg_type);

    /**
     * Create an analog voltage via the FPGA sys pwm in order to test Mbed AnalogIn
     *
     * @param enable Enable the FPGA system PWM (false: of, true: on)
     * @param voltage The analog voltage that will be created by the FPGA CI test shield (float: 0.0 to 1.0)
     */
    void set_analog_out(bool enable, float voltage);

    /**
     * Turn the FPGA ADC on and off (power management data will be collected while the ADC is on)
     *
     * @param val FPGA ADC enable bit (false: off, true: on)
     */
    void set_sample_adc(bool val);

    /**
     * Get the result of the analog to digital conversion computed on the FPGA in the form of a voltage reading. The FPGA ADC operates on 0V-1V, which means this function will only ever return a float ranging from 0.0-1.0.
     *
     * @return The conversion result in the form of a voltage measurement for AnalogMuxIn, Eg. a return value of 0.7 means the ADC on the FPGA read 0.7 volts on its analog input
     */
    float get_analog_in();

    /**
     * Similar to 'get_analog_in' but returns a voltage reading from ANIN0-3
     *
     * @param index ANIN pin to read (0-3)
     * @return The conversion result in the form of a voltage measurement for ANIN0-3
     */
    float get_anin_voltage(int index);

    /* **Mid level system access** */

    /*
     * These are the pins on the FPGA that are not connected to the Mbed board.
     * These can be controlled by using MbedTester::sys_pin_read and MbedTester::sys_pin_write.
     * These should not be used directly when using the FPGA CI Test Shield. The higher layer
     * APIs should be used instead.
     */
    enum SystemPin {
        Reset,
        Reprogram,

        DigitalID0,
        DigitalID1,
        DigitalID2,

        Led0,
        Led1,
        Led2,
        Led3,

        SPIIO0,
        SPIIO1,
        SPIIO2,
        SPIIO3,
        SPIClk,
        SPICS,

        I2CReset,
        I2CSda0,
        I2CSda1,
        I2CSda2,
        I2CScl0,
        I2CScl1,
        I2CScl2,

        AnalogMuxEnable,
        AnalogMuxPwmOut,
        AnalogMuxIn,
        AnalogMuxAddr0,
        AnalogMuxAddr1,
        AnalogMuxAddr2,
        AnalogMuxAddr3,
        AnalogMuxAddr4,
        AnalogMuxAddr5,
        AnalogMuxAddr6,
        AnalogMuxAddr7,

        AnalogInP0,
        AnalogInP1,
        AnalogInP2,
        AnalogInP3,
        AnalogInN0,
        AnalogInN1,
        AnalogInN2,
        AnalogInN3,

        SystemPinCount
    };

    /**
     * Read from the given system pin
     *
     * @param pin The pin to read from
     * @return true if 1 was read, false if 0
     */
    bool sys_pin_read(SystemPin pin);

    /**
     * Write to the given system pin
     *
     * @param pin The pin to write to
     * @param value The value to output on the pin when driven
     * @param true to drive the output, false to set the output high-z
     * @return true if 1 was read, false if 0
     */
    void sys_pin_write(SystemPin pin, int value, bool drive);

    /**
     * I2C read on I2C system channels 0, 1, or 2
     *
     * @param i2c_index The number corresponding to the system i2c bus being used (0, 1, or 2)
     * @param dev_addr The I2C address of the device being read from
     * @param start_reg The internal device address where the read will start
     * @param data Data buffer for data to be read into
     * @param length The number of bytes to read
     * @return 0 on success (ACK), nonzero on failure (NACK)
     */
    int io_expander_i2c_read(uint8_t i2c_index, uint8_t dev_addr, uint8_t start_reg, uint8_t *data, int length);

    /**
     * I2C write on I2C system channels 0, 1, or 2
     *
     * @param i2c_index The number corresponding to the system i2c bus being used (0, 1, or 2)
     * @param dev_addr The I2C address of the device being written to
     * @param data The data to be written
     * @param length The number of bytes to be written
     * @return 0 on success (ACK), nonzero on failure (NACK)
     */
    int io_expander_i2c_write(uint8_t i2c_index, uint8_t dev_addr, uint8_t *data, int length);

    /**
     * I2C read on I2C system channels 0, 1, or 2
     * (bit banged version of function, bit banged over control channel)
     *
     * @param sda System pin used for sda
     * @param scl System pin used for scl
     * @param dev_addr The I2C address of the device being read from
     * @param start_reg The internal device address where the read will start
     * @param data Data buffer for data to be read into
     * @param length The number of bytes to read
     * @return 0 on success (ACK), nonzero on failure (NACK)
     */
    int io_expander_i2c_read_bb(SystemPin sda, SystemPin scl, uint8_t dev_addr, uint8_t start_reg, uint8_t *data, int length);

    /**
     * I2C write on I2C system channels 0, 1, or 2
     * (bit banged version of function, bit banged over control channel)
     *
     * @param sda System pin used for sda
     * @param scl System pin used for scl
     * @param dev_addr The I2C address of the device being written to
     * @param data The data to be written
     * @param length The number of bytes to be written
     * @return 0 on success (ACK), nonzero on failure (NACK)
     */
    int io_expander_i2c_write_bb(SystemPin sda, SystemPin scl, uint8_t dev_addr, uint8_t *data, int length);

    /**
     * Set the AnalogMuxAddr pins on the FPGA via PinName
     *
     * @param pin The Mbed pin that the analog signal will be routed to
     * @return 0 on success, nonzero on failure
     */
    int set_mux_addr(PinName pin);

    /**
     * Set the AnalogMuxAddr pins on the FPGA via pin index
     *
     * @param index The Mbed pin index that the analog signal will be routed to
     * @return 0 on success, nonzero on failure
     */
    int set_mux_addr_index(int index);

    /* **AnalogIn peripheral functions** */

    /**
     * Turn on/off the analog muxes
     *
     * @param val false: off, true: on
     */
    void set_mux_enable(bool val);

    /**
     * Turn on/off pwm output on FPGA to test Mbed AnalogIn
     *
     * @param val false: off, true: on
     */
    void set_pwm_enable(bool val);

    /**
     * Check if FPGA pwm out is on or off
     *
     * @return FPGA enable bit (false: off, true: on)
     */
    bool get_pwm_enable();

    /**
     * Set the pwm output period and number of cycles high (duty cycle) on the FPGA
     *
     * @param period In units of clk cycles
     * @param cycles_high In units of clk cycles
     */
    void set_pwm_period_and_cycles_high(uint32_t period, uint32_t cycles_high);

    /**
     * Get the pwm output period of the FPGA
     *
     * @return FPGA pwm output period in units of clk cycles
     */
    uint32_t get_pwm_period();

    /**
     * Get the number of cycles that are high (duty cycle) from FPGA pwm
     *
     * @return FPGA pwm output cycles high
     */
    uint8_t get_pwm_cycles_high();

    /**
     * Get the 12-bit analog to digital conversion result from the FPGA
     *
     * @return 12-bit FPGA ADC result for AnalogMuxIn
     */
    uint16_t get_analogmuxin_measurement();

    /**
     * Similar to 'get_analogmuxin_measurement' but returns the current XADC measurement for ANIN0-3
     *
     * @param index ANIN pin to read (0-3)
     * @return 12-bit FPGA ADC result for ANIN0-3
     */
    uint16_t get_anin_measurement(int index);

    /**
     * Gets (by reference) the sum of all ANIN ADC results for any of the 4 ANIN pins specified by the index,
     * number of ADC sample sequences that have completed since the XADC was turned on, and the number of FPGA clk cycles
     * that have taken place since the ADC was turned on.
     *
     * @param index ANIN pin of which to get sum of results (0-3)
     * @param sum The sum of all specified ANIN pin's ADC results
     * @param samples The number of ADC sample sequences that have completed since the XADC was turned on
     * @param cycles The number of FPGA clk cycles that have taken place since the ADC was turned on
     */
    void get_anin_sum_samples_cycles(int index, uint64_t *sum, uint32_t *samples, uint64_t *cycles);

    /**
     * Allows safe reading of FPGA ADC related values while the FPGA ADC is on
     * If snapshot is set then the ADC values will be safely latched in the FPGA and safe to read.
     * The RTL will set snapshot to 0 after 1 clk cycle.
     */
    void set_snapshot();

    /**
     * Set the current system pin mode to disabled
     *
     * This releases any pin mappings that were set by the previous pin mode.
     */
    void sys_pin_mode_disabled();

    /**
     * Set the current system pin mode to serial flash
     *
     * Remap physical pins to the serial flash the FPGA boots from.
     * This is used for firmware updates.
     *
     * @param mosi The physical pin index to connect to serial flash mosi
     * @param miso The physical pin index to connect to serial flash miso
     * @param clk The physical pin index to connect to serial flash clk
     * @param ssel The physical pin index to connect to serial flash cs
     */
    void sys_pin_mode_spi_serial_flash(PhysicalIndex mosi, PhysicalIndex miso, PhysicalIndex clk, PhysicalIndex ssel);

    /**
     * Set the current system pin mode to io expander I2C bus
     *
     * Remap physical pins to the io expander I2C bus. The IO expanders
     * are used for setting pullups and pulldowns.
     *
     * @param index The index of the I2C bus to connect to
     * @param sda_in Physical pin index for the FPGA to output the state of SDA on
     * @param sda_val Physical pin index for the FPGA to read SDA from. When in this mode the Mbed board
     *                must always drive this pin. Driving a 0 causes the FPGA to pull the SDA on the I2C bus low. Setting
     *                a 1 causes the FPGA to let SDA on the I2C bus float (and get pulled to 1).
     * @param scl_in Physical pin index for the FPGA to output the state of SCL on
     * @param scl_val Physical pin index for the FPGA to read SCL from. When in this mode the Mbed board
     *                must always drive this pin. Driving a 0 causes the FPGA to pull the SCL on the I2C bus low. Setting
     *                a 1 causes the FPGA to let SDA on the SCL bus float (and get pulled to 1).
     */
    void sys_pin_mode_i2c_io_expander(int index, PhysicalIndex sda_in, PhysicalIndex sda_val, PhysicalIndex scl_in, PhysicalIndex scl_val);

    /**
     * Map a physical pin index to the given logical pin
     *
     * This function will automatically move the control channel
     * pins to avoid interfering with the mapped pin. The physical
     * pin index does not need to be part of the form factor.
     *
     * @param physical_index Index of the physical pin on the board
     * @param logical Logical pin to map to
     */
    void pin_map_index(PhysicalIndex physical_index, LogicalPin logical);


    /* **Low level memory access** */

    /**
     * Write to tester memory
     *
     * @addr addr Address to write to
     * @param data Data to write
     * @param size Number of bytes to write
     */
    void write(uint32_t addr, const uint8_t *data, uint32_t size);

    /**
     * Read from tester memory
     *
     * @addr addr Address to read from
     * @param data Buffer to fill with data
     * @param size Number of bytes to read
     */
    void read(uint32_t addr, uint8_t *data, uint32_t size);

    /* **Self tests** */

    /**
     * Run all self tests
     *
     * @return true if all self tests pass, false otherwise
     */
    bool self_test_all();

    /**
     * Test that all allowed control channels can be used
     *
     * Check that all pairs of clk and mosi which aren't in
     * the restricted list can be used.
     *
     * @note CLK and MOSI lines are paired, where CLK is always
     * on an even index and MOSI is always on an oddd index:
     * clk_index_N = N * 2
     * mosi_index_N = N * 2 + 1
     *
     * @note This functions sets the control pin management mode
     * to automatic when it completes.
     *
     * @return true if all control channel pairs (clk and mosi) of this
     *         configuration can be used, false otherwise
     */
    bool self_test_control_channels();

    /**
     * Test that all allowed control miso lines can be used
     *
     * Check that every pin of this form factor aside from the
     * pins in the restricted list can be used as miso.
     *
     * @note This functions sets the control pin management mode
     * to automatic when it completes.
     *
     * @return true if all control channel miso lines of this
     *         configuration can be used, false otherwise
     */
    bool self_test_control_miso();

    /**
     * Test that the current control channel works
     *
     * @return true if communication is successful, false otherwise
     */
    bool self_test_control_current();

private:

    /*
     * Find suitable control pins
     *
     * This function finds the appropriate set of pins and test to ensure that communication with
     * the FPGA can be performed over them. Before calling this function MbedTester pins must be
     * freed by calling MbedTester::_free_control_pins.
     *
     * @param clk_out Clock pin to find and set
     * @param mosi_out Mosi pin to find and set
     * @param miso_out Miso pin to find and set
     * @param aux_out Aux pin to find and set
     * @return true if all pins were found, false otherwise
     */
    bool _find_control_indexes(PhysicalIndex &clk_out, PhysicalIndex &mosi_out, PhysicalIndex &miso_out, PhysicalIndex &aux_out);

    /*
     * Allocate control pins
     *
     * The pin indexes must already have been found
     */
    void _setup_control_pins();

    /*
     * Free control pins
     *
     * This function is safe to call even if the pins have been freed
     */
    void _free_control_pins();

    /*
     * Update the control channel if needed
     *
     * Open a control channel using allowed pins:
     * - Pin must be in the form factor
     * - Pin must not be in the exclude list
     * - Pin must not be mapped already
     */
    void _update_control_pins();

    /*
     * Check if this control channel needs to be updated
     *
     * @return true if the control channel needs to be updated, false otherwise
     */
    bool _update_needed();

    /*
     * Reset the state of the MbedTester class
     *
     * This does not effect the state of the FPGA in any way.
     */
    void _reset();

    PhysicalIndex _mapping[LogicalPinCount * (LogicalPinBanks + 1)];
    DynamicPinList _form_factor;
    DynamicPinList _exclude_pins;
    bool _control_auto;
    bool _control_valid;
    PhysicalIndex _clk_index;
    PhysicalIndex _mosi_index;
    PhysicalIndex _miso_index;
    PhysicalIndex _aux_index;
    mbed::DigitalInOut *_clk;
    mbed::DigitalInOut *_mosi;
    mbed::DigitalInOut *_miso;
    mbed::DigitalInOut *_aux;
    /*
     * Used to reset IO expander chips only once the first time
     * any IO expander is accessed as well as during an io_exp_test
     */
    int _init_io_exp_rst_flag;
};

#endif
