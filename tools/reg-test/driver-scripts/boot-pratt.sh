#!/bin/bash

# This script contains the necessary bootloader configuration for the
# test host 'pratt' (HP EliteBook 8540p laptop with Intel CPU).  These
# variables are set before invoking an 'expect' script that can handle
# the interactive session with grub via a serial port.

# Values that are actually expected to change across test hosts:
# 1. SINIT module is chipset-dependent on Intel hosts
# 2. ISCSI_INITIATOR should be unique per test host (currently it is 
#    unique because of a suffix containing that test host's ethernet MAC 
#    address)
# 3. hostname= variable (currently ignored by test Linux environment)
# 4. The actual kernel version itself, if we want to test multiple versions

export FIRST_ROOT='root (hd0,0)'
export FIRST_KERNEL='kernel /init-x86.bin serial=115200,8n1,0x6080'
export FIRST_MOD1='module /hypervisor-x86.bin.gz'
export FIRST_MOD2='modulenounzip (hd0)+1'
export FIRST_MOD3='module /i5_i7_DUAL_SINIT_18.BIN'

export SECOND_ROOT='uuid c2a09e3b-08ba-409a-a896-bac38e8a8347'
export SECOND_KERNEL='kernel /vmlinuz-2.6.32.26+drm33.12emhf-jm1 root=UUID=e62ba4c2-87d2-4b42-b66d-dabf9af0c68c ro ip=dhcp hostname=pratt ISCSI_INITIATOR=iqn.2012-02.com.nfsec:01:88ae1dab7eb7 ISCSI_TARGET_NAME=iqn.2012-01.com.nfsec:lucid_rootfs ISCSI_TARGET_IP=10.0.0.1 ISCSI_TARGET_PORT=3260 aufs=tmpfs'
export SECOND_MOD1='initrd /initrd.img-2.6.32.26+drm33.12emhf-jm1'

# This password is sufficient to totally control this test host.  It
# might be better not to have it sitting here in the clear.
export AMT_PASSWORD='AMTp4ssw0rd!'

# In case the machine is likely to already be off
# amttool pratt powerup

./grub-generic.exp amtterm pratt
