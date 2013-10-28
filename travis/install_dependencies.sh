echo "Installing compilation dependencies."

# Install GCC-ARM Compiler.
sudo add-apt-repository -y ppa:terry.guo/gcc-arm-embedded
sudo add-apt-repository -y ppa:ubuntu-wine/ppa

sudo apt-get update
sudo apt-get install -y gcc-arm-none-eabi
sudo apt-get install -y wine1.5

# Download ARMCC (Testing Purposes only at the moment)
wget https://dl.dropboxusercontent.com/u/15449666/ARMCC.tar.gz
tar xvfz ARMCC.tar.gz

# Setup ARMCC environment variables

printf "#%s/bin/bash\nwine armcc.exe" ! > ARMCC/bin/armcc
chmod a+x ARMCC/bin/armcc

printf "#%s/bin/bash\nwine armar.exe" ! > ARMCC/bin/armar
chmod a+x ARMCC/bin/armar

printf "#%s/bin/bash\nwine armasm.exe" ! > ARMCC/bin/armasm
chmod a+x ARMCC/bin/armasm

printf "#%s/bin/bash\nwine armlink.exe" ! > ARMCC/bin/armlink
chmod a+x ARMCC/bin/armlink

printf "#%s/bin/bash\nwine fromelf.exe" ! > ARMCC/bin/fromelf
chmod a+x ARMCC/bin/fromelf

export PATH=$PATH:$TRAVIS_BUILD_DIR"/ARMCC/bin"
