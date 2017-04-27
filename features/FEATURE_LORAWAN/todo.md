## Integration
Minimal work needed to finish an integration of the LoRaWAN stack from Semtech

#### PHY
- [ ] Drivers pulled from upstream gihub repo maintained and certified with MAC by Semtech
- [ ] Drivers updated in components database.
- [ ] Drivers for SX1272 and SX1276 come from src/radio/sx127x and integrate with mbed OS by the src/system interfaces where possible.

#### LoRaMAC
- [ ] Pulled from upstream github repo maintained and certified by Semtech https://github.com/Lora-net/LoRaMac-node (src/mac non v3 code)

#### Channel plan and mbed OS adaptation
- [ ] This is the integration and stitching point between the MAC and PHY to run on all mbed boards
- [ ] Integration point is mbed OS looking like a src/board providing minimal implementation for src/system


## Testing
Make it green!

#### PHY
- [ ] Echo server tests
- [ ] FSK and LoRa modulation
- [ ] Requires local node connected to test infrastructure - docs on how to deploy

#### LoRaWANInterface
- [ ] Echo server tests using LoRaWAN
- [ ] Adaptive data rate tests for network throttling
- [ ] Requires local gateway connected to test infrastructure - docs on how to deploy
- [ ] Requires network server (loriot for example) - docs on how to deploy
- [ ] Requires web application for echo server - docs on how to deploy
