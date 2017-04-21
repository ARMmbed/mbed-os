// List of targets with supported RF shields to compile
def targets = [
  "K64F": ["ATMEL", "MCR20"],
  //"NUCLEO_F401RE": ["ATMEL", "MCR20"],
  "NUCLEO_F429ZI": ["ATMEL", "MCR20"],
  //"NCS36510": ["NCS36510"],
  "UBLOX_EVK_ODIN_W2": ["ATMEL", "MCR20"]
  ]

// Map toolchains to compilers
def toolchains = [
  ARM: "armcc",
  GCC_ARM: "arm-none-eabi-gcc",
  IAR: "iar_arm"
  ]

// Supported RF shields
def radioshields = [
  "ATMEL",
  "MCR20",
  "NCS36510"
  ]

// Mesh interfaces: 6LoWPAN and Thread
def meshinterfaces = [
  "6lp",
  "thd"
  ]

def stepsForParallel = [:]

// Jenkins pipeline does not support map.each, we need to use oldschool for loop
for (int i = 0; i < targets.size(); i++) {
  for(int j = 0; j < toolchains.size(); j++) {
    for(int k = 0; k < radioshields.size(); k++) {
      for(int l = 0; l < meshinterfaces.size(); l++) {
        def target = targets.keySet().asList().get(i)
        def allowed_shields = targets.get(target)
        def toolchain = toolchains.keySet().asList().get(j)
        def compilerLabel = toolchains.get(toolchain)
        def radioshield = radioshields.get(k)
        def meshInterface = meshinterfaces.get(l)
        def stepName = "${target} ${toolchain} ${radioshield} ${meshInterface}"
        if(allowed_shields.contains(radioshield)) {
          stepsForParallel[stepName] = buildStep(target, compilerLabel, toolchain, radioshield, meshInterface)
        }
      }
    }
  }
}

timestamps {
  parallel stepsForParallel
}

def buildStep(target, compilerLabel, toolchain, radioShield, meshInterface) {
  return {
    stage ("${target}_${compilerLabel}_${radioShield}_${meshInterface}") {
      node ("${compilerLabel}") {
        deleteDir()
        execute ("git clone git@github.com:ARMmbed/mbed-os-example-mesh-minimal.git")
        dir("mbed-os-example-mesh-minimal") {

          if ("${radioShield}" == "MCR20") {
            // Replace default rf shield
            execute("sed -i 's/\"value\": \"ATMEL\"/\"value\": \"MCR20\"/' mbed_app.json")
          }

          if ("${radioShield}" == "NCS36510") {
            // Replace default rf shield
            execute("sed -i 's/\"value\": \"ATMEL\"/\"value\": \"NCS36510\"/' mbed_app.json")
          }

          if ("${meshInterface}" == "thd") {
            // Change mesh interface to thread
            execute("sed -i 's/\"value\": \"MESH_LOWPAN\"/\"value\": \"MESH_THREAD\"/' mbed_app.json")

            //Change operation mode to Thread router
            execute("sed -i 's/\"NANOSTACK\", \"LOWPAN_ROUTER\", \"COMMON_PAL\"/\"NANOSTACK\", \"THREAD_ROUTER\", \"COMMON_PAL\"/' mbed_app.json")
          }

          if ("${meshInterface}" == "6lp") {
          // Use systest border router for testing
            execute("sed -i 's/\"mbed-mesh-api.6lowpan-nd-channel\": 12/\"mbed-mesh-api.6lowpan-nd-channel\": 18/' mbed_app.json")
          }

          execute ("mbed deploy --protocol ssh")
          // Checkout latest mbed-os master
          dir("mbed-os") {
            deleteDir()
            checkout scm
          }
          execute ("mbed compile --build out/${target}_${toolchain}_${radioShield}_${meshInterface}/ -m ${target} -t ${toolchain} -c")
        }
        archive '**/mbed-os-example-mesh-minimal.bin'
      }
    }
  }
}
