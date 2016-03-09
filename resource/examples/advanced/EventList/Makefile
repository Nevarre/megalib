#-------------------------------------------------------------------------------
#
# Makefile for a stand alone application based on MEGAlib 
#
# Copyright (C) by Andreas Zoglauer.
# All rights reserved.
#
#
# Usage:
# make -f Makefile.StandAlone PRG=<filename without *.cxx>
#
# Options:
# CMD=<command line parameters>
#
# debug: call gdb
# clean: remove program and library
#
# Example:
# make -f Makefile.StandAlone PRG=Extract
#
# The Makefile needs a completely compiled MEGAlib to function properly.
# The program is automatically placed in $(MEGALIB)/bin.
#
#-------------------------------------------------------------------------------



#-------------------------------------------------------------------------------
# Program and directories

SHELL=/bin/sh
CMD=
PRG1=EventListSimCleaner
PRG2=EventListAnalyzer

# Basic directories
TOP = $(MEGALIB)
SR  = $(TOP)/src
IN  = $(TOP)/include
LB  = $(TOP)/lib
BN  = $(TOP)/bin
HT  = $(TOP)/doc/html
CT  = $(TOP)/cint

MPRG1=$(subst .cxx,,$(PRG1))
MPRG2=$(subst .cxx,,$(PRG2))

MLIBS = -L$(LB) -lMimrecGui -lMimrec -lRevanGui -lRevan -lSivanGui -lSivan -lSpectralyzeGui -lSpectralyze -lGeomegaGui -lGeomega -lCommonMisc -lCommonGui  


#-------------------------------------------------------------------------------
# Included options (result of ./configure)

include $(TOP)/config/Makefile.options
include $(TOP)/config/Makefile.config

CXXFLAGS += -I$(IN)


#-------------------------------------------------------------------------------
# Commands:


all: $(BN)/$(MPRG1) $(BN)/$(MPRG2)


only: $(BN)/$(MPRG1) $(BN)/$(MPRG2)

clean:
	rm -f $(LB)/$(MPRG1).o
	rm -f $(BN)/$(MPRG1)
	rm -f $(LB)/$(MPRG2).o
	rm -f $(BN)/$(MPRG2)


#-------------------------------------------------------------------------------
# Dependencies:

$(BN)/$(MPRG1): $(LB)/$(MPRG1).o
	@$(LD) $(LDFLAGS) $< $(MLIBS) $(GLIBS) $(LIBS) -o $@
	@echo "Program $(MPRG1) successfully linked... and starting..."


$(LB)/$(MPRG1).o: $(MPRG1).cxx
	@$(CXX) $(CXXFLAGS) -c $< -o $@
	@echo "Program $(MPRG1) successfully compiled..."

$(BN)/$(MPRG2): $(LB)/$(MPRG2).o
	@$(LD) $(LDFLAGS) $< $(MLIBS) $(GLIBS) $(LIBS) -o $@
	@echo "Program $(MPRG2) successfully linked... and starting..."


$(LB)/$(MPRG2).o: $(MPRG2).cxx
	@$(CXX) $(CXXFLAGS) -c $< -o $@
	@echo "Program $(MPRG2) successfully compiled..."

#
#-------------------------------------------------------------------------------

