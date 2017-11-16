// Compiler:target pairs for assigning yotta builds to correct slave labels
def yottaTargets = [
  armcc : "frdm-k64f-armcc",
  gcc   : "frdm-k64f-gcc"
  ]

def morpheusTargets = [
  armcc: "K64F",
  gcc  : "K64F"
  ]  
  
// Compilers for makefile based build steps  
def makeCompilers = [
  "armcc", 
  "gcc",
  "arm-none-eabi-gcc"
  ]

// Nanostack configurations for makefile based build steps  
def makeConfigs = [
  "generic",
  "lowpan_border_router",
  "lowpan_border_router_rf_tunnel",
  "lowpan_host",
  "lowpan_router",
  "nanostack_full",
  "rf_interface",
  "thread_border_router",
  "thread_router",
  "thread_end_device",
  "thread_full_end_device",
  "thread_thci"
  ]

/*****************************************************
 *                                                   *
 * Check out sources only once and use stash/unstash *
 * to distribute source to actual builds             *
 *                                                   *
 ****************************************************/
node ("linux") {
  stage "checkout"
  // deleteDir deletes recursively current directory
  deleteDir() 
  // Recursively clone ARMmbed/nanomesh-applications repository
  checkoutApplications()
  
  // After cloning nanomesh-applications we need to clean and
  // re-clone nanostack with the changes under test
  dir ("applications") {
    sh "rm -rf nanostack && mkdir nanostack"
    dir("nanostack") {
      // Check out current branch with changes under test
      checkout scm
    }
  }
  // Check if these variables are available
  echo "source branch: ${env.BRANCH_NAME}\n"
  echo "target branch: ${env.CHANGE_TARGET}\n"
  echo "change id: ${env.CHANGE_ID}\n"
  echo "change title: ${env.CHANGE_TITLE}\n"
  echo "change url: ${env.CHANGE_URL}\n"
  echo "git revision: ${env.GIT_REVISION}\n"  
  
  // Stash stores a set of files for use later in the same build
  stash includes: '**', useDefaultExcludes: false, name: 'workarea'
}
  
/**********************************************
 *                                            *
 * Create step definitions for parallel       *
 * build execution                            *
 *                                            *
 *********************************************/
def stepsForParallel = [:]

// Add yotta build steps to parallel execution
for (int i = 0; i < yottaTargets.size(); i++) {
    def compiler = yottaTargets.keySet().asList().get(i)
    def target = yottaTargets.get(compiler)
    def stepName = "yotta ${target}"
    stepsForParallel[stepName] = yottaBuildStep(compiler, target)
}

// Add morpheus build steps to parallel execution
/* Not yet
for (int i = 0; i < morpheusTargets.size(); i++) {
    def compiler = morpheusTargets.keySet().asList().get(i)
    def target = morpheusTargets.get(compiler)
    def stepName = "morpheus ${target}"
    stepsForParallel[stepName] = morpheusBuildStep(compiler, target)
}
*/
// Add makefile build steps to parallel execution
for (int i = 0; i < makeCompilers.size(); i++) {
  for (int j = 0; j < makeConfigs.size(); j++) {
    def compiler = makeCompilers.get(i)
    def config = makeConfigs.get(j)
    def stepName = "${compiler} ${config}"
    stepsForParallel[stepName] = makeBuildStep(compiler, config)
  }
}

// Add smoketest step to parallel execution
stepsForParallel["smokeTest"] = smokeTestStep()

/**********************************************
 *                                            *
 * Actually run the steps in parallel         *
 *                                            *
 *********************************************/
stage "build"
parallel stepsForParallel


/**********************************************
 *                                            *
 * End of execution, internal functions below *
 *                                            *
 *********************************************/
// Create build step for morpheus
def morpheusBuildStep(compiler, target) {
  return {
    node ("${compiler}") {
      deleteDir()
      unstash 'workarea'

      dir ("applications/nanostack") {
        if("${compiler}" == "armcc") {
          sh "mbed compile -m ${target} -t ARM -c"
        }
        if("${compiler}" == "gcc") {
          sh "mbed compile -m ${target} -t GCC_ARM -c"
        }
      }
    }
  }
}
 
// Create build step definition for yotta build
def yottaBuildStep(compiler, target) {
  return {
    node ("${compiler}") {
      deleteDir()
      unstash 'workarea'

      dir ("applications/nanostack") {
        sh "yotta target ${target}"
        sh "yotta build"
      }
    }
  }
}

// Create build step definition for makefile build
def makeBuildStep(compiler, config) {
  return {
    node ("${compiler}") {
      deleteDir()
      unstash 'workarea'
      
      dir ("applications") {
        sh "make -C libService CC=${compiler} CONFIG=${config} export-headers"
        sh "make -C nanostack CC=${compiler} CONFIG=${config}"
      }
    }
  }
}

// Create build step definition for smoke test
def smokeTestStep() {
  return {
    node ("linux_test") {
      deleteDir()
      unstash 'workarea'
      executeSmokeTest()
      executeSmokePostbuild()
    }
  }
}

// Recursively clone ARMmbed/nanomesh-applications repository
def checkoutApplications() {
  // Check out nanomesh-applications master branch and update all submodules
  checkout([$class: 'GitSCM', 
    branches: [[name: '*/master']], 
    doGenerateSubmoduleConfigurations: false, 
    extensions: [
      [$class: 'RelativeTargetDirectory', relativeTargetDir: 'applications'], 
      [$class: 'ScmName', name: 'nanomesh-applications'], 
      [$class: 'SubmoduleOption', disableSubmodules: false, recursiveSubmodules: true, reference: '', trackingSubmodules: true],
      [$class: 'LocalBranch', localBranch: '**']
    ], 
    submoduleCfg: [], 
    userRemoteConfigs: [[url: 'git@github.com:ARMmbed/nanomesh-applications.git']]
  ])
}


def executeSmokeTest() {
  dir ("applications") {
    sh "make -f Makefile.simulator test-all-builds"
    sh "make -f Makefile.simulator"
    dir ("mbed-clitest") {
      sh "python clitest.py --tcdir testcases/6lowpan --testtype smoke --type simulate --use_sniffer -w --valgrind --valgrind_tool memcheck -v"
      dir ("log") {
        sh "find . -name \\result.html -execdir mv {} result_6lp_smoke.html \\;"
        sh "find . -name \\result_6lp_smoke.html -exec cp {} . \\;"
        sh "find . -name \\*valgrind* -exec cp {} . \\;"
        sh "find . -name \\result.junit.xml -execdir mv {} result.junit_6lp_smoke.xml \\;"
        sh "find . -name \\result.junit_6lp_smoke.xml -exec cp {} . \\;"
      }
      sh "python clitest.py --group thread --testtype smoke --status released --type simulate -w --valgrind --valgrind_tool memcheck -v"
      dir ("log") {
        sh "find . -name \\result.html -execdir mv {} result_thread_smoke.html \\;"
        sh "find . -name \\result_thread_smoke.html -exec cp {} . \\;"
        sh "find . -name \\*valgrind* -exec cp {} . \\;"
        sh "find . -name \\result.junit.xml -execdir mv {} result.junit_thread_smoke.xml \\;"
        sh "find . -name \\result.junit_thread_smoke.xml -exec cp {} . \\;"
      }
    }
    dir ("nanostack") {
      sh "make CC=cppcheck CPPCHECK_OPTS=\"--xml --xml-version=2 2> cppcheck.xml\""
      sh "sed -i 's%\\(<location file=\"\\)%\\1nanomesh-applications/nanostack/%' cppcheck.xml"
    }
  }
}

def executeSmokePostbuild() {
  // Archive artifacts
  archive 'applications/nanostack/cppcheck.xml, applications/mbed-clitest/log/**/*'
  
  // Publish HTML reports
  publishHTML(target: [allowMissing: false, alwaysLinkToLastBuild: false, keepAll: false, 
               reportDir: 'applications/mbed-clitest/log/', 
               reportFiles: 'result_6lp_smoke.html, result_thread_smoke.html', 
               reportName: 'Test Results'])
  
  // Publish JUnit test result report
  step([$class: 'JUnitResultArchiver', testResults: 'applications/mbed-clitest/log/result.junit*.xml'])
  
  
  // This version of plugin currently only on AWS test Jenkins
  /*
  step([$class: 'TextFinderPublisher', 
    fileSet: 'applications/mbed-clitest/log/result*.html', 
    regexp: 'fail<', 
    alsoCheckConsoleOutput: false, 
    succeedIfFound: false, 
    unstableIfFound: true
 ])
*/  
  // This version of plugin currently only on AWS test Jenkins
  /*
  step([$class: 'CppcheckPublisher', 
    pattern: 'applications/nanostack/cppcheck.xml', 
    allowNoReport: true, ignoreBlankFiles: false,
    threshold: "", newThreshold: "",
    failureThreshold: "", newFailureThreshold: "",
    healthy: "", unHealthy: "",
    severityError: true, severityWarning: true,
    severityStyle: true, severityPerformance: true,
    severityInformation: true, severityNoCategory: true,
    severityPortability: true,
    xSize: 500, ySize: 200, numBuildsInGraph: 10,
    displayAllErrors: true,
    displayErrorSeverity: false, displayWarningSeverity: false,
    displayStyleSeverity: false, displayPerformanceSeverity: false,
    displayInformationSeverity: false, displayNoCategorySeverity: false,
    displayPortabilitySeverity: false
  ])
  */
  
  // This version of plugin currently only on AWS test Jenkins
  /*
  step([$class: 'ValgrindPublisher', 
    pattern: "applications/mbed-clitest/log/*valgrind*.xml",
    failThresholdInvalidReadWrite: "",
    failThresholdDefinitelyLost: "",
    failThresholdTotal: "",
    unstableThresholdInvalidReadWrite: "",
    unstableThresholdDefinitelyLost: "8",
    unstableThresholdTotal: "",
    publishResultsForAbortedBuilds: true,
    publishResultsForFailedBuilds: true,
    failBuildOnMissingReports: false,
    failBuildOnInvalidReports: false
  ])    
  */
}
