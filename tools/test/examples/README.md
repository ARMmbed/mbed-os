# Examples testing script

The scripts in this folder are used for testing `mbed-os` official examples. It contains the following files:

- `examples.py` - the main script that serves as the command-line interface.
- `examples.json` - the default configuration file for the script, which contains the information to the examples. If required, you can pass a customized configuration file as an argument to the script.
- `examples_lib.py` - the library file, which contains the main function of the testing scripts.

## The scripts

`examples.py` provides command-line interface and subcommands that makes easier to test examples. Included subcommands are:

* **import** - imports each of the repos and its dependencies (.lib files) associated with the specific examples name from the .json configuration file. If there is already a clone of the repo, it will first be removed to ensure a clean, up-to-date cloning.

* **clone** - clones each of the repos associated with the specific examples name from the .json configuration file. If there is already a clone of the repo, it will first be removed to ensure a clean, up-to-date cloning.

* **deploy** - if the example directory exists as provided by the .json configuration file, pulls in the examples dependencies by using `mbed-cli deploy`.

* **update** - for each example repo identified in the config .json object, updates the version of `mbed-os` to that specified by the supplied GitHub tag. This function assumes that each example repo has already been cloned.

* **compile** - compiles combinations of example programs, targets and compile chains.

* **export** - exports and builds combinations of example programs, targets and IDEs.

* **list** - displays examples in a configuration file in a table.

* **symlink** - creates a symbolic link to a given `mbed-os` PATH.

For more detailed options, please use `-h` or `--help`.

## The configuration file

Here is the section of default configuration file:

 ```json
 {
  "examples": [
    {
      "name": "mbed-os-example-blinky",
      "github": "https://github.com/ARMmbed/mbed-os-example-blinky",
      "sub-repo-example": false,
      "subs": [],
      "features" : [],
      "targets" : [],
      "toolchains" : [],
      "exporters": [],
      "compile" : true,
      "export": true,
      "test" : true,
      "baud_rate": 9600,
      "compare_log": ["mbed-os-example-blinky/tests/blinky.log"],
      "auto-update" : true
    },
      
    ...
    
    {
      "name": "mbed-os-example-tls",
      "github": "https://github.com/ARMmbed/mbed-os-example-tls",
      "sub-repo-example": true,
      "subs": [
          "benchmark",
          "tls-client",
          "hashing",
          "authcrypt"
      ],
      "features" : [],
      "targets" : ["K66F", "NUCLEO_F429ZI"],
      "toolchains" : ["GCC_ARM", "ARM"],
      "exporters": [],
      "compile" : false,
      "export": false,
      "test" : false,
      "baud_rate": 9600,
      "compare_log": [
          "mbed-os-example-tls/tests/benchmark.log",
          "mbed-os-example-tls/tests/tls-client.log",
          "mbed-os-example-tls/tests/hashing.log",
          "mbed-os-example-tls/tests/authcrypt.log"
      ],
      "auto-update" : true
    }
  ]
}

 ```

### Fields

* **name** - name of the example. It should be the base name of the example repository address and will throw if it doesn't match.
* **github** - example GitHub repository address.
* **sub-repo-example** - specifies if the example repository has a subfolder for each example.
* **subs** - array of subexample names.
* **features** - the features that must be in the features array of a target in `targets.json`.
* **baud_rate** - example default baud rate.
* **compare_log** - array of log compared to command-line output during testing. If the example has many subexamples, the order of log should match the order of subexamples. 
* **targets** - list of `mbed-os` development boards that run this example. 
* **targets** - list of targeted development boards.
* **toolchains** - toolchain to use for compiling.
* **exporters** - allowed exporters.
* **compile** - enables compiling.
* **export** - enables exporting.
* **test** - enables testing.

### Values

`[ ]` means all possible alternatives.

## Typical use

In the Mbed OS CI, we follow the below steps to compile and test Mbed OS examples.

1. Clone `mbed-os` repository to the current folder:

   ```
   git clone https://github.com/ARMmbed/mbed-os.git
   ```

1. Clone the examples repo to the current folder. Users can pass an `-e` option to the script to filter out the rest of the examples, so the scripts only run on one particular example:

   ```
   python mbed-os/tools/test/examples/examples.py clone
   ```

1. Create a symbolic link to `mbed-os` for every example. This step lets all the examples share a single `mbed-os` folder, rather than checking out the `mbed-os` folder many times. We highly recommend you pass an absolute path as the argument: 

   ```
   python mbed-os/tools/test/examples/examples.py symlink $PWD/mbed-os
   ```

1. Deploy other dependency libraries:

   ```
   python mbed-os/tools/test/examples/examples.py deploy
   ```

1. Compile the test for the examples on a specific target:

   ```
   python mbed-os/tools/test/examples/examples.py compile -m <target>
   ```

After the compile test finished, the scripts print the result table:

```
Passed example compilation:
+---------------------------------+--------+-----------+----------+--------------+
| EXAMPLE NAME                    | TARGET | TOOLCHAIN | TEST GEN | BUILD RESULT |
+---------------------------------+--------+-----------+----------+--------------+
| mbed-os-example-kvstore         |  K64F  |  GCC_ARM  | TEST_ON  |    PASSED    |
| mbed-os-example-tls-socket      |  K64F  |  GCC_ARM  | TEST_ON  |    PASSED    |
| mbed-os-example-blockdevice     |  K64F  |  GCC_ARM  | TEST_ON  |    PASSED    |
| mbed-os-example-wifi            |  K64F  |  GCC_ARM  | TEST_OFF |    PASSED    |
| mbed-os-example-error-handling  |  K64F  |  GCC_ARM  | TEST_ON  |    PASSED    |
| mbed-os-example-sd-driver       |  K64F  |  GCC_ARM  | TEST_ON  |    PASSED    |
| mbed-os-example-crash-reporting |  K64F  |  GCC_ARM  | TEST_ON  |    PASSED    |
| mbed-os-example-filesystem      |  K64F  |  GCC_ARM  | TEST_ON  |    PASSED    |
| mbed-os-example-blinky          |  K64F  |  GCC_ARM  | TEST_ON  |    PASSED    |
| mbed-os-example-bootloader      |  K64F  |  GCC_ARM  | TEST_OFF |    PASSED    |
| mbed-os-example-cpu-stats       |  K64F  |  GCC_ARM  | TEST_ON  |    PASSED    |
| mbed-os-example-sys-info        |  K64F  |  GCC_ARM  | TEST_ON  |    PASSED    |
| mbed-os-example-attestation     |  K64F  |  GCC_ARM  | TEST_ON  |    PASSED    |
+---------------------------------+--------+-----------+----------+--------------+
Number of failures = 0
```

After the compilation stage, a `test_spec.json` file is generated. Later, Greentea tests will consume this file. They test the compiled example on hardware platform. 
