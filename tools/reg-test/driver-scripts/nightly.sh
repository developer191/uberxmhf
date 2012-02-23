#!/bin/bash

###
#
# Master control script for nightly build and regression
# testing. Should cope if run as user 'driver' and not as root, but
# does employ sudo to elevate privileges to copy to test /boot
# partitions.
#
# Not doing "set -e" because this is the top-level script for
# unattended testing; it needs to cope with its own errors.
#
###

## 1. Build everything

TIMESTAMP=`date --rfc-3339=seconds | tr ' ' - | cut -d - -f 1,2,3,4`
BUILD_LOG=/home/logger/public_html/build-$TIMESTAMP.log

bash build-tv.sh > $BUILD_LOG 2>&1
rc=$?
if [[ $rc != 0 ]] ; then
    echo -e "\nERROR: BUILD FAILED; ABORTING REMAINING REGRESSION TESTING!!!\n" >> $BUILD_LOG
    exit $rc
fi

## 2. Copy the hypervisor files to all the test hosts' /boot's

# TODO: Centralize all path settings
EMHF_RELPATH=../../../emhf/trunk/code
if [ ! -d "$EMHF_RELPATH" ]; then
    echo -e "\nERROR: EMHF_RELPATH $EMHF_RELPATH DOES NOT EXIST; ABORTING REMAINING REGRESSION TESTING!!!\n" >> $BUILD_LOG
fi

# TODO: Perhaps somehow stop requiring 'driver' to be a no-password
# sudoer. Ugh.
sudo -n true
rc=$?
if [[ $rc != 0 ]] ; then
    echo -e "\nERROR: CANNOT sudo FOR ./copy_to_slashboots.sh; ABORTING REMAINING REGRESSION TESTING!!!\n" >> $BUILD_LOG
    exit $rc
fi

sudo bash copy_to_slashboots.sh $EMHF_RELPATH/init-x86.bin $EMHF_RELPATH/hypervisor-x86.bin.gz >> $BUILD_LOG 2>&1
rc=$?
if [[ $rc != 0 ]] ; then
    echo -e "\nERROR: ./copy_to_slashboots.sh FAILED; ABORTING REMAINING REGRESSION TESTING!!!\n" >> $BUILD_LOG
    exit $rc
fi

## 3. Create a tests.tgz tarball
bash ../client-scripts/make_tarball.sh >> $BUILD_LOG 2>&1

# WORK TO DO HERE!
cp -av tests.tgz /home/driver/public_html >> $BUILD_LOG 2>&1

## 4. Invoke the tests on all of the test hosts
## Note: sequence-tests.sh is responsible for its own logging;
## we only grab stderr here.
echo -e "\nBEGINNING sequence-tests.sh\n" >> $BUILD_LOG
/usr/bin/time -o time.log -a ./sequence-tests.sh 2>> $BUILD_LOG
rc=$?
if [[ $rc != 0 ]] ; then
    echo -e "\nERROR: sequence-tests.sh FAILED!!!\n" >> $BUILD_LOG
    exit $rc
fi
cat time.log >> $BUILD_LOG
echo -e "\nCOMPLETED sequence-tests.sh\n" >> $BUILD_LOG
