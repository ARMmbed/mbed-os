#ifndef SYSTEM_CLOCK_GCLK_H_INCLUDED
#define SYSTEM_CLOCK_GCLK_H_INCLUDED

/**
 * \addtogroup asfdoc_sam0_system_clock_group
 *
 * @{
 */

#include <compiler.h>

#ifdef __cplusplus
extern "C" {
#endif

/**
 * \brief List of available GCLK generators.
 *
 * List of Available GCLK generators. This enum is used in the peripheral
 * device drivers to select the GCLK generator to be used for its operation.
 *
 * The number of GCLK generators available is device dependent.
 */
enum gclk_generator {
    /** GCLK generator channel 0. */
    GCLK_GENERATOR_0,
#if defined(__DOXYGEN__) || (GCLK_GEN_NUM_MSB > 0)
    /** GCLK generator channel 1. */
    GCLK_GENERATOR_1,
#endif
#if defined(__DOXYGEN__) || (GCLK_GEN_NUM_MSB > 1)
    /** GCLK generator channel 2. */
    GCLK_GENERATOR_2,
#endif
#if defined(__DOXYGEN__) || (GCLK_GEN_NUM_MSB > 2)
    /** GCLK generator channel 3. */
    GCLK_GENERATOR_3,
#endif
#if defined(__DOXYGEN__) || (GCLK_GEN_NUM_MSB > 3)
    /** GCLK generator channel 4. */
    GCLK_GENERATOR_4,
#endif
#if defined(__DOXYGEN__) || (GCLK_GEN_NUM_MSB > 4)
    /** GCLK generator channel 5. */
    GCLK_GENERATOR_5,
#endif
#if defined(__DOXYGEN__) || (GCLK_GEN_NUM_MSB > 5)
    /** GCLK generator channel 6. */
    GCLK_GENERATOR_6,
#endif
#if defined(__DOXYGEN__) || (GCLK_GEN_NUM_MSB > 6)
    /** GCLK generator channel 7. */
    GCLK_GENERATOR_7,
#endif
#if defined(__DOXYGEN__) || (GCLK_GEN_NUM_MSB > 7)
    /** GCLK generator channel 8. */
    GCLK_GENERATOR_8,
#endif
#if defined(__DOXYGEN__) || (GCLK_GEN_NUM_MSB > 8)
    /** GCLK generator channel 9. */
    GCLK_GENERATOR_9,
#endif
#if defined(__DOXYGEN__) || (GCLK_GEN_NUM_MSB > 9)
    /** GCLK generator channel 10. */
    GCLK_GENERATOR_10,
#endif
#if defined(__DOXYGEN__) || (GCLK_GEN_NUM_MSB > 10)
    /** GCLK generator channel 11. */
    GCLK_GENERATOR_11,
#endif
#if defined(__DOXYGEN__) || (GCLK_GEN_NUM_MSB > 11)
    /** GCLK generator channel 12. */
    GCLK_GENERATOR_12,
#endif
#if defined(__DOXYGEN__) || (GCLK_GEN_NUM_MSB > 12)
    /** GCLK generator channel 13. */
    GCLK_GENERATOR_13,
#endif
#if defined(__DOXYGEN__) || (GCLK_GEN_NUM_MSB > 13)
    /** GCLK generator channel 14. */
    GCLK_GENERATOR_14,
#endif
#if defined(__DOXYGEN__) || (GCLK_GEN_NUM_MSB > 14)
    /** GCLK generator channel 15. */
    GCLK_GENERATOR_15,
#endif
#if defined(__DOXYGEN__) || (GCLK_GEN_NUM_MSB > 15)
    /** GCLK generator channel 16. */
    GCLK_GENERATOR_16,
#endif
};

/**
 * \brief Generic Clock Generator configuration structure.
 *
 * Configuration structure for a Generic Clock Generator channel. This
 * structure should be initialized by the
 * \ref system_gclk_gen_get_config_defaults() function before being modified by
 * the user application.
 */
struct system_gclk_gen_config {
    /** Source clock input channel index, see the \ref system_clock_source. */
    uint8_t source_clock;
    /** If \c true, the generator output level is high when disabled. */
    bool high_when_disabled;
    /** Integer division factor of the clock output compared to the input. */
    uint32_t division_factor;
    /** If \c true, the clock is kept enabled during device standby mode. */
    bool run_in_standby;
    /** If \c true, enables GCLK generator clock output to a GPIO pin. */
    bool output_enable;
};

/**
 * \brief Generic Clock configuration structure.
 *
 * Configuration structure for a Generic Clock channel. This structure
 * should be initialized by the \ref system_gclk_chan_get_config_defaults()
 * function before being modified by the user application.
 */
struct system_gclk_chan_config {
    /** Generic Clock Generator source channel. */
    enum gclk_generator source_generator;
};

/** \name Generic Clock Management
 * @{
 */
void system_gclk_init(void);

/** @} */


/**
 * \name Generic Clock Management (Generators)
 * @{
 */

/**
 * \brief Initializes a Generic Clock Generator configuration structure to defaults.
 *
 * Initializes a given Generic Clock Generator configuration structure to
 * a set of known default values. This function should be called on all
 * new instances of these configuration structures before being modified
 * by the user application.
 *
 * The default configuration is as follows:
 *  \li Clock is generated undivided from the source frequency
 *  \li Clock generator output is low when the generator is disabled
 *  \li The input clock is sourced from input clock channel 0
 *  \li Clock will be disabled during sleep
 *  \li The clock output will not be routed to a physical GPIO pin
 *
 * \param[out] config  Configuration structure to initialize to default values
 */
static inline void system_gclk_gen_get_config_defaults(
    struct system_gclk_gen_config *const config)
{
    /* Sanity check arguments */
    Assert(config);

    /* Default configuration values */
    config->division_factor    = 1;
    config->high_when_disabled = false;
#if SAML21
    config->source_clock       = GCLK_SOURCE_OSC16M;
#else
    config->source_clock       = GCLK_SOURCE_OSC8M;
#endif
    config->run_in_standby     = false;
    config->output_enable      = false;
}

void system_gclk_gen_set_config(
    const uint8_t generator,
    struct system_gclk_gen_config *const config);

void system_gclk_gen_enable(
    const uint8_t generator);

void system_gclk_gen_disable(
    const uint8_t generator);

bool system_gclk_gen_is_enabled(
    const uint8_t generator);

/** @} */


/**
 * \name Generic Clock Management (Channels)
 * @{
 */

/**
 * \brief Initializes a Generic Clock configuration structure to defaults.
 *
 * Initializes a given Generic Clock configuration structure to a set of
 * known default values. This function should be called on all new
 * instances of these configuration structures before being modified by the
 * user application.
 *
 * The default configuration is as follows:
 *  \li Clock is sourced from the Generic Clock Generator channel 0
 *  \li Clock configuration will not be write-locked when set
 *
 * \param[out] config  Configuration structure to initialize to default values
 */
static inline void system_gclk_chan_get_config_defaults(
    struct system_gclk_chan_config *const config)
{
    /* Sanity check arguments */
    Assert(config);

    /* Default configuration values */
    config->source_generator = GCLK_GENERATOR_0;
}

void system_gclk_chan_set_config(
    const uint8_t channel,
    struct system_gclk_chan_config *const config);

void system_gclk_chan_enable(
    const uint8_t channel);

void system_gclk_chan_disable(
    const uint8_t channel);

bool system_gclk_chan_is_enabled(
    const uint8_t channel);

void system_gclk_chan_lock(
    const uint8_t channel);

bool system_gclk_chan_is_locked(
    const uint8_t channel);

/** @} */


/**
 * \name Generic Clock Frequency Retrieval
 * @{
 */

uint32_t system_gclk_gen_get_hz(
    const uint8_t generator);

uint32_t system_gclk_chan_get_hz(
    const uint8_t channel);

/** @} */

#ifdef __cplusplus
}
#endif

/** @} */

#endif
