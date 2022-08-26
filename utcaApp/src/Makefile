TOP=../..

include $(TOP)/configure/CONFIG
#----------------------------------------
#  ADD MACRO DEFINITIONS BELOW HERE

# xxxRecord.h will be created from xxxRecord.dbd
DBDINC +=

# Install xxxSupport.dbd into <top>/dbd
DBD +=

# Build an IOC support library
LIBRARY_IOC +=


utca_DBD += base.dbd

utca_LIBS += asyn
utca_DBD += asyn.dbd

utca_SRCS += pcie-single.cpp rtmlamp.cpp
utca_DBD += drivers.dbd

DECODE_REG = $(EPICS_BASE)/../decode-reg
DECODE_REG_LIBS = $(DECODE_REG)/lib/x86_64-linux-gnu

utca_LIBS += modules utilities
utilities_DIR = $(DECODE_REG_LIBS)
modules_DIR = $(DECODE_REG_LIBS)

OP_SYS_INCLUDES += -I$(DECODE_REG)/include/decode-reg
USR_CXXFLAGS += -std=c++17 -Wall -Wextra

# Build the IOC application
PROD_IOC = utca

# utca.dbd will be created and installed
DBD += utca.dbd

# utca.dbd will include these files:
utca_DBD += base.dbd

# utca_registerRecordDeviceDriver.cpp derives from utca.dbd
utca_SRCS += utca_registerRecordDeviceDriver.cpp

# Build the main IOC entry point where needed
utca_SRCS_DEFAULT += utcaMain.cpp
utca_SRCS_vxWorks += -nil-

# Finally link IOC to the EPICS Base libraries
utca_LIBS += $(EPICS_BASE_IOC_LIBS)

include $(TOP)/configure/RULES
#----------------------------------------
#  ADD EXTRA GNUMAKE RULES BELOW HERE

