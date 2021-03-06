---
layout: page
tocref: uber eXtensible Micro-Hypervisor Framework Documentation &gt; rpi3-cortex_a53-armv8_32  
title: Building
---

## Build Core Micro-Hypervisor Framework

1.  Prepare raspberry pi sd-card image
	1. Download https://downloads.raspberrypi.org/raspbian_lite/images/raspbian_lite-2016-11-29/2016-11-25-raspbian-jessie-lite.zip
	1. Use [win32diskimager](https://sourceforge.net/projects/win32diskimager/) under Windows OS to burn contents of .zip
	1. Use ImageWriter tool under Ubuntu
	
1. Boot into raspbian using the sd-card on the pi and resize the root filesystem to
   fit the entire sd card. Use the following commands once logged in:
   1. `sudo raspi-config`
   1. (optional) run the bottom option `update` to make sure you have latest version of
   the configuration software
   1. run the second option `expand_rootfs`
   1. click `finish`
   1. select `YES` when asked for a reboot
	
1. Development system (VM, baremetal or Windows/WSL with Ubuntu 16.04.x) setup
	1. `sudo apt-get update`
	1. `sudo apt-get install build-essential autoconf autotools-dev`
	1. `sudo apt-get install git`
	1. `sudo apt-get install bc`
	
1. Install Raspberry PI development tools on development system
	1. `git clone https://github.com/raspberrypi/tools`
	1. Add `~/tools/arm-bcm2708/arm-rpi-4.9.3-linux-gnueabihf/bin/` to `PATH`

1. Install and build RPI kernel on development system
	1. `git clone https://github.com/raspberrypi/linux.git`
	1. `cd linux`
	1. `git checkout rpi-4.4.y`
	1. `export KERNEL=kernel7`
	1. `make ARCH=arm CROSS_COMPILE=~/tools/arm-bcm2708/arm-rpi-4.9.3-linux-gnueabihf/bin/arm-linux-gnueabihf- bcm2709_defconfig`
	1. `make -j 4 ARCH=arm CROSS_COMPILE=~/tools/arm-bcm2708/arm-rpi-4.9.3-linux-gnueabihf/bin/arm-linux-gnueabihf- zImage modules dtbs`
	1. `mkdir -p ~/uxmhf-rpi3-staging/mod_install`
	1. `make -j 4 ARCH=arm CROSS_COMPILE=~/tools/arm-bcm2708/arm-rpi-4.9.3-linux-gnueabihf/bin/arm-linux-gnueabihf- INSTALL_MOD_PATH=~/uxmhf-rpi3-staging/mod_install/ modules_install`
	1. `./scripts/mkknlimg arch/arm/boot/zImage ~/uxmhf-rpi3-staging/$KERNEL.img`
	1. `mkdir -p ~/uxmhf-rpi3-staging/overlays`
	1. `cp ./arch/arm/boot/dts/overlays/*.dtb* ~/uxmhf-rpi3-staging/overlays/.`
	1. `cp ./arch/arm/boot/dts/overlays/README ~/uxmhf-rpi3-staging/overlays/.`
	1. `mkdir -p ~/uxmhf-rpi3-staging/boot`
	1. `cp ./arch/arm/boot/dts/*.dtb ~/uxmhf-rpi3-staging/boot/.`

1. Build uberXMHF Raspbery PI 3 on development system
	1. `cd uxmhf-rpi3`
	1. `./bsconfigure.sh`
	1. `./configure` 
	1. `make clean`
	1. `make OSKRNLIMG=~/uxmhf-rpi3-staging/kernel7.img`
	1. `cp uxmhf-rpi3.img ~/uxmhf-rpi3-staging/.`
	1. `cp rpi3-config.txt ~/uxmhf-rpi3-staging/config.txt`

1. Note: you can run `./configure` above using any combination of the following 
   optional (experimental) parameters as needed:
    1. `./configure --enable-dmaprot` to enable DMA protection capabilities
    1. `./configure --enable-secboot --with-boot-partition-start=BP_START_SECTOR --with-boot-partition-end=BP_END_SECTOR` to enable secure boot capabilities. In this case `BP_START_SECTOR` and 
    `BP_END_SECTOR` are the values of the starting and ending sectors of the boot partition (`/dev/mmcblk0p0`) as obtained from the output of the following command: 
    `sudo fdisk -l /dev/mmcblk0`. Replace `/dev/mmcblk0` with the sdcard device on the development
    system.
    1. `./configure --enable-intprot` to enable interrupt protection capabilities
    1. `./configure --enable-fiqreflection` to enable guest FIQ interrupts to be handled within micro-hypervisor
    

<br/>
## Build uberApps

Example uberApps are found within `uxmhf-rpi3/rgapps/linux`. The following
instructions show how the example uberApp `rgapp-uhcalltest` (to test 
hypercalls) is built. 

1. Building `uhcallkmod` on development system
	1. `cd rgapps/linux/rgapp-uhcallkmod`
	1. `./build.sh ~/linux ~/tools/arm-bcm2708/gcc-linaro-arm-linux-gnueabihf-raspbian-x64/bin/`
	1. `cp ./uhcallkmod.ko ~/uxmhf-rpi3-staging/.`

1. Building `uhcalltest` on development system
	1. `cd rgapps/linux`
	1. `make -w all`
	1. `cd rgapp-uhcalltest`
	1. `make -w all`
	1. `cp ./uhcalltest ~/uxmhf-rpi3-staging/.`
 

