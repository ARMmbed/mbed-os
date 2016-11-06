# About the configuration system

The mbed configuration system can be used to customize the compile time configuration of various mbed components (targets, libraries and applications). Each such component can define a number of *configuration parameters*. The values of these configuration parameters can then be *overridden* in various ways. Configuration is defined using [JSON](http://www.json.org/). Some examples of configuration parameters:

- the sampling period for a data acquisition application.
- the default stack size for a newly created OS thread.
- the receive buffer size of a serial communication library.
- the flash and RAM memory size of a mbed target.

The configuration system gathers and interprets all the configuration defined in the source tree. The output of the configuration system is a list of macros that are automatically defined when compiling the code.

# Defining configuration parameters

The configuration system understands configuration data defined in targets, libraries and applications. While there are some slight differences in the way the configuration system works in these cases, the configuration parameters are always defined in a JSON object called "config". An example is given below:

```
{
    "config": {
        "param1": {
            "help": "The first configuration parameter",
            "macro_name": "CUSTOM_MACRO_NAME",
            "value": 0
        },
        "param2": {
            "help": "The second configuration parameter",
            "required": true
        },
        "param3": 10
    }
}
```

The JSON fragment above defines 3 configuration parameters named `param1`, `param2` and `param3`. There are two ways to define a configuration parameter:

- the short way: by name and value. `param3` above is an example of a short definition for a parameter named `param3` with value `10`.
- the long way: by name and description (another JSON object), like `param1` and `param2` above. The JSON description object can have the following keys:
    - `help`: an optional help message that describes the purpose of the parameter.
    - `value`: an optional field that defines the value of the parameter.
    - `required`: an optional key that specifies if the parameter **must** be given a value before compiling the code (`false` by default). It's not possible to compile a source tree with one or more required parameters that don't have a value. Generally, it makes sense to define a required parameter only when it doesn't have a `value` key.
    - `macro_name`: an optional name for the macro defined at compile time for this configuration parameter. The configuration system will automatically figure out the corresponding macro name for a configuration parameter, but the user can override this automatically computed name by specifying `macro_name`.

Note that the name of a parameter in `config` can't contain a dot (`.`) character.

The configuration system automatically appends an *implicit prefix* to the name of each parameter, so you don't have to worry about a name clash if you define a parameter with the same name in a library and a target, for example. The implicit prefix is:

- **target.** if the parameter is defined in a target.
- **app.** if the parameter is defined in the application.
- the name of the library followed by a dot (.) if the parameter is defined in a library.

# Configuration data in libraries

Each mbed library can have an optional `mbed_lib.json` file located in the root folder of the library that defines its configuration. For a library called `mylib`, the configuration file could look like this:

```
{
    "name": "mylib",
    "config": {
        "buffer_size": 1024,
        "timer_period": {
            "help": "The timer period (in us)",
            "macro_name": "INTERNAL_GPTMR_PERIOD",
            "required": true
        },
        "queue_size": {
            "help": "Size of event queue (entries)",
            "value": 10
        }
    },
    "macros": ["MYMOD_MACRO1", "MYMOD_MACRO2=\"TEST\""],
    "target_overrides": {
        "K64F": {
             "timer_period": 100,
             "queue_size": 40
        },
        "NXP": {
             "queue_size": 20,
             "buffer_size": 128
        }
    }
}
```

In this JSON file:

- `name` is the name of the library. **This is a required field.**
- `config` defines the configuration parameters of the library, as explained [here](#defining-configuration-parameters).
- `macros` is a list of extra macros that will be defined when compiling a project that includes this library. A macro can be defined without a value (like `MYMOD_MACRO1` above) or with a value (like `MYMOD_MACRO2` above).
- `target_overrides` is a dictionary with target-specific values for the configuration parameters.

`target_overrides` is used to override the values of the parameters depending on the current compilation target. The keys in `target_overrides` are matched against toolchain *labels* (a description of mbed targets can be found [here](mbed_targets.md)). If a key inside `target_overrides` matches one of the target labels, the parameter values are changed according to the value of the key. In the example above:

- `config` is always processed first, independent of the target. `config` might define values for some of the parameters. In this case, `buffer_size` will be set to 1024, `queue_size` will be set to 10 and `timer_period` will not have a value.
- if the library is compiled for the `K64F` target, `timer_period` will be set to 100 and `queue_size` will be set to 40, since they are overridden by the `K64F` key in `target_overrides`. `buffer_size` will be set to 1024, as defined in `config`.
- assuming that `NXP` is a label defined by **all** NXP based targets, if the library is compiled for **any** `NXP` target (like `LPC1768` or `LPC11U24`), `buffer_size` will be set to 128 and `queue_size` will be set to 20, while `timer_period` will not have a value (since it doesn't get one neither in `config`, nor in the `NXP` override).
- the keys in `target_overrides` are processed in order: if a hypothetical target defines both `K64F` and `NXP` as labels, `timer_period` will be set to 100, `queue_size` will be set to 20 and `buffer_size` will be set to 128.
- if the library is compiled for a target that doesn't have `K64F` or `NXP` as labels, the values of the parameters will be the ones set in `config`.

Except `name`, all the above keys in the JSON file are optional, but if `target_overrides` is defined, `config` must also be defined.

As explained [here](#defining-configuration-parameters), the parameters have an implicit `mylib.` prefix. Outside `mylib`, `buffer_size` is accessible using the name `mylib.buffer_size`. An application will be able to override the value of this parameter, as described in [this section](#configuration-data-in-applications).

 If the source tree has code for more than one library, each library needs its own `mbed_lib.json` file in its root folder.

# Configuration data in targets

Like libraries, targets can define their own configuration data. Additionally, tables can override the configuration of the target(s) they inherit from (for more details about how do define a target and target inheritance, check [this link](mbed_targets.md)). Target configuration data is defined in `targets.json` using `config`, as described [here](#defining-configuration-parameters). An example for a hypothetical `Base` target is given below:

```
"Base": {
    "core": "Cortex-M0",
    "extra_labels": ["BASE_LABEL"],
    "config": {
        "serial_console_speed": {
            "help": "Baud rate of the serial console",
            "value": 115200,
            "macro_name": "MBED_SERIAL_UART_SPEED"
        },
        "stack_size": {
            "help": "Initial stack size of the application",
            "value": 128
        }
    }
}
```

Similar to libraries, the target defined parameters have an implicit prefix. For a target, the prefix is always called `target` (no matter what the actual target name is), so the above configuration parameters will be accessible outside the definition in `Base` (and any other target) as `target.serial_console_speed` and `target.stack_size`.

Targets can inherit from other targets, and their configuration data is also inherited. A target that inherits from one or more other targets can add new parameters in its own `config` section and can also override the configuration parameters defined by its parent(s) in a `overrides` section. For example:

```
"Derived": {
    "inherits": ["Base"],
    "extra_labels_add": ["NXP"],
    "config": {
        "my_own_config": {
            "help": "My very own configuration parameter",
            "value": 0
        }
    },
    "overrides": {
        "stack_size": 256
    }
}
```

`Derived` above defines its own configuration parameter called `my_own_config` and inherits the configuration parameters from `Base`, so its configuration parameters are `serial_console_speed`, `stack_size` and `my_own_config`. It also overrides the value of the `stack_size` parameter defined in `Base`. This means that:

- when compiling for `Base`, the target will define two configuration parameters: `serial_console_speed` with the value 115200 and `stack_size` with the value 128.
- when compiling for `Derived`, the target will define three configuration parameters: `serial_console_speed` with the value 115200, `stack_size` with the value 256 and `my_own_config` with the value 0.

It is an error for a derived target to re-define a configuration parameter already defined by its parent(s) in its `config` section. It is also an error for a derived target to override a configuration parameter that was not defined by its parent(s) in its `overrides` section.

# Configuration data in applications

Like the configuration for targets and libraries, application configuration is optional; if it exists, it must be defined in a `mbed_app.json` file. Unlike library configuration, there can be a single `mbed_app.json` file in the source tree.

There are quite a few similarities between configuration data in applications and libraries:

- applications define their configuration parameters in the `config` section of `mbed_app.json`, as explained [here](#defining-configuration-parameters).
- applications can specify target-dependent values in their `target_overrides` section, as described in the [library configuration paragraph][#configuration-data-in-libraries) (but see below for differences).
- applications can define macros that will be define at compile time by declaring them in `macros`.

There are also a few differences:

- applications **can't** have a `name` key in `mbed_app.json`. The prefix for the configuration parameters defined in an application is always `app.`.
- applications can also override configuration of libraries and targets in addition to its own configuration in its `target_overrides` section.

The last point above is important. The application can freely override the configuration of any of the libraries it depends on, as well as the configuration data in targets, so it has complete control over the configuration of the whole build. For an application called myapp that depends on mylib above, the configuration can look like this:

```
{
    "config": {
        "welcome_string": {
            "help": "The string printed on the display on start-up",
            "value": "\"Hello!\""
        }
    },
    "target_overrides": {
        "*": {
            "target.serial_console_speed": 2400,
            "mylib.timer_period": 100
        },
        "Base": {
            "target.serial_console_speed": 9600
        }
    }
}
```

`target_overrides` works a lot like it does in libraries, but there are a few differences:

- since the application can override any configuration parameter, it must specify them using their prefix (like `mylib.timer_period`). If an overridden parameter doesn't have a prefix, it is assumed that it is one of the parameters defined by the application in its own `config` section.
- the `*` key in `target_overrides` will match *any* target. It is possible to use the `*` key in a library's `target_overrides` too, but it'd make little sense to do so, since it will always override the values defined in the library's `config` section. In an application it might make sense to use the `*` key, since it can be used to override the configuration defined by the target or the dependent libraries, no matter which target is used for building.

Other than this, `target_overrides` works exactly like it does for libraries. Keys in `target_overrides` are still processed in the order they are defined, so for the example above, the `*` override is always processed first (since it matches all targets) and then `Base` is only processed for the `Base` target.

`myapp` above defines its own configuration parameter (`welcome_string`) and overrides the configuration in both the target (`target.serial_console_speed`) and its `mylib` dependency (`mylib.timer_period`):

- when compiling for `Base`, `app.welcome_string` will be set to `"Hello!"`, `target.serial_console_speed` will be set to 9600 (from the `Base` override) and `mylib.timer_period` will be set to 100 (from the `*` override).
- when compiling for `Derived`, `app.welcome_string` will be set to `"Hello!"`, `target.serial_console_speed` will be set to 2400 (from the `*` override) and `mylib.timer_period` will be set to 100 (also from the `*` override).

It is an error for the application configuration to override configuration parameters that were not defined.

## Overriding cumulative target attributes

Target configurations contain a set of cumulative attributes that can be manipulated in the application configuration. These attributes can be overriden as a normal configuration parameter, or manipulated with the special `attribute_add` and `attribute_remove` meta-attributes.

Cumulative attributes:
- features: List of features which will be compiled into the resulting binary and available at runtime. Determines the FEATURE directories included during compilation. These are also emitted as FEATURE macros.
- device_has: List of hardware components available on the target. These are emitted as DEVICE_HAS macros.
- extra_labels: List of target labels which determine the TARGET directories included during compilation. These are also emitted as TARGET macros.
- macros: List of target-specific macros that are defined during compilation.

For example, an application may want to remove features with extra space or runtime cost. This `mbed_app.json` will disable the IPV4 network stack. Attempting to use this network stack will result in a compilation error:

```
{
    "target_overrides": {
        "K64F": {
            "target.features_remove": ["IPV4"]
        }
    }
}
```

# Configuration data precedence

The order in which the various bits of configurations are considered is this:

- the configuration defined by an inherited target overrides the configuration defined by its parent(s), as described [above](#configuration-data-in-targets).
- the configuration of the top level application overrides the configuration defined by the target and any of the libraries on which it depends.

For `myapp` above:

- the value of `target.serial_console_speed` will be 9600 when compiling for `Base` because of the `Base` override in myapp's `target_overrides`.
- the value of `target.serial_console_speed` will be 2400 when compiling for any other target because of the `*` override in myapp's `target_overrides`.
- the value of `target.stack_size` will be 256 when compiling for `Derived` and 128 when compiling for `Base` or any other target that derives from `Base` (assuming of course that `Derived` is the only target that redefines `stack_size`).
- the value of `mylib.timer_period` will be 100, since that's overridden by the application and thus takes precedence over the values defined in `mylib`.
- when compiling for `Base`, the values of `mylib.buffer_size` and `mylib.queue_size` will be 1024 and 10 respectively, as defined in the `config` section of `mylib`.
- when compiling for `Derived`, the values of `mylib.buffer_size `and `mylib.queue_size` will be 128 and 20 respectively, since `Derived` defines the `NXP` label and `mylib` defines a specific configuration for this label. Also, since `Derived` has its own `my_own_config` configuration parameter, `target.my_own_config` will also be defined in this case.

# Using configuration data in the code

When compiling, the configuration system will automatically generate macro definitions for the configuration parameters and all the macros defined in libraries and the application in their `macros` keys. These definitions will be written in a file named `mbed_config.h` located in the build directory. When compiling `myapp` for target `Base`, the `mbed_config.h` file will look like this (note that the order of the definitions might be different):

```
// Automatically generated configuration file.
// DO NOT EDIT, content will be overwritten.

#ifndef __MBED_CONFIG_DATA__
#define __MBED_CONFIG_DATA__

// Configuration parameters
#define MBED_CONF_MYAPP_WELCOME_STRING "Hello!" // set by application
#define MBED_SERIAL_UART_SPEED         9600     // set by application[Base]
#define MBED_CONF_TARGET_STACK_SIZE    128      // set by target
#define INTERNAL_GPTMR_PERIOD          100      // set by application[*]
#define MBED_CONF_MYLIB_BUFFER_SIZE    1024     // set by library:mylib
#define MBED_CONF_MYLIB_QUEUE_SIZE     10       // set by library:mylib
// Macros
#define MYMOD_MACRO1                            // defined by library:mylib
#define MYMOD_MACRO2                   "TEST"   // defined by library:mylib

#endif
```

When compiling for `Derived`, `mbed_config.h` will look like this:


```
// Automatically generated configuration file.
// DO NOT EDIT, content will be overwritten.

#ifndef __MBED_CONFIG_DATA__
#define __MBED_CONFIG_DATA__

// Configuration parameters
#define MBED_CONF_MYAPP_WELCOME_STRING "Hello!" // set by application
#define MBED_SERIAL_UART_SPEED         2400     // set by application[*]
#define MBED_CONF_TARGET_STACK_SIZE    256      // set by target
#define MBED_CONF_TARGET_MY_OWN_CONFIG 0        // set by target
#define INTERNAL_GPTMR_PERIOD          100      // set by application[*]
#define MBED_CONF_MYLIB_BUFFER_SIZE    128      // set by library:mylib[NXP]
#define MBED_CONF_MYLIB_QUEUE_SIZE     20       // set by library:mylib[NXP]
// Macros
#define MYMOD_MACRO1                            // defined by library:mylib
#define MYMOD_MACRO2                   "TEST"   // defined by library:mylib

#endif
```

Note that a macro definition will *not* be generated for a parameter that doesn't have a value.

The names of the macros for the configuration parameter (unless explicitly specified by `macro_name`) are prefixed by **MBED_CONF_**, followed by the full (prefixed) name of the parameter, capitalized and converted to a valid C macro name (if needed).

`mbed_config.h` will be included automatically by the toolchain in all compiled sources, so you'll have access to the configuration data without having to include `mbed_config.h` manually.

*Do not edit mbed_config.h manually*. It will be overwritten the next time you compile or export your project and all your changes will be lost.
