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
stage "parallel build"
try {
  // Actually run the steps in parallel - parallel takes a map as an argument, hence the above.
  parallel stepsForParallel
  
} catch (err) {
  currentBuild.result = 'FAILURE'

} 

//Create yotta build steps for parallel execution
 def morpheusBuildStep(target, compilerLabel, toolchain) {
    return {
      node ("${compilerLabel}") {
        deleteDir()
        dir("mbed-os") {
          checkout scm
          sh "mbed deploy --protocol ssh"
          sh "mbed compile -m ${target} -t ${toolchain} -c"
        }
      }
    }
 }
