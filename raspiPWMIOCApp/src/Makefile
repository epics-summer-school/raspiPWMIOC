TOP=../..

include $(TOP)/configure/CONFIG
#----------------------------------------
#  ADD MACRO DEFINITIONS AFTER THIS LINE
#=============================
SHRLIB_SEARCH_DIRS += /usr/lib
#=============================
# Build the IOC application

PROD_IOC = raspiPWM
# raspiPWM.dbd will be created and installed
DBD += raspiPWM.dbd

# raspiPWM.dbd will be made up from these files:
raspiPWM_DBD += base.dbd
raspiPWM_DBD += drvPI4PWM.dbd
# Include dbd files from all support applications:
raspiPWM_DBD += asyn.dbd

# Add all the support libraries needed by this IOC
raspiPWM_LIBS += wiringPi

# Add additional source files
raspiPWM_SRCS += drvPI4PWM.cpp

# raspiPWM_registerRecordDeviceDriver.cpp derives from raspiPWM.dbd
raspiPWM_SRCS += raspiPWM_registerRecordDeviceDriver.cpp

# Build the main IOC entry point on workstation OSs.
raspiPWM_SRCS_DEFAULT += raspiPWMMain.cpp
raspiPWM_SRCS_vxWorks += -nil-

# Add support from base/src/vxWorks if needed
#raspiPWM_OBJS_vxWorks += $(EPICS_BASE_BIN)/vxComLibrary

# Finally link to the EPICS Base libraries
raspiPWM_LIBS += $(EPICS_BASE_IOC_LIBS)
raspiPWM_LIBS += asyn
#===========================

include $(TOP)/configure/RULES
#----------------------------------------
#  ADD RULES AFTER THIS LINE
