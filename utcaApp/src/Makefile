TOP=../..

include $(TOP)/configure/CONFIG
#----------------------------------------
#  ADD MACRO DEFINITIONS BELOW HERE

# xxxRecord.h will be created from xxxRecord.dbd
DBDINC += xxxRecord

# Install xxxSupport.dbd into <top>/dbd
DBD += xxxSupport.dbd

# Build an IOC support library
LIBRARY_IOC += utcaSupport

# Compile and add the code to the support library
utcaSupport_SRCS += xxxRecord.c
utcaSupport_SRCS += devXxxSoft.c

# Link locally-provided code into the support library,
# rather than directly into the IOC application.
# This is required for Windows DLL builds.
utcaSupport_SRCS += dbSubExample.c
utcaSupport_SRCS += utcaHello.c
utcaSupport_SRCS += initTrace.c

utcaSupport_LIBS += $(EPICS_BASE_IOC_LIBS)


# Build the IOC application
PROD_IOC = utca

# utca.dbd will be created and installed
DBD += utca.dbd

# utca.dbd will include these files:
utca_DBD += base.dbd
utca_DBD += xxxSupport.dbd
utca_DBD += dbSubExample.dbd
utca_DBD += utcaHello.dbd
utca_DBD += initTrace.dbd

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

