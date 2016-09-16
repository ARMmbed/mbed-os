
// List of targets to compile
def targets = [
  //"LPC1768",
  //"NUCLEO_F401RE",
  //"NRF51822",
  "K64F"
  ]

// Map toolchains to compiler labels on Jenkins
def toolchains = [
  ARM: "armcc",
  //IAR: "iar_arm",
  GCC_ARM: "arm-none-eabi-gcc"
  ]

// Create a map of predefined build steps 
def parallelSteps = mbed.createParalleSteps("mbed-os", targets, toolchains)

// Run build steps parallel, map as paramater
mbed.compile(parallelSteps)

def testApps = [
  "mbed-os-cliapp",
  "mbed-client-testapp"
  ]

// buildTestApps accepts array of test application names and a mbed-os commit hash as parameters
mbed.buildTestApps(testApps, "${env.GIT_REVISION}")
