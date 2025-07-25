inherit populate_sdk_qt5

CORE_IMAGE_BASE_INSTALL += " \
	packagegroup-rubikpi \
	rubikpi-bt-staticdev \
	rubikpi-sense-hat \
	rwreservepartition \
	ax88179bprogrammer \
	packagegroup-qt5-toolchain-target \
	rubikpi-wifi \
	rubikpi-config \
	packagegroup-qcom-test-pkgs \
	first-login \
	usb-scripts-automount \
"

IMAGE_INSTALL:append = " hostapd"
IMAGE_INSTALL:append = " i2c-tools"
IMAGE_INSTALL:append = " minicom"
IMAGE_INSTALL:append = " make cmake"
IMAGE_INSTALL:append = " iperf3 iperf2"
IMAGE_INSTALL:append = " tcpdump lmbench wget lighttpd"
IMAGE_INSTALL:append = " adduser iproute2 python3-pip sudo"
IMAGE_INSTALL:append = " rwreservepartition"
IMAGE_INSTALL:append = " ax88179bprogrammer"
IMAGE_INSTALL:append = " libcec"
IMAGE_INSTALL:append = " cec-client"
IMAGE_INSTALL:append = " python3-pyqt5 python3-pytest-qt"
IMAGE_INSTALL:append = " iotop lsof"
IMAGE_INSTALL:append = " var-rubikpi-config-mount"
IMAGE_INSTALL:append = " wiringrp wiringrp-python wiringrp-gpio"

EXTRA_USERS_PARAMS = "\
    usermod -s /bin/bash root; \
    usermod -p '\$6\$FIumPDif04\$xNtcC1aRH.k0FnCrzUH807bD6uND43RMUWPzIDnXgp0JDrC86mCVFfp1o7jH/6qCRXGPpStTcZUo4zkJkcSE31' root; \
    "

EXTRA_IMAGE_FEATURES += "tools-sdk"

# This image is sufficiently large, need to be careful that it fits in the partition.
# Nullify the overhead factor added in minimal image and explicitly add just 1GB.
IMAGE_OVERHEAD_FACTOR = "1.5"
IMAGE_ROOTFS_EXTRA_SPACE = "1048576"

EXTRA_IMAGE_FEATURES:append = " tools-testapps ptest-pkgs"

do_deploy_fixup:append() {
    # copy splash.img
    if [ -f ${DEPLOY_DIR_IMAGE}/splash.img ]; then
        install -m 0644 ${DEPLOY_DIR_IMAGE}/splash.img splash.img
    fi

    # copy rubikpi_dtso.img
    if [ -f ${DEPLOY_DIR_IMAGE}/rubikpi_config.img ]; then
        install -m 0644 ${DEPLOY_DIR_IMAGE}/rubikpi_config.img rubikpi_config.img
    fi

    # copy devcfg_full.img
    if [ -f ${DEPLOY_DIR_IMAGE}/devcfg_full.img ]; then
        install -m 0644 ${DEPLOY_DIR_IMAGE}/devcfg_full.img devcfg_full.img
    fi

    # copy rubikpi_dtso.img
    if [ -f ${DEPLOY_DIR_IMAGE}/rubikpi_dtso.img ]; then
        install -m 0644 ${DEPLOY_DIR_IMAGE}/rubikpi_dtso.img rubikpi_dtso.img
    fi

    # copy RubikPi3_CDT.bin
    if [ -f ${DEPLOY_DIR_IMAGE}/RubikPi3_CDT.bin ]; then
        install -m 0644 ${DEPLOY_DIR_IMAGE}/RubikPi3_CDT.bin RubikPi3_CDT.bin
    fi
    # copy initramfs-ostree-image-qcm6490-idp.cpio.gz
    if [ -f ${DEPLOY_DIR_IMAGE}/initramfs-ostree-image-qcm6490-idp.cpio.gz ]; then
        install -m 0644 ${DEPLOY_DIR_IMAGE}/initramfs-ostree-image-qcm6490-idp.cpio.gz initramfs-ostree-image-qcm6490-idp.cpio.gz
    fi
    # copy ukify
    # if [ -f ${DEPLOY_DIR_IMAGE}/initramfs-ostree-image-qcm6490-idp.cpio.gz ]; then
    #     install -m 0644 ${DEPLOY_DIR_IMAGE}/initramfs-ostree-image-qcm6490-idp.cpio.gz initramfs-ostree-image-qcm6490-idp.cpio.gz
    # fi
    # copy linuxaa64.efi.stub
    if [ -f ${DEPLOY_DIR_IMAGE}/linuxaa64.efi.stub ]; then
        install -m 0644 ${DEPLOY_DIR_IMAGE}/linuxaa64.efi.stub linuxaa64.efi.stub
    fi
}
