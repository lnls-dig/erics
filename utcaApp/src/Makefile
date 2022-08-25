TOP=../..

include $(TOP)/configure/CONFIG
#----------------------------------------
#  ADD MACRO DEFINITIONS BELOW HERE

# xxxRecord.h will be created from xxxRecord.dbd
DBDINC +=

# Install xxxSupport.dbd into <top>/dbd
DBD +=

# Build an IOC support library
LIBRARY_IOC += utcaSupport

# Compile and add the code to the support library
utcaSupport_SRCS += rtmlamp.cpp

utcaSupport_LIBS += $(EPICS_BASE_IOC_LIBS)
utcaSupport_LIBS += asyn

DECODE_REG = $(EPICS_BASE)/../decode-reg
DECODE_REG_LIBS = $(DECODE_REG)/lib/x86_64-linux-gnu

utcaSupport_LIBS += utilities modules
utilities_DIR = $(DECODE_REG_LIBS)
modules_DIR = $(DECODE_REG_LIBS)

OP_SYS_INCLUDES += -I$(DECODE_REG)/include/decode-reg
USR_CXXFLAGS += -std=c++17

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

# Link in the code from our support library
utca_LIBS += utcaSupport

# To build SNL programs, SNCSEQ must be defined
# in the <top>/configure/RELEASE file
ifneq ($(SNCSEQ),)
    # Build sncExample into utcaSupport
    sncExample_SNCFLAGS += +r
    utca_DBD += sncExample.dbd
    # A .stt sequence program is *not* pre-processed:
    utcaSupport_SRCS += sncExample.stt
    utcaSupport_LIBS += seq pv
    utca_LIBS += seq pv

    # Build sncProgram as a standalone program
    PROD_HOST += sncProgram
    sncProgram_SNCFLAGS += +m
    # A .st sequence program *is* pre-processed:
    sncProgram_SRCS += sncProgram.st
    sncProgram_LIBS += seq pv
    sncProgram_LIBS += $(EPICS_BASE_HOST_LIBS)
endif

# Finally link IOC to the EPICS Base libraries
utca_LIBS += $(EPICS_BASE_IOC_LIBS)

include $(TOP)/configure/RULES
#----------------------------------------
#  ADD EXTRA GNUMAKE RULES BELOW HERE

