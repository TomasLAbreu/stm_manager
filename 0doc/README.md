# Setup

## Install ST-Link V2
Check-out [FreeElectron tutorial](https://freeelectron.ro/installing-st-link-v2-to-flash-stm32-targets-on-linux/) for "Installing ST-Link v2 to flash STM32 targets on Linux".

1. Install the necessary libraries and build tools
```shell
~$ sudo apt-get install git make cmake libusb-1.0-0-dev
~$ sudo apt-get install gcc build-essential
```

2. Download and build the ST-Link utilities:
```shell
~$ cd
~$ mkdir stm32
~$ cd stm32
~/stm32$ git clone https://github.com/stlink-org/stlink
~/stm32$ cd stlink
~/stm32$ sudo cmake .
~/stm32$ sudo make
~/stm32$ sudo make install
```

3. Copy the built binaries to their place
```shell
~/stm32$ cd bin
~/stm32/bin$ sudo cp st-* /usr/local/bin
~/stm32/bin$ cd ../lib
~/stm32/lib$ sudo cp *.so* /lib32
~/stm32/lib$ sudo cp ../config/udev/rules.d/49-stlinkv* /etc/udev/rules.d/
```

4. Test if the installation was successful. After plugin in your STM32 into a USB port check the following outputs:
```shell
~$ lsusb
...
Bus 00x ... STMicroelectronics ST-LINK/V2.1
...

~$ st-info --probe
Found 1 stlink programmers
  version: ...
```

## Setup Cross-compiler
Check-out [FreeElectron tutorial](https://freeelectron.ro/arm-cross-compiler-tutorial-stm32/) for "Setting-up cross compiler and build tools for STM32".

1. Select adequate toolchain from [arm-open-source-software](https://developer.arm.com/tools-and-software/open-source-software/developer-tools/gnu-toolchain/gnu-rm/downloads/9-2019-q4-major), and download it into `~/Downloads`.

```shell
~$ cd ~/Downloads
~/Downloads$ tar -xjf gcc-arm-none-eabi-9-2019-q4-major-x86_64-linux.tar.bz2
```

2. Install the compiler and tools in the appropriate place:
```shell
~$ sudo mkdir /opt
~$ cd /opt
~/opt$ sudo mv ~/Downloads/gcc-arm-none-eabi-9-2019-q4-major /opt/gcc-arm
```

3. Setup `PATH` variable to include the toolchain. If you don’t want to have a lengthy `PATH` variable:
```shell
~/opt$ echo 'export PATH="/opt/gcc-arm/bin:$PATH"' > /opt/gcc-arm/set-env.sh
~/opt$ source /opt/gcc-arm/set-env.sh
```

4. Check if the toolchain was successfully installed
```shell
arm-none-eabi-gcc --version
```