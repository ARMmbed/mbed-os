
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

// mbed.getCurrentBranch returns either local branch name or reference to pull request
def currentBranch = mbed.getCurrentBranch()

// Create a map of predefined build steps 
def parallelSteps = mbed.createParalleSteps("mbed-os", targets, toolchains)

// Run build steps parallel, map as paramater
mbed.compile(parallelSteps)

def testApps = [
  "mbed-os-cliapp"
  ]

// buildTestApps accepts array of test application names and a mbed-os branch or PR reference as parameters
mbed.buildTestApps(testApps, "${currentBranch}")
