
# LF Clock configuration using mbed configuration system

In order to provide the configuration for a low frequency (LF) clock, add a description of the LF clock inside a mbed configuration JSON file.
For example at application level the description might be added in a mbed_app.json file and on target level the description might be added in the hal/target.json file.
LF clock source configuration is used for MCU startup initialization and the BLE SoftDevice LF clock configuration (if BLE libraries is used). Advanced configurations are used only for the BLE SoftDevice LF clock configuration.


## Usage:

1. Clock source

Default clock source is XTAL oscillator. It is defined at the target level configuration as the target.lf_clock_src key.
There are three options that can be configured as the clock source:  
    - NRF_LF_SRC_XTAL  
    - NRF_LF_SRC_RC  
    - NRF_LF_SRC_SYNTH  

In order to override this configuration use targed_override section in configuration file (e.g mbed_app.json)

```json
{
     "target_overrides": {
        "*": {
            "target.lf_clock_src": "NRF_LF_SRC_XTAL"
        }
     }
}
```

2a. Advanced configuration of the LFCLK RC oscillator:

```json
{
    "config": {
        "lf_clock_rc_calib_timer_interval": {
            "value": 16,
            "macro_name": "MBED_CONF_NORDIC_NRF_LF_CLOCK_CALIB_TIMER_INTERVAL"
        },
        "lf_clock_rc_calib_mode_config": {
            "value": 1,
            "macro_name": "MBED_CONF_NORDIC_NRF_LF_CLOCK_CALIB_MODE_CONFIG"
        }
    }
}
    
```

"lf_clock_rc_calib_timer_interval" - Calibration timer interval in 250 ms. It is equivalent to nrf_clock_lf_cfg_t::rc_ctiv.
This item generates macro MBED_CONF_NORDIC_NRF_LF_CLOCK_CALIB_TIMER_INTERVAL.
By default, such configuration is set to 16.

"lf_clock_rc_calib_mode_config" - This value configures how often the RC oscillator will be calibrated, in number of calibration intervals.
It is equivalent to nrf_clock_lf_cfg_t::rc_temp_ctiv.
For further information, see the documentation for the [API of a SoftDevice 13x version 2.0.0](http://infocenter.nordicsemi.com/topic/com.nordic.infocenter.s132.api.v2.0.0/structnrf__clock__lf__cfg__t.html)
This item generates macro MBED_CONF_NORDIC_NRF_LF_CLOCK_CALIB_MODE_CONFIG.
By default, such configuration is set to 1.

2b. Advanced configuration of the LFCLK XTAL oscillator:

Accuracy of the clock source can be set. In order to do so macro MBED_CONF_NORDIC_LF_CLOCK_XTAL_ACCURACY should been provided (e.g. in mbed_app.json).
By default such configuration is set to NRF_CLOCK_LF_XTAL_ACCURACY_20_PPM.
For further information, see the documentation for the [API of a SoftDevice 13x version 2.0.0](https://infocenter.nordicsemi.com/index.jsp?topic=%2Fcom.nordic.infocenter.s132.api.v2.0.0%2Fgroup___n_r_f___s_d_m___d_e_f_i_n_e_s.html)

```json
{
    "config": {
        "lf_clock_xtal_accuracy": {
            "value": "NRF_CLOCK_LF_XTAL_ACCURACY_250_PPM",
            "macro_name": "MBED_CONF_NORDIC_LF_CLOCK_XTAL_ACCURACY"
        }
    }
}
    
```



2c. Advance configuration of the LFCLK Synthesized from HFCLK:

Accuracy of the clock source can be set. In order to do so macro MBED_CONF_NORDIC_LF_CLOCK_SYNTH_ACCURACY should been provided (e.g. in mbed_app.json).
By default, such configuration is set to NRF_CLOCK_LF_XTAL_ACCURACY_20_PPM.
For further information, see the documentation for the [API of a SoftDevice 13x version 2.0.0](https://infocenter.nordicsemi.com/index.jsp?topic=%2Fcom.nordic.infocenter.s132.api.v2.0.0%2Fgroup___n_r_f___s_d_m___d_e_f_i_n_e_s.html)

```json
{
    "config": {
        "lf_clock_synth_accuracy": {
            "value": "NRF_CLOCK_LF_SYNTH_ACCURACY_250_PPM",
            "macro_name": "MBED_CONF_NORDIC_LF_CLOCK_XTAL_ACCURACY"
        }
    }
}
    
```


