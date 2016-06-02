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
  mail to: 'iot-eng-mgmt@arm.com', 
  subject: "${env.JOB_NAME} build failed",
  body: "Greetings from mbed Continous integration system. This is an automatic email sent because a build has failed.\n\n\
This CI job checks the compiling of mbed-os with morpheus from Master branch.\n\n\
mbed-os build from branch ${env.BRANCH_NAME} has failed.\nSee ${env.BUILD_URL} for details\n\n\
Regards,\n\
ARM mbed Continuos Integration"
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
