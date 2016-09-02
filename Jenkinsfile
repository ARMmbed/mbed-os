// List of targets to compile
def targets = [
  //"LPC1768",
  //"NUCLEO_F401RE",
  //"NRF51822",
  "K64F"
  ]

// Map toolchains to compiler labels on Jenkins
def toolchains = [
  //ARM: "armcc",
  //IAR: "iar_arm",
  GCC_ARM: "arm-none-eabi-gcc"
  ]

// Create a map of predefined build steps 
stage "generate build steps for parallel execution"
def parallelSteps = mbed.createParalleSteps("mbed-os", targets, toolchains)

// Run build steps parallel, map as paramater
stage "build all targets"
mbed.compile(parallelSteps)

// Run testapps, mbed-os commit hash or master as parameter
stage "run mbed-os testapps"
mbed.runTestApps("${env.GIT_REVISION}")
