#include <port.h>

/**
 *  \brief Writes a Port pin configuration to the hardware module.
 *
 *  Writes out a given configuration of a Port pin configuration to the hardware
 *  module.
 *
 *  \note If the pin direction is set as an output, the pull-up/pull-down input
 *        configuration setting is ignored.
 *
 *  \param[in] gpio_pin  Index of the GPIO pin to configure
 *  \param[in] config    Configuration settings for the pin
 */
void port_pin_set_config(
    const uint8_t gpio_pin,
    const struct port_config *const config)
{
    /* Sanity check arguments */
    Assert(config);

    struct system_pinmux_config pinmux_config;
    system_pinmux_get_config_defaults(&pinmux_config);

    pinmux_config.mux_position = SYSTEM_PINMUX_GPIO;
    pinmux_config.direction    = (enum system_pinmux_pin_dir)config->direction;
    pinmux_config.input_pull   = (enum system_pinmux_pin_pull)config->input_pull;
    pinmux_config.powersave    = config->powersave;

    system_pinmux_pin_set_config(gpio_pin, &pinmux_config);
}

/**
 *  \brief Writes a Port group configuration group to the hardware module.
 *
 *  Writes out a given configuration of a Port group configuration to the
 *  hardware module.
 *
 *  \note If the pin direction is set as an output, the pull-up/pull-down input
 *        configuration setting is ignored.
 *
 *  \param[out] port    Base of the PORT module to write to
 *  \param[in]  mask    Mask of the port pin(s) to configure
 *  \param[in]  config  Configuration settings for the pin group
 */
void port_group_set_config(
    PortGroup *const port,
    const uint32_t mask,
    const struct port_config *const config)
{
    /* Sanity check arguments */
    Assert(port);
    Assert(config);

    struct system_pinmux_config pinmux_config;
    system_pinmux_get_config_defaults(&pinmux_config);

    pinmux_config.mux_position = SYSTEM_PINMUX_GPIO;
    pinmux_config.direction    = (enum system_pinmux_pin_dir)config->direction;
    pinmux_config.input_pull   = (enum system_pinmux_pin_pull)config->input_pull;
    pinmux_config.powersave    = config->powersave;

    system_pinmux_group_set_config(port, mask, &pinmux_config);
}
