TOP=../..
include $(TOP)/configure/CONFIG
#----------------------------------------
#  ADD MACRO DEFINITIONS AFTER THIS LINE

#----------------------------------------------------
#  Optimization of db files using dbst (DEFAULT: NO)
#DB_OPT = YES

#----------------------------------------------------
# Create and install (or just install)
# databases, templates, substitutions like this
DB += plcAdminControlLogix.template
DB += plcAdminSiemens_S7plc.template
DB += s7plc_ai.template
DB += s7plc_bi.template
DB += s7plc_bo.template
DB += s7plc_longin.template

# Install templates for plc with EtherIP driver
DB += plc_analogin.template
DB += plc_analogout.template
DB += plc_digitalin.template
DB += plc_digitalout.template
DB += plc_digitalout_momentary.template

#----------------------------------------------------
# If <anyname>.db template is not named <anyname>*.template add
# <anyname>_TEMPLATE = <templatename>

include $(TOP)/configure/RULES
#----------------------------------------
#  ADD RULES AFTER THIS LINE

