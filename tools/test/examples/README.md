# Examples testing script

The scripts in this folder are used for testing mbed-os official examples. it contains following files:

- `examples.py` the main script which serves as command line interface
- `examples.json` the default configuration file for the script, which contains the information to the examples. if required, a customised configuration file can be pass as argument to the script
- `examples_lib.py` the library file, which contains the main function of the testing scripts


## The Scripts

`examples.py` provide command line interface and sub commands that makes easier to test examples. included  sub commands  are:

* **import** - Imports each of the repos and its dependencies (.lib files) associated  with the specific examples name from the json configuration file. Note if there is already a clone of the repo then it will first be removed to  ensure a clean, up to date cloning.

* **clone** - clones each of the repos associated with the specific examples name from the json configuration file. Note if there is already a clone of the repo then it will first be removed to ensure a clean, up to date cloning.

* **deploy** - If the example directory exists as provided by the json configuration file, pull in the examples dependencies by using `mbed-cli deploy`.

* **update** - For each example repo identified in the config json object, update the version of mbed-os to that specified by the supplied GitHub tag. This function assumes that each example repo has already been cloned.

* **compile** - Compiles combinations of example programs, targets and compile chains.

* **export** - Exports and builds combinations of example programs, targets and IDEs.

* **list** - display examples in configuration file in a table

* **symlink** - create symbolic link to given mbed-os PATH

for more detailed options, please use `-h` or `--help` 


## The Configuration file

here is the section of default configuration file:

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

* **name** - name of the example, should be the base name of the example repository address, will throw if not match
* **github** - example github repository address
* **sub-repo-example** specify if the example repository have sub folder for each examples
* **subs** - array of sub examples names
* **features** The features that must be in the features array of a target in targets.json
* **baud_rate** example default baud rate
* **compare_log**  array of log which is compared to command line output when testing, if example has many sub examples, the order of log should match with the order of sub examples. 
* **targets** - list of mbed-os dev boards that runs this example. 
* **targets** list of targeted development boards
* **toolchains** toolchain to use for compile
* **exporters** allowed exporters
* **compile** enable compiling
* **export** enable exporting
* **test** enable testing

### Values

`[ ]` means all possible alternatives



## Typical usage

In mbed-OS CI, we usually following the bellow steps to compile and test  mbed-os-examples.

1. Clone mbed-os repository to current folder 

```
git clone https://github.com/ARMmbed/mbed-os.git
```

2. Clone the examples repo to current folder , users can pass a `-e` option to the script to filter out rest of the exmpale and let the scripts only run on particular one 

```
python mbed-os/tools/test/examples/examples.py clone
```

3. Create symbolic link to mbed-os for every examples. this is step is to let the all the examples share a single mbed-os folder, rather that checkout the mbed-os folder many times . In bellow command pass an absolute path as the argument is highly recommended  

```
python mbed-os/tools/test/examples/examples.py symlink $PWD/mbedos
```
4. deploy mbed-os other dependency libraries

```
python mbed-os/tools/test/examples/examples.py deploy
```
5. compile test for the examples on a specific target
```
python mbed-os/tools/test/examples/examples.py compile -m <target>
```
once the compile test finished, the scripts will prints the result table as follows:

```
Passed Example Compilation:
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

after the compilation stage, a `test_spec.json` file will be generated, later this file will be consumed by GreenTea tests, which is going to test the compiled example on hardware platform. 


