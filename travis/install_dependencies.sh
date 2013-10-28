echo "Installing compilation dependencies."

# Install GCC-ARM Compiler.
sudo add-apt-repository -y ppa:terry.guo/gcc-arm-embedded
sudo apt-get update
sudo apt-get install gcc-arm-none-eabi
