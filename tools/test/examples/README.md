# Examples testing script

examples.py is script which makes easier to test examples.
```
usage: examples.py [-h] [-c CONFIG] [-e EXAMPLE]
                   {import,clone,deploy,tag,compile,export} ...

positional arguments:
  {import,clone,deploy,tag,compile,export}

optional arguments:
  -h, --help            show this help message and exit
  -c CONFIG             Configuration file. examples.json as default
  -e EXAMPLE, --example EXAMPLE
                        filter the examples used in the script
```

 Configuration example:
 ```
 {
  "examples": [
    {
      "name": "mbed-os-example-blinky",
      "github": "https://github.com/ARMmbed/mbed-os-example-blinky",
      "mbed": [
          "https://os.mbed.com/teams/mbed-os-examples/code/mbed-os-example-blinky"
      ],
      "test-repo-source": "github",
      "features" : [],
      "targets" : ["ARM"],
      "toolchains" : ["K66F", "K64F"],
      "exporters": ["iar", "make_armc5", "make_armc6", "make_gcc_arm", "make_iar"],
      "compile" : true,
      "export": true,
      "test" : true,
      "baud_rate": 9600,
      "compare_log": ["mbed-os-example-blinky/tests/blinky.log"],
      "auto-update" : true
    }
  ]
}
 ```
 Explanations:
 * **name** is example name used mostly with prints
 * **github** is github repository address
 * **mbed** Mercurial repositories in os.mbed.com
 * **test-repo-source** specify which repo source to use for testing
 * **features***
 * **targets** list of targeted development boards
 * **toolchains** toolchain to use for compile
 * **exporters** allowed exporters
 * **compile** enable compiling
 * **export** enable exporting
 * **test** enable testing
 * **bound_rate** used bound rate
 * **compare_log** Log which is compared to command line output when testing
 * **auto-update**
