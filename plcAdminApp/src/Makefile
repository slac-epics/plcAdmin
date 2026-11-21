TOP=../..

DOCS += index.html
DOCS += edm-initial-setup-instructions.txt
DOCS += ioc-initial-setup-instructions.txt
DOCS += ioc-maintenance-instructions.txt
DOCS += plc-initial-setup-instructions.txt
DOCS += plc-maintenance-instructions.txt

include $(TOP)/configure/CONFIG
#----------------------------------------
#  ADD MACRO DEFINITIONS AFTER THIS LINE
#=============================

DBD += plcAdmin.dbd

LIBRARY_IOC += plcAdmin
plcAdmin_SRCS += plcAdminModel.cpp
plcAdmin_LIBS += $(EPICS_BASE_IOC_LIBS)

include $(TOP)/configure/RULES
#----------------------------------------
#  ADD RULES AFTER THIS LINE

