#######################################
#  This makefile is written for a GNU make or equivalent.
#  For example:
#   make clean
#   make
# Copyright (c) 2010, Broadcom Corp., All Rights Reserved.
# Broadcom Bluetooth Core. Proprietary and confidential.
#######################################
# If compiling based on the root directory, path inherits from the root directory
ifeq ($(RUBIKPI_BTAPP_ROOTPATH),)
RUBIKPI_BTAPP_ROOTPATH := ../../../..
RUBIKPI_BTAPP_SRCPATH := $(RUBIKPI_BTAPP_ROOTPATH)/src
endif
ifeq ($(os_name),)
os_name := linux
endif
demo_name := fm


# include common makefile rules
include $(RUBIKPI_BTAPP_SRCPATH)/$(os_name)/common/build/common.def

# extra source list list
srcdirs :=
appsources :=

# include variable definitions for each component
include $(RUBIKPI_BTAPP_DEMO_DIR)/demo_build/def/*.def

appsources += \
    app_mgt.c \
    app_utils.c \
    app_services.c \
    app_mutex.c \
    nanoxml.c

# incdirs uses srcdirs
srcdirs := $(srcdirs) $(demo_csrc_dir)
incdirs := $(incdirs) $(srcdirs) $(demo_cinc_dir)

GLOBAL_DEFINE = -DAPP_FM_DEFAULT_RSSI_THRESH=102

# include the build rules for all the sample applications
include $(RUBIKPI_BTAPP_SRCPATH)/$(os_name)/common/build/common_make.def
