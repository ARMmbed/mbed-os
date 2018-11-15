# HAL  PinMap

<!-- toc -->

- [Description](#Description)
- [Motivation](#Motivation)
- [Requirements](#Requirements)
- [Design choices](#Design-choices)
- [Questions](#Questions)

<!-- tocstop -->

## Description

This update aims at increasing tests thoroughness by enabling them to check driver's reliability for each pin declared as supporting a peripheral.

## Motivation

At the time being, drivers are only tested on a single "default" peripheral. However, some target features the same peripheral through different IP for example the SPI may be provided on a single MCU by its USART, QSPI and SSP peripherals. To ensure that the driver implementation is valid for all these peripherals we want the CI to assess each pin in at least one pin configuration for each peripheral.

## Requirements

1. We want to list all pins for a function on a peripheral.
2. We want to list all functions a pin can provide.
3. We want to list all pins for a form-factor regardless of the function they can provide.
4. We want a printable name for each pin and a method to get that name.
5. We want a list the reserved pins that cannot be tested.
6. We want to select a duplicate-free set a pin from a set of PinMap.

Any of the list mentioned above may be empty if none match the criteria.

## Design choices

These requirements do not impact the current API and thus they can all be implemented as an extension to the existing API.   
It is proposed to introduce the following elements :
- `DEVICE_PIN_EXTENDED_SUPPORT` A `device_has` flag indicating the implementation of this extension.
- The following part of the API provides the features needed for requirements 3, 4 and 5
  ```c
  enum pinmap_form_factor_ {
      PINMAP_FORM_FACTOR_ARDUINO_ZERO,
      PINMAP_FORM_FACTOR_ARDUINO_DUE,
      PINMAP_FORM_FACTOR_NXP_FRDM,
      PINMAP_FORM_FACTOR_ST_MORPHO,
      PINMAP_FORM_FACTOR_MTB,
  } pinmap_form_factor_t;

  /// returns a static NC terminated array of pins.
  const PinName *pinmap_form_factor_pins(pinmap_form_factor_t form_factor);
  /// returns a static NC terminated array of pins.
  const PinName *pinmap_restricted_pins();
  /// returns a null (\0) terminated static character array representing the name of the pin.
  const char *pinmap_pin_to_string(PinName pin);
  ```
- These generic function make use of the existing API to answer requirements 1, 2 and 6.
  ```c
  /// Returns the `n`'th pin supporting the `peripheral` in the `map`.
  PinName pinmap_find_peripheral_pin(const PinMap *map, int peripheral, uint32_t n);

  /// Returns a pin from ((whitelist ∩ map) − blacklist).
  /// Typically, whitelist will be the form factor list of pins.
  /// The blacklist may be a list of pins already visited and/or tied to another peripheral.
  PinName pinmap_find_peripheral_pin(const PinMap *map, const PinName *whitelist,
                                      const PinName *blacklist, int per);

  /// Verifies that `pin` is in `list`.
  bool pinlist_has_pin(const PinName *list, PinName pin);

  /// Applies pinmap_find_peripheral_pin to each map in maps ensuring a pin will not be used twice.
  /// @param[in]  maps      A list of pinmap.
  /// @param[in]  count     Number of elements of maps and pins.
  /// @param[in]  whitelist An NC terminated list of pins.
  /// @param[in]  blacklist An NC terminated list of pins.
  /// @param[in]  per       A peripheral name.
  /// @param[out] pins      An array of PinName where the result is stored.
  bool pinmap_find_peripheral_pins(const PinMap const **maps,
                                   uint32_t count,
                                   const PinName *whitelist,
                                   const PinName *blacklist,
                                   uint32_t per,
                                   PinName *pins);
  ```
- Additionally to these requirements any peripheral API must expose adequate functions to fetch an NC terminated list of pins associated to each of their their functions. These API extensions shall be gated with a `<PERIPHERAL_NAME>_PIN_EXTENTION` where `<PERIPHERAL_NAME>` is the name of the considered peripheral (`SPI`, `I2C`, `CAN` etc).
  
  For example the SPI api may be extended with :
  ```c
  #ifdef DEVICE_SPI_PIN_EXTENDED_SUPPORT
  PinMap *spi_get_mosi_pins(bool as_slave);
  PinMap *spi_get_miso_pins(bool as_slave);
  PinMap *spi_get_clk_pins();
  PinMap *spi_get_cs_pins();
  #endif
  ```

  and the I²C api with :
  ```c
  #ifdef DEVICE_I2C_PIN_EXTENDED_SUPPORT
  PinMap *i2c_get_scl_pins();
  PinMap *i2c_get_sda_pins();
  #endif
  ```
  

## Questions

