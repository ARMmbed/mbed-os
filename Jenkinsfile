// List of targets to compile
def morpheusTargets = [
  //"LPC1768",
  //"NUCLEO_F401RE",
  //"NRF51822",
  "K64F"
  ]

// Map morpheus toolchains to compiler labels on Jenkins
def toolchains = [
  //ARM: "armcc",
  //IAR: "iar_arm",
  GCC_ARM: "arm-none-eabi-gcc"
  ]

// Initial maps for parallel build steps
def stepsForParallel = [:]

// Jenkins pipeline does not support map.each, we need to use oldschool for loop
for (int i = 0; i < morpheusTargets.size(); i++) {
  for(int j = 0; j < toolchains.size(); j++) {
    def target = morpheusTargets.get(i)
    def toolchain = toolchains.keySet().asList().get(j)
    def compilerLabel = toolchains.get(toolchain)

    def stepName = "${target} ${toolchain}"
    stepsForParallel[stepName] = morpheusBuildStep(target, compilerLabel, toolchain)
  }
}

/* Jenkins does not allow stages inside parallel execution,
* https://issues.jenkins-ci.org/browse/JENKINS-26107 will solve this by adding labeled blocks
*/
stage "build mbed-os targets"
// Actually run the steps in parallel - parallel takes a map as an argument, hence the above.
parallel stepsForParallel

stage "build testapps"
parallel testapp: {
build job: 'ARMmbed/mbed-client-testapp/master', parameters: [[$class: 'StringParameterValue', name: 'mbed_os_revision', value: "${env.GIT_REVISION}"]]
}, cliapp: {
build job: 'ARMmbed/mbed-client-cliapp/master', parameters: [[$class: 'StringParameterValue', name: 'mbed_os_revision', value: "${env.GIT_REVISION}"]]
}, failFast: true

def execute(cmd) {
    if (isUnix()) {
        sh "${cmd}"
    } else {
        bat "${cmd}"
    }
}

//Create morpheus build steps for parallel execution
def morpheusBuildStep(target, compilerLabel, toolchain) {
  return {
    node ("${compilerLabel}") {
      deleteDir()
      dir("mbed-os") {
        checkout scm
        execute ("git log -1 --no-merges --pretty=format:'%H'" > GIT_REVISION)
        env.GIT_REVISION = readFile ("GIT_REVISION")
        execute ("mbed deploy --protocol ssh")
        execute ("mbed test --compile -m ${target} -t ${toolchain} -c")
      }
    }
  }
}
